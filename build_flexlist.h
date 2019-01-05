/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   build_flexlist.h
 * Author: yogiraj
 *
 * Created on July 5, 2017, 7:14 PM
 */

#ifndef BUILD_FLEXLIST_H
#define BUILD_FLEXLIST_H

#include"Node.h"

bool check_flexlist_created(string devnm);

Node* build_flexlist(string devnm, string diskname, ZZ N, ZZ g, ZZ p, ZZ q, int *pno_of_blocks);

//vector<Node *> build_flexlists_for_disks(string devnm, vector<string> member_devices);

void write_flexlist_root_hash_value(string devnm, string member_device, Node * root);

#endif /* BUILD_FLEXLIST_H */