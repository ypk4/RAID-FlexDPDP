#ifndef _RankBasedNode_H_
#define _RankBasedNode_H_

#include "AuthenticatedNode.h"

/*------------------------------------------------------------------------
 * Class:   RankBasedNode.h and RankBasedNode.cpp
 *
 * Purpose:   definition and implementation of main methods of rank
 *  	      based nodes
 *
 *------------------------------------------------------------------------
 */

class RankBasedNode : public AuthenticatedNode {
    
public:
    
    RankBasedNode(Key k = 0, int lvl = 0);
    bool canGoRight(Key key);
    bool canGoRightNoStop(Key key);
    bool canGoDown(Key key, int levelToInsert);
    virtual void print();		// checks whether given two values are different
    // first one is found by following below link and second one by following the after link
    virtual bool isIntersectionOfTwo(bool canGoDown, int r1, int r2);
    virtual bool isIntersecting(bool canGoDown, vector<int> chalIndex, int startKey, int* end, int rankState);
    virtual void calculateRank(int diff = 1);
    
protected:
    
private:
				
    friend class RankBasedSkipList;
    friend class AuthenticatedSkipList;
};

#endif /*_RankBasedNode_H_*/