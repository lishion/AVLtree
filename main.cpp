#include <iostream>

#include "AVLTree.h"
#include <vector>
#include <iostream>
#include "stdio.h"
#include "time.h"
#include "algorithm"
using namespace std;
#define NUM 1000
int main() {

    AVLTree avlTree;
    vector<int> vecint;
    int num[NUM]={0};

    for(int i=0;i<NUM;i++){

        vecint.push_back(i);
        avlTree.insert(i);
    }
    random_shuffle(vecint.begin(),vecint.end());
    for(int i=0;i<NUM-8;i++){

        avlTree.deleteNode(vecint.at(i));
    }

    avlTree.visitTree();




    return 0;
}