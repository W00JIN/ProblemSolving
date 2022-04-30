
#include <iostream>
#include <vector>

using namespace std;

struct Attack{
    int d;
    int s;
};

struct Explosion{
    int idx;
    int cnt;
};

int answer=0, N, M, cntOne=0, cntTwo=0, cntThree=0;

int makeMapDx[4] = {0,1,0,-1}; //좌, 하, 우, 상
int makeMapDy[4] = {-1,0,1,0}; //좌, 하, 우, 상

int dx[5] = {0,-1,1,0,0}; // 상, 하, 좌, 우
int dy[5] = {0,0,0,-1,1}; // 상, 하, 좌, 우

vector<vector<int>> map(101, vector<int>(101));
vector<Attack> attacks;
vector<int> balls;

bool validation(int x, int y){
    if(x>0 && x<=N && y>0 && y<=N) return true;
    return false;
}

void debug(){
    for(int i=1; i<=N; i++){
        for(int j=1; j<=N; j++){
            if(i==(N+1)/2 && j==(N+1)/2) cout << 0 << " ";
            else if(balls.size() > map[i][j])
                cout << balls[map[i][j]] << " ";
            else cout << 0 << " ";
        }
        cout << endl;
    }cout << endl;
}

void makeList(){
    vector<vector<int>> newMap(101, vector<int>(101, -1));
    
    int x = (N+1)/2;
    int y = (N+1)/2;
    int nx, ny;
    int cnt = 0;
    
    newMap[x][y] = 0;
    
    while(newMap[1][1]==-1){
        for(int i=0; i<4; i++){
            if(x==1 && y==1) break;
            do{
                nx = x + makeMapDx[i];
                ny = y + makeMapDy[i];
                
                if(!validation(nx,ny)) break;
                
                x = nx;
                y = ny;
                
                newMap[x][y] = cnt++;
                if(map[x][y]>=1 && map[x][y]<=3) balls.push_back(map[x][y]);
                
                nx = x + makeMapDx[(i+1)%4];
                ny = y + makeMapDy[(i+1)%4];
                
            } while(validation(nx,ny) && newMap[nx][ny] !=-1);
        }
    }
    map.swap(newMap);
}

void input(){
    cin >> N >> M;
    
    for(int i=1; i<=N; i++){
        for(int j=1; j<=N; j++){
            cin >> map[i][j];
        }
    }
    
    for(int i=0; i<M; i++){
        int d, s;
        cin >> d >> s;
        attacks.push_back({d,s});
    }
    makeList();
}
void attackBalls(Attack attack){
    vector<int> deleteList;
    
    int x = (N+1)/2;
    int y = (N+1)/2;
    
    for(int i=0; i<attack.s; i++){
        int nx = x + dx[attack.d];
        int ny = y + dy[attack.d];
        
        if(!validation(nx,ny)) break;
        
        x = nx;
        y = ny;
        
        deleteList.push_back(map[x][y]);
    }
    
    vector<int> newBalls;
    
    for(int i=0; i<balls.size(); i++){
        bool isDeleted = false;
        for(int j=0; j<deleteList.size(); j++){
            if(i == deleteList[j]) {
                isDeleted = true;
                break;
            }
        }
        if(isDeleted) continue;
        newBalls.push_back(balls[i]);
    }
    balls.swap(newBalls);
}

bool explodeBalls(){
    
    vector<int> deleteList;
    if(balls.size()<4) return false;
    
    int idx = -1;
    int cnt = 0;

    for(int i=0; i<balls.size()-1; i++){
        if(balls[i] == balls[i+1]){
            if(idx == -1){
                idx = i;
                cnt++;
            }
            cnt++;
        }else{
            if(cnt>=4){
                for(int k=cnt-1; k>=0; k--){
                    deleteList.push_back(idx+k);
                }
            }
            idx = -1;
            cnt = 0;
        }
    }
    if(idx!=-1 && cnt>=4){
        for(int k=cnt-1; k>=0; k--){
            deleteList.push_back(idx+k);
        }
    }
    if(deleteList.size() == 0) return false;
    
    vector<int> newBalls;
    
    for(int i=0; i<balls.size(); i++){
        bool isDeleted = false;
        for(int j=0; j<deleteList.size(); j++){
            if(i == deleteList[j]) {
                if(balls[i] == 1){
                    cntOne++;
                }else if(balls[i]==2){
                    cntTwo++;
                }else if(balls[i]==3){
                    cntThree++;
                }
                isDeleted = true;
                break;
            }
        }
        if(isDeleted) continue;
        newBalls.push_back(balls[i]);
    }
    
    balls.swap(newBalls);
    return true;
}

void reLocate(){
    
    vector<int> newBalls;
    
    int ballType = 0;
    int cnt = 1;
    
    if(balls.size() == 0) return;
    
    for(int i=0; i<balls.size()-1; i++){
        if(balls[i] == balls[i+1]){
            if(ballType == 0){
                ballType = balls[i];
            }
            cnt++;
        }else{
            if(ballType == 0) {
                
                newBalls.push_back(cnt);
                newBalls.push_back(balls[i]);
            }else{
                
                newBalls.push_back(cnt);
                newBalls.push_back(ballType);
                
                ballType = 0;
                cnt = 1;
            }
        }
        if(newBalls.size()>N*N-3) break;
    }
    
    if(newBalls.size()<=N*N-3){
        if(ballType == 0) {
            
            newBalls.push_back(cnt);
            newBalls.push_back(balls[balls.size()-1]);
        }else{
            
            newBalls.push_back(cnt);
            newBalls.push_back(ballType);
        }
    }
    balls.swap(newBalls);
}

void solve(){
    for(int i=0; i<M; i++){
        attackBalls(attacks[i]);
        while(explodeBalls());
        reLocate();
        
//        for(int i=0; i<balls.size(); i++)
//            cout << balls[i] << " ";
//        cout << endl << endl;

//        debug();
    }
    answer = cntOne + 2*cntTwo + 3*cntThree;
    
    
//    cout << "cntOne : " << cntOne << endl <<"cntTwo : " << cntTwo << endl <<"cntThree : " << cntThree << endl;
    
    cout << answer;
}

int main(){
    input();
    solve();
}
