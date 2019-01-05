/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "Node.h"
#include "logger.h"
#include <stdlib.h>
#include <string>
#include <iostream>
#include "Data.h"
#include "Key.h"
#include "Hash.h"
#include "DPDPCONST.h"

Node::Node(Key k, int lvl)
:key(k), level(lvl), data(to_ZZ(1)) {
    after = NULL; below = NULL; leafAfter = NULL;
}

Node::~Node(){}

// upon deletion of a skip list this function is called
void Node::freeAll(){
    if(below != NULL)
        below->freeAll();
    if(after != NULL)
        after->freeAll();
    delete  this;
}

// set and get functions data in the node
Data Node::getData(){
    if ( level != 0 )	return getBelow()->getData();
    else return data;
}

void Node::setData(Data d) { data = d; }

// level
int Node::getLevel() { return level; }

void Node::setLevel(int lvl) { level = lvl; }

// key
Key Node::getKey() { return key; }

void Node::setKey(Key k) { key = k; }

// after link
Node* Node::getAfter() { return after; }

void Node::setAfter(Node* n) { after = n; }

// Leaf level connection
Node* Node::getLeafAfter() {
    if(getLevel() == 0)
        return leafAfter;
    else
    {
        cout<<"ERROR: Not in leaf level but getLeafAfter called!"<<endl;
    }
    return NULL;
}
void Node::setLeafAfter(Node* n) { leafAfter = n; }

// below link
Node* Node::getBelow() { return below; }

void Node::setBelow(Node* n) { below = n; }

// getting key values from its neighbor (after or below)
int Node::getAfterKeyValue() {
    return (getAfter() != NULL)
    ?getAfter()->getKey().getValue()
    :0;
}
int Node::getBelowKeyValue() { return (getBelow() != NULL)
    ?getBelow()->getKey().getValue()
    :getData().length/*1*/;
}//flexlist

// outputs whether the given key can be found by following below link or not
bool Node::canGoDown( Key key, int levelToInsert ) {
    if( getAfter() == NULL ) return true;
    return (key.compareTo(getAfter()->getKey(), levelToInsert) < 0);
}
// outputs whether the given key can be found by following after link or not
bool Node::canGoRight(Key key) {
    if( getAfter() == NULL ) return false;
    return (key.compareTo(getAfter()->getKey()) > 0);
}

void Node::print() {
    cout<< "Node >> key: " << key.getValue() << " level: " << level << endl;
}

char* Node::printString() {
    char* nodeString = new char[250];
    sprintf(nodeString,"Node >> key: %d level: %d ",key.getValue(),level);
    return nodeString;
}

void Node::printLog() {
    logger::log(printString());
}