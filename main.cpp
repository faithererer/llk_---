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
#define ROWCOUNT  10 //��������
#define COLCOUNT 16 //��������
#define ELEMENT_SIZE 40 //Ԫ�سߴ磨pixel��
int element_Kinds = 10; //��������
#define MAX_ELEMENT_SIZE 70
int game_Map[ROWCOUNT+3][COLCOUNT+3] = { 0 }; //����������ÿһ��Ԫ��ͼƬ
char user_id[10]="�������";
IMAGE element_imgs[300] = { 0 };
//����
// ������
POINT begin_ele = { -1,-1 }, end_ele = { -1, -1 };
enum MyEnum {
	BEGIN,
	END,
};

struct point_info {
	POINT p;
	int val;
};
stack<pair<point_info, point_info>> stack_info; //��¼�����������Ϣ
int flag = BEGIN;


/*
���ܣ����������ά���ֵ�ͼ
��������
����ֵ����
���ú�������
�ı��ȫ�ֱ�����game_Map
��ע����
*/ 
void Generate_Map() {
	vector<int> v;
	int cnt = 1,c=0;
	// �ж�ÿ��Ԫ���ظ�����Ϊż����
	int repeat = (ROWCOUNT * COLCOUNT / element_Kinds) % 2 == 0 ? ROWCOUNT * COLCOUNT / element_Kinds  : ROWCOUNT * COLCOUNT / element_Kinds+1 ;
	//���������ż������Ԫ��
	while (v.size() < ROWCOUNT * COLCOUNT) {
		for (int j = 1;j <= repeat;j++) {
			if (v.size() < ROWCOUNT * COLCOUNT) {
				v.push_back(cnt);
			}
		}
		cnt++;
	}
	srand((int)time(0));	//��ʱ�����Ϊ����
	random_shuffle(v.begin(), v.end());	// �������
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
���ܣ����ݶ�ά���ֵ�ͼ��ʾԪ��ͼƬ
��������
����ֵ����
���ú�������
�ı��ȫ�ֱ�����game_Map��element_imgs
��ע����
*/
void Draw_Map() {
	// �洢����Ԫ��ͼƬ��һά����,�±����ö�ӦͼƬ�ļ���
	for (int i = 0; i <= MAX_ELEMENT_SIZE; i++) {
		string file_path = "./Res/Elements/" + to_string(i) + ".png";
		loadimage(&element_imgs[i], file_path.c_str());
	}
	//�����������ͼ�����Ӧ
	for (int i = 1;i <= ROWCOUNT;i++) {
		for (int j = 1;j <= COLCOUNT;j++) {
				putimage(j * ELEMENT_SIZE, i * ELEMENT_SIZE, &element_imgs[game_Map[i][j]]);
				//getimage(&element_imgs[game_Map[i][j]], j * ELEMENT_SIZE, i * ELEMENT_SIZE, ELEMENT_SIZE, ELEMENT_SIZE);

		}
	}
}

/*
���ܣ��������д��ڴ�ӡ��ά����
��������
���ú�������
����ֵ����
�ı��ȫ�ֱ�������
��ע����
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
���ܣ������û����������������Ϣ�¼�
��������
���ú�������
����ֵ����
�ı��ȫ�ֱ�����begin_ele��end_ele
��ע������������
*/
void Game_control() {
	//MOUSEMSG mouse_msg;
	//mouse_msg = GetMouseMsg();

		if (mouse_msg.lbutton) {
			Sleep(40); // �������������
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
���ܣ���������һ��ֱ��ֱ������
��������p1����p2
����ֵ����
���ú�����
�ı��ȫ�ֱ�����
��ע����Ϊ������������
*/
void Draw_Tip_Line(POINT p1, POINT p2) {
	setlinecolor(RED);
	setlinestyle(PS_SOLID, 5);
	line(p1.x * ELEMENT_SIZE + 0.5 * ELEMENT_SIZE, p1.y * ELEMENT_SIZE + 0.5 * ELEMENT_SIZE, p2.x * ELEMENT_SIZE + 0.5 * ELEMENT_SIZE, p2.y * ELEMENT_SIZE + 0.5 * ELEMENT_SIZE);
}
/*
���ܣ�������֮���ֱ��ɾ��
��������p1����p2
����ֵ����
���ú�������
�ı��ȫ�ֱ�������
��ע����Ϊ������������
*/
void Delte_line(POINT p1, POINT p2) {
	setlinecolor(0x00);
	line(p1.x * ELEMENT_SIZE + 0.5 * ELEMENT_SIZE, p1.y * ELEMENT_SIZE + 0.5 * ELEMENT_SIZE, p2.x * ELEMENT_SIZE + 0.5 * ELEMENT_SIZE, p2.y * ELEMENT_SIZE + 0.5 * ELEMENT_SIZE);
}
/*
���ܣ�����һ��·������������ͨ
��������p1����p2���յ�p_Corner_1���յ�p_Corner_2
����ֵ����
���ú�����
	Delte_line
	Draw_Tip_Line
�ı��ȫ�ֱ�������
��ע����
*/
void Draw_Path(POINT p1, POINT p2, POINT p_Corner_1, POINT p_Corner_2) {
	// 1�յ�
	if (!(p_Corner_1.x == -1 && p_Corner_1.y == -1)&& (p_Corner_2.x == -1 && p_Corner_2.y == -1)) {
		
		Draw_Tip_Line(p1, p_Corner_1);
		Draw_Tip_Line(p_Corner_1, p2); FlushBatchDraw();
		Sleep(200);
		Delte_line(p1, p_Corner_1);
		Delte_line(p_Corner_1, p2);

	}
	// 2�յ�
	else if (!(p_Corner_2.x == -1 && p_Corner_2.y == -1)) {
		
		Draw_Tip_Line(p1, p_Corner_1);
		Draw_Tip_Line(p_Corner_1, p_Corner_2);
		Draw_Tip_Line(p_Corner_2, p2);		FlushBatchDraw();
		Sleep(200);
		Delte_line(p1, p_Corner_1);
		Delte_line(p_Corner_1, p_Corner_2);
		Delte_line(p_Corner_2, p2);
	
	}
	// 0�յ�
	else {
		
		Draw_Tip_Line(p1, p2);		FlushBatchDraw();
		Sleep(200);
		Delte_line(p1, p2);
		

	}
}
/*
���ܣ��ж������Ƿ�����0���յ���ͨ 
��������P1����P2��
����ֵ��bool
���ú�������
�ı��ȫ�ֱ�������
��ע������
*/
bool Is_OK_0(POINT P1, POINT P2) {
	//ͬһ�У�
	if (P1.y == P2.y|| P1.x == P2.x) {
		//ˮƽ
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
		// ��ֱ
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
���ܣ��ж������Ƿ�����0���յ���ͨ������������֮�����ͨ·��
��������P1����P2
����ֵ��bool
���ú�����
	Draw_Path
�ı��ȫ�ֱ�������
��ע�����أ���Ϊ������������
*/
bool Is_OK_0_Tip(POINT P1, POINT P2) {
	//ͬһ�У�
	if (P1.y == P2.y || P1.x == P2.x) {
		//ˮƽ
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
		// ��ֱ
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
���ܣ��ж������Ƿ�����1���յ���ͨ������������֮�����ͨ·��
��������P1����P2
����ֵ��bool
���ú�����
	Is_OK_0
	Draw_Path
�ı��ȫ�ֱ�������
��ע����Ϊ������������
*/
bool Is_Ok_1(POINT P1, POINT P2) {
	// �յ�����������������
	//��һ��
	if (Is_OK_0({ P1.x, P2.y }, P1) && Is_OK_0({ P1.x, P2.y }, P2) && game_Map[P2.y][P1.x]==0) {
		cout << "�յ��ǣ�" << P1.x << "," << P2.y << endl;
		Draw_Path(P1, P2, { P1.x, P2.y }, { -1,-1 });
		return true;
	}
	else if (Is_OK_0({ P2.x, P1.y }, P1) && Is_OK_0({ P2.x, P1.y }, P2)&&game_Map[P1.y][ P2.x]==0) {
		cout << "�յ��ǣ�" << P2.x << "," << P1.y << endl;
		Draw_Path(P1, P2, { P2.x, P1.y }, { -1,-1 });
		return true;
	}
	else return false;
}
/*
���ܣ��ж������Ƿ�����2���յ���ͨ������������֮�����ͨ·��
��������P1����P2
����ֵ��bool
���ú�����
	Is_OK_0
	Draw_Path
�ı��ȫ�ֱ�������
��ע����Ϊ������������
*/
bool Is_Ok_2(POINT P1, POINT P2) {
	POINT p1_Corner, p2_Corner;
	// ������ͬһˮƽ��,
	if (P1.y == P2.y) {
		for (int i = 0;i <= ROWCOUNT+1;i++) {
			p1_Corner.x = P1.x;p1_Corner.y = i;
			p2_Corner.x = P2.x;p2_Corner.y = i;
			//�յ㲻���������غ�
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
	// ������ͬһ��ֱ��,
	if (P1.x == P2.x) {
		for (int i = 0;i <= COLCOUNT+1;i++) {
			p1_Corner.x = i;p1_Corner.y = P1.y;
			p2_Corner.x = i;p2_Corner.y = P2.y;
			//�յ㲻���������غ�
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
	// �����ڶԽ���
	else {
		// ���϶���ɨ��
		for (int i = 0;i <= ROWCOUNT + 1;i++) {
			p1_Corner.x = P1.x; p1_Corner.y = i;
			p2_Corner.x = P2.x; p2_Corner.y = i;
			// ���㲻������յ��غϣ��ʲ����ж�
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
		// �������ɨ��
		for (int i = 0;i <= COLCOUNT + 1;i++) {
			p1_Corner.x = i; p1_Corner.y = P1.y;
			p2_Corner.x = i; p2_Corner.y = P2.y;
			// ���㲻������յ��غϣ��ʲ����ж�
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
���ܣ���ƥ����������װ��pair����ѹջ
��������p1����p2
����ֵ����
���ú�������
�ı��ȫ�ֱ�����stack_info
��ע����Ϊ������������
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
���ܣ��ж�ѡ�������Ƿ���ͨ������ͨ������һ����ͨ·��
��������P1����P2
����ֵ����
���ú�����
	Is_OK_0
	Is_OK_1
	Is_OK_2
	Draw_Path
	Push_Points_To_Stack
�ı��ȫ�ֱ�����game_Map��flag
��ע����
*/
void IsOK(POINT &P1, POINT &P2) {// ��Ҫ��&�Դ˶�ʵ�θ���
	int x1 = P1.x, x2 = P2.x, y1 = P1.x, y2 = P2.y;
	//�����Ƿ���ѡ��,�����Ƿ���ͬ,Ԫ���Ƿ���ͬ,Ԫ���Ƿ�ǿ�
	if (!((P1.x == -1 || P1.y == -1 )||( P2.x == -1 || P2.y == -1)) && !(P1.y == P2.y && P1.x == P2.x)&& game_Map[P1.y][P1.x] == game_Map[P2.y][P2.x]&& game_Map[P1.y][P1.x]&&game_Map[P2.y][P2.x]) {
		//�ڵ�ͼ�߽磬�Ƿ���ͨ//����
		//if (
		//	((P1.y == 0) || (P1.x == 0) || (P1.y == ROWCOUNT+1 ) || (P1.x == COLCOUNT+1))
		//	|| ((P2.y == 0) || (P2.x == 0) || (P2.y == ROWCOUNT+1) || (P2.x == COLCOUNT+1))
		//	) {
		//	cout << "��ͼ������" <<P1.x<<","<<P1.y<<"---"<< P2.x << "," << P2.y<< endl;
		//}
		// 0�ս�
		 if (Is_OK_0(P1, P2) ) {
			Push_Points_To_Stack(P1, P2);
			game_Map[P1.y][P1.x] = 0;
			game_Map[P2.y][P2.x] = 0;
			Draw_Path(P1, P2, { -1,-1 }, { -1,-1 });
			cout << "0�ս�����" << P1.x << "," << P1.y << "---" << P2.x << "," << P2.y << endl;;
			
			P1 = P2 = { -1,-1 };
			flag = BEGIN;
			

		}
		// 1�ս�
		else if (Is_Ok_1(P1, P2)) {
			Push_Points_To_Stack(P1, P2);
			game_Map[P1.y][P1.x] = 0;
			game_Map[P2.y][P2.x] = 0;
			cout<<"1�ս�����" << P1.x << "," << P1.y << "---" << P2.x << "," << P2.y << endl;
			P1 = P2 = { -1,-1 };
			flag = BEGIN;

		}
		// 2�ս�
		else if (Is_Ok_2(P1, P2)) {
			Push_Points_To_Stack(P1, P2);
			game_Map[P1.y][P1.x] = 0;
			game_Map[P2.y][P2.x] = 0;
			cout << "2�ս�����" << P1.x << "," << P1.y << "---" << P2.x << "," << P2.y << endl;
	
			P1 = P2 = { -1,-1 };
			flag = BEGIN;
		}
		//����ͨ������
		else if ((P1.x != -1 && P1.y != -1) && (P2.x != -1 && P2.y != -1)) {
			cout << "ƥ��ʧ��" << endl;
			P1 = P2 = { -1,-1 };
			flag = BEGIN;
		}
	}

}
/*
���ܣ���ʾ��ǰ��ͼ�п���ƥ���һ��Ԫ�أ�������·����ʾ
��������
����ֵ����
���ú�����
	Is_OK_0_Tip
	Is_OK_1
	Is_OK_2
�ı��ȫ�ֱ�������
��ע����
*/
void Tip() {
	for (int i = 1;i <= ROWCOUNT + 1;i++) {
		for (int j = 1;j <= COLCOUNT + 1;j++) {
			if (game_Map[i][j] == 0) continue; // ��ѡ��һ����Ӧ����
			for (int q = 1;q <= ROWCOUNT + 1;q++) {
				for (int r = 1;r <= COLCOUNT + 1;r++) {
					if (!game_Map[r][q]&&!(q==i&&r==j)) {//�������겻���غϣ��ҵڶ��������
						POINT P1 = { j,i }, P2 = { r,q }; 
						if (game_Map[P1.y][P1.x] == game_Map[P2.y][P2.x] ) { // ���������Ӧ����Ӧ��ͬ
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
	int check[71] = { 0 }; //�±��ʾԪ�أ�ֵΪ1��ʾ���ڸ�Ԫ��
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
	// �ó���ǰԪ������
	for (int i = 1;i <= 70;i++) {
		if (check[i]) {
			kind++;
		}
	}
	// �ж�ÿ��Ԫ���ظ�����Ϊż����
	//int repeat = (ROWCOUNT * COLCOUNT / element_Kinds) % 2 == 0 ? ROWCOUNT * COLCOUNT / element_Kinds : ROWCOUNT * COLCOUNT / element_Kinds + 1;
	int repeat = (count / kind) % 2 == 0 ? (count / kind) : (count / kind) + 1;
	//���������ż������Ԫ��
	while (v.size() < count) {
		for (int j = 1;j <= repeat;j++) {
			if (v.size() < count) {
				v.push_back(cnt);
			}
		}
		cnt++;
	}

	srand((int)time(0));	//��ʱ�����Ϊ����
	random_shuffle(v.begin(), v.end());	// �������
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
���ܣ��������ֿ�����
������
����ֵ����
���ú�����
�ı��ȫ�ֱ�����
��ע����
*/
void Remake() {
	Generate_Map_Remake();
	Print_Map();
	cout << "---������---" << endl;
}
/*
���ܣ��ж���Ϸ�Ƿ�ʤ��
��������
����ֵ��bool
���ú�����
�ı��ȫ�ֱ�����
��ע����
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
	loadimage(&BK, "./Res/background/rank_UI.jpeg"); //���ر���ͼ

	int score = 100 + score_part * 100;
	char sc[5];
	sprintf_s(sc, "%d", score); // ת����outtexttxy���Խ��յ���ʽ

	// д��rank.txt
	FILE* fp = NULL;
	fopen_s(&fp,"./Res/doc/rank.txt","a");
	if (fp == NULL) {
		cout << "�ļ���ʧ��" << endl;
	}
	else {
		fputs(user_id, fp);
		fputs(" ", fp);
		fputs(sc, fp);
		fputs("\n", fp);
	}
	while (1) {
		FlushBatchDraw();
		putimage(0, 0, &BK); //����ͼ��ʾ

		settextcolor(RED);
		setbkmode(TRANSPARENT); //ȥ�����ֱ���
		settextstyle(28, 0, "����");
		outtextxy(10, 10, "��ս�ɹ������ε÷֣�");

		settextcolor(RED);
		settextstyle(28, 0, "����");
		outtextxy(10, 40, sc);

	}

}
/*
���ܣ�������һ������ԭ�ϴ�ƥ���������Ԫ�ض�
��������
����ֵ����
���ú�����
�ı��ȫ�ֱ�����game_Map,stack_info
��ע����
*/
void Rollback() {
	if (!stack_info.empty()) {
		pair< point_info, point_info> pAir;
		pAir = stack_info.top();
		game_Map[pAir.first.p.y][pAir.first.p.x] = pAir.first.val;
		game_Map[pAir.second.p.y][pAir.second.p.x] = pAir.second.val;
		cout << "��ǰջ��"<< pAir.first.p.x<<","<< pAir.first.p.y<<">"<<pAir.first.val << endl;
		stack_info.pop();

	}
	else {
		cout << "��ǰջΪ�գ�" << endl;
	}

}
void Home_UI();
/*
���ܣ���װ��ʱģʽ��Ϸ����ʱ�ĸ���ģ��
������
����ֵ����
���ú�����
	Generate_Map
	Print_Map
	Draw_Map
	Game_control
	IsOK
	Rollback
	Tip
�ı��ȫ�ֱ�����game_Map, element_imgs, stack_info
��ע����
*/
void Play_Game_UI() {
	initgraph(ELEMENT_SIZE*COLCOUNT+300, ELEMENT_SIZE*ROWCOUNT+200, EW_SHOWCONSOLE); // ����ͼ�δ���
	
	Generate_Map();
	Print_Map();

	IMAGE BK;
	
	struct button* btn_back = create_Button(700, 50, 80, 48, "����", 10, WHITE, LTY_COLOR, WHITE);
	struct button* btn_tip = create_Button(700, 150, 80, 48, "��ʾ", 10, WHITE, LTY_COLOR, WHITE);
	struct button* btn_remake = create_Button(700, 250, 80, 48, "����", 10, WHITE, LTY_COLOR, WHITE);
	struct button* btn_return = create_Button(700, 350, 80, 48, "����", 10, WHITE, LTY_COLOR, WHITE);

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
���ܣ���װ��Ϸ����ʱ�ĸ���ģ��
������
����ֵ����
���ú�����
	Generate_Map
	Print_Map
	Draw_Map
	Game_control
	IsOK
	Rollback
	Tip
�ı��ȫ�ֱ�����game_Map, element_imgs, stack_info
��ע����
*/
void Play_Game_UI_2() {
	initgraph(ELEMENT_SIZE * COLCOUNT + 300, ELEMENT_SIZE * ROWCOUNT + 200, EW_SHOWCONSOLE); // ����ͼ�δ���
	Generate_Map();
	Print_Map();

	IMAGE BK;

	struct button* btn_back = create_Button(700, 50, 80, 48, "����", 10, WHITE, LTY_COLOR, WHITE);
	struct button* btn_tip = create_Button(700, 150, 80, 48, "��ʾ", 10, WHITE, LTY_COLOR, WHITE);
	struct button* btn_remake = create_Button(700, 250, 80, 48, "����", 10, WHITE, LTY_COLOR, WHITE);
	struct button* btn_return = create_Button(700, 350, 80, 48, "����", 10, WHITE, LTY_COLOR, WHITE);

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
���ܣ���ʾ���а���Ϣ
��������
����ֵ����
���ú�����
�ı��ȫ�ֱ�����
��ע����
*/
void Rank_UI() {
	initgraph(482, 651, EW_SHOWCONSOLE); // ����ͼ�δ���
	struct button* btn_return = create_Button(380, 580, 80, 50, "����", 10, WHITE, LTY_COLOR, WHITE);
	Read_Rank();
	rank_sort();
	ppprank();
	IMAGE BK;
	loadimage(&BK, "./Res/background/rank_UI.jpeg");
	putimage(0, 0, &BK);
	while (1) {
		for (int i = 0;i < user.size();i++) {
			settextcolor(RED);
			setbkmode(TRANSPARENT); //ȥ�����ֱ���
			settextstyle(20, 0, "����");
			char sco[20];
			char num[20];
			sprintf_s(num, "%d", i+1);
			sprintf_s(sco, "%d", user[i].score); // ת����ʽ
			outtextxy(100, 100 + 50 * i, num); // ������ʾ
			outtextxy(130, 100+50*i, user[i].name); //id��ʾ
			outtextxy(230, 100+50*i, sco); // �÷���ʾ
			FlushBatchDraw();
		}
		DrawButton(btn_return);
		if (Listen_Button(btn_return)) {
			user.clear();
			Home_UI();
		}
	}
}
// �ı����id
void Change_Player() {
	InputBox(user_id, 6, "���������Id");
}

//��ʾ�����Ϣ
void Player() {
	settextcolor(GREEN);
	setbkmode(TRANSPARENT); //ȥ�����ֱ���
	settextstyle(20, 0, "����");
	outtextxy(600, 450, "��ǰ��ң�");
	outtextxy(700, 450, user_id);
}
/*
���ܣ���װ��Ϸ��ҳ��ĸ���ģ��
��������
����ֵ����
���ú����� 
�ı��ȫ�ֱ����� 
��ע����
*/
void Home_UI() {
	initgraph(800,500, EW_SHOWCONSOLE); // ����ͼ�δ���
	IMAGE BK;
	loadimage(&BK, "./Res/background/homeUI.jpg"); // ���ر���ͼ
	struct button* btn_play_1= create_Button(600, 50, 150,75,"��ʱģʽ",10, WHITE, LTY_COLOR, WHITE);
	struct button* btn_play_2 = create_Button(600, 150, 150, 75, "����ģʽ", 10, WHITE, LTY_COLOR, WHITE);
	struct button* btn_rank = create_Button(600, 250, 150, 75, "���а�", 10, WHITE, LTY_COLOR, WHITE);
	struct button* btn_user = create_Button(600, 350, 150, 75, "�������", 10, WHITE, LTY_COLOR, WHITE);



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