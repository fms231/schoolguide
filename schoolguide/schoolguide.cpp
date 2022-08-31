#include<iostream>
#include<string>
#include<fstream>

using namespace std;

const int maxsize = 100;//最大的地点个数
float** dist;//存放最短路径
int** path;//存放后继
int* visited;//是否已进行访问
int* d_number;//存放走过路径的编号

typedef struct
{
	int pos; //地点编号
	string name; //地点名称
	string introduce; //地点介绍
}Spot;

typedef struct 
{
	Spot spot[maxsize+1];//点的集合
	float arcs[maxsize+1][maxsize+1];//邻接矩阵
	int spot_number; //点的个数
}Graph;

void menu();//显示菜单函数
Graph init();//初始化函数
void xianshi();//显示地图
void pos_data(Graph G);//显示地点信息函数
void Floyd(Graph G);//floyd算法函数实现
void foundmin(Graph G, int m, int n);//查询任意两个地点的最短距离
void foundall(Graph G, int m, int n, int k);//查询任意两个顶点之间所有简单路径
void exitSystem();//退出系统
void Delete();//释放内存
void change_road(Graph G);//修改任意两个地点的路径

void xianshi()
{
	//打开地图文件读入
	ifstream ifs;
	ifs.open("map.txt");
	char c;
	while (!ifs.eof())
	{
		c = ifs.get();
		cout << c;
	}
	cout << endl;
	ifs.close();
}

//初始化图
Graph init()
{
	ifstream ifs; //文件
	int pos;
	string name;
	string introduce;
	Graph G;
	int number = 0;
	ifs.open("data.txt", ios::in); //打开已设置好的文件进行读入
	while (ifs>>pos&&ifs>>name&&ifs>>introduce)
	{
		number++;//记录点的个数
		G.spot[number].pos = pos;
		G.spot[number].name = name;
		G.spot[number].introduce = introduce;	
	}
	G.spot_number = number;
	ifs.close();//关闭文件
	visited = new int[G.spot_number + 1];//访问数组
	//初始化为0，表示为访问
	for (int k = 0; k <= G.spot_number; k++)
	{
		visited[k] = 0;
	}
	//存放顺序访问的结点的编号
	d_number = new int[G.spot_number + 1];
	for (int k = 0; k <= G.spot_number; k++)
	{
		d_number[k] = 0;
	}
	//读入路径长度
	float weight;
	int i = 1, j = 1;
	//打开已有的文件
	ifs.open("arcs.txt", ios::in);
	while (ifs>>weight)
	{
		G.arcs[i][j] = weight;
		j++;
		//每读完与其他点的路径就更换起始点
		if (j == number + 1)
		{
			i++;
			j = 1;
		}
	}
	ifs.close();//关闭文件
	return G;
}
//打印地点的信息
void pos_data(Graph G)
{
	cout << "编号\t" << "名称\t" << "介绍" << endl;
	for (int i = 1; i <= G.spot_number; i++)
	{
		cout << G.spot[i].pos << '\t' << G.spot[i].name << '\t' << G.spot[i].introduce << endl;
	}
	system("pause");
	system("cls");
}

//floyd算法函数实现找到任意两点的最短路径
void Floyd(Graph G)
{
	//分配一个二维数组存放最短路径
	dist = new float* [G.spot_number + 1];
	path = new int* [G.spot_number + 1];
	for (int i = 1; i < G.spot_number + 1; i++)
	{
		dist[i] = new float[G.spot_number + 1];
		path[i] = new int[G.spot_number + 1];
	}
	//初始化最短距离
	for (int i = 1; i <= G.spot_number; i++)
	{
		for (int j = 1; j <= G.spot_number; j++)
		{
			dist[i][j] = G.arcs[i][j];
			//默认后继是j
			path[i][j] = j;
		}
	}
	//floyd算法实现
	for (int k = 1; k <= G.spot_number; k++)
	{
		for (int i = 1; i <= G.spot_number; i++)
		{
			for (int j = 1; j <= G.spot_number; j++)
			{
				//如果i到j的距离大于i到k再到j的距离
				if (dist[i][j] > dist[i][k] + dist[k][j])
				{
					dist[i][j] = dist[i][k] + dist[k][j];
					path[i][j] = path[i][k];//更换新的后继
				}
			}
		}
	}
}
//查找任意两个地点的最短距离
void foundmin(Graph G, int m, int n)
{
	cout << G.spot[m].name << "到" << G.spot[n].name << "的最短路径为：" << endl;
	float temp = dist[m][n];//设置临时变量存放m到n的最短路径
	while (m != n)//当还没有到达n点时
	{
		cout << G.spot[m].name << "->";
		m = path[m][n];//更换新的前趋
	}
	cout << G.spot[m].name;
	cout << endl << "最短路径为：" << temp << endl;
	system("pause");
	system("cls");
}
//查询地点m到地点n的所有简单路径
void foundall(Graph G, int m, int n, int k)
{
	//m点被访问
	visited[m] = 1;
	//第k个访问的点是m
	d_number[k] = m;//d_number数组存放依次访问的结点的编号
	if (m == n)//访问到终点
	{
		float length = 0;//存放m到n点的路径长度
		for (int i = 1; i < k; i++)
		{
			cout << G.spot[d_number[i]].name << "->";
			length = length + G.arcs[d_number[i]][d_number[i + 1]];
		}
		cout << G.spot[d_number[k]].name << "\t" << "路径长度为：" << length << endl;
		return;
	}
	else
	{
		for (int j = 1; j <= G.spot_number; j++)
		{
			if (!visited[j] && G.arcs[m][j] != 0 && G.arcs[m][j] != 100000)
			{
				visited[j] = 1;
				//再次以j为出发点进行访问，深度优先搜索
				foundall(G, j, n, ++k);
				visited[j] = 0;
				k--;
			}
		}
	}
}
//修改任意两点之间的路径
void change_road(Graph G)
{
	int m, n;
	cout << "请输入两点之间的编号：" << endl;
	cin >> m >> n;
	if (m > G.spot_number || m<1 || n>G.spot_number || n < 1||G.arcs[m][n]==0)
	{
		cout << "输入错误！请重新输入" << endl;
		cin >> m >> n;
	}
	cout << "请输入你想要修改的路径长度（100000表示不相连）：" << endl;
	float length;
	cin >> length;
	//m到n和n到m都需要进行修改
	G.arcs[m][n] = length;
	G.arcs[n][m] = length;
	ofstream ofs;
	ofs.open("arcs.txt", ios::out);//打开文件进行修改
	for (int i = 1; i <= G.spot_number; i++)
	{
		for (int j = 1; j <= G.spot_number; j++)
		{
			ofs << G.arcs[i][j] << " ";
		}
		ofs << endl;
	}
	ofs.close();
	cout << "修改成功！！！" << endl;
}
//退出系统
void exitSystem()
{
	cout << "欢迎下次使用" << endl;
	system("pause");
	exit(0);
}
//显示菜单函数
void menu()
{
	cout << "0.退出校园导游系统" << endl
		<< "1.查询景点信息" << endl
		<< "2.查询任意两点之间的最短路径" << endl
		<< "3.查询任意两点之间的所有简单路径" << endl
		<< "4.显示校园地图" << endl
		<< "5.修改任意两点之间的路径" << endl;
}
//释放内存函数
void Delete(Graph G)
{
	//释放二维数组的内存
	for (int i = 0; i <= G.spot_number; i++)
	{
		delete[] dist[i];
		delete[] path[i];
	}
	delete[] dist;
	delete[] path;
	//释放一维数组的内存
	delete[] visited;
	delete[] d_number;
}

int main()
{
	Graph G = init();
	int choice, m, n;
	while (true)
	{
		
		menu();//展示菜单
		cin >> choice;  //接受用户的选择
		system("cls");
		switch (choice)
		{
		case 0://退出系统
			exitSystem();
			break;
		case 1://查询景点的信息
			pos_data(G);
			break;
		case 2://查询任意两点之间的最短路径
			cout << "请输入您想要查询的两个地点的编号：" << endl;
			cin >> m >> n;
			if (m > G.spot_number || m<1 || n>G.spot_number || n < 1)
			{
				cout << "输入错误！请重新输入" << endl;
				cin >> m >> n;
			}
			Floyd(G);
			foundmin(G,m,n);
			break;
		case 3://查询任意两点之间的所有简单路径
			cout << "请输入您想要查询的两个地点的编号：" << endl;
			cin >> m >> n;
			if (m > G.spot_number || m<1 || n>G.spot_number || n < 1)
			{
				cout << "输入错误！请重新输入" << endl;
				cin >> m >> n;
			}
			foundall(G, m, n, 1);
			system("pause");
			system("cls");
			break;
		case 4://显示地图
			xianshi();
			system("pause");
			system("cls");
			break;
		case 5://修改两点之间的路径长度
			change_road(G);
			G = init();
			system("pause");
			system("cls");
			break;
		default:
			break;
		}
	}
	Delete(G);
	system("pause");
	system("cls");
}