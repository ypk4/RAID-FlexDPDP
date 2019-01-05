#ifndef _SkipList_H_
#define _SkipList_H_

#define INT_MAX 2147483647
#include "DiffSkipListSImple.h"
#include <stack>
#include "RankBasedNode.h"
#include "logger.h"
#include <boost/thread.hpp>
#include <boost/date_time.hpp>



/*------------------------------------------------------------------------
 * Class:   SkipList.h and SkipList.cpp
 *
 * Purpose:   Definition and implementation of essential methods for
 *	      any skip list
 *
 * Syntax:
 *              root - leftmost top node, starting point for any query
 *		T - defines type of a skip list
 *------------------------------------------------------------------------
 */

class SkipList {
    
public:
    
    SkipList(Type type = SL);
    SkipList(bool n, Type type);
    ~SkipList();
    
    virtual void multiUpdate(vector<DiffSkipListSimple*> diffs){}
    // basic functions for skip list
    virtual Node* search(Key key, stack<Node*>* s = NULL, bool nullifyHashes = false);
    virtual Node* search(Key *key, stack<Node*>* s = NULL, bool nullifyHashes = false);
    virtual Node* modify(Key key, Data data, ZZ givenTag = 0, stack<Node*>* s = NULL, int* oldLength = NULL, bool nullifyHashes = false);
    virtual Node* insert(Key key, Data data, ZZ givenTag = 0, stack<Node*>* s = NULL, int givenLevel = -1, bool nullifyHashes = false);
    virtual Node* append(Data data, ZZ givenTag = 0, stack<Node*>* s1 = NULL) {}
    virtual Node* remove(Key key, stack<Node*>* s = NULL, bool nullifyHashes = false);
    
    virtual Node* build(vector<Data> Blocks, vector<int> Levels, vector<ZZ> Tags, int maxLevel, int order = 0, int partCount = 1);
    virtual Node* build(vector<ZZ> Blocks, vector<int> Levels, vector<ZZ> Tags, int maxLevel, int order = 0, int partCount = 1);
    
    virtual void print();
    virtual void printLog();
    
    virtual int getSize();
    Node* getRoot();
    void setRoot(Node* newRoot);
    virtual void incrementSize();
    virtual void decrementSize();
    
    bool testPrintHelper(Node *node, stack<ZZ>* m);
    bool testSkipList(stack<ZZ>* m);
    void printHelper(Node node);
    
    
protected:
    
    Node* root;
    
    Type T;
    
    virtual Node* buildLeaf(Data block, ZZ tag, vector<Node*>* topTower);
    virtual Node* buildLeaf(ZZ block, ZZ tag, vector<Node*>* topTower);
    
    virtual void buildTower(Node* previousNode, int level, vector<Node*>* topTower);
    
    // helper functions which are widely used in main functions
    // create missing node, i.e. create new node for new connection in a skip list
    virtual bool createMissingNode(Node** previousNode, Node** currentNode, Key* key, int levelToInsert,  bool canGoDown, stack<Node*> *s = NULL);
    // removes unnecessary node
    virtual bool removeNode(Node** previousNode, Node** currentNode, Key* key, stack<Node*>* s = NULL);
    virtual bool deleteNode(Node** previousNode, Node** currentNode, Key* key, stack<Node*>* s = NULL);
    // traverse function, which moves current node to desired position in a skip list
    bool nextPos(Node** previousNode, Node** currentNode, Key* key, int levelToInsert = -1, bool npi = false, stack<Node*>* s = NULL, bool nullifyHashes = false);
    // helper function of nexPos, checks whether a move to below or after is possible
    virtual bool oneStep(Node** previousNode, Node** currentNode, Key* key, int levelToInsert =-1, bool npi = false, stack<Node*>* s = NULL, bool nullifyHashes = false);
    // make move to below or after
    bool oneStepDo(Node** previousNode, Node** currentNode, Node* nextNode , stack<Node*>* s = NULL, bool nullifyHashes = false);
    
    int tossCoins();
    
    virtual ZZ getModulus() {}
    
    virtual ZZ getElement() {}
    
private:
    
    int size;
    
    void printLogHelper(Node node);
    
    Node* createNode(Key k=0, int lvl=0);
};

#endif /*_SkipList_H_*/