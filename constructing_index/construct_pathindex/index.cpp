
#include "index.h"
#include "pathset.h"

PathIndex::PathIndex(PathSet& k_pathset){

    struct timespec index_startTime, index_endTime;
    long double responseTime;

    k=k_pathset.k;
    labelnum=k_pathset.number_of_labels;

    hash_labelid2index.set_empty_key(-1);
    hash_index2labelid.set_empty_key(-1);
    //clock_gettime(CLOCK_REALTIME, &index_startTime);

    //computing label to path
    for(int for_k=0;for_k < k;for_k++) {
        for (auto segment : k_pathset.seg[for_k]) {
            pair<int,int> path = make_pair(segment.path.src,segment.path.dst);
            for (auto label : segment.labelsequence) {
                 if(hash_labelid2index[label]==0){
                    vector<pair<int,int>> temppathset;
                    temppathset.clear();
                    temppathset.push_back(path);
                    label2path.push_back(temppathset);
                    hash_labelid2index[label]=label2path.size();
                    hash_index2labelid[label2path.size()]=label;
                }
            else {label2path[hash_labelid2index[label]-1].push_back(path);}

            }
        }
    }
    for(int i=0;i<label2path.size();i++) {
        sort(label2path[i].begin(), label2path[i].end(),paircmp);
    }

    //clock_gettime(CLOCK_REALTIME, &index_endTime);
    //responseTime = (index_endTime.tv_sec - index_startTime.tv_sec) + (index_endTime.tv_nsec - index_startTime.tv_nsec) * pow(10, -9);
    //cout<<responseTime<<endl;
}

void PathIndex::Output(string output_filename_){

    string outputpath = "./index/"+output_filename_+"_path";
    std::ofstream fout(outputpath, ios::app);

    cout<<"Output label2path"<<endl;

    fout<<k<<" "<<labelnum<<endl;
    for(int i=0; i<label2path.size();i++) {

        if(label2path[i].empty())continue;

        fout <<hash_index2labelid[i+1]<<" ";
        for(auto path : label2path[i]) {
            fout<<path.first<<" "<<path.second<<" ";
        }
        fout<<endl;
     }

//    int labelcount=0;
//    for(auto l2p : label2path){
//        cout<<hash_index2labelid[labelcount+1]<<" "<<l2p.size()<<endl;
//        labelcount++;
//    }

}