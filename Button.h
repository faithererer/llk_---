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
	int height = 0; // 按钮高度
	int width = 0; // 按钮宽度
	int x = 0; // x位置坐标
	int y = 0; // y位置坐标
	COLORREF curColor = BLACK; // 当前按钮颜色
	COLORREF inColor = BLACK; // 未触碰时按钮颜色
	COLORREF outColor = BLACK; // 触碰时按钮颜色
	char pStr[101] = "?"; // 文本
	int textSize = 0; // 文本大小
	COLORREF textColor = BLACK; //字体颜色
};

// x坐标,y坐标,按钮宽度，按钮高度，文本，文本大小，当前按钮颜色，鼠标触碰颜色，鼠标非触碰颜色
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
	// 画按钮
	setfillcolor(pB->curColor);

	setlinecolor(BLACK);
	fillrectangle(pB->x, pB->y, pB->x + pB->width, pB->y + pB->height);
	// 写文本
	settextcolor(BLACK);
	setbkmode(TRANSPARENT); //去掉文字背景
	settextstyle(pB->textSize, 0, "楷体");
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
			Sleep(50); // 消除鼠标物理抖动
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
