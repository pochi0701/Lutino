# Linux CMake build and install

This document describes the new Linux build/install flow that replaces the old `source/Makefile` + `make install` usage.

## Overview

The CMake-based flow does these jobs:

- builds the `lutino` executable
- builds the vendored BearSSL-based TLS code together with Lutino
- installs the executable and runtime assets
- generates `ltn.conf` during install
- allows the install destination and content roots to be changed

## Basic usage

```sh
cmake -S . -B build
cmake --build build
cmake --install build
```

Default install root:

```text
/usr/local/lutino
```

## Installed items

The install step places these items under the install root unless overridden:

- `lutino`
- `ltn.conf`
- `html`
- `database`
- `system`
- `skin`
- `README.md`
- `LICENSE`

## Changing the install root

Use `CMAKE_INSTALL_PREFIX`:

```sh
cmake -S . -B build -DCMAKE_INSTALL_PREFIX=/opt/lutino
cmake --build build
cmake --install build
```

## Changing document and asset roots

The install step also writes these paths into `ltn.conf`.

Available cache variables:

- `LUTINO_DOCUMENT_ROOT`
- `LUTINO_SKIN_ROOT`
- `LUTINO_SYSTEM_ROOT`
- `LUTINO_DATABASE_ROOT`
- `LUTINO_WORK_ROOT`

Example:

```sh
cmake -S . -B build \
  -DCMAKE_INSTALL_PREFIX=/opt/lutino \
  -DLUTINO_DOCUMENT_ROOT=/srv/www/lutino-html \
  -DLUTINO_SKIN_ROOT=/srv/www/lutino-skin \
  -DLUTINO_SYSTEM_ROOT=/srv/www/lutino-system \
  -DLUTINO_DATABASE_ROOT=/srv/www/lutino-database \
  -DLUTINO_WORK_ROOT=/var/lib/lutino/work

cmake --build build
cmake --install build
```

## Generated configuration

`cmake --install` generates `ltn.conf` and sets at least these items:

- `server_port`
- `server_tls_port`
- `document_root`
- `skin_root`
- `skin_name`
- `skin_menu`
- `flag_execute_cgi`
- `flag_allow_proxy`

## Relation to the old Makefile

The old Makefile install flow was fixed to a `/var/www/...` style layout.

The CMake flow is intended to replace that by making:

- the install root configurable
- the document root configurable
- the skin/system/database/work locations configurable
- TLS/BearSSL sources part of the normal build

## Notes

- TLS/HTTPS support is implemented with vendored BearSSL source under `source/bearssl/`.
- The current first-stage TLS setup still uses an embedded test certificate for development.
- If you use absolute paths for the `LUTINO_*_ROOT` variables, those paths are written directly into `ltn.conf`.
- If you use relative paths, they are treated relative to `CMAKE_INSTALL_PREFIX`.
