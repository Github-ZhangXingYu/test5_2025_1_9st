//
// Created by 15411 on 2025/5/17.
//
#include <bits/stdc++.h>
using namespace std;

const int MAXM = 500000;
const int MAXMEDIA = 50000;
const int MAXST = 210000;  // 后缀自动机最大状态数（2 * 文章总长度）
const int ALPHA = 27;      // 字母表大小：a-z(0-25) + 分隔符#(26)
const int MAXN = 500000;   // 主字符串最大长度
const int INF_ID = 1000000000;

struct Node {
    int maxVal, idMax;
    Node *l, *r;
}; // 线段树节点（区间最大值与对应索引）

Node *nodePool;      // 线段树节点池
int poolPtr = 0;
Node* newNode() {
    Node *nd = &nodePool[poolPtr++];
    nd->maxVal = 0;
    nd->idMax  = INF_ID;
    nd->l = nd->r = nullptr;
    return nd;
}

// 后缀自动机状态结构
struct State {
    int len, link;
    int next[ALPHA];
} sam[MAXST];
int samSize, lastState;

// 媒体文章字符出现记录（链表形式）：每个状态的出现文章ID列表
int headOcc[MAXST];
int occNext[100005], occId[100005];
int occCnt;

// 每个状态对应的线段树根指针，以及辅助合并的大小（近似不同媒体计数）
Node* segRoot[MAXST];
int leafCount[MAXST];

// 后缀自动机链接跳跃表（用以二进制跳跃）
int parentJump[19][MAXST];
// 主字符串匹配到的SAM状态和匹配长度
int posState[MAXN+5], posLen[MAXN+5];

// 将字符映射到SAM的转移索引
inline int charToIndex(char c) {
    return c == '#' ? 26 : (c - 'a');
}

// 在SAM中插入一个字符（用于构建媒体文章的SAM）
void samExtend(int c) {
    int cur = samSize++;
    sam[cur].len = sam[lastState].len + 1;
    for(int i = 0; i < ALPHA; i++) sam[cur].next[i] = -1;
    sam[cur].link = -1;
    int p = lastState;
    while (p != -1 && sam[p].next[c] == -1) {
        sam[p].next[c] = cur;
        p = sam[p].link;
    }
    if (p == -1) {
        sam[cur].link = 0;
    } else {
        int q = sam[p].next[c];
        if (sam[p].len + 1 == sam[q].len) {
            sam[cur].link = q;
        } else {
            int clone = samSize++;
            sam[clone].len = sam[p].len + 1;
            memcpy(sam[clone].next, sam[q].next, sizeof(sam[q].next));
            sam[clone].link = sam[q].link;
            while (p != -1 && sam[p].next[c] == q) {
                sam[p].next[c] = clone;
                p = sam[p].link;
            }
            sam[q].link = sam[cur].link = clone;
        }
    }
    lastState = cur;
}

// 在线更新线段树：在状态root的线段树中，媒体ID为pos的位置加1
void segUpdate(Node* &root, int l, int r, int pos) {
    if (!root) root = newNode();
    if (l == r) {
        root->maxVal += 1;
        root->idMax = l;
        return;
    }
    int mid = (l + r) >> 1;
    if (pos <= mid) segUpdate(root->l, l, mid, pos);
    else           segUpdate(root->r, mid+1, r, pos);
    // 更新当前节点信息（孩子中取最大值，等值时取最小索引）
    int lval = root->l ? root->l->maxVal : 0;
    int rval = root->r ? root->r->maxVal : 0;
    if (lval > rval) {
        root->maxVal = lval;
        root->idMax  = root->l->idMax;
    } else if (rval > lval) {
        root->maxVal = rval;
        root->idMax  = root->r->idMax;
    } else {
        root->maxVal = lval;
        if (lval == 0) root->idMax = INF_ID;
        else {
            int lid = root->l ? root->l->idMax : INF_ID;
            int rid = root->r ? root->r->idMax : INF_ID;
            root->idMax = min(lid, rid);
        }
    }
}

// 合并线段树：将q中的数据合并到p中（返回合并后的根）
Node* segMerge(Node* p, Node* q, int l, int r) {
    if (!p) return q;
    if (!q) return p;
    if (l == r) {
        p->maxVal += q->maxVal;
        // 叶子id不变
        return p;
    }
    int mid = (l + r) >> 1;
    p->l = segMerge(p->l, q->l, l, mid);
    p->r = segMerge(p->r, q->r, mid+1, r);
    int lval = p->l ? p->l->maxVal : 0;
    int rval = p->r ? p->r->maxVal : 0;
    if (lval > rval) {
        p->maxVal = lval;
        p->idMax = p->l->idMax;
    } else if (rval > lval) {
        p->maxVal = rval;
        p->idMax = p->r->idMax;
    } else {
        p->maxVal = lval;
        if (lval == 0) p->idMax = INF_ID;
        else {
            int lid = p->l ? p->l->idMax : INF_ID;
            int rid = p->r ? p->r->idMax : INF_ID;
            p->idMax = min(lid, rid);
        }
    }
    return p;
}

// 在线段树root内查询区间[ql, qr]的最大值及对应ID
pair<int,int> segQuery(Node* root, int l, int r, int ql, int qr) {
    if (!root) return {INF_ID, 0};
    if (ql <= l && r <= qr) {
        return {root->idMax, root->maxVal};
    }
    int mid = (l + r) >> 1;
    pair<int,int> res = {INF_ID, 0};
    if (ql <= mid) {
        auto leftRes = segQuery(root->l, l, mid, ql, qr);
        if (leftRes.second > res.second || (leftRes.second == res.second && leftRes.first < res.first)) {
            res = leftRes;
        }
    }
    if (qr > mid) {
        auto rightRes = segQuery(root->r, mid+1, r, ql, qr);
        if (rightRes.second > res.second || (rightRes.second == res.second && rightRes.first < res.first)) {
            res = rightRes;
        }
    }
    return res;
}

int main(){
#ifdef ONLINE_JUDGE
#else
    freopen("test.in", "r", stdin);
    freopen("test.out", "w", stdout);
#endif
//    std::ios::sync_with_stdio(false);
//    std::cin.tie(nullptr);std::cout.tie(nullptr);
    ios::sync_with_stdio(false);
    cin.tie(NULL);

    // 分配线段树节点池空间
    nodePool = (Node*)malloc(sizeof(Node) * 2000000);

    string mainS;
    cin >> mainS;
    int mainLen = mainS.size();
    int m;
    cin >> m;

    // 初始化SAM
    samSize = 1;
    lastState = 0;
    sam[0].len = 0;
    sam[0].link = -1;
    for(int i = 0; i < ALPHA; i++) sam[0].next[i] = -1;
    // 初始化发生列表头
    for(int i = 0; i < MAXST; i++) headOcc[i] = -1;
    occCnt = 0;

    // 构建媒体文章的SAM，并记录每个字符对应的状态出现的媒体ID
    for(int articleId = 1; articleId <= m; articleId++){
        string article;
        cin >> article;
        for(char c : article) {
            int idx = charToIndex(c);
            samExtend(idx);
            int stid = lastState;
            occId[occCnt] = articleId;
            occNext[occCnt] = headOcc[stid];
            headOcc[stid] = occCnt;
            occCnt++;
        }
        // 插入分隔符#
        int sep = charToIndex('#');
        samExtend(sep);
    }
    // 构建链接二进制跳跃表
    for(int i = 0; i < samSize; i++){
        parentJump[0][i] = sam[i].link;
    }
    for(int k = 1; k < 19; k++){
        for(int i = 0; i < samSize; i++){
            int prev = parentJump[k-1][i];
            parentJump[k][i] = (prev == -1 ? -1 : parentJump[k-1][prev]);
        }
    }
    // 为每个状态构建线段树：首先统计叶子数量（用于合并优化）
    for(int i = 0; i < samSize; i++){
        int p = headOcc[i];
        leafCount[i] = 0;
        while(p != -1){
            leafCount[i]++;
            p = occNext[p];
        }
    }
    // 初始插入每个状态直接出现的媒体ID
    for(int st = 0; st < samSize; st++){
        int p = headOcc[st];
        while(p != -1){
            segUpdate(segRoot[st], 1, m, occId[p]);
            p = occNext[p];
        }
    }
    // 按长度从大到小对状态排序，逐级合并到父链接状态
    vector<int> order(samSize);
    for(int i = 0; i < samSize; i++) order[i] = i;
    sort(order.begin(), order.end(), [&](int a, int b){
        return sam[a].len > sam[b].len;
    });
    for(int idx = 0; idx < samSize; idx++){
        int v = order[idx];
        if(v == 0) continue;
        int p = sam[v].link;
        if(p == -1 || !segRoot[v]) continue;
        if(!segRoot[p]){
            segRoot[p] = segRoot[v];
            leafCount[p] = leafCount[v];
        } else {
            if(leafCount[v] > leafCount[p]){
                swap(segRoot[p], segRoot[v]);
                swap(leafCount[p], leafCount[v]);
            }
            segRoot[p] = segMerge(segRoot[p], segRoot[v], 1, m);
            leafCount[p] += leafCount[v];
        }
    }

    // 对主字符串进行遍历，用SAM匹配模式串计算匹配状态和长度
    int cur = 0, curLen = 0;
    for(int i = 0; i < mainLen; i++){
        int c = charToIndex(mainS[i]);
        while(cur != -1 && sam[cur].next[c] == -1){
            cur = sam[cur].link;
            if(cur != -1) curLen = sam[cur].len;
            else curLen = 0;
        }
        if(cur == -1){
            cur = 0;
            curLen = 0;
        }
        if(sam[cur].next[c] != -1){
            cur = sam[cur].next[c];
            curLen++;
        }
        posState[i] = cur;
        posLen[i]   = curLen;
    }

    int q;
    cin >> q;
    while(q--){
        int l, r, pl, pr;
        cin >> l >> r >> pl >> pr;
        // 转换为0基
        pl--; pr--;
        int length = pr - pl + 1;
        int st = posState[pr];
        if(posLen[pr] < length){
            // 子串没有出现
            cout << l << " " << 0 << "\n";
            continue;
        }
        // 跳链找到刚好覆盖该长度的SAM状态
        for(int k = 18; k >= 0; k--){
            int nxt = parentJump[k][st];
            if(nxt != -1 && sam[nxt].len >= length){
                st = nxt;
            }
        }
        auto res = segQuery(segRoot[st], 1, m, l, r);
        int resId = res.first;
        int resCount = res.second;
        if(resCount == 0) resId = l;
        cout << resId << " " << resCount << "\n";
    }
    return 0;
}
