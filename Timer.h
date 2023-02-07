#pragma once
#include<graphics.h>
#include<time.h>
/*
���ܣ����ص�ǰʣ���ʱ��
������
����ֵ����
���ú�������
�ı��ȫ�ֱ�������
��ע����Ϊ������������
*/

struct Timer {
	int limit = 0; // ����ʱʱ��
	int height = 0; // ��ʱ���߶�
	int width = 0; // ��ʱ�����
	int x = 0; // λ������x
	int y = 0; //λ������y
	COLORREF timer_color = BLACK; //��ʱ����ɫ
	int time_left; // ʣ��ʱ��
	int time_begin=time(0); //��ʼʱ���
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
���ܣ�
������
����ֵ����
���ú�������
�ı��ȫ�ֱ�������
��ע����Ϊ������������
*/

double Draw_Timer(Timer* timer) {
	timer->time_left = time(0)- timer->time_begin;
	setlinecolor(BLACK);
	fillrectangle(timer->x, timer->y, timer->x + timer->width, timer->y + timer->height);
	setfillcolor(timer->timer_color);
	fillrectangle(timer->x, timer->y, timer->x + (int)1.0*timer->width*((double)1.0*(timer->limit- timer->time_left)/timer->limit), timer->y + timer->height);
	return 1.0 * (timer->limit - timer->time_left) / timer->limit;
}