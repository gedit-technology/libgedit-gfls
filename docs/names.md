libgedit-gfls :: names brainstorming
====================================

Class names:
- GflsInfos, replacing GtkSourceFile or TeplFile. In the latter implementations,
  there is the ambiguity between a GFile (called a "location") and a TeplFile
  object (called "file").

  With GflsInfos, a GFile object would remain to be called a "file", simply. And
  GflsInfos reflect the fact that it gathers GFileInfos (basically) for later
  use.

- GflsLoader?
- GflsFile to extend GFile with more functions.
