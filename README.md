#Contents
1\. Introduction

    1.1 Goals

    1.2 Build Instructions

    1.3 Interface

2\. Implementation

    2.1 Huffman Tree

        2.1.1 Encoding

            2.1.1.1 First Pass

                2.1.1.1.1 Frequency analysis

                2.1.1.1.2 Tree building

                2.1.1.1.3 Indexing characters

            2.1.1.2 Second Pass

        2.1.2 Decoding

            2.1.2.1 Tree Files

                2.1.2.1.1 Saving the Trees

                    2.1.2.1.1.1 Directly as a Tree

                    2.1.2.1.1.2 Frequency List

                    2.1.2.1.1.3 Multiple Files vs. Single Files

                2.1.2.1.2 Reading the Tree

                2.1.2.1.3 The Tree-Walking Method

    2.2 Bit Vectors

3\. Results

    3.1 Possible Directions


#1. Introduction
##1.1 Goals
The goal of this assignment was to implement two tools to compress and
decompress a given file using the huffman encoding algorithm. These
tools were implemented using the C++ language. There will be a short
section on how to build and use these tools, followed by an in depth
analysis as to their implementation details. Finally, there will be a
section on how well these tools do against different types of files,
and a discussion on possible ways to improve on them.

##1.2 Build Instructions
The tools are built using the g++ compiler and GNU Make on a Linux
machine. It is probably possible to build them on a Windows machine
using Cygwin or MinGW, but this has not been tested.

To build the tools, simply call `make` in the root directory. Two
binary files should be produced: `huffmanencode` and `huffmandecode`.

##1.3 Interface
Each of the tools takes a mandatory input file and an optional output
file as arguments. If no output file is specified, the output is
printed to standard output. To call, simply type one of

    huffmanencode input [output]
    huffmandecode input [output]

The brackets indicate an optional parameter, and should not be typed
literally.

Each of these calls will produce a single output file, and the output
`huffmanencode` becomes the input to `huffmandecode`.


#2. Implementation
##2.1 Huffman Tree
The huffman tree is the backbone of this algorithm. Building the tree
is easy enough, but there was a significant challenge in finding a
format to save the tree in which would be space efficient. The best
way to discuss the tree, is to discuss the context in which it is
used, so there will be two secions for the two contexts in which the
tree is used: one for the encoding phase, and one for the decoding
phase.

###2.1.1 Encoding
To encode an input file, the algoritm requires two passes. The first
builds the huffman tree, the second uses it to encode the file. To
properly decode the file, the tree must also be saved somehow, but
that will be covered in the section on decoding.

####2.1.1.1 First Pass
#####2.1.1.1.1 Frequency analysis
The first step of building the huffman tree is to analyze the
frequency of each character in the file. In other words, two things
are needed: the set of characters present in the file, and the
frequencies with which those characters appear in the file.

Below is some pseudo code to build a frequency table.

    def freq_table(file):
        frequencies = new empty_array[256]
        for character in file:
            frequencies[character] += 1
        return frequencies

The code creates a map of 256 values, all initially 0. 256 is picked
because it is the number of distinct values possible in a single byte.
It then loops over each character in the file and counts the number of
occurences of each file.

It should be obvious that this code is linear in the number of bytes
in the file. There is some initial overhead involved with allocating
and zeroing out the frequency array, but, as the time required for
that is constant.

#####2.1.1.1.2 Tree building
After the table is constructed, the tree must be built. Here is the
pseudo code to build the tree.

    def huf_tree(freq_table):
        min_freq_priority_queue Q
        for (char, value) in freq_table:
            if value > 0:
                Q.insert(new node(char, value))
        while Q.size() > 1:
            lnode = Q.pop()
            rnode = Q.pop()
            root = new node(0, lnode.freq+rnode.freq)
            root.left = lnode
            root.right = rnode
        return Q.pop()

The code first creates a new binary tree node for each unique
character in the file, and inserts the nodes into a priority queue
which orders nodes by thier frequencies. Let's say the 'k' is the
number of uniqe characters in the file. Inserting into a priority
queue takes logarithmic time, so the first loop runs on the order of
klog(k) time.

After the nodes are inserted into the queue, the algorithm builds the
tree, starting at the bottom level. The nodes with the two lowest
frequencies are taken from the queue (logk time), and are set as the
children to a new 'dummy' node. The frequency of the dummy node is the
combined frequencies of its two children. This dummy is placed back
into the queue, and the process repeats.

At each iteration, there is one less node in the queue than before, so
this loop will run k times. Because inserting and removing from the
queue takes logk time, this loop takes order of klogk time.

When there is only one node left in the queue, that node is the root
node of the huffman tree, so we return it.

#####2.1.1.1.3 Indexing characters
Now that the tree has been constructed, we could go directly to
encoding the file, but then we would have to perform a DFS on the tree
for each character in the file in order to generate the appropriate
bit string for that character. To speed up compression, we first build
an index which maps characters to their pre-computed bit strings. Here
is the code.

    def gen_index(tree):
        index = new empty_array[256]
        gen_index_helper(tree.root, "", index)
        return index

    def gen_index_helper(node, bit_string, index):
        if node.is_leaf():
            index[node.char] = bit_string
        else
            gen_index_helper(node.left, bit_string+"0")
            gen_index_helper(node.left, bit_string+"1")

The code essentially performs a recursive DFS on the tree, starting at
the root. Every time a leaf node is reached, the value of the index is
updated with the bit string representing that node. This DFS, of
course, has to visit every node in the tree. We can say that the
number of nodes in the tree is upper bounded by the number of nodes in
a full binary tree with k leaf nodes, so this algorithm takes time on
the order of 2<sup>k</sup>.

####2.1.1.2 Second Pass
Here is where the actual encoding takes place. At this point, the tree
has been built and indexed, so all that is needed now is to go through
the file character by character and encode. Here's some pseudo code:

    def encode(index, file_in, file_out):
        bit_string encoded_data
        for char in file_in:
            encoded_data.append(index[char])
        file_out.write(encoded_data)

This code is simple; it keeps a bit string representing the encoded
file, and appends to it character by character. The loop runs in time
linear in the size of the file, the append operation takes O(1)
amortized time, and accessing the index also takes O(1) time, so this
code runs in time linear in the size of the file.


###2.1.2 Decoding
In some respects, decoding a file is a simpler process than encoding
it. For one, a tree-walking method can be used which means that no
index has to be built in order to decode a file. For another, only one
pass has to be made on the file because the tree should already have
been constructed during the encoding phase. This, however, leads to
the question of what is the best method of representing the tree
within the file.

####2.1.2.1 Tree Files
#####2.1.2.1.1 Saving the Trees
The problem of saving the tree to a file is, perhaps, a more
interesting problem than the initial problem of encoding the file.
This is because while encoding a file is a well-defined problem with
little room for lee-way, the different methods of saving the tree each
provide their own benefits and issues.  The methods I describe are
relativly simple to more advance compression techniques, but they
still comprise the basic assumptions underlying said techniques, so
they are worth investigating. A particularly interesting read for
further information is the
[RFC1951 standard](http://www.ietf.org/rfc/rfc1951.txt)
which defines the algorithm behind the DEFLATE algorithm.

There are two main approaches to saving the tree to a file. One, you
can save the frequency list; two, you can save a representation of the
tree itself. In this implementation, the tree itself was saved, but
there will be a brief discussion on saving the frequency list.

Further, there are two common options on where to save the tree: as a
seperate file, or as part of the encoded file. I choose to save it as
part of the encoded file, but, again, there will be a breif discussion
on the merits of both.

######2.1.2.1.1.1 Directly as a Tree
The benefits of saving a tree representation to file directly is that
the tree tends to be smaller for low-entropy input. For comparison,
the tree generated by this tool measures 88 bytes for Hamlet, while
the frequency list generated as a proof of concept measures 136 bytes.
For very high entropy input (pre-compressed binay data, for example)
my trees measured arounf 323 bytes. Comparatively, there is a way to
generate frequency lists to have a constant size of 256 bytes. There
is little point in trying to compress high-entropy files with this
tool, however, so the tree tends to fair better on the average case.

Here is the format of the tree:
The first 2 bytes are a 16-bit little-endian integer representing the
number of bytes the tree takes up. This is necissary because the tree
is prepended to the encoded file as a header. In this implementation,
the number is little endian by virtue of the processor, and not by
design. This, of course, leads to problems in portability in the code,
but I chose not to address those issues here.

The next n bytes are a bit string representing the tree. A bit of 0
represents an internal node (which, by the definiton of a binary tree,
always have two children), and a bit of 1 represents a leaf. The next
8 bits after a 1 represent the character that the leaf encodes.

Here is some code which takes a huffman tree and writes it to file:

    def to_file(tree):
        bitstring tree_str
        stack S

        S.push(tree.root)
        while !S.empty():
            node = S.pop()
            if node.is_leaf():
                tree_str.append(1)
                tree_str.append(node.char)
            else
                tree_str.append(0)
                S.push(node.right)
                S.push(node.left)
        return tree_str

This code is simply a stack implementation of a preorder DFS on the
huffman tree. Each iteration of the loop appends the correct value for
the current node to some output bit string. Because the DFS must visit
every node, and the number of nodes is bounded by the number of nodes
in a full binary tree with k leaf nodes (where k is the number of
unique characters in the tree), then this code runs in time on the
order of 2<sup>k</sup>.

######2.1.2.1.1.2 Frequency List
In a frequency list, the frequency of each character is saved to the
file, and the tree is constructed on the spot using these frequencies. There are two simple ways to do it which I tested.

The first way is to save two bytes for each unique character the tree
encodes. The first byte is the frequency of the character, the second
byte is the character to be encoded. This takes 2k+1 space. The extra
byte being to denote the number of bytes the frequency list holds.

The second way is to always allocate 256 bytes at the start of the
file (one for each possible character), and record zeros where a
character does not exist in the tree. It somewhat wasteful for small
files as many entries might end up zero, but could be useful for large
files.

These methods have the problem that the frequency of each character
cannot be greater than 255: an unlikely situation for large files. I
can think of two ways to circumvent this.

One way to get around this is to normalize the frequencies so that
they stay within the range [0-255]. As I did not end up using this
method, I did not test or prove that the normalization would work
correctly, But I can say that some data sets exist where normalization
would lose enough data in the range between integers to jeopordize the integrety of the tree.

Another, simpler, way of getting around this limitation is to simply
use larger integers for the frequency counts. This, of course, greatly
increases the size of the list.

######2.1.2.1.1.3 Multiple Files vs. Single Files
My initial implementations of the tools used seperate files to store
the trees. Doing this simplified the code and saved two bytes in the
output file; however, it soon became cumbersome to manage such a large
number of files the tool was producing. I implemented headers
initially to test performance on compressing files multiple times, but
I stuck with the method in the end.

One possible advantage of saving trees to seperate files is that you
can distribute the tree seperately from the encoded file. One
application is that you could build a tree based on a frequency
analysis of modern english language, and use it to encode all text
passed between parties. Compression ratios would likely be
sub-optimal, but might be good enough for most purposes.

#####2.1.2.1.2 Reading the Tree
Now that it has been established how the tree is formatted, we can see how the tree can be read. Pseudo code:

    def read_tree_str_helper(cur_node, bitstr, pos):
        bit = bitstr.get_bit(pos)
        pos += 1
        if bit == 0:
            pos = read_tree_str_helper(cur_node.left, bitstr, pos)
            pos = read_tree_str_helper(cur_node.right, bitstr, pos)
        else if bit == 1:
            cur_node.char = bitstr.get_byte()
            pos += 8
        return pos

    def read_tree_str(bitstr):
        tree = new huf_tree()
        read_tree_str_helper(tree.root, bitstr, 0)

Essentially, the code performs a postorder DFS on the bitstring
representation of the tree, and reads it into an internal data
structure. Again, the code must touch every node in the tree, and the
number of nodes is bounded by the number of nodes in a complete binary
tree with k leaf nodes where k is the number of unique characters in
the original file. In the end, this code runs in time on the order of
2<sup>k</sup>.

####2.1.2.1.3 The Tree-Walking Method
Now that the tree has been read, it is possible to decode the file. Thepseudo code is as follows:

    def decode(tree, file):
        string output;
        cur = tree.root
        for bit in file.bits:
            if bit == 0:
                cur = cur.left
            else if bit == 1:
                cur = cur.right
            
            if cur.is_leaf():
                output.append(cur.char)
                cur = tree.root
        return output

The code starts with a refrence to the root and takes either the left
or right child depending on whether the next bit is 0 or 1. Once it
reaches a leaf node, it outputs the character connected to that leaf
node and begins its walk again.  It must loop through every bit in the
file, so the runtime is linear in the size of the file.


##2.2. Bit Vectors
Initial implementations used strings of ASCII 1's and 0's instead of
binary bitstrings. When the functionality of the code was complete,
analysis showed certain methods being used on the pseudo bit strings
which I could implement in a real bit string class.

+ Bits needed to be individually accessible, but not necissarily
  mutable.
+ Bit strings only needed to be changed by appending either a bit, a
  byte, or another bit string.
+ The number of used bits in the string needed to accessible.
+ The underlying byte-aligned storage mechanism needed to be
  accessible for writing to a file.

I wrote a class, bitvector, which provided all of the above
functionality and which worked more or less like a drop in replacement
for character strings. C++ does not allow one to access bits
individually in a byte, so, in order to get the full functionality, I
needed to use several bitwise operations. These operations, while
fast, do incur a bit of extra overhead compared to using character
strings, but are neccissary for my implementation.

One major difference between using bit strings vs. regular strings is
that, because the storage medium is byte aligned, there can possibly
be several bits at the end of the bit string which are unused. get
around this, all my file-writing and string-constructing methods for
bitvector prepended the number of unused bits to the string.

#3. Results
File Name     Original  Compressed  Ratio
hamlet:       182581    111802      61.234%
juliuscaesar: 118048    71910       60.916%
macbeth:      105405    64345       61.045%
othello:      156449    95358       60.951%

These results show reasonable performance of this implementation. I
also tested the program on multiple files of varying types. Here are
some average values for compression ratios:

File Type                               Approximate Ratio
ZIP files                               98%
Text files from various short stories   65%
Random data from /dev/urandom           110%
RTF file with large embeded images      40%

Most interesting are the last two. The random data became larger after
compression while the RTF file got a surprisingly low compression
ratio.

After studying the files, I believe that the random data had such a
high entropy, that no reasonable tree could be constructed. At that
point the overhead of storing the tree and other data was enough to
push the file to be slightly larger than the original. A similar situation is likely occuring with the ZIP file.

As for the RTF file, I viewed it under a HEX editor and a text editor,
and the large embeded files were being stored in a raw ascii-based
format. The images themselves consisted of large, single-color areas,
so there were a great many bytes of the same type which is just the
situation where huffman encoding workds very well.

##3.1 Possible Directions
There are a few ideas which I would have liked to implement but which
I could not due to time constraints. The first is a block encoding
scheme which breaks files into several blocks and builds trees for
each block. I find that, in some situations, large sections of files
can have a different freqeuncy table than the rest of the file, and it
might be possible to use this to better compress the data.

Another possible extension is to add the ability to compress streams
of data instead of block files. Because the algorithm calls for a
two-pass approach, the approch to a stream compression cannot be
direct. An easy way to accomplish this is to simply read all of the
data into memory before going about the standard two-pass approach,
but this negates the most important property of stream compression
which is the ability to handle files larger than memory can allow.
Another way to go is to use the proposed block compression to handle
streams block by block. 

Perhaps it is even possible to predict the frequency characters in the
stream. Of course, any such predictions are likely to be sub-optimal,
but they might be useful in certain circumstances.
