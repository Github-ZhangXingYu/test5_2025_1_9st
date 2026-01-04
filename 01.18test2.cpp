//
// Created by 15411 on 2025/1/18.
//模版示例，最好用namespace封装一下，我怕变量名字重复
#include<bits/stdc++.h>

//下面是全局变量，用来和我的代码传送数据
std::string Item_name,Start_time,Arrival_time,Spend_time,Path;//存放项目名字，起始时间，到达时间，路上花费时间，路径
//其中路径Path里存放 1->2->5->6这样的格式存放路径

int ways;//存放走过的边的数量
int path[20005];//存放边的标号，一共ways条，如边1，4,5这样子存放

namespace path_algorithm{

    //下面的函数需要实现
    void Add_edge(int u_id,int v_id,std::string u_name,std::string v_name,int id,int time){//这里还需要一个填单项边u->v的函数，边的id

    }
    void Find(std::string Origin_name,std::string End_name){//计算最短路,我会传入起点和终点
        //计算完成后把答案存入全局变量Start_time,Spend_time,Path；ways，path；
        //我后面直接调用这部数据
    }

}

/*后续优化：可以考虑给查询进行按起点排序，跑过的最短路可以想方法存起来，以后直接调用之类的*/
