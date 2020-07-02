
#ifndef QUERY_PATHINDEX_INDEX_H
#define QUERY_PATHINDEX_INDEX_H
#include "utility.h"

class PathIndex{

public:
    int k;
    int labelnum; //original label num * 2;
    bool workloadindex;

    vector< vector< pair<int,int> > > label2path;
    google::dense_hash_map<long int, int> hash_labelid2index;
    google::dense_hash_map<long int, int> givenindexworkload;

    PathIndex(std::string);
    void SetWorkload(std::string);

};

inline int encodeLabel(vector<int>& labellist, int labelnum, int k){

    int labelsequence=0;
    for(int i=0;i<labellist.size();i++){

        if(i==0)labelsequence += labellist[i];
        else{
            labelsequence = (labelsequence+1)*labelnum+labellist[i];
        }
    }
    return labelsequence;
}
#endif //QUERY_PATHINDEX_INDEX_H
