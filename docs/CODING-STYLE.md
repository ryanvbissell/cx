CODING STANDARDS
================

This file may eventually be fleshed out with more specific information,
but for now:

* Follow existing style in any file you are editing
* Emulate the prevailing style-- which hopefully complies with this
  document-- of existing files in any new file you create
* Start every new source file with a copy of the approprate COPYRIGHT-FOO
  file from within boilerplate/
* Update copyright dates as appropriate


* Try to wrap before 72 columns, but certainly before 78 columns.  This
  is to facilitate quoting code in plain email or newsgroups.
* Indentation is 2 spaces, no tab characters.
* Unix-style line endings


* When feasible & prudent, use pure virtual-base-class-and-derived-impl-class
  (with factory function) idiom
* Use `FirstCapCamelCase()` for public methods (or `short()` when idiomatic)
* Use `lowerFirstCamelCase()` for non-public methods
* Use `short_` for non-public data members
* Use `CX_ALLCAPS` for macros (internal underscores OK)
* Use `ALLCAPS` for enum-class values (avoid underscores)
* Use `FirstCapCamelCase` for types (with some exceptions, TBD.)
* Use `snake_case()` for free functions (in any namespace)
* Use `_snake_case()` for static free functions (not in global namespace)
