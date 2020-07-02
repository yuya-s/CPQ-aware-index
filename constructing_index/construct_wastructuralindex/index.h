

#ifndef K_BISIMULATION_INDEX_INDEX_H_H
#define K_BISIMULATION_INDEX_INDEX_H_H

#include "utility.h"
#include "workloadaware_transequivalent.h"



struct vectoreqr {
	inline bool operator()(const vector<int>& s1, const vector<int>& s2) const {
		for(int i=0;i<s1.size();i++) {
            if (s1[i] != s2[i])return false;
        }
		return true;
	}
};


struct vectorhash {
	inline size_t operator()(const vector<int>& s1) const {
        long int hashvalue=17;
        for(int i=0;i<s1.size();i++) {
            hashvalue += s1[i];
            hashvalue *= 31;
        }
		return hashvalue;
	}
};




class WorkloadawareTransequivalentIndex{

public:
    int k;
    int labelnum; //original label num * 2;

    vector< vector< Path >> history2path;
    vector< vector< int >> label2history;
    google::dense_hash_map<long int, int> hash_labelid2index;
    google::dense_hash_map<int, long int> hash_index2labelid;
    WorkloadawareTransequivalentIndex(WorkloadawareTransequivalent&);
    void Output(string output_filename);
};



#endif //K_BISIMULATION_INDEX_INDEX_H_H
