#include <stack>
#include <cassert>
#include "huffman_node.hpp"

#ifndef NULL
# define NULL 0
#endif

huffman_node::huffman_node(int frequency, char chr)
{
    left = NULL;
    right = NULL;
    freq = frequency;
    ch = chr;
}


// Helper function for huffman_tree::to_string, appends to an output string,
// the string formed 
std::string huffman_node::to_string()
{
    using namespace std;
    string output = "";
    stack<huffman_node *> S;

    S.push(this);

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

bool operator<(const huffman_node& lhs, const huffman_node& rhs)
{
    return (lhs.freq > rhs.freq);
}
