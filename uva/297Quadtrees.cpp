
#include <iostream>
#include <vector>
#include <array>

#include <stdio.h>
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

template<typename T> 
void preorder(KNode<T> *node) {
    if (node == nullptr) {
        //printf("nil-");
    } else {
        cout << "" << node->data << "-";
        for (int i = 0; i < node->children.size(); ++i) {
            preorder(node->children[i]);
        }
    }
}

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
    return _deserialize(src, zero)->children[0];
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
        char x = a->children[i]->data, y = b->children[i]->data; // aliases
        if (x == 'p' and y == 'p')
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
    if (root->data == 'p') {
        for (auto x: root->children) {
            if (x == nullptr) continue;
            sum += sum_pixels(x, amount >> 2);
        }
    } 
    if (root->data == 'f') sum += amount;
    return sum;
}

template<typename T> 
void delete_ktree(KNode<T> *&node) {
    if (node == nullptr) return;
    for (int i = 0; i < node->children.size(); ++i)
        delete_ktree(node->children[i]);
    delete node;
}

void test() {
    string s1, s2;
    array<KNode<char> *, 3> trees;

    //string s1 = "ppeeefpffeefe", s2 = "pefepeefe";
    //string s1 = "peeef", s2 = "peefe";
    //string s1 = "peeef", s2 = "peepefefe";

    s1 = "ppeeefpffeefe", s2 = "pefepeefe";
    trees[0] = deserialize(s1);
    cout << sum_pixels(trees[0]) << endl;
    trees[1] = deserialize(s2);
    cout << sum_pixels(trees[1]) << endl;
    trees[2] = interpolate_quadtrees(trees[0], trees[1]);
    cout << sum_pixels(trees[2]) << endl;

    delete_ktree(trees[0]);

    for (int i = 0; i < 3; ++i) {
        cout << endl;
        preorder(trees[i]);
        cout << endl;
    }

    s1 = "peeef", s2 = "peefe";
    trees[0] = deserialize(s1);
    trees[1] = deserialize(s2);
    trees[2] = interpolate_quadtrees(trees[0], trees[1]);
    cout << sum_pixels(trees[2]) << endl;

    // s1 = "peeef", s2 = "peepefefe";
    // trees[0] = deserialize(s1);
    // trees[1] = deserialize(s2);
    // trees[2] = interpolate_quadtrees(trees[0], trees[1]);
    // cout << sum_pixels(trees[2]) << endl;

}

int main(int argc, char const *argv[]) {
    test();
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
        delete_ktree(tree1);
        delete_ktree(tree2);
        delete_ktree(result);
    }
}

/*

3
ppeeefpffeefe
pefepeefe
peeef
peefe
peeef
peepefefe

*/