//
// Created by sasaki on 19/04/25.
//

#include "workload.h"

void WorkLoad::InputFile(std::string workload_file_name, int k_, int labelnum_)
{

    k=k_;
    labelnum=labelnum_;

    const char *workloadhfile=workload_file_name.c_str();
    std::ifstream inputwork(workloadhfile);

    if(!inputwork){
        std::cout<<"error: cannot open graph and/or workload files"<<std::endl;
        exit(1);
    }

    string line, label;
    //vector<int> labelsequence;
    //labelsequence.resize(k);
    //int labelidsize=labelnum;
    //for(int i=1;i<k;i++)labelidsize += pow(labelnum,i+1);

    //labelset.resize(labelidsize);
    //prefixlabelset.resize(labelidsize);
    //for(int i=0;i<labelidsize;i++){
     //   labelset[i]=false;
     //   prefixlabelset[i]=false;
    //}
    labelidhash.set_empty_key(-1);
    prefixlabelidhash.set_empty_key(-1);
    labelset_k.resize(k);
    for(int i=0;i<k;i++){
        labelset_k[i].resize(labelnum);
        for(int j=0;j<labelnum;j++)labelset_k[i][j]=false;
    }



    int labelcount=0;
    vector<int> given_labelsequence;
    vector<vector<int>> given_labelsequenceset;
    while(std::getline(inputwork,line)){
        std::stringstream ss;
        ss<<line;
        //cout<<ss.str()<<endl;
        labelcount=0;
        given_labelsequence.clear();
        while(ss >> label)
        {
            int size = min(labelcount+1,k);
            if(std::all_of(label.cbegin(),label.cend(), [](char ch) { return isdigit(ch);})){
		    given_labelsequence.push_back(stoi(label));
            }
        }
	    if(!given_labelsequence.empty())given_labelsequenceset.push_back(given_labelsequence);
    }


    for(auto ls : given_labelsequenceset){
        int labelcount=0;
        long int labelsequence=0;
        if(ls.size()==1)continue;
        for(int l : ls){
            labelset_k[labelcount][l]=1;

            if(labelcount==0)labelsequence = l;
            else labelsequence = (labelsequence+1)*labelnum + l;
                labelcount++;
                if(labelcount<ls.size())prefixlabelidhash[labelsequence]=1;
            }
        labelidhash[labelsequence]=1;
    }
    //	for(int i=0;i<prefixlabelset.size();i++){
    //		if(prefixlabelset[i])cout<<i<<endl;
    //	}
    //number_of_edges = edge_list.size();
}
