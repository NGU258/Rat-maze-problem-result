 //用顺序栈求解迷宫问题
#include<bits/stdc++.h>

#include<Windows.h>

/*
#include <stdio.h>
#include <malloc.h>
#include<stdlib.h>
#include<iostream>
#include<vector>
#include<set>
*/

using namespace std;

//---------------------------------------------------------
//--迷宫栈基本运算---------------------------------------
//---------------------------------------------------------

#define MaxSize 100
#define M 8
#define N 8

#define FORB(i,l,r) for(int i=l;i<=r;i++)

bool SIGN[M + 2][N + 2];

int mg[M + 2][N + 2] =
{
	{1,1,1,1,1,1,1,1,1,1},
	{1,0,0,1,0,0,0,1,0,1},
	{1,0,0,1,0,0,0,1,0,1},
	{1,0,0,0,0,1,1,0,0,1},
	{1,0,1,1,1,0,0,0,0,1},
	{1,0,0,0,1,0,0,0,0,1},
	{1,0,1,0,0,0,1,0,0,1},
	{1,0,1,1,1,0,1,1,0,1},
	{1,1,0,0,0,0,0,0,0,1},
	{1,1,1,1,1,1,1,1,1,1}
};

int mgfb[M + 2][N + 2] =
{
	{1,1,1,1,1,1,1,1,1,1},
	{1,0,0,1,0,0,0,1,0,1},
	{1,0,0,1,0,0,0,1,0,1},
	{1,0,0,0,0,1,1,0,0,1},
	{1,0,1,1,1,0,0,0,0,1},
	{1,0,0,0,1,0,0,0,0,1},
	{1,0,1,0,0,0,1,0,0,1},
	{1,0,1,1,1,0,1,1,0,1},
	{1,1,0,0,0,0,0,0,0,1},
	{1,1,1,1,1,1,1,1,1,1}
};

typedef struct
{
	int i;				//当前方块的行号
	int j;				//当前方块的列号
	int move;				//move是下一可走相邻方位的方位号
} NODE;

typedef struct
{
	NODE data[MaxSize];	//存放方块
	int top;			//栈顶指针
} STACK;				//定义栈类型

const int T(300), ps(0);
//控制暂停时间，是否暂停

int cnt(0);
//找到的路径

NODE PATH[MaxSize];

int SSS;

int mn(1e9);

vector<pair<vector<NODE>, int> > t;
//存储路径与对应的路径长度

void print(int x, int y, int move = EOF, bool sign = true)
{
	system("cls");

	if (sign)
	{
		printf("小白鼠演算逃跑路线演示(目前已找到路径数：%d)：\n\n", cnt);
		printf("温馨小提示：\n\n");
		printf("1(墙壁)  0(通道)  *(小白鼠位置) +(走过的路径) 终点($) \n\n");

		//ans.push_back(make_pair(x, y));
	}

	FORB(i, 0, M + 1)
	{
		FORB(j, 0, M + 1)
			if (SIGN[i][j])
				cout << "+ ";
			else if (i == x && y == j)
				cout << "* ";
			else if (i == M && j == N)
				cout << "$ ";
			else
			{
				if (sign)
				{
					if (mgfb[i][j] == -1)
						cout << "+ ";
					else
						cout << mgfb[i][j] << " ";
				}
				else
				{
					if (mg[i][j] == -1)
						cout << "+ ";
					else
						cout << mg[i][j] << " ";
				}

			}
		cout << endl;
	}

	if (sign)
	{

		printf("\n\n当前位置：(%d,%d)", x, y);

		/*for (auto it = ans.begin(); it != ans.end(); it++)
		{
			printf(" (%d,%d)", it->first, it->second);
			if (it != ans.end() - 1)
				cout << "->";
		}
		ans.clear();*/

	}
	else
	{
		cout << "\n行走中……\n" << endl;
		printf("当前位置：(%d,%d) 移动方向：%d\n", x, y, move);

	}

	cout << "	 路径长度: " << SSS << endl << endl;

	if (ps)
		system("pause");
	else Sleep(T);

	SIGN[x][y] = true;

}

void InitStack(STACK*& s)		//初始化栈
{
	s = new STACK;
	s->top = -1;
}

void DestroyStack(STACK*& s)	//销毁栈
{
	free(s);
}

bool StackEmpty(STACK* s)		//判断栈是否为空
{
	return(s->top == -1);
}

bool Push(STACK*& s, NODE &e)	//进栈元素e
{
	if (s->top == MaxSize - 1)
		return false;

	s->top++;
	s->data[s->top] = e;

	return true;
}

bool Pop(STACK*& s, NODE& e)	//出栈元素e
{
	if (s->top == -1)
		return false;

	e = s->data[s->top];
	s->top--;

	return true;
}

bool GetTop(STACK* s, NODE& e)	//取栈顶元素e
{
	if (s->top == -1)
		return false;

	e = s->data[s->top];

	return true;
}

void mgpath(int si, int sj, int ei, int ej)	//求解路径为:(si,sj)->(ei,ej)
{
	NODE Start;

	int i, j, move, ii, jj, k;

	STACK *p;								//定义栈p

	InitStack(p);							//初始化栈顶指针

	Start.i = si; Start.j = sj;	Start.move = EOF;				//设置Start为入口

	Push(p, Start);								//方块Start进栈

	mg[si][sj] = EOF;							//入口的迷宫值置为-1避免重复走到该方块

	//print(si, sj, -1, 0);

	while (!StackEmpty(p))					//栈不空时循环
	{
		GetTop(p, Start);						//取栈顶方块Start

		i = Start.i; j = Start.j; move = Start.move;

		if (i == ei && j == ej)					//找到了出口,输出该路径
		{
			cnt++;

			SSS = p->top;

			vector<NODE> temp;

			FORB(i, 0, p->top)
				temp.push_back(p->data[i]);

			t.push_back({ temp,temp.size() });


			//if (temp.size() == 21)
			//{
			FORB(i, 0, p->top)
				print(p->data[i].i, p->data[i].j, 0, true);
			system("pause");
			//}

			memset(SIGN, 0, sizeof(SIGN));


			if (mn > p->top)
			{
				//寻找最小路径

				k = -1;

				FORB(i, 0, p->top)
					PATH[++k] = p->data[i];

				mn = p->top;

			}

			mg[i][j] = 0;

			p->top--;
			i = p->data[p->top].i;
			j = p->data[p->top].j;
			move = p->data[p->top].move;

		}

		bool find= false;

		while (move < 4 && !find)				//找相邻可走方块(i1,j1)
		{
			move++;
			switch (move)
			{
				case 0:ii = i - 1; jj = j;   break;
				case 1:ii = i;   jj = j + 1; break;
				case 2:ii = i + 1; jj = j;   break;
				case 3:ii = i;   jj = j - 1; break;
			}

			//找到一个相邻可走方块
			if (mg[ii][jj] == 0)
				find = true;
		}

		if (find)							//找到了一个可走的方向
		{
			p->data[p->top].move = move;		//记录方向

			Start.i = ii; Start.j = jj; Start.move = -1;

			Push(p, Start);

			mg[ii][jj] = -1;					//(ii,jj)的迷宫值置为-1，避免重复走到该方块

			//print(p->data[p->top].i, p->data[p->top].j, move, 0);

		}
		else								//没有路径可走,进行回溯 
		{
			Pop(p, Start);						//将栈顶方块退栈

			mg[Start.i][Start.j] = 0;					//让退栈方块的位置变为其他路径可走方块

			//print(p->data[p->top].i, p->data[p->top].j, p->data[p->top].move, 0);\

			//cout << "\n回溯中……\n";
		}
	}

	cout << "\n总共" << cnt << "种路径走法\n\n";
}

void solve()
{
	system("color 0a");

	mgpath(1, 1, M, N);

}

int main()
{
	solve();

	return 0;
}

/*

//最短路径：

//FORB(i, 0, mn)
//	print(PATH[i].i, PATH[i].j, EOF, true);

bool cmp(pair<vector<NODE>, int> &a, pair<vector<NODE>, int> &b)
{
	return a.second < b.second;
}

system("cls");

cout << "cnt: " << cnt << endl;

//sort(t.begin(),t.end(),cmp);

cout <<"t.size: "<< t.size() << endl;

cout << "mn: " << mn+1 << endl;

int bf()
{
	int cnt(0);
	FORB(i, 0, t.size() - 1)
	{
		FORB(j,0,t.size()-1)
			if (i != j)
			{
				if (t[i].second == t[j].second)
				{
					int ct(0);
					FORB(k, 0, t[i].first.size()-1)
					{
						if (t[i].first[k].i == t[j].first[k].i)
						{
							if (t[i].first[k].j == t[j].first[k].j)
								ct++;
						}
						else break;

					}

					if (ct == t[i].first.size())
						cnt++;
				}
			}
	}
	return cnt;
}

*/