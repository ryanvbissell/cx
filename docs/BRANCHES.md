
BRANCHES
========
This file describes the branching strategy for this project.

(You probably want to be on the `master` branch when reading this file,
since any other branch potentially has an older copy.)


DEVELOPMENT BRANCHES
--------------------
* Start with the prefix 'dev-'
* Are named for the major & minor releases that will eventually be made
  from them. Examples:
    * '`dev-2.0`', for work *leading* to r2.0, r2.0.1, r2.0.2, etc.
    * '`dev-2.1`', for work *leading* to r2.1, r2.1.1, etc.
* Dev-branches are not created for work towards point-releases.


RELEASE TAGS
------------
* Start with the lower-case letter 'r'
* Will be made in the appropriate development branch.  Example:
    * `r3.1` and `r3.1.1` tags created in `dev-3.1`


TOPIC BRANCHES
--------------
* Start with the lower-case letter 't'
* Are named for either the release tag they most recently succeed, or
  (failing that) the development branch that they are branched from.
* Should be named in a hierarchical fashion, and should be
  structured as `t#.#[.#]/category/subject`
  Examples:
    * `t1.2.1/bug/90210`
    * `t3.1/maint/fix-typos`
* Permitted category names are:
    `bug`, `maint`, `feature`, `refactor`, `test`, `build`
* When branched from a minor-release tag, these will be primarily for
  bug fixes.  Depending on complexity and the number of developers, such
  minor-topic branches might not be tracked upstream; rather, they would
  likely be pushed by the developer directly into the minor release
  branch, for inclusion in the next point-release.
* From a major development branch (e.g., `dev-#.0`) topic branches
  will be for development of new features intended for inclusion in the
  next minor release.  Such topic branches will most certainly be hosted
  upstream.
  Example:  `t3.0/feature/sentience`


USER BRANCHES
-------------
If there is ever a need for branch namespaces for individual developers,
they should follow the template `u/jsmith/whatever`.  But, presently
this is not planned.


INTEGRATION FLOW
----------------
* All changes will typically flow all the way back to `master`, being
  picked up by the immediate parent dev-branch, and any descendent
  dev-branches that exist at the time.
  Example:
    * `t1.2.1/bug/90210` --> `dev-1.2` (& `dev-1.3` if it exists) --> `master`

