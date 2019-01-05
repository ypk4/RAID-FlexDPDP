/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "RankBasedSkipList.h"


// constructor for authenticated rank based skip list
RankBasedSkipList::RankBasedSkipList(ZZ compositeN, ZZ randomG)
: AuthenticatedSkipList(RankBasedSL, compositeN, randomG)
{
    init();
}
// constructor for authenticated flex list
RankBasedSkipList::RankBasedSkipList(bool isFlexList, ZZ compositeN, ZZ randomG)
: AuthenticatedSkipList(FlexSL, compositeN, randomG)
{
    root->setKey(Key(0));
    root->getAfter()->setKey(Key(0));
    root->getBelow()->setKey(Key(0));
    root->getBelow()->setData(Data());
    root->getAfter()->setData(Data());
    
}
RankBasedSkipList::RankBasedSkipList(bool n, bool isFlexList, ZZ compositeN, ZZ randomG)
: AuthenticatedSkipList(n, FlexSL, compositeN, randomG)
{
}
/*RankBasedSkipList::RankBasedSkipList(bool n, ZZ compositeN = 1, ZZ randomG = 1)
 : AuthenticatedSkipList(n, RankBasedSL, compositeN, randomG)
 {
 }*/
void RankBasedSkipList::init(){
    root->setKey(Key(2));
    root->getAfter()->setKey(Key(1));
    root->getBelow()->setKey(Key(1));
}

void RankBasedSkipList::multiUpdate(vector<DiffSkipListSimple*> diffs){
    
    for(int i = 0 ; i < diffs.size() ; i++)
    {
        if(diffs[i]->getType() == DiffSkipListSimple::MODIFY)
            this->modifyWOHashCalculation(Key(diffs[i]->getIndex()),*(diffs[i]->getData()),diffs[i]->getTag(),NULL);
        if(diffs[i]->getType() == DiffSkipListSimple::INSERT)
            this->insertWOHashCalculation(Key(diffs[i]->getIndex()),*(diffs[i]->getData()),diffs[i]->getTag(),NULL);
        if(diffs[i]->getType() == DiffSkipListSimple::REMOVE)
            this->removeWOHashCalculation(Key(diffs[i]->getIndex()));
    }
    
    ((AuthenticatedNode*)this->root)->calculateHashRec();
}

Node* RankBasedSkipList::parallelBuild(vector<Data> *datas, vector<int> *Levels, vector<ZZ> *tags, int max, int coreCount){
    
    double* timers = new double[3];
    int timer = 0;
    
    //for test(just print):
    //vector<RankBasedSkipList*> sls(coreCount);
    //////////
    
    vector<RankBasedNode*> roots(coreCount);
    
    startTimer();
    vector<Worker*> threads(coreCount);
    float divideTime = 0;
    for(int i = 0; i < coreCount; i++)
    {
        //threads[i] = new Worker(*datas, *Levels, *tags, max, i, coreCount);
        //threads[i]->start(getModulus(), getElement(), &roots);
        
        if(coreCount != 1){
            startTimer();
            int part = (Levels->size()) / coreCount;  //***************SURAYI BIR BASTAN YAZ YAHU
            int lastIndex = (i + 1 == coreCount)? Levels->size() -1 : (part * (i+1)) - 1;
            int firstIndex = i * part;
            
            vector<Data>::const_iterator first = datas->begin() + firstIndex;
            vector<Data>::const_iterator last = datas->begin() + lastIndex+1;
            vector<Data> newDataVec(first, last);
            vector<int>::const_iterator firstL = Levels->begin() + firstIndex;
            vector<int>::const_iterator lastL = Levels->begin() + lastIndex+1;
            vector<int> newLevelVec(firstL, lastL);
            vector<ZZ>::const_iterator firstT = tags->begin() + firstIndex;
            vector<ZZ>::const_iterator lastT = tags->begin() + lastIndex+1;
            vector<ZZ> newTagVec(firstT, lastT);
            divideTime += printTimer(timer, "one divisionTime");
            threads[i] = new Worker(newDataVec, newLevelVec, newTagVec, max);
            
        }else{
            
            threads[i] = new Worker(*datas, *Levels, *tags, max);
        }
        threads[i]->start(getModulus(), getElement(), &roots, i);
    }
    timers[timer++] = printTimer(timer, "divide into tasks");
    startTimer();
    
    
    
    
    for(int i = 0; i < coreCount; i++)
    {
        threads[i]->join();
    }
    timers[timer++] = printTimer(timer, "all subtasks");
    startTimer();
    
    //cout<<"part by part FlexList: "<<endl;
    //for(int i = 0; i < sls.size(); i++)
    //sls[i]->print();
    //roots[i]->printRB();
    
    //**//CONNECT ALL SKIPLISTS TOGETHER //**// SUM UP
    
    //SETTING ALL ROOT NODES AS EACH OTHERS AFTER IN THE ORDER OF THE DATA AND SETTING THEIR LEVELS
    int rootsSize = roots.size();
    for(int i = rootsSize - 1 ; i >= 0 ; i--)
    {
        roots[i]->setLevel(max + 1);
        if(i != 0)
            roots[i-1]->setAfter(roots[i]);
        roots[i]->calculateRank();
    }
    
    //ADDING A ROOT FOR THE NEW SKIP LIST SO THAT WE WILL DO REMOVE OPERATIONS
    //RankBasedSkipList* generatedFlexList = new RankBasedSkipList(true, true, getModulus(), getElement());
    RankBasedNode* newRoot = new RankBasedNode(Key(1) , INT_MAX);
    newRoot->setBelow(roots[0]);
    newRoot->calculateRank();
    //	generatedFlexList->
    setRoot(newRoot);
    
    //generatedFlexList->print();
    
    //NOW WE WILL REMOVE THE SENTINEL NODES WHICH ARE LEFT BEHIND
    //lets find them
    vector<int> sentinelNodeIndices;
    for(int r = 1 ; r < roots.size() ; r++)
    {
        //cout<<"sentinelNodeIndices "<< r <<" :"<< roots[0]->getKey().getValue() - roots[r]->getKey().getValue() <<endl;
        sentinelNodeIndices.push_back(roots[0]->getKey().getValue() - roots[r]->getKey().getValue());
    }
    //lets remove the sentinel node in between
    for(int si = sentinelNodeIndices.size() -1 ; si >= 0 ; si--)
        //generatedFlexList->
        remove(Key(sentinelNodeIndices[si]));
    
    //Now, lest touches, where we set the root level and the sentinel nodes
    setRoot(getRoot()->getBelow());
    getRoot()->setLevel(INT_MAX);
    timers[timer++] = printTimer(timer, "connecting sub lists together time");
    //startTimer();
    //timers[timer++] = printTimer(timer, "Skip list generation by build function multi core");
    
    //generatedFlexList->
    //print();
    
    /*// if you uncomment above timers, you may want to print them here as well...
     cout<<"<------------------>"<<endl;
     cout<<"divide into tasks: "<<timers[0]<<endl;
     cout<<"do tasks: "<< timers[1]<<endl;
     cout<<"connect lists together: "<<timers[2]<<endl;
     cout<<"<------------------>"<<endl;
     cout<<"parallel build skip list time: "<<timers[0] + timers[1] + timers[2]<<endl;
     cout<<"<------------------>"<<endl;
     */
}

//void SkipList::workerFunc(vector<Data> *datas, vector<int> *Levels, vector<ZZ> *tags, int max, int order, int coreCout, vector<RankBasedNode*> *roots/*, vector<RankBasedSkipList*> *sls*/)
//{
//		RankBasedSkipList* aPartOfRbsl = new RankBasedSkipList(true, getModulus(), getElement());
//		aPartOfRbsl->build(*datas, *Levels, *tags, max, order, coreCout);
//test
//(*sls)[order] = aPartOfRbsl;
/////
//		(*roots)[order] = (RankBasedNode*)aPartOfRbsl->getRoot();

//}


void RankBasedSkipList::calculateMultiHash(vector<int> changedNodes) {//XXX: differences from proof mech.
    sort(changedNodes.begin(), changedNodes.begin()+changedNodes.size());
    
    //cout<<"changedNodes(sorted): ";
    //for(int k = 0; k < changedNodes.size() ; k++)
    //	cout<<changedNodes[k]<<" ,";
    //cout<<"------"<<endl;
    
    Node* currentNode = getRoot();
    stack<State*> interStack;
    vector<Node*> changedNodeVector;//XXX: Not proof vectors but pointers to nodes
    State* state = new State(*currentNode, changedNodes.size()-1, 0);
    int rankState = 0;
    for(int i=0; i < changedNodes.size(); i++) {
        // with hashMulti Method we aim to collect all nodes on any search path of changedNodes
        hashMulti(currentNode, changedNodes, i, state->getEndIndex(), state->getRankState(), getModulus(), getElement(), &changedNodeVector, &interStack);
        
        if(!interStack.empty()){
            state = interStack.top(); interStack.pop();
            currentNode = state->getNode().getAfter();
            if(currentNode == NULL) cout<<"PROBLEM with the current Node occured!"<<endl;
            state->setRankState(state->getRankState() + state->getNode().getBelowKeyValue());
        }
    }
    
    delete state;
    
    
    for(int i = changedNodeVector.size() -1 ; i > -1 ; i--){
        //changedNodeVector[i]->print();
        changedNodeVector[i]->calculateHash(getModulus(), getElement()); //XXX:doesnt return but calculates
    }
}
void RankBasedSkipList::hashMulti(Node* currentNode, vector<int> chalIndex, int start, int end, int rankState, ZZ N, ZZ g, vector<Node*>* changedNodeVector, stack<State*>* interStack) {
    
    Key key(chalIndex[start]-rankState);// Index of the challenged block is calculated according to the current imaginary root
    
    bool breakPoint = true;
    bool canGoDown;
    int rankDiff;
    int tempEnd;
    
    while(breakPoint){
        // End of this branch of the seach path is when the current node reaches the changed node
        changedNodeVector->push_back(currentNode);
        if ( currentNode->getLevel() == 0  && /*key.getValue() == 0*/key.getValue() < currentNode->getData().getLength() ) { /*key.getValue() == 0*///this check was direct conversion of rank based to flexlist where only the start index of nodes could be challenged
            
            breakPoint = false;
            //When an intersection is found with another branch of the proof path, it is saved to be continued again, this crucial for outer loop of multi algorithms
            
        }
        canGoDown = currentNode->canGoDown(key, -1);
        
        rankDiff = currentNode->getBelowKeyValue();
        tempEnd = end;
        if (currentNode->isIntersecting(canGoDown, chalIndex, key.getValue(), &end, rankState)) {
            //INTERSECTING NODE
            interStack->push(new State(*currentNode, tempEnd, rankState));
            
        }
        
        if (canGoDown) {
            if(breakPoint)
                currentNode = currentNode->getBelow();
        }
        else if(breakPoint){
            key.setValue(key.getValue() - currentNode->getBelowKeyValue());
            rankState = rankState + currentNode->getBelowKeyValue();
            currentNode = currentNode->getAfter();
        }
        
        
    }
}

Node* RankBasedSkipList::modify(Key key, Data data, ZZ givenTag, stack<Node*>* s1) {
#ifdef DEBUGRB
    char * log = new char[250];	sprintf(log,
                                        "%25s ( with key: %d %s %f )"," RankBasedSkipList::modify", key.getValue(), "Data:", data.getValue());
    logger::log(log);
#endif
    stack<Node*> s;
    // modify using regular skip list method
    Node* returnNode = SkipList::modify(key, data, 0, &s);
    // change tag, recalculate rank and hash values along the modify path
    returnNode->print();
    returnNode->setTag(givenTag);
    calculateHashAndRank(s);
    
#ifdef DETAILEDRB
    printLog();
#endif
    
    return returnNode;
}

Node* RankBasedSkipList::modifyWOHashCalculation(Key key, Data data, ZZ givenTag, stack<Node*>* s1, int* oldLength) {
    
    stack<Node*> s;
    // modify using regular skip list method
    Node* returnNode = SkipList::modify(key, data, 0, &s, oldLength, true);
    // change tag, recalculate rank values along the modify path
    returnNode->setTag(givenTag);
    calculateRank(s);
    
    return returnNode;
}

Node* RankBasedSkipList::insert(Key key, Data data, ZZ givenTag, stack<Node*>* s1, int givenLevel) {
#ifdef DEBUGRB
    char * log = new char[250];	sprintf(log,
                                        "%25s ( with key: %d %s %f %s %2d )"," RankBasedSkipList::insert", key.getValue(), "Data:", data.getValue(), "level: ",givenLevel);
    logger::log(log);
#endif
    stack<Node*> s;
    // call for regular skip list insert
    Node* returnNode = SkipList::insert(key, data, 0, &s, givenLevel);
    // change tag, recalculate rank and hash values along the insert path
    returnNode->setTag(givenTag);
    calculateHashAndRank(s);
    
#ifdef DETAILEDRB
    printLog();
#endif
    return returnNode;
}

Node* RankBasedSkipList::insertWOHashCalculation(Key key, Data data, ZZ givenTag, stack<Node*>* s1, int givenLevel) {
    
    stack<Node*> s;
    // call for regular skip list insert
    Node* returnNode = SkipList::insert(key, data, 0, &s, givenLevel,true);
    // change tag, recalculate rank values along the insert path
    returnNode->setTag(givenTag);
    calculateRank(s);
    
    return returnNode;
}

Node* RankBasedSkipList::remove(Key key, stack<Node*>* s1 ) {
#ifdef DEBUGRB
    char * log = new char[250];	sprintf(log,
                                        "%25s ( with key: %d )"," RankBasedSkipList::remove", key.getValue());
    logger::log(log);
#endif
    
    stack<Node*> s;
    //remove an element at index key, recalculate rank and hash values along the remove path
    Node* node = SkipList::remove(key, &s);
    calculateHashAndRank(s);
    
#ifdef DETAILEDRB
    printLog();
#endif
    
    return node;
}
Node* RankBasedSkipList::removeWOHashCalculation(Key key, stack<Node*>* s1 ) {
    
    stack<Node*> s;
    //remove an element at index key, recalculate rank values along the remove path
    Node* node = SkipList::remove(key, &s, true);
    calculateRank(s);
    
    return node;
}

void RankBasedSkipList::calculateHashAndRank(stack<Node*> s, int diff) {
#ifdef DETAILEDRB
    logger::log("calculateHashAndRank:");
#endif
    // input: stack s, filled with nodes along the path
    // output: recalculated rank and hash values at each node in stack s
    // diff is a number of bytes affected by the update, therefore
    // rank values of all nodes along the path should be updated
    while(!(s).empty())
    {
        Node *node = (s).top();
        node->calculateRank(diff);
        node->calculateHash(getModulus(), getElement());
        (s).pop();
#ifdef DETAILEDRB
        node->printLog();
#endif
    }
    root->calculateRank();
    root->calculateHash(getModulus(), getElement());
    
}
void RankBasedSkipList::calculateHash(stack<Node*> s) {
    
    // input: stack s, filled with nodes along the path
    // output: recalculated hash values at each node in stack s
    
    while(!(s).empty())
    {
        Node *node = (s).top();
        node->calculateHash(getModulus(), getElement());
        (s).pop();
    }
    root->calculateHash(getModulus(), getElement());
}

void RankBasedSkipList::calculateRank(stack<Node*> s, int diff) {
    
    // input: stack s, filled with nodes along the path
    // output: recalculated rank values at each node in stack s
    // diff is a number of bytes affected by the update, therefore
    // rank values of all nodes along the path should be updated
    while(!(s).empty())
    {
        Node *node = (s).top();
        //node->print();
        node->calculateRank(diff);
        (s).pop();
    }
    
    root->calculateRank();
}

bool RankBasedSkipList::createMissingNode(Node** previousNode, Node** currentNode, Key* key,
                                          int levelToInsert,  bool canGoDown, stack<Node*> *s) {
#ifdef DEBUGRB
    char * log = new char[250];	sprintf(log,
                                        "%25s ( with key: %d ) currentNodeKey: %d currentNodeData: %f canGoDown: %s" ," RankBasedSkipList::createMissingNode", key->getValue(), (*currentNode)->getKey().getValue(), (*currentNode)->getData().getValue(), canGoDown?"true":"false");
    logger::log(log);
#endif
    // call regular createMissingNode
    SkipList::createMissingNode (previousNode, currentNode, key, levelToInsert, canGoDown, s);
    // set key of missing node to MAX in order to force current node to go down
    (*currentNode)->setKey(Key(INT_MAX - 1));
    // in case if current node moved to after link then key value should be updated
    if((*previousNode)->getBelow() != *currentNode)
        key->setValue(key->getValue() -
                      ((*previousNode)->getKey().getValue() - (*currentNode)->getBelowKeyValue()));
    return true;
}

bool RankBasedSkipList::deleteNode(Node** previousNode, Node** currentNode, Key* key, stack<Node*>* s) {
#ifdef DEBUGRB
    char * log = new char[250];	sprintf(log,
                                        "%25s ( with key: %d ) currentNodeKey: %d currentNodeData: %f"," RankBasedSkipList::deleteNode", key->getValue(), (*currentNode)->getKey().getValue(), (*currentNode)->getData().getValue());
    logger::log(log);
#endif
    // call regular deleteNode
    SkipList::deleteNode(previousNode, currentNode, key, s);
    // in case if deleted node's after link is connected to previous node's below, then key value is updated
    if((*previousNode)->getBelow() != (*currentNode)->getBelow()){
        key->setValue(key->getValue() + (*previousNode)->getKey().getValue() - (*currentNode)->getKey().getValue());
    }
    // since we deleted a node, to continue we need to recalculate rank of previous node
    (( RankBasedNode* )(*previousNode))->calculateRank();
#ifdef DEBUGRB
    log = new char[250];	sprintf(log,
                                    "%25s ( with key: %d ) new currentNodeKey: %d new currentNodeData: %f"," After RankBasedSkipList::deleteNode", key->getValue(), (*currentNode)->getKey().getValue(), (*currentNode)->getData().getValue());
    logger::log(log);
#endif
    return true;
}

bool RankBasedSkipList::oneStep(Node** previousNode, Node** currentNode, Key* key,
                                int levelToInsert, bool npi, stack<Node*>* s, bool nullifyHashes) {
#ifdef DETAILEDRB
    char * log = new char[250];	sprintf(log,
                                        "%25s ( with key: %d ) currentNodeKey: %d currentNodeData: %f lvl2Insert: %d npi: %s"," RankBasedSkipList::oneStep", key->getValue(), (*currentNode)->getKey().getValue(), (*currentNode)->getData().getValue(), levelToInsert, npi?"true":"false");
    logger::log(log);
#endif
    Node* w =  (*currentNode)->getAfter();
    Node* z =  (*currentNode)->getBelow();
    
    if(z != NULL){
        // if the key value and levelToInsert(insertion level) allows to go down
        // npi is special boolean which allows to continue traverse at insert method
        if(((*currentNode))->canGoDown(*key, levelToInsert) && z->getLevel() >= levelToInsert && npi){
            return oneStepDo(previousNode, currentNode, z, s, nullifyHashes);
        }
    }
    if(w != NULL){
        // if the key value and levelToInsert(insertion level) allows to go right 
        if(((*currentNode))->canGoRight(*key) && w->getLevel() >= levelToInsert){
            key->setValue(key->getValue() - (*currentNode)->getBelowKeyValue());
            return oneStepDo(previousNode, currentNode, w, s, nullifyHashes);
        }
    }
#ifdef DETAILEDRB
    log = new char[250];	sprintf(log,
                                    "%25s ( with key: %d ) currentNodeKey: %d currentNodeData: %f"," After RankBasedSkipList::oneStep (NextPos done)", key->getValue(), (*currentNode)->getKey().getValue(), (*currentNode)->getData().getValue());
    logger::log(log);
#endif		
    return false;
}

// prints skip list
void RankBasedSkipList::printRB() {
    cout<<"/-------------------------------------------------\\"<<endl;
    printHelperRB((RankBasedNode*)root);
    cout<<"size :  "<< getSize() <<endl;
    cout<<"\\-------------------------------------------------/"<<endl;
}

// helper function to print skip list
void RankBasedSkipList::printHelperRB(RankBasedNode* node) {
    
    node->print();
    
    if(node->getAfter() != NULL) {
        printHelperRB((RankBasedNode*)node->getAfter());
    }
    if(node->getBelow() != NULL) {
        printHelperRB((RankBasedNode*)node->getBelow());
    }
}

char* RankBasedSkipList::rankBasedPrint() {
    cout<<"not implemented..."<<endl;
    /*use functino search with a loop*/
    /*temporarily it is used for lvl printing*/
    Node* maxLevelNode = root->getBelow();
    cout<<"Max level in the SL: "<<maxLevelNode->getLevel()<<endl;
}