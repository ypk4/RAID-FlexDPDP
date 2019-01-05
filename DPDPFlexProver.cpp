/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "DPDPFlexProver.h"

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

DPDPFlexProver::DPDPFlexProver() {
    //cout<<"PROVER CREATED WITH EMPTY CONSTRUCTOR"<<endl;
}


DPDPFlexProver::DPDPFlexProver(int* seed, ZZ N, ZZ g/*, connection_ptr con*/) {
    //cout<<"PROVER IS CONSTRUCTED!"<<endl;
    setN(N);setG(g);
    
    sl = new RankBasedSkipList(true,N,g);
    
}

DPDPFlexProver::~DPDPFlexProver() {
    //cout<<"DPDPFlexProver is destructed."<<endl;
    //delete sl;
}
//PLEASE PAY ATTENTION: this full-rewrite is done from last to first
//This function just divides the file into blocks using the seed for both client and server being able to
//generate the same skip list
/*	ZZ DPDPFlexProver::divideAndBuildSkipList_FullRewrite(int seed, char* fileName, vector<ZZ> tags, int blockLength ) {
 
 int blockCounter=0;//XXX:will be read from file and be written to file at the end XXX
 char counterFileName[80] = "connectedClients/";
 strcat(counterFileName, clientIP.c_str());
 strcat(counterFileName, "/counter.txt");
 blockCounter = Data::readCounterInt(counterFileName);
 //blockCounter = Data::readCounterInt("serverFolder/counter.txt");
 char fileNametxt[80] = "";
 strcat(fileNametxt, fileName);
 strcat(fileNametxt, ".txt");
 
 Data d(fileNametxt);
 vector<char> file = d.getBytesFromFile();
 vector<char>::iterator it= file.end();
 
 int tagsItr = 0;
 srand ( seed );//SetSeed( seed );
 for(;it>file.begin() ;blockCounter++)
 {
 it -= blockLength+1;
 if(it < file.begin())
 it = file.begin();
 vector<char> aBlock;
 aBlock.clear();
 
 //cout<<"it:"<<*it<<endl;
 std::copy((it < file.begin()) ? file.begin() : it, it + blockLength, std::back_inserter(aBlock));
 
 char isim[80] = "";
 char str[100];
 sprintf(str, "%d", blockCounter);
 strcat(isim, fileName);
 strcat(isim, str);
 strcat(isim, ".txt");// = fileName + blockCounter + ".txt";
 
 Data d(aBlock,isim,blockCounter);
 sl->insert(Key(0), d, tags[tagsItr++]);
 //Node* rbn = sl->search(1);
 }
	
	
 //Delete counter file and recreate with new counter...
 remove( counterFileName);
 
 
 ofstream counterFile;
 counterFile.open(counterFileName);
 counterFile << blockCounter;
 counterFile.close();
 
 
 
 return sl->getRoot()->getHash();
	}*/

//PLEASE PAY ATTENTION: this full-rewrite is done from last to first
//This function just divides the file into blocks using the seed for both client and server being able to
//generate the same skip list
ZZ DPDPFlexProver::divideAndBuildSkipList_FullRewrite(int seed, char* fileName, vector<ZZ> tags, int blockLength ) {
    
    int blockCounter=0;//XXX:will be read from file and be written to file at the end XXX
    /*blockCounter = Data::readCounterInt("serverFolder/counter.txt");
     char fileNametxt[80] = "";
     strcat(fileNametxt, fileName);
     strcat(fileNametxt, ".txt");*/
    
    char counterFileName[80] = "connectedClients/";
    strcat(counterFileName, clientIP.c_str());
    strcat(counterFileName, "/counter.txt");
    blockCounter = Data::readCounterInt(counterFileName);
    //blockCounter = Data::readCounterInt("serverFolder/counter.txt");
    char fileNametxt[80] = "";
    strcat(fileNametxt, fileName);
    strcat(fileNametxt, ".txt");
    
    Data d(fileNametxt);
    vector<char> file = d.getBytesFromFile();
    vector<char>::iterator it= file.end();
    
    int tagsItr = 0;
    int real = 1;
    cout<<"0. test"<<endl<<"else.real"<<endl;
    //cin>>real;
    srand ( seed );//SetSeed( seed );
    if(real == 0){
        Data dT("clientFolder/aBlockOf2KiB.txt");
        //Data d("clientFolder/aFileOf16000KiB.txt");
        //tag = calculateTag(dT.getValuePersByStringName(), N, g, p, q);//j
        //tags->push_back(tag);
        for(int i = 0; i < 2000 ;i++){
            fileName = new char[50];
            sprintf(fileName,"%s%d%s","serverFolder/aBlockOf2KiB",i,".txt");
            Data da(dT.getBytesFromFile(),fileName);
            sl->insert(Key(0), da, tags[0]);
        }
        
        
    }else{
        for(;it>file.begin() ;blockCounter++)
        {
            it -= blockLength+1;
            if(it < file.begin())
                it = file.begin();
            vector<char> aBlock;
            aBlock.clear();
            
            //cout<<"it:"<<*it<<endl;
            std::copy((it < file.begin()) ? file.begin() : it, it + blockLength, std::back_inserter(aBlock));
            
            char isim[80] = "";
            char str[100];
            sprintf(str, "%d", blockCounter);
            strcat(isim, fileName);
            strcat(isim, str);
            strcat(isim, ".txt");// = fileName + blockCounter + ".txt";
            
            Data d(aBlock,isim,blockCounter);
            sl->insert(Key(0), d, tags[tagsItr++]);
            //Node* rbn = sl->search(1);
        }
        
        
        //Delete counter file and recreate with new counter...
        remove( counterFileName);
        
        
        ofstream counterFile;
        counterFile.open(counterFileName);
        counterFile << blockCounter;
        counterFile.close();
    }
    return sl->getRoot()->getHash();
}
//PLANETLAB VERSION OF THE SAME FUNCTION
//PLEASE PAY ATTENTION: this full-rewrite is done from last to first
//This function just divides the file into blocks using the seed for both client and server being able to
//generate the same skip list
ZZ DPDPFlexProver::divideAndBuildSkipList_FullRewritePL(int seed, char* fileName, vector<ZZ> tags, int blockLength, RankBasedSkipList* rbsl) {
    
    int blockCounter=0;//XXX:will be read from file and be written to file at the end XXX
    /*blockCounter = Data::readCounterInt("serverFolder/counter.txt");
     char fileNametxt[80] = "";
     strcat(fileNametxt, fileName);
     strcat(fileNametxt, ".txt");*/
    
    char counterFileName[80] = "connectedClients/";
    strcat(counterFileName, clientIP.c_str());
    strcat(counterFileName, "/counter.txt");
    blockCounter = Data::readCounterInt(counterFileName);
    //blockCounter = Data::readCounterInt("serverFolder/counter.txt");
    char fileNametxt[80] = "";
    strcat(fileNametxt, fileName);
    strcat(fileNametxt, ".txt");
    
    Data d(fileNametxt);
    vector<char> file = d.getBytesFromFile();
    vector<char>::iterator it= file.end();
    
    int tagsItr = 0;
    int real = 0;
    cout<<"0. test"<<endl<<"else.real"<<endl;
    //cin>>real;
    srand ( seed );//SetSeed( seed );
    
    if(rbsl != NULL){
        
        sl = rbsl;
        
    }else if(real == 0){
        Data dT("clientFolder/aBlockOf2KiB.txt");
        dT.setLength(2048);
        //Data d("clientFolder/aFileOf16000KiB.txt");
        //tag = calculateTag(dT.getValuePersByStringName(), N, g, p, q);//j
        //tags->push_back(tag);
        
        fileName = new char[50];
        sprintf(fileName,"%s","serverFolder/aBlockOf2KiB.txt");
        numberOfBlocks = 500000;
        for(int i = 0; i < numberOfBlocks ;i++){
            
            //Data da(dT.getBytesFromFile(),fileName);
            sl->insert(Key(0), dT, tags[0]);
        }
        
    }else{
        for(;it>file.begin() ;blockCounter++)
        {
            it -= blockLength+1;
            if(it < file.begin())
                it = file.begin();
            vector<char> aBlock;
            aBlock.clear();
            
            //cout<<"it:"<<*it<<endl;
            std::copy((it < file.begin()) ? file.begin() : it, it + blockLength, std::back_inserter(aBlock));
            
            char isim[80] = "";
            char str[100];
            sprintf(str, "%d", blockCounter);
            strcat(isim, fileName);
            strcat(isim, str);
            strcat(isim, ".txt");// = fileName + blockCounter + ".txt";
            
            Data d(aBlock,isim,blockCounter);
            sl->insert(Key(0), d, tags[tagsItr++]);
            //Node* rbn = sl->search(1);
        }
        
        
        //Delete counter file and recreate with new counter...
        remove( counterFileName);
        
        
        ofstream counterFile;
        counterFile.open(counterFileName);
        counterFile << blockCounter;
        counterFile.close();
    }
    return sl->getRoot()->getHash();
}

/*This function just adds blocks up by multiplying each by random variables gotten out from the seed which came from the challenger*/
ZZ DPDPFlexProver::genBlockSum (ZZ seed, vector<ZZ> fileBlocks, int stat) {
    ZZ randValue;
    SetSeed(seed);
    ZZ blockSum = 0;
    
    for(int i = 0 ; i < fileBlocks.size(); i++)
    {
        RandomLen(randValue, stat);
        //cout<<	i << "randval: " <<randValue<<endl;
        blockSum += randValue * fileBlocks[i];
        
    }
    
    //cout<<"blocksum: "<<blockSum<<endl;
    return blockSum;
}

Node* DPDPFlexProver::SearchMulti(Node* currentNode, vector<int> chalIndex, int* start, int end, int rankState, ZZ N, ZZ g, vector<ProofNode>* proofVector, stack<State*>* interStack) {
    
    Key key(chalIndex[*start]-rankState);// Index of the challenged block is calculated according to the current imaginary root
    
    bool breakPoint = true;
    bool canGoDown;
    int rankDiff;
    int tempEnd;
    
    // Create and fetch proof nodes on the search path of challenged block to the proof vector
    while(breakPoint){
        // End of this branch of the proof path is when the current node reaches the challenged node
        //cout<<"chalIndex[*start] : "<<chalIndex[*start] << endl;
        //currentNode->print();
        ProofNode proofNode(currentNode->getLevel(), currentNode->getKey().getValue());
        //INITIAL PROOF NODE CREATED
        if ( currentNode->getLevel() == 0  && /*key.getValue() == 0*/key.getValue() < currentNode->getData().getLength() ) { /*key.getValue() == 0*///this check was direct conversion of rank based to flexlist where only the start index of nodes could be challenged
            
            breakPoint = false;
            proofNode.setEndFlag();
            proofNode.setLength(currentNode->getData().getLength());
            //When an intersection is found with another branch of the proof path, it is saved to be continued again, this crucial for outer loop of multi algorithms
            
            //XXX: WHEN THE SAME NODE IS CHALLENGED WITH DIFFERENT INDICES WE CAN ELIMINATE THE DUPLICATION HERE!!!
            //the challenges are in order.
            int tempStart = *start;
            if(tempStart != chalIndex.size() - 1)
            {   while(chalIndex[tempStart] + currentNode->getData().getLength() - key.getValue() >  chalIndex[*start+1]){
                    cout << "Different indices falling in same block..\n";
                    cout << "Previous value = " << *start << " New value = " << (*start + 1) << endl;
                    cout << chalIndex[tempStart] << " " << currentNode->getData().getLength() << " " << key.getValue() << " " << chalIndex[*start+1] << endl;                   
                    *start = *start +1;
                }
            }
        }
        canGoDown = currentNode->canGoDown(key, -1);
        
        rankDiff = currentNode->getBelowKeyValue();
        tempEnd = end;
        if (currentNode->isIntersecting(canGoDown, chalIndex, key.getValue(), &end, rankState)) {
            //INTERSECTING NODE
            interStack->push(new State(*currentNode, tempEnd, rankState));
            proofNode.setInterFlag();
            if(proofNode.isEnd())
                proofNode.setLength(currentNode->getData().length);
        }
        // Missing fields of the proof node are set according to the link current node follows
        if (canGoDown) {
            proofNode.set(currentNode->getAfterHash(), false);
            if(breakPoint)
                currentNode = currentNode->getBelow();
        }
        else if(breakPoint){
            proofNode.set(currentNode->getBelowHash(N,g), true);
            key.setValue(key.getValue() - currentNode->getBelowKeyValue());
            rankState = rankState + currentNode->getBelowKeyValue();
            currentNode = currentNode->getAfter();
        }// Set key and rank state values according to how many bytes at the leaf level are passed while following after link
        
        proofVector->push_back(proofNode);
        //END OF ONE SEARCH"
    }
    //SEARCH MULTI DONE
    return currentNode;
}

/*GenMultiProof:
	*Upon receipt of the challenges and the random seed
	from the client, the server runs genMultiProof algorithm in order to get tags, file blocks
	*and proof path for the challenged indices. Algorithm searches for the leaf node of each
	*challenged index and stores all nodes across the search path in the proof vector. How-
	*ever, we have observed that regular searching for each particular node is inefficient. If
	*we start from the root for each challenged block, there will be a lot of replicated proof
	*nodes.
	*/
vector<ProofNode> DPDPFlexProver::GenMultiProof(Node *root, vector<int> chalIndex, vector<ZZ>* tags, vector<ZZ>* fileBlocks/*, char* fileNameBase*/) {
    
    // Below line is modified by Yogiraj ...
    Node* currentNode = root;//sl->getRoot();
    
    stack<State*> interStack;
    vector<ProofNode> proofVector;
    State* state = new State(*currentNode, chalIndex.size()-1, 0);
    int rankState = 0;
        
    for(int i=0; i < chalIndex.size(); i++) {
        
        //cout << "Inside GenMultiProof(), iteration no : " << i << endl;
        // Call searchMulti method for each challenged block
        currentNode = SearchMulti(currentNode, chalIndex, &i, state->getEndIndex(), state->getRankState(), N, g, &proofVector, &interStack);
        // Store tag and data of challenged block, they will be used as a part of the proof
                
        tags->push_back(currentNode->getTag(N,g));
        
        ZZ valReturn = currentNode->getData().getValue(); //getValuePersByStringName(/*fileNameBase*//*, &val*/); // Modified by Yogiraj
                
        (*fileBlocks).push_back(valReturn);
        
        if(!interStack.empty()){
            state = interStack.top(); interStack.pop();
            currentNode = state->getNode().getAfter();
            if(currentNode == NULL) cout<<"PROBLEM with the current Node occured inside GenMultiProof!"<<endl;
            state->setRankState(state->getRankState() + state->getNode().getBelowKeyValue());
        }
    }
    
    delete state;
    return proofVector;
}

//PL Test replication of the function. instead of the name from the flexlist, it always returns a preset set of file blocks.
vector<ProofNode> DPDPFlexProver::GenMultiProofPL(vector<int> chalIndex, vector<ZZ>* tags, vector<ZZ>* fileBlocks, char* fileNameBase) {
    
    
    Node* currentNode = sl->getRoot();
    stack<State*> interStack;
    vector<ProofNode> proofVector;
    State* state = new State(*currentNode, chalIndex.size()-1, 0);
    int rankState = 0;
    for(int i=0; i < chalIndex.size(); i++) {
        // Call searchMulti method for each challenged block
        currentNode = SearchMulti(currentNode, chalIndex, &i, state->getEndIndex(), state->getRankState(), N, g, &proofVector, &interStack);
        // Store tag and data of challenged block, they will be used as a part of the proof
        
        tags->push_back(currentNode->getTag(N,g));
        
        char dataName[20];
        sprintf(dataName, "PLTESTDATA/%d.txt", (460<i)?460:i+1);
        //cout<<dataName<<endl;
        Data d(dataName);
        
        ZZ valReturn = d.getValue(); //modified by Yogiraj ();//currentNode->getData().getValuePersByStringName(/*fileNameBase*//*, &val*/);
        
        (*fileBlocks).push_back(valReturn);
        
        if(!interStack.empty()){
            state = interStack.top(); interStack.pop();
            currentNode = state->getNode().getAfter();
            if(currentNode == NULL) cout<<"PROBLEM with the current Node occured!"<<endl;
            state->setRankState(state->getRankState() + state->getNode().getBelowKeyValue());
        }
    }
    
    delete state;
    return proofVector;
}

/* 4 helper functions for basic testing*/
void DPDPFlexProver::update(int index, bool type, int level, ZZ tag, ZZ block){ //if type is true then insert, otherwise delete
    if (type)
        sl->insert(Key(index), Data(block), tag, NULL, level);
    else sl->remove(Key(index), NULL);
}

void DPDPFlexProver::updateDelete(int index){ //delete the element
    sl->remove(Key(index), NULL);
}

void DPDPFlexProver::updateInsert(int index, int level, ZZ tag, Data block){ //insert the element
    sl->insert(Key(index), block, tag, NULL,level);
}

void DPDPFlexProver::updateModify(int index, int level, ZZ tag, Data block){ //modify the element
    sl->modify(Key(index), block, tag, NULL);
}

//PLEASE PAY ATTENTION: this full-rewrite is done from last to first
//An old function left here for later referances
ZZ DPDPFlexProver::fullRewrite(ZZ seed, char* blockName, vector<ZZ> tags, int numOfBlocks) {
    
    srand ( to_int(seed) );//SetSeed( seed );
    //cout<<"fileName: "<<blockName<<endl;
    //sl->print();
    //cout<<"seed which is used in fullrewrite of prover: "<< seed <<endl;
    for(int i=1; i <= numOfBlocks/*fileBlocks->size()*/; i++) {
        //cout<<"i: "<<i<<endl;
        //sl->insert(Key(0), Data((*fileBlocks)[0]), tags[0]);//i i
        sl->insert(Key(0), Data(blockName/*(*fileBlocks)[0], defaultBlockLength*/), tags[0]);
    }
    //sl->print();
    return sl->getRoot()->getHash();
}

//PLEASE PAY ATTENTION: this full-rewrite is done from last to first
//for now, prepared as a test function for reference: used in DPDPTestWithFlexList in Test.cpp
ZZ DPDPFlexProver::fullRewrite(ZZ seed, vector<ZZ> *fileBlocks, vector<ZZ> tags, int numOfBlocks, int defaultBlockLength) {
    
    srand ( to_int(seed) );//SetSeed( seed );
    for(int i=1; i <= numOfBlocks/*fileBlocks->size()*/; i++) {
        //sl->insert(Key(0), Data((*fileBlocks)[0]), tags[0]);//i i
        sl->insert(Key(0), Data("aBlockOf16KiB.txt"/*(*fileBlocks)[0], defaultBlockLength*/), tags[0]);
    }
    //sl->print();
    return sl->getRoot()->getHash();
}

ZZ DPDPFlexProver::getN(){return N;}
void DPDPFlexProver::setN(ZZ n){ N = n;}
ZZ DPDPFlexProver::getG(){return g;}
void DPDPFlexProver::setG(ZZ G){ g = G;}
RankBasedSkipList* DPDPFlexProver::getSL(){return sl;}
void DPDPFlexProver::setSL(RankBasedSkipList* rbsl) { sl = rbsl; }
RankBasedSkipList* DPDPFlexProver::getRBSL() { return sl; }
void DPDPFlexProver::setFileName(string fn){ fileName = fn; cout<<"FileName set to: "<<fileName<<endl;}
string DPDPFlexProver::getFileName(){ //cout<<"FileName get: "<<fileName<<endl;
    return fileName;
}

void DPDPFlexProver::setFileNameBase(string fn){ fileNameBase = fn;
    //cout<<"FileName set to: "<<fileNameBase<<endl;
}
string DPDPFlexProver::getFileNameBase(){ //cout<<"FileName get: "<<fileNameBase<<endl;
    return fileNameBase;
}

void DPDPFlexProver::setClientIP(string IP){clientIP=IP;}
string DPDPFlexProver::getClientIP(){return clientIP;}

void DPDPFlexProver::setNumBlocks(int num) { numberOfBlocks = num;}
int DPDPFlexProver::getNumBlocks() { return numberOfBlocks;}