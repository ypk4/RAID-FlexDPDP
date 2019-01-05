/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "RankBasedNode.h"

RankBasedNode::RankBasedNode(Key k, int lvl)
: AuthenticatedNode(k, lvl){ }

bool RankBasedNode::canGoDown( Key key, int levelToInsert ) {
    // chechks whether the key given is at the below link
    return  key.getValue() < getBelowKeyValue() + ((levelToInsert > -1)?1:0);
}

bool RankBasedNode::canGoRight(Key key) {
    // checks whether the after link leads to the searched key
    if(key.getValue() > getBelowKeyValue()) {
        if( getAfter() != NULL && getAfter()->getData().length > key.getValue() - getBelowKeyValue())	//flexlist
            return false;
        return true;
    }
    return false;
}

bool RankBasedNode::canGoRightNoStop(Key key) {
    // specially implemented for isIntersecting function in searchMulti algorithm
    // checks if the current key value can be found by following after link
    if(key.getValue() >= getBelowKeyValue()) {
        return true;
    }
    return false;
}

void RankBasedNode::calculateRank(int diff) {
    if(getLevel() == 0)
    {
        Node* after = getAfter();
        if(getAfter() != NULL)
            setKey( Key(getData().getLength() + after->getKey().getValue()) );
        else
            setKey( Key(getData().getLength() ) );
    }else
    {
        setKey( Key(getAfterKeyValue() + getBelowKeyValue()) );
    }
}

void RankBasedNode::print() {
    cout<<"---"<<endl; AuthenticatedNode::print(); cout<<"hash after: "<<AuthenticatedNode::getAfterHash()<<endl;cout<<"hash below: "<<AuthenticatedNode::getBelowHash()<<endl;
}

bool RankBasedNode::isIntersectionOfTwo(bool canGoDown, int r1, int r2) {
    if( r1 == r2 ) return false;
    return (canGoDown && canGoRightNoStop(r2));
}

bool RankBasedNode::isIntersecting(bool canGoDown, vector<int> chalIndex, int startKey, int* end, int rankState) {
    if(chalIndex[*end] == rankState && chalIndex[0] != -1) return false;
    if(chalIndex.size() == 1) return false;
    int tempEnd = *end;
    // finds the smallest closest challenged index to the current index at common intersection point
    while(isIntersectionOfTwo(canGoDown, startKey ,chalIndex[*end] - rankState)) {
        *end = *end - 1;
    }
    // if end value has changed then there is an intersection point
    return *end != tempEnd;
}