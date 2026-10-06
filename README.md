# DIR-SYS

DIR-SYS is a directory system for defining a filesystem blueprint and generating a usable directory tree from it. It is written in C and uses only the standard C library plus POSIX filesystem APIs available on normal Unix-like systems.

## Core concept

`directory/:` is the DIR-SYS **Superdirectory**. Its configuration files describe the OS, kernel, and generator. The `root/` directory below it is a template that represents `/` in the generated filesystem.

```text
directory/:/root/home/usr/Downloads/
                    |
                    +----> output/home/usr/Downloads/
```

The literal `:` in `directory/:` is intentional and is part of the repository layout.

## Repository layout

```text
DIR-SYS/
├── README.md
├── LICENSE
├── Makefile
├── src/
├── include/dirsys/
├── directory/:/
│   ├── os.conf
│   ├── kernel.conf
│   ├── dirsys.conf
│   └── root/
├── build/
├── output/
└── tests/
```

The blueprint root contains directories only. Configuration files are kept directly inside the Superdirectory and are never copied into the generated root filesystem by default.

## Configuration

### `directory/:/os.conf`

Defines OS identity:

- `OS_NAME`
- `OS_VERSION`
- `OS_ARCH`
- `OS_BUILD`
- `OS_RELEASE`

### `directory/:/kernel.conf`

Defines kernel identity and boot parameters:

- `KERNEL_NAME`
- `KERNEL_VERSION`
- `KERNEL_ARCH`
- `BOOT_MODE`
- `KERNEL_PARAMETERS`

### `directory/:/dirsys.conf`

Defines DIR-SYS itself:

- `DIRSYS_NAME`
- `DIRSYS_VERSION`
- `ROOT_TEMPLATE`
- `OUTPUT_PATH`

The parser accepts `KEY=VALUE`, optional surrounding whitespace, `#` comments, and double-quoted values. Keys may contain letters, digits, and underscores. Duplicate keys, invalid keys, malformed assignments, empty values, and oversized lines are rejected with an error.

## Building

```bash
make
```

or:

```bash
make build
```

The executable is written to `build/dirsys`.

## Generating a filesystem

From the repository root:

```bash
./build/dirsys generate
```

This reads `directory/:/dirsys.conf`, uses its `ROOT_TEMPLATE` setting to select `directory/:/root`, and generates the tree under `OUTPUT_PATH` (`output/` by default).

You can explicitly select a config or destination:

```bash
./build/dirsys generate --config directory/:/dirsys.conf --output output
```

Generation is directory-only: regular files, FIFOs, sockets, device nodes, and symlinks in the blueprint are rejected. Existing directories are reused safely; existing non-directories cause generation to fail.

## Validation

Validate the configured blueprint without generating it:

```bash
./build/dirsys validate
```

The validator recursively checks that the blueprint exists and contains directories only.

## Tests

Run all parser, generator, mapping, and filesystem tests with:

```bash
make test
```

The test suite verifies valid and invalid configuration parsing, comments and key/value handling, root-to-output mapping, nested directory generation, existing destination handling, invalid paths, the complete blueprint structure, and `/home/usr/` user storage directories.

## Cleaning

```bash
make clean
```

This removes compiled artifacts and generated contents from `output/`, while preserving the repository's `.gitkeep` files.

## User storage

The default user storage area is:

```text
/home/usr/
```

with:

```text
Mobile/
Photo/
Video/
Audio/
Alarm/
Downloads/
Docs/
OS/
Apps/
```

## Safety properties

DIR-SYS does not copy the Superdirectory configuration files into the generated root filesystem. It also does not invent files inside the blueprint: every generated filesystem entry corresponds to an existing directory in the blueprint.

The implementation performs explicit `lstat()` checks and refuses non-directory blueprint entries and symlinks, reducing the risk of accidentally traversing outside the intended blueprint.
