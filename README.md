[![License: MPL 2.0](https://img.shields.io/badge/License-MPL%202.0-brightgreen.svg)](https://opensource.org/licenses/MPL-2.0)

# Shatter Emulator

Shatter is a Gameboy emulator written in modern C++ as a means to learn more about emulation.
- [Shatter Emulator](#shatter-emulator)
- [Building](#building)
- [Running](#running)
- [Future Plans](#future-plans)
- [Screenshots](#screenshots)

# Building

Currently Shatter requires ``SDL`` to be installed. To build, simply run:

``` bash
$ make
```

This will build the release version. For the debug version, run:

``` bash
$ make debug
```

This will create the executables in ``build/release`` and ``build/debug`` respectively.

# Running

To run Shatter, simply execute the program with the first command line argument being the path of the rom
you would like to load.
``` bash
$ shatter.exe <path_to_rom>
```

Additional arguments can be passed as well.

* ``-v`` or ``--verbose`` : Run the emulator with all opcodes logged.

# Future Plans

- [ ] Memory Bank Controllers
- [ ] Complete Graphics
- [ ] Audio
- [ ] Save States
- [ ] GameShark Codes
- [ ] Link Cable
    - [ ] Potentially linking with real Gameboys with [gbplay](https://github.com/mwpenny/gbplay)
- [ ] Gameboy Color (and other Gameboys) support

# Screenshots

|                           Garlbed Nintendo Logo and Blargg's CPU Test                          |
|:----------------------------------------------------------------------------------------------:|
|                ![Nintendont](./screenshots/Nintendont.png?raw=true "Nintendont")               |
|![Blargg cpu_instrs](./screenshots/Blargg%20cpu_instrs.png?raw=true "Blargg cpu_instrs")        |
