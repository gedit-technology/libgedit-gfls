libgedit-gfls :: low-level features
===================================

Done
----

Loading: provide a maximum number of bytes to read, and get a GBytes as a
result, knowing if it is truncated.

Loading: detect if there is a very long line in a UTF-8 string.

To implement
------------

Have a list of encodings.

Loading: encoding auto-detection.

Loading: check if the content is valid in a given encoding (useful if the user
explicitly asks for a certain encoding).

Loading: encoding conversion to UTF-8.

Loading: detect BOM.

Split a GBytes into a list of chunks. Each chunk contains:
- a GBytes
- a boolean to tell if the GBytes is a valid UTF-8 string.
To isolate invalid chars.

Further split the list of chunks to isolate very long lines (in UTF-8) into
their own chunks.

Transform a very-long-line chunk with the line split at a certain column, and
with a certain algorithm to split the line (at chars or word-chars). Adding a
boolean to tell that the very long line has been split.
