#Goals
The goal of this assignment was to implement two tools to compress and
decompress a given file using the huffman encoding algorithm. These
tools were implemented using the C++ language. There will be a short
section on how to build and use these tools, followed by an in depth
analysis as to their implementation details. Finally, there will be a
section on how well these tools do against different types of files,
and a discussion on possible ways to improve on them.

#Build Instructions
The tools are built using the g++ compiler and GNU Make on a Linux machine. It
is probably possible to build them on a Windows machine using Cygwin or MinGW,
but this has not been tested. There are no dependancies other than the standard
libraries.

To build the tools, simply call `make` in the root directory. This will produce
the binary `huf`.

#Interface
Each of the tools takes a mandatory input file and an optional output
file as arguments. If no output file is specified, the output is
printed to standard output. To call, simply type one of

    huf -e input [output]

to encode a file, and 

    huf -d input [output]

to decode it.

The brackets indicate an optional parameter, and should not be typed
literally.

Each of these calls will produce a single output file, and the output
`huf -e` becomes the input to `huf -d`. Note, without a switch, the program
defaults to encoding a file, and calling it as `dehuf` (via symlinking or
renaming the binary) will make it default to decoding.
