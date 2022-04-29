#include <iostream>
#include <deque>
#include <vector>

using namespace std;
deque<deque<int>> map(100,deque<int>(0));
int N, K, w=0, h=0;
int dx[4] = {-1,1,0,0}, dy[4] = {0,0,-1,1};


void input(){
    cin >> N >> K;
    for(int i=0; i<N; i++){
        int a;
        cin >> a;
        map[0].push_back(a);
    }
}

void debug(){
    for(int i=5; i>=0; i--){
        for(int j=0; j<map[i].size(); j++){
            cout << map[i][j] << " ";
        }cout << "\n";
    }cout << "\n";
}

void addFish(){
    vector<int> addList {0};
    
    for(int i=1; i<map[0].size(); i++){
        if(map[0][i] < map[0][addList[0]]){
            addList.clear();
            addList.push_back(i);
        }else if(map[0][i] == map[0][addList[0]]){
            addList.push_back(i);
        }
    }
    for(int i=0; i<addList.size(); i++){
        int idx = addList[i];
        map[0][idx]++;
    }
}

bool spin(){
    if(h+1 > map[0].size()-w-1) return false;
    for(int i=h; i>=0; i--){
        for(int j=0; j<=w; j++){
            int movingCell;
            if(i==0) {
                movingCell = map[i][w-j];
                map[i].erase(map[i].begin()+w-j);
            }else{
                movingCell = map[i].back();
                map[i].pop_back();
            }
            map[1+j].push_front(movingCell);
        }
    }
    int tmp = h;
    h = w+1;
    w = tmp;
    return true;
}

bool validation(int x, int y){
    if(x>=0 && x<=h && y>=0 && y<map[x].size())
        return true;
    
    return false;
}

void controlNumber(){
    deque<deque<int>> store(h+1, deque<int>(map[0].size()));
    for(int i=0; i<=h; i++){
        for(int j=0; j<map[i].size(); j++){
            
            int sum = 0;
            for(int dir=0; dir<4; dir++){
                int x = i + dx[dir];
                int y = j + dy[dir];
                if(!validation(x, y)) continue;
                
                sum += (map[x][y]-map[i][j])/5;
            }
            store[i][j] = sum;
        }
    }
    
    for(int i=0; i<=h; i++){
        for(int j=0; j<map[i].size(); j++){
            map[i][j] += store[i][j];
        }
    }
}

void reLocate(){
    deque<deque<int>> newMap(100,deque<int>(0));
    for(int j=0; j<map[0].size(); j++){
        for(int i=0; i<=h; i++){
            if(!validation(i, j)) continue;
            newMap[0].push_back(map[i][j]);
        }
    }
    map.swap(newMap);
    h = 0;
}

void spinHalf(){
    for(int i=0; i< N/2; i++){
        int a = map[0].front();
        map[0].pop_front();
        map[1].push_front(a);
    }

    h = 1;

    for(int j=1; j>=0; j--){
        for(int i=0; i< N/4; i++){
            int a = map[j].front();
            map[j].pop_front();
            map[h+2-j].push_front(a);
        }
    }
    h = 3;
}

void reLocateHalf(){
    deque<deque<int>> newMap(100,deque<int>(0));
    for(int j=0; j<N/4; j++){
        for(int i=0; i<=h; i++){
            if(!validation(i, j)) continue;
            newMap[0].push_back(map[i][j]);
        }
    }
    map.swap(newMap);
    h = 0;
}

bool checkFinish(){
    int min_val = 987654321;
    int max_val = 0;
    for(int i=0; i<map[0].size(); i++){
        if(map[0][i]>max_val) max_val = map[0][i];
        if(map[0][i]<min_val) min_val = map[0][i];
    }
    
    if(max_val - min_val <= K) return true;
    return false;
}

void solution(){
    bool isFinished = checkFinish();
    int answer = 0;
    while(!isFinished){
        answer++;
        w=0; h=0;
        addFish();
        while(spin());
        controlNumber();
        reLocate();
        spinHalf();
        controlNumber();
        reLocateHalf();
        isFinished = checkFinish();
        
    }
    cout << answer ;
}


int main(){
    input();
    solution();
}
