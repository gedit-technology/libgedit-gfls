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

But:
- Have a convenient API for the above.
