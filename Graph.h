/****************************************************
 *  2016-10-11
 *  Undirected graph based on the adjacency multilist
 *  cxc123
 ****************************************************/

#ifndef GENERIC_H_INCLUDED
#define GENERIC_H_INCLUDED
#define SUCCESSED 1
#define FAILED -1
#define NOTVISITED(vex) (vex->mark == UNVISITED)
#include <vector>
#include <deque>
#include <stack>
#include <stdexcept>
#endif // GENERIC_H_INCLUDED

#ifndef UDGRAPH_H_INCLUDED
#define UDGRAPH_H_INCLUDED
#define VISITED 1
#define UNVISITED 0

template <typename T>
class UDGraph //Undirected Graph
{

public: // Structs
    struct Edge;
    struct Vertex;

    struct Edge{
        int ivex,jvex;
        Edge *ilink,*jlink;
        int weight;
    };

    struct Vertex{
        bool mark;
        int VertexIndex;
        T data;
        Edge *FirstEdge;
    };

    typedef struct Edge Edge,*EdgePtr;
    typedef struct Vertex Vertex,*VertexPtr;

public: //Variates

    int VertexNum;
    int EdgeNum;
    std::vector<VertexPtr> VertexList;

public: // Basic functions

    UDGraph()// Default constructor
    {
        InitGraph();
    }

    UDGraph(const UDGraph &ud_graph) : UDGraph() // Copy constructor
    {
        for(int i=0;i<ud_graph.VertexNum;i++){
            AddVertex(ud_graph.VertexList[i]->data);
        }

        for(int i=0;i<VertexNum;i++){
            EdgePtr edge_tmp = ud_graph.VertexList[i]->FirstEdge;
            if(!edge_tmp) continue;

            while(edge_tmp){
                AddEdge(edge_tmp->ivex,edge_tmp->jvex,edge_tmp->weight);
                edge_tmp = ud_graph.NextEdge(i,edge_tmp);
            }
        }
    }

    ~UDGraph()// Destructor
    {
        DestroyGraph();
    }

    inline void InitGraph() {
        VertexNum = 0;
        EdgeNum = 0;
    }

    void Unvisit_all_vex(){
        for(int i = 0;i < VertexNum; i++){
            VertexList[i]->mark = UNVISITED;
        }
    }

    inline EdgePtr FirstEdge (int VertexIndex) const{
        if(VertexNum <= VertexIndex || VertexNum == 0)  return nullptr;
        return VertexList[VertexIndex]->FirstEdge;
    }

    void AddVertex(const T &data,EdgePtr FirstEdge = nullptr)
    {
        VertexPtr new_vex_ptr = new Vertex;
        new_vex_ptr->data = data;
        new_vex_ptr->mark = UNVISITED;
        new_vex_ptr->FirstEdge = nullptr;
        new_vex_ptr->VertexIndex = VertexNum++;
        VertexList.push_back(new_vex_ptr);
    }

    bool isEdgeExist(int ivex,int jvex) const {
        if(EdgeNum == 0) return false;
        if(ivex >= VertexNum || jvex >= VertexNum || ivex == jvex || VertexNum == 0) return false;
        EdgePtr edge = FirstEdge(ivex);
        if(!edge) return false;

        while(!(edge->ivex == ivex && edge->jvex == jvex)
                && !(edge->jvex == ivex && edge->ivex == jvex)) {
            edge = NextEdge(ivex, edge);
            if(!edge) return false;
         }

        return true;
    }

    bool isVertexExist(int VertexIndex) const{
        if(VertexNum > 0 && VertexIndex <= VertexNum && VertexIndex >= 0)
            return true;
        else
            return false;
    }

    EdgePtr GetEdge(int ivex,int jvex) const{
        if(EdgeNum == 0) return nullptr;
        if(ivex >= VertexNum || jvex >= VertexNum || ivex == jvex || VertexNum == 0) return nullptr;
        EdgePtr edge = FirstEdge(ivex);
        if(!edge) return nullptr;
        while(edge->jvex != jvex) {
            edge = NextEdge(ivex, edge);
            if(!edge) return nullptr;
        }
        return edge;
    }

    EdgePtr NextEdge(int VertexIndex,EdgePtr edge_prior) const{
        EdgePtr ret_edge;
        if(EdgeNum == 0) return nullptr;
        if(VertexIndex >= VertexNum || !edge_prior || VertexNum == 0) return nullptr;
        int vex_index = edge_prior->ivex;
        if(vex_index == VertexIndex)
            ret_edge = edge_prior->ilink;
        else
            ret_edge = edge_prior->jlink;
        return ret_edge;
    }

    EdgePtr LastEdge(int VertexIndex) const{
        if(EdgeNum == 0) return nullptr;
        EdgePtr edge = FirstEdge(VertexIndex);
        if(!edge) return nullptr;

        while((edge->ivex == VertexIndex && edge->ilink)
              || (edge->jvex == VertexIndex && edge->jlink)){
            if(edge->ivex == VertexIndex)
                edge = edge->ilink;
            else
                edge = edge->jlink;
        }
        return edge;
    }

    bool AddEdge(int ivex,int jvex, int weight)
    {
        if(VertexNum == 0) return false;
        if(weight < 0) return false;
        if(isEdgeExist(ivex,jvex)) return false;
        EdgePtr ivex_edge_last = LastEdge(ivex);
        EdgePtr jvex_edge_last = LastEdge(jvex);
        EdgePtr edge_new = new Edge;
        edge_new->weight = weight;
        edge_new->ivex = ivex;
        edge_new->jvex = jvex;
        edge_new->ilink = nullptr;
        edge_new->jlink = nullptr;
        EdgeNum++;

        if(!ivex_edge_last)
            VertexList[ivex]->FirstEdge = edge_new;
        else
            if(ivex_edge_last->ivex == ivex)
                ivex_edge_last->ilink = edge_new;
            else
                ivex_edge_last->jlink = edge_new;
        if(!jvex_edge_last)
            VertexList[jvex]->FirstEdge = edge_new;
        else
            if(jvex_edge_last->jvex == jvex)
                jvex_edge_last->jlink = edge_new;
            else
                jvex_edge_last->ilink = edge_new;

        return true;
    }

    int LocateVertex(VertexPtr vertex_ptr) const{
        if(VertexNum == 0) return FAILED;
        for(int i = 0;i < VertexNum;i++){
            if(VertexList[i] == vertex_ptr)
                return i;
        }
        return FAILED;
    }

    void DestroyGraph(){
        for(int i = VertexNum - 1 ; i >= 0 ; i--){
            DeleteVertex(i);
        }
    }

    inline T GetVertexData(int VertexIndex) const throw(std::logic_error){
        if(VertexNum == 0 || VertexIndex >= VertexNum)
            throw std::out_of_range("Invalid VertexIndex");
        return VertexList[VertexIndex]->data;
    }

    inline void ModifyVertexData(int VertexIndex,const T &data){
        VertexList[VertexIndex]->data = data;
    }

    VertexPtr FirstAdjVex(int VertexIndex) const{
        if(VertexIndex >= VertexNum || VertexNum == 0) return nullptr;
        if(!FirstEdge(VertexIndex)) return nullptr;

        int ret_vex_index = FirstEdge(VertexIndex)->ivex;
        if(ret_vex_index == VertexIndex)
            ret_vex_index = FirstEdge(VertexIndex)->jvex;
        else
            ret_vex_index = FirstEdge(VertexIndex)->ivex;
        return VertexList[ret_vex_index];
    }

    VertexPtr NextAdjVex(int VertexIndex, VertexPtr prior_vex) const {
        EdgePtr edge = FirstEdge(VertexIndex);
        if(!edge) return nullptr;
        VertexPtr ret_vex_ptr = FirstAdjVex(VertexIndex);
        if(!prior_vex) return ret_vex_ptr;

        while(ret_vex_ptr != prior_vex) {
            edge = NextEdge(VertexIndex, edge);
            if(!edge) return nullptr;
            ret_vex_ptr = (edge->ivex == VertexIndex) ? VertexList[edge->jvex]: VertexList[edge->ivex];
        }

        edge = NextEdge(VertexIndex,edge);
        if(!edge) return nullptr;
        ret_vex_ptr = (edge->ivex == VertexIndex) ? VertexList[edge->jvex] : VertexList[edge->ivex];
        return ret_vex_ptr;
    }

    void DeleteEdge(int ivex,int jvex){
        EdgePtr ilink_succ,jlink_succ;
        EdgePtr edge = GetEdge(ivex,jvex);
        if(!edge) return;
        EdgePtr edge_prior = FirstEdge(ivex);
        if(edge->ivex == ivex){
            ilink_succ = edge->ilink;
            jlink_succ = edge->jlink;
        }
        else{
            ilink_succ = edge->jlink;
            jlink_succ = edge->ilink;
        }

        if(FirstEdge(ivex) == edge && FirstEdge(jvex) == edge){
            VertexList[ivex]->FirstEdge = NextEdge(ivex,edge);
            VertexList[jvex]->FirstEdge = NextEdge(jvex,edge);
            delete edge;
            return;
        }
        else if(FirstEdge(ivex) == edge) {
            VertexList[ivex]->FirstEdge = NextEdge(ivex, edge);
            delete edge;
            return;
        }
        else if(FirstEdge(jvex) == edge) {
            VertexList[jvex]->FirstEdge = NextEdge(jvex, edge);
            delete edge;
            return;
        }

        while(edge_prior->ilink != edge && edge_prior->jlink != edge)
            edge_prior = NextEdge(ivex,edge_prior);

        if(edge_prior->ilink == edge)
            edge_prior->ilink = ilink_succ;
        else
            edge_prior->jlink = jlink_succ;

        delete edge;
        return;
    }

    void DeleteVertex(int VertexIndex){
        if(VertexNum == 0 || VertexIndex >= VertexNum) return;
        VertexPtr vertex_ptr = VertexList[VertexIndex];
        for(int i = 0;i < VertexNum; i++){
            DeleteEdge(VertexIndex,i);
        }
        typename std::vector<VertexPtr>::iterator p = VertexList.begin() + VertexIndex;
        VertexList.erase(p);
        VertexNum--;
        delete vertex_ptr;
    }

public: // Auxiliary functions

    std::vector<T> DFS() {
        std::stack<VertexPtr> dfs_stack;
        std::vector<T> dfs_ret;
        VertexPtr dfs_vex_ptr;
        int vex_index;
        Unvisit_all_vex();
        for(int i=0;i<VertexNum;i++){
            dfs_vex_ptr = VertexList[i];
            if(NOTVISITED(dfs_vex_ptr)){
                dfs_stack.push(dfs_vex_ptr);
                while(!dfs_stack.empty()){
                    dfs_vex_ptr = dfs_stack.top();
                    dfs_stack.pop();
                    dfs_vex_ptr->mark = VISITED;
                    dfs_ret.push_back(dfs_vex_ptr->data);
                    vex_index = LocateVertex(dfs_vex_ptr);
                    dfs_vex_ptr = FirstAdjVex(vex_index);
                    while(dfs_vex_ptr) {
                        if (NOTVISITED(dfs_vex_ptr)) {
                            dfs_stack.push(dfs_vex_ptr);
                            break;
                        }
                        dfs_vex_ptr = NextAdjVex(vex_index, dfs_vex_ptr);
                    }
                }
            }
        }
        return dfs_ret;
    }

    std::vector<T> BFS() {
        std::deque<VertexPtr> bfs_deque;
        std::vector<T> bfs_ret;
        Unvisit_all_vex();
        VertexPtr bfs_vex_ptr;
        int vex_index;
        for(int i=0;i<VertexNum;i++){
            bfs_vex_ptr = VertexList[i];
            if(NOTVISITED(bfs_vex_ptr)){
                bfs_deque.push_back(bfs_vex_ptr);
                while(!bfs_deque.empty()){
                    bfs_vex_ptr = bfs_deque.front();
                    bfs_deque.pop_front();
                    vex_index = LocateVertex(bfs_vex_ptr);
                    if(NOTVISITED(bfs_vex_ptr)){
                        bfs_vex_ptr->mark = VISITED;
                        bfs_ret.push_back(bfs_vex_ptr->data);
                    }
                    bfs_vex_ptr = FirstAdjVex(vex_index);
                    while(bfs_vex_ptr) {
                        if (NOTVISITED(bfs_vex_ptr))
                            bfs_deque.push_back(bfs_vex_ptr);
                        bfs_vex_ptr = NextAdjVex(vex_index, bfs_vex_ptr);
                    }
                }
            }
        }
        return bfs_ret;
    }
};

#endif  // UDGRAPH_H_INCLUDED