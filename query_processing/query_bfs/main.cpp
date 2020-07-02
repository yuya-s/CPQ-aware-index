#include "utility.h"
#include "query.h"

int main(int argc, char* argv[])
{

    string resultFile;
    string inputFile;
    string workloadFile;

    try {
        for (int i = 1; i < argc; ++i) {
            std::string s = argv[i];
            if (s[0] == '-') {
                s = s.substr(1);

                if (s == "i") {
                    inputFile = argv[++i];
                }
                else if (s == "o") {
                    resultFile = argv[++i];
                }
                else if (s == "qw") {
                    workloadFile = argv[++i];
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
    result.inputFile = inputFile;
    result.outputFile = "./result/"+resultFile;

    struct timespec query_startTime, query_endTime, index_startTime, index_endTime;
    long double responseTime;
    cout<<inputFile<<endl;
    cout<<"Graph Input Start"<<endl;
    OriginalGraph originalgraph = OriginalGraph();
    originalgraph.InputFile(graphfile);
    cout<<"Graph Input Done"<<endl;

    QuerySet queryset=QuerySet();
    queryset.InputFile(workloadFile);

    result.querynum=queryset.rootqueries.size();

    Query query=Query();


    string outputeach;
    outputeach = "./result/answernum_"+resultFile;
    std::ofstream fout(outputeach, ios::app);

    vector<pair<int,int>> answers;
    for(int for_query =0; for_query<queryset.rootqueries.size();for_query++) {

        answers.clear();
        clock_gettime(CLOCK_REALTIME, &query_startTime);
        queryset.rootqueries[for_query].Evaluation(answers, originalgraph);
        clock_gettime(CLOCK_REALTIME, &query_endTime);
        responseTime = (query_endTime.tv_sec - query_startTime.tv_sec) + (query_endTime.tv_nsec - query_startTime.tv_nsec) * pow(10, -9);


        sort(answers.begin(), answers.end());
        answers.erase(std::unique(answers.begin(), answers.end()), answers.end());

        cout << for_query<<": # of answers: " << answers.size() << ", time: " << responseTime << endl;
        fout <<answers.size() <<","<<responseTime<< endl;
        result.querySumTime+=responseTime;
    }
    result.Output();

    return 0;
}
