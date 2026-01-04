//
// Created by 15411 on 2025/3/2.
//
#include<bits/stdc++.h>
#include<fstream>
//下面是全局变量，用来和我的代码传送数据
//std::string Item_name, Start_time, Arrival_time, Spend_time, Path;//存放项目名字，起始时间，到达时间，路上花费时间，路径
//其中路径Path里存放 1->2->5->6这样的格式存放路径

class Task{
public:
    // 已知变量
    std::string Item_name,Arrival_time,Origin_name;//物品名称，到达时间，起点名称
    int Node_num;//要经过的点数量
    std::vector<std::string> Node_name;//经过的点
//    // 待求解变量
    std::string Start_time,Spend_time,Path;//开始时间，花费时间，路径1->2->3型
    int ways;//路径数量
    int path[20005];//存放边的标号，一共ways条
};

namespace path_algorithm {
    int all_path_point_cost = 0;
    std::vector<int> point_cost;
    int ways;//存放走过的边的数量
    int tot_points = 0, edge_id = 0;// 总共的点的数量, 边的数量
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
        point_cost.clear();
        tot_points = edge_id = 0;
        all_path_point_cost = 0;
        for (int i = 1; i <= mxn; ++i) {
            e[i].clear();
            product_vis[i] = -1;
        }
        mxn = 0;
    }

    void Add_point(std::string u_name, int cost){
        ++ tot_points;
        pid[u_name] = tot_points, idp[tot_points] = u_name;
        point_cost.push_back(cost);
        mxn = tot_points;
    }

    void Add_edge(std::string u_name, std::string v_name, int time) {//这里还需要一个填单项边u->v的函数，边的id
        ++ edge_id;
        int u_id = pid[u_name], v_id = pid[v_name];
        e[u_id].push_back(Edge(v_id, time, edge_id));
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

    std::string get_path_module(std::string Origin_name, std::string End_name, int &success, int &fact_time){
        if((pid.find(Origin_name) == pid.end()) || (pid.find(End_name) == pid.end())){
            success = 0;
            return "";
        }
        int origin_id = pid[Origin_name], end_id = pid[End_name];
        dijkstra(origin_id);
        if (arrive_time[end_id] == INF || mxn == 0) {
            success = 0;
            return "";
        }
        fact_time += arrive_time[end_id]; //每一段路的时间
        std::vector<int> path_point;
        path_point.push_back(end_id);
        all_path_point_cost = point_cost[end_id - 1];
        while (end_id != origin_id) {
            ++ ways;
            path[ways] = pre_edge[end_id];
            end_id = pre_point[end_id];
            path_point.push_back(end_id);
            all_path_point_cost += point_cost[end_id - 1];
        }
        std::reverse(path_point.begin(), path_point.end());
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
            return;
        }
        Path = vv[0];
        int success = 1, fact_time = 0;
        for(int i = 1; i < nsz; ++ i){
            Path += get_path_module(vv[i - 1], vv[i], success, fact_time);
            if(!success) { //无解了
                Spend_time = Start_time = Path = "无法抵达";
                return;
            }
        }
        Spend_time = get_string_by_int(fact_time + all_path_point_cost, 0);
        int infact_time = get_int_by_string(Arrival_time);
        Start_time = get_string_by_int(infact_time - fact_time, 1);
    }

    Task path_find(Task need){
        ways = 0;
        Task ans = need;
        Arrival_time = ans.Arrival_time;
        get_path(ans.Node_name);
        ans.Start_time = path_algorithm::Start_time;
        ans.Spend_time = path_algorithm::Spend_time;
        ans.Path = path_algorithm::Path;
        ans.ways = ways;
        for(int i = 1; i <= ways; ++ i)
            ans.path[i] = path[i];
        return ans;
    }
}

signed main() {
    std::ofstream output_file("9029.txt");

//    std::string ss;
//    while (input_file >> ss) {
//        output_file << ss << ' ';
//    }
//    output_file << '\n';

    // 重新读取 902.in 文件
//    input_file.close();
//    input_file.open("902.in");

//    int _n = 0;
//    input_file >> _n;
//    output_file << _n << std::endl;

    path_algorithm::Add_point("bus_step", 10);
    path_algorithm::Add_point("home", 100);
    path_algorithm::Add_point("school", 1000);

    path_algorithm::Add_edge("bus_step", "school", 24 * 3600);
    path_algorithm::Add_edge("home", "bus_step", 600);
    path_algorithm::Add_edge("home", "school", 10);
//    path_algorithm::Find("home", "school");
    Task tmp;
    tmp.Arrival_time = "10:00:00";
    std::vector<std::string> vv;
    vv.push_back("home");
//    vv.push_back("bus_step");
    vv.push_back("school");
    tmp.Node_name = vv;
//    path_algorithm::get_path(tmp.Node_name);
    output_file << "test - 1:" << std::endl;
    tmp = path_algorithm::path_find(tmp);
    output_file << tmp.Path << std::endl;
    output_file << tmp.Start_time << std::endl;
    output_file << tmp.Spend_time << std::endl;
//    for (int i = 1; i <= ways; ++i)
//        std::cout << path[i] << " \n"[i == ways];
//        output_file << path[i] << " \n"[i == ways];
    output_file << "test - 2:" << std::endl;
    vv.clear();
    vv.push_back("home");
    vv.push_back("bus_step");
    vv.push_back("school");
    tmp.Node_name = vv;
    tmp.Arrival_time = "8:00:00";
    tmp = path_algorithm::path_find(tmp);
    output_file << tmp.Path << std::endl;
    output_file << tmp.Start_time << std::endl;
    output_file << tmp.Spend_time << std::endl;
    return 0;
}
