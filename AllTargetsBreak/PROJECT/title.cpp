//--------------------------
//Author:三上航世
//タイトル画面の処理(title.cpp)
//--------------------------
#include "input.h"
#include "title.h"
#include "fade.h"
#include "sound.h"
#include "bg.h"
#include "titleUi.h"
#include "result.h"
#include "padinput.h"

//グローバル変数
int g_nTtime;

//初期化処理
void InitTitle(void)
{
	InittitleUi();
	g_nTtime = 0;
	SetBg(1, false);
	if (Demo() == false)
	{
		PlaySound(SOUND_LABEL_BGM000);
	}
}

//タイトル画面の終了処理
void UninitTitle(void)
{
	UninittitleUi();
}

//タイトル画面の更新処理
void UpdateTitle(void)
{
	g_nTtime++;
	UpdatetitleUi();
	
	//何か押すかつフェード動いてない
	if (GetAnyKey() == true && GetFade() == FADE_NONE || GetAnyButton() == true && GetFade() == FADE_NONE)
	{
		//チュートリアルに移行
		PlaySound(SOUND_LABEL_YES);
		SetFade(MODE_TUTORIAL);
		Demoresult(false);
	}
	//時間がたつかつフェード動いてない
	if (g_nTtime >= 1200 && GetFade() == FADE_NONE)
	{
		//リザルトに移行
		SetFade(MODE_RESULT);
		Demoresult(true);
	}
}

//描画処理
void DrawTitle(void)
{
	DrawtitleUi();
}