libgedit-gfls :: file loading TODO list
=======================================

Check the file size
-------------------

Very first thing to check: **the file size**.

If `file_size` > max, show an error or warning infobar.

File loading steps
------------------

Name: GflsLoadingDriver, or GflsLoadingManager, or GflsLoader.

Steps:

1. Create a GflsLoaderConfig(Simple).
2. (Optional) If GFile as input, query the GFileInfo.
3. (Optional) Check the GFileInfo against the GflsLoaderConfig(Simple) (check
   the file size).
4. Load all the raw content into memory (with max size).
5. (Optional) Transform if needed the raw content into the final content
   suitable to be inserted into a GtkTextBuffer (encoding conversion, splitting
   very long lines, escaping invalid characters and maybe others, keeping the
   relevant information along the way in order to reverse the operation - or
   show an error - upon saving).
6. Insert the final content into the GtkTextBuffer.
