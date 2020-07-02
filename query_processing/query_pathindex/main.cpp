#include "utility.h"
#include "index.h"
#include "query.h"

int main(int argc, char* argv[])
{

    string resultFile;
    string inputFile;
    string workloadFile;
    string indexFile;
    string indexworkloadFile;
    bool workloadindex=false;
    int given_k;
    string string_k;
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
                    string_k = argv[++i];
                    given_k = stoi(argv[i]);
                }
                else if (s == "qw") {
                    workloadFile = argv[++i];
                }
                else if(s == "iw"){
                    indexworkloadFile = argv[++i];
                    workloadindex = true;
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

    struct timespec query_startTime, query_endTime, index_startTime, index_endTime;
    long double responseTime;
    cout<<inputFile<<endl;

    cout<<"Index Input Start"<<endl;
    PathIndex pathindex(indexFile);

    if (workloadindex) {
        pathindex.SetWorkload(indexworkloadFile);
        pathindex.workloadindex = true;
    }

    cout<<"Index Input Done"<<endl;

    QuerySet queryset=QuerySet();

    queryset.InputFile(workloadFile, pathindex);
    result.querynum=queryset.rootqueries.size();


    Query query=Query();

    string outputeach;
    outputeach = "./result/answernum_"+resultFile;
    std::ofstream fout(outputeach, ios::app);

    vector<pair<int,int>> answers;
    for(int for_query =0; for_query<queryset.rootqueries.size();for_query++) {
    

        answers.clear();
        clock_gettime(CLOCK_REALTIME, &query_startTime);
        queryset.rootqueries[for_query].Evaluation(answers, pathindex);

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
