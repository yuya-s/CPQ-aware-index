


#ifndef K_BISIMULATION_KBISIMULATION_H
#define K_BISIMULATION_KBISIMULATION_H


#include "utility.h"
#include "graph.h"
#include "workload.h"
struct Path {// for google hats map
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
    //int segid;
    int order;
    set<long int> labelidset;

    void clear(){
    	labelidset.clear();
    }
    bool operator==(const Segment& s){return (path.src == s.path.src&&path.dst==s.path.dst&&labelidset == s.labelidset);}
};



class PathSet {

public:
	int k; //given parameters. keep k length paths.
    int number_of_labels; //original label num * 2;
	int number_of_vertices; // number of vertices
    int number_of_edges;


    vector<vector<Segment>> seg; //[length]->segment

	PathSet(int k_, OriginalGraph graph_, WorkLoad workload_);
	//void Output(string output_filename, OriginalGraph graph_);

};

inline bool pathcmp(const Path &p1, const Path &p2){
	if(p1.src>p2.src)return true;
	if(p1.src==p2.src&&p1.dst>=p2.dst)return true;
	return false;
}

inline bool paircmp(const pair<int,int> &p1, const pair<int,int> &p2){
	if(p1.first<p2.first)return true;
	if(p1.first==p2.first&&p1.second<=p2.second)return true;
	return false;
}

inline bool segmentcmp(const Segment &s1, const Segment &s2){
  	if(s1.path.src<s2.path.src)return true;
    else if(s1.path.src==s2.path.src&&s1.path.dst<s2.path.dst)return true;
  	else if(s1.path.src==s2.path.src&&s1.path.dst==s2.path.dst&&s1.labelidset<s2.labelidset)return true;
//  	if(s1.path.src==s2.path.src&&s1.path.dst==s2.path.dst&&s1.labelidset==s2.labelidset&&s1.order<s2.order)return true;
	return false;
}

inline bool segmentcmppointer(const Segment *s1, const Segment *s2){
		//cout<<s1->labelidset.size()<<"vs"<<s2->labelidset.size()<<","<<s1->path.src<<"->"<< s1->path.dst<<","<<s2->path.src<<"->"<<s2->path.dst<<endl;
	if(s1->labelidset.size() == s2->labelidset.size()){
		if(s1->labelidset.size()==0)cout<<"ohno"<<endl;
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



inline bool cmpsrc(const Segment *a, const Segment *b)
{
	if(a->path.src==b->path.src){
		return a->path.dst< b->path.dst;
	}
    else return a->path.src < b->path.src;
}

inline bool cmpdst(const Segment &a, const Segment &b)
{
	if(a.path.dst==b.path.dst){
		return a.path.src< b.path.src;
	}
    else return a.path.dst < b.path.dst;
}


inline Path MinPath(vector<Segment*> &segvector){

    Path p = segvector[0]->path;

    for(int i=1;i < segvector.size();i++){
        if(p.src > segvector[i]->path.src){
            p = segvector[i]->path;
        }
        else if(p.src==segvector[i]->path.src&&p.dst > segvector[i]->path.dst){
            p=segvector[i]->path;
        }
    }
    return p;
}

inline vector<int> decodeLabel(long int labelid, int labelnum, int k){

    vector<int> labelsequence;
    int k_value=k;

    while(1){
        if(labelid/(int)pow(labelnum,k_value)>0) {
            labelsequence.push_back(labelid % (int) pow(labelnum, k_value)-1);
            labelid = labelid/(int)pow(labelnum,k_value);
        }
        k_value--;
        if(k_value==0){
            labelsequence.push_back(labelid);
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
