#pragma once
#include <iostream>
#include <iomanip>
#include <time.h>
#include <vector>
#include <queue>
#include <algorithm>
#include <windows.h>
#include "define.h"

using namespace std;


class CGame
{
private:                       //0  1  2  3  4
	vector<vector<int>> map;   //╨Л бл ю╤ во ╩ф
	int score;
	int count;
	void dfs_find(int x, int y);
public:
	CGame();
	void ReGame();
	void Print(HDC hdc);
	void Change(int x, int y, char direction);
	bool Remove();
};