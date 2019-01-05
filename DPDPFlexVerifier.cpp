/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "DPDPFlexVerifier.h"
#include "GroupRSA.h"
#include "Timer.h"
#include "RankBasedSkipList.h"
#include "MultiExp.h"
#include "DummyRankBasedNode.h"

DPDPFlexVerifier::DPDPFlexVerifier(vector<ZZ> *Data, ZZ *N, ZZ *g)
{
    data = Data;
    //srand ( time(NULL) );
    srand(DPDPCONST::Seed);
    generateGroup(N, g);
}

DPDPFlexVerifier::DPDPFlexVerifier()
{
    srand(DPDPCONST::Seed);
    generateGroup(&N, &g);
}

DPDPFlexVerifier::~DPDPFlexVerifier()
{
    data->clear();
    //delete data;
}

void DPDPFlexVerifier::generateGroup(ZZ *N, ZZ *g) {
    ZZ returnN, returng;
    //SetSeed( to_ZZ(rand() % 10000) );
    SetSeed( DPDPCONST::Seed );
    
    
    GroupRSA* group = new GroupRSA("Client1", 1024, 80);
    returnN = group->getModulus();
    returng = RandomBnd(returnN);
    while (GCD(returng, returnN) != 1) {
        returng = RandomBnd(returnN);
    }
    setN(returnN);
    setG(returng);
    setP(group->getP());
    setQ(group->getQ());
    *N = returnN; *g= returng;
}

long long DPDPFlexVerifier::divideAndBuildSkipList_sendFileToServer(ZZ seed, vector<ZZ> *tags, char* fileName, int blockLength)
{
    SetSeed( seed );
    rbsl = new RankBasedSkipList(true,N, g); //flexlist: true
    //rbslTemp = new RankBasedSkipList(true,N, g); //flexlist: true
    tempDatas = new vector<Data>();
    
    
    //srand ( to_int(seed) );
    ZZ tag;
    ZZ MD;
    long long timer = 0;
    
    int blockCounter=0;//XXX:will be read from file and be written to file at the end XXX
    blockCounter = Data::readCounterInt("clientFolder/counter.txt");
    char fileNametxt[80] = "";
    strcat(fileNametxt, fileName);
    strcat(fileNametxt, ".txt");
    
    
    Data d(fileNametxt);
    
    
    int real = 1;
    cout<<"0. test"<<endl<<"else.real"<<endl;
    //cin>>real;
    if(real == 0){
        Data dT("clientFolder/aBlockOf2KiB.txt");
        tag = calculateTag(dT.getValue(), N, g, p, q);//j
        tags->push_back(tag);
        srand ( to_int(seed) );
        for(int i = 0; i < 2000 ; i++){
            fileName = new char[50];
            sprintf(fileName,"%s%d%s","clientFolder/aBlockOf2KiB",i,".txt");
            Data da(dT.getBytesFromFile(),fileName);
            rbsl->insert(Key(0), da, tag);
        }
        MD = rbsl->getRoot()->getHash();
        MetaData = MD;
        
    }else{
        
        vector<char> file = d.getBytesFromFile();
        vector<char>::iterator it= file.end();
        //for(int i = 0; i < 5 ; i++,blockCounter++)
        srand ( to_int(seed) );
        
        ZZ blockSum;
        blockSum = 0;
        
        for(;it>file.begin() ;blockCounter++)
        {
            it -= blockLength+1;
            if(it < file.begin())
                it = file.begin();
            vector<char> aBlock;
            aBlock.clear();
            
            //			cout<<"it:"<<*it<<endl;
            std::copy((it < file.begin()) ? file.begin() : it, it + blockLength, std::back_inserter(aBlock));
            
            char str[100];
            sprintf(str, "%d", blockCounter);
            char isim[80] = "";
            
            char fileNameTemptxt[80] = "";
            strcat(fileNameTemptxt, fileName);
            strcat(fileNameTemptxt, str);
            strcat(fileNameTemptxt, "temp");
            strcat(fileNameTemptxt, ".txt");
            
            strcat(isim, fileName);
            strcat(isim, str);
            strcat(isim, ".txt");// = fileName + blockCounter + ".txt";
            
            Data d(aBlock,isim,blockCounter);
            Data dtemp(aBlock,fileNameTemptxt,blockCounter); //for temp files that are needed for alterations ONLY ON VERIFIER SIDE
            startTimer();
            //ZZ val;
            //val = 1;
            tag = calculateTag(d.getValuePers(fileName/*"clientFolder/aFileOf16000KiB"*//*, &val*/), N, g, p, q);//j
            timer += elapsedTime();
            tags->push_back(tag);
            //cout<<"tag first calc: "<<tag<<endl;
            rbsl->insert(Key(0), d, tag);
            // for alterations management
            tempDatas->push_back(dtemp); // XXX: to be used in reverse order!!!
            
            //blockSum+=d.getValuePers("clientFolder/aFileOf16000KiB"/*, &val*/);
            
        }
        
        
        //verifyTags(*tags, blockSum, &timer);
        
        //Delete counter file and recreate with new counter...
        remove( "clientFolder/counter.txt" );
        //!!!		remove( fileNametxt ) ;
        
        ofstream counterFile;
        counterFile.open("clientFolder/counter.txt");
        counterFile << blockCounter;
        counterFile.close();
        
        MD = rbsl->getRoot()->getHash();
        MetaData = MD;
        
    }
    
    //return timer/1000.0;
    //}
    
    //verifyTags(*tags, blockSum, &timer);
    
    //Delete counter file and recreate with new counter...
    remove( "clientFolder/counter.txt" );
    //!!!		remove( fileNametxt ) ;
    
    
    //XXX: THIS PART IS VERY IMPORTANT, MAKE THIS AVAILABLE, COMMENTED OUT FOR PL TESTS
    ofstream counterFile;
    counterFile.open("clientFolder/counter.txt");
    counterFile << blockCounter;
    counterFile.close();
    
    MD = rbsl->getRoot()->getHash();
    MetaData = MD;
    return timer/1000.0;
}


long long DPDPFlexVerifier::divideAndBuildSkipList_sendFileToServerPL(ZZ seed, vector<ZZ> *tags, char* fileName, int blockLength)
{
    SetSeed( seed );
    rbsl = new RankBasedSkipList(true,N, g); //flexlist: true
    //rbslTemp = new RankBasedSkipList(true,N, g); //flexlist: true
    tempDatas = new vector<Data>();
    
    //srand ( to_int(seed) );
    ZZ tag;
    ZZ MD;
    long long timer = 0;
    
    int blockCounter=0;//XXX:will be read from file and be written to file at the end XXX
    blockCounter = Data::readCounterInt("clientFolder/counter.txt");
    char fileNametxt[80] = "";
    strcat(fileNametxt, fileName);
    strcat(fileNametxt, ".txt");
    
    Data d(fileNametxt);
    
    int real = 0;
    cout<<"0. test"<<endl<<"else.real"<<endl;
    //cin>>real;
    if(real == 0){
        /*Data dT("clientFolder/aBlockOf2KiB.txt");
         dT.setLength(2048);
         tag = calculateTag(dT.getValuePersByStringName(), N, g, p, q);//j
         tags->push_back(tag);
         srand ( to_int(seed) );
         fileName = new char[50];
         sprintf(fileName,"%s","clientFolder/aBlockOf2KiB.txt");
         
         size = 500000;
         for(int i = 0; i < size ; i++){
         
         //Data da(dT.getBytesFromFile(),fileName);
         rbsl->insert(Key(0), dT, tag);
         }
         MD = rbsl->getRoot()->getHash();
         MetaData = MD;*/
        
        
    }else{
        
        vector<char> file = d.getBytesFromFile();
        vector<char>::iterator it= file.end();
        //for(int i = 0; i < 5 ; i++,blockCounter++)
        srand ( to_int(seed) );
        
        ZZ blockSum;
        blockSum = 0;
        
        for(;it>file.begin() ;blockCounter++)
        {
            it -= blockLength+1;
            if(it < file.begin())
                it = file.begin();
            vector<char> aBlock;
            aBlock.clear();
            
            //			cout<<"it:"<<*it<<endl;
            std::copy((it < file.begin()) ? file.begin() : it, it + blockLength, std::back_inserter(aBlock));
            //for(int i = 0; i < aBlock.size() ; i++)
            //	{
            //		cout<<aBlock[i];
            //	}
            //	cout<<endl;
            
            
            char str[100];
            sprintf(str, "%d", blockCounter);
            char isim[80] = "";
            
            char fileNameTemptxt[80] = "";
            strcat(fileNameTemptxt, fileName);
            strcat(fileNameTemptxt, str);
            strcat(fileNameTemptxt, "temp");
            strcat(fileNameTemptxt, ".txt");
            
            
            strcat(isim, fileName);
            strcat(isim, str);
            strcat(isim, ".txt");// = fileName + blockCounter + ".txt";
            //cout<<"blockCounter: "<<blockCounter<<endl;
            Data d(aBlock,isim,blockCounter);
            Data dtemp(aBlock,fileNameTemptxt,blockCounter); //for temp files that are needed for alterations ONLY ON VERIFIER SIDE
            startTimer();
            //ZZ val;
            //val = 1;
            tag = calculateTag(d.getValuePers(fileName/*"clientFolder/aFileOf16000KiB"*//*, &val*/), N, g, p, q);//j
            timer += elapsedTime();
            tags->push_back(tag);
            //cout<<"tag first calc: "<<tag<<endl;
            rbsl->insert(Key(0), d, tag);
            // for alterations management
            tempDatas->push_back(dtemp); // XXX: to be used in reverse order!!!
            
            //blockSum+=d.getValuePers("clientFolder/aFileOf16000KiB"/*, &val*/);
            
        }
        
        
        //verifyTags(*tags, blockSum, &timer);
        
        //Delete counter file and recreate with new counter...
        remove( "clientFolder/counter.txt" );
        //!!!		remove( fileNametxt ) ;
        
        ofstream counterFile;
        counterFile.open("clientFolder/counter.txt");
        counterFile << blockCounter;
        counterFile.close();
        
        MD = rbsl->getRoot()->getHash();
        MetaData = MD;
        
    }
    
    //return timer/1000.0;
    //}
    
    //verifyTags(*tags, blockSum, &timer);
    
    //Delete counter file and recreate with new counter...
    remove( "clientFolder/counter.txt" );
    //!!!		remove( fileNametxt ) ;
    
    
    //XXX: THIS PART IS VERY IMPORTANT, MAKE THIS AVAILABLE, COMMENTED OUT FOR PL TESTS
    //ofstream counterFile;
    //counterFile.open("clientFolder/counter.txt");
    //counterFile << blockCounter;
    //counterFile.close();
    
    MD = rbsl->getRoot()->getHash();
    MetaData = MD;
    return timer/1000.0;
}


//for now, prepared as a test function for reference: used in DPDPTestWithFlexList in Test.cpp
//used with fullRewrite from DPDPFlexProver
long long DPDPFlexVerifier::sendFileToServer(ZZ seed, vector<ZZ> *tags, int numOfBlocks, char* blockName)
{
    SetSeed( seed );
    RankBasedSkipList *sl = new RankBasedSkipList(true,N, g); //flexlist: true
    
    srand ( to_int(seed) );
    ZZ tag;
    ZZ MD;
    long long timer = 0;
    //cout<<"number of blocks: "<<numOfBlocks<<endl;
    
    ////////////////This section is to calculate Tag before-hand for test issues...
    //char* blockName = "aBlockOf16KiB.txt";
    ZZ value;
    ifstream theBlock;
    long size;
    theBlock.open (blockName);
    theBlock.seekg (0, ios::end);
    size = theBlock.tellg();
    char *memblock = new char[size];
    theBlock.seekg (0, ios::beg);
    theBlock.read (memblock, size);
    theBlock.close();
    ZZFromBytes(value, (unsigned char*)memblock, size);
    delete[] memblock;
    ////////////////
    
    
    startTimer();
    tag = calculateTag(/*(*data)[0]*/value, N, g, p, q);//j
    timer += elapsedTime();
    tags->push_back(tag);
    ///////////////////////
    for(int j=1; j<= numOfBlocks; j++) {
        
        //PLEASE PAY ATTENTION: this full-rewrite is done from last to first
        
        //sl->insert(Key(0), Data((*data)[0]), tag);//j
        sl->insert(Key(0), Data(blockName), tag); //flexlist: sl->insert(Key(j), Data((*data)[0]), tag);//j to current line
    }
    MD = sl->getRoot()->getHash();
    //sl->print();
    delete sl; // for test purposes
    MetaData = MD;
    return timer/1000.0;
}


bool DPDPFlexVerifier::verifyRewrite(ZZ rootRewrite) {
    if(MetaData == rootRewrite) {
        //data->clear(); if not dropbox type, the file will be deleted!
        cout<<"Verifier : FULL RE-WRITE VERIFIED"<<endl;
        return true;
    }
    
    cout<<"Verifier : FULL RE-WRITE NOT VERIFIED!!!"<<endl;
    return false;
}


ZZ DPDPFlexVerifier::calculateTag(ZZ data, ZZ modulus, ZZ element, ZZ p, ZZ q, ZZ order) {
    ZZ exp = to_ZZ(1);
    mpz_mod(MPZ(exp), MPZ(data), ((order == to_ZZ(1)) ? MPZ((to_ZZ((p-1) * (q-1)))):MPZ(order)) );
    //exp = (data % (to_ZZ((p-1) * (q-1))));
    return PowerMod(element, exp, modulus);
}


/*THIS NEEDS WORK SINCE CHALLENGING REQUIRES THE START POINTS OF BLOCKS FOR NOT TO CHALLENGE SAME BLOCK TWICE
 OR THERE SHOULD BE A DISTANCE OF MAXIMUM BLOCK SIZE BETWEEN TWO CHALLENGED BLOCKS(WHICH REQUIRES A NEW GENERATECHALLENGES MECHANISM)*/
vector<int> DPDPFlexVerifier::generateChallenges(int nChallenges, int numBlocks, int* seed) {
    *seed = rand() % 10000;
    this->seed = to_ZZ(*seed);
    vector<int> toReturn;
     
    // Commented by Yogiraj
    /*while(toReturn.size() < nChallenges) {
        int chalx = NTL::RandomBnd(numBlocks);//rand() % numBlocks;
        vector<int>::iterator it = std::find(toReturn.begin(), toReturn.end(), chalx);
        if (it != toReturn.end() || chalx == 0)
        {   cout << "continued\n";
            continue;
        }
        cout << chalx << " " << ((chalx * DPDPCONST::blockSize) - 1) << endl;
        
        toReturn.push_back((chalx * DPDPCONST::blockSize) - 1);          
    }*/
    
    // This new challenge creation algorithm creates unique challenge indices each time such that all indices will belong to different blocks -- Yogiraj
    for(int i = 0; i < nChallenges; i++)
    {
        int r;
        do
        {
              r = rand() % numBlocks;
        } while(std::find(toReturn.begin(), toReturn.end(), r) != toReturn.end());            // while r is found in result vector
        
        toReturn.push_back(r);   
    }
    
    sort(toReturn.begin(), toReturn.begin() + nChallenges);
    
    for(int i = 0; i < nChallenges; i++)
        toReturn[i] = DPDPCONST::blockSize * toReturn[i] - 1;
    
    return toReturn;
}


bool DPDPFlexVerifier::verifyTags(vector<ZZ> tags, ZZ blockSum, long long* timer) {
    ZZ randValue;
    ZZ tagMul;
    ZZ result;
    tagMul = 1;
    SetSeed(seed);
 //   startTimer();
    
    vector<ZZ> rndVals;
    for(int i = 0 ; i < tags.size(); i++)
    {
        RandomLen(randValue, DPDPCONST::stat);
        //			cout<<	i << "randval: verif: " <<randValue<<endl;
        rndVals.push_back(randValue);
    }
    
    tagMul = MultiExp(tags, rndVals, getN(), false);
    //for(int i = 0 ; i < tags.size(); i++)
    //{
    //RandomLen(randValue, DPDPCONST::stat);
    //tagMul *= PowerMod(tags[i], randValue, getN());
    //}
    
    
    mpz_mod(MPZ(tagMul), MPZ(tagMul), MPZ(getN()));
    //tagMul = to_ZZ(tagMul % getN());
 //   *timer = elapsedTime()/1000.0;
    mpz_mod(MPZ(blockSum), MPZ(blockSum), MPZ((to_ZZ((p-1) * (q-1)))));
    //blockSum = (blockSum % (to_ZZ((p-1) * (q-1))));
    result = PowerMod(getG(), blockSum, getN());
    
    cout << "\nTag verification ..\n tagMul = " << tagMul << " \n result = " << result << endl;
    
    if(tagMul == result) return true;
    return false;
}


void DPDPFlexVerifier::updateRankSum(int rankDiff, vector<int>* vecRankSum, int start, int end) {
    for(int i=start; i<=end; i++)
        (*vecRankSum)[i] += rankDiff;
}


bool DPDPFlexVerifier::VerifyMultiProof(vector<int> chalIndex, vector<ProofNode> proofVector, vector<ZZ> tags, ZZ blockSum) {
    
    /*cout << "Challenged indices inside verifyMultiProof() : \n";
    for(int i = 0; i < chalIndex.size(); i++)
        cout << chalIndex[i] << " ";
    cout << endl; */
    
    int start = tags.size() - 1; int end =start;
    int tagIndex = tags.size()-1;
    vector<int> vecRankSum(chalIndex.size(), 0);
    ZZ hash, hashOfPrev;
    int rankOfPrev = 0;
    //XXX v2v: added for when we do not know the boundaries of files
    vector<int> lengthsOfChallengedNodes(chalIndex.size(), 0);
    int lastChallengedIndex = chalIndex.size()-1;
    //		cout<<"Tag size "<<tags.size()<<endl;
    stack<ZZ> interHashStack;
    stack< pair<int,int> > index;
    
    cout << "\nProof vector size = " << proofVector.size() << endl;
    
    for(int i = proofVector.size()-1; i > -1 ; i--) {
        
        //cout << "i = " << i << endl;
        
        if(proofVector[i].isEnd() && proofVector[i].isInter()) {
            hash = ProofNode::calculateHash(proofVector[i].getLevel(), proofVector[i].getRank(), tags[tagIndex--], hashOfPrev);
            updateRankSum(proofVector[i].getLength(), &vecRankSum, start, end); //flexlist: 1 to proofVector[i].getLength()
            start--;
            lengthsOfChallengedNodes[lastChallengedIndex--] = proofVector[i].getLength();
            //				cout<<"end and inter "<<endl;
        }
        
        else if(proofVector[i].isEnd()) {
            //XXX v1v: added for when we do not know the boundaries of files
            lengthsOfChallengedNodes[lastChallengedIndex--] = proofVector[i].getLength();
            
            if(tagIndex != tags.size()-1) {
                interHashStack.push(hashOfPrev);
                index.push(pair<int,int>(start, end));
                start--;
                end = start;
                //					cout<<"end if"<<endl;
            }
            hash = ProofNode::calculateHash(proofVector[i].getLevel(), proofVector[i].getRank(), tags[tagIndex--], proofVector[i].getHash());
            //				cout<<"end"<<endl;
        }
        
        else if(proofVector[i].isInter()) {
            end = index.top().second;
            updateRankSum(rankOfPrev, &vecRankSum, index.top().first, index.top().second); index.pop();
            hash = ProofNode::calculateHash(proofVector[i].getLevel(), proofVector[i].getRank(), hashOfPrev,  interHashStack.top());
            interHashStack.pop();
            //				cout<<"inter "<<endl;
        }
        
        else if(proofVector[i].getRgtOrDwn() || proofVector[i].getLevel() == 0) {
            hash = ProofNode::calculateHash(proofVector[i].getLevel(), proofVector[i].getRank(), proofVector[i].getHash(), hashOfPrev);
            updateRankSum(proofVector[i].getRank() - rankOfPrev, &vecRankSum, start, end);
            //				cout<<"right"<<endl;
        }
        
        else {
            /*cout << "PROBLEM HERE --------------\n";
            cout << "proofVector[i].getLevel() " << proofVector[i].getLevel() << "\n" << "proofVector[i].getRank() " << proofVector[i].getRank() << 
                    "\n hashOfPrev " << hashOfPrev << "\nproofVector[i].getHash() " << proofVector[i].getHash() << endl; */
            
            hash = ProofNode::calculateHash(proofVector[i].getLevel(), proofVector[i].getRank(), hashOfPrev, proofVector[i].getHash());
            
            /*cout << "hash = " << hash << endl;
            
            cout<<"down"<<endl;
            cout << "-----------------------------------\n";                            */
               
        }
        
        hashOfPrev = hash;
        //cout << "multihash:"<<hash<<endl;
        rankOfPrev = proofVector[i].getRank();
        
        //Commented out segment below is to see a bigger picture in case of investigation.
        
        /*cout<<"level "<<proofVector[i].getLevel()<<endl;
         cout<<"isInter "<<proofVector[i].isInter()<<endl;
         cout<<"isEnd "<<proofVector[i].isEnd()<<endl;
         cout<<"length "<<proofVector[i].getLength()<<endl;
         cout<<"rgtOrDwn "<<proofVector[i].getRgtOrDwn()<<endl;
         cout<<"hash "<< proofVector[i].getHash()<< endl;
         cout << "hash2 " << hash << endl;
         cout<<"hash of prev "<<hashOfPrev<<endl;
         cout<<"rank of prev "<<rankOfPrev<<endl;
         cout<<"================================="<<endl;*/
    }
    
    cout << "\nVerifyMultiProof..\n";
    
    if(hash == MetaData) cout<<"hash == metadata"<<endl;
    else	cout<<"hash != metadata"<<endl;
    cout<<"List RootMetadata = " << MetaData<<endl;
    cout << "Calculated Hash = " << hash << endl;
    cout << endl;
    
    //if(hash == MetaData)
    //	cout<<"hash == MD"<<endl;
    //else cout<<"hash != MD"<<endl;
    
    //if(verifyRankSum(lengthsOfChallengedNodes, chalIndex, vecRankSum))
    //	cout<<"verifyRankSum(lengthsOfChallengedNodes, chalIndex, vecRankSum)"<<endl;
    //else cout<<"!!!!!!!verifyRankSum(lengthsOfChallengedNodes, chalIndex, vecRankSum)"<<endl;
                
    bool res = verifyRankSum(lengthsOfChallengedNodes, chalIndex, vecRankSum);
    cout << "verifyRankSum result = " << res << endl;
                
    return ((hash == MetaData) && res);
}


bool DPDPFlexVerifier::VerifyMultiProofWOchalIndex(vector<ProofNode> proofVector, vector<ZZ> tags, ZZ blockSum) {
    
    int tagIndex = tags.size()-1;
    ZZ hash, hashOfPrev;
    int rankOfPrev = 0;
    stack<ZZ> interHashStack;
    for(int i=proofVector.size()-1; i > -1 ; i--) {
        
        
        if(proofVector[i].isEnd() && proofVector[i].isInter()) {
            hash = ProofNode::calculateHash(proofVector[i].getLevel(), proofVector[i].getRank(), tags[tagIndex--], hashOfPrev);
        }
        else if(proofVector[i].isEnd()) {
            
            
            if(tagIndex != tags.size()-1) {
                interHashStack.push(hashOfPrev);
            }
            hash = ProofNode::calculateHash(proofVector[i].getLevel(), proofVector[i].getRank(), tags[tagIndex--], proofVector[i].getHash());
            
        }
        else if(proofVector[i].isInter()) {
            hash = ProofNode::calculateHash(proofVector[i].getLevel(), proofVector[i].getRank(), hashOfPrev,  interHashStack.top());
            interHashStack.pop();
        }
        else if(proofVector[i].getRgtOrDwn() || proofVector[i].getLevel() == 0) {
            hash = ProofNode::calculateHash(proofVector[i].getLevel(), proofVector[i].getRank(), proofVector[i].getHash(), hashOfPrev);
        }
        else {
            hash = ProofNode::calculateHash(proofVector[i].getLevel(), proofVector[i].getRank(), hashOfPrev, proofVector[i].getHash());
        }
        
        hashOfPrev = hash;
        
        rankOfPrev = proofVector[i].getRank();
        
    }
    
    return ((hash == MetaData));
}

bool DPDPFlexVerifier::verifyRankSum(vector<int> lengthsOfChallengedNodes, vector<int> chalIndex, vector<int> vecRankSum){
    
    /*XXX: this check is enough for rank-based skip list!*/
    /*return equal(chalIndex.begin(),chalIndex.end() , vecRankSum.begin());*/
    for(int i = 0 ; i < chalIndex.size() ; i++){
        
        if(!(chalIndex[i] >= vecRankSum[i]  && chalIndex[i]<vecRankSum[i]+lengthsOfChallengedNodes[i]) ){
            if(chalIndex[i] >= 0)//HA-HA
            {
                cout<<"verify rank sum return false"<<endl;
                cout << "index i in chalVector = " << i << endl;
                return false;
            }
        }
    }
    
    return true;
    
}

ZZ DPDPFlexVerifier::genBlockSum (ZZ seed, vector<ZZ> fileBlocks, int stat) {
    ZZ randValue;
    SetSeed(seed);
    ZZ blockSum = 0;
    
    for(int i = 0 ; i < fileBlocks.size(); i++)
    {
        RandomLen(randValue, stat);
        blockSum += randValue * fileBlocks[i];
    }
    return blockSum;
}

bool DPDPFlexVerifier::verifyInsert(vector<int> chalIndex, vector<ProofNode> proofVector, vector<ZZ> tags, ZZ blockSum, ZZ tag, int level, int newDataSize, ZZ newMetaData) {
    vector<int> initialInsertChalenge;
    initialInsertChalenge.push_back(chalIndex[0] -1);
    if(!VerifyMultiProof(initialInsertChalenge/*chalIndex*/, proofVector, tags, blockSum)){
        cout << "insert not verified, first chal"<<endl;
        return false;
    }
    else
    {
        //cout<<"newDataSize in verifyInsert = "<< newDataSize<<endl;
        int i = proofVector.size() - 1,rank = proofVector[i].getLength(), rankTower = proofVector[i].getRank() - proofVector[i].getLength() + newDataSize;
        
        ZZ hash = tags[0], hashTower = ProofNode::calculateHash(0, rankTower, tag, proofVector[i].getHash());
        if (level != 0){
            hash = ProofNode::calculateHash(0,proofVector[i].getLength(), hash, to_ZZ(0)); // is last argument right?
            
        }
        i--;
        
        while (proofVector[i].getLevel() < level || (proofVector[i].getLevel() == level && !proofVector[i].getRgtOrDwn()) ) {
            //cout<<"while loop un icinde"<<endl;
            if (proofVector[i].getRgtOrDwn()) {
                rank += proofVector[i].getRank() - proofVector[i+1].getRank();
                hash = ProofNode::calculateHash(proofVector[i].getLevel(), rank, proofVector[i].getHash(), hash);
            }
            else {
                rankTower += proofVector[i].getRank() - proofVector[i+1].getRank();
                hashTower = ProofNode::calculateHash(proofVector[i].getLevel(), rankTower, hashTower, proofVector[i].getHash());
            }
            i--;
        }
        /*			if(proofVector[i].getLevel() == 0) {
         i--;
         }*/
        hash = ProofNode::calculateHash(level, rank + rankTower, hash, hashTower);
        MetaData = calculateRemainingHashes(i, hash, newDataSize, proofVector);
        
        return (MetaData == newMetaData);
    }
}

bool DPDPFlexVerifier::verifyModify(vector<int> chalIndex, vector<ProofNode> proofVectorBefore, vector<ProofNode> proofVectorAfter, vector<ZZ> tagsBefore,vector<ZZ> tags, ZZ blockSum, ZZ blockSumAfter, ZZ tag, int level, int blockSize, int newDataSize, ZZ newMetaData) {
    if(!VerifyMultiProof(chalIndex, proofVectorBefore, tagsBefore, blockSum)){
        cout<<"first not verify"<<endl;
        return false;
    }
    else {
        int i= proofVectorBefore.size() - 1;
        //16256 is the modified file size,it should change
        
        ZZ hash = ProofNode::calculateHash(proofVectorBefore[i].getLevel(), proofVectorBefore[i].getRank()-blockSize+newDataSize/*16384*//*16406*/, tag, proofVectorBefore[i].getHash());
        //cout<<"-blockSize+newDataSize= "<<newDataSize-blockSize<<endl;
        
        ZZ oldMetaData=MetaData;
        MetaData = calculateRemainingHashes(i-1, hash,/*16406*//*16384*/ newDataSize-blockSize, proofVectorBefore);
        
        return (newMetaData == MetaData);
    }
}

bool DPDPFlexVerifier::verifyModify(vector<int> chalIndex, vector<ProofNode> proofVectorBefore, vector<ZZ> tagsBefore, ZZ blockSum, ZZ tag, int blockSize, int newDataSize, ZZ newMetaData) {
    if(!VerifyMultiProof(chalIndex, proofVectorBefore, tagsBefore, blockSum)){
        cout<<"first not verify"<<endl;
        return false;
    }
    else {
        int i= proofVectorBefore.size() - 1;
        //16256 is the modified file size,it should change
        
        ZZ hash = ProofNode::calculateHash(proofVectorBefore[i].getLevel(), proofVectorBefore[i].getRank()-blockSize+newDataSize/*16384*//*16406*/, tag, proofVectorBefore[i].getHash());
        
        ZZ oldMetaData=MetaData;
        MetaData = calculateRemainingHashes(i-1, hash,/*16406*//*16384*/ newDataSize-blockSize, proofVectorBefore);
        
        
        return (newMetaData == MetaData);
        
    }
}

bool DPDPFlexVerifier::verifyDelete(vector<int> chalIndex, vector<ProofNode> proofVector, vector<ZZ> tags, ZZ blockSum, ZZ newMetaData) {
    if(!VerifyMultiProof(chalIndex, proofVector, tags, blockSum)){
        cout << "-----------------not verified by first multiproof"<<endl;
        return false;
    }
    else {
        //cout << "VerifyDelete"<<endl;
        int dn = proofVector.size() - 1, i = proofVector.size() - 2;
        while (!proofVector[i].isEnd()) {
            i--;
            cout << "i:"<<i<<endl;
        }
        int rank = proofVector[i].getLength() + proofVector[dn].getRank() - proofVector[dn].getLength();
        ZZ hash = ProofNode::calculateHash(0, rank, tags[0], proofVector[dn].getHash());
        
        cout << "INTER dn:"<<proofVector[dn].isInter()<<endl;
        dn--;
        
        if (!proofVector[dn].isEnd() || !proofVector[i].isInter()){
            cout << "i:"<<i<< " hash: "<<hash<<endl;
            i--;
        }
        cout << "enterWhile"<<endl;
        while (!proofVector[dn].isEnd() || !proofVector[i].isInter()){
            cout << "END i:"<<proofVector[i].isEnd()<<endl;
            
            if (proofVector[i].getLevel() < proofVector[dn].getLevel() || proofVector[dn].isEnd()) {
                rank += proofVector[i].getRank() - proofVector[i+1].getRank();
                hash = ProofNode::calculateHash(proofVector[i].getLevel(), rank, proofVector[i].getHash(), hash);
                cout << "i:"<<i<< " hash: "<<hash<<endl;
                i--;
            }
            else {
                rank += proofVector[dn].getRank() - proofVector[dn+1].getRank();
                hash = ProofNode::calculateHash(proofVector[dn].getLevel(), rank, hash, proofVector[dn].getHash());
                cout << "dn:"<<dn<< " hash: "<<hash<<endl;
                dn--;
                
            }
            //if(i<0) break; // this i becomes - and returns to the begining of the loop :(
            cout << "i:"<< i << " dn: " <<dn<<endl;
        }
        cout << "leaveWhile"<<endl;
        i--;
        cout << "i:"<<i<<endl;
        MetaData = calculateRemainingHashes(i, hash, -(proofVector[proofVector.size() - 1].getLength()), proofVector);
        //MetaData = calculateRemainingHashes(i, hash, -1, proofVector);
        
        return (newMetaData == MetaData);
    }
}

ZZ DPDPFlexVerifier::calculateNewRoot(vector<int> chalIndex, vector<ProofNode> proofVector, vector<ZZ> tags, ZZ blockSum) {
    int start= tags.size() - 1; int end =start;
    int tagIndex = tags.size()-1;
    vector<int> vecRankSum(chalIndex.size(), 0);
    ZZ hash, hashOfPrev;
    int rankOfPrev = 0;
    stack<ZZ> interHashStack;
    stack< pair<int,int> > index;
    for(int i=proofVector.size()-1; i > -1 ; i--) {
        if(proofVector[i].isEnd() && proofVector[i].isInter()) {
            hash = ProofNode::calculateHash(proofVector[i].getLevel(), proofVector[i].getRank(), tags[tagIndex--], hashOfPrev);
            updateRankSum(proofVector[i].getLength(), &vecRankSum, start, end); //flexlist: 1 to proofVector[i].getLength()
            start--;
        }
        else if(proofVector[i].isEnd()) {
            if(tagIndex != tags.size()-1) {
                interHashStack.push(hashOfPrev);
                index.push(pair<int,int>(start, end));
                start--;
                end = start;
            }
            hash = ProofNode::calculateHash(proofVector[i].getLevel(), proofVector[i].getRank(), tags[tagIndex--], proofVector[i].getHash());
            
        }
        else if(proofVector[i].isInter()) {
            end = index.top().second;
            updateRankSum(rankOfPrev, &vecRankSum, index.top().first, index.top().second); index.pop();
            hash = ProofNode::calculateHash(proofVector[i].getLevel(), proofVector[i].getRank(), hashOfPrev,  interHashStack.top());
            interHashStack.pop();
        }
        else if(proofVector[i].getRgtOrDwn() || proofVector[i].getLevel() == 0) {
            hash = ProofNode::calculateHash(proofVector[i].getLevel(), proofVector[i].getRank(), proofVector[i].getHash(), hashOfPrev);
            updateRankSum(proofVector[i].getRank() - rankOfPrev, &vecRankSum, start, end);
        }
        else {
            hash = ProofNode::calculateHash(proofVector[i].getLevel(), proofVector[i].getRank(), hashOfPrev, proofVector[i].getHash());
        }
        hashOfPrev = hash;
        //cout << "multihash:"<<hash<<endl;
        rankOfPrev = proofVector[i].getRank();
    }
    
    return hash;
}

bool DPDPFlexVerifier::OldVerifyInsert(vector<int> chalIndex, vector<ProofNode> proofVector, vector<ZZ> tags, ZZ blockSum, ZZ tag, int level) {
    if(!VerifyMultiProof(chalIndex, proofVector, tags, blockSum)){
        //cout << "wtf"<<endl;
        return false;
    }
    else {
        int rank = 1, i = proofVector.size() - 1, rankTower = proofVector[i].getRank();
        ZZ hash = tags[0], hashTower = ProofNode::calculateHash(0, rankTower, tag, proofVector[i].getHash());
        if (level != 0){
            hash = ProofNode::calculateHash(0, 1, hash, to_ZZ(0)); // is last argument right?
        }
        i--;
        while (proofVector[i].getLevel() < level || (proofVector[i].getLevel() == level && !proofVector[i].getRgtOrDwn()) ) {
            if (proofVector[i].getRgtOrDwn()) {
                rank += proofVector[i].getRank() - proofVector[i+1].getRank();
                hash = ProofNode::calculateHash(proofVector[i].getLevel(), rank, proofVector[i].getHash(), hash);
            }
            else {
                rankTower += proofVector[i].getRank() - proofVector[i+1].getRank();
                hashTower = ProofNode::calculateHash(proofVector[i].getLevel(), rankTower, hashTower, proofVector[i].getHash());
            }
            i--;
        }
        hash = ProofNode::calculateHash(level, rank + rankTower, hash, hashTower);
        MetaData = calculateRemainingHashes(i, hash, 1, proofVector);
        return true;
    }
}

ZZ DPDPFlexVerifier::calculateRemainingHashes(int i, ZZ hash, int rankDiff, vector<ProofNode> proofVector) { // is proofVector necessary?
    
    while (i >= 0) {
        if (proofVector[i].getRgtOrDwn()) {
            hash = ProofNode::calculateHash(proofVector[i].getLevel(), proofVector[i].getRank() + rankDiff, proofVector[i].getHash(), hash);
        }
        else {
            hash = ProofNode::calculateHash(proofVector[i].getLevel(), proofVector[i].getRank() + rankDiff, hash, proofVector[i].getHash());
        }
        
        i--;
    }
    
    return hash;
}

bool DPDPFlexVerifier::OldVerifyDelete(vector<int> chalIndex, vector<ProofNode> proofVector, vector<ZZ> tags, ZZ blockSum) {
    if(!VerifyMultiProof(chalIndex, proofVector, tags, blockSum)){
        //cout << "wtf"<<endl;
        return false;
    }
    else {
        
        int dn = proofVector.size() - 1, i = proofVector.size() - 2;
        while (!proofVector[i].isEnd()) {
            i--;
            
        }
        int rank = proofVector[dn].getRank();
        ZZ hash = ProofNode::calculateHash(0, rank, tags[0], proofVector[dn].getHash());
        
        
        dn--;
        if (!proofVector[dn].isEnd() || !proofVector[i].isInter()){
            
            i--;
        }
        //cout << "enterWhile"<<endl;
        while (!proofVector[dn].isEnd() || !proofVector[i].isInter()){
            if (proofVector[i].getLevel() < proofVector[dn].getLevel() || proofVector[dn].isEnd()) {
                rank += proofVector[i].getRank() - proofVector[i+1].getRank();
                hash = ProofNode::calculateHash(proofVector[i].getLevel(), rank, proofVector[i].getHash(), hash);
                
                i--;
            }
            else {
                rank += proofVector[dn].getRank() - proofVector[dn+1].getRank();
                hash = ProofNode::calculateHash(proofVector[dn].getLevel(), rank, hash, proofVector[dn].getHash());
                
                dn--;
                
            }
            
        }
        
        i--;
        //cout << "i:"<<i<<endl;
        MetaData = calculateRemainingHashes(i, hash, -1, proofVector);
        return true;
    }
}

RankBasedSkipList* DPDPFlexVerifier::generateDummySkipList(DPDPFlexVerifier* verifier,vector<ProofNode> firstProof, vector<ZZ> tags)
{
    RankBasedSkipList* dummyRbsl = new RankBasedSkipList(true, true, verifier->getN(), verifier->getG());
    stack<Node*> interStack;
    Node* currentNode;
    //Preparing the root to be put later in the flexlist
    RankBasedNode* root = new RankBasedNode(firstProof[0].getRank(), firstProof[0].getLevel());
    DummyRankBasedNode* dummyAfter = new DummyRankBasedNode(0, 0);
    dummyAfter->setHash(firstProof[0].getHash());
    root->setAfter(dummyAfter);
    //Root ready
    currentNode = root;
    int tagCount = 0;
    for(int i = 0; i < firstProof.size(); i++)
    {
        RankBasedNode* newNode = new RankBasedNode(firstProof[i+1].getRank(), firstProof[i+1].getLevel());
        
        
        if(firstProof[i].isEnd() && firstProof[i].isInter())
        {
            currentNode->setTag(tags[tagCount++]);
            currentNode->setData(Data(firstProof[i].getLength()));
            currentNode->setAfter(newNode);
            currentNode = currentNode->getAfter();
        }else if(firstProof[i].isEnd())
        {
            currentNode->setTag(tags[tagCount++]);
            currentNode->setData(Data(firstProof[i].getLength()));
            if(firstProof[i].getRank() != firstProof[i].getLength())
            {
                DummyRankBasedNode* dummyAfter = new DummyRankBasedNode(firstProof[i].getRank() - firstProof[i].getLength(), 0);
                dummyAfter->setHash(firstProof[i].getHash());
                currentNode->setAfter(dummyAfter);
            }
            if(!interStack.empty())
            {
                currentNode = interStack.top(); interStack.pop();
                currentNode->setAfter(newNode);
                currentNode = currentNode->getAfter();
            }
        }else if(firstProof[i].getLevel() == 0)
        {
            currentNode->setTag(firstProof[i].getHash());
            currentNode->setData(Data(firstProof[i].getRank() - firstProof[i+1].getRank()));
            currentNode->setAfter(newNode);
            currentNode = currentNode->getAfter();
            
        }else if(firstProof[i].isInter())
        {
            interStack.push(currentNode);
            currentNode->setBelow(newNode);
            currentNode = currentNode->getBelow();
        }else if(firstProof[i].getRgtOrDwn() == true) //prev => rgt
        {
            currentNode->setAfter(newNode);
            DummyRankBasedNode* dummyBelow = new DummyRankBasedNode(firstProof[i].getRank() - firstProof[i+1].getRank(), 0);
            dummyBelow->setHash(firstProof[i].getHash());
            currentNode->setBelow(dummyBelow);
            currentNode = currentNode->getAfter();
            
        }else { //prev => dwn
            currentNode->setBelow(newNode);
            DummyRankBasedNode* dummyAfter = new DummyRankBasedNode(firstProof[i].getRank() - firstProof[i+1].getRank(), 0);
            dummyAfter->setHash(firstProof[i].getHash());
            currentNode->setAfter(dummyAfter);
            currentNode = currentNode->getBelow();
        }
    }
    
    
    dummyRbsl->setRoot(root);
    
    return dummyRbsl;
    
}

ZZ DPDPFlexVerifier::getSeed(){return seed;}
void DPDPFlexVerifier::setSeed(ZZ s){ seed = s;}
ZZ DPDPFlexVerifier::getN(){return N;}
void DPDPFlexVerifier::setN(ZZ n){ N = n;}
ZZ DPDPFlexVerifier::getG(){return g;}
void DPDPFlexVerifier::setG(ZZ G){ g = G;}
ZZ DPDPFlexVerifier::getP(){return p;}
void DPDPFlexVerifier::setP(ZZ P){ p = P;}
ZZ DPDPFlexVerifier::getQ(){return q;}
void DPDPFlexVerifier::setQ(ZZ Q){ q = Q;}

ZZ DPDPFlexVerifier::getMetaData(){return MetaData;}
void DPDPFlexVerifier::setMetaData(ZZ MD){MetaData = MD;}

RankBasedSkipList* DPDPFlexVerifier::getRBSL() { return rbsl; }
//	RankBasedSkipList* DPDPFlexVerifier::getRBSLTEMP() { return rbslTemp; }
vector<Data>* DPDPFlexVerifier::getTempDatas() { return tempDatas; }

int DPDPFlexVerifier::getSize(){return size;}
void DPDPFlexVerifier::setSize(int s){ size = s;}


/*long long DPDPFlexVerifier::divideAndBuildSkipList_sendFileToServer(ZZ seed, vector<ZZ> *tags, char* fileName, int blockLength)
 {
 SetSeed( seed );
 rbsl = new RankBasedSkipList(true,N, g); //flexlist: true
 //rbslTemp = new RankBasedSkipList(true,N, g); //flexlist: true
 tempDatas = new vector<Data>();
 
 
 //srand ( to_int(seed) );
 ZZ tag;
 ZZ MD;
 long long timer = 0;
 
 int blockCounter=0;//XXX:will be read from file and be written to file at the end XXX
 blockCounter = Data::readCounterInt("clientFolder/counter.txt");
 char fileNametxt[80] = "";
 strcat(fileNametxt, fileName);
 strcat(fileNametxt, ".txt");
 
 
 Data d(fileNametxt);
 
 vector<char> file = d.getBytesFromFile();
 vector<char>::iterator it= file.end();
 //for(int i = 0; i < 5 ; i++,blockCounter++)
 srand ( to_int(seed) );
 
 ZZ blockSum;
 blockSum = 0;
 
 for(;it>file.begin() ;blockCounter++)
 {
 it -= blockLength+1;
 if(it < file.begin())
 it = file.begin();
 vector<char> aBlock;
 aBlock.clear();
 
 //			cout<<"it:"<<*it<<endl;
 std::copy((it < file.begin()) ? file.begin() : it, it + blockLength, std::back_inserter(aBlock));
 //for(int i = 0; i < aBlock.size() ; i++)
 //	{
 //		cout<<aBlock[i];
 //	}
 //	cout<<endl;
 
 
 char str[100];
 sprintf(str, "%d", blockCounter);
 char isim[80] = "";
 
 char fileNameTemptxt[80] = "";
 strcat(fileNameTemptxt, fileName);
 strcat(fileNameTemptxt, str);
 strcat(fileNameTemptxt, "temp");
 strcat(fileNameTemptxt, ".txt");
 
 
 strcat(isim, fileName);
 strcat(isim, str);
 strcat(isim, ".txt");// = fileName + blockCounter + ".txt";
 //cout<<"blockCounter: "<<blockCounter<<endl;
 Data d(aBlock,isim,blockCounter);
 Data dtemp(aBlock,fileNameTemptxt,blockCounter); //for temp files that are needed for alterations ONLY ON VERIFIER SIDE
 startTimer();
 //ZZ val;
 //val = 1;
 tag = calculateTag(d.getValuePers(fileName), N, g, p, q);//j
 timer += elapsedTime();
 tags->push_back(tag);
 //cout<<"tag first calc: "<<tag<<endl;
 rbsl->insert(Key(0), d, tag);
 // for alterations management
 tempDatas->push_back(dtemp); // XXX: to be used in reverse order!!!
 
 //blockSum+=d.getValuePers("clientFolder/aFileOf16000KiB");
 
 }
 */