// vim: set tabstop=2 softtabstop=2 shiftwidth=2 expandtab :
/*
 * Copyright (c) 2013-2017, Ryan V. Bissell
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-2-Clause
 * See the enclosed "LICENSE" file for exact license terms.
 */

#ifndef LINUX_SHELL_HPP
#define LINUX_SHELL_HPP


class Shell_Linux : public ShellBase<Shell_Linux>
{
  friend ShellBase<Shell_Linux>;

  public:
    Shell_Linux();

  protected:
    Path _home() const;
    Path _getCwd() const;
    void _setCwd(Path const& p) const;
    Path _getTemp() const;
    bool _pathExists(Path const& p) const;
    bool _isFile(Path const& p) const;
    bool _isOther(Path const& p) const;
    bool _isSymlink(Path const& p) const;
    bool _isDirectory(Path const& p) const;
    Path _systemComplete(Path const& p) const;
    Path _resolveSymlink(Path const& p) const;

    bool _userCanRead(Path const& p) const;
    bool _userCanWrite(Path const& p) const;
    bool _userCanExecute(Path const& p) const;
    bool _userCanTraverse(Path const& p) const;
    void _createSymlink(Path const& from, Path const& to) const;
    void _createHardlink(Path const& from, Path const& to) const;
    void _createDirectory(Path const& p) const;
    void _createDirectories(Path const& p) const;
    void _copy(Path const& from, Path const& to) const;
    void _delete(Path const& p, bool nothrow=false) const;
    void _deleteTree(Path const& p) const;
    uintmax_t _fileSize(Path const& p) const;
    void _resizeFile(Path const& p, uintmax_t size) const;
    bool _pathsEquivalent(Path const& p1, Path const& p2) const;
#if 0
    void _copyFile(Path const& from, Path const& to) const;
    void _copySymlink(Path const& from, Path const& to) const;
    void _copyDirectory(Path const& from, Path const& to) const;
#endif
    U8 * _openMapped(Path const& name, int *fd, size_t *psize) const;
    void _unmapAndClose(int fd, U8 *pbuf, size_t size) const;
    Process* _spawn(char const** argv, bool traced) const;
};

using Shell = Shell_Linux;


#endif // LINUX_SHELL_HPP
