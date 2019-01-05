#ifndef _DPDPFlexVerifier_H_
#define _DPDPFlexVerifier_H_

#include "GroupRSA.h"
#include "Timer.h"
#include "RankBasedSkipList.h"
#include "MultiExp.h"
#include "DummyRankBasedNode.h"


/*------------------------------------------------------------------------
 * Program:   	DPDPFlexVerifier
 *
 * Purpose:   	this class is where the client operations are implemented. Client can challege and revirfy through this
 *				class. Each client should have one instance of this object to verify the responses came from the prover
 *
 *
 * Note:
 *
 *------------------------------------------------------------------------
 */

class DPDPFlexVerifier {
    
public:
    
    DPDPFlexVerifier(vector<ZZ> *Data, ZZ *N, ZZ *g);
    DPDPFlexVerifier();
    ~DPDPFlexVerifier();
    void generateGroup(ZZ *N, ZZ *g);
    //int seed, char* fileName, vector<ZZ> *tags, int blockLength
    //long long divideAndBuildSkipList_sendFileToServer(ZZ seed, vector<ZZ> *tags, char* fileName, int blockLength);
    long long divideAndBuildSkipList_sendFileToServer(ZZ seed, vector<ZZ> *tags, char* fileName, int blockLength);
    long long divideAndBuildSkipList_sendFileToServerPL(ZZ seed, vector<ZZ> *tags, char* fileName, int blockLength);
    //for now, prepared as a test function for reference: used in DPDPTestWithFlexList in Test.cpp
    //used with fullRewrite from DPDPFlexProver
    long long sendFileToServer(ZZ seed, vector<ZZ> *tags, int numOfBlocks, char* blockName = "aBlockOf16KiB.txt");
    bool verifyRewrite(ZZ rootRewrite);
    ZZ calculateTag(ZZ data, ZZ modulus, ZZ element, ZZ p, ZZ q, ZZ order = to_ZZ(1));
    /*THIS NEEDS WORK SINCE CHALLENGING REQUIRES THE START POINTS OF BLOCKS FOR NOT TO CHALLENGE SAME BLOCK TWICE
     OR THERE SHOULD BE A DISTANCE OF MAXIMUM BLOCK SIZE BETWEEN TWO CHALLENGED BLOCKS(WHICH REQUIRES A NEW GENERATECHALLENGES MECHANISM)*/
    vector<int> generateChallenges(int nChallenges, int numBlocks, int* seed);
    bool verifyTags(vector<ZZ> tags, ZZ blockSum, long long* timer);
    void updateRankSum(int rankDiff, vector<int>* vecRankSum, int start, int end);
    bool VerifyMultiProof(vector<int> chalIndex, vector<ProofNode> proofVector, vector<ZZ> tags, ZZ blockSum = NULL);
    bool VerifyMultiProofWOchalIndex(vector<ProofNode> proofVector, vector<ZZ> tags, ZZ blockSum = NULL);
    bool verifyRankSum(vector<int> lengthsOfChallengedNodes, vector<int> chalIndex, vector<int> vecRankSum);
    ZZ genBlockSum (ZZ seed, vector<ZZ> fileBlocks, int stat);
    bool verifyInsert(vector<int> chalIndex, vector<ProofNode> proofVector, vector<ZZ> tags, ZZ blockSum, ZZ tag, int level,int newDataSize = DPDPCONST::blockSize, ZZ newMetaData = to_ZZ(1));
    bool verifyModify(vector<int> chalIndex, vector<ProofNode> proofVectorBefore, vector<ProofNode> proofVectorAfter, vector<ZZ> tagsBefore,vector<ZZ> tags, ZZ blockSum, ZZ blockSumAfter, ZZ tag, int level, int blockSize = DPDPCONST::blockSize, int newDataSize = 16384, ZZ newMetaData = to_ZZ(1));
    bool verifyModify(vector<int> chalIndex, vector<ProofNode> proofVectorBefore, vector<ZZ> tagsBefore, ZZ blockSum, ZZ tag, int blockSize = DPDPCONST::blockSize, int newDataSize = 16384, ZZ newMetaData = to_ZZ(1));
    bool verifyDelete(vector<int> chalIndex, vector<ProofNode> proofVector, vector<ZZ> tags, ZZ blockSum, ZZ newMetaData = to_ZZ(1));
    ZZ calculateNewRoot(vector<int> chalIndex, vector<ProofNode> proofVector, vector<ZZ> tags, ZZ blockSum);
    bool OldVerifyInsert(vector<int> chalIndex, vector<ProofNode> proofVector, vector<ZZ> tags, ZZ blockSum, ZZ tag, int level);
    ZZ calculateRemainingHashes(int i, ZZ hash, int rankDiff, vector<ProofNode> proofVector);
    bool OldVerifyDelete(vector<int> chalIndex, vector<ProofNode> proofVector, vector<ZZ> tags, ZZ blockSum);
    static RankBasedSkipList* generateDummySkipList(DPDPFlexVerifier* verifier,vector<ProofNode> firstProof, vector<ZZ> tags);
    
    ZZ getSeed();
    void setSeed(ZZ s);
    ZZ getN();
    void setN(ZZ n);
    ZZ getG();
    void setG(ZZ G);
    ZZ getP();
    void setP(ZZ P);
    ZZ getQ();
    void setQ(ZZ Q);
    ZZ getMetaData();
    void setMetaData(ZZ MD);
    RankBasedSkipList* getRBSL();
    //	RankBasedSkipList* getRBSLTEMP();
    vector<Data>* getTempDatas();
    
    int getSize();
    void setSize(int s);
private:
    ZZ N,g,p,q, MetaData, seed;
    vector<ZZ> *data; //*
    int size;////////////////////////////////////////////////////XXX
    RankBasedSkipList* rbsl;
    //RankBasedSkipList* rbslTemp; // is here for alteration management, actually not better than just a linked list.
    vector<Data>* tempDatas;
    
    
};

#endif /* _DPDPFlexVerifier_H_ */