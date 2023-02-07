#ifndef __RANK_H__
#define __RANK_H__
#include <algorithm>
#include <iostream>
#include<vector>
using namespace std;
struct User { //��װ��ҵ���Ϣ�������ɼ���id
	int score = 0;
	char name[104] = {};
};


vector<User> user; // ����������ҵ���Ϣ

/*
���ܣ���ȡrank.txt�е���Ϣ
��������
����ֵ����
���ú�����
�ı��ȫ�ֱ�����user
��ע����
*/

void Read_Rank() {
	FILE* fp = NULL; // �����ļ�ָ�����ͱ���fp
	fopen_s(&fp, "./Res/doc/rank.txt", "r"); // �򿪴����·�����ļ�rank.txt,"r"��ʾ��ֻ����ʽ�򿪣��򿪳ɹ��򷵻��ļ�ָ�븳ֵ��fp
	if (fp == NULL) { // ��ʧ��
		cout << "��ȡ���а�ʧ��!!!" << endl;
	}
	else { //�򿪳ɹ�
		char temp_s[1024]; // ��ʱ���浥�����id
		int temp_int; // ��ʱ���浥����ҵ÷�
		while (fscanf_s(fp, "%s %d", temp_s, 1024, &temp_int) != EOF) { //���и�ʽ����ȡrank.txt�ļ�,�����ȡ������˳�ѭ����ÿ�ΰ�id�͵÷ֱַ����temp_s[1024]��temp_int
			struct User uu; // ������ҽṹ�����uu
			strcpy_s(uu.name, temp_s); //��temp_s�����������uu.name
			uu.score = temp_int; //��temp_int����ҵ÷ָ���uu.score
			user.push_back(uu); //��uu�Ž�user�������������ҵ���Ϣ������
		}
	}
}

bool cmp(User u1, User u2) { //��дsort�ıȽϺ�����ʹ����user���÷ֽ���
	return u1.score > u2.score;
}

void rank_sort() {
	sort(user.begin(), user.end(), cmp); //����c++���sort������
}

void ppprank() { // ��ʾ�����Ϣ�ںڴ���,��ʵ������
	int n = user.size();
	for (int i = 0;i < n;i++) {
		cout << "--" << user[i].name << "--" << user[i].score << "--" << endl;
	}
}
#endif

/*
* 
����main.cpp������

Rank_UI() �������ִ�д�����ҵ���ˡ����а񡱰�ť��
�������Read_Rank(����ȡ���а�
�������rank_sort()��ȫ�ֱ���user����
����Rank_UI()�����user��ʾ�ź���������Ϣ
*/