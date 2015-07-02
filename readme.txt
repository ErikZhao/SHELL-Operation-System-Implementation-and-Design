Project 1 of shell design in Minix 3.2.1
Enviroment variable file: PROFILE

Usage:
============
1. Invoke our shell
Put myshell folder to directory /root/usr and start Minix.
Type: cd /usr/myshell (cd directory to our shell) 
 make shell (compile shell) 
     ./shell (run shell and directory will be HOME) 

2. Commands 
Type: pwd (check current directory)

Provide commands like: ls cat wc date

Provide redirector pipe and indirection operator
Type: wc $ fgrep malloc shell.c 
Type: ls => FILES 

Support integer variables and store it in calculator.txt
Type: $ a = 4

Type: exit (shell will be terminated)
Press Ctrl+C ＾Are you sure?￣ and exits after confirmation

3. Calculator 
Enter the calculator type: ./cal.o 
Write value format: $variable name=variable value (eg. $a=10). Variable will be stroed in calculator.txt file.
Calculate format: $variable name operator another variable name (eg. a+b). Result will be displayed on screen.
To exit, press Ctrl+C.
After write the process will automatically exit calculator to reset file pointer.

Attention:
　　Because of the lack of support for regular expressions, file's names such as *.c are not support.

Consist of:
============
-shell.c
　　main program

-prompt.c
　　Print out the prompt of wshell including path,hostname

-read_command.c
　　Read command input, and analyse the command and parameter(s).

-builtin_command.c
　　Support some built-in command, such as exit,quit,about, and cd.

-parsing.c
　　Analyses user's input line and tell them to wshell.

-cal.c
    Calculator application can use the variables as input or output and operate them.
