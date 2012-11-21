#ifndef HUFFMAN_NODE_HPP
#define HUFFMAN_NODE_HPP

#ifndef NULL
# define NULL 0
#endif

class huffman_node
{
  public:
    huffman_node *left;
    huffman_node *right;

    char chr;
    int freq;

    huffman_node(char ch='\0')
    {
        left = NULL;
        right = NULL;
        freq = 0;
        chr = ch;
    }

    bool operator <(huffman_node& other)
    {
        return (this->freq < other.freq);
    }
};

#endif
