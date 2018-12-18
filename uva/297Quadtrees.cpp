
#include <iostream>
#include <vector>

#include <stdlib.h>

using namespace std;

template<typename T>
struct KNode {
    vector<KNode *> children;
    int curr_pos;
    T data;

    KNode(const int &n) {
        children.resize(n);
        for (int i = 0; i < n; ++i) children[i] = nullptr;
        curr_pos = 0;
    }

    KNode(const int &n, const T &_data): data(_data) {
        children.resize(n);
        for (int i = 0; i < n; ++i) children[i] = nullptr;
        curr_pos = 0;
    }

    KNode(const vector<KNode *> &_children)  {
        children = _children;
        curr_pos = _children.size();
    }

    inline void append(const vector<KNode *> &_children) { children = _children; }
    inline void append(const KNode son) { children[curr_pos++] = son; }

};

// helper function
KNode<char> *_deserialize(const string &src, int &pos) {
    if (pos >= src.size()) return nullptr;
    vector<KNode<char> *> children(4);
    for (int i = 0; i < 4; ++i) {
        if (src[pos] == 'p') 
            children[i] = _deserialize(src, ++pos);
        else {
            children[i] = new KNode<char>(4, src[pos]);
            ++pos;
        }
    }
    auto node = new KNode<char>(children);
    node->data = 'p';
    return node;
}

inline KNode<char> *deserialize(const string &src) {
    int zero = 0;
    return _deserialize(src, zero);
}

/**
 * ee
 * ep
 * pe
 * pp 
 * 
 * ff
 * ef
 * fe
 * fp
 * pf
 */
KNode<char> *interpolate_quadtrees(KNode<char> *&a, KNode<char> *&b) {  
    vector<KNode<char> *> children(4);
    for (int i = 0; i < 4; ++i) {
        char x = a->children[i]->data; 
        char y = b->children[i]->data; // aliases
        if (x == 'p' && y == 'p')
            children[i] = interpolate_quadtrees(a->children[i], b->children[i]);
        else if (x == 'f') children[i] = a->children[i];
        else if (y == 'f') children[i] = b->children[i];
        else if (x == 'p') children[i] = a->children[i];
        else if (y == 'p') children[i] = b->children[i];
        else children[i] = a->children[i]; // x is e and y is e
    }    
    auto node = new KNode<char>(children);
    node->data = 'p';
    return node;
}

int sum_pixels( KNode<char> *&root, int amount = 1024) {
    int sum = 0;
    for (auto x: root->children) {
        if (x == nullptr) break;
        if (x->data == 'p') sum += sum_pixels(x, amount >> 2);
        if (x->data == 'f') sum += amount;
    }
    return sum;
}

void test() {
    string s1, s2;
    KNode<char> *tree1, *tree2, *result;

    //string s1 = "ppeeefpffeefe", s2 = "pefepeefe";
    //string s1 = "peeef", s2 = "peefe";
    //string s1 = "peeef", s2 = "peepefefe";

    s1 = "ppeeefpffeefe", s2 = "pefepeefe";
    tree1 = deserialize(s1);
    tree2 = deserialize(s2);
    result = interpolate_quadtrees(tree1, tree2);
    cout << sum_pixels(result) << endl;

    s1 = "peeef", s2 = "peefe";
    tree1 = deserialize(s1);
    tree2 = deserialize(s2);
    result = interpolate_quadtrees(tree1, tree2);
    cout << sum_pixels(result) << endl;

    // s1 = "peeef", s2 = "peepefefe";
    // tree1 = deserialize(s1);
    // tree2 = deserialize(s2);
    // result = interpolate_quadtrees(tree1, tree2);
    // cout << sum_pixels(result) << endl;

}

void ask_parameters();

int main(int argc, char const *argv[]) {
    test();
    //ask_parameters();
    return 0;
}

void ask_parameters() {
    int n;
    cin >> n;
    while (n--) {
        string s1, s2;
        cin >> s1;
        cin >> s2;
        auto tree1 = deserialize(s1);
        auto tree2 = deserialize(s2);
        auto result = interpolate_quadtrees(tree1, tree2);
        cout << "There are " << sum_pixels(result) << " black pixels." << endl;
    }
}

// ppeeefpffeefe
// pefepeefe

// ppeeefffpeefe