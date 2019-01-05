#ifndef _AuthenticatedSkipList_H_
#define _AuthenticatedSkipList_H_

#include "SkipList.h"
#include "AuthenticatedNode.h"

/*------------------------------------------------------------------------
 * Class:   AuthenticatedSkipList.h and AuthenticatedSkipList.cpp
 *
 * Purpose:   Definition and implementation of essential methods for
 *	      authenticated skip list
 *
 * Syntax:
 *               modulus  - composite number N of Group ZN*
 *               element  - generator of Group ZN*
 *
 *------------------------------------------------------------------------
 */

class AuthenticatedSkipList : public SkipList {
    
public:
    // constructor for authenticated skip list
    AuthenticatedSkipList(ZZ compositeN = 1, ZZ randomG = 0);
    // constructor for other types of skip list
    AuthenticatedSkipList(Type T, ZZ compositeN = 1, ZZ randomG = 0);
    AuthenticatedSkipList(bool n, Type T, ZZ compositeN = 1, ZZ randomG = 0);
    virtual void multiUpdate(vector<DiffSkipListSimple*> diffs){}
    virtual Node* modify(Key key, Data data, ZZ givenTag = 0, stack<Node*>* s1 = NULL);
    virtual Node* insert(Key key, Data data, ZZ givenTag = 0, stack<Node*>* s1 = NULL, int givenLevel = -1);
    virtual Node* append(Data data, ZZ givenTag = 0, stack<Node*>* s1 = NULL) {}
    virtual Node* remove(Key key, stack<Node*>* s1 = NULL);
    
protected:
    virtual void calculateHash(stack<Node*>* s);
    virtual ZZ getModulus();
    virtual ZZ getElement();
    ZZ modulus;
    ZZ element;
    
private:
    
};

#endif /*_AuthenticatedSkipList_H_*/