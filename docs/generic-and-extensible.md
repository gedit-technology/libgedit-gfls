libgedit-gfls :: generic and extensible API
===========================================

Have for Gfls a **generic** and **extensible** API.

Assumptions
-----------

- Assume **GIO** use, in all cases.
- Assume the intention to work with **text** (but allow workarounds if there are
  invalid chars).

Use-case
--------

Tepl has a framework (so with an inversion of control), with TeplAbstractFactory
that apps subclass to customize the default behavior of Tepl.

For a high-level file loading and saving API, Tepl can thus call the generic
Gfls functions (i.e., probably just using interfaces, not concrete classes), and
call TeplAbstractFactory functions to create and configure the concrete Gfls
objects.
