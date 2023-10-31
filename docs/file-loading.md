libgedit-gfls :: file loading
=============================

Very first thing to check: **the file size**.

If file_size <= max, continue.

If file_size > max, show infobar with different solutions:
- Load the first max bytes only (attention the content is truncated at the end,
  so the end may be invalid chars).
- Cancel.
- Load the whole file anyway (?). Warn about the consequences (potential app
  freeze), recommend to close all other tabs before.

In all cases (except if forcing to load the whole file), stop reading after max
bytes. To avoid a
[TOC/TOU](https://en.wikipedia.org/wiki/Time-of-check_to_time-of-use)
problem.

Simple implementation first
---------------------------

Show simply an error infobar in the following cases:
- File size > max
- When loading the content, > max bytes reached.
- Invalid UTF-8.
- Contains a very long line.

Simple workflow
---------------

In that order, from a GFile:
- Query desired GFileInfos (full list of attributes provided by the
  application), in one query. Some required/default attributes, like the file
  size and etag.
- Open the GFile with `g_file_read_async()`, to get a GInputStream.
- Use GflsInputStream to load the content and get a GBytes.
- Ensure the string is valid UTF-8 and is not truncated.
- Call `gfls_utf8_has_very_long_line()`.
- Put the string into a GtkTextBuffer.
