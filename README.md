# Chip 8 Emulator/Interpreter

An emulator and interpreter for the Chip 8 system written in C++.
Graphics were done using the [SDL 2.0 library](https://www.libsdl.org).
Unit tests on the emulator written using [GoogleTest](https://github.com/google/googletest).

Currently, there are two GUI views available a simple and detailed view. 
The simple view only displays the graphics output by the Chip 8 emulator.

The detailed view includes the graphics output along with information about the Chip 8's registers and memory.
In the detailed view, the user can pause the program and step through each instruction one-by-one.

## Screenshots (Simple View)

![](screenshots/vbrix.png)
![](screenshots/vbrix2.png)


## Screenshots (Detailed View)

![](screenshots/pong.png)
![](screenshots/maze.png)

## Attributions

The font sprite sheet used was created by user "Hobo" and was downloaded from [here](https://opengameart.org/content/monospace-bitmap-fonts-english-russian)

The Chip 8 Programs included in the screenshots and used during development (except for  `LonghornDemo.ch8`) were found [here](https://github.com/dmatlack/chip8/tree/master/roms).

The Octo Chip8 IDE made by John Earnest was extremely useful in creating `LonghornDemo.ch8`. It can be used [here](http://johnearnest.github.io/Octo/).

## References

[Cowgod's C8 Technical Reference](http://devernay.free.fr/hacks/chip8/C8TECH10.HTM#2.5)

[LazyFoo's SDL Reference](http://www.lazyfoo.net/tutorials/SDL/index.php)

