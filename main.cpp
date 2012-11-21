#include <set>
#include <iostream>
#include "huffman_node.hpp"

int main()
{
    huffman_node one;
    one.freq = 10;

    huffman_node two;
    two.freq = 0;
    
    std::cout << (one < two) << std::endl;
    return 0;
}
