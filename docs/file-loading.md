libgedit-gfls :: file loading TODO list
=======================================

About file size
---------------

Very first thing to check: **the file size**.

If `file_size` > max, show infobar with different solutions:
- Load the first max bytes only (attention the content is truncated at the end,
  so the end may be invalid chars). Mark that the content has been truncated, so
  when trying to save the document a warning is shown (if the document is
  editable).
- Cancel.
- Load the whole file anyway (?). Warn about the consequences (potential app
  freeze), recommend to close all other tabs before.

Get desired GFileInfos in one query
-----------------------------------

In one query: to avoid unnecessary round-trips (especially for remote files).

Full list of attributes provided by the application.

Some required/default attributes, like the file size and etag.
