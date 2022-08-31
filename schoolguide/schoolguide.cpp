#include<iostream>
#include<string>
#include<fstream>

using namespace std;

const int maxsize = 100;//���ĵص����
float** dist;//������·��
int** path;//��ź��
int* visited;//�Ƿ��ѽ��з���
int* d_number;//����߹�·���ı��

typedef struct
{
	int pos; //�ص���
	string name; //�ص�����
	string introduce; //�ص����
}Spot;

typedef struct 
{
	Spot spot[maxsize+1];//��ļ���
	float arcs[maxsize+1][maxsize+1];//�ڽӾ���
	int spot_number; //��ĸ���
}Graph;

void menu();//��ʾ�˵�����
Graph init();//��ʼ������
void xianshi();//��ʾ��ͼ
void pos_data(Graph G);//��ʾ�ص���Ϣ����
void Floyd(Graph G);//floyd�㷨����ʵ��
void foundmin(Graph G, int m, int n);//��ѯ���������ص����̾���
void foundall(Graph G, int m, int n, int k);//��ѯ������������֮�����м�·��
void exitSystem();//�˳�ϵͳ
void Delete();//�ͷ��ڴ�
void change_road(Graph G);//�޸����������ص��·��

void xianshi()
{
	//�򿪵�ͼ�ļ�����
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

//��ʼ��ͼ
Graph init()
{
	ifstream ifs; //�ļ�
	int pos;
	string name;
	string introduce;
	Graph G;
	int number = 0;
	ifs.open("data.txt", ios::in); //�������úõ��ļ����ж���
	while (ifs>>pos&&ifs>>name&&ifs>>introduce)
	{
		number++;//��¼��ĸ���
		G.spot[number].pos = pos;
		G.spot[number].name = name;
		G.spot[number].introduce = introduce;	
	}
	G.spot_number = number;
	ifs.close();//�ر��ļ�
	visited = new int[G.spot_number + 1];//��������
	//��ʼ��Ϊ0����ʾΪ����
	for (int k = 0; k <= G.spot_number; k++)
	{
		visited[k] = 0;
	}
	//���˳����ʵĽ��ı��
	d_number = new int[G.spot_number + 1];
	for (int k = 0; k <= G.spot_number; k++)
	{
		d_number[k] = 0;
	}
	//����·������
	float weight;
	int i = 1, j = 1;
	//�����е��ļ�
	ifs.open("arcs.txt", ios::in);
	while (ifs>>weight)
	{
		G.arcs[i][j] = weight;
		j++;
		//ÿ�������������·���͸�����ʼ��
		if (j == number + 1)
		{
			i++;
			j = 1;
		}
	}
	ifs.close();//�ر��ļ�
	return G;
}
//��ӡ�ص����Ϣ
void pos_data(Graph G)
{
	cout << "���\t" << "����\t" << "����" << endl;
	for (int i = 1; i <= G.spot_number; i++)
	{
		cout << G.spot[i].pos << '\t' << G.spot[i].name << '\t' << G.spot[i].introduce << endl;
	}
	system("pause");
	system("cls");
}

//floyd�㷨����ʵ���ҵ�������������·��
void Floyd(Graph G)
{
	//����һ����ά���������·��
	dist = new float* [G.spot_number + 1];
	path = new int* [G.spot_number + 1];
	for (int i = 1; i < G.spot_number + 1; i++)
	{
		dist[i] = new float[G.spot_number + 1];
		path[i] = new int[G.spot_number + 1];
	}
	//��ʼ����̾���
	for (int i = 1; i <= G.spot_number; i++)
	{
		for (int j = 1; j <= G.spot_number; j++)
		{
			dist[i][j] = G.arcs[i][j];
			//Ĭ�Ϻ����j
			path[i][j] = j;
		}
	}
	//floyd�㷨ʵ��
	for (int k = 1; k <= G.spot_number; k++)
	{
		for (int i = 1; i <= G.spot_number; i++)
		{
			for (int j = 1; j <= G.spot_number; j++)
			{
				//���i��j�ľ������i��k�ٵ�j�ľ���
				if (dist[i][j] > dist[i][k] + dist[k][j])
				{
					dist[i][j] = dist[i][k] + dist[k][j];
					path[i][j] = path[i][k];//�����µĺ��
				}
			}
		}
	}
}
//�������������ص����̾���
void foundmin(Graph G, int m, int n)
{
	cout << G.spot[m].name << "��" << G.spot[n].name << "�����·��Ϊ��" << endl;
	float temp = dist[m][n];//������ʱ�������m��n�����·��
	while (m != n)//����û�е���n��ʱ
	{
		cout << G.spot[m].name << "->";
		m = path[m][n];//�����µ�ǰ��
	}
	cout << G.spot[m].name;
	cout << endl << "���·��Ϊ��" << temp << endl;
	system("pause");
	system("cls");
}
//��ѯ�ص�m���ص�n�����м�·��
void foundall(Graph G, int m, int n, int k)
{
	//m�㱻����
	visited[m] = 1;
	//��k�����ʵĵ���m
	d_number[k] = m;//d_number���������η��ʵĽ��ı��
	if (m == n)//���ʵ��յ�
	{
		float length = 0;//���m��n���·������
		for (int i = 1; i < k; i++)
		{
			cout << G.spot[d_number[i]].name << "->";
			length = length + G.arcs[d_number[i]][d_number[i + 1]];
		}
		cout << G.spot[d_number[k]].name << "\t" << "·������Ϊ��" << length << endl;
		return;
	}
	else
	{
		for (int j = 1; j <= G.spot_number; j++)
		{
			if (!visited[j] && G.arcs[m][j] != 0 && G.arcs[m][j] != 100000)
			{
				visited[j] = 1;
				//�ٴ���jΪ��������з��ʣ������������
				foundall(G, j, n, ++k);
				visited[j] = 0;
				k--;
			}
		}
	}
}
//�޸���������֮���·��
void change_road(Graph G)
{
	int m, n;
	cout << "����������֮��ı�ţ�" << endl;
	cin >> m >> n;
	if (m > G.spot_number || m<1 || n>G.spot_number || n < 1||G.arcs[m][n]==0)
	{
		cout << "�����������������" << endl;
		cin >> m >> n;
	}
	cout << "����������Ҫ�޸ĵ�·�����ȣ�100000��ʾ����������" << endl;
	float length;
	cin >> length;
	//m��n��n��m����Ҫ�����޸�
	G.arcs[m][n] = length;
	G.arcs[n][m] = length;
	ofstream ofs;
	ofs.open("arcs.txt", ios::out);//���ļ������޸�
	for (int i = 1; i <= G.spot_number; i++)
	{
		for (int j = 1; j <= G.spot_number; j++)
		{
			ofs << G.arcs[i][j] << " ";
		}
		ofs << endl;
	}
	ofs.close();
	cout << "�޸ĳɹ�������" << endl;
}
//�˳�ϵͳ
void exitSystem()
{
	cout << "��ӭ�´�ʹ��" << endl;
	system("pause");
	exit(0);
}
//��ʾ�˵�����
void menu()
{
	cout << "0.�˳�У԰����ϵͳ" << endl
		<< "1.��ѯ������Ϣ" << endl
		<< "2.��ѯ��������֮������·��" << endl
		<< "3.��ѯ��������֮������м�·��" << endl
		<< "4.��ʾУ԰��ͼ" << endl
		<< "5.�޸���������֮���·��" << endl;
}
//�ͷ��ڴ溯��
void Delete(Graph G)
{
	//�ͷŶ�ά������ڴ�
	for (int i = 0; i <= G.spot_number; i++)
	{
		delete[] dist[i];
		delete[] path[i];
	}
	delete[] dist;
	delete[] path;
	//�ͷ�һά������ڴ�
	delete[] visited;
	delete[] d_number;
}

int main()
{
	Graph G = init();
	int choice, m, n;
	while (true)
	{
		
		menu();//չʾ�˵�
		cin >> choice;  //�����û���ѡ��
		system("cls");
		switch (choice)
		{
		case 0://�˳�ϵͳ
			exitSystem();
			break;
		case 1://��ѯ�������Ϣ
			pos_data(G);
			break;
		case 2://��ѯ��������֮������·��
			cout << "����������Ҫ��ѯ�������ص�ı�ţ�" << endl;
			cin >> m >> n;
			if (m > G.spot_number || m<1 || n>G.spot_number || n < 1)
			{
				cout << "�����������������" << endl;
				cin >> m >> n;
			}
			Floyd(G);
			foundmin(G,m,n);
			break;
		case 3://��ѯ��������֮������м�·��
			cout << "����������Ҫ��ѯ�������ص�ı�ţ�" << endl;
			cin >> m >> n;
			if (m > G.spot_number || m<1 || n>G.spot_number || n < 1)
			{
				cout << "�����������������" << endl;
				cin >> m >> n;
			}
			foundall(G, m, n, 1);
			system("pause");
			system("cls");
			break;
		case 4://��ʾ��ͼ
			xianshi();
			system("pause");
			system("cls");
			break;
		case 5://�޸�����֮���·������
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