libgedit-gfls :: simple implementation first
============================================

Requirements
------------

Validate input for GtkTextView purposes:
- Must not exceed a certain size.
- Must be valid UTF-8 already.
- Must not contain very long lines.

So:
- No charset auto-detection and conversion.
- No workarounds for problems found, just return an error without the
  possibility to re-configure the loading.

But:
- Have a convenient API for the above (GFile loading into a GtkTextBuffer).
