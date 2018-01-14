// vim: set tabstop=2 softtabstop=2 shiftwidth=2 expandtab :
/*
 * Copyright (c) 2016-2017, Ryan V. Bissell
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-2-Clause
 * See the enclosed "LICENSE" file for exact license terms.
 */

#define CX_TESTING

#include "cx-test-support.hpp"
#include "cx-os.hpp"

#include <functional>

using CX::OS::Path;
using CX::OS::Shell;

// testing infrastructure guarantees this is our sandbox
Path sandbox = Shell::GetCwd();

std::vector<std::string> reasons;

bool _test_qualify_partial( std::string const& name,
                            Path const& path)
{
  // we don't print out the path, because it can vary by system & user,
  // and that would cause the test to fail checksum.
  bool valid = path.IsValid();
  bool exists = Shell::PathExists(path);
  printf( "'%s' directory is semantically %s",
      name.c_str(), valid ? "valid, " : "INVALID.\n" );

  if (valid)
    printf( "%s", exists ? "exists, " : "but DOESN'T exist.\n" );

  return valid && exists;
}

void _test_qualify_directory( std::string const& name,
                              Path const& path)
{
  if (_test_qualify_partial(name, path))
  {
    bool isdir = Shell::IsDirectory(path);
    printf( "%s a directory.\n", isdir ? "and is" : "but ISN'T" );
  }
}

void test_shell_home()
{
  Path homedir = Shell::Home();
  _test_qualify_directory("home", homedir);
}


void test_shell_get_temp()
{
  Path tempdir = Shell::GetTempDirectory();
  _test_qualify_directory("temp", tempdir);
}


void test_shell_get_cwd()
{
  Path cwddir = Shell::GetCwd();
  _test_qualify_directory("cwd", cwddir);
}


void test_shell_get_initdir()
{
  Path initdir = Shell::GetInitialPath();
  _test_qualify_directory("init", initdir);
  printf("\n");
  Path cwddir = Shell::GetCwd();
  if (cwddir == initdir)
    printf("Initially, 'cwd' is the same as 'init'\n");
  Shell::SetCwd(Path("/"));
  cwddir = Shell::GetCwd();
  printf("But after changing cwd, %s.\n",
          cwddir == initdir ? "THEY STILL ARE" : "they no longer are" );
  Shell::SetCwd(initdir);
}


void test_shell_homedir_types()
{
  printf("\n");
  bool istype;
  Path homedir = Shell::Home();
  istype = Shell::IsFile(homedir);
  printf("'homedir' %s a file.\n", istype ? "IS" : "isn't");
  istype = Shell::IsOther(homedir);
  printf("'homedir' %s an other.\n", istype ? "IS" : "isn't");

  printf("('homedir' may or may not be a symlink.)\n");

  istype = Shell::UserCanTraverse(homedir);
  printf( "'homedir' %s traversable.\n",
          istype ? "is" : "ISN'T" );
  istype = Shell::UserCanExecute(homedir);
  printf( "'homedir' %s an executable (directories shouldn't be).\n",
          istype ? "IS" : "isn't" );
}


void test_shell_perms(std::string name, Path path)
{
  bool read = Shell::UserCanRead(path);
  bool write = Shell::UserCanWrite(path);
  bool exec = Shell::UserCanExecute(path);
  bool trav = Shell::UserCanTraverse(path);

  printf( "'%s' is %s, %s, %s, and %s\n",
          name.c_str(),
          read ? "readable" : "not readable",
          write ? "writable" : "not writable",
          trav ? "traversable" : "not traversable",
          exec ? "EXECUTABLE" : "not executable" );
}


void test_shell_dir_perms()
{
  printf("\n");
  test_shell_perms("homedir", Shell::Home());
  test_shell_perms("rootdir", Path("/"));
}


void test_shell_base_relative()
{
  // Shell::Relative()'s results are system- and runtime-
  // dependent, making it ill-suited for this kind of testing.
}


void test_shell_base_absolute()
{
  // Shell::Absolute()'s results are runtime-dependent,
  // making it ill-suited for this kind of testing.
  // Also, it basically just calls Path::LexicallyAbsolute().
}


void test_shell_base_canonical()
{
  // Shell::Canonical()'s results are system- and runtime-
  // dependent, makign it ill-suited for this kind of testing.
}


void test_shell_base_weakly_canonical()
{
  // Shell::WeaklyCanonical()'s results are system- and runtime-
  // dependent, makign it ill-suited for this kind of testing.
}


void test_shell_system_complete()
{
  // Shell::SystemComplete()'s results are runtime-dependent,
  // making it ill-suited for this kind of testing.
}


typedef void (*sandboxed_test)(Path const& temp);


void sandboxed_try(std::string what, std::function<void()> f)
{
  std::string result("PASS");

  CX_TRY
  {
    f();
  }
  CX_CATCH(CX::OS::Exception const& e)
  {
    reasons.push_back(e.Reason());
    result = "<throws, reason #" + std::to_string(reasons.size()) + ">";
  }
  CX_ENDTRY

  printf("%20s: %s\n", what.c_str(), result.c_str());
}

void _create_directory(Path const& temp)
{
  Shell::CreateDirectory(temp);
}



void sandboxed_tests()
{
  printf("\n\n");
  reasons.clear();

  Path temp = sandbox / Path("oops/../created");
  sandboxed_try("CreateDirectories", [&](){Shell::CreateDirectories(temp);});

  if (reasons.size())
  {
    printf("\n");
    for (size_t r=0; r<reasons.size(); ++r)
      printf("[%lu] '%s'\n", r+1, reasons[r].c_str());
  }
}


int main(int argc, char** argv)
{
  printf("Sandbox is %s\n", sandbox.str().c_str());

  test_shell_home();
  test_shell_get_temp();
  test_shell_get_cwd();
  test_shell_get_initdir();
  test_shell_homedir_types();
  test_shell_dir_perms();

  // these tests are presently impotent
  test_shell_base_relative();
  test_shell_base_absolute();
  test_shell_base_canonical();
  test_shell_base_weakly_canonical();
  test_shell_system_complete();

  sandboxed_tests();
}






