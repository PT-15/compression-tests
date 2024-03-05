#include "dictionary.h"

#include <bitset>
#include <fstream>
#include <map>
#include <queue>
#include <unistd.h>
#include <queue>

#include "files.h"
#include "node.h"

#define KEY first
#define LEN second

typedef std::map<char,std::pair<int,char>> encoder_map;

void get_frequencies(const int fd, std::map<char,int> &dict)
{
    char element;

    while (read(fd, &element, 1) > 0) {
        dict[element]++;
    }
}

void create_leaves (std::priority_queue<Node*, std::vector<Node*>, Node::Compare>& nodes, const std::map<char,int>& dict)
{
    for (std::pair<char,int> p : dict) {
        Leaf *leaf = new Leaf(p.second, p.first);
        nodes.push(leaf);
    }
}

void create_group_node (std::priority_queue<Node*, std::vector<Node*>, Node::Compare>& nodes)
{
    Node *left = nodes.top();
    nodes.pop();
    Node *right = nodes.top();
    nodes.pop();
    Group *parent = new Group(
            left->get_frequency() + right->get_frequency(),
            left,
            right
            );
    nodes.push(parent);
}

Node* create_tree (const std::map<char,int> &dict)
{
    std::priority_queue<Node*, std::vector<Node*>, Node::Compare> nodes;
    create_leaves(nodes, dict);

    while (nodes.size() > 1) {
        create_group_node(nodes);
    }

    return nodes.top();
}

void get_code (std::pair<int,char> code, encoder_map& code_map, const Node* current)
{
    if (current->is_leaf()) {
        Leaf *leaf = (Leaf*) current;
        code_map[leaf->get_element()] = code;
        return;
    }

    Group *node = (Group*) current;
    if (node->get_left_child() != NULL) {
        get_code({code.KEY << 1, code.LEN+1}, code_map, node->get_left_child());
    }
    if (node->get_right_child() != NULL) {
        get_code({(code.KEY << 1) + 1, code.LEN+1}, code_map, node->get_right_child());
    }
}


// file: file from which to generate the code
// code: empty map to which the code is going to be written
void create_dict(const std::string file, encoder_map& code)
{
    int in_fd, out_fd;
    open_files(file, file + ".dic", in_fd, out_fd); // [dic]tionary file

    std::map<char,int> dict;
    get_frequencies(in_fd, dict);

    Node *root = create_tree(dict);
    get_code({0,0}, code, root);

    close(in_fd);
    close(out_fd);
}
