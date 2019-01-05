#ifndef _DPDPCONST_H_
#define _DPDPCONST_H_

#include "Hash.h"

/*------------------------------------------------------------------------
 * Class:   DPDPCONST.h
 *
 * Purpose:   This file defines DEFAULT values for different test cases
 *	      
 *------------------------------------------------------------------------
 */

class DPDPCONST {

public: 
	// value for creation of Group
	static const int stat = 80;
	// hash function type is defined SHA1
	static const Hash::alg_t hashAlg = Hash::SHA1;
	// range for seed choice 	
	static const int seedRange = 10000;
	// seed value 
	static const int Seed = 2743;
	// block size	
	static const int blockSize = 16384 / 8; // 16Kib / 8 = 2kb (2048 bytes)
	// number of challenges 	
	static const int numOfChallenges = 460;
	// number of blocks in a skip list	
	static const int numOfBlocks = 489*2;
	// size of a file	
	static const int fileSize = 1000000; //NUM OF CHARACTERS IN THE FILE, DEPENDS ON NUMOFBLOCKS AND DEFAULT BLOCK LENGTH ATM
	// not yet used
	static  ZZ fblock;
	// block length (flexlist)	
	static const int defaultBlockLength = 16384/8; 
	// minimum length of a block (flexlist)	
	static const int minBlockLength = 16384 / (2 * 8); 
	// maximum length of a block (flexlist)	
	static const int maxBlockLength = 16384 * 2 / 8; 
};

#endif /* _DPDPCONST_H_ */