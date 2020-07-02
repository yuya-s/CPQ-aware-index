

#ifndef K_BISIMULATION_KBISIMULATION_H
#define K_BISIMULATION_KBISIMULATION_H


#include "utility.h"
#include "graph.h"
#include "workload.h"
struct Path {// for google hash map
	int src;
	int dst;
	bool operator==(const Path& p){return (src == p.src&&dst == p.dst);}
	bool operator!=(const Path& p){return (src != p.src||dst != p.dst);}
    bool operator<(const Path& p){return (src < p.src||(src == p.src&&dst < p.dst));}
    bool operator>(const Path& p){return (src > p.src||(src == p.src&&dst > p.dst));}
};


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



struct Segment {
public:
    Path path;
    int order;
    set<long int> labelidset;
    void clear(){
    	labelidset.clear();
    }
    bool operator==(const Segment& s){return (path.src == s.path.src&&path.dst==s.path.dst&&labelidset == s.labelidset);}
};



class WorkloadawareTransequivalent {

public:
	int k; //given parameters. keep 2^k length paths.
    int number_of_labels; //original label num * 2;
	int number_of_vertices;
    int number_of_edges;

    vector<vector<Segment>> seg; //[length]->segment


	WorkloadawareTransequivalent(int k_, OriginalGraph& graph_, WorkLoad& workload_);
	//void Output(string output_filename, OriginalGraph graph_);

};


inline bool segmentcmppointer(const Segment *s1, const Segment *s2){
	if(s1->labelidset.size() == s2->labelidset.size()){
		if(s1->labelidset.size()==0)cout<<"bug"<<endl;
		auto itrs1 = s1->labelidset.begin();
		auto itrs2 = s2->labelidset.begin();
		for(int i=0;i<s1->labelidset.size();i++){
			if(*itrs1 != *itrs2)return *itrs1 < *itrs2;//return true;
			itrs1++;itrs2++;
		}
		if(s1->path.src == s1->path.dst&&s2->path.src != s2->path.dst){
			return true;
		}
		else if(s1->path.src != s1->path.dst&&s2->path.src == s2->path.dst){
			return false;
		}
		else if(s1->path.src == s1->path.dst&&s2->path.src == s2->path.dst){
			return s1->path.src < s2->path.src;
		}
		else{
			if(s1->path.src == s2->path.src){
				return s1->path.dst < s2->path.dst;
			}
			else{ 
				return s1->path.src < s2->path.src;
			}
		}
	}
	else return s1->labelidset.size()<s2->labelidset.size();
}	


inline vector<int> decodeLabel(long int labelid, int labelnum, int k){

    vector<int> labelsequence;
    int k_value=k;

    while(1){
        if(labelid/(int)pow(labelnum,k_value)>0) {
            labelsequence.push_back((int)(labelid % (int) pow(labelnum, k_value)-1));
            labelid = labelid/(int)pow(labelnum,k_value);
        }
        k_value--;
        if(k_value==0){
            labelsequence.push_back((int)labelid);
            break;
        }
    }
    return labelsequence;
}

inline long int encodeLabel(vector<int>& labelsequence, int labelnum, int k){

    long int labelid=0;
    for(int i=0;i<labelsequence.size();i++){

        if(i==0)labelid += labelsequence[i];
        else{
            labelid += (labelsequence[i]+1)*labelnum;
        }
    }
    return labelid;
}

#endif //K_BISIMULATION_KBISIMULATION_H
