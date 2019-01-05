/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "Key.h"


Key::Key(){}

Key::Key(int k) {
    value = k;
}

int Key::getValue() {
    return value;
}

void Key::setValue(int val) {
    value = val;
}
// comparison function of two keys
// 0 (equal)
// 1 this.key is larger
// -1 this.key is smaller
int Key::compareTo(Key newKey) {
    if(value < newKey.getValue()) return -1;
    else if(value > newKey.getValue()) return 1;
    return 0;
}

int Key::compareTo(Key newKey, int lvl) {
    if(value < newKey.getValue() + ((lvl > -1)?1:0)) return -1;
    else if(value > newKey.getValue() + ((lvl > -1)?1:0)) return 1;
    return 0;
}