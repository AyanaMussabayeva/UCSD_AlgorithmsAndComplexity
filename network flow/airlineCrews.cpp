#include <iostream>
#include <string.h>
#include <vector>
using namespace std;

#define MAXN 100                
int nx,ny;                                      
int graphMatrix[MAXN][MAXN];               
int cx[MAXN],cy[MAXN];        
                                                        
//bipartite graph solution

int mk[MAXN];


int path(int u,vector<vector<bool> > graphMatrix){
    
    for(int v=0;v<ny;++v){     
        if(graphMatrix[u][v] && !mk[v]){     
            mk[v]=1;                      

           
            if(cy[v]==-1 || path(cy[v],graphMatrix)){
                cx[u]=v;         
                cy[v]=u;            
                return 1;
            }
        }
    }
    return 0;                        
}

int maxMatch(vector<vector<bool> > graphMatrix){       

    int res=0;
    memset(cx,-1,sizeof(cx));        
    memset(cy,-1,sizeof(cy));
    for(int i = 0;i < nx; ++i){
        if(cx[i]==-1){                                  
            memset(mk,0,sizeof(mk));
            res+=path(i,graphMatrix);
        }
    }
    return res;
}

vector<vector<bool> > ReadData() {
    int n, m;
    cin >> n >> m;
    vector<vector<bool> > graphMatrix(n, vector<bool>(m));
    for (int i = 0; i < n; ++i)
      for (int j = 0; j < m; ++j) {
        int bit;
        cin >> bit;
        graphMatrix[i][j] = (bit == 1);
      }
    return graphMatrix;
}
int main() {
    //input graph 
    vector<vector<bool> > graphMatrix = ReadData();
    nx = graphMatrix.size();
    ny = graphMatrix[0].size();
    int num= maxMatch(graphMatrix);
    //cout<<"num="<<num<<endl;
    for(int num = 0; num < nx; ++num){
        if (cx[num]+1)
        {
            cout<<cx[num]+1<<' ';
        }
        else
        {
            cout<<-1<<' ';
        }
    }

    return 0;
}