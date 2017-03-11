//
// Created by Lishion on 2017/3/11.
//

#ifndef AVL_TREE_NODE_H
#define AVL_TREE_NODE_H

#include <iostream>
#include <cmath>
#include <stdio.h>
#include <cstdlib>
#include <string>
#include <sstream>
using namespace std;
class Node{
private:
    int BF;//平衡因子
    int value;//值
    Node* left;//左节点
    Node* right;//右节点
    Node* father;//父节点

    string convertToString(Node *node){
        stringstream ss;
        return  (   node== nullptr ? "null" : (ss<<node->value,ss.str()) );
    }
    string toString(){
        stringstream ss;
        ss <<" value: "<<value
          <<" father: "<<convertToString(father)
          <<" left: "<<convertToString(left)
          <<" right: "<<convertToString(right)
          <<" BF: "<<BF<<endl;
        return  ss.str();
    }
    friend class AVLTree;
};
#endif //AVL_TREE_NODE_H
