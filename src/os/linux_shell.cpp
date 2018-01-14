// vim: set tabstop=2 softtabstop=2 shiftwidth=2 expandtab :
/*
 * Copyright (c) 2013-2017, Ryan V. Bissell
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-2-Clause
 * See the enclosed "LICENSE" file for exact license terms.
 */

#define CX_TRACE_SECTION "os"

#include "cx-os.hpp"
#include "cx-exceptions.hpp"

#include <cstdlib>
#include <sys/ptrace.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <unistd.h>
#include <pwd.h>
#include <fcntl.h>
#include <sys/mman.h>

using std::string;

namespace CX {
namespace OS {


CX_CONSTRUCTOR(Shell_Linux::Shell_Linux, :ShellBase<Shell_Linux>())
CX_ENDMETHOD



CX_FUNCTION(static struct stat _lstat, Path const& path)

  struct stat lstats;
  int result;

  errno = 0;
  string pathstr = path.str();
  result = ::lstat(pathstr.c_str(), &lstats);
  if (result < 0)
  {
    // provide specific what-strings for the more common error cases
    switch (errno)
    {
      case EACCES:
        CX_THROW( Exception,
            Error::FILESYSTEM,
            "Permissions problem accessing filesystem item '%s'",
            pathstr.c_str() );
        break;

      default:
        CX_THROW( Exception,
            Error::FILESYSTEM,
            "lstat('%s') reports errno %d ('%s')",
            pathstr.c_str(), errno, strerror(errno));
    }
  }

  CX_RETURN(lstats);

CX_ENDMETHOD



CX_FUNCTION(static bool _lstat, Path const& path, struct stat* lstats)

  CX_TRY
  {
    *lstats = _lstat(path);
  }
  CX_CATCH(Exception const& e)
  {
    // this doesn't count as an error in this routine.
    if (errno == ENOENT)
      CX_RETURN(false);

    // but everything else does
    throw;
  }
  CX_ENDTRY

  CX_RETURN(true);

CX_ENDMETHOD



CX_CONSTMETHOD(Path Shell_Linux::_home)

  char const* dir = getenv("HOME");

  if (!dir)
    dir = getpwuid(getuid())->pw_dir;

  if (!dir)
    dir = "";

  CX_RETURN(Path(dir));

CX_ENDMETHOD



CX_CONSTMETHOD(Path Shell_Linux::_getCwd)

  errno = 0;
  char* dir = getcwd(nullptr, 0);
  if (!dir)
    CX_THROW( Exception,
              Error::FILESYSTEM,
              "getcwd() reports errno %d ('%s')",
              errno, strerror(errno) );

  Path cwd(dir);
  free(dir);
  CX_RETURN(cwd);

CX_ENDMETHOD



CX_CONSTMETHOD(void Shell_Linux::_setCwd, Path const& p)

  errno = 0;
  int error = chdir(p.str().c_str());
  if (error)
  {
    CX_THROW( Exception,
              Error::FILESYSTEM,
              "Error changing to directory '%s': %s",
              p.str().c_str(), strerror(errno) );
  }

CX_ENDMETHOD



CX_CONSTMETHOD(Path Shell_Linux::_getTemp)

  char const* temp = nullptr;

  // this order is per ISO/IEC 9945
      (temp = ::getenv("TMPDIR"))
  ||  (temp = ::getenv("TMP"))
  ||  (temp = ::getenv("TEMP"))
  ||  (temp = ::getenv("TEMPDIR"))
  ||  (temp = "/tmp");

  Path path(temp);

  if (!PathExists(path))
    CX_THROW( Exception,
              Error::FILESYSTEM,
              "Temp directory '%s' does not exist",
              temp );

  if (!IsDirectory(path))
    CX_THROW( Exception,
              Error::FILESYSTEM,
              "'%s' is not a directory",
              temp );

  CX_RETURN(path);

CX_ENDMETHOD



CX_CONSTMETHOD(bool Shell_Linux::_pathExists, Path const& p)

  struct stat lstats;

  bool exists = _lstat(p, &lstats);
  CX_RETURN(exists);

CX_ENDMETHOD



CX_CONSTMETHOD(bool Shell_Linux::_isFile, Path const& p)

  struct stat lstats;

  lstats = _lstat(p);
  CX_RETURN( (bool)(S_ISREG(lstats.st_mode) != 0) );

CX_ENDMETHOD



CX_CONSTMETHOD(bool Shell_Linux::_isOther, Path const& p)

  struct stat lstats;

  lstats = _lstat(p);
  if (   S_ISREG(lstats.st_mode)
      || S_ISDIR(lstats.st_mode)
      || S_ISLNK(lstats.st_mode) )
    CX_RETURN(false);

  CX_RETURN(true);

CX_ENDMETHOD



CX_CONSTMETHOD(bool Shell_Linux::_isSymlink, Path const& p)

  struct stat lstats;

  lstats = _lstat(p);
  CX_RETURN( (bool)(S_ISLNK(lstats.st_mode) != 0) );

CX_ENDMETHOD



CX_CONSTMETHOD(bool Shell_Linux::_isDirectory, Path const& p)

  struct stat lstats;

  lstats = _lstat(p);
  CX_RETURN( (bool)(S_ISDIR(lstats.st_mode) != 0) );

CX_ENDMETHOD



static
CX_FUNCTION(bool _in_groups, gid_t find, int ngroups, gid_t* groups)

  while (ngroups--)
    if (find == groups[ngroups])
      return true;

  return false;

CX_ENDMETHOD



static
CX_FUNCTION(bool _user_has_permission,  Path const& p, mode_t uperm,
                                        mode_t gperm,  mode_t operm)

  struct stat lstats = _lstat(p.str());

  uid_t euid = geteuid();
  uid_t egid = getegid();

  int ngroups = getgroups(0, NULL);
  gid_t* gidlist = (gid_t*)malloc((1+ngroups) * sizeof(gid_t));
  ngroups = getgroups(ngroups, gidlist);
  gidlist[ngroups] = egid; // per the 'getgroups(2)' manpage
  bool user_in_group = _in_groups(lstats.st_gid, 1+ngroups, gidlist);
  free(gidlist);

  if (euid == lstats.st_uid)
  {
    if (lstats.st_mode & uperm)
      return true;

    // if the user is the owner of this file, but they are explictly
    // disallowed from accessing it via those owner permissions, then
    // the file is considered not accessible for the owner.  There are
    // no "second chances" via 'group' or 'other' permissions.
    return false;
  }

  if (user_in_group)
  {
    if (lstats.st_mode & gperm)
      return true;

    // if the user is a member of the 'group' of this file, and they
    // are explictly disallowed from accessing it via those 'group'
    // permissions, then the file is considered not accessible for that
    // user.  There are no "second chances" via 'other' permissions.
    return false;
  }

  // last resort: 'other' permissions
  return (bool)!!(lstats.st_mode & operm);

CX_ENDFUNCTION



CX_CONSTMETHOD(bool Shell_Linux::_userCanRead, Path const& p)

  CX_RETURN(_user_has_permission(p, S_IRUSR, S_IRGRP, S_IROTH));

CX_ENDMETHOD



CX_CONSTMETHOD(bool Shell_Linux::_userCanWrite, Path const& p)

  CX_RETURN(_user_has_permission(p, S_IWUSR, S_IWGRP, S_IWOTH));

CX_ENDMETHOD



CX_CONSTMETHOD(bool Shell_Linux::_userCanExecute, Path const& p)

  CX_RETURN(_user_has_permission(p, S_IXUSR, S_IXGRP, S_IXOTH));

CX_ENDMETHOD



CX_CONSTMETHOD(bool Shell_Linux::_userCanTraverse, Path const& p)

  CX_RETURN(_userCanExecute(p));

CX_ENDMETHOD



CX_CONSTMETHOD(Path Shell_Linux::_systemComplete, Path const& p)

  if (p.IsEmpty())
    CX_RETURN(p);

  if (p.IsAbsolute())
    CX_RETURN(p);

  Path complete = GetCwd() / p;
  CX_RETURN(complete);

CX_ENDMETHOD



CX_CONSTMETHOD(Path Shell_Linux::_resolveSymlink, Path const& p)

  if (!_isSymlink(p))
  {
    CX_THROW( Exception,
              Error::FILESYSTEM,
              "Attempted to resolve '%s' as a symbolic link, "
              "which it is not",
              p.str().c_str() );
  }

  ssize_t r;
  struct stat lstats;

  lstats = _lstat(p);
  // presumably, for a symlink, 'lstats.st_size' will
  // now contain the length of the path that the symlink
  // resolves to.  But, I've seen on Linux where 'st_size'
  // is reported back as 0.

  string pstr = p.str();
  size_t size = lstats.st_size;
  char* buf = nullptr;
  do
  {
    size += 32;
    errno = 0;
    buf = (char*)::realloc(buf, size+1);
    r = ::readlink(pstr.c_str(), buf, size);
    if (r < 0)
    {
      free(buf);
      CX_THROW( Exception,
                Error::FILESYSTEM,
                "readlink('%s') reports errno %d ('%s')",
                p.str().c_str(), errno, strerror(errno) );
    }
  } while (r == size); // means 'buf not big enough'

  buf[r] = '\0';
  Path resolved(buf);
  free(buf);

  CX_RETURN(resolved);

CX_ENDMETHOD



CX_CONSTMETHOD(void Shell_Linux::_createSymlink,  Path const& from,
                                                  Path const& to)

  int result;

  errno = 0;
  result = ::symlink(from.str().c_str(), to.str().c_str());
  if (result != 0)
    CX_THROW( Exception,
              Error::FILESYSTEM,
              "symlink('%s', '%s') reports errno %d ('%s')",
              from.str().c_str(), to.str().c_str(),
              errno, strerror(errno) );

CX_ENDMETHOD



CX_CONSTMETHOD(void Shell_Linux::_createHardlink, Path const& from,
                                                  Path const& to)

  // TODO

CX_ENDMETHOD



CX_CONSTMETHOD(void Shell_Linux::_createDirectory, Path const& p)

  int result;

  errno = 0;
  result = ::mkdir(p.str().c_str(), S_IRWXU | S_IRWXG | S_IRWXO);
  if (result != 0)
  {
    Error what;
    switch (errno)
    {
      case EEXIST:  what = Error::E_EXIST;    break;
      default:      what = Error::FILESYSTEM; break;
    }
    CX_THROW( Exception,
              what,
              "mkdir('%s') reports errno %d ('%s')",
              p.str().c_str(), errno, strerror(errno) );
  }

CX_ENDMETHOD



CX_CONSTMETHOD(void Shell_Linux::_copy, Path const& from,
                                        Path const& to)

  // TODO

CX_ENDMETHOD



CX_CONSTMETHOD(void Shell_Linux::_delete, Path const& p,
                                          bool nothrow)

  // TODO

CX_ENDMETHOD



CX_CONSTMETHOD(void Shell_Linux::_deleteTree, Path const& p)

  // TODO

CX_ENDMETHOD



CX_CONSTMETHOD(uintmax_t Shell_Linux::_fileSize, Path const& p)

  // TODO
  CX_RETURN(0);

CX_ENDMETHOD



CX_CONSTMETHOD(void Shell_Linux::_resizeFile, Path const& p,
                                              uintmax_t size)

  // TODO

CX_ENDMETHOD



CX_CONSTMETHOD(bool Shell_Linux::_pathsEquivalent,  Path const& p1,
                                                    Path const& p2)

  // TODO
  CX_RETURN(false);

CX_ENDMETHOD



CX_CONSTMETHOD(U8* Shell_Linux::_openMapped,  Path const& name,
                                              int *fd,
                                              size_t *psize)

  int result;
  struct stat fstats;
  U8 *fbuffer = nullptr;

  *fd = ::open(name.str().c_str(), O_RDONLY);
  if (*fd <= -1)
  {
    CX_THROW( Exception,
              Error::FILESYSTEM,
              "unable to open file '%s'", name.str().c_str() );
  }

  result = ::fstat(*fd, &fstats);
  if (result <= -1)
  {
    ::close(*fd);
    CX_THROW( Exception,
              Error::FILESYSTEM,
              "unable to fstat file '%s'", name.str().c_str() );
  }

  fbuffer = (U8*)::mmap(/*addr*/NULL, fstats.st_size,
                        PROT_READ, MAP_SHARED, *fd, /*offset*/0);
  if (fbuffer == MAP_FAILED)
  {
    ::close(*fd);
    CX_THROW( Exception,
              Error::FILESYSTEM,
              "unable to mmap file '%s'", name.str().c_str() );
  }

  *psize = fstats.st_size;
  CX_RETURN(fbuffer);

CX_ENDMETHOD



CX_CONSTMETHOD(void Shell_Linux::_unmapAndClose, int fd,  U8 *buffer,
                                                          size_t size)

  ::munmap(buffer, size);
  close(fd);

CX_ENDMETHOD


CX_CONSTMETHOD(Process* Shell_Linux::_spawn,  char const** argv,
                                              bool traced)

  long error;
  pid_t pid;

  pid = ::vfork();
  if (pid == 0)
  {
    // this is the nascent child process

    // following ptrace tutorial at
    // http://mikecvet.wordpress.com/2010/08/14/ptrace-tutorial/

    if (traced)
    {
      // before exec'ing, ask the parent to trace.
      error = ::ptrace(PTRACE_TRACEME, 0, nullptr, nullptr);
      if (error)
      {
        // can't throw an exception here, because we've already forked.
        ::perror(CX_STRCAT(__func__, ": vfork()").c_str());
        ::exit(EXIT_FAILURE);
      }
    }

    // N.B. It seems like we might need to do something special
    // to support inferiors which themselves fork child processes
    // of their own.  See GDB's "follow-fork-mode" and "detach-on-fork".
    // See also http://sourceware.org/gdb/onlinedocs/gdb/Forks.html

    // TODO: consider use of 'execve' here
    error = ::execv(argv[0], const_cast<char* const*>(argv));
    if (error)
    {
      // can't throw an exception here, because we've already forked.
      ::perror(CX_STRCAT(__func__, ": execv()").c_str());
      ::exit(EXIT_FAILURE);
    }
  }
  else if (pid < 0)
  {
    // we failed to fork, so this is still the parent process, and it
    // can throw an exception.
    CX_THROW( CX::OS::Exception,
              CX::OS::Error::FORK,
              "Unable to fork inferior process; vfork() reports "
              "errno='%s'.", strerror(errno));
  }
  else
  {
    int status;
    pid_t wpid;
    char errbuf[1024];

    // this is the parent process

    if (!traced)
      CX_RETURN( new Process((CXPID)pid) );

    // following ptrace tutorial at
    //http://mikecvet.wordpress.com/2010/08/14/ptrace-tutorial/

    // The man page for ptrace(2) says:
    // "The  parent  can  initiate  a  trace  by  calling  fork(2) and
    // having the  resulting  child do a PTRACE_TRACEME, followed
    // (typically) by an exec(3).  Alternatively, the parent may
    // commence trace of an existing process using PTRACE_ATTACH."
    //
    // I think the former sounds superior, for our needs.

    // TODO, see manpage for wait() if this does not work ("options")
    wpid = ::waitpid(pid, &status, 0);
    switch (wpid)
    {
      case -1:
        snprintf(errbuf, 1024,
                 "waitpid() reports errno='%s'", ::strerror(error));
        CX_THROW( CX::OS::Exception,
                  CX::OS::Error::WAITPID,
                  errbuf );
        break;

      case 0:
        // TODO: should only happen if WNOHANG was used (it wasn't)
        // IIRC, we could use WNOHANG as a way to timeout if inferior
        // takes longer stop than expected.
        break;

      default:
        CX_ASSERT(wpid == pid);
        CX_ASSERT(WIFSTOPPED(status));  // fork good, but execv failed?

        CX_DEBUGOUT("Spawned inferor pid=%d has stopped due to %s\n",
                    wpid, ::strsignal(WSTOPSIG(status)));
        // we now have control of the (stopped) child process, and
        // can set breakpoints, continue execution, etc.
        CX_RETURN( new Process((CXPID)pid) );
        break;
    }
  }

CX_ENDMETHOD


} // namespace OS
} // namespace CX

