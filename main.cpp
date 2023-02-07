#include<stdlib.h>
#include<graphics.h>
#include<iostream>
#include<time.h>
#include<string>
#include<algorithm>
#include<vector>
#include<stack>
#include<utility>
#include"Button.h"
#include"timer.h"
#include"Rank.h"
using namespace std;
#define ROWCOUNT  10 //生成行数
#define COLCOUNT 16 //生成列数
#define ELEMENT_SIZE 40 //元素尺寸（pixel）
int element_Kinds = 10; //精灵种类
#define MAX_ELEMENT_SIZE 70
int game_Map[ROWCOUNT+3][COLCOUNT+3] = { 0 }; //用整数代表每一个元素图片
char user_id[10]="匿名玩家";
IMAGE element_imgs[300] = { 0 };
//控制
// 鼠标控制
POINT begin_ele = { -1,-1 }, end_ele = { -1, -1 };
enum MyEnum {
	BEGIN,
	END,
};

struct point_info {
	POINT p;
	int val;
};
stack<pair<point_info, point_info>> stack_info; //记录已消除点对信息
int flag = BEGIN;


/*
功能：生成随机二维数字地图
参数：无
返回值：无
调用函数：无
改变的全局变量：game_Map
备注：无
*/ 
void Generate_Map() {
	vector<int> v;
	int cnt = 1,c=0;
	// 判定每种元素重复个数为偶数个
	int repeat = (ROWCOUNT * COLCOUNT / element_Kinds) % 2 == 0 ? ROWCOUNT * COLCOUNT / element_Kinds  : ROWCOUNT * COLCOUNT / element_Kinds+1 ;
	//生成有序的偶数个数元素
	while (v.size() < ROWCOUNT * COLCOUNT) {
		for (int j = 1;j <= repeat;j++) {
			if (v.size() < ROWCOUNT * COLCOUNT) {
				v.push_back(cnt);
			}
		}
		cnt++;
	}
	srand((int)time(0));	//以时间戳作为种子
	random_shuffle(v.begin(), v.end());	// 随机打乱
	for (int i = 1;i <= ROWCOUNT;i++) {
		for (int j = 1;j <= COLCOUNT;j++) {
			game_Map[i][j] = v[c++];
		}
	}
	//for (int i=0; i<ROWCOUNT*COLCOUNT; i++) {
	//	game_Map[i / COLCOUNT][i % COLCOUNT] = v[i];
	//}
}

/*
功能：根据二维数字地图显示元素图片
参数：无
返回值：无
调用函数：无
改变的全局变量：game_Map，element_imgs
备注：无
*/
void Draw_Map() {
	// 存储所有元素图片进一维数组,下标正好对应图片文件名
	for (int i = 0; i <= MAX_ELEMENT_SIZE; i++) {
		string file_path = "./Res/Elements/" + to_string(i) + ".png";
		loadimage(&element_imgs[i], file_path.c_str());
	}
	//根据随机数地图填入对应
	for (int i = 1;i <= ROWCOUNT;i++) {
		for (int j = 1;j <= COLCOUNT;j++) {
				putimage(j * ELEMENT_SIZE, i * ELEMENT_SIZE, &element_imgs[game_Map[i][j]]);
				//getimage(&element_imgs[game_Map[i][j]], j * ELEMENT_SIZE, i * ELEMENT_SIZE, ELEMENT_SIZE, ELEMENT_SIZE);

		}
	}
}

/*
功能：在命令行窗口打印二维数组
参数：无
调用函数：无
返回值：无
改变的全局变量：无
备注：无
*/
void Print_Map() {
	for (int i = 0; i < ROWCOUNT; i++) {
		for (int j = 0; j < COLCOUNT; j++) {
			printf("%3d", game_Map[i][j]);
		}
		cout << endl;
	}
}
/*
功能：启动用户操作，监听鼠标消息事件
参数：无
调用函数：无
返回值：无
改变的全局变量：begin_ele，end_ele
备注：监控鼠标的左击
*/
void Game_control() {
	//MOUSEMSG mouse_msg;
	//mouse_msg = GetMouseMsg();

		if (mouse_msg.lbutton) {
			Sleep(40); // 消除鼠标物理抖动
			if (flag == BEGIN) {
				begin_ele.x = mouse_msg.x/ELEMENT_SIZE;
				begin_ele.y = mouse_msg.y/ELEMENT_SIZE;
				cout << "first:" << begin_ele.x <<","<< begin_ele.y << "-->" << game_Map[begin_ele.y][begin_ele.x]<< endl;
				flag = END;
			}
			else if (flag == END) {
				end_ele.x = mouse_msg.x/ ELEMENT_SIZE;
				end_ele.y = mouse_msg.y/ ELEMENT_SIZE;
				cout << "end:"<<end_ele.x  << "," << end_ele.y <<"-->"<<game_Map[end_ele.y][end_ele.x]<< endl;
				flag = BEGIN;
			}
		}
}
/*
功能：把两点用一条直线直接连接
参数：点p1，点p2
返回值：无
调用函数：
改变的全局变量：
备注：作为后续函数调用
*/
void Draw_Tip_Line(POINT p1, POINT p2) {
	setlinecolor(RED);
	setlinestyle(PS_SOLID, 5);
	line(p1.x * ELEMENT_SIZE + 0.5 * ELEMENT_SIZE, p1.y * ELEMENT_SIZE + 0.5 * ELEMENT_SIZE, p2.x * ELEMENT_SIZE + 0.5 * ELEMENT_SIZE, p2.y * ELEMENT_SIZE + 0.5 * ELEMENT_SIZE);
}
/*
功能：把两点之间的直线删除
参数：点p1，点p2
返回值：无
调用函数：无
改变的全局变量：无
备注：作为后续函数调用
*/
void Delte_line(POINT p1, POINT p2) {
	setlinecolor(0x00);
	line(p1.x * ELEMENT_SIZE + 0.5 * ELEMENT_SIZE, p1.y * ELEMENT_SIZE + 0.5 * ELEMENT_SIZE, p2.x * ELEMENT_SIZE + 0.5 * ELEMENT_SIZE, p2.y * ELEMENT_SIZE + 0.5 * ELEMENT_SIZE);
}
/*
功能：绘制一条路径，把两点连通
参数：点p1，点p2，拐点p_Corner_1，拐点p_Corner_2
返回值：无
调用函数：
	Delte_line
	Draw_Tip_Line
改变的全局变量：无
备注：无
*/
void Draw_Path(POINT p1, POINT p2, POINT p_Corner_1, POINT p_Corner_2) {
	// 1拐点
	if (!(p_Corner_1.x == -1 && p_Corner_1.y == -1)&& (p_Corner_2.x == -1 && p_Corner_2.y == -1)) {
		
		Draw_Tip_Line(p1, p_Corner_1);
		Draw_Tip_Line(p_Corner_1, p2); FlushBatchDraw();
		Sleep(200);
		Delte_line(p1, p_Corner_1);
		Delte_line(p_Corner_1, p2);

	}
	// 2拐点
	else if (!(p_Corner_2.x == -1 && p_Corner_2.y == -1)) {
		
		Draw_Tip_Line(p1, p_Corner_1);
		Draw_Tip_Line(p_Corner_1, p_Corner_2);
		Draw_Tip_Line(p_Corner_2, p2);		FlushBatchDraw();
		Sleep(200);
		Delte_line(p1, p_Corner_1);
		Delte_line(p_Corner_1, p_Corner_2);
		Delte_line(p_Corner_2, p2);
	
	}
	// 0拐点
	else {
		
		Draw_Tip_Line(p1, p2);		FlushBatchDraw();
		Sleep(200);
		Delte_line(p1, p2);
		

	}
}
/*
功能：判断两点是否能以0个拐点连通 
参数：点P1，点P2，
返回值：bool
调用函数：无
改变的全局变量：无
备注：重载
*/
bool Is_OK_0(POINT P1, POINT P2) {
	//同一行？
	if (P1.y == P2.y|| P1.x == P2.x) {
		//水平
		if (P1.y == P2.y) {
			int l = min(P1.x, P2.x);
			int r = max(P1.x, P2.x);
			for (int i = l + 1;i <= r - 1;i++) {
				if (game_Map[P1.y][i] != 0) {
					return false;
				}
			}
			return true;
		}
		// 垂直
		else if (P1.x == P2.x) {
			int l = min(P1.y, P2.y);
			int r = max(P1.y, P2.y);
			for (int i = l + 1;i <= r - 1;i++) {
				if (game_Map[i][P1.x] != 0) {
					return false;
				}
			}
			return true;
		}
	}
	else return false;
}
/*
功能：判断两点是否能以0个拐点连通，并绘制两点之间的连通路径
参数：点P1，点P2
返回值：bool
调用函数：
	Draw_Path
改变的全局变量：无
备注：重载，作为后续函数调用
*/
bool Is_OK_0_Tip(POINT P1, POINT P2) {
	//同一行？
	if (P1.y == P2.y || P1.x == P2.x) {
		//水平
		if (P1.y == P2.y) {
			int l = min(P1.x, P2.x);
			int r = max(P1.x, P2.x);
			for (int i = l + 1;i <= r - 1;i++) {
				if (game_Map[P1.y][i] != 0) {
					return false;
				}
			}
			Draw_Path(P1, P2, { -1, -1 }, { -1,-1 });
			return true;
		}
		// 垂直
		else if (P1.x == P2.x) {
			int l = min(P1.y, P2.y);
			int r = max(P1.y, P2.y);
			for (int i = l + 1;i <= r - 1;i++) {
				if (game_Map[i][P1.x] != 0) {
					return false;
				}
			}
			Draw_Path(P1, P2, { -1, -1 }, { -1,-1 });
			return true;
		}
	}
	else return false;
}
/*
功能：判断两点是否能以1个拐点连通，并绘制两点之间的连通路径
参数：点P1，点P2
返回值：bool
调用函数：
	Is_OK_0
	Draw_Path
改变的全局变量：无
备注：作为后续函数调用
*/
bool Is_Ok_1(POINT P1, POINT P2) {
	// 拐点有两个，分类讨论
	//第一种
	if (Is_OK_0({ P1.x, P2.y }, P1) && Is_OK_0({ P1.x, P2.y }, P2) && game_Map[P2.y][P1.x]==0) {
		cout << "拐点是：" << P1.x << "," << P2.y << endl;
		Draw_Path(P1, P2, { P1.x, P2.y }, { -1,-1 });
		return true;
	}
	else if (Is_OK_0({ P2.x, P1.y }, P1) && Is_OK_0({ P2.x, P1.y }, P2)&&game_Map[P1.y][ P2.x]==0) {
		cout << "拐点是：" << P2.x << "," << P1.y << endl;
		Draw_Path(P1, P2, { P2.x, P1.y }, { -1,-1 });
		return true;
	}
	else return false;
}
/*
功能：判断两点是否能以2个拐点连通，并绘制两点之间的连通路径
参数：点P1，点P2
返回值：bool
调用函数：
	Is_OK_0
	Draw_Path
改变的全局变量：无
备注：作为后续函数调用
*/
bool Is_Ok_2(POINT P1, POINT P2) {
	POINT p1_Corner, p2_Corner;
	// 两点在同一水平线,
	if (P1.y == P2.y) {
		for (int i = 0;i <= ROWCOUNT+1;i++) {
			p1_Corner.x = P1.x;p1_Corner.y = i;
			p2_Corner.x = P2.x;p2_Corner.y = i;
			//拐点不可与两点重合
			if (p1_Corner.y != P1.y) {
				if (
					Is_OK_0(P1, p1_Corner) && game_Map[p1_Corner.y][p1_Corner.x] == 0 &&
					Is_OK_0(p2_Corner, p1_Corner) && game_Map[p1_Corner.y][p1_Corner.x] == 0 && game_Map[p2_Corner.y][p2_Corner.x] == 0 &&
					Is_OK_0(P2, p2_Corner) && game_Map[p2_Corner.y][p2_Corner.x] == 0
					) {
					Draw_Path(P1, P2, p1_Corner, p2_Corner);
					return true;
				}
				else continue;
			}
		}
		return false;
	}
	// 两点在同一竖直线,
	if (P1.x == P2.x) {
		for (int i = 0;i <= COLCOUNT+1;i++) {
			p1_Corner.x = i;p1_Corner.y = P1.y;
			p2_Corner.x = i;p2_Corner.y = P2.y;
			//拐点不可与两点重合
			if (p1_Corner.x != P1.x) {
				if (
					Is_OK_0(P1, p1_Corner) && game_Map[p1_Corner.y][p1_Corner.x] == 0 &&
					Is_OK_0(p2_Corner, p1_Corner) && game_Map[p1_Corner.y][p1_Corner.x] == 0 && game_Map[p2_Corner.y][p2_Corner.x] == 0 &&
					Is_OK_0(P2, p2_Corner) && game_Map[p2_Corner.y][p2_Corner.x] == 0
					) {
					Draw_Path(P1, P2, p1_Corner, p2_Corner);
					return true;
				}
				else continue;
			}
		}
		return false;
	}
	// 两点在对角线
	else {
		// 自上而下扫描
		for (int i = 0;i <= ROWCOUNT + 1;i++) {
			p1_Corner.x = P1.x; p1_Corner.y = i;
			p2_Corner.x = P2.x; p2_Corner.y = i;
			// 两点不可能与拐点重合，故不需判断
			if (
				Is_OK_0(P1, p1_Corner) && game_Map[p1_Corner.y][p1_Corner.x] == 0 &&
				Is_OK_0(p2_Corner, p1_Corner) && game_Map[p1_Corner.y][p1_Corner.x] == 0 && game_Map[p2_Corner.y][p2_Corner.x] == 0 &&
				Is_OK_0(P2, p2_Corner) && game_Map[p2_Corner.y][p2_Corner.x] == 0
				) {
				Draw_Path(P1, P2, p1_Corner, p2_Corner);
				return true;
			}
			else continue;
		}
		// 自左而右扫描
		for (int i = 0;i <= COLCOUNT + 1;i++) {
			p1_Corner.x = i; p1_Corner.y = P1.y;
			p2_Corner.x = i; p2_Corner.y = P2.y;
			// 两点不可能与拐点重合，故不需判断
			if (
				Is_OK_0(P1, p1_Corner) && game_Map[p1_Corner.y][p1_Corner.x] == 0 &&
				Is_OK_0(p2_Corner, p1_Corner) && game_Map[p1_Corner.y][p1_Corner.x] == 0 && game_Map[p2_Corner.y][p2_Corner.x] == 0 &&
				Is_OK_0(P2, p2_Corner) && game_Map[p2_Corner.y][p2_Corner.x] == 0
				) {
				Draw_Path(P1, P2, p1_Corner,  p2_Corner);
				return true;
			}
			else continue;
		}

	}
	return false;
}
/*
功能：把匹配的两个点封装进pair，并压栈
参数：点p1，点p2
返回值：无
调用函数：无
改变的全局变量：stack_info
备注：作为后续函数调用
*/
void Push_Points_To_Stack(POINT P1, POINT P2) {
	pair< point_info, point_info> pAir;
	struct point_info p_info1;
	struct point_info p_info2;
	p_info1.p.x = P1.x;p_info1.p.y=P1.y;p_info1.val = game_Map[P1.y][P1.x];
	p_info2.p.x = P2.x;p_info2.p.y = P2.y;p_info2.val = game_Map[P2.y][P2.x];
	pAir.first = p_info1;
	pAir.second = p_info2;
	stack_info.push(pAir);
}
/*
功能：判断选中两点是否连通，若连通，绘制一条连通路径
参数：点P1，点P2
返回值：无
调用函数：
	Is_OK_0
	Is_OK_1
	Is_OK_2
	Draw_Path
	Push_Points_To_Stack
改变的全局变量：game_Map，flag
备注：无
*/
void IsOK(POINT &P1, POINT &P2) {// 需要加&以此对实参更改
	int x1 = P1.x, x2 = P2.x, y1 = P1.x, y2 = P2.y;
	//两点是否都已选中,坐标是否相同,元素是否相同,元素是否非空
	if (!((P1.x == -1 || P1.y == -1 )||( P2.x == -1 || P2.y == -1)) && !(P1.y == P2.y && P1.x == P2.x)&& game_Map[P1.y][P1.x] == game_Map[P2.y][P2.x]&& game_Map[P1.y][P1.x]&&game_Map[P2.y][P2.x]) {
		//在地图边界，是否连通//多余
		//if (
		//	((P1.y == 0) || (P1.x == 0) || (P1.y == ROWCOUNT+1 ) || (P1.x == COLCOUNT+1))
		//	|| ((P2.y == 0) || (P2.x == 0) || (P2.y == ROWCOUNT+1) || (P2.x == COLCOUNT+1))
		//	) {
		//	cout << "非图块区域" <<P1.x<<","<<P1.y<<"---"<< P2.x << "," << P2.y<< endl;
		//}
		// 0拐角
		 if (Is_OK_0(P1, P2) ) {
			Push_Points_To_Stack(P1, P2);
			game_Map[P1.y][P1.x] = 0;
			game_Map[P2.y][P2.x] = 0;
			Draw_Path(P1, P2, { -1,-1 }, { -1,-1 });
			cout << "0拐角消除" << P1.x << "," << P1.y << "---" << P2.x << "," << P2.y << endl;;
			
			P1 = P2 = { -1,-1 };
			flag = BEGIN;
			

		}
		// 1拐角
		else if (Is_Ok_1(P1, P2)) {
			Push_Points_To_Stack(P1, P2);
			game_Map[P1.y][P1.x] = 0;
			game_Map[P2.y][P2.x] = 0;
			cout<<"1拐角消除" << P1.x << "," << P1.y << "---" << P2.x << "," << P2.y << endl;
			P1 = P2 = { -1,-1 };
			flag = BEGIN;

		}
		// 2拐角
		else if (Is_Ok_2(P1, P2)) {
			Push_Points_To_Stack(P1, P2);
			game_Map[P1.y][P1.x] = 0;
			game_Map[P2.y][P2.x] = 0;
			cout << "2拐角消除" << P1.x << "," << P1.y << "---" << P2.x << "," << P2.y << endl;
	
			P1 = P2 = { -1,-1 };
			flag = BEGIN;
		}
		//非连通，重置
		else if ((P1.x != -1 && P1.y != -1) && (P2.x != -1 && P2.y != -1)) {
			cout << "匹配失败" << endl;
			P1 = P2 = { -1,-1 };
			flag = BEGIN;
		}
	}

}
/*
功能：提示当前地图中可以匹配的一对元素，并绘制路径提示
参数：无
返回值：无
调用函数：
	Is_OK_0_Tip
	Is_OK_1
	Is_OK_2
改变的全局变量：无
备注：无
*/
void Tip() {
	for (int i = 1;i <= ROWCOUNT + 1;i++) {
		for (int j = 1;j <= COLCOUNT + 1;j++) {
			if (game_Map[i][j] == 0) continue; // 所选第一个点应非零
			for (int q = 1;q <= ROWCOUNT + 1;q++) {
				for (int r = 1;r <= COLCOUNT + 1;r++) {
					if (!game_Map[r][q]&&!(q==i&&r==j)) {//两点坐标不能重合，且第二个点非零
						POINT P1 = { j,i }, P2 = { r,q }; 
						if (game_Map[P1.y][P1.x] == game_Map[P2.y][P2.x] ) { // 两点数组对应数字应相同
							if (Is_OK_0_Tip(P1, P2) || Is_Ok_1(P1, P2) || Is_Ok_2(P1, P2)) {
								return;
							}
						}
					}
				}
			}
		}
	}
}

void Generate_Map_Remake() {
	int check[71] = { 0 }; //下标表示元素，值为1表示存在该元素
	vector<int> v;
	int cnt = 1, c = 0, count = 0, kind = 0;
	for (int i = 1;i <= ROWCOUNT;i++) {
		for (int j = 1;j <= COLCOUNT;j++) {
			if (game_Map[i][j] != 0) {
				count++;
				check[game_Map[i][j]] = 1;
			}
		}
	}
	// 得出当前元素种类
	for (int i = 1;i <= 70;i++) {
		if (check[i]) {
			kind++;
		}
	}
	// 判定每种元素重复个数为偶数个
	//int repeat = (ROWCOUNT * COLCOUNT / element_Kinds) % 2 == 0 ? ROWCOUNT * COLCOUNT / element_Kinds : ROWCOUNT * COLCOUNT / element_Kinds + 1;
	int repeat = (count / kind) % 2 == 0 ? (count / kind) : (count / kind) + 1;
	//生成有序的偶数个数元素
	while (v.size() < count) {
		for (int j = 1;j <= repeat;j++) {
			if (v.size() < count) {
				v.push_back(cnt);
			}
		}
		cnt++;
	}

	srand((int)time(0));	//以时间戳作为种子
	random_shuffle(v.begin(), v.end());	// 随机打乱
	for (int i = 1;i <= ROWCOUNT;i++) {
		for (int j = 1;j <= COLCOUNT;j++) {
			if (game_Map[i][j]) {
				game_Map[i][j] = v[c++];
			}
		}
	}
	//for (int i=0; i<ROWCOUNT*COLCOUNT; i++) {
	//	game_Map[i / COLCOUNT][i % COLCOUNT] = v[i];
	//}
}
/*
功能：如遇死局可重排
参数：
返回值：无
调用函数：
改变的全局变量：
备注：无
*/
void Remake() {
	Generate_Map_Remake();
	Print_Map();
	cout << "---已重置---" << endl;
}
/*
功能：判断游戏是否胜利
参数：无
返回值：bool
调用函数：
改变的全局变量：
备注：无
*/
bool Is_Win() {
	for (int i = 1;i <= ROWCOUNT  ;i++) {
		for (int j = 1;j <= COLCOUNT ;j++) {				
			if (game_Map[i][j]) {
				return false;
			}
		}
	}
	return true;
}
void Win_UI(double score_part) {
	initgraph(400, 400, 1);
	IMAGE BK;
	loadimage(&BK, "./Res/background/rank_UI.jpeg"); //加载背景图

	int score = 100 + score_part * 100;
	char sc[5];
	sprintf_s(sc, "%d", score); // 转换成outtexttxy可以接收的形式

	// 写入rank.txt
	FILE* fp = NULL;
	fopen_s(&fp,"./Res/doc/rank.txt","a");
	if (fp == NULL) {
		cout << "文件打开失败" << endl;
	}
	else {
		fputs(user_id, fp);
		fputs(" ", fp);
		fputs(sc, fp);
		fputs("\n", fp);
	}
	while (1) {
		FlushBatchDraw();
		putimage(0, 0, &BK); //背景图显示

		settextcolor(RED);
		setbkmode(TRANSPARENT); //去掉文字背景
		settextstyle(28, 0, "楷体");
		outtextxy(10, 10, "挑战成功！本次得分：");

		settextcolor(RED);
		settextstyle(28, 0, "楷体");
		outtextxy(10, 40, sc);

	}

}
/*
功能：撤回上一步，还原上次匹配后消除的元素对
参数：无
返回值：无
调用函数：
改变的全局变量：game_Map,stack_info
备注：无
*/
void Rollback() {
	if (!stack_info.empty()) {
		pair< point_info, point_info> pAir;
		pAir = stack_info.top();
		game_Map[pAir.first.p.y][pAir.first.p.x] = pAir.first.val;
		game_Map[pAir.second.p.y][pAir.second.p.x] = pAir.second.val;
		cout << "当前栈顶"<< pAir.first.p.x<<","<< pAir.first.p.y<<">"<<pAir.first.val << endl;
		stack_info.pop();

	}
	else {
		cout << "当前栈为空！" << endl;
	}

}
void Home_UI();
/*
功能：封装限时模式游戏进行时的各种模块
参数：
返回值：无
调用函数：
	Generate_Map
	Print_Map
	Draw_Map
	Game_control
	IsOK
	Rollback
	Tip
改变的全局变量：game_Map, element_imgs, stack_info
备注：无
*/
void Play_Game_UI() {
	initgraph(ELEMENT_SIZE*COLCOUNT+300, ELEMENT_SIZE*ROWCOUNT+200, EW_SHOWCONSOLE); // 创建图形窗口
	
	Generate_Map();
	Print_Map();

	IMAGE BK;
	
	struct button* btn_back = create_Button(700, 50, 80, 48, "撤回", 10, WHITE, LTY_COLOR, WHITE);
	struct button* btn_tip = create_Button(700, 150, 80, 48, "提示", 10, WHITE, LTY_COLOR, WHITE);
	struct button* btn_remake = create_Button(700, 250, 80, 48, "重排", 10, WHITE, LTY_COLOR, WHITE);
	struct button* btn_return = create_Button(700, 350, 80, 48, "返回", 10, WHITE, LTY_COLOR, WHITE);

	struct Timer* timer = creat_Timer(300, 700, 30, GREEN);
	loadimage(&BK, "./Res/background/play_game_UI.jpg");
	double score_part;
	BeginBatchDraw();
	while (1) {
		
		FlushBatchDraw();
 
		putimage(0, 0, &BK);
		
		Draw_Map();
		Game_control();
		IsOK(begin_ele, end_ele);
		DrawButton(btn_back);
		DrawButton(btn_return);
		DrawButton(btn_tip);
		DrawButton(btn_remake);
		score_part = Draw_Timer(timer);
		if (Listen_Button(btn_back)) {
			Rollback();
		}
		if (Listen_Button(btn_tip)) {
			Tip();
			FlushBatchDraw();
		}
		if (Is_Win()) {
			Win_UI(score_part);
		}
		if (Listen_Button(btn_remake)) {
			Remake();
		}
		if (Listen_Button(btn_return)) {
			Home_UI();
		}
	}
	EndBatchDraw();
}
/*
功能：封装游戏进行时的各种模块
参数：
返回值：无
调用函数：
	Generate_Map
	Print_Map
	Draw_Map
	Game_control
	IsOK
	Rollback
	Tip
改变的全局变量：game_Map, element_imgs, stack_info
备注：无
*/
void Play_Game_UI_2() {
	initgraph(ELEMENT_SIZE * COLCOUNT + 300, ELEMENT_SIZE * ROWCOUNT + 200, EW_SHOWCONSOLE); // 创建图形窗口
	Generate_Map();
	Print_Map();

	IMAGE BK;

	struct button* btn_back = create_Button(700, 50, 80, 48, "撤回", 10, WHITE, LTY_COLOR, WHITE);
	struct button* btn_tip = create_Button(700, 150, 80, 48, "提示", 10, WHITE, LTY_COLOR, WHITE);
	struct button* btn_remake = create_Button(700, 250, 80, 48, "重排", 10, WHITE, LTY_COLOR, WHITE);
	struct button* btn_return = create_Button(700, 350, 80, 48, "返回", 10, WHITE, LTY_COLOR, WHITE);

	loadimage(&BK, "./Res/background/play_game_UI.jpg");

	double score_part=0;
	BeginBatchDraw();
	while (1) {

		FlushBatchDraw();		putimage(0, 0, &BK);
		Draw_Map();
		Game_control();
		IsOK(begin_ele, end_ele);
		DrawButton(btn_back);
		DrawButton(btn_return);
		DrawButton(btn_remake);
		DrawButton(btn_tip);
		if (Listen_Button(btn_back)) {
			Rollback();
		}
		if (Listen_Button(btn_tip)) {
			Tip();
			FlushBatchDraw();
		}
		if (Is_Win()) {
			Win_UI(score_part);
		}
		if (Listen_Button(btn_remake)) {
			Remake();
		}
		if (Listen_Button(btn_return)) {
			Home_UI();
		}
	}
	EndBatchDraw();
}


/*
功能：显示排行榜信息
参数：无
返回值：无
调用函数：
改变的全局变量：
备注：无
*/
void Rank_UI() {
	initgraph(482, 651, EW_SHOWCONSOLE); // 创建图形窗口
	struct button* btn_return = create_Button(380, 580, 80, 50, "返回", 10, WHITE, LTY_COLOR, WHITE);
	Read_Rank();
	rank_sort();
	ppprank();
	IMAGE BK;
	loadimage(&BK, "./Res/background/rank_UI.jpeg");
	putimage(0, 0, &BK);
	while (1) {
		for (int i = 0;i < user.size();i++) {
			settextcolor(RED);
			setbkmode(TRANSPARENT); //去掉文字背景
			settextstyle(20, 0, "楷体");
			char sco[20];
			char num[20];
			sprintf_s(num, "%d", i+1);
			sprintf_s(sco, "%d", user[i].score); // 转换格式
			outtextxy(100, 100 + 50 * i, num); // 排名显示
			outtextxy(130, 100+50*i, user[i].name); //id显示
			outtextxy(230, 100+50*i, sco); // 得分显示
			FlushBatchDraw();
		}
		DrawButton(btn_return);
		if (Listen_Button(btn_return)) {
			user.clear();
			Home_UI();
		}
	}
}
// 改变玩家id
void Change_Player() {
	InputBox(user_id, 6, "请输入玩家Id");
}

//显示玩家信息
void Player() {
	settextcolor(GREEN);
	setbkmode(TRANSPARENT); //去掉文字背景
	settextstyle(20, 0, "楷体");
	outtextxy(600, 450, "当前玩家：");
	outtextxy(700, 450, user_id);
}
/*
功能：封装游戏主页面的各种模块
参数：无
返回值：无
调用函数： 
改变的全局变量： 
备注：无
*/
void Home_UI() {
	initgraph(800,500, EW_SHOWCONSOLE); // 创建图形窗口
	IMAGE BK;
	loadimage(&BK, "./Res/background/homeUI.jpg"); // 加载背景图
	struct button* btn_play_1= create_Button(600, 50, 150,75,"限时模式",10, WHITE, LTY_COLOR, WHITE);
	struct button* btn_play_2 = create_Button(600, 150, 150, 75, "休闲模式", 10, WHITE, LTY_COLOR, WHITE);
	struct button* btn_rank = create_Button(600, 250, 150, 75, "排行榜", 10, WHITE, LTY_COLOR, WHITE);
	struct button* btn_user = create_Button(600, 350, 150, 75, "更换玩家", 10, WHITE, LTY_COLOR, WHITE);



	BeginBatchDraw();
	while (1) {
		putimage(0, 0, &BK);

		Player();

		DrawButton(btn_play_1);
		DrawButton(btn_play_2);
		DrawButton(btn_rank);
		DrawButton(btn_user);

		FlushBatchDraw();
		if (Listen_Button(btn_play_1)) {
			Play_Game_UI();
		}
		if (Listen_Button(btn_play_2)) {
			Play_Game_UI_2();
		}
		if (Listen_Button(btn_rank)) {
			Rank_UI();
		}
		if (Listen_Button(btn_user)) {
			Change_Player();
		}
	}
	EndBatchDraw();
}

int main() {
	Home_UI();
}