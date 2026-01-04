//
// Created by 15411 on 2025/1/18.
//模版示例，最好用namespace封装一下，我怕变量名字重复
#include<bits/stdc++.h>

//下面是全局变量，用来和我的代码传送数据
std::string Item_name, Start_time, Arrival_time, Spend_time, Path;//存放项目名字，起始时间，到达时间，路上花费时间，路径
//其中路径Path里存放 1->2->5->6这样的格式存放路径

int ways;//存放走过的边的数量
int path[500010];//存放边的标号，一共ways条，如边1，4,5这样子存放

namespace path_algorithm {
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
    }

    //下面的函数需要实现
    void Add_edge(int u_id, int v_id, std::string u_name, std::string v_name, int id, int time) {//这里还需要一个填单项边u->v的函数，边的id
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

    void Find(std::string Origin_name, std::string End_name) {//计算最短路,我会传入起点和终点
        //计算完成后把答案存入全局变量Start_time,Spend_time,Path；ways，path；
        //我后面直接调用这部数据
        int origin_id = pid[Origin_name], end_id = pid[End_name];
        dijkstra(origin_id);
        if (arrive_time[end_id] == INF) {
            Spend_time = Start_time = Path = "无法抵达";
            return;
        }
        Spend_time = get_string_by_int(arrive_time[end_id], 0);
        int infact_time = get_int_by_string(Arrival_time);
        std::cout << infact_time << ' ' << arrive_time[end_id] << '\n';
        Start_time = get_string_by_int(infact_time - arrive_time[end_id], 1);
        std::vector<int> path_point;
        path_point.push_back(end_id);
        ways = 0;
        while (end_id != origin_id) {
            ++ways;
            path[ways] = pre_edge[end_id];
            end_id = pre_point[end_id];
            path_point.push_back(end_id);
        }
        std::reverse(path_point.begin(), path_point.end());
        Path = "路径:";
        for (int i = 0; i < path_point.size(); ++i) {
            if (i) Path += "->";
            Path += idp[path_point[i]];
        }
        std::reverse(path + 1, path + ways + 1);
//        for(int i = 1; i <= 3; ++ i)
//            std::cout << arrive_time[i] << '\n';
    }
}

/*后续优化：可以考虑给查询进行按起点排序，跑过的最短路可以想方法存起来，以后直接调用之类的*/

signed main() {
#ifdef ONLINE_JUDGE
#else
    freopen("902.in", "r", stdin);
    freopen("902.out", "w", stdout);
#endif
    Arrival_time = "10:40:30";
    path_algorithm::Add_edge(2, 3, "bus_step", "school", 2, 24 * 3600 + 5);
    path_algorithm::Add_edge(1, 2, "home", "bus_step", 1, 2430);
    path_algorithm::Find("home", "school");
    std::cout << Path << std::endl;
    std::cout << Start_time << std::endl;
    std::cout << Spend_time << std::endl;
    for (int i = 1; i <= ways; ++i)
        std::cout << path[i] << " \n"[i == ways];
    return 0;
}
