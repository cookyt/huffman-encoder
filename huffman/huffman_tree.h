#ifndef HUFFMAN_TREE_H
#define HUFFMAN_TREE_H

#include <string>
#include "util/bitvector.h"

class huffman_node;

class huffman_tree
{
  public:
    // The root of this tree
    huffman_node *root;

    // Performs a DFS on the tree, deleting its nodes.
    ~huffman_tree();

    // Constructs a huffman tree from any given input file.
    //
    // fin - the file to read from.
    static huffman_tree from_input_file(FILE *fin);

    // Constructs a huffman tree from a string containing the contents of
    // huffman_tree::to_string.
    static huffman_tree from_tree_string(std::string tree_str);

    // Encodes a file with this huffman tree.  Returns a bitvector representing
    // the encoded data. The method reads until EOF.
    //
    // fin - the file to read from
    bitvector encode(FILE *fin);

    // Decodes a file using this huffman tree.  Quits if an impossible bit
    // string is found.
    // 
    // fin - the file to read from
    std::string decode(FILE *fin);

    // Returns a string representation of this huffman tree. This string can be
    // saved and fed into huffman_tree::from_tree_string  to construct a new
    // tree.
    std::string to_string();

  private:
    // An index mapping characters to their corresponding bit strings
    bitvector *index[256];

    // Constructs an empty tree w/ empty index and NULL root.
    huffman_tree();

    // Goes over the file and counts the frequency of each byte.
    //
    // fin - the file to read from
    // out - [out] an array of at least 256 ints
    static void freq_list(FILE *fin, int *out);

    // Builds an index of characters to bitvectors to aid in the encoding.
    void build_index();

    // Recursive parsing helper method. Reads a single node into the tree, and
    // recursively adds its children too. It can be thought of the helper
    // method to a DFS.
    //
    // node - pointer to where this node will be referenced in the tree.
    // data - character buffer containing the huffman tree
    // pos - the position of the next character in the buffer (initially 0)
    // str_len - the number of characters in the buffer
    static void readNode(huffman_node * &node, bitvector &data, int &pos,
                         int str_len);
};

#endif
