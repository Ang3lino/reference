
#include <iostream> // input output stream
#include <string>
#include <stdlib.h> // ssize_t type

using namespace std; // by using this we save time in writing std:: everytime

// every method an propierty of a struct is public by default
struct Rational {
    ssize_t num, den; // signed long long int

    // constructor, every method inside a struct is public by default
    Rational(): num(0), den(1) {  } 
    /** equivalen to 
     * num = 0;
     * den = 1;
     */

    Rational(const ssize_t &num, const ssize_t &den): 
        num(num), 
        den(den) {  } 

    bool is_zero() { return num == 0 && den != 0; }

    // by using & we obtain the value by reference
    Rational(const Rational &other): num(other.num), den(other.den) {  } 
};

template<typename T> // this is a generic structure, "generics" in java
struct BinaryNode {
    BinaryNode<T> *left, *right; // we must use a pointer if we wanna use the same structure inside the same in c++
    T elem;

    // nullptr keyword is an improvement of c++11 of NULL
    BinaryNode(): left(nullptr), right(nullptr) {  }

    BinaryNode(T data): left(nullptr), right(nullptr), elem(data) {  }

    BinaryNode(T data, BinaryNode<T> izq, BinaryNode<T> dcha): 
        elem(data), left(izq), right(dcha) {  }

    BinaryNode(BinaryNode<T> *izq, BinaryNode<T> *dcha): left(izq), right(dcha) {  }
};

BinaryNode<Rational> *build_tree(Rational &ans, Rational &left, 
        Rational &right, int i, const string &path) {
    Rational r(left.num + right.num, left.den + right.den); // here we're calling the constructor of the Rational class
    auto root = new BinaryNode<Rational>(r); // auto keyword will infer the data type used (c++11)
    if (i == path.size()) {
        ans = root->elem;
        return root; // base case
    }
    if (path[i] == 'L') {
        root->left = build_tree(ans, left, r, i + 1, path);
    } else {
        root->right = build_tree(ans, r, right, i + 1, path);
    }
    return root;
}

Rational rational_from_sbt(BinaryNode<Rational> *&src, const string &path) {
    Rational l(0, 1), r(1, 0);
    Rational ans;
    auto tmp = build_tree(ans, l, r, 0, path);
    return ans;
}

int main(int argc, char const *argv[]) {
    int n; // test cases
    cin >> n;
    //n = 3;
    BinaryNode<Rational> *tree = nullptr;
    while (n--) {
        string path ;
        cin >> path;
        Rational r = rational_from_sbt(tree, path);
        cout << r.num << "/" << r.den << endl;
    } 
    return 0;
} // Angel

