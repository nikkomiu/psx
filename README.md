# PS1 Development

## Development Requirements

The development environment is a bit strange for PSX nowadays.
To get it working "my way" check how `.devcontainer/Dockerfile` does it.
I don't currently recommend using the Dev Container as PCSX-Redux isn't working very well
within the Dev Container. Also, running PCSX-Redux on your system while developing within the
Dev Container is really annoying because I haven't gotten GDB to work across the chasm of the
Docker-host connection.

### Required Software

The following software is required to be in your `PATH`:

- [ARMIPS Assembler](https://github.com/Kingcom/armips)

The following libraries need to be on your system:

- [Nugget](https://github.com/pcsx-redux/nugget.git)
- [PsyQ](http://psx.arthus.net/sdk/Psy-Q/psyq-4.7-converted-full.7z)

> **Note:** I have mine in `/usr/local/<nugget,psyq>`. However, you could also put them in the project directory
> or under either the `third_party` or `thirdparty` subdirs within the project directory.

### VS Code Development

For developing using VS Code:

- Install all recommended extensions
- Debugging for C projects should just work with the debugging interface built-in to VS Code.

### Add an Assembly Project

Create a `Makefile` in the project directory. You can use the following as a starting point:

```makefile
all: thing.ps-exe

include ../scripts/asm.mk
```

Replace **thing** with your source file name (`thing.s` would be the source file for this).
Also, make sure the `.create "thing.bin", 0x80010000` directive is set to the same name as the asm file.

> **Note:** With assembly there isn't any way (currently) to load multiple source files
> for the build so you can have many "games" within a single directory.

### Add a C Project

Create a `Makefile` in the project directory. You can use the following as a starting point:

```makefile
TARGET = thing

SRCS = main.c

include ../scripts/common.mk
```

> **Note:** Each directory must contain only one project.

## Emulators

Debugging Emulators:

- [no$psx](https://problemkaputt.de/psx.htm) (for 32-bit OS)
- [PCSX-Redux](https://github.com/grumpycoders/pcsx-redux) (for 64-bit OS)
