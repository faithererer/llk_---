#ifndef __RANK_H__
#define __RANK_H__
#include <algorithm>
#include <iostream>
#include<vector>
using namespace std;
struct User { //封装玩家的信息，包括成绩和id
	int score = 0;
	char name[104] = {};
};


vector<User> user; // 储存所有玩家的信息

/*
功能：读取rank.txt中的信息
参数：无
返回值：无
调用函数：
改变的全局变量：user
备注：无
*/

void Read_Rank() {
	FILE* fp = NULL; // 定义文件指针类型变量fp
	fopen_s(&fp, "./Res/doc/rank.txt", "r"); // 打开此相对路径的文件rank.txt,"r"表示以只读方式打开，打开成功则返回文件指针赋值给fp
	if (fp == NULL) { // 打开失败
		cout << "读取排行榜失败!!!" << endl;
	}
	else { //打开成功
		char temp_s[1024]; // 临时储存单个玩家id
		int temp_int; // 临时储存单个玩家得分
		while (fscanf_s(fp, "%s %d", temp_s, 1024, &temp_int) != EOF) { //按行格式化读取rank.txt文件,如果读取完毕则退出循环，每次把id和得分分别存入temp_s[1024]，temp_int
			struct User uu; // 定义玩家结构体变量uu
			strcpy_s(uu.name, temp_s); //把temp_s的玩家名赋给uu.name
			uu.score = temp_int; //把temp_int的玩家得分赋给uu.score
			user.push_back(uu); //把uu放进user这个储存所有玩家的信息的数组
		}
	}
}

bool cmp(User u1, User u2) { //重写sort的比较函数，使数组user按得分降序
	return u1.score > u2.score;
}

void rank_sort() {
	sort(user.begin(), user.end(), cmp); //调用c++库的sort排序函数
}

void ppprank() { // 显示玩家信息在黑窗口,无实际作用
	int n = user.size();
	for (int i = 0;i < n;i++) {
		cout << "--" << user[i].name << "--" << user[i].score << "--" << endl;
	}
}
#endif

/*
* 
它与main.cpp交互：

Rank_UI() 这个函数执行代表玩家点击了“排行榜”按钮，
它会调用Read_Rank(）读取排行榜，
它会调用rank_sort()给全局变量user排序
后续Rank_UI()会根据user显示排好序的玩家信息
*/