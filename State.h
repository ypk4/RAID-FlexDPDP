#ifndef _State_H_
#define _State_H_

#include "Node.h"

class State {
	public:
		State(Node n, int end=0, int rank=0)
			:node(n), endIndex(end), rankState(rank) { }
		Node getNode() { return node; }
		int getEndIndex() { return endIndex; }
		int getRankState() { return rankState; }
		void setRankState(int rank) { rankState = rank; }
	private:
		Node node;
		int endIndex;
		int rankState;
		friend class RankBasedSkipList;
		friend class AuthenticatedSkipList;
};

#endif /*_State_H_*/