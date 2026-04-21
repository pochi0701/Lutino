# Lightweight Web/Application Servers
- Built-in JavaScript in the server-side language
- SQL already implemented
- Just download and start, no installation required, ready to go in 10 seconds.
- HTTPS/TLS support implemented
- TLS implementation uses vendored BearSSL source code

List of Functions
1. Web Editor
1. SQL (values returned in JSON)
1. Markdown file support
1. Video streaming available

## TLS / HTTPS
- Lutino now supports HTTPS alongside HTTP.
- TLS is implemented with BearSSL source files vendored under `source\bearssl\`.
- Current configuration uses `server_tls_port` in `ltn.conf`.
- The current first-stage implementation includes an embedded test certificate for development.

## Linux build / install with CMake
```sh
cmake -S . -B build
cmake --build build
cmake --install build
```

- The Linux CMake install replaces the old `make install` flow.
- Default install root is `/usr/local/lutino`.
- Installed assets include `lutino`, `ltn.conf`, `html`, `database`, `system`, and `skin`.
- You can change the install root with `-DCMAKE_INSTALL_PREFIX=/your/path`.
- You can also override content locations written into `ltn.conf`:
  - `-DLUTINO_DOCUMENT_ROOT=/your/document/root`
  - `-DLUTINO_SKIN_ROOT=/your/skin/root`
  - `-DLUTINO_SYSTEM_ROOT=/your/system/root`
  - `-DLUTINO_DATABASE_ROOT=/your/database/root`
  - `-DLUTINO_WORK_ROOT=/your/work/root`
- Detailed notes: `CMAKE_INSTALL.md`

Example:
```sh
cmake -S . -B build \
  -DCMAKE_INSTALL_PREFIX=/opt/lutino \
  -DLUTINO_DOCUMENT_ROOT=/srv/www/lutino-html
cmake --build build
cmake --install build
```

## ver. 1.00
Lutino has changed its name from wizd and Cybele.
MFC removed.

