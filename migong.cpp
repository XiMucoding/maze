#include <iostream>
#include <vector>
#include <map>
#include <queue>
#include <fstream>
#include <string>
#include <windows.h>
using namespace std;
#define M 8
#define Maxsize 500
struct ANode // 边结点
{
    int i, j;
    ANode *next;
};
struct VNode
{
    ANode *fristarc;
};
struct AdjGraph
{
    VNode adjlist[M + 2][M + 2];
};
struct Point // 坐标
{
    int i, j;
};
struct Pathtype // 记录人当前走到哪了
{
    Point data[Maxsize]; // 记录走过的路径
    int length;
};
int d[][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}}; // 一个点有四个方向可以选择(辅助获得当前点左右下上的结点坐标)
//---------------创建迷宫-----------------
void CreateAdjGraph(AdjGraph *&G, int A[M + 2][M + 2])
{
    int i, j, k;
    ANode *p;
    G = new AdjGraph();
    for (i = 0; i < M + 2; i++)
        for (j = 0; j < M + 2; j++)
            G->adjlist[i][j].fristarc = NULL;
    for (i = 1; i <= M; i++)
        for (j = 1; j <= M; j++)
        {
            if (!A[i][j]) // A[i][j]=0表示可以走的通
            {
                for (k = 0; k < 4; k++)
                {
                    int x = d[k][0] + i;
                    int y = d[k][1] + j;
                    if (!A[x][y]) // A[x][y]=0表示可以走建立邻接表
                    {
                        ANode *p = new ANode();
                        p->i = x;
                        p->j = y;
                        p->next = G->adjlist[i][j].fristarc; // 头插法
                        G->adjlist[i][j].fristarc = p;
                    }
                }
            }
        }
}
int visited1[M + 2][M + 2] = {0}; // 记录是否访问过
int num = 0;                      // 记录走出迷宫的路径总数
vector<vector<Point>> Min_path;   // 记录走出迷宫的路径
multimap<int, int> Min_Nums;      // 记录走出迷宫的路径长度(key:路径长度，value:索引)
/*选择multimap的原因：
1、key值可以重复
2、自动排序（从小到大），方便最短路径的输出
3、用起来，所用的空间比其他容器更小
*/
//---------找到所有出去的路径并打印（DFS）-----------------
bool is(vector<Point> vp, int x, int y) // 判断(x,y)是否在vp中
{
    for (int i = 0; i < vp.size(); i++)
        if (vp[i].i == x && y == vp[i].j)
            return true;
    return false;
}
void printAllPath(vector<Point> vp, int A[M + 2][M + 2])
{
    int i, j;
    cout << " ";
    for (j = 0; j < M + 2; j++)
        cout << " " << j;
    cout << endl;
    for (i = 0; i < M + 2; i++)
    {
        cout << i << " ";
        for (j = 0; j < M + 2; j++)
        {
            if (A[i][j] == 1)
            {
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x4);
                cout << "="
                     << " ";
            }
            else
            {
                if (is(vp, i, j))
                {
                    // 2-绿色，4--红色，7---白色，1--蓝色
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x2);
                    cout << ">"
                         << " ";
                }
                else
                    cout << A[i][j] << " ";
            }
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x7);
        }
        cout << endl;
    }
}
// DFS
void FindAllPath(AdjGraph *G, int x1, int y1, int x2, int y2, Pathtype path, int A[M + 2][M + 2])
{
    visited1[x1][y1] = 1;
    path.data[path.length].i = x1; // 当前的坐标
    path.data[path.length++].j = y1;
    if (x1 == x2 && y1 == y2) // 输出路径（递归的停止条件）
    {
        cout << "迷宫出路" << ++num << ":";
        Min_Nums.insert(make_pair(path.length, num - 1)); // 第一个数用于记录路径的长度
        vector<Point> v;                                  // 记录坐标继而存进Min_path
        printf(" 路程为：%d", path.length);
        for (int k = 0; k < path.length; k++)
        {
            printf(" (%d,%d)", path.data[k].i, path.data[k].j);
            Point p;
            p.i = path.data[k].i;
            p.j = path.data[k].j;
            v.push_back(p);
        }
        Min_path.push_back(v);
        puts("");
        printAllPath(v, A);
    }
    ANode *p = G->adjlist[x1][y1].fristarc; // 找到当前点的邻接表
    while (p != NULL)
    {
        if (!visited1[p->i][p->j])
            FindAllPath(G, p->i, p->j, x2, y2, path, A);
        p = p->next;
    }
    visited1[x1][y1] = 0; // 遍历输出一条路后，重新开始
}
//-----------输出最短路径（BFS）---------------
Point path[M + 2][M + 2]; // 存储下一个点的坐标
void clear(Point p[M + 2][M + 2])
{
    for (int i = 0; i < M + 2; i++)
    {
        for (int j = 0; j < M + 2; j++)
        {
            p[i][j].i = 0;
            p[i][j].j = 0;
        }
    }
}
void print(int x, int y, vector<Point> &vp) // 递归打印,vector<Point>vp用于记录点坐标，好用于显示界面轨迹
{                                           // 别忘记引用vp,不然无法记录
    if (x == 1 && y == 1)
        return;
    print(path[x][y].i, path[x][y].j, vp);
    printf(" (%d,%d)", path[x][y].i, path[x][y].j);
    Point xy;
    xy.i = path[x][y].i;
    xy.j = path[x][y].j;
    vp.push_back(xy);
}
void printBFS(vector<Point> vp, int A[M + 2][M + 2])
{
    Point temp;
    temp.i = M;
    temp.j = M;
    vp.push_back(temp);
    cout << " 路程为:" << vp.size() << endl;
    printAllPath(vp, A);
}
// BFS
void BMin_Path(int x1, int y1, int A[M + 2][M + 2])
{
    int visited2[M + 2][M + 2] = {0}; // 别忘记初始化了，BFS的visited别用全局函数
    int k;
    clear(path); // 初始化，便于其他地图
    Point p, temp;
    queue<Point> q;
    visited2[x1][y1] = 1;
    p.i = x1; // 记录起点坐标
    p.j = y1;
    q.push(p);
    while (!q.empty())
    {
        p = q.front();
        q.pop();
        int x2 = p.i;
        int y2 = p.j;
        for (k = 0; k < 4; k++) // 遍历四个方向
        {
            int x = d[k][0] + x2; // （下一步的坐标）
            int y = d[k][1] + y2;
            if (x < M + 2 && x >= 0 && y < M + 2 && y >= 0 && A[x][y] != 1 && visited2[x][y] == 0) // 没有走过的点
            {
                visited2[x][y] = 1;
                temp.i = x;
                temp.j = y;
                q.push(temp);
                temp.i = x2;
                temp.j = y2;
                path[x][y] = temp;    // 记录路径
                if (x == M && y == M) // 出口
                    return;
            }
        }
    }
}
//-----------输出最短路径(数组)---------------
void printMin_path()
{
    int Min_key; // 记录最小的路径
    for (multimap<int, int>::iterator it = Min_Nums.begin(); it != Min_Nums.end();)
    {
        Min_key = it->first;
        for (int k = 0; k < Min_key; k++)
            printf(" (%d,%d)", Min_path[it->second][k].i, Min_path[it->second][k].j);
        cout << endl;
        it++;
        if (it->first != Min_key)
            it = Min_Nums.end();
    }
}
//---------------输出迷宫------------------
void printAdjGraph(int A[M + 2][M + 2])
{
    int i, j;
    cout << " ";
    for (j = 0; j < M + 2; j++)
        cout << " " << j;
    cout << endl;
    for (i = 0; i < M + 2; i++)
    {
        cout << i << " ";
        for (j = 0; j < M + 2; j++)
        {
            if (A[i][j] == 1)
            {
                SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x4);
                cout << "="
                     << " ";
            }
            else
                cout << A[i][j] << " ";
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x7);
        }
        cout << endl;
    }
}
//------------菜单-----------------
void Menu()
{
    cout << "----------迷宫系统-----------" << endl;
    cout << "----------------------------" << endl;
    cout << "------1、输出已有的迷宫------" << endl;
    cout << "------2、输入迷宫      ------" << endl;
    cout << "------3、退出迷宫系统   -----" << endl;
    cout << "----------------------------" << endl;
    cout << "----------------------------" << endl;
}
int main()
{
    int Total_File = 3; // 现有迷宫地图数量
    // 文件名 AdjGraph+x+.txt
    string FileName1 = "AdjGraph"; // 文件名part1
    // string FileName2;
    string FileName3 = ".txt"; // 文件后缀part3
    while (true)
    {
        system("cls");
        Menu();
        string temp; // 用字符串是为了防止错误的输入
        bool flag = true;
        while (flag)
        {
            cin >> temp;
            if (temp == "1" || temp == "2" || temp == "3")
                flag = false;
            else
                cout << "指令输入有误，请重新输入，正确格式为（1/2/3）。" << endl;
        }
        int select = temp[0] - '0';
        switch (select)
        {
        // 输出已有的迷宫
        case 1:
        {
            cout << "现有迷宫地图数量为:" << Total_File << endl;
            cout << "请输入要查看的迷宫地图：";
            string FileName2; // 用字符串是为了防止错误的输入
            bool flag = true;
            while (flag)
            {
                cin >> FileName2;
                string index = to_string(Total_File);
                if (index >= FileName2 && FileName2 > "0") // 防止文件名part2出错
                    flag = false;
                else
                    cout << "指令输入有误，请重新输入，正确格式为要小于" << Total_File << endl;
            }
            string FileName = FileName1 + FileName2 + FileName3; // 要打开的文件名
            ifstream infile(FileName, ios::in);                  // 打开文件
            if (!infile)                                         // 若打不开，报错（编写日志）
            {
                cout << "open error!" << endl;
                exit(-1);
            }
            int A[M + 2][M + 2]; // 迷宫矩阵
            int count = 0;
            while (!infile.eof())
            {
                for (int i = 0; i < M + 2; i++)
                    infile >> A[count][i]; // 遇到空格输出停止，空格后的内容无法读取
                count++;
            }
            infile.close(); // 关闭文件
            cout << "==========迷宫========" << endl;
            printAdjGraph(A);
            AdjGraph *G;
            CreateAdjGraph(G, A);
            Pathtype path;
            path.length = 0;
            cout << "迷宫所有出路" << endl;
            FindAllPath(G, 1, 1, M, M, path, A);
            cout << "BFS找到的最短路径" << endl;
            BMin_Path(1, 1, A);
            vector<Point> temp;
            print(M, M, temp);
            printf(" (%d,%d)\n", M, M);
            printBFS(temp, A); // 输出BFS找到的路径轨迹
            cout << "其中走出迷宫的最短路径：" << endl;
            printMin_path();
            // 清理处理过的迷宫的数据
            num = 0;
            Min_Nums.clear();
            Min_path.clear();

            system("pause");
            break;
        }
        case 2:
        {
            string FileName2 = to_string(++Total_File);
            string FileName = FileName1 + FileName2 + FileName3; // 要打开的文件名
            ofstream ofs;
            ofs.open(FileName, ios::out);
            for (int i = 0; i < M + 2; i++)
            {
                string str;
                if (i == 0 || i == (M + 1))
                    str = "1 1 1 1 1 1 1 1 1 1\n";
                else
                {
                    cout << "请写入第" << i << "行" << endl;
                    cout << "照此格式(1 1 1 1 1 1 1 1)" << endl;
                    bool flag = true;
                    while (flag) // 容错
                    {
                        // 前面用到了cin
                        cin.clear(); // 更改cin的状态标识符
                        cin.sync();  // 清除缓存区的字符流
                        getline(cin, str);
                        int j;
                        if (str.size() > 2 * M - 1)
                        {
                            cout << "迷宫矩阵大小超过8,请重新输入！" << endl;
                            continue;
                        }
                        else
                        {
                            for (j = 0; j < 2 * M - 1; j++)
                            {
                                if (str[j] == '1' || str[j] == '0' || str[j] == ' ')
                                {
                                    continue;
                                }
                                else
                                {
                                    cout << "输入有误，请重新输入！" << endl;
                                    break;
                                }
                            }
                        }
                        if (j == 2 * M - 1)
                            flag = false;
                    }
                    str = "1 " + str + " 1\n"; // 矩阵列数由M->M+2
                }
                ofs << str;
            }
            ofs.close();
            break;
        }
        case 3:
            return 0;
            break;
        }
    }
    system("pause");
    return 0;
}
/*迷宫1
1 1 1 1 1 1 1 1 1 1
1 0 0 0 1 1 1 1 1 1
1 1 0 0 0 0 0 1 1 1
1 1 0 1 1 1 0 1 1 1
1 1 0 0 0 1 0 1 1 1
1 1 1 1 0 1 0 1 1 1
1 1 1 1 0 1 0 1 1 1
1 1 1 1 0 1 0 1 1 1
1 1 1 1 0 0 0 0 0 1
1 1 1 1 1 1 1 1 1 1*/
/*迷宫2
1 1 1 1 1 1 1 1 1 1
1 0 0 1 1 1 1 1 1 1
1 1 0 0 0 0 0 1 1 1
1 1 0 1 1 1 0 1 1 1
1 1 0 0 0 0 0 1 1 1
1 1 1 1 0 1 0 0 1 1
1 1 1 0 0 1 1 0 1 1
1 1 1 0 1 1 1 0 1 1
1 1 1 0 0 0 0 0 0 1
1 1 1 1 1 1 1 1 1 1*/
/*迷宫3
1 1 1 1 1 1 1 1 1 1
1 0 0 1 1 1 1 1 1 1
1 0 0 0 1 1 1 1 1 1
1 1 0 0 0 1 1 1 1 1
1 1 1 0 0 0 1 1 1 1
1 1 1 1 0 0 0 1 1 1
1 1 1 1 1 0 1 1 1 1
1 1 1 1 1 0 1 1 1 1
1 1 1 1 1 0 0 0 0 1
1 1 1 1 1 1 1 1 1 1*/