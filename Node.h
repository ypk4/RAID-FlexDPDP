#ifndef _Node_H_
#define _Node_H_

//#define DEBUG
//#define DEBUGRB
//#define DEBUGASL
//#define DEBUGSL
//#define DETAILEDRB
//#define DETAILEDASL
//#define DETAILEDSL

#include <stdlib.h>
#include <string>
#include <iostream>
#include "Data.h"
#include "Key.h"
#include "Hash.h"
#include "DPDPCONST.h"
//#include "DiffSkipList.h"

/*------------------------------------------------------------------------
 * Class:   Node.h
 *
 * Purpose:   Class for definition of nodes in a skip list
 *
 *------------------------------------------------------------------------
 */

using namespace std;

// there can be different types of skip lists
// if skip list is rank based then all its nodes are rank based nodes
// Note: Flexlist also uses rank based nodes
enum Type { SL, AuthenticatedSL ,RankBasedSL, FlexSL };

class Node {
    
public:
    Node(Key k = 0, int lvl = 0);
    ~Node();
    // upon deletion of a skip list this function is called
    void freeAll();
    // set and get functions data in the node
    Data getData();
    void setData(Data d);
    // level
    int getLevel();
    void setLevel(int lvl);
    // key
    Key getKey();
    void setKey(Key k);
    // after link
    Node* getAfter();
    void setAfter(Node* n);
    // Leaf level connection
    Node* getLeafAfter();
    void setLeafAfter(Node* n);
    // below link
    Node* getBelow();
    void setBelow(Node* n);
    // getting key values from its neighbor (after or below)
    int getAfterKeyValue();
    int getBelowKeyValue();
    virtual void setHash(ZZ h){}
    // print values inside the node
    virtual void print();
    char* printString();
    void printLog();
    // traverse functions
    virtual bool canGoRight(Key key);
    virtual bool canGoDown(Key key, int levelToInsert);
    
    // get hash values
    virtual ZZ getHash(){}
    virtual ZZ getAfterHash(){}
    virtual ZZ getBelowHash(ZZ modulus, ZZ element){}
    // to calculate its own hash value
    virtual void calculateHash(ZZ modulus, ZZ element) {}
    // to get if there is a tag or calculate its own tag value
    virtual ZZ getTag(ZZ modulus, ZZ element){}
    virtual void setTag(ZZ t) {}
    // checking if the node is intersection point
    virtual bool isIntersectionOfTwo(bool canGoDown, int r1, int r2){}
    virtual bool isIntersecting(bool canGoDown, vector<int> chalIndex, int startKey, int* end, int rankState){}
    virtual void calculateRank(int diff = 0) {}
    
protected:
    
    Key key;
    
private:
    
    // values inside a node
    Data data;
    int level;
    Node* after;
    Node* below;
    Node* leafAfter;
    
    friend class RankBasedSkipList;
    friend class AuthenticatedSkipList;
    friend class SkipList;
};

#endif /*_Node_H_*/