
CX
========
A C++ development toolkit, including:

* exception-aware topical trace-logging
* intrusive-pointer implementation
* os abstraction
* ncurses encapsulation

and more.

This is really just a personal toolbox of code that I like to reuse
from one project to another.  It isn't likely to be interesting to
anyone else.

But in the event that it is, I offer it under a BSD-2-Clause license.
(See the enclosed 'LICENSE' file for exact license terms.)

It is intended to be grafted into other projects, such as via a git
submodule.  It's only dependencies are:

* C++17 compiler (tested w/ G++ on Linux)
* GNU Make 4.1 or better

My other project 'make-forge' will be recursively fetched whenever
you clone this repo; make-forge is required for building cx, or
running the CX test suite.

<sup>
The name 'CX' is historical; before the turn of the millennium, I was
working on a Xlib widget set called 'Criterion X', and 'CX' was the prefix
I used for all the typedef names, class definitions, etc. within that
code base.  That project has long-since been abandoned, however I've
continued to use CX's debug tracing functionality and exception classes,
in my more modern personal projects.  The name stuck, but no longer
means the same thing.  Perhaps today the X in 'Criterion X' could mean
"fill-in-the-blank"
</sub>
