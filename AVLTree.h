// File Name : AVLTree.h
// Created by cxc123 on 16-10-26.
// Description : the implementation of a AVL tree

#ifndef GENERIC_H_INCLUDED
#define MAX(A,B) (A>B ? A:B)
#define SUCCESSED 1
#define FAILED -1
#include <stdexcept>
#include <vector>
#include <stack>
#include <deque>
#endif // GENERIC_H_INCLUDED

#ifndef AVLTREE_H_INCLUDED
#define AVLTREE_H_INCLUDED
#define EQUALLY_HIGH 0
#define LEFT_HIGH -1
#define RIGHT_HIGH 1
#define LEFT -1
#define RIGHT 1
#include "BiSearchTree.h"

template <typename T>
class AVL: public BST<T>
{

public: // Structs

    struct AVLNode {
        T data;
        struct AVLNode *left;
        struct AVLNode *right;
        int bf;
    };

    typedef struct AVLNode AVLNode,*AVLTree;

public: // Variables

    AVLTree root;

public: // Constructors

    AVL() // Default constructor
    {
        root = nullptr;
    }

    explicit AVL(const T &data){ // Copy constructor
        this->root = new AVLNode;
        this->root->left =this->root->right = nullptr;
        this->root->bf = EQUALLY_HIGH;
    }

    AVL(const std::vector<T> &vec_data) : AVL()
    {
        typename std::vector<T>::const_iterator pvec;
        for(pvec = vec_data.begin();pvec < vec_data.end();pvec++){
            AddData(*pvec);
        }
    }

    virtual ~AVL(){ // Destructor
        DeleteNode(root);
    }

public: // Basic functions

    virtual bool SearchNode(const T &data,AVLTree &ret_prior) const{
        AVLTree tmp = nullptr;
        if(!this->root) return false;
        tmp = this->root;

        while(tmp) {
            if(tmp->data == data)
                return true;
            ret_prior = tmp;
            if(tmp->data < data)
                tmp = tmp->right;
            else
                tmp = tmp->left;
        }
        return false;
    }

    void SingleRotate_L(AVLTree &node){
        AVLTree  rc = node->right;
        node->right = rc->left;
        rc->left = node;
        node = rc;
    }

    void SingleRotate_R(AVLTree &node){
        AVLTree lc = node->left;
        node->left = lc->right;
        lc->right = node;
        node = lc;
    }

    void LeftBalance(AVLTree & Root){
        AVLTree lc = Root->right;
        switch(lc->bf){
            case RIGHT_HIGH:
                Root->bf = lc->bf = EQUALLY_HIGH;
                SingleRotate_L(Root);
                break;
            case LEFT_HIGH:
                AVLTree rd = lc->left;
                switch (rd->bf){
                    case LEFT_HIGH:
                        Root->bf = EQUALLY_HIGH;
                        lc->bf = RIGHT_HIGH;
                        break;
                    case EQUALLY_HIGH:
                        Root->bf = lc->bf = EQUALLY_HIGH;
                        break;
                    case RIGHT_HIGH:
                        Root->bf = LEFT_HIGH;
                        lc->bf = EQUALLY_HIGH;
                        break;
                }
                rd->bf = EQUALLY_HIGH;
                SingleRotate_R(Root->right);
                SingleRotate_L(Root);
        }
    }

    void RightBalance(AVLTree &Root){
        AVLTree lc = Root->left;
        switch(lc->bf){
            case LEFT_HIGH:
                Root->bf = lc->bf = EQUALLY_HIGH;
                SingleRotate_R(Root);
                break;
            case RIGHT_HIGH:
                AVLTree rd = lc->right;
                switch (rd->bf){
                    case LEFT_HIGH:
                        Root->bf = RIGHT_HIGH;
                        lc->bf = EQUALLY_HIGH;
                        break;
                    case EQUALLY_HIGH:
                        Root->bf = lc->bf = EQUALLY_HIGH;
                        break;
                    case RIGHT_HIGH:
                        Root->bf = EQUALLY_HIGH;
                        lc->bf = LEFT_HIGH;
                        break;
                }
                rd->bf = EQUALLY_HIGH;
                SingleRotate_L(Root->left);
                SingleRotate_R(Root);
        }
    }

    virtual bool AddData(const T &data)
    {
        bool taller = false;
        int direc;
        std::stack<AVLTree> path;
        AVLTree node = root;
        if(!root){
            root = new AVLNode;
            root->left = this->root->right = nullptr;
            root->data = data;
            root->bf = EQUALLY_HIGH;
            return true;
        }
        while(node){
            path.push(node);
            if(node->data == data)
                return false;
            if(node->data < data)
                node = node->right;
            else
                node = node->left;
        }

        AVLTree tmp = new AVLNode;
        tmp->left = tmp->right = nullptr;
        tmp->bf = EQUALLY_HIGH;
        tmp->data = data;

        if(path.top()->data < data) {
            direc = RIGHT;
            switch (path.top()->bf) {
                case EQUALLY_HIGH:
                    taller = true;
                    path.top()->bf = RIGHT_HIGH;
                    path.top()->right = tmp;
                    break;
                case LEFT_HIGH:
                    taller = false;
                    path.top()->bf = EQUALLY_HIGH;
                    path.top()->right = tmp;
                    break;
                case RIGHT_HIGH:
                    LeftBalance(path.top());
                    taller = false;
                    break;
            }
        }
        else
        {
            direc = LEFT;
            switch (path.top()->bf){
                case EQUALLY_HIGH:
                    taller = true;
                    path.top()->bf = LEFT_HIGH;
                    path.top()->left = tmp;
                    break;
                case LEFT_HIGH:
                    RightBalance(path.top());
                    taller = false;
                    break;
                case RIGHT_HIGH:
                    taller = false;
                    path.top()->bf = EQUALLY_HIGH;
                    path.top()->left = tmp;
                    break;
            }
        }

        path.pop();
        AVLTree tmp_last;

        while(!path.empty()){
            tmp_last = path.top();
            path.pop();
            if(direc == LEFT)
            {
                if(taller){
                    switch (tmp_last->bf){
                        case EQUALLY_HIGH:
                            tmp_last->bf = LEFT_HIGH;
                            break;
                        case LEFT_HIGH:
                            RightBalance(tmp_last);
                            tmp_last->bf = EQUALLY_HIGH;
                            break;
                        case RIGHT_HIGH:
                            tmp_last->bf = EQUALLY_HIGH;
                            break;
                    }
                }
            }
            else
            {
                if(taller){
                    switch (tmp_last->bf){
                        case EQUALLY_HIGH:
                            tmp_last->bf = RIGHT_HIGH;
                            break;
                        case LEFT_HIGH:
                            tmp_last->bf = EQUALLY_HIGH;
                            break;
                        case RIGHT_HIGH:
                            LeftBalance(tmp_last);
                            tmp_last->bf = EQUALLY_HIGH;
                            break;
                    }
                }
            }
        }

        return true;
    }

    virtual int GetDepth(const AVLTree &root) const
    {
        if(!root) return 0;
        return 1 + MAX(GetDepth(root->left),GetDepth(root->right));
    }

    virtual void DeleteNode(AVLTree &root) throw()
    {
        if(!root) return;
        if(root->left)
            DeleteNode(root->left);
        if(root->right)
            DeleteNode(root->right);
        delete root;
    }

    virtual std::vector<T> InOrder_traverse() const throw(){
        AVLTree tmp;
        std::vector<T> ret;
        std::stack<AVLTree> stack;
        if(!root) return ret;
        stack.push(root);

        while(!stack.empty()){
            tmp = stack.top();
            if(tmp){
                stack.push(tmp->left);
            }
            else
            {
                stack.pop();
                if(!stack.empty()) {
                    tmp = stack.top();
                    stack.pop();
                    ret.push_back(tmp->data);
                    stack.push(tmp->right);
                }
            }
        }

        return ret;
    }

    virtual std::vector<T> PostOrder_traverse() const throw(){
        AVLTree tmp,rear = nullptr;
        std::vector<T> ret;
        std::stack<AVLTree> stack;
        if(!root) return ret;
        stack.push(root);

        while(!stack.empty()){
            tmp = stack.top();
            if((!tmp->left && !tmp->right) ||
               (rear && (rear == tmp->left || rear == tmp->right)))
            {
                ret.push_back(tmp->data);
                stack.pop();
                rear = tmp;
            }
            else
            {
                if(tmp->right)
                    stack.push(tmp->right);
                if(tmp->left)
                    stack.push(tmp->left);
            }
        }

        return ret;
    }

    virtual std::vector<T> PreOrder_traverse() const throw(){
        AVLTree tmp;
        std::vector<T> ret;
        std::stack<AVLTree> stack;
        if(!root) return ret;
        stack.push(root);

        while(!stack.empty()){
            tmp = stack.top();
            if(tmp){
                ret.push_back(tmp->data);
                stack.push(tmp->left);
            }
            else {

                stack.pop();
                if (!stack.empty()) {
                    tmp = stack.top();
                    stack.pop();
                    stack.push(tmp->right);
                }
            }
        }

        return ret;
    }

    virtual std::vector<T> LevelOrder_traverse() const throw(){
        AVLTree tmp;
        std::vector<T> ret;
        std::deque<AVLTree> deque;
        if(!root) return ret;
        deque.push_back(root);

        while(!deque.empty())
        {
            tmp = deque.front();
            deque.pop_front();
            ret.push_back(tmp->data);
            if(tmp->left)
                deque.push_back(tmp->left);
            if(tmp->right)
                deque.push_back(tmp->right);
        }
        return ret;
    }

};

#endif // AVLTREE_H_INCLUDED