
#include "pathset.h"
#include "graph.h"



PathSet::PathSet(int k_ , OriginalGraph graph_,WorkLoad workload) {

    struct timespec test_startTime, test_endTime, enumerate_startTime, enumerate_endTime, sort_startTime, sort_endTime, id_startTime, id_endTime;

    k=k_;
    number_of_labels = graph_.number_of_labels*2;
    number_of_vertices=graph_.number_of_vertices;
    number_of_edges=graph_.number_of_edges;

    seg.resize(k);
    Path pathkey;


    vector<google::dense_hash_map< int, int>> pathlist;
    vector<google::dense_hash_map< int, int>> pathlist0;

    pathkey.src=-1;
    pathkey.dst=-1;
    pathlist.resize(number_of_vertices);
    pathlist0.resize(number_of_vertices);
    vector<int> tempvector={-1};
    for(int i=0;i<number_of_vertices;i++) {
        pathlist[i].set_empty_key(-1);
        pathlist0[i].set_empty_key(-1);
    }

    vector<vector<vector<pair<int,int>>>> pathlist_k; // for efficient join
    pathlist_k.resize(k);
    for(int i=0;i<k;i++)pathlist_k[i].resize(graph_.number_of_edges);
    int count_enumeration=0;
    clock_gettime(CLOCK_REALTIME, &enumerate_startTime);
    vector<vector<Segment>> seg_prefix;
    seg_prefix.resize(k);

    for(int i=0;i<k;i++)seg_prefix[i].clear();   
    vector<Segment> seg0;
    //cout<<"path 0 start"<<endl;

    for(auto edge : graph_.edge_list){
        //cout<<count_enumeration<<"/"<<number_of_edges<<endl;
        //count_enumeration++;

        Segment tempSegment;
        int pathid;
        pathkey.src = edge.src;pathkey.dst=edge.dst;

        pathid = pathlist0[pathkey.src][pathkey.dst];

        if(pathid == 0) {
            tempSegment.clear();
            tempSegment.path = pathkey;
            tempSegment.labelidset.insert(edge.label);
            tempSegment.order = seg[0].size();
            seg[0].push_back(tempSegment);

            pathlist0[pathkey.src][pathkey.dst] = seg[0].size();
        }
        else{
            seg[0][pathid-1].labelidset.insert(edge.label);

        }

        pathkey.src = edge.dst;pathkey.dst=edge.src;
        pathid = pathlist0[pathkey.src][pathkey.dst];
         if(pathid == 0) {
             tempSegment.clear();
             tempSegment.path = pathkey;
             tempSegment.labelidset.insert(edge.label + graph_.number_of_labels);
             tempSegment.order = seg[0].size();
             seg[0].push_back(tempSegment);

             pathlist0[pathkey.src][pathkey.dst] = seg[0].size();
         }
        else{
             seg[0][pathid-1].labelidset.insert(edge.label+graph_.number_of_labels);
         }
    }

    for(int i=0;i<k;i++){//for efficient join
        for(auto segment : seg[0]){
            for(auto label: segment.labelidset) {
                if(workload.kth_labels_workload[i][label]) {
                    pathlist_k[i][segment.path.src].push_back(make_pair(segment.path.dst, segment.order));
                    if(i==0){
                        seg_prefix[0].push_back(segment);
                    }
                    break;
                }
            }
        }
    }

    int countid=1;

    int count;
    int previousSegvalue;
    int previousloop;

    count=0;
    countid=1;
    vector<Segment> prefixPath;

    clock_gettime(CLOCK_REALTIME, &enumerate_endTime);

    cout<<"length0 DONE ";
    cout<<(enumerate_endTime.tv_sec - enumerate_startTime.tv_sec) + (enumerate_endTime.tv_nsec - enumerate_startTime.tv_nsec) * pow(10, -9)<<endl;


    Segment tempSegment;
    Segment prefixtempSegment;
    vector<Segment> segmentVectorSameSrc;
    segmentVectorSameSrc.clear();

    for(int for_k = 1;for_k<k;for_k++) {// for all k length path
    //computing label sequence for all paths
    count_enumeration=0;
    //cout<<"length" <<for_k<< " size = "<<seg.size()<<": start"<<endl;
    clock_gettime(CLOCK_REALTIME, &enumerate_startTime);
	if(for_k==1){
        for (Segment segment : seg_prefix[0]) {
	
            //cout<<count_enumeration<<"/"<<seg.size()<<endl;
            count_enumeration++;

            Path pathl; pathl.src=segment.path.src;pathl.dst=segment.path.dst;
            int id_path0;

            for (auto endVertex : pathlist_k[for_k][segment.path.dst]) {
                pathkey.src= segment.path.src;
                pathkey.dst= endVertex.first;

                int pathid = pathlist[pathkey.src][pathkey.dst];
                id_path0 = endVertex.second;
                if(pathid == 0) {
                    tempSegment.clear();
                    tempSegment.path = pathkey;
                    tempSegment.order = seg[for_k].size();
		    
                    for (auto labelid : segment.labelidset) {
                        for (auto label_0 : seg[0][id_path0].labelidset) {
                            long int newlabelid = (1 + labelid) * number_of_labels + label_0;
                            if (workload.prefixgivenworkload[newlabelid]||workload.givenworkload[newlabelid])tempSegment.labelidset.insert(newlabelid);
                        }
                    }
                    if(!tempSegment.labelidset.empty()){
                        seg[for_k].push_back(tempSegment);
                        pathlist[pathkey.src][pathkey.dst]=seg[for_k].size();
		            }
                }
                else{
                    for (auto labelid : segment.labelidset) {
                        for (auto label_0 : seg[0][id_path0].labelidset) {
                            long int edgelabel = (1 + labelid) * number_of_labels + label_0;
                            if (workload.prefixgivenworkload[edgelabel]==1||workload.givenworkload[edgelabel]==1)seg[for_k][pathid-1].labelidset.insert(edgelabel);
                            //seg[for_k][pathid-1].labelidset.insert(edgelabel);
                        }
                    }
                }
            }
        }
	}
	else{
        for (Segment segment : seg[for_k - 1]) {
	
            //cout<<count_enumeration<<"/"<<seg.size()<<endl;
            count_enumeration++;

            Path pathl; pathl.src=segment.path.src;pathl.dst=segment.path.dst;
            int id_path0;

            for (auto endVertex : pathlist_k[for_k][segment.path.dst]) {

                pathkey.src= segment.path.src;
                pathkey.dst= endVertex.first;

                int pathid = pathlist[pathkey.src][pathkey.dst];
                id_path0 = endVertex.second;
                if(pathid == 0) {
                    tempSegment.clear();
                    tempSegment.path = pathkey;
                    tempSegment.order = seg[for_k].size();
		    
                    for (auto labelid : segment.labelidset) {
                        for (auto label_0 : seg[0][id_path0].labelidset) {
                            long int newlabelid = (1 + labelid) * number_of_labels + label_0;
                            if (workload.prefixgivenworkload[newlabelid]||workload.givenworkload[newlabelid])tempSegment.labelidset.insert(newlabelid);
                        }
                    }
                    if(!tempSegment.labelidset.empty()){
                        seg[for_k].push_back(tempSegment);
                        pathlist[pathkey.src][pathkey.dst]=seg[for_k].size();
		            }
                }
                else{
                    for (auto labelid: segment.labelidset) {
                        for (auto label_0 : seg[0][id_path0].labelidset) {
                            long int newlabelid = (1 + labelid) * number_of_labels + label_0;
                            if (workload.prefixgivenworkload[newlabelid]||workload.givenworkload[newlabelid])seg[for_k][pathid-1].labelidset.insert(newlabelid);
                        }
                    }
                }
            }
        }
	}

        for(int i=0;i<number_of_vertices;i++) {
            pathlist[i].clear();
        }
        clock_gettime(CLOCK_REALTIME, &enumerate_endTime);
        cout<<"length" <<for_k<<": enumarate DONE ";
        cout<<(enumerate_endTime.tv_sec - enumerate_startTime.tv_sec) + (enumerate_endTime.tv_nsec - enumerate_startTime.tv_nsec) * pow(10, -9)<<endl;
        //cout<<seg.size()<<endl;

        //clock_gettime(CLOCK_REALTIME, &enumerate_endTime);
        //cout<<"length" <<for_k<<": sort DONE ";
        //cout<<(enumerate_endTime.tv_sec - enumerate_startTime.tv_sec) + (enumerate_endTime.tv_nsec - enumerate_startTime.tv_nsec) * pow(10, -9)<<endl;


        //cout<<(enumerate_endTime.tv_sec - enumerate_startTime.tv_sec) + (enumerate_endTime.tv_nsec - enumerate_startTime.tv_nsec) * pow(10, -9)<<endl;

    }//for_k


   for(int for_k=1;for_k <k ;for_k++){
	for(Segment segment : seg[for_k]){	
                pathkey.src= segment.path.src;
                pathkey.dst= segment.path.dst;

            int pathid = pathlist0[pathkey.src][pathkey.dst];
            if(pathid == 0) {
                tempSegment.clear();
                tempSegment.path = pathkey;
                tempSegment.order = seg[0].size();

                for (auto label_l : segment.labelidset) {
                        if (workload.givenworkload[label_l]==1)tempSegment.labelidset.insert(label_l);
                    }

                if(!tempSegment.labelidset.empty()){
                    seg[0].push_back(tempSegment);
                    pathlist0[pathkey.src][pathkey.dst]=seg[0].size();
		        }
            }
            else{
                for (auto label_l : segment.labelidset) {
                        if (workload.givenworkload[label_l]==1)seg[0][pathid-1].labelidset.insert(label_l);
                }
            }
		}
   }

    cout<<"k-path-set DONE"<<endl;

}







//void PathSet::Output(string indexfilename, OriginalGraph graph_){
//
//    std::ofstream fout(indexfilename, ios::app);
//
//    for(int for_k=0;for_k<k;for_k++){
//        fout<<"length = "<<for_k<<endl;
//        for(auto segment : seg[for_k]){
//            fout<<segment.path.src<<"->"<<segment.path.dst<<"(";
//            for (auto ittA = segment.labelidset.begin(); ittA != segment.labelidset.end(); ittA++) {
//                //for(auto label : hash2pathlabel[*ittA]){
//                fout << *ittA;
//                //}
//                fout<<",";
//            }
//            fout <<")"<<endl;
//
//        }
//
//    }
//}



