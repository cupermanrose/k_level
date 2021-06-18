//
// Created by cuper on 2021/3/30.
//

#include "k_level_lib.h"

void BuildIndex(level& idx, string datafile, fstream& log, string idxfile) {
    ofstream idxout(idxfile);
    idx.LoadData(datafile);
    idx.Build(log,idxout);
    log.close();
    idxout.close();
}

void IncBuildIndex(level& idx, string datafile, fstream& log, string idxfile) {
    ofstream idxout(idxfile);
    idx.LoadData(datafile);
    idx.IncBuild(log,idxout);
    log.close();
    idxout.close();
}

void DFSBuildIndex(level& idx, string datafile, fstream& log, string idxfile) {
    ofstream idxout(idxfile);
    idx.LoadData(datafile);
    idx.DFSBuild(log,idxout);
    log.close();
    idxout.close();
}

void LoadIndex(level& idx, string datafile, fstream& log, string idxfile) {
    ifstream idxin(idxfile);
    idx.LoadData(datafile);
    vector<int> candidate;
    idx.GlobalFilter(log,candidate); //TODO remove this and load from index file

    vector<kcell> empty_level; empty_level.clear();
    idx.idx.emplace_back(empty_level);
    for (int k=1;k<=idx.tau;k++){
        if (k>idx.ik) break;
        idx.idx.push_back(empty_level);
        idx.ReadFromDisk(k,idxin);
        cout << "The size of level " << k << ": "<<idx.idx[k].size() << endl;
        log << "The size of level " << k << ": "<<idx.idx[k].size() << endl;
    }
    idxin.close();

    cout << "Load Index Done!" << endl;
    log << "Load Index Done!" << endl;
    idx.print_system_info(log);
}

void Vertex2BOX(const vector<kcell> &L, vector<vector<float>>& MBRs, int dim){
    // the convex hull defined by vertexes to lower bound upper bound box
    // dim-1 vertex to dim box
    if(L.empty() || L.begin()->r.V.empty()){
        MBRs.clear();
        return;
    }
    MBRs.clear();
    for(auto &iter: L){
        vector<float> box(dim*2);
        for (int j = 0; j < dim; ++j) {
            box[j]=1;
        }
        for(auto &vertex: iter.r.V){
            float bias=1;

            for(auto &attr: vertex){
                bias-=attr;
            }

            for (int i = 0; i < vertex.size(); ++i) {
                box[i]=min(box[i], vertex[i]);
            }
            box[dim-1]=min(box[dim-1], bias);

            for (int i = 0; i < vertex.size(); ++i) {
                box[i+dim]=max(box[i+dim], vertex[i]);
            }
            box[dim-1+dim]=max(box[dim-1+dim], bias);
        }
        MBRs.emplace_back(box);
    }
    return;
}

void BuildRtree(const vector<kcell> &L, Rtree* &rt, unordered_map<long int, RtreeNode*>& ramTree, int dim){
    vector<vector<float>> MBRs;
    Vertex2BOX(L,MBRs, dim);
    box2rtree(rt, ramTree, MBRs);
}

float sum(vector<float> &v){
    float ret=0;
    for(auto &f:v){
        ret+=f;
    }
    return ret;
}

void RangeQueryFromRtree(Rtree* rt, unordered_map<long int, RtreeNode*> &ramTree,
                         vector<float> &ql, vector<float> &qu, vector<int> &ret_ids){
    vector<float> target_l(ql);
    vector<float> target_u(qu);

    target_l.push_back(1-sum(ql));
    target_u.push_back(1-sum(qu));

    rtree_boxInter(ret_ids, rt, ramTree, target_l, target_u);
}


