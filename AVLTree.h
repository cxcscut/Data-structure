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

    AVLTree SingleRotate_L(AVLTree node){
        AVLTree k1 = node->right;
        node->right = k1->left;
        k1->left = node;
        return k1;
    }

    AVLTree SingleRotate_R(AVLTree node){
        AVLTree k1 = node->left;
        node->left = k1->right;
        k1->right = node;
        return k1;
    }

    AVLTree LeftBalance(AVLTree & Root){
        AVLTree lc = Root->right;
        switch(lc->bf){
            case RIGHT_HIGH:
                Root->bf = lc->bf = EQUALLY_HIGH;
                return SingleRotate_L(Root);
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

                Root->right = SingleRotate_R(Root->right);
                return SingleRotate_L(Root);
        }
    }

    AVLTree RightBalance(AVLTree &Root){
        AVLTree lc = Root->left;
        switch(lc->bf){
            case LEFT_HIGH:
                Root->bf = lc->bf = EQUALLY_HIGH;
                return SingleRotate_R(Root);
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
                Root->left = SingleRotate_L(Root->left);
                return SingleRotate_R(Root);
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
            root->left = root->right = nullptr;
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
                            tmp_last->bf = EQUALLY_HIGH;
                            if(tmp_last == root)
                                root = RightBalance(tmp_last);
                            else
                            {
                                if(path.top()->left == tmp_last)
                                    path.top()->left = LeftBalance(tmp_last);
                                else
                                    path.top()->right = LeftBalance(tmp_last);
                            }
                            return true;
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
                            tmp_last->bf = EQUALLY_HIGH;
                            if(tmp_last == root)
                                root = LeftBalance(tmp_last);
                            else
                            {
                                if(path.top()->left == tmp_last)
                                    path.top()->left = LeftBalance(tmp_last);
                                else
                                    path.top()->right = LeftBalance(tmp_last);
                            }
                            return true;
                    }
                }
            }
        }

        return true;
    }

    virtual bool DeleteData(const T &data){
        if(!root) return false;
        std::stack<AVLTree> path;
        int direc;
        bool lower = false;
        AVLTree node = root,prior = root;
        if(root->data == data)
        {
            delete root;
            root = nullptr;
            return true;
        }

        while(node && node->data != data){
            path.push(node);
            prior = node;
            if(node->data < data) {
                node = node->right;
                direc = RIGHT;
            }
            if(node->data > data) {
                node = node->left;
                direc = LEFT;
            }
        }

        if(!node) return false;

        if(!node->left && !node->right)
        {
            if(direc == LEFT){
                prior->left = nullptr;
                switch (path.top()->bf)
                {
                    case EQUALLY_HIGH:
                        lower = false;
                        path.top()->bf = RIGHT_HIGH;
                        break;
                    case LEFT_HIGH:
                        lower = true;
                        path.top()->bf = EQUALLY_HIGH;
                        break;
                    case RIGHT_HIGH:
                        lower = true;
                        prior->bf = EQUALLY_HIGH;
                        if(prior == root){
                            root = LeftBalance(root);
                        }
                        else
                        {
                            path.pop();
                            AVLTree  tmp_rotate = path.top();
                            if(tmp_rotate->left == prior)
                                tmp_rotate->left = LeftBalance(prior);
                            else
                                tmp_rotate->right = LeftBalance(prior);
                        }
                        break;
                }
            }
            else {
                prior->right = nullptr;
                switch (path.top()->bf)
                {
                    case EQUALLY_HIGH:
                        lower = false;
                        path.top()->bf = LEFT_HIGH;
                        break;
                    case LEFT_HIGH:
                        lower = true;
                        prior->bf = EQUALLY_HIGH;
                        if(prior == root){
                            root = RightBalance(root);
                        }
                        else
                        {
                            path.pop();
                            AVLTree tmp_rotate = path.top();
                            if(tmp_rotate->left == prior)
                                tmp_rotate->left = RightBalance(prior);
                            else
                                tmp_rotate->right = RightBalance(prior);
                        }
                        break;
                    case RIGHT_HIGH:
                        lower = true;
                        path.top()->bf = EQUALLY_HIGH;
                        break;
                }
            }
            delete node;
        }
        else if(node->left && node->right){
            AVLTree tmp_prior = node->right;
            AVLTree tmp = tmp_prior->left;
            path.push(node);

            while(tmp){
                path.push(tmp_prior);
                tmp_prior = tmp;
                tmp = tmp->left;
            }

            path.pop();
            path.top()->right = tmp_prior->right;
            tmp_prior->right = node->right;
            tmp_prior->left = node->left;

            if(direc == LEFT)
                prior->left = tmp_prior;
            else
                prior->right = tmp_prior;
            switch (path.top()->bf)
            {
                case EQUALLY_HIGH:
                    lower = false;
                    path.top()->bf = RIGHT_HIGH;
                    break;
                case LEFT_HIGH:
                    lower = true;
                    path.top()->bf = EQUALLY_HIGH;
                    break;
                case RIGHT_HIGH:
                    lower = true;
                    path.top()->bf = EQUALLY_HIGH;
                    AVLTree tmp_rotate = path.top();
                    path.pop();
                    if(path.top() == root)
                    {
                        root = LeftBalance(root);
                    }
                    else
                    {
                        if(tmp_rotate == path.top()->left)
                            path.top()->left = LeftBalance(tmp_rotate);
                        else
                            path.top()->right = LeftBalance(tmp_rotate);
                    }
                    break;
            }
            direc = LEFT;
        }
        else
        {
            if(node->left)
                if(direc == LEFT)
                    prior->left = node->left;
                else
                    prior->right = node->left;
            if(node->right)
                if(direc == RIGHT)
                    prior->right = node->right;
                else
                    prior->left = node->right;
            delete node;

            if(direc == LEFT)
            {
                switch (prior->bf)
                {
                    case EQUALLY_HIGH:
                        lower = false;
                        prior->bf = RIGHT_HIGH;
                        break;
                    case LEFT_HIGH:
                        lower = true;
                        prior->bf = EQUALLY_HIGH;
                        break;
                    case RIGHT_HIGH:
                        lower = true;
                        prior->bf = EQUALLY_HIGH;
                        if(prior == root)
                        {
                            root = LeftBalance(root);
                        }
                        else
                        {
                            path.pop();
                            AVLTree tmp_rotate = path.top();
                            if(tmp_rotate->left == prior)
                                tmp_rotate->left = LeftBalance(prior);
                            else
                                tmp_rotate->right = LeftBalance(prior);
                        }
                        break;
                }
            }
            else
            {
                switch (prior->bf)
                {
                    case EQUALLY_HIGH:
                        lower = false;
                        prior->bf = LEFT_HIGH;
                        break;
                    case LEFT_HIGH:
                        lower = true;
                        prior->bf = EQUALLY_HIGH;
                        if(prior == root)
                        {
                            root = RightBalance(root);
                        }
                        else
                        {
                            path.pop();
                            AVLTree tmp_rotate = path.top();
                            if(tmp_rotate->left == prior)
                                tmp_rotate->left = RightBalance(prior);
                            else
                                tmp_rotate->right = RightBalance(prior);
                        }
                        break;
                    case RIGHT_HIGH:
                        lower = true;
                        prior->bf = EQUALLY_HIGH;
                        break;
                }
            }

        }

        while(!path.empty())
        {
            AVLTree tmp_node = path.top();
            path.pop();
            if(lower)
            {
                if(direc == LEFT)
                {
                    switch (tmp_node->bf)
                    {
                        case EQUALLY_HIGH:
                            tmp_node->bf = RIGHT_HIGH;
                            break;
                        case LEFT_HIGH:
                            tmp_node->bf = EQUALLY_HIGH;
                            break;
                        case RIGHT_HIGH:
                            tmp_node->bf = EQUALLY_HIGH;
                            if(path.top() == root){
                                root = LeftBalance(root);
                            }
                            else
                            {
                                AVLTree tmp_rotate = path.top();
                                path.pop();
                                if(path.top()->left == tmp_rotate)
                                    path.top()->left = LeftBalance(tmp_rotate);
                                else
                                    path.top()->right = LeftBalance(tmp_rotate);
                            }
                            break;
                    }
                }
                else
                {
                    switch (tmp_node->bf)
                    {
                        case EQUALLY_HIGH:
                            tmp_node->bf = LEFT_HIGH;
                            break;
                        case LEFT_HIGH:
                            tmp_node->bf = EQUALLY_HIGH;
                            if(path.top() == root){
                                root = RightBalance(root);
                            }
                            else
                            {
                                AVLTree tmp_rotate = path.top();
                                path.pop();
                                if(path.top()->left == tmp_rotate)
                                    path.top()->left = RightBalance(tmp_rotate);
                                else
                                    path.top()->right = RightBalance(tmp_rotate);
                            }
                            break;
                        case RIGHT_HIGH:
                            tmp_node->bf = EQUALLY_HIGH;
                            break;
                    }
                }
            }
        }
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