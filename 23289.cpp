#include <iostream>
#include <vector>
#include <cstring>
#include <algorithm>
#include <queue>

using namespace std;

struct Cell{
    int heater = 0, degree = 0;
    vector<int> wall {0};
};
struct Wind{
    int x, y, depth;
};
// wall   : 1-좌 2-우 3-하 4-상
// heater : 1-우 2-좌 3-상 4-하

int dx[2][5] = {{ 0, 0, 0, -1, 1}, {0, 1, 1, 0, 0}},dy[2][5] = {{ 0, 1, -1, 0, 0}, {0, 0, 0, 1, 1}};
int diagonal[5][2]= {{0,0}, {4,3}, {4,3}, {1,2}, {1,2}};

vector<vector<Cell>> map(21, vector<Cell>(21,{0,0,{0}}));

vector<pair<int,int>> searchList;
vector<pair<int,int>> heaterList;
int n, m, k, answer = 0;

void debug(){
    for(int i=0; i<n; i++){
        for(int j=0; j<m; j++){
            cout << map[i][j].degree << " ";
        }cout << "\n";
    }cout << "\n";
}

void input(){
    cin >> n >> m >> k;
    for(int i=0; i<n; i++){
        for(int j=0; j<m; j++){
            int param;
            cin >> param;

            if(param==5){
                searchList.push_back({i,j});
            }else if(param > 0){
                heaterList.push_back({i,j});
                map[i][j].heater = param;
            }
        }
    }
    int wall;
    cin >> wall;
    
    for(int i=0; i<wall; i++){
        int x, y, dir;
        cin >> x >> y >> dir;
        
        if(dir){
            if(y < m) map[x-1][y].wall.push_back(1);
            map[x-1][y-1].wall.push_back(2);
            
        }else{
            if(x-2 >= 0) map[x-2][y-1].wall.push_back(3);
            map[x-1][y-1].wall.push_back(4);
        }
    }
}

void output(){
    cout << answer;
}
bool validation(int x, int y, int heaterDir){
    if(x>=0 && x<n && y>=0 && y<m){
        for(int i=0; i<map[x][y].wall.size(); i++){
            if(map[x][y].wall[i] == heaterDir) return false;
        }
        return true;
    }
    return false;
}

void runHeater(){
    for(int i=0; i<heaterList.size(); i++){
        vector<vector<Cell>> addDegree(21, vector<Cell>(21,{0,0,{0}}));
        queue<Wind> q;
        int start_x = heaterList[i].first;
        int start_y = heaterList[i].second;
        
        int dir = map[start_x][start_y].heater;
        int start_nx = start_x + dx[0][dir];
        int start_ny = start_y + dy[0][dir];
        
        if(validation(start_nx,start_ny,dir)){
            addDegree[start_nx][start_ny].degree = 5;
            q.push({start_nx,start_ny, 4});
        }
        
        while(!q.empty()){
            int x = q.front().x;
            int y = q.front().y;
            int depth = q.front().depth;
            q.pop();
            
            int nx = x + dx[0][dir];
            int ny = y + dy[0][dir];
            
            int nx1 = nx - dx[1][dir];
            int ny1 = ny - dy[1][dir];
            
            int nx2 = nx + dx[1][dir];
            int ny2 = ny + dy[1][dir];
            
            if(validation(x,y,diagonal[dir][0]) && validation(nx1,ny1,dir) && !addDegree[nx1][ny1].degree) {
                addDegree[nx1][ny1].degree += depth;
                if(depth > 1) q.push({nx1,ny1,depth-1});
            }
            
            if(validation(nx,ny,dir) && !addDegree[nx][ny].degree) {
                addDegree[nx][ny].degree += depth;
                if(depth > 1) q.push({nx,ny,depth-1});
            }
            
            if(validation(x,y,diagonal[dir][1]) && validation(nx2,ny2,dir) && !addDegree[nx2][ny2].degree) {
                addDegree[nx2][ny2].degree += depth;
                if(depth > 1) q.push({nx2,ny2,depth-1});
            }
        }
        
        for(int i=0; i<n; i++){
            for(int j=0; j<m; j++){
                map[i][j].degree += addDegree[i][j].degree;
            }
        }
    }
}

void controlDegree(){
    vector<vector<Cell>> control = map;
    for(int i=0; i<n; i++){
        for(int j=0; j<m; j++){
            control[i][j].degree = 0;
        }
    }
    
    for(int x=0; x<n; x++){
        for(int y=0; y<m; y++){
            int sum = 0;
            for(int i=1; i<=4; i++){
                int nx = x + dx[0][i];
                int ny = y + dy[0][i];
                
                if(validation(nx, ny, i)){
                    int calculation = (map[x][y].degree - map[nx][ny].degree)/4;
                    sum += calculation;
                }
            }
            control[x][y].degree = map[x][y].degree - sum;
        }
    }
    
    for(int i=0; i<n; i++){
        if(control[i][0].degree > 0) control[i][0].degree--;
        if(control[i][m-1].degree > 0)control[i][m-1].degree--;
    }
    for(int i=1; i<m-1; i++){
        if(control[0][i].degree > 0) control[0][i].degree--;
        if(control[n-1][i].degree > 0) control[n-1][i].degree--;
    }
    
    map = control;
}

bool isFinished(){
    for(int i=0; i<searchList.size(); i++){
        int x = searchList[i].first;
        int y = searchList[i].second;
        if(map[x][y].degree < k) return false;
    }
    return true;
}

void solution(){
    bool finish = false;
    while(answer<=100 && !finish){
        runHeater();
        controlDegree();
        answer++;
        finish = isFinished();
    }
}

int main(){
    input();
    solution();
    output();
}
