#ifndef _DPDPFlexProver_H_
#define _DPDPFlexProver_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/lexical_cast.hpp>
#include <iostream>
#include <vector>
#include "connection.hpp" // Must come before boost/serialization headers.
#include <boost/serialization/vector.hpp>
#include <boost/thread.hpp>

#include "GroupRSA.h"
#include "RankBasedSkipList.h"
#include "DPDPCONST.h"
#include "RankBasedSkipList.h"


using namespace s11n_example;

/*------------------------------------------------------------------------
 * Pro	gram:   	DPDPFlexProver
 *
 * Purpose:   	this class is where the server operations are implemented. Server can respond to challenges through this
 *				class. Each client should have one instance of this object saved at the server side
 *
 *
 * Note:
 *
 *------------------------------------------------------------------------
 */

class DPDPFlexProver {
public:
    
    DPDPFlexProver();
    DPDPFlexProver(int* seed, ZZ N, ZZ g/*, connection_ptr con*/);
    ~DPDPFlexProver();
    //PLEASE PAY ATTENTION: this full-rewrite is done from last to first
    //This function just divides the file into blocks using the seed for both client and server being able to
    //generate the same skip list
    //  ZZ divideAndBuildSkipList_FullRewrite(int seed, char* fileName, vector<ZZ> tags, int blockLength );
    
    //PLEASE PAY ATTENTION: this full-rewrite is done from last to first
    //This function just divides the file into blocks using the seed for both client and server being able to
    //generate the same skip list
    ZZ divideAndBuildSkipList_FullRewrite(int seed, char* fileName, vector<ZZ> tags, int blockLength );
    //PLANETLAB VERSION OF THE SAME FUNCTION
    //PLEASE PAY ATTENTION: this full-rewrite is done from last to first
    //This function just divides the file into blocks using the seed for both client and server being able to
    //generate the same skip list
    ZZ divideAndBuildSkipList_FullRewritePL(int seed, char* fileName, vector<ZZ> tags, int blockLength, RankBasedSkipList* rbsl = NULL);
    /*This function just adds blocks up by multiplying each by random variables gotten out from the seed which came from the challenger*/
    ZZ genBlockSum (ZZ seed, vector<ZZ> fileBlocks, int stat);
    Node* SearchMulti(Node* currentNode, vector<int> chalIndex, int* start, int end, int rankState, ZZ N, ZZ g, vector<ProofNode>* proofVector, stack<State*>* interStack);
    /*GenMultiProof:
     *Upon receipt of the challenges and the random seed
     from the client, the server runs genMultiProof algorithm in order to get tags, file blocks
     *and proof path for the challenged indices. Algorithm searches for the leaf node of each
     *challenged index and stores all nodes across the search path in the proof vector. How-
     *ever, we have observed that regular searching for each particular node is inefficient. If
     *we start from the root for each challenged block, there will be a lot of replicated proof
     *nodes.
     */
    vector<ProofNode> GenMultiProof(Node *root, vector<int> chalIndex, vector<ZZ>* tags, vector<ZZ>* fileBlocks/*, char* fileNameBase = ""*/);
    //PL Test replication of the function. instead of the name from the flexlist, it always returns a preset set of file blocks.
    vector<ProofNode> GenMultiProofPL(vector<int> chalIndex, vector<ZZ>* tags, vector<ZZ>* fileBlocks, char* fileNameBase = "");
    /* 4 helper functions for basic testing*/
    void update(int index, bool type, int level = 0, ZZ tag = to_ZZ(0), ZZ block = to_ZZ(0));
    void updateDelete(int index);
    void updateInsert(int index, int level = 0, ZZ tag = to_ZZ(0), Data block = NULL);
    void updateModify(int index, int level = 0, ZZ tag = to_ZZ(0), Data block = NULL);
    //PLEASE PAY ATTENTION: this full-rewrite is done from last to first
    //An old function left here for later referances
    ZZ fullRewrite(ZZ seed, char* blockName, vector<ZZ> tags, int numOfBlocks);
    //PLEASE PAY ATTENTION: this full-rewrite is done from last to first
    //for now, prepared as a test function for reference: used in DPDPTestWithFlexList in Test.cpp
    ZZ fullRewrite(ZZ seed, vector<ZZ> *fileBlocks, vector<ZZ> tags, int numOfBlocks, int defaultBlockLength);
    ZZ getN();
    void setN(ZZ n);
    ZZ getG();
    void setG(ZZ G);
    RankBasedSkipList* getSL();
    void setSL(RankBasedSkipList* rbsl);
    RankBasedSkipList* getRBSL();
    void setFileName(string fn);
    string getFileName();
    void setFileNameBase(string fn);
    string getFileNameBase();
    void setClientIP(string IP);
    string getClientIP();
    void setNumBlocks(int num);
    int getNumBlocks();
    
    RankBasedSkipList *sl;
    
private:
    
    ZZ N,g;
    int numberOfBlocks;
    //char* fileName;
    string fileName;
    string fileNameBase;
    string clientIP;
    //connection_ptr conn;
};

#endif /* _DPDPFlexProver_H_ */

