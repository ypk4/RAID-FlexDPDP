#ifndef _Key_H_
#define _Key_H_

/*------------------------------------------------------------------------
 * Class:   Key.h
 *
 * Purpose:   Class for definition of keys in a skip list node
 *
 *------------------------------------------------------------------------
 */

class Key {
    
public:
    // constructors
    Key();
    Key(int k);
    
    int getValue();
    void setValue(int val);
    // comparison function of two keys
    // 0 (equal)
    // 1 this.key is larger
    // -1 this.key is smaller
    int compareTo(Key newKey);
    int compareTo(Key newKey, int lvl);
private:
    int value;
    
};

#endif /*_Key_H_*/