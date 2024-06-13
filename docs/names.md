libgedit-gfls :: names brainstorming
====================================

Class or interface names:
- GflsInfo, replacing GtkSourceFile or TeplFile. In the latter implementations,
  there is the ambiguity between a GFile (called a "location") and a TeplFile
  object (called "file").

  With GflsInfo, a GFile object would remain to be called a "file", simply. And
  GflsInfo reflect the fact that it gathers a GFileInfo (basically) for later
  use.

- GflsFile to extend GFile with more functions.

- GflsLoader interface (that takes a GflsLoaderConfig as an input), with
  GflsLoaderBasic for dealing with UTF-8 only.

- Driver: like a compiler driver that invokes several smaller programs. Have a
  driver for loading, and a driver for saving, that invoke smaller utilities
  with phases. The utilities are just classes and functions to be called (in the
  same process). A bit like a "Manager" class, except that the name is shorter.
