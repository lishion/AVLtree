//
// Created by Lishion on 2017/3/11.
//
#include "AVLTree.h"

Node* AVLTree:: createNode(int value){
    Node *node = new Node();
    node->left = nullptr;
    node->right = nullptr;
    node->value = value;
    node->father = nullptr;
    return node;
}

//释放节点内存
void AVLTree::releaseNode(Node *&node){
    delete node;
    node= nullptr;
}

void AVLTree:: setAsRoot(Node *node){

    root = node;
    root->father = nullptr;
}

//访问每个节点
//start_node: 开始遍历的节点
//fun : 类型是 void(* func )(Node *node),用于对节点进行操作
template < typename T>
void AVLTree:: visitNode( Node *start_node,T func){

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
Node* AVLTree:: findByValue(Node *start_node,int value,T func){

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
//start_node:
Node* AVLTree:: findMaxNode( Node *start_node){
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


//包括一个子节点和没有子节点的情况 可以合并到一起处理
//不包括平衡处理 平衡处理位于--deleteHelper--函数中
AVLTree* AVLTree:: deleteGeneral(Node *node){

    Node *child = ( node->left == nullptr  ?  node->right : node->left );//只有一个子节点，先找到该子节点,如果返回空 则表示没有子节点
    Node *father = node->father;

    if(  node == root     ){ // 如果该节点是根节点

        if(child != nullptr){// 如果有子节点 先将子节点设为根节点
            setAsRoot(child);
        }
        releaseNode(node);//释放根节点 注意这里不能改为 releaseNode(root) 因为根节点已经更新了
        return this;
    }

    reLink(node,child);//将原来指向该节点的指针指向子节点
    releaseNode(node);//释放根节点

    return this;
}



//插入时回溯
//插入时普遍满足小值位于左子树 ，大值或相等的值位于右子树
//所以可以采用值比较法判断插入位置从而修改BF值
Node* AVLTree:: reBackWhenInsert(Node *start_node){

    Node *now_node = start_node->father;
    int value = start_node->value;

    while(now_node != nullptr){

        int &BF = now_node->BF;
        (value >= now_node->value) ? BF-- : BF++;
        if(BF == 0){
            return nullptr;//对父节点影响消失 退出
        }
        if(abs(BF) == 2){
            return now_node;//找到失衡节点 退出
        }

        now_node = now_node->father;//更新回溯节点
    }
    return nullptr;
}

//删除时回溯
//删除时树往往已经经历过若干次旋转 可能出现相等的值在左边的情况 所以不能使用值大小判断
//采用的是直接判断子节点与父节点的连接关系方式回溯 从而修改BF值
//接受的节点必须有父节点
Node * AVLTree::rebackWhenDelete(Node *startNode){

    Node *now_Node = startNode;
    while (now_Node -> father != nullptr){

        int &BF = now_Node->father->BF;//得到父节点的BF值
        ( getLinkType(now_Node) == LEFT ) ? BF-- : BF++;//如果删除点位于父节点的左边 BF值-1 或者+1
        if(abs(BF) == 1) {
            return nullptr;//如果BF值为1 则对父节点影响消失 退出
        }
        else if(abs(BF) == 2){
            return  now_Node->father; //找到失衡节点  退出
        }
        now_Node = now_Node->father ;

    }

}




//得到失衡类型 只需要失衡点即可判断
AVLTree::TYPE AVLTree:: getInbalanceType(Node *inbalance_node){

    unsigned char code = 0x00;
    Node *next_node = nullptr
    ;
    //判断位于根的左边还是右边
    if( inbalance_node->BF <0 ){
        code |= 0x90;//右子树
        next_node = inbalance_node->right;
    } else{
        code |= 0x60;
        next_node = inbalance_node->left;
    }

    if(next_node == nullptr){
        visitNode(inbalance_node,[](Node *node){cout<<"error!! the node is: "<<node->toString()<<endl;});
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
AVLTree::LINK_TYPE AVLTree:: getLinkType(Node *node){

    return ( node->father->left == node )? LEFT : RIGHT;
}

//创建连接
//type: 左子树还是右子树
void AVLTree:: createLink(Node *father_node,Node *child_node,LINK_TYPE type){
    if(father_node == nullptr ){
        return;
    }
    if(type == LEFT){
        father_node->left = child_node;
    }
    else {
        father_node->right = child_node;
    }

    if(child_node != nullptr){
        child_node->father = father_node;
    }

}

//将指向该节点的指针指向另一个节点
void AVLTree:: reLink(Node *node,Node *target_node){
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

void AVLTree:: rotateLeft(Node *node){

    int B2 = node->BF; //旋转节点的BF值
    int B1 = node->right->BF; // 轴节点的BF值

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
void AVLTree:: rotateRight(Node *node){

    int B2 = node->BF; //旋转节点的BF值
    int B1 = node->left->BF;// 轴节点的BF值

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
void AVLTree:: deleteHelper(Node *node){
    Node* inbalance_array[32] = {nullptr}; //存放非失衡节点的数组

    Node *real_delete_node = nullptr; //真正删除点

    //如果该节点的两个子节点都存在 则需要用左子树最大的节点和该节点交换 然后删除该节点
    if(node->left != nullptr && node->right != nullptr){
        real_delete_node = findMaxNode(node->left);
    }
    else {
        real_delete_node = node;
    }

    Node *inbalance_node =  rebackWhenDelete(real_delete_node); //在删除前回溯 回溯起点为真正的删除点

    node->value = real_delete_node->value;//替换值
    deleteGeneral( real_delete_node );


    //将所用不平衡的点放在一个数组中
    //和插入不同 删除可能导致不止一个节点失衡 所以需要多次处理
    for(int i=0;  inbalance_node != nullptr;i++ ){

        TYPE type = getInbalanceType(inbalance_node);
        inbalance_array[i] = inbalance_node;

        if(type == TYPE::LLS || type == TYPE::RRS || inbalance_node == root){
            break;
        }

        inbalance_node = rebackWhenDelete(inbalance_node);
    }

    //处理所有不平衡的点
    for (int i=0; inbalance_array[i] != nullptr; i++){
        TYPE type = getInbalanceType(inbalance_array[i]);
        doBanlance(inbalance_array[i],type);
    }

}

void AVLTree::  visitTree(){
    if(root == nullptr)
        return;

    visitNode(root,[]( Node *node){cout<<node->toString()<<endl;} );
}

//平衡处理 根据不同的失衡方式调用左旋或右旋平衡
//node: 失衡节点
//type: 失衡类型
void AVLTree:: doBanlance(Node *node,TYPE type){

    switch(type){
        case TYPE::LLS :
        case TYPE::LL  : rotateRight(node);
            break;
        case TYPE::LR  : rotateLeft(node->left);rotateRight(node);
            break;
        case TYPE::RRS :
        case TYPE::RR  : rotateLeft(node);
            break;
        case TYPE::RL  :rotateRight(node->right);rotateLeft(node);
            break;
    }
}

//插入
AVLTree* AVLTree::insert(int value){

    if(root == nullptr){
        root = createNode(value);//确认根节点建立
        return this;
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

    Node *inbalance = reBackWhenInsert(new_node);//找到失衡节点 如果为空 则表示此次插入没引起失衡

    if(inbalance != nullptr){

        doBanlance(inbalance,getInbalanceType(inbalance));
    }

    return this;

}

AVLTree* AVLTree:: remove(int value){

    while(true){//循环查找节点 直到删除所有对应节点
        Node *node = findByValue(root,value,
                                 [value](Node *now_node){return now_node->value == value;} );//先找到要删除的节点
        if(node == nullptr){
            return this;
        }
        deleteHelper(node);
    }

}