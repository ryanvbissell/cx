// vim: set tabstop=2 softtabstop=2 shiftwidth=2 expandtab :
/*
 * Copyright (c) 2013-2017, Ryan V. Bissell
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-2-Clause
 * See the enclosed "LICENSE" file for exact license terms.
 */

#ifndef CX_OS_HPP
#define CX_OS_HPP

#include <string>
#include <vector>

#include "cx-types.hpp"
#include "cx-exceptions.hpp"

#ifndef CX_OPSYS
#error CX_OPSYS must be defined.
#endif

#if 1

#define CX_MAKE_OPSYS_HEADER2(os, section) CX_STRINGIZE(os##_##section)
#define CX_MAKE_OPSYS_HEADER1(os, file) CX_MAKE_OPSYS_HEADER2(os, file)
#define CX_OPSYS_HEADER(file) CX_MAKE_OPSYS_HEADER1(os/CX_OPSYS, file)

#else

#define IDENT(x) x
#define _IDENT(x) _##x
#define XSTR(x) #x
#define STR(x) XSTR(x)
#define PATH(x,y,z) STR(IDENT(x)IDENT(y)_IDENT(z))

#define Dir os/
#define File CX_OPSYS

// use above as:  #include PATH(Dir,CX_OPSYS,xpath.hpp)

#endif

namespace CX {
namespace OS {

  #define CX_OS_EXCEPTIONS                                             \
    X(FORK,       "Some call to fork() or similar/equivalent failed.") \
    X(WAITPID,    "Some call to waitpid() or similar failed.")         \
    X(FILESYSTEM, "Some filesystem operation failed.")                 \
    X(PERMISSION, "Access denied.")                                    \
    X(BADPATH,    "Invalid or nonconformant filesystem path.")         \
    X(ERRNO,      "Errno set by system runtime.")                      \
    X(E_NOENT,    "Filesystem object does not exist.")                 \
    X(E_EXIST,    "Filesystem object already exists.")                 \
    X(E_NOTDIR,   "Filesystem object is not a directory.")

  #define X(v,str)  v,
  enum class Error: U32
  {
    CX_OS_EXCEPTIONS
  };
  extern std::map<Error, char const*> ErrorNameMap;
  extern std::map<Error, char const*> ErrorStringMap;
  #undef X

  CX_DECLARE_EXCEPTION_CLASS(Exception, Error, CX::Exception);

// ===================================================================

  template <class T>
  class PathBase
  {
  public:
    PathBase();
    PathBase(std::string const& p);

    static const size_t npos = -1;

    typedef std::vector<std::string> pathvec;
    typedef pathvec::iterator iterator;
    typedef pathvec::const_iterator const_iterator;
    typedef pathvec::reverse_iterator reverse_iterator;
    iterator begin()                      { return _path.begin();   }
    iterator end()                        { return _path.end();     }
    const_iterator cbegin() const         { return _path.cbegin();  }
    const_iterator cend() const           { return _path.cend();    }
    reverse_iterator rbegin()             { return _path.rbegin();  }
    reverse_iterator rend()               { return _path.rend();    }
    size_t size() const                   { return _path.size();    }
    std::string const& at(size_t n) const { return _path.at(n);     }
    bool empty() const                    { return _path.empty();   }
    std::string const& back() const       { return _path.back();    }

    std::string str() const;

    static std::string dot();
    static std::string dotdot();

    bool IsSeparator(char c) const;
    char PreferredSeparator() const;

    // lexical attributes
    bool IsValid() const;
    bool IsEmpty() const;
    bool IsAbsolute() const;
    bool IsRelative() const;

    // lexical decomposition
    std::string Stem() const;
    std::string Extension() const;
    T Filename() const;
    T RootName() const;
    T RootDirectory() const;
    T RootPath() const; // RootName + RootDirectory
    T ParentPath() const;
    T RelativePath() const;

    bool HasRootPath() const;
    bool HasRootName() const;
    bool HasRootDirectory() const;

    T LexicallyNormal() const;
    T LexicallyRelative(T const& base) const;
    T LexicallyAbsolute(T const& base) const;

    // lexical composition
    PathBase<T>& operator/=(T const& rhs);
    PathBase<T>& operator+=(T const& rhs);

    void RemoveFilename();
    void RemoveExtension();
    void Normalize();  // calls LexicallyNormal() on self
    void Clear();

  protected:
    pathvec _path;
    size_t _rootNameIndex;
    size_t _rootDirIndex;
    size_t _relativePathIndex;
    size_t _filenameIndex;

  private:
    bool _isValid() const = delete;   // OS-specific
    std::string _separators() const = delete; // OS-specific
    T _component(size_t index) const;
    std::string _component_str(size_t index) const;
    void _initialize(std::string const& p);
    void _initRootName(std::string* text) = delete; // OS-specific
    void _initRootDirectory(std::string* text);
    void _initRelativePath(std::string* text);
    void _initFilename(std::string* text);
    void _cullRedundantSeparators(std::string* text);
    size_t _getExtensionPos(std::string const& name) const;
  };

#include CX_OPSYS_HEADER(path.hpp)

  // PATH OPERATORS (lexical equivalence)
  bool operator==(Path const& lhs, Path const& rhs);
  bool operator!=(Path const& lhs, Path const& rhs);
  bool operator<=(Path const& lhs, Path const& rhs);
  bool operator>=(Path const& lhs, Path const& rhs);
  bool operator<(Path const& lhs, Path const& rhs);
  bool operator>(Path const& lhs, Path const& rhs);

  // PATH OPERATORS (composition)
  Path operator / (Path const& lhs, Path const& rhs);  // append
  Path operator + (Path const& lhs, Path const& rhs);  // concat


// ===================================================================


  template <class T>
  class PathListBase
  {
  public:
    PathListBase();
    PathListBase(std::string const& p);

    static const size_t npos = -1;

    typedef std::vector<Path> listvec;
    typedef listvec::iterator iterator;
    typedef listvec::const_iterator const_iterator;
    typedef listvec::reverse_iterator reverse_iterator;
    iterator begin()                      { return _list.begin();   }
    iterator end()                        { return _list.end();     }
    const_iterator cbegin() const         { return _list.cbegin();  }
    const_iterator cend() const           { return _list.cend();    }
    reverse_iterator rbegin()             { return _list.rbegin();  }
    reverse_iterator rend()               { return _list.rend();    }
    size_t size() const                   { return _list.size();    }
    Path const& at(size_t n) const        { return _list.at(n);     }
    bool empty() const                    { return _list.empty();   }
    Path const& back() const              { return _list.back();    }

    static char Delim();     // between full paths in a list
    static bool IsDelim(char c);

    std::string str() const;

    PathListBase<T>& operator+=(Path const& p);
    PathListBase<T>& operator+=(T const& p);

  private:
    listvec _list;
    static char _delim() = delete;
    void _initialize(std::string const& text);
  };


#include CX_OPSYS_HEADER(pathlist.hpp)

  // PATH-LIST OPERATORS
  PathList operator + (PathList const& lhs, PathList const& rhs);


// ===================================================================


  template <class T>
  class ProcessBase
  {
  public:
    ProcessBase(CXPID pid);

    CXPID GetPid();
    Path ExecSpec();
    void RegisterSet(size_t* size, void* buf);

  protected:
    CXPID _pid;
    Path _execSpec() const = delete;
    void _registerSet(size_t* size, void* buf) const = delete;
  };

#include CX_OPSYS_HEADER(process.hpp)


// ===================================================================


  typedef std::vector<std::string> ArgsVector;
  template <class T>
  class ShellBase
  {
  public:
    ShellBase();

    static Path Home();
    static Path GetCwd();
    static void SetCwd(Path const& p);
    static Path GetInitialPath();
    static Path GetTempDirectory();
    static Path GetUniquePath(Path const& model=Path("%%%%-%%%%-%%%%-%%%%"));

    static bool PathExists(Path const& p);
    static bool IsFile(Path const& p);
    static bool IsOther(Path const& p);
    static bool IsSymlink(Path const& p);
    static bool IsDirectory(Path const& p);
    static bool IsFileOrSymlinkToFile(Path const& p);

    static bool UserCanRead(Path const& p);
    static bool UserCanWrite(Path const& p);
    static bool UserCanExecute(Path const& p);
    static bool UserCanTraverse(Path const& p);

    static Path Relative(Path const& p, Path const& base=GetCwd());
    static Path Absolute(Path const& p, Path const& base=GetCwd());
    static Path Canonical(Path const& p, Path const& base=GetCwd());
    static Path WeaklyCanonical(Path const& p);

    static Path SystemComplete(Path const& p);

    static Path ResolveSymlink(Path const& p);

    static void CreateSymlink(Path const& from, Path const& to);
    static void CreateHardlink(Path const& from, Path const& to);
    static void CreateDirectory(Path const& p);
    static void CreateDirectories(Path const& p);
    static void Copy(Path const& from, Path const& to);
    static void Delete(Path const& p, bool nothrow=false);
    static void DeleteTree(Path const& p);

    static uintmax_t FileSize(Path const& p);
    static void ResizeFile(Path const& p, uintmax_t size);

    static bool PathsEquivalent(Path const& p1, Path const& p2);  // filesystem object equivalence

    static U8* OpenMapped(Path const& name, int *fd, size_t *psize);
    static void UnmapAndClose(int fd, U8 *buffer, size_t size);

    static Process* Spawn(Path const& execspec,
                          ArgsVector const& args,
                          bool traced);

  private:
    static Path _initialPath;

  protected:
    Path _home() const = delete;
    Path _getCwd() const = delete;
    void _setCwd(Path const& p) const = delete;
    Path _getTemp() const = delete;
    bool _pathExists(Path const& p) const = delete;
    bool _isFile(Path const& p) const = delete;
    bool _isOther(Path const& p) const = delete;
    bool _isSymlink(Path const& p) const = delete;
    bool _isDirectory(Path const& p) const = delete;
    Path _systemComplete(Path const& p) const = delete;
    Path _resolveSymlink(Path const& p) const = delete;

    bool _userCanRead(Path const& p) const = delete;
    bool _userCanWrite(Path const& p) const = delete;
    bool _userCanExecute(Path const& p) const = delete;
    bool _userCanTraverse(Path const& p) const = delete;
    void _createSymlink(Path const& from, Path const& to) const = delete;
    void _createHardlink(Path const& from, Path const& to) const = delete;
    void _createDirectory(Path const& p) const = delete;
    void _copy(Path const& from, Path const& to) const = delete;
    void _delete(Path const& p, bool nothrow=false) const = delete;
    void _deleteTree(Path const& p) const = delete;
    uintmax_t _fileSize(Path const& p) const = delete;
    void _resizeFile(Path const& p, uintmax_t size) const = delete;
    bool _pathsEquivalent(Path const& p1, Path const& p2) const = delete;
#if 0
    void _copyFile(Path const& from, Path const& to) const = delete;
    void _copySymlink(Path const& from, Path const& to) const = delete;
    void _copyDirectory(Path const& from, Path const& to) const = delete;
#endif

    U8 * _openMapped(Path const& name, int *fd, size_t *psize) const = delete;
    void _unmapAndClose(int fd, U8 *pbuf, size_t size) const = delete;

    Process* _spawn(char const** argv, bool traced) const = delete;
  };

#include CX_OPSYS_HEADER(shell.hpp)


// ===================================================================


  template <class T>
  class SystemBase
  {
  public:
    SystemBase();

    static std::string GetHardwarePlatform();

  protected:
    std::string _getHardwarePlatform() const = delete;
  };

#include CX_OPSYS_HEADER(system.hpp)


} // namespace 'OS'
} // namespace 'CX'

#endif // CX_OS_HPP

