#ifndef _AuthenticatedNode_H_
#define _AuthenticatedNode_H_

#include "Node.h"
#include "ProofNode.h"
#include "State.h"

/*------------------------------------------------------------------------
 * Class:   AuthenticatedNode
 *
 * Purpose:   class supports creation of authenticated nodes
 *
 *------------------------------------------------------------------------
 */

class AuthenticatedNode : public Node {
    
public:
    AuthenticatedNode(Key k, int lvl);
    
    virtual void print();
    
    virtual bool isIntersectionOfTwo(bool canGoDown, int r1, int r2);
    virtual bool isIntersecting(bool canGoDown, vector<int> chalIndex, int startKey, int* end, int rankState);
    
    virtual ZZ getHash();
    void setHash(ZZ h);
    ZZ getTag(ZZ modulus, ZZ element);
    
    void setTag(ZZ t);
    
    // gets N and g as modulus and element and returns hash of below node
    // which can possibly be a tag, therefore N and g are needed
    ZZ getBelowHash(ZZ modulus = 0, ZZ element = 0);
    ZZ getAfterHash();
    // returns hash(level, key, hashBelow, HashAfter)
    virtual void calculateHash(ZZ modulus, ZZ element);
    virtual ZZ calculateHashRec();
    virtual ZZ getBelowHashRec();
    virtual ZZ getAfterHashRec();
    virtual void calculateRank(int diff = 1){}
    
protected:
    ZZ hash;
    ZZ tag;
    
private:
    friend class RankBasedSkipList;
    friend class AuthenticatedSkipList;
};



#endif /*_AuthenticatedNode_H_*/