#ifndef _ProofNode_H_
#define _ProofNode_H_

#include "Node.h"
#include "Timer.h"
/*------------------------------------------------------------------------
 * Program:   	ProofNode
 *
 * Purpose:   	this is a class in which all elements required for an hash calculation for a particular node on a proof path
 *				is stored to be sent. It is also designed to be sent over the network
 *
 *
 * Note:      	This class is mostly used and especially written to be used in generateMultiProof and verifyMultiProof
 *				algorithms
 *
 *------------------------------------------------------------------------
 */

class ProofNode {
    
public:
    
    ProofNode( int lvl=0, int r=0 );
    ProofNode(ZZ h, bool rgtOrDwn, bool interFlag, bool endFlag, int level, int rank, int length );
    ZZ getHash();
    void setHash(ZZ hashOfNotIncluded);
    int getLength();
    void setLength(int len);
    int getLevel();
    int getRank();
    bool isInter();
    void setInterFlag();
    bool isEnd();
    void setEndFlag();
    bool getRgtOrDwn();
    void set(ZZ hashOfNotIncluded, bool includedPos);
    static ZZ calculateHash(int level, int rank, ZZ hashBelow, ZZ hashAfter);
    void print();
    
private:
    
    ZZ hash;
    bool rgtOrDwn; //rgt:true dwn:false
    bool interFlag;
    bool endFlag;
    int level;
    int rank;
    int length; //for end and inter case when using flexlist
    friend class RankBasedSkipList;
    friend class AuthenticatedSkipList;
    
    friend class boost::serialization::access;
    template <class Archive>
    void serialize(Archive& ar, const unsigned int ver) {
        ar  & auto_nvp(hash)
        & auto_nvp(rgtOrDwn)
        & auto_nvp(interFlag)
        & auto_nvp(endFlag)
        & auto_nvp(level)
        & auto_nvp(rank)
        & auto_nvp(length);
    }

    
};

#endif /*_ProofNode_H_*/