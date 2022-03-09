# SingleLinkedList
## About

Container implementation based on a linear singly linked list. Implementation of forward iterators. Ability to compare and copy lists. The ability to use initialization lists to set the contents of the list. Insertion and deletion operations at an arbitrary location. Safety sharing of list content.

## Tools

For development, C ++ standard 17 and the STL library were used.

## Compile

For building, use the utility *CMake* of version 3.1 or higher and any C++ compiler that supports the C++-17 standard. You will also need a utility *make* or its equivalent. To compile, you need to launch a terminal, go to the source folder and run the following commands: 

Linux:

    mkdir build
    cd build
    cmake ..
    make
    
Windows:

    mkdir build
    cd build
    cmake.exe ..
    mingw32-make.exe
(here, as an example, the utility *make*, which is part of the *mingw* package, is used)

If there are no errors, the *SingleLinkedList.exe* executable file will appear in the current folder for *Windows*, or *SingleLinkedList* for *Linux*.
