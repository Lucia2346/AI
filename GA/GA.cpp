//���Ŵ��㷨����f=x*(sin(10PI*x))+2
#include<iostream>
#include<cstdio>
#include<cmath>
#include <string>
#include<vector>
#include <ctime>
#include<cstring>
#define eps 1e-6 
#define Pi acos(-1)
const int SIZE = 50;
const int Len = 22;
const int Val = 4194304;
const int Times = 1e2;
using namespace std;
/*------------------------------------------����------------------------------------------*/
string Encode(double x) {/*(1+x)*(4194303)/3=x1 2^22=4194304*/
	string ans = "";
	int tmp = (1 + x) * (Val - 1) / 3.0 + 0.5;//��������
	int Binpow = Val / 2;
	while (Binpow >= 1) {
		if (tmp >= Binpow) {
			ans += '1';	tmp -= Binpow;
		}
		else ans += '0';
		Binpow >>= 1;
	}
	return ans;
}
double Decode(string s) {/* 3/(2^22-1)*x1 - 1 = x��*/
	int ans = 0;
	for (int i = 0; i < 22; ++i)
		ans = (ans << 1) + s[i] - '0';
	return 3.0 * (ans) / (Val - 1) - 1.0;
}
/*----------------------------------------------------------------------------------------*/
/*--------------------------------------������ʼ��Ⱥ--------------------------------------*/
vector<string>Species;
void Init() {//��ʼ��������22λ�Ļ�����
	for (int i = 0; i < 50; ++i) {
		string s = "";
		for (int j = 0; j < Len; ++j)
			s += rand() % 2 + '0';
		Species.push_back(s);
	}
}
/*----------------------------------------------------------------------------------------*/
/*---------------------------------------������Ӧ��---------------------------------------*/
double fitness[SIZE + 5] = { 0 };//��50��������Ӧ��
double Max_fitness = 0;
int index = 0;
double Function(double x) { return x * sin(10.0 * Pi * x) + 2.0; }
void CalcuFitness() {//������Ӧ��
	memset(fitness, 0, sizeof(fitness));
	for (int i = 0; i < SIZE; ++i) {
		fitness[i] = Function(Decode(Species[i]));
		fitness[50] += fitness[i];
		if (fitness[i] > Max_fitness) { Max_fitness = fitness[i]; index = i; }
	}
}
/*----------------------------------------------------------------------------------------*/
/*------------------------------------------ѡ��------------------------------------------*/
int Select() {
	double proportion = 1.0 * rand() / RAND_MAX;//���ɴ�0��1�ĸ�����
	int i = rand() % SIZE;//�����㣬����ÿ�ζ���ͷ������
	for (int j = i - 1; i < SIZE && j >= 0; ++i, --j) {//��iΪ��������߱���
		if (i<SIZE && fitness[i] * 1.0 / fitness[50] > proportion) return i;
		if (j >= 0 && fitness[j] * 1.0 / fitness[50] > proportion) return j;
	}
	return Select();
}
/*----------------------------------------------------------------------------------------*/
/*----------------------------------------�Ŵ�����----------------------------------------*/
vector<string>Offspring;
int Rand() { return rand() % SIZE; }
void Born() {
	Offspring.push_back(Encode(Max_fitness));//�������ܳ��Ļ��һ�㡣
	int mother, father, pos, cnt;
	while (Offspring.size() < SIZE) {
		mother = Select(), father = Select();//�Ҹ�ĸ
		double tmp1 = fitness[mother], tmp2 = fitness[father];
		if (mother == father) continue;//�Ź��Լ���
		cnt = Rand() % 4;//�𽻲�̫�࣬0��1��2��3
		while (cnt-- > 0) {//����
			pos = Rand() % Len;
			swap(Species[mother][pos], Species[father][pos]);
		}
		if (Function(Decode(Species[mother])) > fitness[mother])//ѡ��˫��ǿ��
			Offspring.push_back(Species[mother]);
		if (Function(Decode(Species[father])) > fitness[father])
			Offspring.push_back(Species[father]);
	}
	if (Offspring.size() > 50)
		Offspring.pop_back();//��������������һ�ӣ����ܻ���51��,��ôҪȥ��һ����
}

/*----------------------------------------------------------------------------------------*/
/*----------------------------------------�������----------------------------------------*/
void Vary() {
	int pos = Rand() % Len, cnt = Rand() % 6, index = Rand();
	while (cnt-- > 0)//�����μ��������������졣
		Offspring[index][pos] = (Offspring[index][pos] - '0') ? '0' : '1';
	Species.assign(Offspring.begin(), Offspring.end());//���ӱ�ɴ���
}
/*----------------------------------------------------------------------------------------*/
inline void GA() {//һ���Ŵ��㷨
	CalcuFitness();
	Born();
	Vary();
}
int main() {
	srand((unsigned)time(NULL));
	double start = clock();
	Init();
	for (int i = 1; i <= Times; ++i)
		GA();
	double end = clock();
	printf("��x=%.6lf,f(x)=%.6lf��x�Ķ�����=%s\n", Decode(Species[index]), Max_fitness, Species[index].c_str());
	printf("Times=%d��,Time spent: %lfs\n", Times, 1.0 * (end - start) / CLOCKS_PER_SEC);
	return 0;
}

