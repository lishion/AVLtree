//
// Created by Lishion on 2017/3/5.
// 该代码对AVL树进行一个简单的实现，包括了插入 和 删除 功能
// 只能作为对AVL树理解之用
// 本身并无任何应用价值
// 针对该代码的文档位于github.com/lishion/AVLtree
// 注释未解释或未解释清楚的部分 文档中都会有解释

#ifndef AVL_TREE_AVLTREE_H
#define AVL_TREE_AVLTREE_H

#include <iostream>
#include <cmath>
#include <stdio.h>
#include <cstdlib>
#include <string>
#include "Node.h"
using namespace std;
class AVLTree {
public:
    enum  class TYPE{LL=0X66,LR=0X69,RL=0X96,RR=0X99,LLS = 0XFF,RRS = 0X00};
    enum  class LINK_TYPE{LEFT=0,RIGHT};

private:
    const static LINK_TYPE LEFT = LINK_TYPE ::LEFT;
    const static LINK_TYPE RIGHT = LINK_TYPE :: RIGHT;
    Node *createNode(int value);//创建节点
    void releaseNode(Node *&node);//释放节点
    void setAsRoot(Node *node);//将节点设置为根节点
    template < typename T>
    void visitNode( Node *start_node,T func);//访问节点
    template <typename T>
    Node *findByValue(Node *start_node,int value,T func);//通过值找到节点
    Node* findMaxNode( Node *start_node);//找到最大节点
    Node *reBackWhenInsert(Node *start_node);//插入时回溯
    Node *rebackWhenDelete(Node *startNode);//删除时回溯
    AVLTree *deleteGeneral(Node *node);//一般情况删除
    TYPE getInbalanceType(Node *inbalance_node);//得到失衡类型
    LINK_TYPE getLinkType(Node *node);//获取该节点连接父节点上的位置
    void createLink(Node *father_node,Node *child_node,LINK_TYPE type);//建立一个；连接
    void reLink(Node *node,Node *target_node);//重新建立连接
    void rotateLeft(Node *node);//左旋
    void rotateRight(Node *node);//右旋
    void deleteHelper(Node *node);//删除一个元素
    void doBanlance(Node *node,TYPE type);//平衡处理
    Node *root;//根节点

public:

    //构造函数
    AVLTree(int value){
        root = createNode(value);
    }
    AVLTree():root(nullptr){};



    void  visitTree();//遍历树
    AVLTree* insert(int value);//插入
    AVLTree* remove(int value);//删除



};


#endif //AVL_TREE_AVLTREE_H
