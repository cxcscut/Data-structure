/********************************************************
 * 2016-10-12
 * Implementation of binary tree
 * cxc123
 ********************************************************/

#ifndef GENERIC_H_INCLUDED
#define MAX(A,B) (A>B ? A:B)
#define SUCCESSED 1
#define FAILED -1
#include <stdexcept>
#include <vector>
#include <stack>
#include <deque>
#endif // GENERIC_H_INCLUDED

#ifndef BITREE_H_INCLUDED
#define BITREE_H_INCLUDED

template <typename T>
class BinaryTree
{

public: //Structs

    struct BiNode {
        T data;
        struct BiNode *left;
        struct BiNode *right;
    };

    typedef struct BiNode BiNode,*BiTree;

public: //Variables

    BiTree root;

public: // Constructor

    BinaryTree() // Default constructor
    {
        root = nullptr;
    }

    explicit BinaryTree(const T &data)
    {
        root = new BiNode;
        root->data = data;
        root->left = nullptr;
        root->right = nullptr;
    }

    BinaryTree(const BinaryTree &bitree) //Copy constructor
    {
        root = CopyNode(root,bitree.root);
    }

    virtual ~BinaryTree() // Destructor
    {
        DeleteNode(root);
    }

public: // Operators overload

    bool operator=(const BinaryTree &bitree) const{
        return isBiTreeEqual(this->root,bitree->root);
    }

public: // Basic functions

    bool isBiTreeEqual(const BiTree root_1,const BiTree root_2) const{
        if(!root_1 && !root_2)
            return true;
        if(root_1 && root_2)
            return (root_1->data == root_2->data) &&
                 isBiTreeEqual(root_1->left,root_2->left)
                 && isBiTreeEqual(root_1->right,root_2->right);
        else
            return false;
    }

    virtual BiTree CopyNode(BiTree Node_copy,const BiTree Node_original) throw()
    {
        if(!Node_original)
        {
            Node_copy = nullptr;
            return nullptr;
        }

        Node_copy = new BiNode;
        Node_copy->data = Node_original->data;

        Node_copy->left = CopyNode(Node_copy->left,Node_original->left);
        Node_copy->right = CopyNode(Node_copy->right,Node_original->right);

        return Node_copy;
    }

    virtual void DeleteNode(BiTree root) throw()
    {
        if(!root) return;
        if(root->left)
            DeleteNode(root->left);
        if(root->right)
            DeleteNode(root->right);
        delete root;
    }

    virtual BiTree AddNode_left(const T &data,BiTree root) throw()
    {
        if(!root) return nullptr;
        if(root->left) return nullptr;

        BiTree tmp = new BiNode;
        tmp->data = data;
        tmp->left = nullptr;
        tmp->right = nullptr;
        root->left = tmp;
        return tmp;
    }

    virtual BiTree AddNode_right(const T &data,BiTree root) throw()
    {
        if(!root) return nullptr;
        if(root->right) return nullptr;

        BiTree tmp = new BiNode;
        tmp->data = data;
        tmp->left = nullptr;
        tmp->right = nullptr;
        root->right = tmp;
        return tmp;
    }

    virtual int GetDepth(const BiTree root) const
    {
        if(!root) return 0;
        return 1 + MAX(GetDepth(root->left),GetDepth(root->right));
    }

public: // Auxiliary functions

    virtual std::vector<T> LevelOrder_traverse() const throw(){
        BiTree tmp;
        std::vector<T> ret;
        std::deque<BiTree> deque;
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

    virtual std::vector<T> PreOrder_traverse() const throw(){
        BiTree tmp;
        std::vector<T> ret;
        std::stack<BiTree> stack;
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

    virtual std::vector<T> InOrder_traverse() const throw(){
        BiTree tmp;
        std::vector<T> ret;
        std::stack<BiTree> stack;
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
        BiTree tmp,rear = nullptr;
        std::vector<T> ret;
        std::stack<BiTree> stack;
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

    virtual int GetLeafNum(const BiTree root) const throw() {
        if(!root) return 0;
        if(!root->left && !root->right) return 1;
        return GetLeafNum(root->left) + GetLeafNum(root->right);
    }

};

#endif //BITREE_H_INCLUDED