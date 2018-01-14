// vim: set tabstop=2 softtabstop=2 shiftwidth=2 expandtab :
/*
 * Copyright (c) 2013-2016, Ryan V. Bissell
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-2-Clause
 * See the enclosed "LICENSE" file for exact license terms.
 */

#define CX_TRACE_SECTION "os"

#include <sstream>

#include <stdlib.h>
#include <sys/ptrace.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <linux/elf.h> // for NT_PRSTATUS and friends

#include "os/gnu_linux.hpp"
#include "cx-exceptions.hpp"

using CX::OS::PID;

using std::string;

CX_CONSTRUCTOR(CX::OS::Path__gnu_linux::Path__gnu_linux, :CX::OS::PathBase())
CX_ENDMETHOD



CX_CONSTMETHOD(struct stat CX::OS::Path__gnu_linux::_lstat,
                                                    string const &path)

  struct stat lstats;
  int result;

  errno = 0;
  result = ::lstat(path.c_str(), &lstats);
  if (result < 0)
  {
    // provide specific what-strings for the more common error cases
    switch (errno)
    {
      case EACCES:
        CX_THROW( CX::OS::Exception,
                  CX::OS::Error::FILESYSTEM,
                  "Permissions problem accessing filesystem item '%s'",
                  path.c_str() );
        break;

      default:
        CX_THROW( CX::OS::Exception,
                  CX::OS::Error::FILESYSTEM,
                  "lstat('%s') reports errno %d ('%s')",
                  path.c_str(), errno, strerror(errno));
    }
  }

  CX_RETURN(lstats);

CX_ENDMETHOD



CX_CONSTMETHOD(const char* CX::OS::Path__gnu_linux::_delims)

  CX_RETURN("/");

CX_ENDMETHOD



CX_CONSTMETHOD(bool CX::OS::Path__gnu_linux::_exists, string const &path)

  struct stat lstats;

  CX_TRY
  {
    lstats =_lstat(path);
  }
  CX_CATCH(CX::OS::Exception& e)
  {
    // this doesn't count as an error in this routine.
    if (errno == ENOENT)
      CX_RETURN(false);

    // rethrow anything else as-is.
    throw;
  }
  CX_ENDTRY

  // whatever it is (file, fifo, symlink, etc.) it exists.
  CX_RETURN(true);

CX_ENDMETHOD



CX_CONSTMETHOD(bool CX::OS::Path__gnu_linux::_isFile, string const &path)

  struct stat lstats;

  lstats = _lstat(path);
  CX_RETURN( (bool)(S_ISREG(lstats.st_mode) != 0) );

CX_ENDMETHOD



CX_CONSTMETHOD(bool CX::OS::Path__gnu_linux::_isSymlink,
                                                    string const &path)

  struct stat lstats;

  lstats = _lstat(path);
  CX_RETURN( (bool)(S_ISLNK(lstats.st_mode) != 0) );

CX_ENDMETHOD



CX_CONSTMETHOD(std::string CX::OS::Path__gnu_linux::_readlink,
                                                    string const &path)

  char buf[4096];
  size_t size;

  // first we confirm that 'path' is a symlink
  if (!IsSymlink(path))
  {
    CX_THROW( CX::OS::Exception,
              CX::OS::Error::FILESYSTEM,
              "Given path '%s' is not a symlink.",
              path.c_str());
  }

  size = ::readlink(path.c_str(), buf, sizeof(buf));
  if (size < 0)
  {
    CX_THROW( CX::OS::Exception,
              CX::OS::Error::FILESYSTEM,
              "readlink('%s') reports errno %d ('%s')",
              path.c_str(), errno, strerror(errno));
  }

  CX_RETURN(std::string(buf));

CX_ENDMETHOD


// ====================================================================


CX_CONSTRUCTOR(CX::OS::File__gnu_linux::File__gnu_linux, :CX::OS::FileBase())
CX_ENDMETHOD



CX_DESTRUCTOR(CX::OS::File__gnu_linux::~File__gnu_linux)
CX_ENDMETHOD



CX_CONSTMETHOD(U8* CX::OS::File__gnu_linux::_openMapped,
                                                string const &name,
                                                int *fd, size_t *psize)

  int result;
  struct stat fstats;
  U8 *fbuffer = nullptr;

  *fd = ::open(name.c_str(), O_RDONLY);
  if (*fd <= -1)
  {
    CX_THROW( CX::OS::Exception,
              CX::OS::Error::FILESYSTEM,
              "unable to open file '%s'", name.c_str() );
  }

  result = ::fstat(*fd, &fstats);
  if (result <= -1)
  {
    ::close(*fd);
    CX_THROW( CX::OS::Exception,
              CX::OS::Error::FILESYSTEM,
              "unable to fstat file '%s'", name.c_str() );
  }

  fbuffer = (U8*)::mmap(/*addr*/NULL, fstats.st_size,
                        PROT_READ, MAP_SHARED, *fd, /*offset*/0);
  if (fbuffer == MAP_FAILED)
  {
    ::close(*fd);
    CX_THROW( CX::OS::Exception,
              CX::OS::Error::FILESYSTEM,
              "unable to mmap file '%s'", name.c_str() );
  }

  *psize = fstats.st_size;
  CX_RETURN(fbuffer);

CX_ENDMETHOD



CX_CONSTMETHOD(void CX::OS::File__gnu_linux::_unmapAndClose, int fd,
                                                  U8* buffer,
                                                  size_t size)
  ::munmap(buffer, size);
  close(fd);

CX_ENDMETHOD


// ====================================================================


CX_CONSTRUCTOR(CX::OS::Process__gnu_linux::Process__gnu_linux,
                                                :CX::OS::ProcessBase())
CX_ENDMETHOD



CX_DESTRUCTOR(CX::OS::Process__gnu_linux::~Process__gnu_linux)
CX_ENDMETHOD




CX_CONSTMETHOD(PID CX::OS::Process__gnu_linux::_spawn,
                                                string const &execspec,
                                                ArgsVector const &args,
                                                bool traced)
  long error;
  pid_t pid;

  // checking that 'execspec' exists (before forking) will make for
  // better error reporting.

  if (!CX::OS::Path->Exists(execspec))
  {
    CX_THROW( CX::OS::Exception,
              CX::OS::Error::FILESYSTEM,
              "The file '%s' does not exist.",
              execspec.c_str() );
  }

  if (!CX::OS::Path->IsFileOrSymlinkToFile(execspec))
  {
    CX_THROW( CX::OS::Exception,
              CX::OS::Error::FILESYSTEM,
              "The filesystem item '%s' is not a "
              "file or a symlink to a file.",
              execspec.c_str() );
  }

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

    char** argv = CreateExecArgsList(execspec.c_str(), &args);
    // TODO: consider use of 'execve' here
    error = ::execv(execspec.c_str(), argv);
    DestroyExecArgsList(argv);
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
      CX_RETURN( (PID)pid );

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
        CX_RETURN( (PID)pid );
        break;
    }
  }

CX_ENDMETHOD



CX_CONSTMETHOD(string CX::OS::Process__gnu_linux::_getExecSpec, PID pid)

  std::string result;
  std::stringstream proc_pid_exe;

  proc_pid_exe << "/proc/" << pid << "/exe";

  result = CX::OS::Path->Readlink(proc_pid_exe.str());
  CX_RETURN(result);

CX_ENDMETHOD



CX_CONSTMETHOD(void CX::OS::Process__gnu_linux::_getRegSet, PID pid,
                                                          size_t* size,
                                                          void* buf)
  long ret;
  iovec regvec;

  regvec.iov_len = *size;
  regvec.iov_base = buf;
  ret = ::ptrace(PTRACE_GETREGSET, pid, NT_PRSTATUS, &regvec);
  if (ret < 0)
  {
    CX_ASSERT(false);  // TODO, need exception here
  }
  if (regvec.iov_len != *size)
  {
    CX_ASSERT(false); // TODO, warning or exception?
  }

CX_ENDMETHOD

