//--------------------------
//Author:三上航世
//リザルト画面の処理(result.cpp)
//--------------------------
#include "input.h"
#include "result.h"
#include "bg.h"
#include "fade.h"
#include "sound.h"
#include "gameui.h"
#include "rank.h"
#include "padinput.h"
#include "resultui.h"

//グローバル宣言
int g_nRtime;
bool g_bDemo;

//初期化処理
void InitResult(void)
{
	SetBg(0, false);
	Rankmain(TimeMin(), TimeSec(), g_bDemo);
	InitresultUi();
	g_nRtime = 0;
	if (g_bDemo == false)
	{
		if (TimeMin() == TIME_LIMIT_MIN)
		{
			PlaySound(SOUND_LABEL_BGM003);
		}
		else
		{
			PlaySound(SOUND_LABEL_BGM002);
		}
	}
}

//リザルト画面の終了処理
void UninitResult(void)
{
	UninitresultUi();
}

//リザルト画面の更新処理
void UpdateResult(void)
{
	UpdateresultUi();
	g_nRtime++;

	if (GetAnyKey() == true || GetAnyButton() == true || g_nRtime == 1200)
	{
		if (GetFade() == FADE_NONE && g_bDemo == true || g_bDemo == false && g_nRtime > 300 && GetFade() == FADE_NONE)
		{
			SetFade(MODE_TITLE);
			if (GetAnyKey() == true || GetAnyButton() == true)
			{
				PlaySound(SOUND_LABEL_YES);
			}
		}
	}
}

//描画処理
void DrawResult(void)
{
	DrawresultUi();
}

void Demoresult(bool bDemo)
{
	g_bDemo = bDemo;
}

bool Demo(void)
{
	return g_bDemo;
}

int ResultTime(void)
{
	return g_nRtime;
}