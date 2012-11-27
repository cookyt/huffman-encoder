#ifndef HUFFMAN_NODE_H
#define HUFFMAN_NODE_H
#include <string>
#include "util/bitvector.h"

// Represents a single node in a huffman tree.
// A node is internal if neither of its children are NULL.
class huffman_node
{
  public:
    huffman_node *left;
    huffman_node *right;

    //The character contained in this node
    char ch;

    //Frequency of this character (doesn't get set if tree is read from a tree
    //file)
    int freq;

    //Cached bitstring represented by this node
    bitvector bits;

    huffman_node(int frequency=0, char chr='\0');
};

#endif
