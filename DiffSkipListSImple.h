#ifndef _DiffSkipListSimple_H_
#define _DiffSkipListSimple_H_

#include "NTL/ZZ.h"
#include "Data.h"

/*------------------------------------------------------------------------
 * Program:   	DiffSkipListSimple
 *
 * Purpose:   	This class is an information package for an update operation
 *				
 *
 *
 * Note:      	Especially used for a list of updates to be sent to multiple update operations
 *
 *------------------------------------------------------------------------
 */

class DiffSkipListSimple {
	
public:

	enum TYPE { MODIFY = 1, INSERT, REMOVE};
	
	DiffSkipListSimple() {	}

	DiffSkipListSimple(TYPE t, int idx, Data *d=NULL, ZZ tag=NULL) {
		type = t;
		index = idx;
		data = d;
		tagOfData = tag;
	}
	
	TYPE getType() {return type;}
	int getIndex() {return index;}
	Data* getData(){return data;}
	ZZ getTag()    {return tagOfData;}
	
private: 

	TYPE type;			//Type of the block operation
	int index;			//To which block this will get applied
	Data* data;	//The data if this is an insert operation or a modify operation
	ZZ tagOfData;		//The tag previously calculated by the client
	

};

#endif /*_DiffSkipListSimple_H_*/