/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "AuthenticatedNode.h"

AuthenticatedNode::AuthenticatedNode(Key k, int lvl)
: Node(k, lvl){ tag = 0; hash = NULL;}

void AuthenticatedNode::print() {
    Node::print(); cout<<"hash: "<<getHash()<<endl;
}

bool AuthenticatedNode::isIntersectionOfTwo(bool canGoDown, int r1, int r2) {
    /*NOT IMPLEMENTED*/
}
bool AuthenticatedNode::isIntersecting(bool canGoDown, vector<int> chalIndex, int startKey, int* end, int rankState) {
    /*NOT IMPLEMENTED*/
}

ZZ AuthenticatedNode::getHash() { return hash; }
void AuthenticatedNode::setHash(ZZ h){ hash = h;}

ZZ AuthenticatedNode::getTag(ZZ modulus, ZZ element) {
    if(tag == 0) {
        // if there is no tag present, compute it
        tag = PowerMod(element, getData().getValue(), modulus);
    }
    return tag;
}

void AuthenticatedNode::setTag(ZZ t) { tag = t; }

// gets N and g as modulus and element and returns hash of below node
// which can possibly be a tag, therefore N and g are needed
ZZ AuthenticatedNode::getBelowHash(ZZ modulus, ZZ element){
    if(getBelow() != NULL)
        return ((AuthenticatedNode*)getBelow())->getHash();
    if(modulus != 0)
        tag = getTag(modulus, element);
    return tag;
}
ZZ AuthenticatedNode::getAfterHash(){
    if(getAfter() != NULL)
        return ((AuthenticatedNode*)getAfter())->getHash();
    else return to_ZZ(0);
}
// returns hash(level, key, hashBelow, HashAfter)
void AuthenticatedNode::calculateHash(ZZ modulus, ZZ element) {
    vector<ZZ> vectorZZ;
    vectorZZ.push_back(to_ZZ(getLevel()));
    vectorZZ.push_back(to_ZZ(getKey().getValue()));
    vectorZZ.push_back(getBelowHash(modulus, element));
    vectorZZ.push_back(getAfterHash());
    hash = Hash::hash(vectorZZ, DPDPCONST::hashAlg);
}
ZZ AuthenticatedNode::calculateHashRec()
{
    //this->print();
    //cout<<"theKey: "<<getKey().getValue()<<endl;
    if(hash != NULL)
        return getHash();
    else {
        vector<ZZ> vectorZZ;
        vectorZZ.push_back(to_ZZ(getLevel()));
        vectorZZ.push_back(to_ZZ(getKey().getValue()));
        vectorZZ.push_back(getBelowHashRec());
        vectorZZ.push_back(getAfterHashRec());
        hash = Hash::hash(vectorZZ, DPDPCONST::hashAlg);
        return hash;
    }
    
}
ZZ AuthenticatedNode::getBelowHashRec(){	 //used for dummy sl
    if(getBelow() != NULL)
        return ((AuthenticatedNode*)getBelow())->calculateHashRec();
    //if(modulus != 0)
				//tag = getTag(modulus, element);	//Therefore no tag calculation considered
    return tag;
}
ZZ AuthenticatedNode::getAfterHashRec(){
    
    if(getAfter() != NULL)
        return ((AuthenticatedNode*)getAfter())->calculateHashRec();
    else return to_ZZ(0);
}
