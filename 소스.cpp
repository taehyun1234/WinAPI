//#include "resource.h"
#include <time.h>
#include <stdlib.h>
#include <fmod.h>
#include <Windows.h>
#include <atlimage.h>
#include "적스킬들.h"

#define PARA 1
#define HANJO 2
#define LINE1 3
#define LINE2 4
#define MACREE 5
#define SOLDIER 6
#define BASTION 7
#define RIPER 8
#define tutomob 9
#define HEAL 120
#define jilpung 90
#define suriken 91
#define SKILL_DRAGON 100
//적캐릭터
//장애물
#define FLAME 20
#define THUNDER 21
#define TUES 22

FMOD_SYSTEM * soundSystem;// 선언
FMOD_SOUND *soundFile[11];
FMOD_CHANNEL *channel;  //배경음 1 채널
FMOD_CHANNEL *channel2;
FMOD_CHANNEL *channel3; // 배경음 2 채널


HINSTANCE g_hInst;
LPCTSTR lpszClass = "WINDOW";
LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM IParam);

void Soldier_Ani(HDC hdc, CImage *c, int x, int y, int count, int, int);

struct Bullet
{
	int x, y;
	int r;
	Bullet * next;
};
struct EnemyBullet
{
	int x, y;
	int r;
	int state;
	int js;
	EnemyBullet * next;
};
struct Enemy
{
	int x, y;
	int py;
	int r;
	int state;
	int hp;
	int invi;
	int time;
	int hittime;
	int hit;
	BOOL enemydie; //적이 죽을떄
	BOOL invisible;//리퍼전용변수
	Enemy * next;
};

void showsprite(HDC hdc, CImage *c, int x, int y, int Ccount, int jcount)
{
	static int w = 0;
	static int h = 0;
	if (w == 13)
		w = 0;
	if (jcount == 0) {
		c[0].Draw(hdc, x, y, 300, 300, w * 300, 0, 300, 300);
	}
	if (Attack == FALSE)
	{
		if (jumpstate == FALSE)
			c[0].Draw(hdc, x, y, 300, 300, w * 300, 300, 300, 300);
	}
	else {

		if (Acount == 9)
		{
			Attack = FALSE;
		}
		c[0].Draw(hdc, x, y, 300, 300, Acount * 300, 600, 300, 300);
		Acount++;
	}
	w++;
}
void jumpsp(HDC hdc, CImage *c, int x, int y, int jcount)
{
	static int j = 1;
	if (jumpstate == FALSE)
		c[1].Draw(hdc, x, y, 300, 300, 0, 0, 300, 300);
	else {
		if (j == 8)
		{
			j = 0;
			ak++;
			if (ak == 2)
				jumpstate = FALSE;
		}
		c[1].Draw(hdc, x, y, 300, 300, j * 300, 0, 300, 300);
		j++;
	}
}
void Dagger_sprite(HDC hdc, CImage *c, int x, int y, int type)
{
	static int Dw = 0;
	if (Dw == 4)
		Dw = 0;
	c[0].Draw(hdc, x, y, 30, 30, 30 * Dw, type * 30, 30, 30);
	Dw++;
}

void Stun_Ani(HDC hdc, CImage *c, CImage *c2, int x, int y, int count)
{
	c[count % 2].TransparentBlt(hdc, x + 100, y + 50, 100, 50, 0, 0, 200, 100, RGB(255, 255, 255));
	c2[count % 2].TransparentBlt(hdc, x, y + 100, 300, 300, 0, 0, 300, 300, RGB(255, 255, 255));
}
void Hit_Ani(HDC hdc, CImage *c, int x, int y, BOOL * hit)
{
	static int a = 0;
	c[a++ / 5].TransparentBlt(hdc, x, y - 50, 100, 100, RGB(0, 0, 0));
	if (a > 29)
	{
		a = 0;
		*hit = 0;
		return;
	}
}

void Explode_Ani(HDC hdc, CImage *c, int x, int y, BOOL * hit)
{
	static int a = 0;
	c[a++ / 5].TransparentBlt(hdc, x, y, 300, 300, RGB(255, 255, 255));
	if (a > 24)
	{
		a = 0;
		*hit = FALSE;
		return;
	}
}


void Hanjoultra(HDC hdc, CImage *c, int x, int y, BOOL * hit)
{
	static int a = 0;
	static BOOL out;
	c[0].TransparentBlt(hdc, x + 2 * a, y, 300, 300, 0, 0, 512, 512, RGB(255, 255, 255));
	if (a < -30)
	{
		out = TRUE;
	}
	if (out == FALSE)
	{
		a--;
	}
	else
	{
		a++;
	}
	if (a > 30)
	{
		a = 0;
		*hit = 0;
		return;
	}
}

void BulletHit_Ani(HDC hdc, CImage *c, int x, int y, BOOL * hit)
{
	static int a = 0;
	c[a++ / 3].TransparentBlt(hdc, x, y, 100, 100, 0, 0, 276, 208, RGB(255, 255, 255));
	if (a > 8)
	{
		a = 0;
		*hit = 0;
		return;
	}
}

void showsprite2(HDC hdc, CImage *c, int x, int y, int Ccount, int jcount)
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
	}
	c[0].Draw(hdc, x, y, 300, 300, ww * 300, 300, 300, 300);
	ww++;

}

void Bastion_sprite(HDC hdc, CImage * c, int x, int y)
{
	static int Dw = 0;
	if (Dw == 8)
	{
		Dw = 0;
	}
	c[0].Draw(hdc, x, y, 140, 120, 105 * Dw, 0, 105, 79);
	Dw++;
}
void coolTime(HDC hdc, CImage *c, int x, int y, int cooltime)
{
	cooltime = (100 - cooltime) / 2;
	c[1].Draw(hdc, x, y, 50, 50, 0, 0, 193, 198);//궁오프
	c[0].Draw(hdc, x, y + cooltime, 50, 50 - cooltime, 0, cooltime, 50, 50 - cooltime);//궁온
}
void paraboost(HDC hdc, CImage *c, int x, int y, int count)
{
	static int p = 0;
	if (p == 3)
		p = 0;

	c[0].Draw(hdc, x + p * 10, y, 100, 75, p * 200, 0, 200, 150);
	if (count % 3 == 0)
		p++;
}

int starselect;
void stage(int);
void DeleteBullet(Bullet * delbullet);
void CreateBullet(int px, int py);
void DeleteEnemyBullet(EnemyBullet * delbullet);
void CreateEnemyBullet(int ex, int ey, int state, int jumpstate);
void DeleteEnemy(Enemy * delEnemy);
Enemy * CreateEnemy(int state);
BOOL CollisionCheck(int x, int y, int mx, int my, int size);
double LengthCheck(int x, int y, int mx, int my);
bool Animation(CImage * img, HDC memdc, int x, int y, bool a);
//BOOL CALLBACK DlgSelectstar(HWND hDlg, UINT iMsg, WPARAM wParam, LPARAM lParam);
Bullet * bulletHead = nullptr;
Enemy * EnemyHead = nullptr;
Enemy * etemp = nullptr;
Bullet * temp = nullptr;
EnemyBullet * enemybulletHead = nullptr;
EnemyBullet * enemybullettemp = nullptr;

void Animation(CImage * img, HDC memdc, int x, int y, int count);
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow)

{
	srand((unsigned)time(NULL));
	HWND hWnd;
	MSG Message;
	WNDCLASSEX WndClass;
	g_hInst = hInstance;
	WndClass.cbSize = sizeof(WndClass);
	WndClass.style = CS_HREDRAW | CS_VREDRAW;
	WndClass.lpfnWndProc = (WNDPROC)WndProc;
	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hInstance = hInstance;
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	WndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	WndClass.lpszMenuName = NULL;
	WndClass.lpszClassName = lpszClass;
	WndClass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	RegisterClassEx(&WndClass);

	hWnd = CreateWindow(lpszClass, "windows program 1-1", WS_OVERLAPPEDWINDOW, 0, 0, 1200, 700, NULL, (HMENU)NULL, hInstance, NULL); // 윈도우창 설정창
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	while (GetMessage(&Message, 0, 0, 0)) {
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
	return Message.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT iMessage, WPARAM wParam, LPARAM IParam)
{
	static HFONT hfont, oldfont;
	static RECT rect;
	static CImage img[2];
	static int damage; // 캐릭터의 공격력
	static CImage ultcoolTime[2];
	static CImage Dagger[1];
	static CImage genji[2];
	static CImage explode[5];
	static CImage healpack;
	static CImage Line2;
	static CImage bastion[1];
	static CImage riper;
	static CImage stunGenji[2];
	static CImage hit[6];
	static CImage joldagu;
	static CImage skillCool1[4];
	static CImage Para, P_Boost[1];
	static CImage stunani[2];
	static CImage Effect[8];
	static CImage Hanjocutscene[1];
	static CImage Background[2];
	static CImage Background2[2];
	static CImage soldier[9];
	static CImage ebullet;
	static CImage hole;
	static CImage hpRec;//피
	static int hp; // 겐지 피
	static CImage arrow, bastionbullet, parabullet;
	static CImage startImage;
	static int cut1, cut2 = 0;
	static HBITMAP hbitmap, oldbitmap;
	static int score;
	static BOOL noenemy;
	PAINTSTRUCT ps;
	static int Ccount; //객체시간
	static HDC hdc;
	static char buf[60];
	static CImage Hanjo_Move[2];
	static CImage bullet_hit[3];
	static   CImage Mac_Move[2];
	static int jcount;
	static int boxX, boxY;
	static int clickcount; //기절빨리풀기
	static int gy, gx;//겐지 y좌표
	static   Enemy * enemytemp;//임시저장용
	static BOOL hanjoultOn;
	static BOOL stun;
	static BOOL reload;
	static int stunCount = 0;
	static BOOL cool1;
	static BOOL bulhit;
	static BOOL start;
	static int x = 1200;
	static int jump;
	static int ultTime;
	static int k;
	static int enemySpeed = 0;
	static int scount = 0; // 스턴시간
	static int count = 0; // 시간
	static BOOL ghit; // 겐지의 히트 여부
	static BOOL shoot;
	static bool Skill_1 = FALSE;
	static bool Effect_1 = FALSE;
	static int S1_count = 0;
	switch (iMessage) {
	case WM_CREATE:
		ultTime = 5;
		FMOD_System_Create(&soundSystem); // 생성 
		FMOD_System_Init(soundSystem, 10, FMOD_INIT_NORMAL, NULL);
		FMOD_System_CreateStream(soundSystem, "Hanamura.mp3", FMOD_LOOP_NORMAL, 0, &soundFile[0]); //배경음
		FMOD_System_CreateStream(soundSystem, "main.mp3", FMOD_LOOP_NORMAL, 0, &soundFile[9]); //배경음
		FMOD_System_CreateSound(soundSystem, "genji.mp3", FMOD_LOOP_OFF, 0, &soundFile[1]); //겐지궁
		FMOD_System_CreateSound(soundSystem, "riper.mp3", FMOD_LOOP_OFF, 0, &soundFile[2]); //리퍼공격
		FMOD_System_CreateSound(soundSystem, "linehart.mp3", FMOD_LOOP_OFF, 0, &soundFile[3]); //라인하르트 등장
		FMOD_System_CreateSound(soundSystem, "hanjo.mp3", FMOD_LOOP_OFF, 0, &soundFile[4]); //한조궁
		FMOD_System_CreateSound(soundSystem, "junkret.mp3", FMOD_LOOP_OFF, 0, &soundFile[5]); //정크랫궁
		FMOD_System_CreateSound(soundSystem, "basicattack.mp3", FMOD_LOOP_OFF, 0, &soundFile[6]); //평타
		FMOD_System_CreateSound(soundSystem, "jilpung.wav", FMOD_LOOP_OFF, 0, &soundFile[7]); //질풍참
		FMOD_System_CreateSound(soundSystem, "enemykill.wav", FMOD_LOOP_OFF, 0, &soundFile[8]); //질풍참
		FMOD_System_CreateSound(soundSystem, "sword.wav", FMOD_LOOP_OFF, 0, &soundFile[10]); //질풍참

		hp = 1000;
		FMOD_System_PlaySound(soundSystem, soundFile[9], NULL, 0, &channel);
		start = FALSE;
		stun = FALSE;
		damage = 10;
		Ccount = 0;
		gy = 360;
		score = 0;
		starselect = 1;
		boxX = 265;
		boxY = 160;

		result.Load(TEXT("result.png"));
		RankingImage.Load(TEXT("티어 베이스.png"));
		tierImage[0].Load(TEXT("브론즈.png")); tierImage[1].Load(TEXT("실버.png")); tierImage[2].Load(TEXT("골드.png")); tierImage[3].Load(TEXT("플래티넘.png"));
		tierImage[4].Load(TEXT("다이아.png")); tierImage[5].Load(TEXT("마스터.png")); tierImage[6].Load(TEXT("그마.png")); tierImage[7].Load(TEXT("그마2.png"));
		KeymenuImage.Load(TEXT("조작법.bmp"));
		Genji_DeathImage.Load(TEXT("겐지사망.png"));
		Menuclick.Load(TEXT("겐지표창메뉴.png"));

		healpack.Load(TEXT("힐팩.jpg"));

		startImage.Load(TEXT("시작화면.BMP"));
		joldagu.Load(TEXT("쫄따구.png"));
		hit[0].Load(TEXT("겐지타격1.BMP"));
		hit[1].Load(TEXT("겐지타격2.BMP"));
		hit[2].Load(TEXT("겐지타격3.BMP"));
		hit[3].Load(TEXT("겐지타격4.BMP"));
		hit[4].Load(TEXT("겐지타격5.BMP"));
		hit[5].Load(TEXT("겐지타격6.BMP"));
		testback.Load(TEXT("테스트검정.png"));
		Genji_Dragon_Attack[0].Load(TEXT("용검베이스.png"));
		Genji_Dragon_Attack[1].Load(TEXT("휘두르기1.png"));
		Genji_Dragon_Attack[2].Load(TEXT("휘두르기2.png"));
		Genji_Dragon[0].Load(TEXT("용검이펙트.png"));
		bullet_hit[0].Load(TEXT("총알타격1.BMP"));
		bullet_hit[1].Load(TEXT("총알타격2.BMP"));
		bullet_hit[2].Load(TEXT("총알타격3.BMP"));
		Line2.Load(TEXT("라인돌진.png"));
		riper.Load(TEXT("ㅇㅇㅇ.png"));
		Hanjo_Skill[0].Load(TEXT("D1.png"));
		Hanjo_Skill[1].Load(TEXT("D2.png"));
		Hanjo_Skill[2].Load(TEXT("D3.png"));
		stunGenji[0].Load(TEXT("겐지스턴1.png"));
		stunGenji[1].Load(TEXT("겐지스턴2.png"));
		stunani[0].Load(TEXT("스턴효과1.png"));
		stunani[1].Load(TEXT("스턴효과2.png"));
		Hanjo_Move[0].Load(TEXT("한조걷기1.png"));
		Hanjo_Move[1].Load(TEXT("한조걷기2.png"));
		Mac_Move[0].Load(TEXT("맥크리걷기1.png"));
		Mac_Move[1].Load(TEXT("맥크리걷기2.png"));
		soldier[0].Load(TEXT("sold1.png"));
		soldier[1].Load(TEXT("sold2.png"));
		soldier[2].Load(TEXT("sold3.png"));
		soldier[3].Load(TEXT("sold4.png"));
		soldier[4].Load(TEXT("sold5.png"));
		soldier[5].Load(TEXT("sold6.png"));
		soldier[6].Load(TEXT("sold7.png"));
		soldier[7].Load(TEXT("sold8.png"));
		soldier[8].Load(TEXT("sold9.png"));
		explode[0].Load(TEXT("ghit1.BMP"));
		explode[1].Load(TEXT("ghit2.BMP"));
		explode[2].Load(TEXT("ghit3.BMP"));
		explode[3].Load(TEXT("ghit4.BMP"));
		explode[4].Load(TEXT("ghit5.BMP"));
		Hanjocutscene[0].Load(TEXT("한조컷신.BMP"));
		skillCool1[1].Load(TEXT("쿨타임2.png"));
		skillCool1[2].Load(TEXT("쿨타임3.png"));
		skillCool1[3].Load(TEXT("쿨타임4.png"));
		skillCool1[0].Load(TEXT("질풍참.png"));
		ultcoolTime[0].Load(TEXT("궁온.PNG"));
		ultcoolTime[1].Load(TEXT("궁오프.PNG"));
		hpRec.Load(TEXT("칸.png"));
		bastionbullet.Load(TEXT("바스티온총알.bmp"));
		parabullet.Load(TEXT("파라총알.png"));
		arrow.Load(TEXT("한조화살.PNG"));
		hole.Load(TEXT("구덩이.png"));
		ebullet.Load(TEXT("총알.PNG"));
		Dagger[0].Load(TEXT("표창모음.png"));
		img[0].Load(TEXT("Line1.png"));
		img[1].Load(TEXT("Line2.png"));
		genji[0].Load(TEXT("겐지스프라이트.png"));
		genji[1].Load(TEXT("점프수정.png"));
		Background[0].Load(TEXT("background.png"));
		Background[1].Load(TEXT("background2.png"));
		Background2[0].Load(TEXT("hanamura1.png"));
		Background2[1].Load(TEXT("hanamura2.png"));
		bastion[0].Load(TEXT("baba.png"));
		Para.Load(TEXT("파라테스트.png"));
		P_Boost[0].Load(TEXT("파라부스터W200.png"));
		Effect[0].Load(TEXT("1.png"));
		Effect[1].Load(TEXT("2.png"));
		Effect[2].Load(TEXT("3.png"));
		Effect[3].Load(TEXT("4.png"));
		Effect[4].Load(TEXT("5.png"));
		Effect[5].Load(TEXT("6.png"));
		Effect[6].Load(TEXT("7.png"));

		GetClientRect(hwnd, &rect);
		DCx = 0;
		DCy = 0;
		DCw = rect.right;
		DCh = rect.bottom;
		break;
	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_UP:
			if (start == TRUE) {
				if (stun == FALSE) {
					if (jcount < 2)
					{
						jcount++;
						jump = 30;
						if (jcount == 2) {
							jumpstate = TRUE;
							ak = 0;
						}
					}
				}
			}
			else  if (Menu_Ranking == FALSE && Menu_Key == FALSE && Menu_insertid == FALSE)
			{
				if (boxY>160)
					boxY -= 60;
			}
			break;
		case VK_DOWN:
			if (boxY < 280 && Menu_Ranking == FALSE && Menu_Key == FALSE && Menu_insertid == FALSE) {
				boxY += 60;
			}
			break;
		case VK_RETURN:if (deathcount != 50) {

			if (Menu_insertid && strlen(IDdata) >0)
			{

				strcpy(tempdata.ID, IDdata);


				start = TRUE;
				SetTimer(hwnd, 1, 1, NULL);
				FMOD_Channel_Stop(channel);
				FMOD_System_PlaySound(soundSystem, soundFile[0], NULL, 0, &channel3);
				Menu_insertid = FALSE;
				strcpy(IDdata, "");
			}
			if (boxY == 160)
			{
				Menu_insertid = TRUE;

			}
			if (boxY == 220) {

				if (Menu_Ranking == FALSE && Menu_insertid == FALSE && start == FALSE) {
					if (Menu_Key == FALSE)
						Menu_Key = TRUE;
					else
						Menu_Key = FALSE;
				}
			}
			if (boxY == 280) {//랭킹
				if (Menu_Ranking == FALSE && Menu_insertid == FALSE&& start == FALSE) {

					FILE *fp;// 파일 읽음
					fp = fopen("Ranking.txt", "r");
					for (int i = 0; i < 10; i++)
					{
						fscanf(fp, "%d %s %d\n", &Rankdata[i].tier, &Rankdata[i].ID, &Rankdata[i].score);
					}
					fclose(fp);
					Menu_Ranking = TRUE;
				}
				else
				{
					Menu_Ranking = FALSE;
				}
			}

		}
					   break;
					   break;
		case VK_SPACE:
			if (stun == FALSE&&start == TRUE) {
				if (Genji_Dragonmod == FALSE) {
					if (stun == FALSE) {
						if (reload == FALSE) {
							FMOD_System_PlaySound(soundSystem, soundFile[6], NULL, 0, &channel);
							CreateBullet(250 + gx, gy + 70);
							CreateBullet(220 + gx, gy + 70);
							CreateBullet(190 + gx, gy + 70);
							Attack = TRUE;
						}
						reload = TRUE;
						Acount = 1;
					}
				}
				else
				{
					Attack = TRUE;
					attackcount++;
					FMOD_System_PlaySound(soundSystem, soundFile[10], NULL, 0, &channel);
					for (etemp = EnemyHead; etemp != NULL; etemp = etemp->next)
					{
						if (0 <= etemp->x && etemp->x <= 500 && gy + 450 >= etemp->y && etemp->y >= gy&&etemp->state != HEAL)
						{
							etemp->hit = suriken;
							etemp->hp -= 500;
						}

					}
				}

			}
			break;
			break;
		case VK_LEFT:
			if (stun == TRUE&&start == TRUE && Genji_Death == FALSE) {
				clickcount++;
			}
			break;
		case VK_RIGHT:
			if (stun == TRUE&&start == TRUE  &&Genji_Death == FALSE) {
				clickcount++;
			}
			break;
		case VK_SHIFT:
		{
			if (stun == FALSE&&start == TRUE) {
				if (Skill_1 == FALSE&&cool1 == FALSE) {
					FMOD_System_PlaySound(soundSystem, soundFile[7], NULL, 0, &channel);
					Skill_1 = TRUE;
					gx += 600;
					Effect_1 = TRUE;
				}
			}
			if (Skill_1 == TRUE&&start == TRUE) {
				for (etemp = EnemyHead; etemp != NULL; etemp = etemp->next)
				{

					if (0 <= etemp->x && etemp->x <= 700 && gy + 450 >= etemp->y && etemp->y >= gy&&etemp->state != HEAL&&etemp->state!=SKILL_DRAGON)
					{
						etemp->hp -= 80;
						etemp->hit = jilpung;
					}

				}
			}
		}
		break;
		}
		InvalidateRect(hwnd, NULL, FALSE);
		break;
	case WM_CHAR:
		if (Menu_insertid)
		{
			if (wParam == VK_BACK)
			{
				if (IDcount >= 1)
				{
					IDdata[IDcount] = '\0';
					IDcount--;

				}
				else if (IDcount == 0)
				{
					IDdata[IDcount] = '\0';
				}
			}
			else if (IDcount < 20 && wParam != VK_RETURN)
			{
				if (IDcount <= 18)
					IDdata[IDcount++] = wParam;
				else if (IDcount == 19)
					IDdata[IDcount] = wParam;
			}

		}
		if (wParam == 'f' )
		{
			noenemy = TRUE;
		}
		if (wParam == 'q' && ultTime >= 95 && start)
		{
			FMOD_System_PlaySound(soundSystem, soundFile[1], NULL, 0, &channel);
			ultTime = 5;
			jcount++;
			jump = 35;
			jcount++;
			jump = 35;
			Genji_Dragonmod_Cool = 1000;
			if (jcount == 2) {
				jumpstate = TRUE;
				ak = 0;
			}
			Genji_DragonmodEffect = TRUE;
			Genji_Dragonmod = TRUE;
			GD_count = 0;
		}
		InvalidateRect(hwnd, NULL, FALSE);
		break;
	case WM_LBUTTONDOWN:
		mx = LOWORD(IParam);
		my = HIWORD(IParam);
		if (deathcount == 50)
		{
			if (951 <= mx && my >= 513 && mx <= 1104 && my <= 561)
			{
				start = FALSE;
				FMOD_Channel_Stop(channel3);
				FMOD_System_PlaySound(soundSystem, soundFile[9], NULL, 0, &channel);
				hp = 1000;
				DCx = 0;
				DCy = 0;
				DCw = rect.right;
				DCh = rect.bottom;
				stun = FALSE;
				score = 0;
				Killcount = 0;
				COMBO = 0;
				deathcount = 0;
				Genji_Death = FALSE;
				ultTime = 5;
				count = 0;
				IDcount = 0;
				start = FALSE;
				Menu_insertid = FALSE;
				Menu_Ranking = FALSE;
				enemySpeed = 0;
				Genji_Dragonmod = FALSE;
				SaveMaxCombo = 0;
			}
		}

		InvalidateRect(hwnd, NULL, FALSE);
		break;
	case WM_TIMER: {
		x -= 5;
		if (noenemy == TRUE) {
			hp = 1000;//무적모드
		}
		count++;
		if (Genji_Dragonmod == TRUE)
		{
			Genji_Dragonmod_Cool--;
			if (Genji_Dragonmod_Cool < 0)
			{
				Genji_Dragonmod = FALSE;
			}
		}
		if (count % 500 == 0 && ultTime < 100)
		{
			ultTime++;

		}
		if (Genji_DragonmodEffect)
		{
			if (GD_count == 9)
			{
				Genji_Dragonmod = TRUE;
				enemySpeed = 5;
			}
			if (GD_count < 10) {
				DCy += 14;
				DCw -= 24;
				DCh -= 14;
				testb += 20;
			}
			GD_count++;
		}
		else if (DCy != 0 && Genji_Death == FALSE)
		{

			DCy -= 14;
			DCw += 24;
			DCh += 14;
			testb -= 20;
		}
		if (count % 1000 == 0) {
			FMOD_System_Update(soundSystem);
		}
		if (count % 50 == 0 && reload == TRUE)
		{
			reload = FALSE;
		}
		if (count % 10 == 0)
		{
			Ccount++;
			score++;
		}
		if (stun == FALSE) {
			if (cut1 == 1200)
			{
				cut1 = 0;
			}
			cut1 += 2;
			if (cut2 == 1200)
			{
				cut2 = 0;
			}cut2 += 10;
		}
		else
		{
			if (cut1 == 1200)
			{
				cut1 = 0;
			}
			cut1 += 0;
			if (cut2 == 1200)
			{
				cut2 = 0;
			}cut2 += 0;
		}

		if (Skill_1)
		{
			if (gy == 360) {
				gx -= 20;
				if (gx == 0)
					Skill_1 = FALSE;
				cool1 = TRUE;
			}
		}
		if (cool1 == TRUE)
		{
			if (count % 50 == 0) {
				S1_count++;
			}
			if (S1_count == 4)
			{
				S1_count = 0;
				cool1 = FALSE;
			}
		}
		//2단 점프
		if (jcount > 0) {
			if (jump > -20)
				jump -= 2;
			gy -= jump;
			if (gy > 360)
			{
				gy = 360;
				jumpstate = FALSE;
				jcount = 0;
			}
		}
		//표창발사
		for (temp = bulletHead; temp != NULL; temp = temp->next)
		{
			temp->x += 20;
			if (temp->x > rect.right)
			{
				if (temp != NULL) {
					DeleteBullet(temp);
				}
			}
		}
		//적좌표 이동
		/*
		if (count % 300 == 0) // 캐릭터 생성간격
		{
		int randnum;
		randnum = rand() % 8 + 1;
		if (randnum == LINE2)
		{
		FMOD_System_PlaySound(soundSystem, soundFile[3], NULL, 0, &channel2);
		}
		CreateEnemy(randnum);
		}
		if (count % 1200 == 0)
		{
		FMOD_System_PlaySound(soundSystem, soundFile[4], NULL, 0, &channel2); //한조궁 생성
		CreateEnemy(SKILL_DRAGON);
		hanjoultOn = TRUE;
		}
		if (count % 100 == 0)
		{
		CreateEnemy(tutomob);
		}*/
		stage(count);
		if (count > 8000) {
			if (count % 2000 == 0)
			{
				FMOD_System_PlaySound(soundSystem, soundFile[4], NULL, 0, &channel2); //한조궁 생성
				CreateEnemy(SKILL_DRAGON);
				hanjoultOn = TRUE;
			}
		}
		if (count % 100 == 0)
		{
			CreateEnemy(tutomob);
		}

		for (etemp = EnemyHead; etemp != NULL; etemp = etemp->next)
		{
			if (CollisionCheck(gx + 150, gy + 135, etemp->x, etemp->y, 50) == TRUE&&etemp->state == LINE2)
			{
				stun = TRUE;
				hp -= 50;
				COMBO = 0;
			}
			if (CollisionCheck(gx + 150, gy + 135, etemp->x, etemp->y, 50) == TRUE&&etemp->state == SOLDIER)
			{
				stun = TRUE;
				COMBO = 0;
			}
			if (CollisionCheck(gx + 150, gy + 135, etemp->x, etemp->y, 100) == TRUE&&etemp->state == tutomob&&etemp->enemydie == FALSE)
			{
				ghit = TRUE;
				if (etemp != NULL&&etemp->enemydie==FALSE) {
					DeleteEnemy(etemp);
				}
				hp -= 20;
				COMBO = 0;
			}
			if (CollisionCheck(gx + 150, gy + 135, etemp->x, etemp->y, 50) == TRUE&&etemp->state == RIPER)
			{
				FMOD_System_PlaySound(soundSystem, soundFile[2], NULL, 0, &channel3);
				hp -= 150;
				COMBO = 0;
				if (etemp != NULL&&etemp->enemydie == FALSE) {
					DeleteEnemy(etemp);
				}
			}
			if (CollisionCheck(gx + 150, gy + 135, etemp->x, etemp->y, 50) == TRUE&&etemp->state == HEAL)
			{
				if (hp <= 800) {
					hp += 200;
				}
				else
				{
					hp = 1000;
				}
				if (etemp != NULL) {
					DeleteEnemy(etemp);
				}
			}
			if (IntersectRect(&temprect, &DragonRect, &MakeRect(gx + 150, gy + 135, 50)) && etemp->state == SKILL_DRAGON)
			{
				hp -= 2;
			}

			if (etemp->state == PARA) {
				if (count % 3 == 0)
				{
					etemp->y += etemp->py;
					if (etemp->y >= 130)
						etemp->py = -3;
					if (etemp->y <= 100)
						etemp->py = 3;
				}
			}

			if (stun == FALSE) {
				if (etemp->state == PARA)
					etemp->x -= 1 + enemySpeed;
				else if (etemp->state == LINE1)
					etemp->x -= 1 + enemySpeed;
				else if (etemp->state == tutomob) {
					etemp->x -= 5 + enemySpeed;
					if (etemp->y < gy + 70)
					{
						etemp->y += 2;
					}
					if (etemp->y > gy + 70)
					{
						etemp->y -= 2;
					}
				}
				else if (etemp->state == SOLDIER)
					etemp->x -= 7 + enemySpeed;
				else if (etemp->state == MACREE)
					etemp->x -= 1 + enemySpeed;
				else if (etemp->state == RIPER)
					etemp->x -= 4 + enemySpeed;
				else if (etemp->state == HANJO)
					etemp->x -= 2 + enemySpeed;
				else if (etemp->state == LINE2)
					etemp->x -= 15;
				else if (etemp->state == HEAL)
					etemp->x -= 10;
				else if (etemp->state == SKILL_DRAGON) {// 한조궁 이동
					etemp->x -= 5;
					etemp->y = cos(AngletoRadi(Dpos[0].x)) * 30 + Dragon_y;
					SetDpos(Dpos, etemp->x, etemp->y);
					DragonRect.top = etemp->y; DragonRect.right = etemp->x + 500;
					DragonRect.left = etemp->x - 30; DragonRect.bottom = etemp->y + 50;
				}
			}
			else
			{
				if (etemp->state == PARA)
					etemp->x -= 1 + enemySpeed;
				else if (etemp->state == LINE1)
					etemp->x -= 1 + enemySpeed;
				else if (etemp->state == SOLDIER)
					etemp->x -= 3 + enemySpeed;
				else if (etemp->state == MACREE)
					etemp->x -= 1 + enemySpeed;
				else if (etemp->state == RIPER)
					etemp->x -= 1 + enemySpeed;
				else if (etemp->state == HANJO)
					etemp->x -= 1 + enemySpeed;
				else if (etemp->state == tutomob)
					etemp->x -= 1 + enemySpeed;
				else if (etemp->state == LINE2)
					etemp->x -= 10;
				else if (etemp->state == HEAL)
					etemp->x -= 5;
				else if (etemp->state == SKILL_DRAGON) {// 한조궁 이동
					etemp->x -= 5;
					etemp->y = cos(AngletoRadi(Dpos[0].x)) * 30 + Dragon_y;
					SetDpos(Dpos, etemp->x, etemp->y);
					DragonRect.top = etemp->y; DragonRect.right = etemp->x + 500;
					DragonRect.left = etemp->x - 30; DragonRect.bottom = etemp->y + 50;
				}
			}
			//바스티온은 움직이지 않음
			if (etemp->x < 0)
			{
				if (etemp != NULL&&etemp->enemydie == FALSE) {
					DeleteEnemy(etemp);
				}
			}
			for (temp = bulletHead; temp != NULL; temp = temp->next)
			{
				if (etemp->state != RIPER&&etemp->state != HEAL) {
					if (CollisionCheck(temp->x, temp->y, etemp->x, etemp->y, etemp->r) == TRUE&&etemp->state != tutomob&&etemp->enemydie == FALSE&&etemp->state != SKILL_DRAGON)
					{
						if (temp != NULL) {
							DeleteBullet(temp);
						}
						etemp->hit = suriken;
						etemp->hp -= damage;
					}
				}
				if (etemp->state == RIPER)
				{
					if (CollisionCheck(temp->x, temp->y, etemp->x, etemp->y, etemp->r) == TRUE&&etemp->invisible == FALSE)
					{
						if (temp != NULL) {
							DeleteBullet(temp);
						}
						etemp->hit = suriken;
						etemp->hp -= damage;
					}
				}
				else if (etemp->state == tutomob)
				{
					if (CollisionCheck(temp->x, temp->y, etemp->x, etemp->y, etemp->r) == TRUE&&etemp->enemydie == FALSE)
					{
						if (temp != NULL) {
							DeleteBullet(temp);
						}
						etemp->hit = suriken;
						etemp->hp -= damage;
					}
				}
			}
			if (etemp->hp < 1) // 죽을때 효과 추가
			{
				etemp->enemydie = TRUE;
			}
			if (etemp->enemydie == TRUE)
			{
				if (etemp->state == PARA)
				{
					etemp->y += 5;
				}
				if (etemp->time == 0) {
					etemp->invi = 0;
				}
				etemp->time++;
				if (etemp->time < 40)
				{
					etemp->invi -= 4;
				}
				if (etemp->time > 40)
				{
					if (etemp != NULL) {
						FMOD_System_PlaySound(soundSystem, soundFile[8], NULL, 0, &channel);
						if (etemp->state == HANJO) {
							score += 30 + COMBO * 2;
							COMBO++;	Killcount++;
							if (Genji_Dragonmod)
							{
								score += 50 + COMBO;
							}
						}
						else if (etemp->state == SOLDIER) {
							score += 4 + COMBO * 2;
							COMBO++;	Killcount++;
							if (Genji_Dragonmod)
							{
								score += 50 + COMBO;
							}
						}
						else if (etemp->state == MACREE) {
							score += 60 + COMBO * 2;
							COMBO++;	Killcount++;
							if (Genji_Dragonmod)
							{
								score += 50 + COMBO;
							}
						}
						else if (etemp->state == BASTION) {
							score += 30 + COMBO * 2;
							COMBO++;	Killcount++;
							if (Genji_Dragonmod)
							{
								score += 50 + COMBO;
							}
						}
						else if (etemp->state == LINE1) {
							score += 100 + COMBO * 2;
							COMBO++;	Killcount++;
							if (Genji_Dragonmod)
							{
								score += 50 + COMBO;
							}
						}
						else if (etemp->state == tutomob) {
							score += 10 + COMBO * 2;
							COMBO++;	Killcount++;
							if (Genji_Dragonmod)
							{
								score += 50 + COMBO;
							}
						}
						else if (etemp->state == RIPER) {
							score += 30 + COMBO * 2;
							COMBO++;	Killcount++;
							if (Genji_Dragonmod)
							{
								score += 50 + COMBO;
							}
						}
						else if (etemp->state == PARA) {
							score += 30 + COMBO * 2;
							COMBO++;	Killcount++;
							if (Genji_Dragonmod)
							{
								score += 50 + COMBO;
							}
						}
						if (ultTime <= 95) {
							ultTime += 5;
						}				
						if (etemp != NULL) {
							DeleteEnemy(etemp);
						}

					}
					else {}
				}
			}
			if (etemp != NULL&&etemp->enemydie == FALSE) {
				if (count % 10 == 0 && etemp->state == BASTION) {
					CreateEnemyBullet(etemp->x, etemp->y, etemp->state, jcount);
				}
				else if (count % 100 == 0 && etemp->state == MACREE)
				{
					CreateEnemyBullet(etemp->x, etemp->y, etemp->state, jcount);
					CreateEnemyBullet(etemp->x - 15, etemp->y, etemp->state, jcount);
					CreateEnemyBullet(etemp->x + 15, etemp->y, etemp->state, jcount);
				}
				else if (count % 100 == 0 && etemp->state == SOLDIER)
				{
					CreateEnemyBullet(etemp->x, etemp->y, etemp->state, jcount);
				}
				else if (count % 100 == 0 && etemp->state == HANJO)
				{
					CreateEnemyBullet(etemp->x, etemp->y, etemp->state, jcount);
				}
				else if (count % 100 == 0 && etemp->state == PARA)
				{
					CreateEnemyBullet(etemp->x, etemp->y, etemp->state, jcount);
				}
			}
		}


		if (hp <= 0) // 겐지 죽는곳
		{
			hp = 0;

			stun = TRUE;

			deathcount++;
			Genji_Death = TRUE;

			if (deathcount < 20) {
				DCy += 14;
				DCw -= 24;
				DCh -= 14;
			}

		}
		if (SaveMaxCombo <= COMBO)
		{
			SaveMaxCombo = COMBO;
		}
		if (Genji_Death && stun && gy == 360 && deathcount == 50)//땅에 내려와서 쓰러지도록 하는 부분
		{
			KillTimer(hwnd, 1);
			for (etemp = EnemyHead; etemp != NULL; etemp = etemp->next)
			{
				if (etemp != NULL) {
					DeleteEnemy(etemp);
				}

			}
			for (enemybullettemp = enemybulletHead; enemybullettemp != NULL; enemybullettemp = enemybullettemp->next)
			{
				if (enemybullettemp != NULL) {
					DeleteEnemyBullet(enemybullettemp);
				}
			}


			tempdata.score = score + (Killcount * 2) + (SaveMaxCombo * 5);
			tempdata.tier = AddTier(tempdata.score);




			FILE *fp;
			fp = fopen("Ranking.txt", "r");
			for (int i = 0; i < 10; i++)
			{
				fscanf(fp, "%d %s %d\n", &Rankdata[i].tier, &Rankdata[i].ID, &Rankdata[i].score);
			}
			fclose(fp);

			Insert_Rankdata(tempdata);

			FILE *fp2;
			fp2 = fopen("Ranking.txt", "w");
			for (int i = 0; i < 10; i++)
			{
				fprintf(fp2, "%d %s %d\n", Rankdata[i].tier, Rankdata[i].ID, Rankdata[i].score);
			}
			fclose(fp2);


		}

		/////////////////////////////////////////////////////
		for (enemybullettemp = enemybulletHead; enemybullettemp != NULL; enemybullettemp = enemybullettemp->next)
		{
			if (CollisionCheck(gx + 150, gy + 135, enemybullettemp->x, enemybullettemp->y, 50) && Genji_Dragonmod == FALSE)
			{
				bulhit = TRUE;
				if (enemybullettemp->state == PARA)
				{
					hp -= 40;
					COMBO = 0;
					if (enemybullettemp != NULL)
						DeleteEnemyBullet(enemybullettemp);
				}
				if (enemybullettemp->state == HANJO)
				{
					hp -= 40;
					COMBO = 0;
					if (enemybullettemp != NULL)
						DeleteEnemyBullet(enemybullettemp);
				}
				if (enemybullettemp->state == MACREE)
				{
					hp -= 40;
					COMBO = 0;
					if (enemybullettemp != NULL)
						DeleteEnemyBullet(enemybullettemp);
				}
				if (enemybullettemp->state == BASTION)
				{
					hp -= 10;
					COMBO = 0;
					if (enemybullettemp != NULL)
						DeleteEnemyBullet(enemybullettemp);
				}
				if (enemybullettemp->state == SOLDIER)
				{
					hp -= 20;
					COMBO = 0;
					if (enemybullettemp != NULL)
						DeleteEnemyBullet(enemybullettemp);
				}
			}
			else if (CollisionCheck(gx + 150, gy + 135, enemybullettemp->x, enemybullettemp->y, 50) && Genji_Dragonmod == TRUE)
			{
				bulhit = TRUE;
				if (enemybullettemp->state == PARA)
				{
					hp -= 20;
					COMBO = 0;
					if (enemybullettemp != NULL)
						DeleteEnemyBullet(enemybullettemp);
				}
				if (enemybullettemp->state == HANJO)
				{
					hp -= 20;
					COMBO = 0;
					if (enemybullettemp != NULL)
						DeleteEnemyBullet(enemybullettemp);
				}
				if (enemybullettemp->state == MACREE)
				{
					hp -= 20;
					COMBO = 0;
					if (enemybullettemp != NULL)
						DeleteEnemyBullet(enemybullettemp);
				}
				if (enemybullettemp->state == BASTION)
				{
					hp -= 5;
					COMBO = 0;
					if (enemybullettemp != NULL)
						DeleteEnemyBullet(enemybullettemp);
				}
				if (enemybullettemp->state == SOLDIER)
				{
					hp -= 10;
					COMBO = 0;
					if (enemybullettemp != NULL)
						DeleteEnemyBullet(enemybullettemp);
				}
			}
			if (enemybullettemp->state == PARA)
			{
				enemybullettemp->x -= 20;
				if (enemybullettemp->js == 2)//겐지가 점프상태이면
				{
					enemybullettemp->y += 3;
				}
				else if (enemybullettemp->js == 1)
				{
					enemybullettemp->y += 7;
				}
				else
				{
					enemybullettemp->y += 10;
				}
			}
			else if (enemybullettemp->state == BASTION)
			{
				enemybullettemp->x -= 20;
				enemybullettemp->y -= 8;
			}
			else
			{
				enemybullettemp->x -= 20;
			}

			if (enemybullettemp->x < 0)
			{
				if (enemybullettemp != NULL)
					DeleteEnemyBullet(enemybullettemp);
			}
		}
		if (stun == TRUE)
		{
			stunCount++;
			if (stunCount == 200)
			{
				stun = FALSE;
				stunCount = 0;
			}
		}
		if (clickcount == 10)
		{
			stunCount = 0;
			clickcount = 0;
			stun = FALSE;
		}

		if (ultTime >= 95)
		{
			ultTime = 95;
		}
		InvalidateRect(hwnd, NULL, FALSE);
		break;
	}
				   break;
	case WM_PAINT:
	{
		hdc = BeginPaint(hwnd, &ps);
		{
			HDC memDC = CreateCompatibleDC(hdc);
			SetBkMode(memDC, WHITE_BRUSH);
			HBITMAP memBit = CreateCompatibleBitmap(hdc, rect.right, rect.bottom);
			SelectObject(memDC, memBit);
			FillRect(memDC, &rect, (HBRUSH)GetStockObject(WHITE_BRUSH));

			//

			if (start == FALSE)
			{
				startImage.Draw(memDC, 0, 0);
				Menuclick.Draw(memDC, boxX, boxY - 35, 175 / 2, 156 / 2);
				//Rectangle(memDC, boxX, boxY - 30, boxX + 60, boxY + 30);
				if (Menu_insertid)
				{
					hfont = CreateFont(35, 0, 0, 0, 0, 0, 0, 0, HANGEUL_CHARSET, 0, 0, 0, 0, "Koverwatch");
					oldfont = (HFONT)SelectObject(memDC, hfont);
					SetTextColor(memDC, RGB(255, 255, 255));
					wsprintf(buf, "ID : ");
					TextOut(memDC, 250, 50, buf, strlen(buf));
					TextOut(memDC, 290, 50, IDdata, strlen(IDdata));
					SelectObject(hdc, oldfont);
					DeleteObject(hfont);
				}
				else if (Menu_Ranking)
				{
					hfont = CreateFont(35, 0, 0, 0, 0, 0, 0, 0, HANGEUL_CHARSET, 0, 0, 0, 0, "Koverwatch");
					oldfont = (HFONT)SelectObject(memDC, hfont);
					SetTextColor(memDC, RGB(255, 255, 255));
					RankingImage.Draw(memDC, 0, 0, rect.right, rect.bottom);

					for (int i = 0; i < 10; i++) {
						wsprintf(buf, "%d", i + 1);
						TextOut(memDC, 290, 175 + 45 * i, buf, strlen(buf));
						TextOut(memDC, 420, 175 + 45 * i, Rankdata[i].ID, strlen(Rankdata[i].ID));
						wsprintf(buf, "%d", Rankdata[i].score);
						TextOut(memDC, 750, 175 + 45 * i, buf, strlen(buf));
						tierImage[Rankdata[i].tier].Draw(memDC, 220, 175 + 45 * i, 50, 40);
					}



					SelectObject(hdc, oldfont);
					DeleteObject(hfont);
				}

				else if (Menu_Key)
				{
					KeymenuImage.AlphaBlend(memDC, 400, 100, 225);
				}
			}
			else {
				if (count < 4500) {
					Background2[0].Draw(memDC, 0, 0, 1200, 700, cut1, 0, 1200, 700);
					Background2[1].Draw(memDC, 0, 0, 1200, 700, cut2, 0, 1200, 700);
				}
				else
				{
					Background[0].Draw(memDC, 0, 0, 1200, 700, cut1, 0, 1200, 700);
					Background[1].Draw(memDC, 0, 0, 1200, 700, cut2, 0, 1200, 700);
				}
				coolTime(memDC, ultcoolTime, 600, 0, ultTime);

				if (stun == FALSE&& Genji_Death == FALSE) {

					if (Genji_Dragonmod == TRUE) {
						Genji_Dragon_AttackSprite(memDC, Genji_Dragon_Attack, gx + 40, gy - 40, count, jcount);

					}
					else {
						if (Genji_Dragonmod == FALSE&&jcount == 0)
							showsprite(memDC, genji, gx, gy, Ccount % 2, 0);
						else if (jcount > 0) {
							jumpsp(memDC, genji, gx, gy, 1);
							showsprite(memDC, genji, gx, gy, Ccount % 2, 1);
						}
					}

					//겐지출력
					if (Genji_DragonmodEffect)
					{
						testback.AlphaBlend(memDC, 0, 0, testb);
					}
				}
				if (stun == TRUE && Genji_Death == FALSE) {
					Stun_Ani(memDC, stunani, stunGenji, gx, gy - 100, Ccount + clickcount);
				}


				else if (stun && Genji_Death) {
					Genji_DeathImage.Draw(memDC, gx, gy);
				}



				for (temp = bulletHead; temp != NULL; temp = temp->next) // 표창발사
				{
					if (starselect == 1)
					{
						Dagger_sprite(memDC, Dagger, temp->x, temp->y, 1);
						//star.TransparentBlt(memDC, temp->x, temp->y, temp->r, temp->r, RGB(255, 255, 255));
					}
				}
				for (enemybullettemp = enemybulletHead; enemybullettemp != NULL; enemybullettemp = enemybullettemp->next)
				{
					if (enemybullettemp->state == HANJO)
					{
						arrow.TransparentBlt(memDC, enemybullettemp->x, enemybullettemp->y, 100, 20, 0, 0, 500, 80, RGB(255, 255, 255));
					}
					else if (enemybullettemp->state == BASTION)
					{
						bastionbullet.TransparentBlt(memDC, enemybullettemp->x, enemybullettemp->y, enemybullettemp->r * 2, enemybullettemp->r, 0, 0, 30, 11, RGB(255, 255, 255));
					}
					else if (enemybullettemp->state == PARA)
					{
						parabullet.TransparentBlt(memDC, enemybullettemp->x, enemybullettemp->y, enemybullettemp->r * 2, enemybullettemp->r * 2, 0, 0, 52, 49, RGB(0, 0, 0));
					}
					else
					{
						ebullet.TransparentBlt(memDC, enemybullettemp->x, enemybullettemp->y, enemybullettemp->r * 2, enemybullettemp->r, 0, 0, 30, 11, RGB(255, 255, 255));
					}
				}
				for (etemp = EnemyHead; etemp != NULL; etemp = etemp->next)
				{
					if (etemp->state == HANJO)
					{
						if (etemp->enemydie == FALSE) {
							Soldier_Ani(memDC, Hanjo_Move, etemp->x - etemp->r, etemp->y - etemp->r - 35, Ccount % 2, 225, 188);
						}
						else
						{
							Hanjo_Move[0].AlphaBlend(memDC, etemp->x - 100, etemp->y - 100, 255, 188, 0, 0, 300, 250, etemp->invi);
						}
					}
					else if (etemp->state == PARA) //파라는 공중캐릭이기때문에 크리에이트에서 y좌표가 남들보다 떠있음
					{
						if (etemp->enemydie == FALSE)
						{
							paraboost(memDC, P_Boost, etemp->x, etemp->y - 75, count);
							Para.Draw(memDC, etemp->x - 100, etemp->y - 100, 200, 180);
							//파라이면 파라비트맵 출력
						}
						else
						{
							paraboost(memDC, P_Boost, etemp->x, etemp->y - 75, count);
							Para.AlphaBlend(memDC, etemp->x - 100, etemp->y - 100, 200, 180, 0, 0, 415, 365, etemp->invi);
						}
					}
					else if (etemp->state == MACREE)
					{
						if (etemp->enemydie == FALSE)
						{

							Soldier_Ani(memDC, Mac_Move, etemp->x - etemp->r, etemp->y - 80, Ccount % 2, 225, 188);
						}
						else
						{
							Mac_Move[0].AlphaBlend(memDC, etemp->x - 80, etemp->y - 70, 200, 180, 0, 0, 350, 350, etemp->invi);
						}

						//맥크리이면 맥크리비트맵출력
					}
					else if (etemp->state == LINE1)
					{
						if (etemp->enemydie == FALSE) {
							img[Ccount % 2].Draw(memDC, etemp->x, etemp->y - 100);
						}
						else
						{
							img[0].AlphaBlend(memDC, etemp->x, etemp->y - 100, etemp->invi);
						}
						//라인하르트이면 라인하르트 비트맵출력
					}
					else if (etemp->state == LINE2)
					{
						Line2.TransparentBlt(memDC, etemp->x, etemp->y - 100, 200, 200, 0, 0, 300, 300, RGB(255, 255, 255));
						//맥크리이면 맥크리비트맵출력
					}
					else if (etemp->state == SOLDIER)
					{
						if (etemp->enemydie == FALSE) {
							Soldier_Ani(memDC, soldier, etemp->x - 130, etemp->y - 150, Ccount % 9, 300, 300);
						}
						else
						{
							soldier[0].AlphaBlend(memDC, etemp->x - 130, etemp->y - 150, etemp->invi);
						}
						//맥크리이면 맥크리비트맵출력
					}
					else if (etemp->state == BASTION)
					{
						if (etemp->enemydie == FALSE) {
							Bastion_sprite(memDC, bastion, etemp->x, etemp->y);
						}
						else
						{
							bastion[0].AlphaBlend(memDC, etemp->x, etemp->y, 140, 120, 0, 0, 105, 79, etemp->invi);
						}
					}
					else if (etemp->state == HEAL)
					{
						healpack.TransparentBlt(memDC, etemp->x, etemp->y, 80, 80, 0, 0, 371, 412, RGB(255, 255, 255));
					}
					else if (etemp->state == RIPER)
					{
						etemp->time++;
						if (etemp->time < 100)
						{
							etemp->invi += 2;
							etemp->invisible = FALSE;
						}
						if (etemp->time > 100)
						{
							etemp->invi -= 2;
							etemp->invisible = TRUE;
						}
						if (etemp->time > 190)
						{
							etemp->time = 0;
						}
						riper.AlphaBlend(memDC, etemp->x, etemp->y - 130, etemp->invi);

					}
					else if (etemp->state == SKILL_DRAGON)// 한조 궁 출력
					{
						for (int i = 19; i >= 0; i--)
						{
							if (i == 0)
								Hanjo_Skill[0].Draw(memDC, Dpos[i].x - 90, Dpos[i].y - 25);
							else if (i == 19)
								Hanjo_Skill[2].Draw(memDC, Dpos[i].x, Dpos[i].y);
							else
								Hanjo_Skill[1].Draw(memDC, Dpos[i].x, Dpos[i].y);
						}
					}
					else if (etemp->state == tutomob)
					{
						if (etemp->enemydie == FALSE) {
							joldagu.Draw(memDC, etemp->x, etemp->y, 80, 80,
								90 * (Ccount % 4), 0, 90, 86);
						}
						else
						{
							joldagu.AlphaBlend(memDC, etemp->x, etemp->y, 80, 86, 0, 0, 80, 86, etemp->invi);
						}
					}
				}


				for (int i = 0; i < hp / 100; i++) {
					hpRec.TransparentBlt(memDC, 100 + i * 30, 0, 40, 30, 0, 0, 54, 44, RGB(255, 255, 255));
				}
				for (etemp = EnemyHead; etemp != NULL; etemp = etemp->next)
				{
					if (etemp->hit == suriken) {
						Hit_Ani(memDC, hit, etemp->x, etemp->y, &etemp->hit);
					}
					if (etemp->hit == jilpung) {
						Hit_Ani(memDC, hit, etemp->x, etemp->y, &etemp->hit);
					}
				}
				if (bulhit == TRUE)
				{
					BulletHit_Ani(memDC, bullet_hit, gx + 100, gy + 50, &bulhit);
				}
				if (ghit == TRUE)
				{
					Explode_Ani(memDC, explode, gx, gy, &ghit);
				}
				hfont = CreateFont(40, 0, 0, 0, 0, 0, 0, 0, HANGEUL_CHARSET, 0, 0, 0, 0, "Koverwatch");
				oldfont = (HFONT)SelectObject(memDC, hfont);
				SetTextColor(memDC, RGB(255, 255, 255));
				wsprintf(buf, "%d", hp);
				TextOut(memDC, 0, 00, buf, strlen(buf));
				wsprintf(buf, "점수:%d 콤보:%d", score, COMBO);
				TextOut(memDC, 750, 00, buf, strlen(buf));
				if (hanjoultOn == TRUE)
				{
					Hanjoultra(memDC, Hanjocutscene, 1000, 300, &hanjoultOn);
				}
				if (Effect_1) {
					Effect_1 = Animation(Effect, memDC, 70, gy + 40, Effect_1);
				}
				if (Genji_DragonmodEffect == TRUE)
				{
					GenjiDragon(memDC, Genji_Dragon, gx, gy - 150, count);
				}

				if (count < 50) {
					wsprintf(buf, "stage1");
					TextOut(memDC, 500, 500, buf, strlen(buf));
				}
				if (count>3000 && count < 3050) {
					wsprintf(buf, "stage2");
					TextOut(memDC, 500, 500, buf, strlen(buf));
				}
				if (count>6000 && count < 6050) {
					wsprintf(buf, "stage3");
					TextOut(memDC, 500, 500, buf, strlen(buf));
				}
				if (count>9000 && count < 9050) {
					wsprintf(buf, "stage4");
					TextOut(memDC, 500, 500, buf, strlen(buf));
				}
				if (count>12000 && count < 12050) {
					wsprintf(buf, "stage5");
					TextOut(memDC, 500, 500, buf, strlen(buf));
				}
				if (count>15000 && count < 15050) {
					wsprintf(buf, "stage6");
					TextOut(memDC, 500, 500, buf, strlen(buf));
				}

				skillCool1[S1_count].TransparentBlt(memDC, 500, 0, 50, 50, 0, 0, 50, 50, RGB(255, 255, 255));
				SelectObject(memDC, oldfont);
				DeleteObject(hfont);

			}
			StretchBlt(hdc, 0, 0, rect.right, rect.bottom, memDC, DCx, DCy, DCw, DCh, SRCCOPY);//출력용
			if (deathcount == 50 && start)
			{
				SetBkMode(hdc, WHITE_BRUSH);
				result.Draw(hdc, 550, 50);
				hfont = CreateFont(58, 0, 0, 0, 0, 0, 0, 0, HANGEUL_CHARSET, 0, 0, 0, 0, "Koverwatch");
				oldfont = (HFONT)SelectObject(hdc, hfont);
				SetTextColor(hdc, RGB(255, 255, 255));

				wsprintf(buf, "%d", SaveMaxCombo);
				TextOut(hdc, 780, 180, buf, strlen(buf));
				wsprintf(buf, "%d", Killcount);
				TextOut(hdc, 780, 285, buf, strlen(buf));
				wsprintf(buf, "%d", score + (Killcount * 2) + (SaveMaxCombo * 5));
				TextOut(hdc, 810, 390, buf, strlen(buf));
				tierImage[tempdata.tier].Draw(hdc, 700, 500);
			}
			SelectObject(hdc, oldfont);
			DeleteObject(hfont);
			DeleteObject(memBit);
			DeleteDC(memDC);
		}
		EndPaint(hwnd, &ps);
		//ondraw(hwnd, rect, img);
	}
	break;
	case WM_DESTROY:
		FMOD_System_Release(soundSystem);
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hwnd, iMessage, wParam, IParam));
}


void Animation(CImage * img, HDC memdc, int x, int y, int count)
{
	img[count].TransparentBlt(memdc, x, y, 125, 125, RGB(255, 255, 255));
}
void CreateBullet(int px, int py)
{
	Bullet * newBullet = (Bullet*)malloc(sizeof(Bullet));
	newBullet->x = px;
	newBullet->y = py + 20;
	newBullet->r = 30;
	if (bulletHead == NULL || bulletHead->next == NULL)
	{
		if (bulletHead == NULL)
		{
			bulletHead = newBullet;
			newBullet->next = NULL;
		}
		if (bulletHead->next == NULL)
		{
			bulletHead->next = newBullet;
			newBullet->next = NULL;
		}
	}
	else
	{
		for (temp = bulletHead; temp->next != NULL; temp = temp->next)
		{
		}
		temp->next = newBullet;
		newBullet->next = NULL;
	}
}
void DeleteBullet(Bullet * delbullet)
{
	if (delbullet == bulletHead)
	{
		if (delbullet->next == NULL)
		{
			bulletHead = NULL;
		}
		else
		{
			bulletHead = bulletHead->next;
		}
	}
	else
	{
		for (temp = bulletHead; temp->next != delbullet; temp = temp->next)
		{
		}
		temp->next = delbullet->next;
		free(delbullet);
	}
}

void DeleteEnemy(Enemy * delenemy)
{
	if (delenemy == EnemyHead)
	{
		if (EnemyHead->next == NULL)
		{
			EnemyHead = NULL;
		}
		else
		{
			EnemyHead = EnemyHead->next;
		}
	}
	else
	{
		if (etemp != nullptr) {
			for (etemp = EnemyHead; etemp->next != delenemy; etemp = etemp->next)
			{
			}
			etemp->next = delenemy->next;
			if (delenemy != nullptr) {
				free(delenemy);
			}
		}
	}
}
Enemy *  CreateEnemy(int state)
{
	Enemy * newEnemy = (Enemy*)malloc(sizeof(Enemy));
	newEnemy->x = 1200;
	newEnemy->enemydie = FALSE;
	newEnemy->time = 0;
	newEnemy->invi = 0;
	newEnemy->hit = 0;
	if (state == PARA) {
		newEnemy->y = 100;
		newEnemy->r = 70;
	}
	else
	{
		newEnemy->y = 500;
		newEnemy->r = 70;
	}
	newEnemy->state = state;
	if (state == HANJO)
	{
		newEnemy->hp = 70;
	}
	if (state == RIPER)
	{
		newEnemy->invisible = FALSE;
		newEnemy->hp = 70;
	}
	else if (state == PARA)
	{
		newEnemy->hp = 40;
		newEnemy->py = 3;
	}
	else if (state == LINE1)
	{
		newEnemy->hp = 200;
	}
	else if (state == LINE2)
	{
		newEnemy->hp = 1200;
	}
	else if (state == MACREE)
	{
		newEnemy->hp = 50;
	}
	else if (state == SOLDIER)
	{
		newEnemy->hp = 50;
	}
	else if (state == BASTION)
	{
		newEnemy->x = 1000;
		newEnemy->y = 450;
		newEnemy->hp = 30;
	}
	else if (state == SKILL_DRAGON)
	{
		newEnemy->x = 1300;
		newEnemy->y = cos(AngletoRadi(Dpos[0].x)) * 40 + rand() % 600;
		newEnemy->hp = 3000;

	}
	else if (state == tutomob)
	{
		newEnemy->r = 70;
		newEnemy->hp = 10;
		newEnemy->x = 1200;
		newEnemy->y = rand() % 400;
	}
	else if (state == HEAL)
	{
		newEnemy->x = 1200;
		newEnemy->y = 200;
		newEnemy->hp = 100000000;
	}
	if (EnemyHead == NULL || EnemyHead->next == NULL)
	{
		if (EnemyHead == NULL)
		{
			EnemyHead = newEnemy;
			EnemyHead->next = NULL;
		}
		if (EnemyHead->next == NULL)
		{
			EnemyHead->next = newEnemy;
			newEnemy->next = NULL;
		}
	}
	else
	{
		for (etemp = EnemyHead; etemp->next != NULL; etemp = etemp->next)
		{
		}
		etemp->next = newEnemy;
		newEnemy->next = NULL;
	}
	return newEnemy;
}
void DeleteEnemyBullet(EnemyBullet * delbullet)
{
	if (delbullet == enemybulletHead)
	{
		if (delbullet->next == NULL)
		{
			enemybulletHead = NULL;
		}
		else
		{
			enemybulletHead = enemybulletHead->next;
		}
	}
	else
	{

		for (enemybullettemp = enemybulletHead; enemybullettemp->next != delbullet; enemybullettemp = enemybullettemp->next)
		{
		}
		enemybullettemp->next = delbullet->next;
		if (enemybullettemp != nullptr) {
			free(delbullet);
		}
	}
}
void CreateEnemyBullet(int ex, int ey, int state, int jcount)
{
	EnemyBullet * newBullet = (EnemyBullet*)malloc(sizeof(EnemyBullet));
	newBullet->x = ex;
	newBullet->y = ey;
	if (state == HANJO)
	{
		newBullet->y = ey - 10;
	}
	if (state == SOLDIER)
	{
		newBullet->y = ey - 10;
	}
	newBullet->r = 20;
	newBullet->state = state;
	if (jcount == 0) {
		newBullet->js = 0;
	}
	else if (jcount == 1)
	{
		newBullet->js = 1;
	}
	else if (jcount == 2)
	{
		newBullet->js = 2;
	}
	if (enemybulletHead == NULL || enemybulletHead->next == NULL)
	{
		if (enemybulletHead == NULL)
		{
			enemybulletHead = newBullet;
			enemybulletHead->next = NULL;
		}
		if (enemybulletHead->next == NULL)
		{
			enemybulletHead->next = newBullet;
			newBullet->next = NULL;
		}
	}
	else
	{
		for (enemybullettemp = enemybulletHead; enemybullettemp->next != NULL; enemybullettemp = enemybullettemp->next)
		{
		}
		enemybullettemp->next = newBullet;
		newBullet->next = NULL;
	}
}

BOOL CollisionCheck(int x, int y, int mx, int my, int size)
{
	if (LengthCheck(x, y, mx, my)<size)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}
double LengthCheck(int x, int y, int mx, int my)
{
	return sqrt((x - mx)*(x - mx) + (y - my)*(y - my));
}
bool Animation(CImage * img, HDC memdc, int x, int y, bool a)
{
	static int count = 0;
	if (count == 7) {
		count = 0;
		a = FALSE;
		return a;
	}
	img[count].Draw(memdc, x, y, 650, 159);
	count++;
	return a;
}
void Soldier_Ani(HDC hdc, CImage *c, int x, int y, int count, int sizex, int sizey)
{
	c[count].Draw(hdc, x, y, sizex, sizey);
}

void stage(int count) // 시간초에 따라서 나옴.
					  //stage1 등장몹 : 솔져, 맥크리
{
	int randnum;
	if (count == 100)
	{
		CreateEnemy(SOLDIER);
	}
	if (count == 200)
	{
		CreateEnemy(SOLDIER);
	}
	if (count == 400)
	{
		CreateEnemy(MACREE);
	}
	if (count == 600)
	{
		CreateEnemy(SOLDIER);
	}
	if (count == 800)
	{
		CreateEnemy(SOLDIER);
	}
	if (count == 950)
	{
		CreateEnemy(SOLDIER);
	}
	if (count == 1100)
	{
		CreateEnemy(MACREE);
	}
	if (count == 1350)
	{
		CreateEnemy(SOLDIER);
	}
	if (count == 1500)
	{
		CreateEnemy(SOLDIER);
	}
	if (count == 1700)
	{
		CreateEnemy(SOLDIER);
	}
	if (count == 1900)
	{
		CreateEnemy(MACREE);
	}
	if (count == 2100)
	{
		CreateEnemy(SOLDIER);
	}
	if (count == 2250)
	{
		CreateEnemy(SOLDIER);
	}
	if (count == 2600)
	{
		CreateEnemy(SOLDIER);
	}
	if (count == 2900)
	{
		CreateEnemy(MACREE);
	}
	//2스테이지 등장몹 솔져, 맥크리, 한조
	if (count == 3100)
	{
		CreateEnemy(HEAL);
		CreateEnemy(SOLDIER);
	}
	if (count == 3300)
	{
		CreateEnemy(HANJO);
	}
	if (count == 3550)
	{
		CreateEnemy(MACREE);
	}
	if (count == 3750)
	{
		CreateEnemy(HANJO);
	}
	if (count == 4000)
	{
		CreateEnemy(HANJO);
	}
	if (count == 4300)
	{
		CreateEnemy(SOLDIER);
		CreateEnemy(MACREE);
	}
	if (count == 4700)
	{
		CreateEnemy(SOLDIER);
		CreateEnemy(MACREE);
	}
	if (count == 4850)
	{
		CreateEnemy(HANJO);
	}
	if (count == 4930)
	{
		CreateEnemy(HANJO);
	}
	if (count == 5120)
	{
		CreateEnemy(HANJO);
	}
	if (count == 5500)
	{
		CreateEnemy(MACREE);
	}
	if (count == 5900)
	{
		CreateEnemy(MACREE);
		CreateEnemy(HANJO);
	}
	//3스테이지 한조, 맥크리, 솔져, 파라

	if (count == 6100)
	{
		CreateEnemy(HEAL);
		CreateEnemy(SOLDIER);
		CreateEnemy(PARA);
	}
	if (count == 6300)
	{
		CreateEnemy(HANJO);
	}
	if (count == 6550)
	{
		CreateEnemy(MACREE);
	}
	if (count == 6750)
	{
		CreateEnemy(HANJO);
		CreateEnemy(PARA);
	}
	if (count == 7000)
	{
		CreateEnemy(PARA);
	}
	if (count == 7300)
	{
		CreateEnemy(SOLDIER);
		CreateEnemy(MACREE);
		CreateEnemy(PARA);
	}
	if (count == 7700)
	{
		CreateEnemy(SOLDIER);
		CreateEnemy(MACREE);
	}
	if (count == 7850)
	{
		CreateEnemy(PARA);
		CreateEnemy(HANJO);
	}
	if (count == 7930)
	{
		CreateEnemy(PARA);
		CreateEnemy(HANJO);
	}
	if (count == 8120)
	{
		CreateEnemy(HANJO);
	}
	if (count == 8500)
	{
		CreateEnemy(MACREE);
	}
	if (count == 8900)
	{
		CreateEnemy(PARA);
		CreateEnemy(MACREE);
		CreateEnemy(HANJO);
	}
	//4스테이지 파라 맥크리 한조 바스티온 솔져

	if (count == 9100)
	{
		CreateEnemy(HEAL);
		CreateEnemy(PARA);
		CreateEnemy(BASTION);
	}
	if (count == 9300)
	{
		CreateEnemy(PARA);
		CreateEnemy(HANJO);
	}
	if (count == 9550)
	{
		CreateEnemy(MACREE);
		CreateEnemy(BASTION);
	}
	if (count == 9750)
	{
		CreateEnemy(HANJO);
		CreateEnemy(PARA);
	}
	if (count == 10000)
	{
		CreateEnemy(HANJO);
		CreateEnemy(PARA);
	}
	if (count == 10300)
	{
		CreateEnemy(SOLDIER);
		CreateEnemy(MACREE);
		CreateEnemy(PARA);
		CreateEnemy(BASTION);
	}
	if (count == 10700)
	{
		CreateEnemy(SOLDIER);
		CreateEnemy(MACREE);
	}
	if (count == 10850)
	{
		CreateEnemy(PARA);
		CreateEnemy(HANJO);
		CreateEnemy(BASTION);
	}
	if (count == 10930)
	{
		CreateEnemy(PARA);
		CreateEnemy(HANJO);
	}
	if (count == 11120)
	{
		CreateEnemy(HANJO);
	}
	if (count == 11500)
	{
		CreateEnemy(SOLDIER);
	}
	if (count == 11900)
	{
		CreateEnemy(PARA);
		CreateEnemy(MACREE);
		CreateEnemy(HANJO);
	}
	//5스테이지 4스테이지+라인1,2
	if (count == 12100)
	{
		CreateEnemy(HEAL);
		CreateEnemy(PARA);
		CreateEnemy(BASTION);
	}
	if (count == 12300)
	{
		CreateEnemy(PARA);
		CreateEnemy(HANJO);
	}
	if (count == 12550)
	{
		CreateEnemy(LINE1);
		CreateEnemy(MACREE);
		CreateEnemy(BASTION);
	}
	if (count == 12750)
	{
		CreateEnemy(LINE2);
		FMOD_System_PlaySound(soundSystem, soundFile[3], NULL, 0, &channel);
		CreateEnemy(HANJO);
		CreateEnemy(PARA);
	}
	if (count == 13000)
	{
		CreateEnemy(HANJO);
		CreateEnemy(PARA);
	}
	if (count == 13300)
	{
		CreateEnemy(LINE2);
		FMOD_System_PlaySound(soundSystem, soundFile[3], NULL, 0, &channel);
		CreateEnemy(SOLDIER);
		CreateEnemy(MACREE);
		CreateEnemy(PARA);
		CreateEnemy(BASTION);
	}
	if (count == 13700)
	{
		CreateEnemy(LINE2);
		FMOD_System_PlaySound(soundSystem, soundFile[3], NULL, 0, &channel);
		CreateEnemy(SOLDIER);
		CreateEnemy(MACREE);
	}
	if (count == 13850)
	{
		CreateEnemy(PARA);
		CreateEnemy(HANJO);
		CreateEnemy(BASTION);
		CreateEnemy(LINE2);
		FMOD_System_PlaySound(soundSystem, soundFile[3], NULL, 0, &channel);
	}
	if (count == 13930)
	{
		CreateEnemy(PARA);
		CreateEnemy(HANJO);
	}
	if (count == 14120)
	{
		CreateEnemy(HANJO);
	}
	if (count == 14500)
	{
		CreateEnemy(LINE2);
		FMOD_System_PlaySound(soundSystem, soundFile[3], NULL, 0, &channel);
		CreateEnemy(SOLDIER);
	}
	if (count == 14900)
	{
		CreateEnemy(PARA);
		CreateEnemy(MACREE);
		CreateEnemy(HANJO);
	}
	if (count > 15000)
	{
		if (count % 200==0)
		{
			randnum = rand() % 7 + 1;
			CreateEnemy(randnum);
			CreateEnemy(randnum+1);
		}
		if (count % 2000==0)
		{
			CreateEnemy(HEAL);
		}
	}
}