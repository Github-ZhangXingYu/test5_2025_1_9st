#include "widget.h"
#include "ui_widget.h"
#include "task.h"
#include<iostream>
#include<map>
#include<vector>
#include<queue>

int ways;//存放走过的边的数量
int path[500010];//存放边的标号，一共ways条，如边1，4,5这样子存放
std::string Item_name, Start_time, Arrival_time, Spend_time, Path;//存放项目名字，起始时间，到达时间，路上花费时间，路径
//    其中路径Path里存放 1->2->5->6这样的格式存放路径
const int N = 500010, INF = 0x3f3f3f3f, day = 24 * 60 * 60;
typedef std::pair<int, int> PII;
struct Edge {
    int v, time, id; //去哪里，
    Edge(int a, int b, int c) {
        v = a;
        time = b;
        id = c;
    }
};
std::vector<Edge> e[N];
std::map<std::string, int> pid; //名字对应编号
std::map<int, std::string> idp; //编号对应名字
int arrive_time[N], pre_point[N], pre_edge[N], mxn = 0;
int product_vis[N]; //是否到达
void clear() {
    pid.clear();
    idp.clear();
    for (int i = 1; i <= mxn; ++i) {
        e[i].clear();
        product_vis[i] = -1;
    }
    mxn = 0;
}

//添加点
void Add_point(int u_id, std::string u_name){
    pid[u_name] = u_id, idp[u_id] = u_name;
    mxn = std::max(mxn, u_id);
}

//下面的函数需要实现
void Add_edge(int u_id, int v_id, std::string u_name, std::string v_name, int id, int time) {//这里还需要一个填单项边u->v的函数，边的id
    //反向边
    std::swap(u_id, v_id);
    std::swap(u_name, v_name);
    mxn = std::max(mxn, u_id);
    mxn = std::max(mxn, v_id);
    Edge tmp = Edge(v_id, time, id);
    e[u_id].push_back(tmp);
    pid[u_name] = u_id, idp[u_id] = u_name;
    pid[v_name] = v_id, idp[v_id] = v_name;
}

void dijkstra(int $id) { //产品编号id
    for (int i = 1; i <= mxn; ++i) {
        arrive_time[i] = INF;
        product_vis[i] = -1;
    }
    std::priority_queue<PII, std::vector<PII>, std::greater<PII> > q;
    arrive_time[$id] = 0;
    q.push({arrive_time[$id], $id});
    while (!q.empty()) {
        int t = q.top().second;
        q.pop();
        if (product_vis[t] == $id) continue;
        product_vis[t] = $id;
        for (auto [j, w, id]: e[t]) {
            if (arrive_time[j] > arrive_time[t] + w) {
                arrive_time[j] = arrive_time[t] + w;
                pre_point[j] = t;
                pre_edge[j] = id;
                q.push({arrive_time[j], j});
            }
        }
    }
}

std::string get_string_by_int(int the_time, int op) { //op = 0表示只计算数字代表的时间， op = 1表示起始时间
    std::string res = "";
    if (op) {
        int day_num = 0;
        if (the_time < 0) {
            day_num = the_time / day;
            day_num++;
            the_time = (the_time % day + day) % day;
        }
        if (day_num) res = std::to_string(day_num) + "天前的";
        else res = "当天";
    }
    int hour = the_time / 3600, minute = (the_time) % 3600 / 60, second = the_time % 60;
    res += std::to_string(hour) + ":" + std::to_string(minute) + ":" + std::to_string(second);
    return res;
}

int get_int_by_string(std::string the_time) {
    int hour = 0, minute = 0, second = 0;
    for (int i = 0, j = 1; i < the_time.size(); ++i) {
        if (the_time[i] == ':') {
            ++ j;
            continue;
        }
        int now_num = the_time[i] - '0';
        if (j == 1) hour = hour * 10 + now_num;
        else if (j == 2) minute = minute * 10 + now_num;
        else if (j == 3) second = second * 10 + now_num;
    }
    return hour * 3600 + minute * 60 + second;
}

int get_second(std::string s) {
    int second = 0, pos = 0;
    while (s[pos] != ':') {
        second *= 10;
        second += s[pos] - '0';
        pos++;
    }
    pos++;
    second *= 60 * 60;
    int now = 0;
    while (pos < s.size() && s[pos] != ':') {
        now *= 10;
        now += s[pos] - '0';
        pos++;
    }
    second = now * 60 + second;
    if (std::count(s.begin(), s.end(), ':') == 2) {
        pos++;
        now = 0;
        while (pos < s.size()) {
            now *= 10;
            now += s[pos] - '0';
            pos++;
        }
        second += now;
    }
    return second;
}

void Find(std::string Origin_name, std::string End_name) {//计算最短路,我会传入起点和终点
    //计算完成后把答案存入全局变量Start_time,Spend_time,Path；ways，path；
    //我后面直接调用这部数据
    if((pid.find(Origin_name) == pid.end()) || (pid.find(End_name) == pid.end())){
        Spend_time = Start_time = Path = "无法抵达";
        ways=0;
        return;
    }
    int origin_id = pid[Origin_name], end_id = pid[End_name];
    dijkstra(end_id);
    if (arrive_time[origin_id] == INF || mxn == 0) {
        Spend_time = Start_time = Path = "无法抵达";
        ways=0;
        return;
    }
    Spend_time = get_string_by_int(arrive_time[origin_id], 0);
    int infact_time = get_int_by_string(Arrival_time);
//        std::cout << infact_time << ' ' << arrive_time[origin_id] << '\n';
    Start_time = get_string_by_int(infact_time - arrive_time[origin_id], 1);
    std::vector<int> path_point;
    path_point.push_back(origin_id);
    ways = 0;
    while (end_id != origin_id) {
        ++ ways;
        path[ways] = pre_edge[origin_id];
        origin_id = pre_point[origin_id];
        path_point.push_back(origin_id);
    }
//        std::reverse(path_point.begin(), path_point.end());
    Path = "路径:";
    for (int i = 0; i < path_point.size(); ++i) {
        if (i) Path += "->";
        Path += idp[path_point[i]];
    }
//        std::reverse(path + 1, path + ways + 1);
//        for(int i = 1; i <= 3; ++ i)
//            std::cout << arrive_time[i] << '\n';
}

std::string get_path_module(std::string Origin_name, std::string End_name, int &success, int &fact_time){
    if((pid.find(Origin_name) == pid.end()) || (pid.find(End_name) == pid.end())){
        success = 0;
        return "";
    }
    int origin_id = pid[Origin_name], end_id = pid[End_name];
    dijkstra(end_id);
    if (arrive_time[origin_id] == INF || mxn == 0) {
        success = 0;
        return "";
    }
    fact_time += arrive_time[origin_id]; //每一段路的时间
    std::vector<int> path_point;
    path_point.push_back(origin_id);
    while (end_id != origin_id) {
        ++ ways;
        path[ways] = pre_edge[origin_id];
        origin_id = pre_point[origin_id];
        path_point.push_back(origin_id);
    }
    std::string res = "";
    for (int i = 1; i < path_point.size(); ++i) { //起点已经有了 不需要加了
        res += "->";
        res += idp[path_point[i]];
    }
    return res;
}

void get_path(std::vector<std::string>& vv){ //路径
    int nsz = vv.size();
    if(nsz < 2) {
        Spend_time = Start_time = Path = "无法抵达";
        ways=0;
        return;
    }
    Path = vv[0];
    int success = 1, fact_time = 0;
    for(int i = 1; i < nsz; ++ i){
        Path += get_path_module(vv[i - 1], vv[i], success, fact_time);
        if(!success) { //无解了
            Spend_time = Start_time = Path = "无法抵达";
            ways=0;
            return;
        }
    }
    Spend_time = get_string_by_int(fact_time, 0);
    int infact_time = get_int_by_string(Arrival_time);
    Start_time = get_string_by_int(infact_time - fact_time, 1);
}

Task path_find(Task need){
    ways = 0;
    Task ans = need;
    Arrival_time = ans.Arrival_time;
    get_path(ans.Node_name);
    ans.Start_time = Start_time;
    ans.Spend_time = Spend_time;
    ans.Path = Path;
    ans.ways = ways;
    for(int i = 1; i <= ways; ++ i)
        ans.path[i] = path[i];
    return ans;
}


/*后续优化：可以考虑给查询进行按起点排序，跑过的最短路可以想方法存起来，以后直接调用之类的*/
