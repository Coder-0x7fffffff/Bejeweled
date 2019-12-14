#include "stdafx.h"
#include "CGame.h"

CGame::CGame()
{
	srand((unsigned int)time(NULL));
	map.resize(10);
	for (int i = 0; i < 10; i++)
	{
		map.at(i).resize(10);
		for (int j = 0; j < 10; j++)
		{
			map[i][j] = rand() % 5;
		}
	}
	while (Remove());
	score = 0;
}

void CGame::ReGame()
{
	srand((unsigned int)time(NULL));
	map.resize(10);
	for (int i = 0; i < 10; i++)
	{
		map.at(i).resize(10);
		for (int j = 0; j < 10; j++)
		{
			map[i][j] = rand() % 5;
		}
	}
	while (Remove());
	score = 0;
}

void CGame::Print(HDC hdc)
{
	RECT rec;
	wchar_t str[100];
	HBRUSH brush;
	for (int j = 0; j < 10; j++)
	{
		wsprintf(str, TEXT("%d"), j + 1);
		rec.top = FRAME_GAP_HEIGHT + BLOCK_WIDE * 10;
		rec.bottom = FRAME_GAP_HEIGHT + BLOCK_WIDE * 10 + FRAME_GAP_HEIGHT;
		rec.left = FRAME_GAP_WIDE*2 + BLOCK_WIDE * j;
		rec.right = FRAME_GAP_WIDE*2 + BLOCK_WIDE * (j + 1);
		DrawText(hdc, str, wcslen(str), &rec, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		for (int i = 0; i < 10; i++)
		{
			switch (map[i][9-j])//�� �� �� �� ��
			{
			case 0:
				brush = CreateSolidBrush(RGB(255,0,0));
				break;
			case 1:
				brush = CreateSolidBrush(RGB(0, 255, 0));
				break;
			case 2:
				brush = CreateSolidBrush(RGB(0, 0, 255));
				break;
			case 3:
				brush = CreateSolidBrush(RGB(160, 32, 240));
				break;
			case 4:
				brush = CreateSolidBrush(RGB(255, 255, 0));
				break;
			default:
				brush = CreateSolidBrush(RGB(255, 255, 255));
			}
			SelectObject(hdc, brush);
			Rectangle(hdc,FRAME_GAP_WIDE*2 + BLOCK_WIDE * i,
				FRAME_GAP_HEIGHT + BLOCK_WIDE * j,
				FRAME_GAP_WIDE*2 + BLOCK_WIDE * (i+1),
				FRAME_GAP_HEIGHT + BLOCK_WIDE * (j+1)
			);
			DeleteObject(brush);
		}
	}
	for (int i = 0; i < 10; i++) 
	{
		wsprintf(str, TEXT("%d"), 10 - i );
		rec.top = FRAME_GAP_HEIGHT + BLOCK_WIDE * i;
		rec.bottom = FRAME_GAP_HEIGHT + BLOCK_WIDE * (i + 1);
		rec.left = FRAME_GAP_WIDE;
		rec.right = FRAME_GAP_WIDE*2;
		DrawText(hdc, str, wcslen(str), &rec, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	}
	///score
	wsprintf(str, TEXT("      ���ĵ÷���:%d"), score);
	rec.top = FRAME_GAP_HEIGHT + BLOCK_WIDE * 10;
	rec.bottom = FRAME_GAP_HEIGHT + BLOCK_WIDE * 11;
	rec.left = FRAME_GAP_WIDE;
	rec.right = FRAME_GAP_WIDE * 2 + BLOCK_WIDE * 5;
	DrawText(hdc, str, wcslen(str), &rec, DT_VCENTER | DT_SINGLELINE);
	//help
	wsprintf(str, TEXT("      ���ѡ�񽻻����飬�Ҽ�ȡ��ѡ��"), score);
	rec.top = FRAME_GAP_HEIGHT + BLOCK_WIDE * 10 + 20;
	rec.bottom = FRAME_GAP_HEIGHT + BLOCK_WIDE * 11 + 20;
	rec.left = FRAME_GAP_WIDE;
	rec.right = FRAME_GAP_WIDE * 2 + BLOCK_WIDE * 5;
	DrawText(hdc, str, wcslen(str), &rec, DT_VCENTER | DT_SINGLELINE);
}

void CGame::Change(int x, int y, char direction)
{
	int temp;
	switch (direction)
	{
	case 'u':
		if (y + 1 >= 10)  //Խ�緵��
			break;
		temp = map[x][y];  //�����滻
		map[x][y] = map[x][y + 1];
		map[x][y + 1] = temp;
		if (Remove())  //���Ƿ����ɾ��
		{
			while (Remove());
		}
		else // ����ɾ������
		{
			temp = map[x][y];
			map[x][y] = map[x][y + 1];
			map[x][y + 1] = temp;
		}
		break;
	case 'd':
		if (y - 1 < 0)
			break;
		temp = map[x][y];
		map[x][y] = map[x][y - 1];
		map[x][y - 1] = temp;
		if (Remove())
		{
			while (Remove());
		}
		else
		{
			temp = map[x][y];
			map[x][y] = map[x][y - 1];
			map[x][y - 1] = temp;
		}
		break;
	case 'l':
		if (x - 1 < 0)
			break;
		temp = map[x][y];
		map[x][y] = map[x - 1][y];
		map[x - 1][y] = temp;
		if (Remove())
		{
			while (Remove());
		}
		else 
		{
			temp = map[x][y];
			map[x][y] = map[x - 1][y];
			map[x - 1][y] = temp;
		}
		break;
	case 'r':
		if (x + 1 >= 10)
			break;
		temp = map[x][y];
		map[x][y] = map[x + 1][y];
		map[x + 1][y] = temp;
		if (Remove())
		{
			while (Remove());
		}
		else
		{
			temp = map[x][y];
			map[x][y] = map[x + 1][y];
			map[x + 1][y] = temp;
		}
		break;
	}
}

void CGame::dfs_find(int x, int y)
{
	int now = map[x][y];
	map[x][y] = -1;
	if (y + 1 < 10 && map[x][y + 1] == now)  //up
	{
		dfs_find(x, y + 1);
		count++;
	}
	if (y - 1 >= 0 && map[x][y - 1] == now)  //down
	{
		dfs_find(x, y - 1);
		count++;
	}
	if (x + 1 < 10 && map[x + 1][y] == now)  //right
	{
		dfs_find(x + 1, y);
		count++;
	}
	if (x - 1 >= 0 && map[x - 1][y] == now)  //left
	{
		dfs_find(x - 1, y);
		count++;
	}
}

bool CGame::Remove()
{
	bool ifremove = false;
	int up, right, down, left;
	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			up = right = down = left = 0;   //���Ҹ���
			for (int k = 1; j + k < 10 && map[i][j + k] == map[i][j]; k++)  //��
				up = k;
			for (int k = 1; j - k >= 0 && map[i][j - k] == map[i][j]; k++)  //��
				down = k;
			for (int k = 1; i + k < 10 && map[i + k][j] == map[i][j]; k++)  //��
				right = k;
			for (int k = 1; i - k >= 0 && map[i - k][j] == map[i][j]; k++)  //��
				left = k;
			if (up + down >= 2 || right + left >= 2)
			{
				ifremove = true;
				count = 0;
				dfs_find(i, j);  //ɾ��ʱ���������ģ�������������
				for (int k = 0; k < count; k++)//ɾ��
				{
					for (int i = 0; i < 10; i++)
					{
						for (int j = 0; j < 10; j++)
						{
							if (map[i][j] == -1)  //����-1
							{
								map.at(i).erase(map.at(i).begin() + j);
								map.at(i).push_back(rand() % 5);
							}
						}
					}
				}
				score += up + down + right + left + 1;
			}
		}
	}
	return ifremove;
}