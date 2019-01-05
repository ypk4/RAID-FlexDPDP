/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include <stdlib.h>
#include <ctime>
#include <math.h>
#include "SkipList.h"
#include <algorithm>

SkipList::SkipList(Type type) {
    T = type;
    size = 0;
    srand((unsigned)time(0));
    // creation of root and sentinel nodes
    root = createNode(-1 * INT_MAX , INT_MAX);
    Node* after = createNode(INT_MAX -1 , 0);
    root->setAfter(after);
    Node* below = createNode(-1 * INT_MAX , 0);
    root->setBelow(below);
    
    root->getBelow()->setLeafAfter(root->getAfter()); //Leaf level connection
}
SkipList::SkipList(bool n, Type type) {
    T = type;
    size = 0;
}

SkipList::~SkipList() {
    root->freeAll(); // free all nodes in a skip list
}

Node* SkipList::createNode(Key k, int lvl) {
    return
    (T == SL)
    ?new Node(k , lvl)
    :(T == AuthenticatedSL)
    ?new AuthenticatedNode(k , lvl)
    :(T == RankBasedSL || FlexSL) //flexlist
    ?new RankBasedNode(Key(1) , lvl)
    :NULL;//This can be  extended for further use(extentions)
}

int SkipList::getSize() {
    return size;
}

Node* SkipList::getRoot() {
    return root;
}
void SkipList::setRoot(Node* newRoot) {
    root = newRoot;
}

void SkipList::incrementSize() {
    size++;
}

void SkipList::decrementSize() {
    size--;
}



Node* SkipList::build(vector<Data> Blocks, vector<int> Levels, vector<ZZ> Tags, int maxLevel, int order, int partCount){
    vector<Node*> topTower(maxLevel + 1); //entries initialized to NULL?
    Node* newNode;
    int part = (Levels.size()) / partCount;  //***************SURAYI BIR BASTAN YAZ YAHU
    int lastIndex = (order + 1 == partCount)? Levels.size() -1 : (part * (order+1)) - 1;
    int firstIndex = order * part;
    //cout<<"firstIndex: "<<firstIndex<<endl;
    //cout<<"lastIndex: "<<lastIndex<<endl;
    //for(int i = Levels.size()-1; i >= 0; i--){
    for(int i = lastIndex; i >= firstIndex ; i--){
        newNode = buildLeaf(Blocks[i], Tags[i], &topTower);  //blocks and tags were unsing [0] for buildSL test
        //newNode->calculateRank();
        buildTower(newNode, Levels[i], &topTower);
    }
    /*if(order == 0){
     newNode = buildLeaf(Blocks[0], Tags[0], &topTower);  //blocks and tags are [0] on purpose as they are the beginning blocks
     //newNode->calculateRank();
     buildTower(newNode, Levels[0], &topTower);
     }*/
    
    // Build sentinel nodes
    newNode = createNode(1, 0); //How to initialize nodes? is key the rank?
    if(order != 0)
        newNode->setData(Data(1));
    //newNode->calculateHash(getModulus(),getElement());  //XXX:WAS WRONG HERE, CAN NOT CALCULATE HASH WO SETTING ITS AFTER NODE!!
    topTower.push_back(newNode);
    newNode = buildLeaf((order != 0)?Data(1):Data(0), to_ZZ(-1), &topTower);
    buildTower(newNode, maxLevel + 1, &topTower);
    root = topTower[maxLevel + 1];
    root->setLevel(INT_MAX); //how to handle infinity? include <limits>?
    
    cout << "Inside build() : root hash value = (uninitialized value) " << root->getHash() << endl;

    root->calculateHash(getModulus(),getElement());
    size = Levels.size();  //not correct for parallel case, but can be set at the end already
    
    cout << "Inside build() : root hash value = " << root->getHash() << endl;
    
    root->getAfter()->setData(Data(0));
    root->getAfter()->calculateRank();
    root->calculateRank();
    
    return root;
}

Node* SkipList::buildLeaf(Data block, ZZ tag, vector<Node*>* topTower){
    Node* newNode = createNode(1, 0);
    newNode->setData(block);
    newNode->setTag(tag);
    if ((*topTower)[0] != NULL){
        newNode->setAfter((*topTower)[0]);
        (*topTower)[0] = NULL;
    }
    //if( T == RankBasedSL) {
    newNode->calculateRank();
    if(tag != to_ZZ(-1)){  //XXX: I ADDED THIS CHECK FOR NOT TO CALCULATE HASH VALUE FOR SENTINEL NODES. AS LONG AS TWO SIDES USE THE SAME ALGORITHM SHOUDNT BE A PROBLEM BUT WE MAY DECIDE THIS LATER!!!
        newNode->calculateHash(getModulus(),getElement());
    }
    //}
    //else if(T == AuthenticatedSL)
    //	newNode->calculateHash(getModulus(),getElement()); // how to calculate hash?
    return newNode;
}

Node* SkipList::build(vector<ZZ> Blocks, vector<int> Levels, vector<ZZ> Tags, int maxLevel, int order, int partCount){
    vector<Node*> topTower(maxLevel + 1); //entries initialized to NULL?
    Node* newNode;
    int part = (Levels.size() - 1) / partCount;
    //for(int i = Levels.size()-1; i >= 0; i--){
    for(int i = floor(part * (order+1)); i > ((double)part * ((double)order)) ; i--){
        newNode = buildLeaf(Blocks[0], Tags[0], &topTower);  //blocks and tags were unsing [0] for buildSL test
        //newNode->calculateRank();
        buildTower(newNode, Levels[i], &topTower);
    }
    if(order == 0){
        newNode = buildLeaf(Blocks[0], Tags[0], &topTower);  //blocks and tags are [0] on purpose as they are the beginning blocks
        //newNode->calculateRank();
        buildTower(newNode, Levels[0], &topTower);
    }
    
    // Build sentinel nodes
    newNode = createNode(1, 0); //How to initialize nodes? is key the rank?
    
    newNode->calculateHash(getModulus(),getElement());
    topTower.push_back(newNode);
    newNode = buildLeaf(to_ZZ(-1), 0, &topTower);
    buildTower(newNode, maxLevel + 1, &topTower);
    root = topTower[maxLevel + 1];
    root->setLevel(INT_MAX); //how to handle infinity? include <limits>?
    
    root->calculateHash(getModulus(),getElement());
    size = Levels.size();  //not correct for parallel case, but can be set at the end already
    return root;
}

Node* SkipList::buildLeaf(ZZ block, ZZ tag, vector<Node*>* topTower){
    Node* newNode = createNode(1, 0);
    newNode->setData(block);
    newNode->setTag(tag);
    if ((*topTower)[0] != NULL){
        newNode->setAfter((*topTower)[0]);
        (*topTower)[0] = NULL;
    }
    //if( T == RankBasedSL) {
    newNode->calculateRank();
    newNode->calculateHash(getModulus(),getElement());
    //}
    //else if(T == AuthenticatedSL)
    //	newNode->calculateHash(getModulus(),getElement()); // how to calculate hash?
    return newNode;
}

void SkipList::buildTower(Node* previousNode, int level, vector<Node*>* topTower){
    for (int i = 1; i <= level; i++){
        if ((*topTower)[i] != NULL){
            Node* newNode = createNode(0, i);
            newNode->setBelow(previousNode);
            newNode->setAfter((*topTower)[i]);
            //if( T == RankBasedSL) {
            newNode->calculateRank();
            newNode->calculateHash(getModulus(),getElement());
            //}
            //else if(T == AuthenticatedSL)
            //	newNode->calculateHash(getModulus(),getElement());
            previousNode = newNode;
            (*topTower)[i] = NULL;
        }
    }
    (*topTower)[level] = previousNode;
}

Node* SkipList::search(Key key, stack<Node*>* s, bool nullifyHashes) {
    return search(&key, s, nullifyHashes);
}

// searches for the given keyed node in a skip list
Node* SkipList::search(Key *key, stack<Node*>* s, bool nullifyHashes) {//flexlist
#ifdef DEBUG
    char * log = new char[250];	sprintf(log,
                                        "%25s ( with key: %d )"," SkipList::search", key.getValue());
    logger::log(log);
#endif
    if((T == SL || T == AuthenticatedSL) && (key->getValue() >= INT_MAX -1 || key->getValue() <= -1 * (INT_MAX - 1 ))) return NULL;
    if(T == RankBasedSL)
        if((key->getValue() < 1) || (key->getValue() > getSize()))
            return NULL;
    Node* previousNode = root;
    Node* currentNode = root;
    if(nullifyHashes) currentNode->setHash(NULL);
    // currentNode moves until currentNode.after is a tower node of the searched node
    nextPos (&previousNode, &currentNode, key, -1 , true, s, nullifyHashes);
    Key keyCheck = *key;
    key->setValue(key->getValue() - currentNode->getBelowKeyValue()); //flexlist
    if(currentNode->getAfter() == NULL) return NULL;
    currentNode = currentNode->getAfter();
    if(currentNode != NULL && nullifyHashes) currentNode->setHash(NULL);
    if(s != NULL && currentNode != NULL) s->push(currentNode);
    // currentNode is moved below until the node at the leaf level, which has the data
    while(currentNode != NULL && currentNode->getLevel() != 0) {
        currentNode = currentNode->getBelow();
        if (nullifyHashes) currentNode->setHash(NULL);
        if(s != NULL) s->push(currentNode);
    }
    
    if(currentNode != NULL && ((currentNode->getKey().compareTo(keyCheck) == 0) || T == RankBasedSL || T == FlexSL)){ //flexlist
        return currentNode;
    }
    return NULL;
}

// searches for a node with given key and modifies its data and tag values
Node* SkipList::modify(Key key, Data data, ZZ givenTag, stack<Node*>* s, int* oldLength, bool nullifyHashes) {
#ifdef DEBUGSL
    char * log = new char[250];	sprintf(log,
                                        "%25s ( with key: %d %s %f )"," SkipList::modify", key.getValue(), "Data:", data.getValue());
    logger::log(log);
#endif
    //if((T == SL || T == AuthenticatedSL) && (key.getValue() >= INT_MAX -1 || key.getValue() <= -1 * (INT_MAX - 1 ))) return NULL;
    Node* returnNode = search(key, s, nullifyHashes);
    if(oldLength != NULL)
        *oldLength = returnNode->getData().getLength();
    if(returnNode != NULL){
        returnNode->setData(data);
    }
    
#ifdef DETAILEDSL
    printLog();
#endif
    
    return returnNode;
}


// inserts new node into a skip list with given key, data, tag and at given level
Node* SkipList::insert(Key key, Data data, ZZ givenTag, stack<Node*>* s, int givenLevel, bool nullifyHashes) {
#ifdef DEBUGSL
    char * log = new char[250];	sprintf(log,
                                        "%25s ( with key: %d %s %f %s %2d )"," SkipList::insert", key.getValue(), "Data:", data.getValue(), "level: ",givenLevel);
    logger::log(log);
#endif
    
    if((T == SL || T == AuthenticatedSL) && (key.getValue() >= INT_MAX -1 || key.getValue() <= -1 * (INT_MAX - 1 ))) return NULL;
    
    Node* previousNode = root;
    Node* currentNode = root;
    if(nullifyHashes) currentNode->setHash(NULL);
    int levelToInsert = (givenLevel < 0) ? tossCoins() : givenLevel;
    
    Node* newNode = createNode(key, levelToInsert); //new node's hash value is null already
    // currentNode moves until it finds a missing node or currentNode.after is where newNode is to be inserted
    nextPos(&previousNode, &currentNode, &key, levelToInsert, true, s, nullifyHashes);
    
    bool canGoDown = currentNode->canGoDown(key, levelToInsert);
    // check if there is a node where new node will be linked. If not, create one
    if(!canGoDown || currentNode->getLevel() != levelToInsert){
        createMissingNode(&previousNode, &currentNode, &key, levelToInsert,
                          !canGoDown, s);
    }
    // create a new node and insert after the currentNode
    newNode->setAfter(currentNode->getAfter());
    currentNode->setAfter(newNode);
    
    if(s != NULL) s->push(newNode);
    // create insertion tower until the leaf level is reached
    while(currentNode->getBelow() != NULL) {
        if(newNode->getAfter() != NULL){
            newNode->setBelow(createNode(key)); // the hash value for the newly created node is left null
            newNode = newNode->getBelow();
            if(s != NULL) s->push(newNode);
        }
        if(currentNode->canGoDown(key,0))
            oneStepDo( &previousNode, &currentNode, currentNode->getBelow(), s, nullifyHashes);
        // currentNode moves until we reach an after link that passes through a tower.
        // That is the insertion point for the new node.
        nextPos(&previousNode, &currentNode, &key, 0, false, s, nullifyHashes);
        // Create next node of the insertion tower
        newNode->setAfter(currentNode->getAfter());
        newNode->setLevel(currentNode->getLevel());
        // currentNode becomes unnecessary as it looses its after link, therefore it is deleted
        removeNode(&previousNode, &currentNode, &key, s);
    }
    // done inserting, put the data and return this last node
    newNode->setLeafAfter(currentNode->getLeafAfter());
    currentNode->setLeafAfter(newNode);
    
    newNode->setData(data);
    incrementSize();
    
#ifdef DETAILEDSL
    printLog();
#endif
    
    return newNode;
}

// remove a node from a skip list with the given key
Node* SkipList::remove(Key key, stack<Node*>* s, bool nullifyHashes) {
#ifdef DEBUGSL
    char * log = new char[250];	sprintf(log,
                                        "%25s ( with key: %d )"," SkipList::remove", key.getValue());
    logger::log(log);
#endif
    if((T == SL || T == AuthenticatedSL) && (key.getValue() >= INT_MAX -1 || key.getValue() <= -1 * (INT_MAX - 1 ))) return NULL;
    Node* previousNode = root;
    Node* currentNode = root;
    if(nullifyHashes) currentNode->setHash(NULL);
    Node* freeNode;
    // currentNode moves until after of the currentNode is the node at the top of deletion tower
    nextPos(&previousNode, &currentNode, &key, -1, true, s, nullifyHashes);
    if(currentNode->getAfter() == NULL) return NULL;
    Node* nodeToDelete = currentNode->getAfter();
    // check if currentNode is necessary, if so it can steal after of the node to delete, otherwise delete currentNode
    if(currentNode->getLevel() == nodeToDelete->getLevel()) {
        currentNode->setAfter(nodeToDelete->getAfter());
        if(nodeToDelete->getBelow() != NULL) { // unless at leaf level
            freeNode = nodeToDelete;
            nodeToDelete = nodeToDelete->getBelow();
            delete freeNode;
        }
    }
    else {
        removeNode(&previousNode , &currentNode, &key, s);
    }
    // delete whole deletion tower until the leaf level is reached
    while(currentNode->getBelow() != NULL) {
        // currentNode moves until it finds a missing node
        nextPos(&previousNode ,&currentNode, &key, nodeToDelete->getLevel(), true, s, nullifyHashes);
        // create missing node unless at leaf level and steal the after link of the node to delete
        if(currentNode->getLevel() != 0)
            createMissingNode(&previousNode, &currentNode, &key, nodeToDelete->getLevel(),
                              !currentNode->canGoDown(key,nodeToDelete->getLevel()), s); //missing node's hash value is left null
        currentNode->setAfter( nodeToDelete->getAfter());
        // move nodeToDelete to the next node in the deletion tower unless at leaf level
        if(nodeToDelete->getBelow() != NULL) {
            freeNode = nodeToDelete;
            nodeToDelete = nodeToDelete->getBelow();
            delete freeNode;
        }
    }
    // for leaf level connections to be maintained
    currentNode->setLeafAfter(nodeToDelete->getLeafAfter());
    
    decrementSize();
    
#ifdef DETAILEDSL
    printLog();
#endif
    
    return nodeToDelete;
}

// function creates a node to maintain links, a node is to be connected to a place where a node is missing
bool SkipList::createMissingNode(Node** previousNode, Node** currentNode, Key* key, int levelToInsert,  bool canGoDown, stack<Node*> *s) {
#ifdef DEBUGSL
    char * log = new char[250];	sprintf(log,
                                        "%25s ( with key: %d ) currentNodeKey: %d currentNodeData: %f canGoDown: %s" ," SkipList::createMissingNode", key->getValue(), (*currentNode)->getKey().getValue(), (*currentNode)->getData().getValue(), canGoDown?"true":"false");
    logger::log(log);
#endif
    
    Node* w = (*currentNode)->getAfter();
    Node* z = (*currentNode)->getBelow();
    Node* missNode = createNode((canGoDown? w->getKey() : z->getKey()), levelToInsert);
    missNode->setBelow( canGoDown?w : z);
    canGoDown? (*currentNode)->setAfter(missNode) : (*currentNode)->setBelow(missNode);
    *previousNode = *currentNode;
    *currentNode = missNode;
    
    //the hash value for the created missing node remains null.
    if(s!= NULL) s->push(*currentNode);
    return true;
}

// deletes unnecessary node by maintaining the links
bool SkipList::removeNode(Node** previousNode, Node** currentNode, Key* key, stack<Node*>* s) {
    if((*currentNode)->getLevel() != 0) {
        Node* forFree = *currentNode;
        deleteNode(previousNode, currentNode, key, s);
        
        *currentNode = *previousNode;
        delete forFree;
    }
    else
        (*currentNode)->setAfter(NULL);
    
    return true;
}

// method takes two nodes n1 and n2 as input. It removes n2 from skip list by setting n1's after to n2's after
bool SkipList::deleteNode(Node** previousNode, Node** currentNode,  Key* key, stack<Node*>* s) {
#ifdef DEBUGSL
    char * log = new char[250];	sprintf(log,
                                        "%25s ( with key: %d ) currentNodeKey: %d currentNodeData: %f"," SkipList::deleteNode", key->getValue(), (*currentNode)->getKey().getValue(), (*currentNode)->getData().getValue());
    logger::log(log);
#endif
    
    if(s != NULL) s->pop();
    if((*previousNode)->getBelow() == *currentNode)
        (*previousNode)->setBelow((*currentNode)->getBelow());
    else
        (*previousNode)->setAfter((*currentNode)->getBelow());
    
#ifdef DEBUGSL
    log = new char[250];	sprintf(log,
                                    "%25s ( with key: %d ) new currentNodeKey: %d new currentNodeData: %f"," After SkipList::deleteNode", key->getValue(), (*currentNode)->getKey().getValue(), (*currentNode)->getData().getValue());
    logger::log(log);
#endif
    
    return true;
}

// method moves currentNode repetitively until the desired position according to the
// method (search, insert, remove) from which it is called
bool SkipList::nextPos(Node** previousNode, Node** currentNode, Key* key, int levelToInsert, bool npi, stack<Node*>* s, bool nullifyHashes) {
    while(oneStep(previousNode, currentNode, key, levelToInsert, npi, s, nullifyHashes));
    return true;
}

// method checks where next move is to be made, whether we can follow below link or after link
bool SkipList::oneStep(Node** previousNode, Node** currentNode, Key* key, int levelToInsert, bool npi, stack<Node*>* s, bool nullifyHashes) {
#ifdef DETAILEDSL
    char * log = new char[250];	sprintf(log,
                                        "%25s ( with key: %d ) currentNodeKey: %d currentNodeData: %f lvl2Insert: %d npi: %s"," SkipList::oneStep", key->getValue(), (*currentNode)->getKey().getValue(), (*currentNode)->getData().getValue(), levelToInsert, npi?"true":"false");
    logger::log(log);
#endif
    
    Node* w =  (*currentNode)->getAfter();
    Node* z =  (*currentNode)->getBelow();
    if(z != NULL){
        if((*currentNode)->canGoDown(*key, levelToInsert) && z->getLevel() >= levelToInsert && npi){
            return oneStepDo(previousNode, currentNode, z, s, nullifyHashes);
        }
    }
    if(w != NULL){
        if((*currentNode)->canGoRight(*key) && w->getLevel() >= levelToInsert){
            return oneStepDo(previousNode, currentNode, w, s, nullifyHashes);
        }
    }
    
#ifdef DETAILEDSL
    log = new char[250];	sprintf(log,
                                    "%25s ( with key: %d ) currentNodeKey: %d currentNodeData: %f"," After SkipList::oneStep (NextPos done)", key->getValue(), (*currentNode)->getKey().getValue(), (*currentNode)->getData().getValue());
    logger::log(log);
#endif
    
    return false;
}

// performs a move after decision in OneStep function
bool SkipList::oneStepDo(Node** previousNode, Node** currentNode, Node* nextNode , stack<Node*>* s, bool nullifyHashes) {
    *previousNode = *currentNode;
    *currentNode = nextNode;
    if(s != NULL) s->push(*currentNode);
    if(*currentNode != NULL)
        if(nullifyHashes)
            (*currentNode)->setHash(NULL);
    return true;
}
// randomly tosses coins and determines level for insertion
int SkipList::tossCoins() {
    int coinsTotal = 0;
    while((rand() % 2) == 1)
        coinsTotal++;
    
#ifdef DEBUG
    char* nodeString = new char[128];
    sprintf(nodeString,"Insert levelToInsert: %d",coinsTotal);
    logger::log(nodeString);
#endif
    
    return coinsTotal;
}
// prints skip list
void SkipList::print() {
    cout<<"/-------------------------------------------------\\"<<endl;
    cout<<"size :  "<< getSize() <<endl;
    printHelper(*root);
    cout<<"\\-------------------------------------------------/"<<endl;
}
// helper function to print skip list
void SkipList::printHelper(Node node) {
    
    node.print();
    
    if(node.getAfter() != NULL) {
        printHelper(*node.getAfter());
    }
    if(node.getBelow() != NULL) {
        printHelper(*node.getBelow());
    }
}

void SkipList::printLog() {
    char* nodeString = new char[128];
    logger::log("/-------------------------------------------------\\");
    sprintf(nodeString,"size: %d",getSize());
    logger::log(nodeString);
    printLogHelper(*root);
    logger::log("/-------------------------------------------------\\");
}

void SkipList::printLogHelper(Node node) {
    
    node.printLog();
    
    if(node.getAfter() != NULL) {
        printLogHelper(*node.getAfter());
    }
    if(node.getBelow() != NULL) {
        printLogHelper(*node.getBelow());
    }
}


bool SkipList::testPrintHelper(Node *node, stack<ZZ>* m) {
    ZZ check;
    if(m != NULL && !m->empty()) {
        check = (ZZ) m->top();
        m->pop();
        if(check != node->getData().getValue())
            return false;
    }
    
    bool child = true;
    if(node->getAfter() != NULL) {
        child = testPrintHelper(node->getAfter(), m);
    }
    if(node->getBelow() != NULL) {
        child = child && testPrintHelper(node->getBelow(), m);
    }
    return child;
}
bool SkipList::testSkipList(stack<ZZ>* m) {
    return testPrintHelper(root, m);
}
