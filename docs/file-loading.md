libgedit-gfls :: file loading TODO list
=======================================

Check the file size
-------------------

Very first thing to check: **the file size**.

If `file_size` > max, show an error infobar.

Get desired GFileInfos in one query
-----------------------------------

In one query: to avoid unnecessary round-trips (especially for remote files).

Full list of attributes provided by the application.

Some required/default attributes, like the file size and etag.
