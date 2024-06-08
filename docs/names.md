libgedit-gfls :: names brainstorming
====================================

Class or interface names:
- GflsInfo, replacing GtkSourceFile or TeplFile. In the latter implementations,
  there is the ambiguity between a GFile (called a "location") and a TeplFile
  object (called "file").

  With GflsInfo, a GFile object would remain to be called a "file", simply. And
  GflsInfo reflect the fact that it gathers a GFileInfo (basically) for later
  use.

- GflsLoaderConfig interface with GflsLoaderConfigSimple/Basic implementation.
- GflsLoader interface (that takes a GflsLoaderConfig as an input), with
  GflsLoaderBasic for dealing with UTF-8 only.

- GflsFile to extend GFile with more functions.
