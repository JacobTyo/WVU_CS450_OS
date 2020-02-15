Original Repository: https://bitbucket.org/jtyo/mpx-spring2016-group9/src/default/

WVU CS450 Group9 {Phishing for Cookies} Project

NOTE TO USER (GRADER):
Documentation is generated in the form on HTML, rtf, and man.  The most convenient way to view all documentation is to open index.html in the $PROJECT_HOME/docs/R5/html/index.html and then navigate through the documents that way.  It is also worth mentioning that the main page of the opened website will be blank, but the data structures and files will contain all relevant information.  

CURRENT PROJECT STATUS:
(4/11/16 - jitschne) - TODO List

(4/7/16 - ALL) - Finished

NEXT STEPS:

TODO:
Print Boot Sector Information
Print Root Directory
Change Directory
List Directory (like ls), can specify (list --*.txt)
Type
Rename

REMEMBER:
command -filename -filepath/name contained in the disk image


NOTES:
http://www.eit.lth.se/fileadmin/eit/courses/eitn50/Projekt1/FAT12Description.pdf

http://www.ntfs.com/fat-systems.htm


CONTRIBUTORS:
Chance Grillot
Jonathan Itschner
Shelby Shuff
Jacob Tyo




MPX STUFF:

To boot, run -> qemu-system-i386 -nographic -kernel kernel.bin -s
   Be sure to first loudify Ubuntu 15.04 enviornment and 'source cs450-setup'

Currently (unless something changes) only add and push the modified/added kernel and module c files.  In other words, do not add the files generated after running 'make' on the project.

Environment used for editing is currently just the command line and emacs
