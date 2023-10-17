libgedit-gfls :: split backend from frontend
============================================

Have two main headers, something like that:
```
#include <gfls/gfls-glib.h>
#include <gfls/gfls-gtk.h>
```

To allow the possibility of a GTK 3 and GTK 4 frontends, sharing the single
codebase for the backend (based on GLib/GIO and libICU).
