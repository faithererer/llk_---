#ifndef __BUTTON_H__
#define __BUTTON_H__
#include<graphics.h>
#include<graphics.h>
#include<string>
#include<iostream>
#include<stdlib.h>
using namespace std;
#define LTY_COLOR RGB(102,204,255)
ExMessage mouse_msg;

struct button {
	int height = 0; // ��ť�߶�
	int width = 0; // ��ť���
	int x = 0; // xλ������
	int y = 0; // yλ������
	COLORREF curColor = BLACK; // ��ǰ��ť��ɫ
	COLORREF inColor = BLACK; // δ����ʱ��ť��ɫ
	COLORREF outColor = BLACK; // ����ʱ��ť��ɫ
	char pStr[101] = "?"; // �ı�
	int textSize = 0; // �ı���С
	COLORREF textColor = BLACK; //������ɫ
};

// x����,y����,��ť��ȣ���ť�߶ȣ��ı����ı���С����ǰ��ť��ɫ����괥����ɫ�����Ǵ�����ɫ
button* create_Button(int x, int  y, int width, int height, LPCTSTR  pStr,int textSize,
	COLORREF curColor, COLORREF inColor, COLORREF outColor) {
	struct button* btn = new button;
	btn->x = x;
	btn->y = y;
	btn->width = width;
	btn->height = height;
	strcpy_s(btn->pStr, pStr);
	btn->curColor = curColor;
	btn->inColor = inColor;
	btn->outColor = outColor;
	return btn;
}
void DrawButton(struct button* pB)
{
	// ����ť
	setfillcolor(pB->curColor);

	setlinecolor(BLACK);
	fillrectangle(pB->x, pB->y, pB->x + pB->width, pB->y + pB->height);
	// д�ı�
	settextcolor(BLACK);
	setbkmode(TRANSPARENT); //ȥ�����ֱ���
	settextstyle(pB->textSize, 0, "����");
	int textH = textheight(pB->pStr);
	int textW = textwidth(pB->pStr);
	outtextxy(pB->x + (pB->width - textW) / 2, pB->y + (pB->height - textH) / 2, pB->pStr);
}


bool Listen_Button(struct button* btn) {
	peekmessage(&mouse_msg, EM_MOUSE);
	if (mouse_msg.x >= btn->x && mouse_msg.x <= btn->width + btn->x
		&& mouse_msg.y >= btn->y && mouse_msg.y <= btn->height + btn->y) {
		btn->curColor = btn->inColor;
		if (mouse_msg.lbutton) {
			Sleep(50); // �������������
			return true;
		}
		return false;
	}
	else {
		btn->curColor = btn->outColor;
		return false;
	}
}


#endif
