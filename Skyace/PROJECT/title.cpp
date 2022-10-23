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

//初期化処理
void InitTitle(void)
{
	InittitleUi();
	SetBg(1,false);
	PlaySound(SOUND_LABEL_BGM000);
}

//タイトル画面の終了処理
void UninitTitle(void)
{
	UninittitleUi();
}

//タイトル画面の更新処理
void UpdateTitle(void)
{
	UpdatetitleUi();
	if (GetKeyboardTrigger(DIK_J) == true && GetKeyboardRelease(DIK_J) == false)
	{
		PlaySound(SOUND_LABEL_YES000);
		SetFade(MODE_CHOICE, true);
	}
}

//描画処理
void DrawTitle(void)
{
	DrawtitleUi();
}