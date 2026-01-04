//
// Created by 15411 on 2025/1/9.
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

struct Material{ //材料
    int name; //材料的名字
    int count; //需要的数量
    int to; //去哪个产品
    int times; //这个位置最晚到达时间
};

struct Product{ //产品
    vector<Material> v; //需要的材料
    int st_time; //产品制作开始的时间
} products[N], product_ans[N];

int arrive_time[N];//到达i号点的时间
int product_vis[N];//是否到达过

void dijkstra(int id){ //产品编号id
    for(int i = 1; i <= n + m + k; ++ i) {
        arrive_time[i] = INF;
        if(i > n) {
            products[i].v.clear();
            products[i].st_time = INF;
        }
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
    if(num) cout << num << "天前的";
    else cout << "当天的";
    int hours = x / 3600, minutes = (x % 3600) / 60, seconds = (x % 60);
    cout << hours << "时" << minutes << "分" << seconds << "秒";
}

void solve() {
    cin >> n >> m >> k; //产品的种类 材料的种类 货物可以运送过去的地点数量
    /*
        如果前面的产品作为后面的材料,那么前面的产品也要算作材料,后面需要将当前产品口和物料口连接，运送时间为0
        产品口编号为1-n
        材料编号为n+1 - n+m
        其他点就是n+m+1 - n+m+k
     * */
    for(int i = 1, num; i <= n; ++ i){ //第i个产品
        int minute, hour;
        char $_not;
        cin >> hour >> $_not >> minute; //24小时制， 开始制造的时间
        products[i].st_time = hour * 3600 + 60 * minute;
        cin >> num; //需要的材料数量
        for(int j = 0; j < num; ++ j){
            Material material_j;
            cin >> material_j.name >> material_j.count; //材料名字和需要数量
            material_j.times = products[i].st_time; //材料最晚送达时间是开始制作的时间
            material_j.to = i; //送往i号产品口
            products[i].v.push_back(material_j);
        }
    }

    int edge_num, u, v, need_time; //需要的时间
    cin >> edge_num; //所有可以相互传输的路径条数
    while(edge_num --){ //u，v相互到达需要的时间
        cin >> u >> v >> need_time;
        e[u].push_back({v, need_time});
        e[v].push_back({u, need_time});
    }

    for(int i = 1; i <= n; ++ i){
        dijkstra(i);
        product_ans[i].v.clear();
        for(auto &material_j : products[i].v){ //枚举所有需要的材料
            Material material_k = material_j;
            material_k.times -= arrive_time[n + material_k.name];
            product_ans[i].v.push_back(material_k);
        }
        sort(product_ans[i].v.begin(), product_ans[i].v.end(), [&](auto x, auto y){
            return x.name < y.name;
        });
        cout << "产品" << i << "在";
        get_time(products[i].st_time);
        cout << "时间制作，需要的材料及其时间:\n";
        for(auto &material_j : product_ans[i].v){
            cout << "材料" << material_j.name << "需要" << material_j.count << "个，最晚在";
            get_time(material_j.times);
            cout << "开始配送" << endl;
        }
        cout << endl;
    }
}

signed main() {
#ifdef ONLINE_JUDGE
#else
    freopen("test.in", "r", stdin);
    freopen("test.out", "w", stdout);
#endif
    ios::sync_with_stdio(false);//cin.tie(0);cout.tie(0);
//    ll _=1;cin>>_;while(_--)
        solve();
    return 0;
}
/*2 2 1
10:05 1
1 2
14:20 2
2 6
1 3
4
1 3 600
3 2 3600
2 5 1200
5 4 600*/