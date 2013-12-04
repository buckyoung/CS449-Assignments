CS449-Assignments
=================

Assignments written in C for CS449 (Fall 2013)


###Assignment 1
#####RoShamBo and EXIF Viewer
The first half of Assignment 1 introduces us to C through a simple game of rock-paper-scissors. This small project focuses on reading in user input and displaying it using format specifiers. For reference, I have found the following to be useful:
- The Secrets of printf : http://byuh.doncolton.com/courses/tut/printf.pdf
- The C Library Reference Guide : http://www.acm.uiuc.edu/webmonkeys/book/c_guide/

The second half of Assignment 1 is a bit more involved, as it has us parsing through a JPEG file to read the EXIF/TIFF information. 

Our exifview program accepts a filename (jpg) as a command-line argument. It then determines if the tags can be read -- that is, it determines if the file exists, if the endianness is correct, if the exif tag is of type APP1, etc. Then it will go through the tiff-tag array and pull out the following information:

- Manufacturer:   Canon
- Model:          Canon PowerShot SX100 IS
- Exposure Time:  1/60 second
- F-stop:         f/8.0
- ISO:            ISO 100
- Date Taken:     2008:02:20 18:52:03
- Focal Length:   60 mm
- Width:          360 pixels
- Height:         240 pixels

###Assignment 2
#####MyStrings and PasswordCracking
The first half of Assignment 2 required us to write a program similiar to the "strings" process in unix. Namely, our program would accept a filename and open it in binary mode then search for any series of 4 or more printable characters. It will then print each string to the screen, one per line. This was created to, perhaps, help us with the next half of the assignment. 

The second half of the assignment was to crack 3 programs that were provided to us -- each required a password to unlock. We used gdb to disassemble the code, backtrace, and examine register values. We also utilized the mystrings program to parse the binary and objdump to view shared object files. 

Program 1: easy to unlock, the required password was moved to a register for a string comparison and was easily found.

Program 2: this program received the local date and required the first letter of the current day and month to unlock. Example, "MO" during a Monday in October.

Program 3: this program was orders of magnitude more difficult. The program was dynamically loaded and couldn't be disassembled nor were there any string compares. It boiled down to this: each character was treated as an integer and sent through a mathematical function. Certain characters, thusly, were deemed special and the 10-character-long password required exactly four special characters. Special characters are: b, c, m, n, v, x, z
So a working password would be "BaaCaaMaaN" or "BuckBuckaa" etc.

###Assignment 3
#####Custom Malloc Implementation
Created a custom implementation of malloc and free -- along with a driver to test the program. My implementation uses the next-fit algorithm for dynamic memory management.

###Assignment 4
#####Linux Device Driver and Yahtzee
My introduction to Linux device drivers and virtual file systems. Created a virtual file ("/dev/dice") which returns a one-byte number, in the range of 1-6 inclusive. Also built an implementation of Yahtzee to test the dice-rolling feature of this file. 

###Assignment 5
#####Custom POSIX Shell
Shellax is a custom POSIX shell written in C. 
