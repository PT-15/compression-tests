#include "debug.h"

#include <stdio.h>
#include <string>
#include <fstream>

void dbg::msg (std::string msg)
{
    printf("%s\n", msg.c_str());
    fflush(stdout);
}

// Trees

void dbg::print_tree(Node *root)
{
    if (root->is_leaf()) {
        Leaf* leaf = (Leaf*) root;
        printf("Leaf %d: %c\n", root->get_frequency(), leaf->get_element());
    }
    else {
        Group* node = (Group*) root;
        printf("Node %d: %d %d\n", root->get_frequency(), node->get_left_child()->get_frequency(), node->get_right_child()->get_frequency());
        dbg::print_tree(node->get_left_child());
        dbg::print_tree(node->get_right_child());
    }
}

void dbg::print_tree_map(Node *root, std::map<void*,int> &index_map)
{
    if (root->is_leaf()) {
        Leaf* leaf = (Leaf*) root;
        printf("Leaf %d (%p): %c\n", index_map[root], root, leaf->get_element());
    }
    else {
        Group* node = (Group*) root;
        printf("Node %d (%p): %d %d\n", index_map[root], root, index_map[node->get_left_child()], index_map[node->get_right_child()]);
        dbg::print_tree_map(node->get_left_child(), index_map);
        dbg::print_tree_map(node->get_right_child(), index_map);
    }
}

std::string print_graph (Node *root, std::ofstream &output)
{
    static int idx = 0;
    int curr_idx = idx;
    idx++;

    if (root->is_leaf()) {
        Leaf *leaf = (Leaf*) root;
        std::string leaf_id = "\"" + std::to_string(curr_idx)
                          + ":" + std::to_string(root->get_frequency())
                          + ":" + leaf->get_element() + "\"";
        return leaf_id;
    }
    else {
        Group *node = (Group*) root;
        std::string node_id = "\"" + std::to_string(curr_idx) + ":" +
                          std::to_string(root->get_frequency()) + "\"";
        output << node_id << " -> " << print_graph(node->get_left_child(), output) << ";\n";
        output << node_id << " -> " << print_graph(node->get_right_child(), output) << ";\n";

        return node_id;
    }
}

void dbg::graph_to_img (Node* root)
{
    static int img = 0;
    std::ofstream output;
    output.open("graph_" + std::to_string(img) + ".gv");
    img++;

    output << "digraph G {\n";
    print_graph(root, output);
    output << "}\n";
}

std::string print_graph_with_code (Node *root, std::ofstream &output, std::map<char, std::pair<long long,char>> code)
{
    static int idx = 0;
    int curr_idx = idx;
    idx++;

    if (root->is_leaf()) {
        Leaf *leaf = (Leaf*) root;
        printf("%c: %lld\n", leaf->get_element(), code[leaf->get_element()].first);
        std::string leaf_id = "\"";
        leaf_id.push_back(leaf->get_element());
        leaf_id += ":" + std::to_string((int)code[leaf->get_element()].first) + "\"";
        return leaf_id;
    }
    else {
        Group *node = (Group*) root;
        std::string node_id = "\"" + std::to_string(curr_idx) + "\"";
        output << node_id << " -> " << print_graph_with_code(node->get_left_child(), output, code) << ";\n";
        output << node_id << " -> " << print_graph_with_code(node->get_right_child(), output, code) << ";\n";

        return node_id;
    }
}

void dbg::graph_to_img_with_code (Node* root, std::map<char,std::pair<long long,char>> code)
{
    static int img = 0;
    std::ofstream output;
    output.open("graph_" + std::to_string(img) + ".gv");
    img++;

    output << "digraph G {\n";
    print_graph_with_code(root, output, code);
    output << "}\n";
}
