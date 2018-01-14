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

using std::string;

namespace CX {
namespace OS {


extern Shell g_shell;


template <class T>
Path ShellBase<T>::_initialPath;


template <class T>
CX_CONSTRUCTOR0(ShellBase<T>::ShellBase)

  srand(time(NULL));
  _initialPath = GetCwd();

CX_ENDMETHOD



template <class T>
CX_METHOD(Path ShellBase<T>::Home)

  CX_RETURN(g_shell._home());

CX_ENDMETHOD



template <class T>
CX_METHOD(Path ShellBase<T>::GetCwd)

  CX_RETURN(g_shell._getCwd());

CX_ENDMETHOD



template <class T>
CX_METHOD(void ShellBase<T>::SetCwd, Path const& p)

  g_shell._setCwd(p);

CX_ENDMETHOD



template <class T>
CX_METHOD(Path ShellBase<T>::GetInitialPath)

  CX_RETURN(ShellBase<T>::_initialPath);

CX_ENDMETHOD



template <class T>
CX_METHOD(Path ShellBase<T>::GetTempDirectory)

  CX_RETURN(g_shell._getTemp());

CX_ENDMETHOD



template <class T>
CX_METHOD(Path ShellBase<T>::GetUniquePath, Path const& model)

  std::string temp = model.str();

  for (char& c : temp)
  {
    if (c == '%')
    {
      int r = std::rand() % 16;
      if (r < 10)
        c = '0' + r;
      else
        c = 'A' + r-10;
    }
  }

  CX_RETURN(Path(temp));

CX_ENDMETHOD



template <class T>
CX_METHOD(bool ShellBase<T>::PathExists, Path const& p)

  CX_RETURN(g_shell._pathExists(p));

CX_ENDMETHOD



template <class T>
CX_METHOD(bool ShellBase<T>::IsFile, Path const& p)

  CX_RETURN(g_shell._isFile(p));

CX_ENDMETHOD



template <class T>
CX_METHOD(bool ShellBase<T>::IsOther, Path const& p)

  CX_RETURN(g_shell._isOther(p));

CX_ENDMETHOD



template <class T>
CX_METHOD(bool ShellBase<T>::IsSymlink, Path const& p)

  CX_RETURN(g_shell._isSymlink(p));

CX_ENDMETHOD



template <class T>
CX_METHOD(bool ShellBase<T>::IsDirectory, Path const& p)

  CX_RETURN(g_shell._isDirectory(p));

CX_ENDMETHOD



template <class T>
CX_METHOD(bool ShellBase<T>::IsFileOrSymlinkToFile, Path const& p)

  if (IsFile(p))
    CX_RETURN(true);

  if (!IsSymlink(p))
    CX_RETURN(false);

  Path resolved = ResolveSymlink(p);
  while (IsSymlink(resolved))
    resolved = ResolveSymlink(resolved);

  CX_RETURN(IsFile(resolved));

CX_ENDMETHOD



template <class T>
CX_METHOD(bool ShellBase<T>::UserCanRead, Path const& p)

  CX_RETURN(g_shell._userCanRead(p));

CX_ENDMETHOD



template <class T>
CX_METHOD(bool ShellBase<T>::UserCanWrite, Path const& p)

  CX_RETURN(g_shell._userCanWrite(p));

CX_ENDMETHOD



template <class T>
CX_METHOD(bool ShellBase<T>::UserCanExecute, Path const& p)

  // within CX::OS, directories are not executable; only files can be.
  // (the executable permission for directories on Linux filesystems
  // is referred to as 'traversable' in CX::OS)
  CX_RETURN(!IsDirectory(p) && g_shell._userCanExecute(p));

CX_ENDMETHOD



template <class T>
CX_METHOD(bool ShellBase<T>::UserCanTraverse, Path const& p)

  // only directories are traversable
  CX_RETURN(IsDirectory(p) && g_shell._userCanTraverse(p));

CX_ENDMETHOD



template <class T>
CX_METHOD(Path ShellBase<T>::Relative, Path const& p, Path const& base)

  Path relative = WeaklyCanonical(base);
  relative = WeaklyCanonical(p).LexicallyRelative(relative);
  CX_RETURN(relative);

CX_ENDMETHOD



template <class T>
CX_METHOD(Path ShellBase<T>::Absolute, Path const& p, Path const& base)

  Path absolute = p.LexicallyAbsolute(base);
  CX_RETURN(absolute);

CX_ENDMETHOD



template <class T>
CX_METHOD(Path ShellBase<T>::Canonical, Path const& p, Path const& base)

  // converts 'p', which must exist, to an absolute path that has
  // no symbolic link, dot, or dot-dot elements.

  Path abase = p.LexicallyAbsolute(base);

  // XXX why must it be a directory?
  if (!PathExists(abase) || !IsDirectory(abase))
    CX_THROW( Exception,
              Error::E_NOENT,
              "Argument 'base' to Canonical() must name a directory "
              "and must exist on a mounted filesystem." );


  Path root = abase.RootPath();
  Path canonical;

  bool restart;
  do
  {
    restart = false;
    for (Path::iterator iter = abase.begin(); iter != abase.end(); ++iter)
    {
      if (*iter == Path::dot())
        continue;

      if (*iter == Path::dotdot())
      {
        if (canonical != root)
          canonical.RemoveFilename();
        continue;
      }

      canonical /= *iter;

      if (IsSymlink(canonical))
      {
        Path link = ResolveSymlink(canonical);
        canonical.RemoveFilename();
        if (link.IsAbsolute())
        {
          for (++iter; iter != abase.end(); ++iter)
            link /= *iter;
          abase = link;
        }
        else
        {
          Path new_abase(canonical);
          new_abase /= link;
          for (++iter; iter != abase.end(); ++iter)
            new_abase /= *iter;
          abase = new_abase;
        }
        canonical.Clear();
        restart = true;
        break;
      }
    }
  } while (restart);

  CX_RETURN(canonical);

CX_ENDMETHOD



template <class T>
CX_METHOD(Path ShellBase<T>::WeaklyCanonical,  Path const& p)

  Path head(p);
  Path tail;
  Path::const_iterator iter = p.cend();

  // find the longest 'head' that exists
  for (; !head.IsEmpty(); --iter)
  {
    if (PathExists(head))
      break;
    head.RemoveFilename();
  }

  // build up the tail, and note if it contains any . or .. elements
  bool taildots = false;
  for (; iter != p.cend(); ++iter)
  {
    tail /= *iter;
    if ((*iter == Path::dot()) || (*iter == Path::dotdot()))
      taildots = true;
  }

  if (head.IsEmpty())
    CX_RETURN(p.LexicallyNormal());

  head = Canonical(head);

  if (tail.IsEmpty())
    CX_RETURN(head);

  if (taildots)
    CX_RETURN((head / tail).LexicallyNormal());

  CX_RETURN(head / tail);

CX_ENDMETHOD



template <class T>
CX_METHOD(Path ShellBase<T>::SystemComplete, Path const& p)

  CX_RETURN(g_shell._systemComplete(p));

CX_ENDMETHOD



template <class T>
CX_METHOD(Path ShellBase<T>::ResolveSymlink, Path const& p)

  CX_RETURN(g_shell._resolveSymlink(p));

CX_ENDMETHOD



template <class T>
CX_METHOD(void ShellBase<T>::CreateSymlink, Path const& from,
                                            Path const& to)

  g_shell._createSymlink(from, to);

CX_ENDMETHOD



template <class T>
CX_METHOD(void ShellBase<T>::CreateHardlink,  Path const& from,
                                              Path const& to)

  g_shell._createHardlink(from, to);

CX_ENDMETHOD



template <class T>
CX_METHOD(void ShellBase<T>::CreateDirectory,  Path const& p)

  if (!p.IsValid())
    CX_THROW( Exception,
              Error::BADPATH,
              "Cannot create a directory named '%s'",
              p.str().c_str() );

  CX_TRY
  {
    g_shell._createDirectory(p);
  }
  CX_CATCH(CX::OS::Exception const& e)
  {
    if (e.What() != Error::E_EXIST)
      throw;

    if (!Shell::IsDirectory(p))
      CX_THROW( Exception,
                Error::E_NOTDIR,
                "The filesystem object named '%s' already exists, but "
                "is not a directory.", p.str().c_str() );
  }
  CX_ENDTRY

CX_ENDMETHOD



template <class T>
CX_METHOD(void ShellBase<T>::CreateDirectories,  Path const& p)

  Path norm = p.LexicallyNormal();

  Path current("");
  bool frontier = false;
  bool past_dots = false;
  for (Path::iterator iter = norm.begin(); iter != norm.end(); ++iter)
  {
    current /= *iter;

    // because we normalized, this should only ever happen at the start
    if ((*iter == Path::dot()) || (*iter == Path::dotdot()))
    {
      CX_ASSERT(!past_dots);
      continue;
    }
    past_dots = true;

    if (!frontier && Shell::PathExists(current))
    {
      if (Shell::IsDirectory(current))
        continue;

      // here we know that we need to throw E_NOTDIR, but we're
      // going to be lazy and let CreateDirectory() do that for us.
    }

    CreateDirectory(current);
    frontier = true;  // the point at which things don't exist yet
  }

CX_ENDMETHOD



template <class T>
CX_METHOD(void ShellBase<T>::Copy,  Path const& from,
                                    Path const& to)

  g_shell._copy(from, to);

CX_ENDMETHOD



template <class T>
CX_METHOD(void ShellBase<T>::Delete,  Path const& p,
                                      bool nothrow)

  g_shell._delete(p, nothrow);

CX_ENDMETHOD



template <class T>
CX_METHOD(void ShellBase<T>::DeleteTree,  Path const& p)

  g_shell._deleteTree(p);

CX_ENDMETHOD



template <class T>
CX_METHOD(uintmax_t ShellBase<T>::FileSize, Path const& p)

  CX_RETURN(g_shell._fileSize(p));

CX_ENDMETHOD



template <class T>
CX_METHOD(void ShellBase<T>::ResizeFile, Path const& p,
                                              uintmax_t size)

  g_shell._resizeFile(p, size);

CX_ENDMETHOD



template <class T>
CX_METHOD(bool CX::OS::ShellBase<T>::PathsEquivalent, Path const& p1,
                                                      Path const& p2)

  CX_RETURN(g_shell._pathsEquivalent(p1, p2));

CX_ENDMETHOD



template <class T>
CX_METHOD(U8* ShellBase<T>::OpenMapped, Path const& name,
                                        int *fd, size_t *psize)

  CX_RETURN(g_shell._openMapped(name, fd, psize));

CX_ENDMETHOD



template <class T>
CX_METHOD(void ShellBase<T>::UnmapAndClose, int fd,  U8 *buffer,
                                            size_t size)

  g_shell._unmapAndClose(fd, buffer, size);

CX_ENDMETHOD



template <class T>
CX_METHOD(Process* ShellBase<T>::Spawn, Path const& execspec,
                                        ArgsVector const& args,
                                        bool traced)

  // checking (before forking) that 'execspec' exists, is a file, and
  // can be executed will make for better error reporting.

  if (!Shell::PathExists(execspec))
  {
    CX_THROW( CX::OS::Exception,
              CX::OS::Error::FILESYSTEM,
              "The file '%s' does not exist.",
              execspec.str().c_str() );
  }

  if (!Shell::IsFileOrSymlinkToFile(execspec))
  {
    CX_THROW( CX::OS::Exception,
              CX::OS::Error::FILESYSTEM,
              "The filesystem item '%s' is not a "
              "file or a symlink to a file.",
              execspec.str().c_str() );
  }

  if (!Shell::UserCanExecute(execspec))
  {
    CX_THROW( CX::OS::Exception,
              CX::OS::Error::FILESYSTEM,
              "No permissions to run '%s' as this user.",
              execspec.str().c_str() );
  }

  std::vector<char const*> argv;
  argv.push_back(execspec.str().c_str());
  for (auto& string : args)
    argv.push_back(string.data());
  argv.push_back(nullptr);

  Process* process = g_shell._spawn(argv.data(), traced);
  CX_RETURN(process);

CX_ENDMETHOD


// ===================================================================


template class ShellBase<Shell>;

// the one and only instantiation of CX::OS::Shell
Shell g_shell;


} // namespace 'OS'
} // namespace 'CX'
