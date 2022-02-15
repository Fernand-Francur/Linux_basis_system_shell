Copyright Timothy Borunov 2020

PROGRAM: myshell
// The purpose of this program is to act as a simple shell within a linux C environment
// The program consists of a regular expression parser, fork/pipe multiple processing, 
// and signal processing via a signal handler.
// The program can run any path command via execvp and can run and catch processes 
// running in the background.

// The program consists of the following files compiled via a makefile:
FILES: myshell.c myshell.h myshell_parser.c myshell_parser.h

// Run program using: make
// Remove all created objects by using: make clean

// The following links were utilized in the creation of the program:

// Utilized for understanding how to capture output of child process and redirection 
// via dups and pipes:
http://www.microhowto.info/howto/capture_the_output_of_a_child_process_in_c.html

// I was having a hard time understanding the syntax of piping, and this resource was
// the most useful in understanding how to link more than 2 piped commands together
// I used this source as a reference for making the pipe while loop.
http://www.cs.loyola.edu/~jglenn/702/S2005/Examples/dup2.html

// Used for understanding and developing my own regular expression in order to parse 
// the command line:
https://regex101.com/
https://developer.mozilla.org/en-US/docs/Web/JavaScript/Guide/Regular_Expressions/Cheatsheet
https://pubs.opengroup.org/onlinepubs/7908799/xsh/regex.h.html

// Used for understanding the basic form and flags of sigchld_handler function:
https://diveintosystems.org/book/C13-OS/ipc_signals.html
https://www.qnx.com/developers/docs/6.5.0SP1.update/com.qnx.doc.neutrino_lib_ref/s/sigaction_struct.html

// Learning outcomes: Regular expressions, Pipes, Forks, Intro to signals, Filed 
// redirection, proper memory allocation