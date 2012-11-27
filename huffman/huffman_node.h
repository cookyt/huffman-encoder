#ifndef HUFFMAN_NODE_H
#define HUFFMAN_NODE_H
#include <string>
#include "util/bitvector.h"

class huffman_node
{
  public:
    huffman_node *left;
    huffman_node *right;

    char ch;
    int freq;
    bitvector bits;

    huffman_node(int frequency=0, char chr='\0');
};

#endif
