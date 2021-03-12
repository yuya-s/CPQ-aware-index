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

    string line, string_label;

    givenworkload.set_empty_key(-1);
    prefixgivenworkload.set_empty_key(-1);
    kth_labels_workload.resize(k);
    for(int i=0;i<k;i++){
        kth_labels_workload[i].resize(labelnum);
        for(int j=0;j<labelnum;j++)kth_labels_workload[i][j]=false;
    }



    int labelcount=0;
    int previouslabel;
    vector<int> given_labelsequence;
    vector<vector<int>> given_labelsequenceset;
    while(std::getline(inputwork,line)){
//        cout<<line<<endl;
        bool queryDoneFlag=false;
        bool labelFlag=false;
        std::stringstream ss;
        ss<<line;
        //cout<<ss.str()<<endl;
        labelcount=0;
        given_labelsequence.clear();
        //ss.ignore(line.size(), ' ');
        while(ss >> string_label)
        {
            int size = min(labelcount+1,k);
            if(std::all_of(string_label.cbegin(),string_label.cend(), [](char ch) { return isdigit(ch);})){
		given_labelsequence.push_back(stoi(string_label));
	    }
	}
	if(!given_labelsequence.empty())given_labelsequenceset.push_back(given_labelsequence);
    }

                
    for(auto labelsequence : given_labelsequenceset){
        int labelcount=0;
        long int labelid=0;

        if(labelsequence.size()==1)continue;
        for(int label : labelsequence){
            kth_labels_workload[labelcount][label]=true;

            if(labelcount==0)labelid = label;
            else labelid = (labelid+1)*labelnum + label;
                labelcount++;
            if(labelcount<labelsequence.size())prefixgivenworkload[labelid]=true;
        }
        givenworkload[labelid]=true;
     }

}
