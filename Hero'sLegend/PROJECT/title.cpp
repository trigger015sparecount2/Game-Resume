//--------------------------
//Author:三上航世
//タイトル画面の処理(title.cpp)
//--------------------------
#include "input.h"
#include "title.h"
#include "fade.h"
#include "mouse.h"
#include "gamepad.h"
#include "titleUi.h"
#include "sound.h"

//初期化処理
void InitTitle(void)
{
	InittitleUi();
	PlaySound(SOUND_LABEL_BGM0);
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
	if (GetAnyKey() == true || GetMouseButton(DIM_L) == true || GetAnyButton() == true)
	{
		if (GetFade() == FADE_NONE)
		{
			SetFade(MODE_TUTORIAL);
		}
	}
}

//描画処理
void DrawTitle(void)
{
	
}