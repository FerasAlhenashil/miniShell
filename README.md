Author: Feras Alhenashil

Program: miniShell

Date: 23/4/2020

Version: 1.0

******* About the program *******

This software is a terminal (shell/bash) simulator for linux.
This shell is capable of running all of the regular linux shell 
commands (internal and external commands e.g. ls, cat etc) as well
as a few extra commands discussed in the possible input section. 



******* Operating system *******

This program only runs on a linux operating system.



******* How to Compile this program *******

Compile with gcc

To compile this program navigate to the directory of the program
from a terminal and type the following command:

gcc -o shell miniShell.c



******* To run this program *******

from command line: "./[the name of the object file that has been linked]"

(in this case "./shell").



******* Description of possible input *******

The input must be a linux-terminal command, or one of the extra built in commnads.

This shell supports the following commands:

>exit [n] where n is the exit code. 0 if no n provided

>cd [path] to change the directory to path

>cpuinfo [options] -c (for cpu clock speed) --help (for help)
-t (for cpu type) -n (for number of cores)

>meminfo [options] -t (for total RAM) -u (for used RAM) 
-c (for L2 cache size) --help (for help)

Also, all of the regular linux shell commands (internal and external commands e.g. ls, cat etc)

