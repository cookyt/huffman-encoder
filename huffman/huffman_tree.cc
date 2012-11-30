#include "huffman_tree.h"

#include <cassert>
#include <cstdio>
#include <cstring>
#include <stdint.h>
#include <queue>
#include <stack>
#include <vector>

#include "huffman_node.h"

using namespace std;

// Constructs an empty tree w/ empty index and NULL root.
huffman_tree::huffman_tree()
{
    bzero(index, 256*sizeof(*index));
    root = NULL;
}

    // Performs a DFS on the tree, deleting its nodes.
huffman_tree::~huffman_tree()
{
    stack<huffman_node *> S;
    S.push(root);
    while (!S.empty())
    {
        huffman_node *node = S.top();
        S.pop();

        if (node->left != NULL && node->right != NULL)
        {
            S.push(node->right);
            S.push(node->left);
        }
        else
        {
            // For a huffman tree, either both nodes are NULL or both
            // nodes are not NULL, there can't be one NULL and another not
            // NULL.
            assert(node->left == NULL && node->right == NULL);
        }

        delete node;
    }
}

// Goes over the file and counts the frequency of each byte.
//
// fin - the file to read from
// out - [out] an array of at least 256 ints
void huffman_tree::freq_list(FILE *fin, int *out)
{
    bzero(out, 256*sizeof(*out));
    for (;;)
    {
        unsigned char c = fgetc(fin);
        if (feof(fin))
            break;
        out[c]++;
    }
}

// Builds an index of characters to bitvectors to aid in the encoding.
// Traverses the tree with a DFS, and sets the bits member of each node to
// a bitvector containing the encoded bitvector for that node in the
// context of the tree.
void huffman_tree::build_index()
{
    bzero(index, 256*sizeof(*index));

    if (root == NULL)
        return;

    // DFS on the tree, and build an array index mapping chars to bit strings
    stack<huffman_node *> S;
    S.push(root);
    while (!S.empty())
    {
        huffman_node *node = S.top();
        S.pop();

        if (node->left != NULL && node->right != NULL)
        {
            node->right->bits += node->bits;
            node->left->bits += node->bits;

            node->right->bits += true;
            node->left->bits += false;

            S.push(node->right);
            S.push(node->left);
        }
        else
        {
            assert(node->left == NULL && node->right == NULL);
            index[(unsigned char)node->ch] = &(node->bits);
        }
    }
}

// Comparator used in the priority queue implementation of
// huffman_tree::from_input_file
struct huffman_tree::HuffLess
{
    bool operator()(huffman_node *p1, huffman_node *p2) const 
    {
        return (p1->freq > p2->freq);
    }
};

// Constructs a huffman tree from any given input file. It does this by
// building a frequency list for the file and building the huffman tree
// from that. It also builds the index when it's done.
//
// fin - the file to read from.
huffman_tree huffman_tree::from_input_file(FILE *fin)
{
    huffman_tree tree;

    int freq[256];
    freq_list(fin, freq);

    priority_queue<huffman_node*, vector<huffman_node *>, HuffLess> Q;
    for (int i=0; i<256; i++)
    {
        if (freq[i] == 0)
            continue;
        huffman_node *node = new huffman_node(freq[i], i);
        Q.push(node);
    }

    while (Q.size() > 1)
    {
        huffman_node *lnode = Q.top(); Q.pop();
        huffman_node *rnode = Q.top(); Q.pop();
        huffman_node *node = new huffman_node(lnode->freq + rnode->freq);
        node->left = lnode;
        node->right = rnode;
        Q.push(node);
    }

    huffman_node *node = Q.top();
    tree.root = node;
    tree.build_index();

    return tree;
}

// Recursive parsing helper method. Reads a single node into the tree, and
// recursively adds its children too. It can be thought of the helper
// method to a DFS.
//
// node - pointer to where this node will be referenced in the tree.
// data - character buffer containing the huffman tree
// pos - the position of the next character in the buffer (initially 0)
// str_len - the number of characters in the buffer
void huffman_tree::readNode(huffman_node * &node, bitvector &data, int &pos,
                            int str_len)
{
    assert (pos < str_len);
    bool b = data[pos++];

    node = new huffman_node();
    switch (b)
    {
        case false:
            readNode(node->left, data, pos, str_len);
            readNode(node->right, data, pos, str_len);
            break;
        case true:
            assert (pos < str_len);
            node->ch = data.char_at(pos);
            pos += 8;
            break;
        default:
            assert(false);
    }
}

// Constructs a huffman tree from a string containing the contents of
// huffman_tree::serialize. It also builds the index when it's done.
huffman_tree huffman_tree::from_tree_string(string tree_str)
{
    huffman_tree tree;

    bitvector data(tree_str);
    if (tree_str.size() > 1)
    {
        // assert (data[0] == 'b');
        int pos=0;
        readNode(tree.root, data, pos, data.size());
    }

    tree.build_index();
    return tree;
}

// Encodes a file with this huffman tree.  Returns a bitvector representing
// the encoded data. The method reads until EOF.
//
// fin - the file to read from
bitvector huffman_tree::encode(FILE *fin)
{
    bitvector output;
    for (;;)
    {
        unsigned char c = fgetc(fin);
        if (feof(fin))
            break;
        assert(index[c] != NULL);
        output += *(index[c]);
    }
    return output;
}

// Decodes a file using this huffman tree.  Quits if an impossible bit
// string is found.
// 
// fin - the file to read from
string huffman_tree::decode(FILE *fin)
{
    string output = "";
    bitvector input(fin);
    huffman_node *cur = root;
    for (int i=0; i<input.size(); i++)
    {
        assert(cur != NULL);
        switch (input[i])
        {
            case false:
                cur = cur->left;
                break;
            case true:
                cur = cur->right;
                break;
            default:
                assert(false);
        }
        assert(cur != NULL);
        if (cur->left == NULL && cur->right == NULL)
        {
            output += cur->ch;
            cur = root;
        }
        else
        {
            assert(cur->left != NULL && cur->right != NULL);
        }
    }
    return output;
}

// Returns a string representation of this huffman tree. This string can be
// saved and fed into huffman_tree::from_tree_string  to construct a new
// tree.
string huffman_tree::serialize()
{
    bitvector output;
    stack<huffman_node *> S;

    S.push(root);

    //DFS loop
    while (!S.empty())
    {
        huffman_node *node = S.top();
        S.pop();

        if (node == NULL)
            continue;
        
        if (node->left != NULL && node->right != NULL)
        {
            output += false;
        }
        else
        {
            output += true;
            output += node->ch;
        }

        S.push(node->right);
        S.push(node->left);
    }

    string str_out = output.to_string();
    uint16_t len = str_out.size();
    str_out.insert(0, (const char *) &len, sizeof(len));
    return str_out;
}
