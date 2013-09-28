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

Manufacturer:   Canon
Model:          Canon PowerShot SX100 IS
Exposure Time:  1/60 second
F-stop:         f/8.0
ISO:            ISO 100
Date Taken:     2008:02:20 18:52:03
Focal Length:   60 mm
Width:          360 pixels
Height:         240 pixels
