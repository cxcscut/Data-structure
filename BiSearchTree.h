// File Name : BiSearchTree.h
// Created by cxc123 on 16-10-25.
// Description : the implementation of a binary search tree

#ifndef GENERIC_H_INCLUDED
#define MAX(A,B) (A>B ? A:B)
#define SUCCESSED 1
#define FAILED -1
#include <stdexcept>
#include <vector>
#include <stack>
#include <deque>
#endif // GENERIC_H_INCLUDED

#ifndef BITREE_BISEARCHTREE_H
#define BITREE_BISEARCHTREE_H
#include "BiTree.h"

template <typename T>
class BST : public BinaryTree<T>
{

    typedef typename BinaryTree<T>::BiNode BSTNode,*BSTree;

public: // Constructors

    BST() = default; // Default constructor

    BST(const std::vector<T> &data_vector){ // constructor
        typename std::vector<T>::const_iterator pvec;
        for(pvec = data_vector.begin(); pvec < data_vector.end(); pvec++){
            AddData(*pvec);
        }
    }

    virtual ~BST(){} // Destructor

public: // Basic function

    virtual bool SearchNode(const T &data,BSTree &ret_prior) const{
        BSTree tmp = nullptr;
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

    virtual bool AddData(const T &data){
        BSTree prior_new = nullptr;
        if(SearchNode(data,prior_new)) return false;
        BSTree new_node = new BSTNode;
        new_node->data =data;
        new_node->left = new_node->right = nullptr;
        if(!this->root) {
            this->root = new_node;
            return true;
        }
        if(prior_new->data < data)
            prior_new->right = new_node;
        else
            prior_new->left = new_node;
        return true;
    }

   virtual bool DeleteData(const T &data){
       BSTree prior = nullptr;
       BSTree tmp;
       if(!SearchNode(data,prior)) return false;
       if(!prior){
           tmp = this->root;
           if(!tmp->left && !tmp->right){
               delete tmp;
               this->root = nullptr;
               return true;
           }
           if(!tmp->left){
               this->root = this->root->right;
               delete tmp;
               return true;
           }
           if(!tmp->right){
               this->root = this->root->left;
               delete tmp;
               return true;
           }
           BSTree last = this->root->left;
           BSTree last_prior = nullptr;

           while(last->right){
               last_prior = last;
               last = last->right;
           }

           if(!last_prior){
               this->root->left->right = this->root->right;
               this->root = this->root->left;
               delete tmp;
               return true;
           }
           else
           {
               last_prior->right = nullptr;
               last->right = this->root->right;
               last->left = this->root->left;
               this->root = last;
               delete tmp;
               return true;
           }
       }
       else
       {
           if(prior->data > data) {
               tmp = prior->left;
               if(!tmp->left && !tmp->right)
               {
                   delete tmp;
                   prior->left = nullptr;
                   return true;
               }
               if(!tmp->left){
                   prior->left = tmp->right;
                   delete tmp;
                   return true;
               }
               if(!tmp->left)
               {
                   prior->left = tmp->left;
                   delete tmp;
                   return true;
               }
               BSTree last = tmp->left;
               BSTree last_prior = nullptr;

               while(last->right){
                   last_prior = last;
                   last = last->right;
               }

               if(!last_prior){
                   tmp->left->right = tmp->right;
                   prior->left = tmp->left;
                   delete tmp;
                   return true;
               }
               else
               {
                   last_prior->right = nullptr;
                   prior->left = last;
                   last->right = tmp->right;
                   last->left = tmp->left;
                   delete tmp;
                   return true;
               }
           }
           else {
               tmp = prior->right;
               if(!tmp->left && !tmp->right)
               {
                   delete tmp;
                   prior->right = nullptr;
                   return true;
               }
               if(!tmp->left){
                   prior->right = tmp->right;
                   delete tmp;
                   return true;
               }
               if(!tmp->right){
                   prior->right = tmp->left;
                   delete tmp;
                   return true;
               }
               BSTree first = tmp->left;
               BSTree first_last = nullptr;

               while(first->right){
                   first_last = first;
                   first = first->right;
               }

               if(!first_last){
                   prior->right = tmp->left;
                   tmp->left->right = tmp->right;
                   delete tmp;
                   return true;
               }
               else
               {
                   first_last->right = nullptr;
                   prior->right = first;
                   first->right = tmp->right;
                   first->left = tmp->left;
                   delete tmp;
                   return true;
               }
           }
       }
   }
};

#endif //BITREE_BISEARCHTREE_H
