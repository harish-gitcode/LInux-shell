# Linux-Sytem-Monitor
A basic System Monitor for Linux

This is a basic project that builds an htop-like process monitor for Linux
To run it Download or clone the repository locally and go to the Download directory
run
$ ./Linux-System-Monitor/build/monitor

This may not run if you don't have Ncurses Library installed or configured properly as the project uses Ncurses Display for displaying the processes' and system's stats on to the terminal(like htop)

If you wish to build or modify you may need to install CMake(or any other build system)
I've included the corresponding CMakeTxt file and MakeFile configuration in case you build it
I've also included the .vscode folder that does the building and running as a vscode tasks/launch.json if you wish to build it in VSCode editor

The project mainly reads the stats from the Linux's /proc (pseudo)filesystem and displays accordingly. It may appear broken if you have a different filesystem 
The calculations for memory and CPU are based on proc man page https://man7.org/linux/man-pages/man5/proc.5.html
However, since there's no absolute way of defining memory so may come up with your own formula for caclulating the RAM usage
