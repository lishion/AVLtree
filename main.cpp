#include <iostream>

#include "AVLTree.h"
#include <vector>
#include <iostream>
#include "stdio.h"
using namespace std;


int main() {

    AVLTree avlTree;
    int i=9;

    while((i--) > 0){

        srand(i);
        int x = rand()%10;
        avlTree.insert(x);
    }
    avlTree.visitTree();
    return 0;
}