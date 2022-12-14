//--------------------------
//Author:三上航世
//チュ−トリアル画面の処理(tutorial.cpp)
//--------------------------
#include "input.h"
#include "bg.h"
#include "sound.h"
#include "fade.h"
#include "tutorialui.h"

//初期化処理
void InitTutorial(void)
{
	InittutorialUi();
}

//チュートリアル画面の終了処理
void UninitTutorial(void)
{
	UninittutorialUi();
}

//チュートリアル画面の更新処理
void UpdateTutorial(void)
{
	UpdatetutorialUi();
}

//描画処理
void DrawTutorial(void)
{
	DrawtutorialUi();
}