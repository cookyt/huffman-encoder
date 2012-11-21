#include <queue>
#include <stack>
#include <vector>

#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "huffman_tree.hpp"

using namespace std;

// Performs a DFS on the tree, deleting its nodes.
huffman_tree::~huffman_tree()
{
    stack<huffman_node *> S;
    S.push(root);

    //DFS loop
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
            // For a huffman tree, either both nodes are NULL or both nodes are not
            // NULL, there can't be one NULL and another not NULL.
            assert(node->left == NULL && node->right == NULL);
        }

        delete node;
    }
}

// Goes over the file and counts the frequency of each byte.
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

// Comparator for two 
struct HuffLess
{
    bool operator()(huffman_node *p1, huffman_node *p2) const 
    {
        return (*p1 < *p2);
    }
};

// Constructs a huffman tree from any given input file.
// fin - the file to read from.
huffman_tree huffman_tree::from_input_file(FILE *fin)
{
    huffman_tree tree;

    int freq[256];
    freq_list(fin, freq);

    priority_queue<huffman_node *, vector<huffman_node *>,HuffLess> Q;
    for (int i=0; i<256; i++)
    {
        if (freq[i] == 0)
            continue;
        huffman_node *node = new huffman_node(freq[i], i);
        Q.push(node);
    }

    while (Q.size() > 1)
    {
        huffman_node *lnode = Q.top();
        Q.pop();
        huffman_node *rnode = Q.top();
        Q.pop();
        huffman_node *node = new huffman_node(lnode->freq + rnode->freq);
        node->left = lnode;
        node->right = rnode;
        Q.push(node);
    }

    huffman_node *node = Q.top();
    tree.root = node;

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

// Encodes a file with this huffman tree. Returns a string of ASCII 1's and 0's
// representing the encoded file. The method reads until EOF.
// fin - the file to read from
string huffman_tree::encode(FILE *fin)
{
}

// Decodes a file using this huffman tree. Quits if an impossible bit string is
// found.
// fin - the file to read from
string huffman_tree::decode(FILE *fin)
{
}

// Returns a string representation of this huffman tree. This string can be fed
// into huffman_tree::from_string to construct a new tree.
// The EBNF of this string is
// ==========================
// Tree ::= N | E
// Node ::= B[C]
// Children ::= NN | E
// Byte ::= (a single byte)
// Empty ::= (the empty string)
string huffman_tree::to_string()
{
    string output = "";
    stack<huffman_node *> S;

    S.push(root);

    // Used to mark when to print a closing bracket in the DFS stack.
    huffman_node sentinal(-1, 0);

    //DFS loop
    while (!S.empty())
    {
        huffman_node *node = S.top();
        S.pop();

        if (node == &sentinal)
        {
            output += ']';
            continue;
        }

        output += node->ch;
        output += '[';

        if (node->left != NULL && node->right != NULL)
        {
            S.push(&sentinal);
            S.push(node->right);
            S.push(node->left);
        }
        else
        {
            // For a huffman tree, either both nodes are NULL or both nodes are not
            // NULL, there can't be one NULL and another not NULL.
            assert(node->left == NULL && node->right == NULL);
        }
    }

    return output;
}
