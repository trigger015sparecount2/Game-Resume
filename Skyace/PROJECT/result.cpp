//--------------------------
//Author:三上航世
//リザルト画面の処理(result.cpp)
//--------------------------
#include "input.h"
#include "result.h"
#include "bg.h"
#include "fade.h"
#include "sound.h"
#include "ui.h"
#include "rank.h"
#include "resultui.h"

//グローバル宣言
int g_ntime;

//初期化処理
void InitResult(void)
{
	SetBg(0,false);
	Rankmain(Score(),Kill());
	InitresultUi();
	g_ntime = 0;
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
	g_ntime++;
	//J押すかつ時間内または時間過ぎる
	if (GetKeyboardTrigger(DIK_J) == true && GetKeyboardRelease(DIK_J) == false && 300 <= g_ntime && g_ntime < 1200 || g_ntime == 1200)
	{
		SetFade(MODE_TITLE, true);
		//J押すかつ時間内、音を鳴らす
		if (GetKeyboardTrigger(DIK_J) == true && GetKeyboardRelease(DIK_J) == false && 300 <= g_ntime && g_ntime < 1200)
		{
			PlaySound(SOUND_LABEL_YES000);
		}
	}
}

//描画処理
void DrawResult(void)
{
	DrawresultUi();
}