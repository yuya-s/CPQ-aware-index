#include "index.h"
#include "query.h"


int main(int argc, char* argv[])
{

    string resultFile;
    string inputFile;
    string workloadFile;
    string graphFile;
    string indexworkloadFile;
    string indexFile;
    int given_k=0;
    string string_k;
    bool workloadindex=false;
    string maintenancegraph;
    bool maintenancegraphFlag=false;
    string maintenanceworkload;
    bool maintenanceworkloadFlag=false;
    bool maintainedindexoutput=false;

    try {
        for (int i = 1; i < argc; ++i) {
            std::string s = argv[i];
            if (s[0] == '-') {
                s = s.substr(1);

                if (s == "i") {
                    inputFile = argv[++i];
                }
                else if (s == "if") {
                    indexFile = argv[++i];
                }
                else if (s == "o") {
                    resultFile = argv[++i];
                }
                else if (s == "k"){
                    string_k=argv[++i];
                    given_k = stoi(argv[i]);
                }
                else if (s == "qw") {
                    workloadFile = argv[++i];
                }
                else if(s == "iw"){
                    indexworkloadFile = argv[++i];
                    workloadindex = true;
                }
                else if(s == "mg"){
                    maintenancegraph = argv[++i];
                    maintenancegraphFlag=true;
                }
                else if(s == "mw"){
                    maintenanceworkload = argv[++i];
                    maintenanceworkloadFlag=true;
                }
                else if(s == "mo"){
                    maintainedindexoutput=true;
                }
                else {
                    throw std::exception();
                }
            }
            else {
                throw std::exception();
            }
        }
    }
    catch (std::exception& e) {
        cout<<"error input"<<endl;
        return 1;
    }


    Result result;
    result.Crear();
    string graphfile = inputFile;
    result.k = given_k;
    result.inputFile = inputFile;
    result.outputFile = "./result/"+resultFile;

    struct timespec query_startTime, query_endTime, maintenance_startTime, maintenance_endTime;
    long double responseTime;

    cout<<inputFile<<endl;
    cout<<"Index Input Start"<<endl;

    BisimulationIndex bisimulationindex(indexFile);
    //bisimulationindex.CheckIndex("check1");

    if (workloadindex) {
        bisimulationindex.SetWorkload(indexworkloadFile);
        bisimulationindex.workloadindex = true;
    }

    cout<<"Index Input Done"<<endl;

    cout<<"Graph Input Start"<<endl;
    OriginalGraph originalgraph = OriginalGraph();
    originalgraph.InputFile(graphfile);
    cout<<"Graph Input Done"<<endl;




    if(maintenancegraphFlag) {//mentenance test
        string maintain_outputeach;
        maintain_outputeach = "./result/maintenance_"+resultFile;
        std::ofstream fout_maintain(maintain_outputeach, ios::app);
        vector<Edge> maintenanceEdgeSet;
        maintenanceEdgeSet.clear();
        double maintainTime;

        vector<int> deleteedgeidlist;
        deleteedgeidlist.clear();

        std::ifstream inputgraphmaintenance(maintenancegraph);
        if(!inputgraphmaintenance){
            std::cout<<"error: cannot open index update edge file"<<std::endl;
            exit(1);
        }
        string line, string_edgeid;
        while(std::getline(inputgraphmaintenance,line)){
    //        cout<<line<<endl;
            std::stringstream ss;
            ss<<line;
            //cout<<ss.str()<<endl;

            while(ss >> string_edgeid)
            {
                 if(std::all_of(string_edgeid.cbegin(),string_edgeid.cend(), [](char ch) { return isdigit(ch);})) {
                     deleteedgeidlist.push_back(stoi(string_edgeid));
                 }
            }
        }

        for(int g=0;g<deleteedgeidlist.size();g++) {

            Edge dedge = originalgraph.edge_list[deleteedgeidlist[g]];

            maintenanceEdgeSet.push_back(dedge);
            cout<<g<<":delete"<<dedge.src<<":"<<originalgraph.degrees[dedge.src]<<"->"<<dedge.dst<<":"<<originalgraph.degrees[dedge.dst]<<endl;

            clock_gettime(CLOCK_REALTIME, &maintenance_startTime);

            bisimulationindex.DeleteEdge(dedge, originalgraph);

            clock_gettime(CLOCK_REALTIME, &maintenance_endTime);
            maintainTime = (maintenance_endTime.tv_sec - maintenance_startTime.tv_sec) + (maintenance_endTime.tv_nsec - maintenance_startTime.tv_nsec) * pow(10, -9);
            cout<<"delete time: "<<maintainTime<<endl;
            fout_maintain <<originalgraph.vertex_edge_list[dedge.src].size()+originalgraph.vertex_inverseedge_list[dedge.src].size()<<","<< originalgraph.vertex_edge_list[dedge.dst].size()+originalgraph.vertex_inverseedge_list[dedge.dst].size()<<","<<maintainTime<< endl;
            result.maintenanceDeleteEdgeTime += maintainTime;
        }
        cout<<"total delete time: "<<result.maintenanceDeleteEdgeTime<<endl;

        for(int i=0;i<maintenanceEdgeSet.size();i++) {
            Edge aedge = maintenanceEdgeSet[i];
            cout<<i<<":add"<<aedge.src<<"->"<<aedge.dst<<endl;
            clock_gettime(CLOCK_REALTIME, &maintenance_startTime);

            bisimulationindex.AddEdge(aedge, originalgraph);

            clock_gettime(CLOCK_REALTIME, &maintenance_endTime);
            maintainTime = (maintenance_endTime.tv_sec - maintenance_startTime.tv_sec) + (maintenance_endTime.tv_nsec - maintenance_startTime.tv_nsec) * pow(10, -9);

            cout<<"add time: "<<maintainTime<<endl;
            fout_maintain <<originalgraph.vertex_edge_list[aedge.src].size()+originalgraph.vertex_inverseedge_list[aedge.src].size()<<","<< originalgraph.vertex_edge_list[aedge.dst].size()+originalgraph.vertex_inverseedge_list[aedge.dst].size()<<","<<maintainTime<< endl;
            result.maintenanceAddEdgeTime += maintainTime;
        }

        result.maintenanceEdgeNum=deleteedgeidlist.size();
        result.OutputMaintenance();
        //bisimulationindex.CheckIndex("check2");
        bisimulationindex.Reoptimization();
        //bisimulationindex.CheckIndex("check3");
        if(maintainedindexoutput) {
            string mainteinedindex;
            mainteinedindex = indexFile + "MG";
            bisimulationindex.Output(mainteinedindex);
        }

    }


    if(maintenanceworkloadFlag && bisimulationindex.workloadindex) {
        string maintain_outputeach;
        maintain_outputeach = "./result/workloadmaintenance_"+resultFile;

        std::ofstream fout_maintainwa(maintain_outputeach, ios::app);
        vector<vector<int>> maintenancelabelset;
        vector<int> maintenancelabel;
        std::ifstream inputworkmaintenance(maintenanceworkload);
        double maintainTime;
        if(!inputworkmaintenance){
            std::cout<<"error: cannot open index givenindexworkload file"<<std::endl;
            exit(1);
        }
        string line, string_label;
        while(std::getline(inputworkmaintenance,line)){
    //        cout<<line<<endl;
            std::stringstream ss;
            ss<<line;
            //cout<<ss.str()<<endl;
            maintenancelabel.clear();
            while(ss >> string_label)
            {
                 if(std::all_of(string_label.cbegin(),string_label.cend(), [](char ch) { return isdigit(ch);})) {
                     maintenancelabel.push_back(stoi(string_label));
                 }
            }
            maintenancelabelset.push_back(maintenancelabel);
        }

        for(auto label : maintenancelabelset) {
           clock_gettime(CLOCK_REALTIME, &maintenance_startTime);

            bisimulationindex.DeleteWorkload(label,originalgraph);

            clock_gettime(CLOCK_REALTIME, &maintenance_endTime);
            maintainTime = (maintenance_endTime.tv_sec - maintenance_startTime.tv_sec) + (maintenance_endTime.tv_nsec - maintenance_startTime.tv_nsec) * pow(10, -9);
            fout_maintainwa <<maintainTime<< endl;
            cout<<"delete givenindexworkload time: "<<maintainTime<<endl;
            result.maintenanceDeleteWorkloadTime += maintainTime;
        }
        for(auto label : maintenancelabelset) {
            clock_gettime(CLOCK_REALTIME, &maintenance_startTime);

            bisimulationindex.AddWorkload(label,originalgraph);

            clock_gettime(CLOCK_REALTIME, &maintenance_endTime);
            maintainTime = (maintenance_endTime.tv_sec - maintenance_startTime.tv_sec) + (maintenance_endTime.tv_nsec - maintenance_startTime.tv_nsec) * pow(10, -9);
            fout_maintainwa <<maintainTime<< endl;
            cout<<"add givenindexworkload time: "<<maintainTime<<endl;
            result.maintenanceAddWorkloadTime = maintainTime;

        }

        result.OutputMaintenance();

        if(maintainedindexoutput) {
            string mainteinedindex = indexFile + "MW";
            bisimulationindex.Output(mainteinedindex);
        }
    }

    cout<<"Query Input Start"<<endl;
    QuerySet queryset=QuerySet();
    queryset.InputFile(workloadFile, bisimulationindex);
    cout<<"Query Input Done"<<endl;

    result.querynum = queryset.rootqueries.size();

    cout<<"query START"<<endl;
    string outputeach;
    outputeach = "./result/answernum_"+resultFile;
    std::ofstream fout(outputeach, ios::app);

    vector<pair<int,int>> answers;
    for(int for_query =0; for_query<queryset.rootqueries.size();for_query++) {

        answers.clear();
        clock_gettime(CLOCK_REALTIME, &query_startTime);
        queryset.rootqueries[for_query].Evaluation(answers, bisimulationindex);

        clock_gettime(CLOCK_REALTIME, &query_endTime);
        responseTime = (query_endTime.tv_sec - query_startTime.tv_sec) +
                       (query_endTime.tv_nsec - query_startTime.tv_nsec) * pow(10, -9);

        sort(answers.begin(), answers.end());
        answers.erase(std::unique(answers.begin(), answers.end()), answers.end());

        cout << for_query<<": # of answers: " << answers.size() << ", time: " << responseTime << endl;
        fout <<answers.size() <<","<<responseTime<< endl;
        result.querySumTime+=responseTime;
    }
    result.Output();

    return 0;
}
