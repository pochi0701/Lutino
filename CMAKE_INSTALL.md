# Linux build and install

Both the Makefile and CMake install Lutino into a single directory. The default
directory is:

```text
/var/www/html9
```

The install operation places only these items directly in that directory:

- the `lutino` executable
- `ltn.conf.linux`, installed as `ltn.conf`
- the `html/` directory
- the `database/`, `skin/`, and `system/` directories

The installed `ltn.conf` has its active `document_root`, `skin_root`, and system
alias changed to the corresponding `html/`, `skin/`, and `system/` paths under
the selected install directory. The destination directory is created
automatically when it does not exist.

Installation stops with a warning before copying anything if the destination
directory already contains a file or directory.

## Makefile

Build and install from the `source/` directory:

```sh
cd source
make
sudo make install
```

Change the destination with `INSTALL_DIR`:

```sh
sudo make install INSTALL_DIR=/srv/www/lutino
```

## CMake

Configure and build from the repository root:

```sh
cmake -S . -B build
cmake --build build
sudo cmake --install build
```

Change the destination during configuration:

```sh
cmake -S . -B build -DCMAKE_INSTALL_PREFIX=/srv/www/lutino
cmake --build build
sudo cmake --install build
```

The destination can also be selected at install time:

```sh
sudo cmake --install build --prefix /srv/www/lutino
```
