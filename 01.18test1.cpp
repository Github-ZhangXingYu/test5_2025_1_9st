//
// Created by 15411 on 2025/1/18.
//

#include <bits/stdc++.h>
#define endl '\n'
using namespace std;
typedef pair<int, int> PII;
using i64 = long long;
const int g = 3, mod = 1e9 + 7, INF = 0x3f3f3f3f;
const int N = 500010, M = N << 1;

int n, m, k;

vector<PII> e[N];
int arrive_time[N];//到达i号点的时间
int product_vis[N];//是否到达过
int idx = 0;
map<string, int> pid;
map<int, string> idp;
vector<string> product[N]; //产品i由什么材料组成
int start_time[N]; //产品i在start时间开始生产
int pre[N]; //i号点从哪里过来

void dijkstra(int id){ //产品编号id
    for(int i = 1; i <= n; ++ i) {
        arrive_time[i] = INF;
    }
    priority_queue<PII, vector<PII>, greater<PII> > q;
    arrive_time[id] = 0;
    q.push({arrive_time[id], id});
    while(!q.empty()){
        int t = q.top().second;
        q.pop();
        if(product_vis[t] == id) continue;
        product_vis[t] = id;
        for(auto [j, w] : e[t]){
            if(arrive_time[j] > arrive_time[t] + w) {
                arrive_time[j] = arrive_time[t] + w;
                pre[j] = t;
                q.push({arrive_time[j], j});
            }
        }
    }
}

void get_time(int x){
    int num = 0;
    while(x < 0) {
        num ++;
        x += 86400;
    }
    string _ans = "";
    if(num) {
//        cout << num << "天前的";
        _ans = to_string(num) + "天前的";
    }
    else {
//        cout << "当天的";
        _ans = "当天的";
    }
    int hours = x / 3600, minutes = (x % 3600) / 60, seconds = (x % 60);
//    cout << hours << "时" << minutes << "分" << seconds << "秒";
    _ans += to_string(hours) + "时" + to_string(minutes) + "分" + to_string(seconds) + "秒";
    //这个_ans需要输出
    cout << _ans;
}

void get(int &a, string &s){
    if(pid.find(s) != pid.end()) a = pid[s];
    else {
        a = pid[s] = ++ idx;
        idp[a] = s;
    }
}

void out(int u, int st_time){
    int v;
    for(auto &material_j : product[u]){
        get(v, material_j);
        string _ans = "材料" + material_j + "需要最晚在";
//        _ans需要输出
        cout << _ans;
//        cout << "材料" << material_j << "需要最晚在";
        get_time(st_time - arrive_time[v]);
//        cout << "开始向" << idp[u] << "配送" << endl;
        _ans = "开始向" + idp[u] + "配送\n";
        //_ans需要输出
        cout << _ans;
    }
}

void out2(int u){ //路径
    int v;
    for(auto &material_j : product[u]){
        get(v, material_j);
        string _ans = "产品" + material_j + "的路径";
//        cout << "产品" << material_j << "的路径" << endl;
        while(v != u) {
            _ans += " " + to_string(v);
//            cout << v << ' ';
            v = pre[v];
        }
        _ans += " " + to_string(v) + '\n';
        //_ans需要输出
        cout << _ans;
    }
}

void solve(){
    cin >> n; //材料和产品总的种类数
    string _name;
    for(int _id, i = 1; i <= n; ++ i){
        cin >> _name;
        get(_id, _name);
    }
    cin >> m; //m种产品
    vector<string> m_pro;
    for(int i = 1, u, v; i <= m; ++ i){
        int craft_num, need_times; //当前产品由多少种其他的产品或者材料组成, 需要的时间
        string cailiao, chanpin;
        int minute, hour;
        char $_;
        cin >> hour >> $_ >> minute;
        int _st_time = 3600 * hour + 60 * minute;
        cin >> chanpin >> craft_num;
        get(u, chanpin); //得到产品的id
        start_time[u] = _st_time;
        m_pro.push_back(chanpin);
        //起点 终点 运送时间
        while(craft_num --){
            cin >> cailiao >> need_times;
            get(v, cailiao); //得到材料的id
            product[u].push_back(cailiao);
            e[u].push_back({v, need_times});
        }
    }

    for(auto _chanpin : m_pro){
        dijkstra(pid[_chanpin]);
        int u, v;
        get(u, _chanpin); //得到产品的编号
        string _ans = "产品" + _chanpin + "在";
//        cout << "产品" << _chanpin << "在";
//        _ans需要输出;
        cout << _ans;
        get_time(start_time[u]);
        _ans = "时间制作，需要的材料及其时间:\n";
//        _ans需要输出;
        cout << _ans;
//        cout << "时间制作，需要的材料及其时间:\n";
        out(u, start_time[u]);
        _ans = "\n产品路径";
        //_ans需要输出;
        cout << _ans;
//        cout << "\n产品路径" << endl;
        vector<int> road;
        int _t = pid[_chanpin];
        out2(_t);
//        cout << endl;
        _ans = '\n';
        //_ans需要输出
        cout << _ans;
    }
//    for(auto x : product[pid["phone"]])
//        cout << x << endl;
//    cout << endl;
//    for(auto x : product[pid["xinpian"]])
//        cout << x << endl;
}

signed main() {
#ifdef ONLINE_JUDGE
#else
    freopen("test.in", "r", stdin);
    freopen("test.out", "w", stdout);
#endif
    ios::sync_with_stdio(false);
    cin.tie(0), cout.tie(0);
    solve();
    return 0;
}
