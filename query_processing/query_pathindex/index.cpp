//
// Created by sasaki on 19/04/16.
//
#include "index.h"

PathIndex::PathIndex(std::string input_file_name)
{

    std::ifstream input(input_file_name);

    if(!input){
        std::cout<<"error: cannot open index file"<<std::endl;
        exit(1);
    }

    input>>k>>labelnum;

    //int labelidsize=labelnum;
    //for(int i=1;i<k;i++)labelidsize += pow(labelnum,i+1);
    //label2path.resize(labelidsize);

    labelidhash.set_empty_key(-1);
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
            //label2path[labelId].push_back(make_pair(v1Id,v2Id));
        }
        label2path.push_back(temppathset);
        labelidhash[labelId]=label2path.size();
        //std::cout << "\n";
    }
//
//    while(!input.eof()){
//
//        input >> labelId >> v1Id >>v2Id;
//        if(input.fail())break;
//        label2path[labelId].push_back(make_pair(v1Id,v2Id));
//    }
    //number_of_edges = edge_list.size();
}