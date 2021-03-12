//
// Created by sasaki on 19/04/25.
//

#ifndef WORKLOADAWARE_K_BISIMULATION_INDEX_WORKLOAD_H
#define WORKLOADAWARE_K_BISIMULATION_INDEX_WORKLOAD_H

#include "utility.h"

class WorkLoad {

public:


    int k;
    int labelnum;

    google::dense_hash_map<long int, bool> givenworkload;
    google::dense_hash_map<long int, bool> prefixgivenworkload;
    vector<vector<bool>>kth_labels_workload;

	WorkLoad(){
    }

    void InputFile(std::string,int k, int labelnum);
};

#endif //WORKLOADAWARE_K_BISIMULATION_INDEX_WORKLOAD_H
