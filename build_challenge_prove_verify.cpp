/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include"build_challenge_prove_verify.h"
#include"build_flexlist.h"

using namespace std;


bool build_challenge_prove_verify(string devnm, string member_device, bool are_flexlists_created)
{
    ZZ N, g;
    vector<ZZ> fileBlocks;
    int no_of_blocks;
    Node* root_of_flexlist;
    
    DPDPFlexVerifier* verifier = new DPDPFlexVerifier(&fileBlocks, &N, &g);
      
    if(!are_flexlists_created)
    {
        // Building FlexList :-
    
        no_of_blocks;
    
        root_of_flexlist = build_flexlist(devnm, member_device, N, g, verifier->getP(), verifier->getQ(), &no_of_blocks);
    
//    srand(DPDPCONST::Seed );

        // Writing metadata :-
        write_flexlist_root_hash_value(devnm, member_device, root_of_flexlist);
    
        // Serialize FlexList objects (i.e. store objects as files) :-
        //cout << "Serialization of list (storing object as file not yet implemented\n";
        
        //serialize_flexlist(devnm, member_device, root_of_flexlist);    
            // not implemented yet
    }
    
    else
    {
        // Generate FlexLists objects from stored files (De-serialization) :-
        cout << "Generating FlexLists objects from stored files - Not implemented yet\n";
            
        //root_of_flexlist = deserialize_flexlist(devnm, member_device);
            // not implemented yet
    }
    
    int proverSeed;
    
    DPDPFlexProver* prover = new DPDPFlexProver(NULL, N, g);    

    
    // Create challenge vector :-

    cout << "\nCreating challenge vector..\n";
        
    vector<int> challenge_vector;
    
    challenge_vector = verifier->generateChallenges(DPDPCONST::numOfChallenges /*default value = 460*/ , no_of_blocks, &proverSeed);
    // this same seed value should be used by DPDPFlexProver
    // No of blocks in FlexList should be here as 2nd argument !!
    
    int n = challenge_vector.size();
    
    cout << "Challenges size : " << n << endl << "Challenged indices :- \n";
    
    for(int j = 0; j < n; j++)
        cout << challenge_vector[j] << " ";
    cout << endl;
    
            
    // Generate proof vector :-
    
    cout << "\nGenerating proof for challenge.." << endl;
    
    vector<ZZ> tags, file_blocks;    
    
    vector<ProofNode> proof_vector;
    proof_vector = prover->GenMultiProof(root_of_flexlist, challenge_vector, &tags, &file_blocks);
    
    cout << "Prover tags length = " << tags.size() << endl;
    
  /*  cout << "Prover tags  : " << endl;
    
    for(int i = 0; i < tags.size(); i++)
    {    cout << tags[i] << endl;
        cout << endl;
    }*/
    
    cout << "Prover fileblocks length = " << file_blocks.size() << endl;
    
  /*  cout << "Prover fileblocks  : " << endl;
    
    for(int i = 0; i < file_blocks.size(); i++)
    {    cout << file_blocks[i] << endl;
        cout << endl;
    }*/
    
    ZZ blockSum = prover->genBlockSum(proverSeed, file_blocks, DPDPCONST::stat);
    
    cout << "\nBlockSum : " << blockSum << endl;
    
    delete prover;
    
    cout << "Proof generated \n";
    
    
    // Verify the proof vector :-
        
    cout << "\nVerifying proof vector..\n";
      
    bool result1, result2;
    
    
    // Read RootHashValue (client metadata) from file /etc/raid-dpdp/RootMetaData/<diskname> :-
    ZZ rootHashValue;
    
    // Commented code below was attempt to read stored root hash value (metadata) and set it as verifier's metadata.. For some reason, this value was incorrect.
    // But anyway, we have the list root pointer, so we can get root hash value from it directly.
 /*   ifstream file;
    long size;
    
    string filename = "/etc/raid-dpdp/RootMetaData/" + devnm + "/" + member_device;
    file.open(filename.c_str());
    
    file.seekg (0, ios::end);
    size = file.tellg();
    
    char *memblock = new char[size];
    file.seekg (0, ios::beg);
    file.read (memblock, size);
    file.close();
    
    ZZFromBytes(rootHashValue, (unsigned char*)memblock, size);
    delete[] memblock; */
   
    rootHashValue = root_of_flexlist->getHash();
    
    verifier->setMetaData(rootHashValue);
        
    cout << "rootHashValue set as verifier's metadata = " << rootHashValue << endl;
    
    result1 = verifier->verifyTags(tags, blockSum, NULL);
        
    result2 = verifier->VerifyMultiProof(challenge_vector, proof_vector, tags, blockSum);   //blockSum is not used inside VerifyMultiProof() though
    
    cout << "result1 = " << result1 << " .. result2 = " << result2 << endl;
            
    return (result1 && result2);        
}