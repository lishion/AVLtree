//
// Created by Lishion on 2017/3/5.
//

#ifndef AVL_TREE_AVLTREE_H
#define AVL_TREE_AVLTREE_H

#include <iostream>
#include <cmath>
#include <stdio.h>
#include <cstdlib>
#include <string>
#include <sstream>

using namespace std;
class AVLTree {
public:
    enum  class TYPE{LL=0X66,LR=0X69,RL=0X96,RR=0X99,LLS = 0XFF,RRS = 0X00};
    enum  class LINK_TYPE{LEFT=0,RIGHT};
    const LINK_TYPE LEFT = LINK_TYPE ::LEFT;
    const LINK_TYPE RIGHT = LINK_TYPE :: RIGHT;
private:
    class Node{
    public:
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

            ss<<" value: "<<value
            //  <<" father: "<<convertToString(father)
              <<" left: "<<convertToString(left)
              <<" right: "<<convertToString(right)
              <<" BF: "<<BF<<endl;
            return  ss.str();
        }
    };

    Node *root;
    Node* createNode(int value){
        Node *node = new Node();
        node->left = nullptr;
        node->right = nullptr;
        node->value = value;
        node->father = nullptr;
        return node;
    }

    //释放节点内存
    void releaseNode(Node *&node){
        delete node;
        node= nullptr;
    }

    void setAsRoot(Node *node){

        root = node;
        root->father = nullptr;
    }

    //访问每个节点
    //start_node: 开始遍历的节点
    //fun : 类型是 void(* func )(Node *node),用于对节点进行操作
    template < typename T>
    void visitNode( Node *start_node,T func){

        func(start_node);
        if(start_node->left)
            visitNode(start_node->left,func);
        if(start_node->right)
            visitNode(start_node->right,func);
    }

    //通过值找到适合条件的节点
    //start_node: 起始节点
    //value:值
    //func: 条件函数
    template <typename T>
    Node *findByValue(Node *start_node,int value,T func){

        if(start_node == nullptr){
            return nullptr;
        }

        if( func(start_node) ){
            return start_node;
        }

        if(value >= start_node->value){
            return findByValue(start_node->right,value,func);
        } else{
            return findByValue(start_node->left,value,func);
        }
    }

    //找到最大值对应的节点
    //start_node: 最大值
    Node* findMaxNode( Node *start_node){
        int  x= 0;
        Node *n = nullptr;
        auto func = [&x,&n]( Node *now_node){

            if(now_node->value >= x){
                x = now_node->value;
                n = now_node;
            }
        };
        visitNode(start_node,func);
        return n;
    }

    //一般情况删除
    //
    Node *deleteGeneral(Node *node){

        Node *child = ( node->left == nullptr  ?  node->right : node->left );//只有一个子节点，先找到该子节点,如果返回空 则表示没有子节点
        Node *father = node->father;

        if(  node == root    ){ // 如果该节点是根节点

           if(child == nullptr){//如果子节点为空  直接删除子节点
               delete root;
               root = nullptr;
           } else{
               setAsRoot(child);//子节点不为空，则先将此节点的子节点设置为根节点 然后删除该节点
               releaseNode(node);
           }
            return nullptr;
        }

        reLink(node,child);//将原来指向该节点的指针指向子节点

        releaseNode(node);
        return  father;
    }


   template <typename T>
   Node *reBack(Node *start_node,T func){

       Node *now_node = start_node;
       while(now_node != nullptr){

           int &BF = now_node->BF;
           if( func(now_node,BF) ){
               return nullptr;
           }
           if(abs(BF) == 2){
               return now_node;//找到失衡节点 退出
           }

           now_node = now_node->father;//更新回溯节点
       }
       return nullptr;
   }
    Node *rebackWhenDelete(Node *startNode){
        Node *now_Node = startNode;
        if(startNode == root) return nullptr;
        while (now_Node!= nullptr){


            int &BF = now_Node->father->BF;
            ( getLinkType(now_Node) == LEFT ) ? BF-- : BF ++;
            if(abs(BF) == 1) {
                return nullptr;
            }
            else if(abs(BF) == 2){
                return  now_Node->father;
            }
            now_Node = now_Node->father ;
            if(now_Node == root){
                return nullptr;
            }
        }

    }




    //得到失衡类型 只需要引起失衡的节点和失衡的节点即可
    //target_node: 引起失衡的节点
    //inbalace_node: 不平衡节点
    TYPE getInbalanceType(Node *inbalance_node){

        unsigned char code = 0x00;
        Node *next_node = nullptr;
        //判断位于根的左边还是右边

        if( inbalance_node->BF <0 ){
            code |= 0x90;//右子树
            next_node = inbalance_node->right;
        } else{
            code |= 0x60;
            next_node = inbalance_node->left;
        }

        if(next_node == nullptr){
            visitNode(inbalance_node->father,[]( Node *node){cout<<node->toString()<<endl;} );
        }

        switch (next_node->BF){
            case 0:  if(inbalance_node->BF < 0){
                    code &= 0x00;
                }
                else{
                    code |= 0xff;
                }
                break;
            case 1: code |= 0x06;
                break;
            case -1: code |= 0x09;
                break;
            default:;
        }

        return  (TYPE)code;
    }

    //判断该节点连接在父节点的左枝还是右枝
    LINK_TYPE getLinkType(Node *node){

        return ( node->father->left == node )? LEFT : RIGHT;
    }

    //创建链接
    //type: 左子树还是右子树
    void createLink(Node *node_start,Node *node_end,LINK_TYPE type){
        if(node_start == nullptr ){
            return;
        }
        if(type == LINK_TYPE::LEFT){
            node_start->left = node_end;
        }
        else {
            node_start->right = node_end;
        }

        if(node_end != nullptr){
            node_end->father = node_start;
        }

    }

    //将指向该节点的指针指向另一个节点
    void reLink(Node *node,Node *target_node){
         createLink(node->father,target_node,getLinkType(node));
    }


    //左旋节点
    //左旋时，右子节点一定存在
    //左旋定义:
    //1. 若非头结点，将父节点指向右子节点
    //2. 保存 右子节点 的 左子节点
    //3. 将该节点作为右子节点的左子节点
    //4. 将保存的左子节点作为该节点的右子节点
    //5. 旋转后，如果该节点是根节点，则应该将该节点的右子节点设置为根节点

    void rotateLeft(Node *node){

        int B2 = node->BF;
        int B1 = node->right->BF;
        Node *temp_right_node = node->right;


        if(node->father != nullptr){
            reLink(node,node->right);            //1
        }

        bool isRoot = (node->father == nullptr) ;//判断该节点是否是头结点

        Node *temp_node = node->right->left;     //2
        createLink(node->right,node,LEFT);      //3
        createLink(node,temp_node,RIGHT);       //4

        if(isRoot){
            setAsRoot(node->father);//将该节点的新父节点作为头结点
        }

        if(B1 > 0){
            node->BF = B2 + 1;
            temp_right_node->BF = (B1 + B2 + 1) > (B1 + 1) ? (B1 + B2 + 1) : (B1 + 1);
        } else{
            node->BF = B2 - B1 + 1;
            temp_right_node->BF = ( B2 + 2 ) > (B1 + 1) ? (B2 + 2 ) : ( B1 + 1 );
        }



    }

    //右旋节点
    //右旋时，右子节点一定存在
    //右旋定义:
    //1. 若非头结点，将父节点指向左子节点
    //2. 保存 左子节点 的 右子节点
    //3. 将该节点作为左子节点的右子节点
    //4 . 将保存的右子节点作为该节点的左子节点
    void rotateRight(Node *node){

        int B2 = node->BF;
        int B1 = node->left->BF;

        if(node->value == 52){
            node->value = 52;
        }
        Node *temp_left_node = node->left;

        if(node->father != nullptr){
            reLink(node,node->left);           //1

        }

        bool isRoot = (node->father == nullptr) ;

        Node *temp_node = node->left->right;     //2
        createLink(node->left,node,RIGHT);      //3
        createLink(node,temp_node,LEFT);       //4

        if(isRoot){
            setAsRoot(node->father);
        }

        if(B1 > 0){
            node->BF = B2 - B1 -1;
            temp_left_node ->BF = (B1 - 1) < ( B2 - 2 ) ? (B1-1) : (B2 - 2);
        } else{
            node->BF = B2-1;
            temp_left_node ->BF = (B1 - 1) < ( B1 + B2 - 2 ) ? ( B1-1 ) : (B1 + B2 - 2);
        }


    }

public:

    AVLTree(int value){
        root = createNode(value);
    }
    AVLTree(){
        root = nullptr;
    }

    void  visitTree(){
        if(root == nullptr)
            return;

        visitNode(root,[]( Node *node){cout<<node->toString()<<endl;} );
    }

    void doBanlance(Node *node,TYPE type){

        switch(type){
            case TYPE ::LLS :
            case TYPE::LL : rotateRight(node);
                break;
            case TYPE::LR : rotateLeft(node->left);rotateRight(node);
                break;
            case TYPE ::RRS :
            case TYPE::RR : rotateLeft(node);
                break;
            case TYPE::RL :rotateRight(node->right);rotateLeft(node);
                break;
        }
    }



    //插入
    Node* insert(int value){

        if(root == nullptr){
            root = createNode(value);//确认根节点建立
            return root;
        }

        //查找可以存放的节点的条件函数
        auto func = [value](Node *now_node) -> bool {
            bool get = (value >= now_node->value && (now_node->right == nullptr) ) ||
                        (value < now_node->value && (now_node->left == nullptr) );
            return get;
        };

        Node *node =  findByValue(root,value,func);//找到存放节点
        Node *new_node = createNode(value);//建立新节点

        new_node->father = node;
        (value >= node->value) ? node->right = new_node : node->left = new_node;//建立链接关系

        auto func2 = [value](Node *now_node,int &BF){
            (value >= now_node->value) ? BF-- : BF++;//插在左子树增加 插在右子树减少
            return  BF == 0 ;
        };

        Node *inbalance = reBack(new_node->father,func2);

        if(inbalance != nullptr){

             doBanlance(inbalance,getInbalanceType(inbalance));
        }

        return new_node;

    }

    void deleteNode(int value){

       while(true){
           Node *node = findByValue(root,value,
                                    [value](Node *now_node){return now_node->value == value;} );//先找到要删除的节点
           if(node == nullptr){
               return;
           }
           deleteHelper(node);
       }
    }

    void deleteHelper(Node *node){
        Node* inbalance_array[32] = {nullptr};

        Node *real_delete_node = nullptr;
        //如果该节点的两个子节点都存在 则需要用左子树最大的节点和该节点交换 然后删除该节点

        if(node->left != nullptr && node->right != nullptr){
             real_delete_node = findMaxNode(node->left);
        }
        else {
            real_delete_node = node;
        }

        Node *inbalance_node =  rebackWhenDelete(real_delete_node);//在删除前回溯 回溯起点为真正的删除点

        node->value = real_delete_node->value;
        deleteGeneral( real_delete_node );


        for(int i=0;  inbalance_node != nullptr;i++ ){

                TYPE type = getInbalanceType(inbalance_node);
                inbalance_array[i] = inbalance_node;

                if(type == TYPE::LLS || type == TYPE::RRS || inbalance_node == root){
                    break;
                }


                inbalance_node = rebackWhenDelete(inbalance_node);

        }


        for (int i=0; inbalance_array[i] != nullptr; i++){
            TYPE type = getInbalanceType(inbalance_array[i]);
            doBanlance(inbalance_array[i],type);
            i++;
        }





    }

};


#endif //AVL_TREE_AVLTREE_H
