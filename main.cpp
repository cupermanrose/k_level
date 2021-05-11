#include "RboxPoints.h"
#include "QhullError.h"
#include "QhullQh.h"
#include "QhullFacet.h"
#include "QhullFacetList.h"
#include "QhullFacetSet.h"
#include "QhullLinkedList.h"
#include "QhullPoint.h"
#include "QhullUser.h"
#include "QhullVertex.h"
#include "QhullVertexSet.h"
#include "Qhull.h"
#include <iostream>
#include <iomanip> // setw
#include <unordered_map>
#include <unordered_set>
//TODO merge to qhull_adapter
using std::cerr;
using std::cin;
using std::cout;
using std::endl;
using std::cout;
using std::endl;
using std::string;
using orgQhull::Qhull;
using orgQhull::QhullError;
using orgQhull::QhullFacet;
using orgQhull::QhullFacetList;
using orgQhull::QhullFacetSet;
using orgQhull::QhullPoint;
using orgQhull::QhullPoints;
using orgQhull::QhullQh;
using orgQhull::QhullUser;
using orgQhull::QhullVertex;
using orgQhull::QhullVertexSet;
using orgQhull::RboxPoints;
using orgQhull::Coordinates;

#include "k_level_lib.h"
#include "kspr.h"
#include "utk.h"
#include "oru.h"

enum func_type{buildidx, loadidx};
enum query_type{kspr, utk, oru};

void Config(int dim, int tau, int ik, string root_directory, string filename,
            string& datafile, string& logfile, string& idxfile, fstream& log,
            string func_str, func_type& func, string query_str, query_type& query){

    if (func_str=="buildidx") func=buildidx;
    else if (func_str=="loadidx") func=loadidx;
    else cout << "Unknown function!" << endl;

    if (query_str=="kspr") query=kspr;
    else if (query_str=="utk") query=utk;
    else if (query_str=="oru") query=oru;
    else cout << "Unknown query!" << endl;

    datafile=root_directory+"data/"+filename+".dat";
    if (func==buildidx){
        logfile=root_directory+"log/"+filename+"_dim"+to_string(dim)+"_tau"+to_string(tau)+".log";
    }
    else if (func==loadidx) {
        switch (query){
            case kspr:
                logfile=root_directory+"log/"+filename+"_dim"+to_string(dim)+"_tau"+to_string(tau)+"_query"+query_str+".ans";
                break;
            case utk:
                logfile=root_directory+"log/"+filename+"_dim"+to_string(dim)+"_tau"+to_string(tau)+"_query"+query_str+".ans";
                break;
            case oru:
                logfile=root_directory+"log/"+filename+"_dim"+to_string(dim)+"_tau"+to_string(tau)+"_query"+query_str+".ans";
                break;
        }
    }
    idxfile=root_directory+"index/"+filename+"_dim"+to_string(dim)+"_tau"+to_string(tau)+"_ik"+to_string(ik)+".idx";
    log.open(logfile, ios::out);
}

void ParameterInput(int argc, char* argv[], int& dim, int& tau, int& ik,
                    string& root_directory, string& filename, string& func_str, int& q_num, int& k, string& query_str){
    dim=4;
    tau=2; // NBA: tau=30
    ik=2;
    root_directory="/home/jiahaozhang/data/klevel/";
    filename="inde/U100K4";
    //filename="real/HOUSE6D";
    func_str="buildidx";

    q_num=5;
    k=1;
    query_str="oru";

}

int main(int argc, char* argv[]) {
    int dim,tau,ik,q_num,k;
    float utk_side_length;
    string datafile, logfile, idxfile, root_directory, filename, func_str,query_str;
    fstream log;
    query_type query;
    func_type func;
    ParameterInput(argc, argv, dim,tau,ik,root_directory,filename,func_str, q_num, k, query_str);
    Config(dim,tau,ik,root_directory,filename,datafile,logfile,idxfile,log, func_str, func, query_str, query);

    level idx(dim,tau,ik);

    switch (func) {
        case buildidx:
            IncBuildIndex(idx,datafile,log,idxfile);
            //DFSBuildIndex(idx,datafile,log,idxfile);
            //BuildIndex(idx, datafile, log, idxfile);
            break;
        case loadidx:
            LoadIndex(idx, datafile, log, idxfile);
            switch (query) {
                case kspr:
                    kspr::multiple_query(idx, 1, q_num, log);
                    kspr::multiple_query(idx, 5, q_num, log);
                    kspr::multiple_query(idx, 10, q_num, log);
                    kspr::multiple_query(idx, 15, q_num, log);
                    kspr::multiple_query(idx, 20, q_num, log);
                    kspr::multiple_query(idx, 30, q_num, log);
                    //kspr::multiple_query(idx, 40, q_num, log);
                    //kspr::multiple_query(idx, 50, q_num, log);
                    break;
                case utk:
                    utk_side_length=0.001;
                    cout << utk_side_length << endl;
                    log << utk_side_length << endl;
                    utk::multiple_query(idx, 1, q_num, utk_side_length, log);
                    utk::multiple_query(idx, 5, q_num, utk_side_length, log);
                    utk::multiple_query(idx, 10, q_num, utk_side_length, log);
                    utk::multiple_query(idx, 15, q_num, utk_side_length, log);
                    utk::multiple_query(idx, 20, q_num, utk_side_length, log);
                    utk::multiple_query(idx, 30, q_num, utk_side_length, log);
                    utk_side_length=0.005;
                    cout << utk_side_length << endl;
                    log << utk_side_length << endl;
                    utk::multiple_query(idx, 1, q_num, utk_side_length, log);
                    utk::multiple_query(idx, 5, q_num, utk_side_length, log);
                    utk::multiple_query(idx, 10, q_num, utk_side_length, log);
                    utk::multiple_query(idx, 15, q_num, utk_side_length, log);
                    utk::multiple_query(idx, 20, q_num, utk_side_length, log);
                    utk::multiple_query(idx, 30, q_num, utk_side_length, log);
                    utk_side_length=0.01;
                    cout << utk_side_length << endl;
                    log << utk_side_length << endl;
                    utk::multiple_query(idx, 1, q_num, utk_side_length, log);
                    utk::multiple_query(idx, 5, q_num, utk_side_length, log);
                    utk::multiple_query(idx, 10, q_num, utk_side_length, log);
                    utk::multiple_query(idx, 15, q_num, utk_side_length, log);
                    utk::multiple_query(idx, 20, q_num, utk_side_length, log);
                    utk::multiple_query(idx, 30, q_num, utk_side_length, log);
                    utk_side_length=0.05;
                    cout << utk_side_length << endl;
                    log << utk_side_length << endl;
                    utk::multiple_query(idx, 1, q_num, utk_side_length, log);
                    utk::multiple_query(idx, 5, q_num, utk_side_length, log);
                    utk::multiple_query(idx, 10, q_num, utk_side_length, log);
                    utk::multiple_query(idx, 15, q_num, utk_side_length, log);
                    utk::multiple_query(idx, 20, q_num, utk_side_length, log);
                    utk::multiple_query(idx, 30, q_num, utk_side_length, log);
                    utk_side_length=0.1;
                    cout << utk_side_length << endl;
                    log << utk_side_length << endl;
                    utk::multiple_query(idx, 1, q_num, utk_side_length, log);
                    utk::multiple_query(idx, 5, q_num, utk_side_length, log);
                    utk::multiple_query(idx, 10, q_num, utk_side_length, log);
                    utk::multiple_query(idx, 15, q_num, utk_side_length, log);
                    utk::multiple_query(idx, 20, q_num, utk_side_length, log);
                    utk::multiple_query(idx, 30, q_num, utk_side_length, log);
                    break;
                case oru:
                    k=1;
                    cout << k << endl;
                    oru::multiple_query(idx,k, 10,q_num,log);
                    oru::multiple_query(idx,k, 30,q_num,log);
                    oru::multiple_query(idx,k, 50,q_num,log);
                    oru::multiple_query(idx,k, 70,q_num,log);
                    oru::multiple_query(idx,k, 90,q_num,log);
                    k=5;
                    cout << k << endl;
                    oru::multiple_query(idx,k, 10,q_num,log);
                    oru::multiple_query(idx,k, 30,q_num,log);
                    oru::multiple_query(idx,k, 50,q_num,log);
                    oru::multiple_query(idx,k, 70,q_num,log);
                    oru::multiple_query(idx,k, 90,q_num,log);
                    k=10;
                    cout << k << endl;
                    oru::multiple_query(idx,k, 10,q_num,log);
                    oru::multiple_query(idx,k, 30,q_num,log);
                    oru::multiple_query(idx,k, 50,q_num,log);
                    oru::multiple_query(idx,k, 70,q_num,log);
                    oru::multiple_query(idx,k, 90,q_num,log);
                    k=15;
                    cout << k << endl;
                    oru::multiple_query(idx,k, 10,q_num,log);
                    oru::multiple_query(idx,k, 30,q_num,log);
                    oru::multiple_query(idx,k, 50,q_num,log);
                    oru::multiple_query(idx,k, 70,q_num,log);
                    oru::multiple_query(idx,k, 90,q_num,log);
                    k=20;
                    cout << k << endl;
                    oru::multiple_query(idx,k, 10,q_num,log);
                    oru::multiple_query(idx,k, 30,q_num,log);
                    oru::multiple_query(idx,k, 50,q_num,log);
                    oru::multiple_query(idx,k, 70,q_num,log);
                    oru::multiple_query(idx,k, 90,q_num,log);
                    break;
            }
            break;
    }

    cout << "DONE" << endl;
    log.close();
    return 0;
}


