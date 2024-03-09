#pragma once

#include <map>
#include <string>
#include <iostream>
#include <vector>
#include <bitset>

#include "node.h"

namespace dbg {

    void msg (std::string msg);

    template<typename T> void value (std::string name, T val)
    {
        std::cout << name << " is: " << val << std::endl;
    }

    template<typename T> void bits (std::string name, T val) {
        std::bitset<sizeof(val)*8> b (val);
        std::cout << name << " is: " << b << std::endl;
    }

    // Vectors
    template<typename T> void print_vector (std::vector<T> v)
    {
        for (T element : v) {
            std::cout << element << " ";
        }
        std::cout << std::endl;
    }
    template<typename T> void print_pairs_vector(std::vector<T> pairs)
    {
        for (T pair : pairs)
            std::cout << pair.first << " " << pair.second << "\n";
    }

    // Graphs
    void print_tree (Node* root);
    void print_tree_map (Node* root, std::map<void*,int> &index_map);
    void graph_to_img (Node* root);
    void graph_to_img_with_code (Node* root, std::map<char,std::pair<long long,char>> code);
}
