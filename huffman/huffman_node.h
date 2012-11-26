#ifndef HUFFMAN_NODE_H
#define HUFFMAN_NODE_H
#include <string>

class huffman_node
{
  public:
    huffman_node *left;
    huffman_node *right;

    char ch;
    int freq;
    std::string bits;

    huffman_node(int frequency=0, char chr='\0');
};

#endif
