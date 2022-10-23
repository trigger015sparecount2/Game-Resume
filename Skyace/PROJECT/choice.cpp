//--------------------------
//Author:三上航世
//選択画面の処理(choice.cpp)
//--------------------------
#include "choice.h"
#include "bg.h"
#include "choiceui.h"

//初期化処理
void InitChoice(void)
{
	InitchoiceUi();
	SetBg(0,false);
}

//選択画面の終了処理
void UninitChoice(void)
{
	UninitchoiceUi();
}

//選択画面の更新処理
void UpdateChoice(void)
{
	UpdatechoiceUi();
}

//描画処理
void DrawChoice(void)
{
	DrawchoiceUi();
}