/*
 * MSVC compatibility shim for BearSSL sources.
 * Force-included via /FI to handle GCC-specific constructs.
 */
#ifndef MSVC_COMPAT_H
#define MSVC_COMPAT_H

#ifdef _MSC_VER
/* __attribute__((...)) is a GCC extension; make it a no-op on MSVC */
#define __attribute__(x)
#endif

#endif
