

#ifndef QUERY_BISIMULATION_INDEX_H
#define QUERY_BISIMULATION_INDEX_H

#include "utility.h"
#include "graph.h"

struct Path {// for google hatsh map
	int src;
	int dst;
	bool operator==(const Path& p){return (src == p.src&&dst == p.dst);}
	bool operator!=(const Path& p){return (src != p.src||dst != p.dst);}

};
struct PathwithIDs {
public:
    Path path;

    set<long int> labelidset;
    set<int> segvalue;
    bool operator==(const PathwithIDs& s){return (path.src == s.path.src&&path.dst==s.path.dst&&segvalue == s.segvalue&&labelidset == s.labelidset);}
    bool operator==(const PathwithIDs* s){return (path.src == s->path.src&&path.dst==s->path.dst&&segvalue == s->segvalue&&labelidset == s->labelidset);}
    void clear(){
    	labelidset.clear();
        segvalue.clear();
    }
};


struct Segment {
public:
    Path path;
    set<long int> labelidset;

    void clear(){
    	labelidset.clear();
    }
};

struct PathHistory {
public:
    Path path;
    int history;

};


class BisimulationIndex{

public:
    int k;
    int labelnum; //original label num * 2;
    bool workloadindex;

    vector< vector<pair<int,int>>> history2path;
    vector< vector< int >> label2history;

    google::dense_hash_map<string, int> path2history;
    google::dense_hash_map<long int, int> hash_labelid2index;
    google::dense_hash_map<long int, int> hash_index2labelid;
    google::dense_hash_map<long int, bool> givenindexworkload;

    BisimulationIndex(std::string);
    void Output(string output_filename);
    void DeleteEdge(Edge, OriginalGraph&);
    void AddEdge(Edge, OriginalGraph&);
    void DeleteWorkload(vector<int>, OriginalGraph&);
    void FindHistory(int, set<int>);
    void AddWorkload(vector<int>, OriginalGraph&);
    void SetWorkload(std::string);
};

inline long int encodeLabel(vector<int>& labelsequence, int labelnum, int k){

    long int labelid=0;
    for(int i=0;i<labelsequence.size();i++){

        if(i==0)labelid += labelsequence[i];
        else{
            labelid = (labelid+1)*labelnum+labelsequence[i];
        }
    }
    return labelid;
}

inline vector<int> decodeLabel(long int labelid, int labelnum, int k){

    vector<int> labelsequence;
    int k_value=1;

    while(1){
        labelsequence.push_back(labelid % (int) pow(labelnum, k_value));
        if(labelid < (int) pow(labelnum, k_value))return labelsequence;
        labelid = labelid/(int)pow(labelnum,k_value) -1;

        if(k_value==k){
            break;
        }
         k_value++;
    }
    return labelsequence;
}

struct patheqr {
	inline bool operator()(const Path& s1, const Path& s2) const {
		if (s1.src == s2.src && s1.dst == s2.dst)return true;
		else return false;
	}
};


struct pathhash {
	inline size_t operator()(const Path& s1) const {
		return ((17+s1.src)*31+s1.dst)*31;
	}
};




inline bool cmppathhistorypath(const PathHistory &p1, const PathHistory &p2){
	if(p1.path.src<p2.path.src)return true;
	if(p1.path.src==p2.path.src&&p1.path.dst<=p2.path.dst)return true;
	return false;
}

inline bool cmpsegmentpath(const Segment &p1, const Segment &p2){
	if(p1.path.src<p2.path.src)return true;
	if(p1.path.src==p2.path.src&&p1.path.dst<=p2.path.dst)return true;
	return false;
}



#endif //QUERY_BISIMULATION_INDEX_H
