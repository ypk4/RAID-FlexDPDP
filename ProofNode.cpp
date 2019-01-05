/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "ProofNode.h"
#include "Node.h"
#include "Timer.h"

ProofNode::ProofNode( int lvl, int r )
: level(lvl), rank(r){
    endFlag = false; interFlag = false; hash = to_ZZ(100); rgtOrDwn = NULL; length = 0;
}

ProofNode::ProofNode(ZZ h, bool rgtOrDwn, bool interFlag, bool endFlag, int level, int rank, int length )
: hash(h), rgtOrDwn(rgtOrDwn), interFlag(interFlag), endFlag(endFlag), level(level), rank(rank), length(length){}
ZZ ProofNode::getHash() {
    return hash;
}

void ProofNode::setHash(ZZ hashOfNotIncluded) {
    hash = hashOfNotIncluded;
    /*cout<<"!!!!!!setHash: "<<hash<<endl;*/
}

int ProofNode::getLength() { return length; }
void ProofNode::setLength( int len ) { length = len;}
int ProofNode::getLevel() { return level; }
int ProofNode::getRank() { return rank; }
bool ProofNode::isInter() { return interFlag; }
void ProofNode::setInterFlag() { interFlag = true; }
bool ProofNode::isEnd() { return endFlag; }
void ProofNode::setEndFlag() { endFlag = true; }
bool ProofNode::getRgtOrDwn() { return rgtOrDwn; }
void ProofNode::set(ZZ hashOfNotIncluded, bool includedPos) {
    setHash(hashOfNotIncluded); rgtOrDwn = includedPos;
}

ZZ ProofNode::calculateHash(int level, int rank, ZZ hashBelow, ZZ hashAfter) {
    vector<ZZ> vectorZZ;
    vectorZZ.push_back(to_ZZ(level));
    vectorZZ.push_back(to_ZZ(rank));
    vectorZZ.push_back(hashBelow);
    vectorZZ.push_back(hashAfter);
    return Hash::hash(vectorZZ, DPDPCONST::hashAlg);
}

void ProofNode::print() {
    cout<<"<--------------->"<<endl;
    cout<<"-PROOF NODE-"<<endl;
    cout<<"Rank    : "<< getRank() 		<<endl;
    cout<<"Level   : "<< getLevel()		<<endl;
    cout<<"isInter : "; ((interFlag)?cout<<"true":cout<<"false");cout<<endl;
    cout<<"isEnd   : "; ((endFlag)?cout<<"true":cout<<"false");cout<<endl;
    cout<<"rgtOrDwn: "; ((rgtOrDwn)?cout<<"true":cout<<"false");cout<<endl;
    cout<<"Hash    : "<< getHash()		<<endl;
    cout<<"Length  : "<< length		<<endl;
    cout<<"<--------------->"<<endl;
}

//template <class Archive>
//void ProofNode::serialize(Archive& ar, const unsigned int ver) {
//    ar  & auto_nvp(hash)
//    & auto_nvp(rgtOrDwn)
//    & auto_nvp(interFlag)
//    & auto_nvp(endFlag)
//    & auto_nvp(level)
//    & auto_nvp(rank)
//    & auto_nvp(length);
//}
