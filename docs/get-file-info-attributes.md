libgedit-gfls :: get desired GFileInfo attributes in one query
==============================================================

Reasons to do it in one query:
1. To avoid unnecessary round-trips (?) (especially for remote files). Actually
   I don't know how it is implemented in GIO and GVfs. In case of doubt a single
   query is better since the implementation has the *possibility* to optimize
   network usage.
2. For convenience in the implementation, to just call one pair of
   async()/finish() functions for that task.

Full list of attributes provided by the application.

Some required/default attributes, like the file size and etag. Can depend on
which file loader options are used. The file loader adds the list of attributes
that it needs.
