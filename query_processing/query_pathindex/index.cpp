
#include "index.h"

PathIndex::PathIndex(std::string input_file_name)
{

    std::ifstream input(input_file_name);

    if(!input){
        std::cout<<"error: cannot open index file"<<std::endl;
        exit(1);
    }

    input>>k>>labelnum;

    hash_labelid2index.set_empty_key(-1);
    string v1,v2,label;
    int v1Id,v2Id;
    long int labelId;
    string line;
    while(std::getline(input,line)){
//        cout<<line<<endl;
        std::stringstream ss(line);
        ss >> labelId;
        ss.ignore(line.size(), ' ');
        //std::cout << name << " = ";
        vector<pair<int,int>> temppathset;
        temppathset.clear();
        while(ss >> v1Id >>v2Id)
        {
            temppathset.push_back(make_pair(v1Id,v2Id));
        }
        label2path.push_back(temppathset);
        hash_labelid2index[labelId]=label2path.size();
        //std::cout << "\n";
    }

}



void PathIndex::SetWorkload(std::string workload_file_name){

    std::ifstream inputwork(workload_file_name);

    if(!inputwork){
        std::cout<<"error: cannot open index index workload file"<<std::endl;
        exit(1);
    }

    string line, string_label;
    long int labelsequence;
    givenindexworkload.set_empty_key(-1);

    while(std::getline(inputwork,line)) {
        std::stringstream ss;
        ss << line;
        //cout<<ss.str()<<endl;
        int labelcount = 0;
        while (ss >> string_label) {
            if (std::all_of(string_label.cbegin(), string_label.cend(), [](char ch) { return isdigit(ch); })) {
                if (labelcount == 0) {
                    labelsequence = stol(string_label);
                } else {
                    labelsequence = (labelsequence + 1) * labelnum + stol(string_label);
                }
                labelcount++;
            }
        }
        if (hash_labelid2index[labelsequence] == 0){
            vector<pair<int,int>> temppathset;
            temppathset.clear();
            label2path.push_back(temppathset);
            hash_labelid2index[labelsequence] = label2path.size();
        }
        givenindexworkload[labelsequence]=true;
    }
}
