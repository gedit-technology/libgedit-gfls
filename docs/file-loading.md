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
