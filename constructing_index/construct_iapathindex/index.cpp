//
// Created by sasaki on 19/03/11.
//
#include "index.h"
#include "pathset.h"

PathIndex::PathIndex(PathSet& k_pathset){

    struct timespec index_startTime, index_endTime;
    long double responseTime;

    k=k_pathset.k;
    labelnum=k_pathset.number_of_labels;

    hash_labelid2index.set_empty_key(-1);
    hash_index2labelid.set_empty_key(-1);

    clock_gettime(CLOCK_REALTIME, &index_startTime);

    for (auto segment : k_pathset.seg[0]) {
        pair<int,int> path = make_pair(segment.path.src,segment.path.dst);
        for (auto labelid : segment.labelidset) {

             if(hash_labelid2index[labelid]==0){
                    vector<pair<int,int>> temppathset;
                    temppathset.clear();
                    temppathset.push_back(path);
                    label2path.push_back(temppathset);
                    hash_labelid2index[labelid]=label2path.size();
                    hash_index2labelid[label2path.size()]=labelid;
                }
            else {label2path[hash_labelid2index[labelid]-1].push_back(path);}
        }
    }


    for(int i=0;i<label2path.size();i++) {
        sort(label2path[i].begin(), label2path[i].end(),paircmp);
    }

    clock_gettime(CLOCK_REALTIME, &index_endTime);
    responseTime = (index_endTime.tv_sec - index_startTime.tv_sec) + (index_endTime.tv_nsec - index_startTime.tv_nsec) * pow(10, -9);
    cout<<responseTime<<endl;

}

void PathIndex::Output(string output_filename_){


    string outputpath = "./index/"+output_filename_+"_path";
    std::ofstream fout(outputpath, ios::app);

    cout<<"label2path"<<endl;

    fout<<k<<" "<<labelnum<<endl;
    for(int i=0; i<label2path.size();i++) {

        if(label2path[i].empty())continue;

        fout <<hash_index2labelid[i+1]<<" ";
        for(auto path : label2path[i]) {
            fout<<path.first<<" "<<path.second<<" ";
        }
        fout<<endl;
     }
}
