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
struct ANode // �߽��
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
struct Point // ����
{
    int i, j;
};
struct Pathtype // ��¼�˵�ǰ�ߵ�����
{
    Point data[Maxsize]; // ��¼�߹���·��
    int length;
};
int d[][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}}; // һ�������ĸ��������ѡ��(������õ�ǰ���������ϵĽ������)
//---------------�����Թ�-----------------
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
            if (!A[i][j]) // A[i][j]=0��ʾ�����ߵ�ͨ
            {
                for (k = 0; k < 4; k++)
                {
                    int x = d[k][0] + i;
                    int y = d[k][1] + j;
                    if (!A[x][y]) // A[x][y]=0��ʾ�����߽����ڽӱ�
                    {
                        ANode *p = new ANode();
                        p->i = x;
                        p->j = y;
                        p->next = G->adjlist[i][j].fristarc; // ͷ�巨
                        G->adjlist[i][j].fristarc = p;
                    }
                }
            }
        }
}
int visited1[M + 2][M + 2] = {0}; // ��¼�Ƿ���ʹ�
int num = 0;                      // ��¼�߳��Թ���·������
vector<vector<Point>> Min_path;   // ��¼�߳��Թ���·��
multimap<int, int> Min_Nums;      // ��¼�߳��Թ���·������(key:·�����ȣ�value:����)
/*ѡ��multimap��ԭ��
1��keyֵ�����ظ�
2���Զ����򣨴�С���󣩣��������·�������
3�������������õĿռ������������С
*/
//---------�ҵ����г�ȥ��·������ӡ��DFS��-----------------
bool is(vector<Point> vp, int x, int y) // �ж�(x,y)�Ƿ���vp��
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
                    // 2-��ɫ��4--��ɫ��7---��ɫ��1--��ɫ
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
    path.data[path.length].i = x1; // ��ǰ������
    path.data[path.length++].j = y1;
    if (x1 == x2 && y1 == y2) // ���·�����ݹ��ֹͣ������
    {
        cout << "�Թ���·" << ++num << ":";
        Min_Nums.insert(make_pair(path.length, num - 1)); // ��һ�������ڼ�¼·���ĳ���
        vector<Point> v;                                  // ��¼����̶����Min_path
        printf(" ·��Ϊ��%d", path.length);
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
    ANode *p = G->adjlist[x1][y1].fristarc; // �ҵ���ǰ����ڽӱ�
    while (p != NULL)
    {
        if (!visited1[p->i][p->j])
            FindAllPath(G, p->i, p->j, x2, y2, path, A);
        p = p->next;
    }
    visited1[x1][y1] = 0; // �������һ��·�����¿�ʼ
}
//-----------������·����BFS��---------------
Point path[M + 2][M + 2]; // �洢��һ���������
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
void print(int x, int y, vector<Point> &vp) // �ݹ��ӡ,vector<Point>vp���ڼ�¼�����꣬��������ʾ����켣
{                                           // ����������vp,��Ȼ�޷���¼
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
    cout << " ·��Ϊ:" << vp.size() << endl;
    printAllPath(vp, A);
}
// BFS
void BMin_Path(int x1, int y1, int A[M + 2][M + 2])
{
    int visited2[M + 2][M + 2] = {0}; // �����ǳ�ʼ���ˣ�BFS��visited����ȫ�ֺ���
    int k;
    clear(path); // ��ʼ��������������ͼ
    Point p, temp;
    queue<Point> q;
    visited2[x1][y1] = 1;
    p.i = x1; // ��¼�������
    p.j = y1;
    q.push(p);
    while (!q.empty())
    {
        p = q.front();
        q.pop();
        int x2 = p.i;
        int y2 = p.j;
        for (k = 0; k < 4; k++) // �����ĸ�����
        {
            int x = d[k][0] + x2; // ����һ�������꣩
            int y = d[k][1] + y2;
            if (x < M + 2 && x >= 0 && y < M + 2 && y >= 0 && A[x][y] != 1 && visited2[x][y] == 0) // û���߹��ĵ�
            {
                visited2[x][y] = 1;
                temp.i = x;
                temp.j = y;
                q.push(temp);
                temp.i = x2;
                temp.j = y2;
                path[x][y] = temp;    // ��¼·��
                if (x == M && y == M) // ����
                    return;
            }
        }
    }
}
//-----------������·��(����)---------------
void printMin_path()
{
    int Min_key; // ��¼��С��·��
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
//---------------����Թ�------------------
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
//------------�˵�-----------------
void Menu()
{
    cout << "----------�Թ�ϵͳ-----------" << endl;
    cout << "----------------------------" << endl;
    cout << "------1��������е��Թ�------" << endl;
    cout << "------2�������Թ�      ------" << endl;
    cout << "------3���˳��Թ�ϵͳ   -----" << endl;
    cout << "----------------------------" << endl;
    cout << "----------------------------" << endl;
}
int main()
{
    int Total_File = 3; // �����Թ���ͼ����
    // �ļ��� AdjGraph+x+.txt
    string FileName1 = "AdjGraph"; // �ļ���part1
    // string FileName2;
    string FileName3 = ".txt"; // �ļ���׺part3
    while (true)
    {
        system("cls");
        Menu();
        string temp; // ���ַ�����Ϊ�˷�ֹ���������
        bool flag = true;
        while (flag)
        {
            cin >> temp;
            if (temp == "1" || temp == "2" || temp == "3")
                flag = false;
            else
                cout << "ָ�������������������룬��ȷ��ʽΪ��1/2/3����" << endl;
        }
        int select = temp[0] - '0';
        switch (select)
        {
        // ������е��Թ�
        case 1:
        {
            cout << "�����Թ���ͼ����Ϊ:" << Total_File << endl;
            cout << "������Ҫ�鿴���Թ���ͼ��";
            string FileName2; // ���ַ�����Ϊ�˷�ֹ���������
            bool flag = true;
            while (flag)
            {
                cin >> FileName2;
                string index = to_string(Total_File);
                if (index >= FileName2 && FileName2 > "0") // ��ֹ�ļ���part2����
                    flag = false;
                else
                    cout << "ָ�������������������룬��ȷ��ʽΪҪС��" << Total_File << endl;
            }
            string FileName = FileName1 + FileName2 + FileName3; // Ҫ�򿪵��ļ���
            ifstream infile(FileName, ios::in);                  // ���ļ�
            if (!infile)                                         // ���򲻿���������д��־��
            {
                cout << "open error!" << endl;
                exit(-1);
            }
            int A[M + 2][M + 2]; // �Թ�����
            int count = 0;
            while (!infile.eof())
            {
                for (int i = 0; i < M + 2; i++)
                    infile >> A[count][i]; // �����ո����ֹͣ���ո��������޷���ȡ
                count++;
            }
            infile.close(); // �ر��ļ�
            cout << "==========�Թ�========" << endl;
            printAdjGraph(A);
            AdjGraph *G;
            CreateAdjGraph(G, A);
            Pathtype path;
            path.length = 0;
            cout << "�Թ����г�·" << endl;
            FindAllPath(G, 1, 1, M, M, path, A);
            cout << "BFS�ҵ������·��" << endl;
            BMin_Path(1, 1, A);
            vector<Point> temp;
            print(M, M, temp);
            printf(" (%d,%d)\n", M, M);
            printBFS(temp, A); // ���BFS�ҵ���·���켣
            cout << "�����߳��Թ������·����" << endl;
            printMin_path();
            // ����������Թ�������
            num = 0;
            Min_Nums.clear();
            Min_path.clear();

            system("pause");
            break;
        }
        case 2:
        {
            string FileName2 = to_string(++Total_File);
            string FileName = FileName1 + FileName2 + FileName3; // Ҫ�򿪵��ļ���
            ofstream ofs;
            ofs.open(FileName, ios::out);
            for (int i = 0; i < M + 2; i++)
            {
                string str;
                if (i == 0 || i == (M + 1))
                    str = "1 1 1 1 1 1 1 1 1 1\n";
                else
                {
                    cout << "��д���" << i << "��" << endl;
                    cout << "�մ˸�ʽ(1 1 1 1 1 1 1 1)" << endl;
                    bool flag = true;
                    while (flag) // �ݴ�
                    {
                        // ǰ���õ���cin
                        cin.clear(); // ����cin��״̬��ʶ��
                        cin.sync();  // ������������ַ���
                        getline(cin, str);
                        int j;
                        if (str.size() > 2 * M - 1)
                        {
                            cout << "�Թ������С����8,���������룡" << endl;
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
                                    cout << "�����������������룡" << endl;
                                    break;
                                }
                            }
                        }
                        if (j == 2 * M - 1)
                            flag = false;
                    }
                    str = "1 " + str + " 1\n"; // ����������M->M+2
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
/*�Թ�1
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
/*�Թ�2
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
/*�Թ�3
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