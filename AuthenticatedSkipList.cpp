/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "AuthenticatedSkipList.h"

AuthenticatedSkipList::AuthenticatedSkipList(ZZ compositeN, ZZ randomG)
: SkipList(AuthenticatedSL){
    getRoot()->getAfter()->calculateHash(compositeN, randomG);
    getRoot()->getBelow()->calculateHash(compositeN, randomG);
    modulus = compositeN;
    element = randomG;
}

AuthenticatedSkipList::AuthenticatedSkipList(Type T, ZZ compositeN, ZZ randomG)
: SkipList(T){
    getRoot()->getAfter()->calculateHash(compositeN, randomG);
    getRoot()->getBelow()->calculateHash(compositeN, randomG);
    modulus = compositeN;
    element = randomG;
}

AuthenticatedSkipList::AuthenticatedSkipList(bool n, Type T, ZZ compositeN, ZZ randomG)
: SkipList(n, T){
    modulus = compositeN;
    element = randomG;
}

Node* AuthenticatedSkipList::modify(Key key, Data data, ZZ givenTag, stack<Node*>* s1) {
#ifdef DEBUGASL
    char * log = new char[250];	sprintf(log,
                                        "%25s ( with key: %d %s %f )"," AuthenticatedSkipList::modify", key.getValue(), "Data:", data.getValue());
    logger::log(log);
#endif
    
    stack<Node*> s;
    // modify using regular skip list method
    Node* returnNode = SkipList::modify(key, data, 0, &s);
    // change tag and recalculate hash values along the modify path
    returnNode->setTag(givenTag);
    calculateHash(&s);
    
#ifdef DETAILEDASL
    printLog();
#endif
    
    return returnNode;
}

Node* AuthenticatedSkipList::insert(Key key, Data data, ZZ givenTag, stack<Node*>* s1, int givenLevel) {
#ifdef DEBUGASL
    char * log = new char[250];	sprintf(log,
                                        "%25s ( with key: %d %s %f %s %2d )"," AuthenticatedSkipList::insert", key.getValue(), "Data:", data.getValue(), "level: ",givenLevel);
    logger::log(log);
#endif
    
    stack<Node*> s;
    // call for skip list insert
    Node* returnNode = SkipList::insert(key, data, 0, &s, givenLevel);
    // change tag and recalculate hash values along the insert path
    returnNode->setTag(givenTag);
    calculateHash(&s);
    
#ifdef DETAILEDASL
    printLog();
#endif
    
    return returnNode;
}

Node* AuthenticatedSkipList::remove(Key key, stack<Node*>* s1) {
    
#ifdef DEBUGASL
    char * log = new char[250];	sprintf(log,
                                        "%25s ( with key: %d )"," AuthenticatedSkipList::remove", key.getValue());
    logger::log(log);
#endif
    
    stack<Node*> s;
    //remove element and recalculate hash values along the remove path
    Node* node = SkipList::remove(key, &s);
    calculateHash(&s);
    
#ifdef DETAILEDASL
    printLog();
#endif
    
    return node;
}

void AuthenticatedSkipList::calculateHash(stack<Node*>* s) {
#ifdef DETAILEDASL
    logger::log("calculateHash:");
#endif
    // input: stack s, filled with nodes along the path
    // output: recalculated hash values at each node in stack s
    while(!s->empty())
    {
        Node* node = s->top();
        node->calculateHash(getModulus(), getElement());
        s->pop();
#ifdef DETAILEDASL
        node->printLog();
#endif
    }
    root->calculateHash(getModulus(), getElement());
}

ZZ AuthenticatedSkipList::getModulus()
{
    return modulus;
}

ZZ AuthenticatedSkipList::getElement()
{
    return element;
}


