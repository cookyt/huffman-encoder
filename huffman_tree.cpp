#include <stdio.h>
#include <string.h>
#include "huffman_tree.hpp"

using namespace std;

// Performs a DFS on the tree, deleting its nodes.
huffman_tree::~huffman_tree()
{
}

// Goes over the file and counts the frequency of each byte.
// fin - the file to read from
// out - [out] an array of at least 255 ints
void huffman_tree::freq_list(FILE *fin, int *out)
{
    bzero(out, 255*sizeof(int));
    for (;;)
    {
        char c = fgetc(fin);
        if (feof(fin))
            break;
        out[c]++;
    }
}

// Constructs a huffman tree from any given input file.
// fin - the file to read from.
huffman_tree huffman_tree::from_input_file(FILE *fin)
{
    huffman_tree tree;

    int frequencies[255];
    freq_list(fin, frequencies);

    return tree;
}

// Constructs a huffman tree from a file containing the contents of
// huffman_tree::to_string.
// fin - the file to read from
huffman_tree huffman_tree::from_tree_file(FILE *fin)
{
    huffman_tree tree;
    return tree;
}

// Constructs a huffman tree from a string containing the contents of
// huffman_tree::to_string.
huffman_tree huffman_tree::from_string(string sin)
{
    huffman_tree tree;
    return tree;
}

// Encodes a file with this huffman tree. Returns a string of ASCII 1's and 0's
// representing the encoded file. The method reads until EOF.
// fin - the file to read from
string huffman_tree::encode(FILE *fin)
{
}

// Returns a string representation of this huffman tree. This string can be fed
// into huffman_tree::from_string to construct a new tree.
string huffman_tree::to_string()
{
}
