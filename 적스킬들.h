#pragma once
#include <time.h>
#include <stdlib.h>
#include <Windows.h>
#include <atlimage.h>
#include <math.h>

#define PIE 3.14;

struct RankingData {
	int tier;
	char ID[20];
	int score;
};

RankingData Rankdata[10];
CImage Hanjo_Skill[3];
CImage Genji_stun[2];
CImage Line2;
CImage Genji_Dragon[1];
CImage Genji_Dragon_Attack[3];
CImage testback;
CImage Genji_DeathImage;
CImage DragonSkill_Icon[2];
CImage result;
CImage RankingImage;
CImage tierImage[8];
CImage KeymenuImage;
CImage Menuclick;
CImage MapImage_Hanamura[2];

RankingData tempdata;
int Killcount = 0;
int testb = 0;
int  Dragon_y = 300; //한조궁 나오는곳 조정할떄 씀
RECT DragonRect; //한조궁 충돌체크용
RECT temprect; //충돌체크용 
bool Genji_DragonmodEffect = FALSE;
int Genji_Dragonmod_Cool = 0;
bool Attack = FALSE;
int Acount = 0;
int attackcount; //겐지 공격 모션 결정하는 카운트
bool jumpstate = FALSE;
bool Genji_Death = FALSE;
int deathcount = 0;
int ak = 0;
int jump;
int jcount;
int DCx, DCy; //출력할 처음좌표
int DCw, DCh; //출력할 좌표넓이 높이
int GD_count;
int GD_SKILLTIME;
int COMBO;
bool Genji_Dragonmod = FALSE;
int mx, my;
bool Menu_Ranking = FALSE;
int IDcount = 0;
char IDdata[20];
bool Menu_insertid = FALSE;
bool Menu_Key = FALSE;
int SaveMaxCombo = 0;


double AngletoRadi(int Angle)
{
	return Angle * 3.14 / 180;
}

struct DragonPos
{
	int x, y;
};

DragonPos Dpos[20];
void SetDpos(DragonPos *d, int headx, int heady)
{
	Dpos[0].x = headx;
	Dpos[0].y = heady;
	for (int i = 1; i < 20; i++)
	{
		Dpos[i].x = Dpos[i - 1].x + 25;
		Dpos[i].y = cos(AngletoRadi(Dpos[i].x)) * 40 + Dragon_y;
	}
}
void GenjiDragon(HDC hdc, CImage *c, int x, int y, int count)
{
	static int gd = 0;
	if (gd == 16) {
		gd = 0;
		Genji_DragonmodEffect = FALSE;
	}

	c[0].Draw(hdc, x, y, 348, 445, gd * 348, 0, 348, 445);
	if (count % 4 == 0)
		gd++;
}
RECT MakeRect(int x, int y, int size)
{
	RECT a;
	a.top = y - size;
	a.bottom = y + size;
	a.left = x - size;
	a.right = x + size;

	return a;
}


void Genji_Dragon_AttackSprite(HDC hdc, CImage *c, int x, int y, int Ccount, int jcount)
{

	static int ww = 0;
	if (ww == 13)
		ww = 0;
	if (Attack == FALSE)
	{
		c[0].Draw(hdc, x, y, 300, 300, ww * 300, 0, 300, 300);
	}
	else {
		if (attackcount % 2 == 0)
			c[1].Draw(hdc, x, y, 300, 300);
		else
			c[2].Draw(hdc, x + 45, y, 300, 300);

		if (Ccount % 5 == 0)
			Attack = FALSE;
	}
	c[0].Draw(hdc, x, y, 300, 300, ww * 300, 300, 300, 300);
	ww++;

}

int AddTier(int a)
{
	int t;
	if (0 <= a && a <= 2000)
	{
		t = 0;
	}
	else  if (2000 <= a && a <= 5000)
	{
		t = 1;
	}
	else  if (5000 <= a && a <= 10000)
	{
		t = 2;
	}
	else  if (10000 <= a && a <= 15000)
	{
		t = 3;
	}
	else  if (15000 <= a && a <= 20000)
	{
		t = 4;
	}
	else  if (20000 <= a && a <= 25000)
	{
		t = 5;
	}
	else  if (25000 <= a && a <= 35000)
	{
		t = 6;
	}
	else  if (35000 <= a)
	{
		t = 7;
	}
	return t;
}

void Insert_Rankdata(RankingData t)
{
	if (Rankdata[9].score < t.score)
	{
		Rankdata[9] = t;

		for (int i = 0; i < 10; i++)
		{
			for (int j = 0; j < 9; j++)
			{
				if (Rankdata[j].score < Rankdata[j + 1].score)
				{
					t = Rankdata[j];
					Rankdata[j] = Rankdata[j + 1];
					Rankdata[j + 1] = t;

				}
			}
		}

	}

}

