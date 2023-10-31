libgedit-gfls :: split backend from frontend
============================================

To allow the possibility of a GTK 3 and GTK 4 frontends, sharing the single
codebase for the backend (based on GLib/GIO and libICU).

First idea
----------

Have two main headers, something like that:
```
#include <gfls/gfls-glib.h>
#include <gfls/gfls-gtk.h>
```

Inspired by libpeas 1.x.

Second idea
-----------

- The Gfls lib contains only the backend, it must not depend on GTK.
- Implement the GTK 3 frontend in Tepl (where TeplInfoBar is directly available,
  and the libgedit-gtksourceview API too).

The backend still has GtkTextView and its limitations in mind. Example:
- Instead of providing at the end a single UTF-8 string (which is not possible
  in certain cases like invalid chars that need to be escaped), provide _a list_
  of a data structure containing: a GBytes, boolean infos such as whether it is
  valid UTF-8, whether it's a very long line that has already been split, etc.
