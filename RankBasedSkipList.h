#ifndef _RankBasedSkipList_H_
#define _RankBasedSkipList_H_

#include "AuthenticatedSkipList.h"


/*------------------------------------------------------------------------
 * Class:   RankBasedSkipList.h and RankBasedSkipList.cpp
 *
 * Purpose:   Definition and implementation of essential methods for
 *	      rank-based skip list
 *
 * Syntax:
 *               modulus  - composite number N of Group ZN*
 *               element  - generator of Group ZN*
 *
 *------------------------------------------------------------------------
 */

class RankBasedSkipList : public AuthenticatedSkipList {
    
public:
    // constructor for authenticated rank based skip list
    RankBasedSkipList(ZZ compositeN = 1, ZZ randomG = 1);
    // constructor for authenticated flex list
    RankBasedSkipList(bool isFlexList, ZZ compositeN = 1, ZZ randomG = 1);
    RankBasedSkipList(bool n, bool isFlexList, ZZ compositeN = 1, ZZ randomG = 1);
    /*RankBasedSkipList(bool n, ZZ compositeN = 1, ZZ randomG = 1);
     }*/
    void init();
    virtual void multiUpdate(vector<DiffSkipListSimple*> diffs);
    virtual Node* modify(Key key, Data data, ZZ givenTag = 0, stack<Node*>* s1 = NULL);
    Node* modifyWOHashCalculation(Key key, Data data, ZZ givenTag = 0, stack<Node*>* s1 = NULL, int* oldLength = NULL);
    virtual Node* insert(Key key, Data data, ZZ givenTag = 0, stack<Node*>* s1 = NULL, int givenLevel = -1);
    Node* insertWOHashCalculation(Key key, Data data, ZZ givenTag = 0, stack<Node*>* s1 = NULL, int givenLevel = -1);
    // inserts a new node at the end of the skip list
    virtual Node* append(Data data, ZZ givenTag = 0, stack<Node*>* s1 = NULL) {return insert(Key(getSize()+1), data, givenTag, s1, -1);}
    virtual Node* remove(Key key, stack<Node*>* s1 = NULL);
    Node* removeWOHashCalculation(Key key, stack<Node*>* s1 = NULL);
    void calculateMultiHash(vector<int> ChangedNodes);
    void hashMulti(Node* currentNode, vector<int> chalIndex, int start, int end, int rankState, ZZ N, ZZ g, vector<Node*>* proofVector, stack<State*>* interStack);
    
    Node* parallelBuild(vector<Data> *datas, vector<int> *Levels, vector<ZZ> *tags, int max, int coreCount = 2);
    //void workerFunc(vector<Data> *datas, vector<int> *Levels, vector<ZZ> *tags, int max, int order, int coreCout, vector<RankBasedNode*> *roots/*, vector<RankBasedSkipList*> *sls*/);
    
    // prints skip list
    void printRB();
    // helper function to print skip list
    void printHelperRB(RankBasedNode* node);
    
    char* rankBasedPrint();
    
    
protected:
    // couple of methods, which required modifications at rank based skip list
    bool oneStep(Node** previousNode, Node** currentNode, Key* key,
                 int levelToInsert =-1, bool npi = false, stack<Node*>* s = NULL, bool nullifyHashes = false);
    bool createMissingNode(Node** previousNode, Node** currentNode, Key* key,
                           int levelToInsert,  bool canGoDown, stack<Node*> *s = NULL);
    bool deleteNode(Node** previousNode, Node** currentNode, Key* key, stack<Node*>* s = NULL);
    
    virtual void calculateHashAndRank(stack<Node*> s, int diff = 0);
    void calculateHash(stack<Node*> s);
    void calculateRank(stack<Node*> s, int diff = 0);
    virtual ZZ getModulus() {return AuthenticatedSkipList::getModulus();}
    virtual ZZ getElement() {return AuthenticatedSkipList::getElement();}
};

class Worker
{
public:
    
    Worker(vector<Data> d, vector<int> l, vector<ZZ> t, int m, int o = 0, int c = 1)
    {
        datas = d;
        Levels = l;
        tags = t;
        max = m;
        order = o;
        coreCout = c;
    }
    
    void start(ZZ N, ZZ g, vector<RankBasedNode*> *r, int arrayIndex = -1)
    {
        m_Thread = boost::thread(&Worker::processQueue, this, N, g, r, arrayIndex);
    }
    
    void join()
    {
        m_Thread.join();
    }
    
    void processQueue(ZZ N, ZZ g, vector<RankBasedNode*> *roots, int arrayIndex = -1)
    {
        RankBasedSkipList* aPartOfRbsl = new RankBasedSkipList(true, N, g);
        aPartOfRbsl->build(datas, Levels, tags, max, order, coreCout);
        //test
        //(*sls)[order] = aPartOfRbsl;
        /////
        (*roots)[(arrayIndex == -1)?order:arrayIndex] = (RankBasedNode*)aPartOfRbsl->getRoot();
    }
    
private:
    
    boost::thread m_Thread;
    vector<Data> datas;
    vector<int> Levels;
    vector<ZZ> tags;
    int max;
    int order;
    int coreCout;
    
};

#endif /*_RankBasedSkipList_H_*/