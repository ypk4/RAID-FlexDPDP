#ifndef _DummyRankBasedNode_H_
#define _DummyRankBasedNode_H_

#include "RankBasedNode.h"

/*------------------------------------------------------------------------
 * Class:   DummyRankBasedNode.h
 *
 * Purpose:   for rebuilding the skip list out of a proof vector
 *  	      
 *
 *------------------------------------------------------------------------
 */

class DummyRankBasedNode : public RankBasedNode {

	public:
		DummyRankBasedNode(Key k=0, int lvl=0)
			: RankBasedNode(k, lvl){}

		
		
		virtual void calculateHash(ZZ modulus, ZZ element) {
			/*DO NOTHING*/
			//As this node is dummy, it should just return the value provided in the begining since it is not meant to be changed
		}
		void calculateRank(int diff = 1){
			/*DO NOTHING*/
			//As this node is dummy, it should just return the value provided in the begining since it is not meant to be changed
		}

	private:		
	
		friend class RankBasedSkipList;
		friend class AuthenticatedSkipList;
};

#endif /*_DummyRankBasedNode_H_*/