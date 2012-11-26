#include "huffman_node.h"

#ifndef NULL
# define NULL 0
#endif

huffman_node::huffman_node(int frequency, char chr)
{
    left = NULL;
    right = NULL;
    freq = frequency;
    ch = chr;
    bits = "";
}


bool operator<(const huffman_node& lhs, const huffman_node& rhs)
{
    return (lhs.freq > rhs.freq);
}
