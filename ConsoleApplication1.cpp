// ConsoleApplication1.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "pch.h"
#include <iostream>
#include<windows.h>
#include<vector>
#include<deque>
#include<time.h>
#include <GdiPlus.h>
//#include<stdio.h>
using namespace std;
HANDLE hMutex = NULL;//互斥量
enum fangxiang{Up,Down,Left,Right};
fangxiang dir=Up;
//画圆
void circle(HDC hdc,int radius,int x,int y,COLORREF color) {
	
	HPEN hpen = CreatePen(PS_SOLID, 0, color);
	HBRUSH brush = (HBRUSH)CreateSolidBrush(color);
	SelectObject(hdc, brush);
	SelectObject(hdc, hpen);
	Ellipse(hdc, x - radius, y - radius,x + radius, y + radius);
	DeleteObject(brush);
}
//键盘响应
DWORD WINAPI Fun(LPVOID lpParamter){
	int up = 0, down = 0, left = 0, right=0;
	while (1) {
		up = GetAsyncKeyState(VK_UP);
		down = GetAsyncKeyState(VK_DOWN);
		left = GetAsyncKeyState(VK_LEFT);
		right = GetAsyncKeyState(VK_RIGHT);
		if (up) {
			if (dir == Down) {up = 0; continue;}
			else {dir = Up; }
		}
		else if (down) {
			if (dir == Up) { down = 0; continue; }
			else { dir = Down;  }
		}
		else if (left) {
			if (dir == Right) { left = 0; continue; }
			else { dir = Left;  }
		}
		else if (right) {
			if (dir == Left) { right = 0; continue; }
			else { dir =Right; }
		}
	}
	return 0L;

}
//画球
pair<int, int> draw_qiu(HDC hdc, deque<pair<int, int>> sna,int radius) {
	int x_qiu = 0, y_qiu = 0;
	srand(time(NULL));
	bool bo = true;
	while (1) {
		x_qiu = (200 + rand() % 820) /20 * 20;
		y_qiu = 50 + (rand() % 520)/ 20* 20;
		for (int i = 0; i < sna.size(); i++) {
			if (x_qiu == sna[i].first && y_qiu == sna[i].second) {
				bo = false;
				break;
			}
		}
		if (bo == false)bo = true; 
		else break;
	}
	circle(hdc, radius, x_qiu, y_qiu, RGB(60, 220,110));
	return make_pair(x_qiu, y_qiu);
}
void snake(HDC hdc, int radius) {
	int x=600, y=290;
	int len_snake = 3;
	deque<pair<int, int>>sna;
	pair<int, int>temp;
	COLORREF color = RGB(255, 0, 0);
	//初始化  蛇 球
	for (int i = 0; i < len_snake; i++) {
		temp = { x,y+radius*i*2 };
		sna.push_back(temp);
	}
	color = RGB(255, 0, 0);
	for (int i = 0; i < sna.size(); i++) {
		circle(hdc, radius, sna[i].first, sna[i].second, color);
	}
	pair<int, int>posi_qiu = draw_qiu(hdc, sna, radius);
	
	while (1) {
		//WaitForSingleObject(hMutex, INFINITE);
		//擦除蛇
		color = RGB(255, 255, 255);
		for (int i = 0; i < sna.size(); i++) {
			circle(hdc, radius, sna[i].first, sna[i].second, color);
		}
		
		//向前移动，判断边界（判断相撞）
		if (dir == Up) {
			if (sna[0].second == 50) {//在边界
				for (int i = 1; i < sna.size(); i++) {//前方为自身
					if (sna[i].first == sna[0].first && sna[i].second == 550) {
						cout << "失败"; getchar();
					}
				}
				sna.push_front(make_pair(sna[0].first, 550));
				sna.pop_back();
			}
			else {
				for (int i = 1; i < sna.size(); i++) {//前方为自身
					if (sna[i].first == sna[0].first && sna[i].second == sna[0].second - 20) {
						cout << "失败"; getchar();
					}
				}
				if (sna[0].first == posi_qiu.first  && sna[0].second - 20 == posi_qiu.second) {//前方为球,吃完不再前进
					sna.push_front(posi_qiu);
					posi_qiu = draw_qiu(hdc, sna, radius);
				}
				else {
					sna.push_front(make_pair(sna[0].first, sna[0].second - 20));
					sna.pop_back();
				}
			}
		}
		else if (dir == Down) {
			if (sna[0].second == 550) {//在边界
				for (int i = 1; i < sna.size(); i++) {//前方为自身
					if (sna[i].first == sna[0].first && sna[i].second == 50) {
						cout << "失败"; getchar();
					}
				}
				sna.push_front(make_pair(sna[0].first, 50));
				sna.pop_back();
			}
			else {
				for (int i = 1; i < sna.size(); i++) {//前方为自身
					if (sna[i].first == sna[0].first && sna[i].second == sna[0].second + 20) {
						cout << "失败"; getchar();
					}
				}
				if (sna[0].first == posi_qiu.first  && sna[0].second + 20 == posi_qiu.second) {//前方为球,吃完不再前进
					sna.push_front(posi_qiu);
					posi_qiu = draw_qiu(hdc, sna, radius);
				}
				else {
					sna.push_front(make_pair(sna[0].first, sna[0].second + 20));
					sna.pop_back();
				}
			}
		}
		else if (dir == Left) {
			if (sna[0].first == 200) {//在边界
				for (int i = 1; i < sna.size(); i++) {//前方为自身
					if (sna[i].second == sna[0].second && sna[i].first == 1000) {
						cout << "失败"; getchar();
					}
				}
				sna.push_front(make_pair(1000, sna[0].second));
				sna.pop_back();
			}
			else {
				for (int i = 1; i < sna.size(); i++) {//前方为自身
					if (sna[i].second == sna[0].second && sna[i].first == sna[0].first - 20) {
						cout << "失败"; getchar();
					}
				}
				if (sna[0].second == posi_qiu.second  && sna[0].first - 20 == posi_qiu.first) {//前方为球,吃完不再前进
					sna.push_front(posi_qiu);
					posi_qiu = draw_qiu(hdc, sna, radius);
				}
				else {
					sna.push_front(make_pair(sna[0].first - 20,sna[0].second ));
					sna.pop_back();
				}
			}
		}
		else if (dir == Right) {
			if (sna[0].first == 1000) {//在边界
				for (int i = 1; i < sna.size(); i++) {//前方为自身
					if (sna[i].second == sna[0].second && sna[i].first == 200) {
						cout << "失败"; getchar();
					}
				}
				sna.push_front(make_pair(200, sna[0].second));
				sna.pop_back();
			}
			else {
				for (int i = 1; i < sna.size(); i++) {//前方为自身
					if (sna[i].second == sna[0].second && sna[i].first == sna[0].first + 20) {
						cout << "失败"; getchar();
					}
				}
				if (sna[0].second == posi_qiu.second  && sna[0].first + 20 == posi_qiu.first) {//前方为球,吃完不再前进
					sna.push_front(posi_qiu);
					posi_qiu = draw_qiu(hdc, sna, radius);
				}
				else {
					sna.push_front(make_pair( sna[0].first + 20,sna[0].second));
					sna.pop_back();
				}
			}
		}

		color = RGB(255, 0, 0);
		for (int i = 0; i < sna.size(); i++) {
			circle(hdc, radius, sna[i].first, sna[i].second, color);
		}
		Sleep(200);
		//ReleaseMutex(hMutex);
	}
}

int main()
{
	HWND hwnd = GetConsoleWindow();
	HDC hdc = GetDC(hwnd);
	HBRUSH brush = (HBRUSH)CreateSolidBrush(RGB(255,255,255));
	SelectObject(hdc, brush);
	int radius = 10;
	int frame = 50;
	Rectangle(hdc, 200-radius, 50 - radius, 1000 + radius, 550 + radius);
	DeleteObject(brush);


	HANDLE hThread = CreateThread(NULL, 0, Fun, NULL, 0, NULL);
	hMutex = CreateMutex(NULL, FALSE, TEXT("screen"));
	CloseHandle(hThread);
	
	snake(hdc, radius);
	
    std::cout << "Hello World!\n"; 
}


// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门提示: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
