#pragma once
#include<graphics.h>
#include<time.h>
/*
功能：返回当前剩余的时间
参数：
返回值：无
调用函数：无
改变的全局变量：无
备注：作为后续函数调用
*/

struct Timer {
	int limit = 0; // 倒计时时长
	int height = 0; // 计时器高度
	int width = 0; // 计时器宽度
	int x = 0; // 位置坐标x
	int y = 0; //位置坐标y
	COLORREF timer_color = BLACK; //计时器颜色
	int time_left; // 剩余时间
	int time_begin=time(0); //开始时间戳
};

Timer* creat_Timer(int limit, int w, int h, int color) {
	struct Timer* t = new Timer;

	t->limit = limit;
	t->width = w;
	t->height = h;
	t->timer_color = color;

	return t;

}
/*
功能：
参数：
返回值：无
调用函数：无
改变的全局变量：无
备注：作为后续函数调用
*/

double Draw_Timer(Timer* timer) {
	timer->time_left = time(0)- timer->time_begin;
	setlinecolor(BLACK);
	fillrectangle(timer->x, timer->y, timer->x + timer->width, timer->y + timer->height);
	setfillcolor(timer->timer_color);
	fillrectangle(timer->x, timer->y, timer->x + (int)1.0*timer->width*((double)1.0*(timer->limit- timer->time_left)/timer->limit), timer->y + timer->height);
	return 1.0 * (timer->limit - timer->time_left) / timer->limit;
}