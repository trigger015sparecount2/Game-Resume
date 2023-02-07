//--------------------------
//Author:三上航世
//ゲーム画面の処理(game.cpp)
//--------------------------
#include "input.h"
#include "game.h"
#include "bg.h"
#include "player.h"
#include "gameui.h"
#include "sound.h"
#include "fade.h"
#include "target.h"
#include "block.h"
#include "pauseui.h"
#include "cross.h"
#include "padinput.h"

//グローバル変数
bool g_bStop; //ゲームが一時停止してるかどうか

//初期化処理
void InitGame(void)
{
	InitPlayer();
	InitBlock();
	InitCross();
	InitTarget();
	InitGameUi();
	InitPauseUi();
	SetObject();
	g_bStop = false;
	SetBg(1, true);
	PlaySound(SOUND_LABEL_BGM001);
}

//ゲーム画面の終了処理
void UninitGame(void)
{
	UninitBlock();
	UninitTarget();
	UninitPlayer();
	UninitCross();
	UninitGameUi();
	UninitPauseUi();
}

//ゲーム画面の更新処理
void UpdateGame(void)
{
	if (GetKeyboardKey(DIK_P) == true && GameOver() == false || GetGamePadButton(11) == true && GameOver() == false)
	{
		if (g_bStop == false)
		{
			PlaySound(SOUND_LABEL_PAUSE);
			g_bStop = true;
		}
		else
		{
			PlaySound(SOUND_LABEL_CANCEL);
			g_bStop = false;
		}
	}
	if (g_bStop == false)
	{
		ScrollBg(true);
		UpdateBlock();
		UpdatePlayer();
		UpdateTarget();
		UpdateCross();
		UpdateGameUi();
	}
	else
	{
		UpdatePauseUi();
		ScrollBg(false);
	}
}

//描画処理
void DrawGame(void)
{
	DrawBlock();
	DrawTarget();
	DrawCross();
	DrawPlayer();
	DrawGameUi();
	if (g_bStop == true)
	{
		DrawPauseUi();
	}
}

//ゲームスタート
void StartGame(void)
{
	g_bStop = false; 
}

//リスタート処理
void RestartGame(void)
{
	ResetBlock();
	ResetPlayer();
	ResetCross();
	ResetTarget();
	ResetGameUi();
	SetObject();
	SetBg(1, true);
	StopSound();
	PlaySound(SOUND_LABEL_BGM001);
}

//オブジェクトセットさせる
void SetObject(void)
{
	SetTarget(D3DXVECTOR3(260.0f, 620.0f, 0.0f));  //真ん中下左側降りた先
	SetTarget(D3DXVECTOR3(820.0f, 550.0f, 0.0f));  //真ん中下右側
	SetTarget(D3DXVECTOR3(1210.0f, 280.0f, 0.0f)); //右隙間狙い
	SetTarget(D3DXVECTOR3(25.0f, 100.0f, 0.0f));   //左タイミング
	SetTarget(D3DXVECTOR3(1200.0f, 620.0f, 0.0f)); //右下ワープホールに手裏剣
	SetTarget(D3DXVECTOR3(500.0f, 160.0f, 0.0f));  //真ん中上
	SetBlock(D3DXVECTOR3(0.0f, 700.0f, 0.0f), 1280.0f, 20.0f, D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0, BLOCK_NORMAL);    //画面下
	SetBlock(D3DXVECTOR3(0.0f, 0.0f, 0.0f), 20.0f, 720.0f, D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0, BLOCK_NORMAL);       //画面左
	SetBlock(D3DXVECTOR3(20.0f, 0.0f, 0.0f), 1260.0f, 20.0f, D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0, BLOCK_NORMAL);     //画面上
	SetBlock(D3DXVECTOR3(1260.0f, 20.0f, 0.0f), 20.0f, 680.0f, D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0, BLOCK_NORMAL);   //画面右
	SetBlock(D3DXVECTOR3(580.0f, 460.0f, 0.0f), 100.0f, 20.0f, D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0, BLOCK_NORMAL);   //スタート地点
	SetBlock(D3DXVECTOR3(1200.0f, 20.0f, 0.0f), 20.0f, 270.0f, D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0, BLOCK_NORMAL);   //右のわずかな隙間がある壁上側
	SetBlock(D3DXVECTOR3(1200.0f, 320.0f, 0.0f), 20.0f, 280.0f, D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0, BLOCK_NORMAL);  //右のわずかな隙間がある壁下側
	SetBlock(D3DXVECTOR3(1100.0f, 160.0f, 0.0f), 60.0f, 20.0f, D3DXVECTOR3(0.0f, 2.2f, 0.0f), 280, BLOCK_NORMAL); //右のわずかな隙間がある壁付近の動く床
	SetBlock(D3DXVECTOR3(80.0f, 20.0f, 0.0f), 20.0f, 440.0f, D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0, BLOCK_NORMAL);     //左のわずかな隙間がある壁
	SetBlock(D3DXVECTOR3(80.0f, 460.0f, 0.0f), 320.0f, 20.0f, D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0, BLOCK_NORMAL);    //左下の囲みブロック上
	SetBlock(D3DXVECTOR3(120.0f, 480.0f, 0.0f), 20.0f, 100.0f, D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0, BLOCK_NORMAL);   //左下の囲みブロック右
	SetBlock(D3DXVECTOR3(20.0f, 580.0f, 0.0f), 120.0f, 20.0f, D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0, BLOCK_NORMAL);    //左下の囲みブロック下
	SetBlock(D3DXVECTOR3(-80.0f, 180.0f, 0.0f), 80.0f, 20.0f, D3DXVECTOR3(1.2f, 0.0f, 0.0f), 160, BLOCK_NORMAL);  //左妨害用ブロック左側
	SetBlock(D3DXVECTOR3(100.0f, 220.0f, 0.0f), 80.0f, 20.0f, D3DXVECTOR3(-0.3f, 0.0f, 0.0f), 220, BLOCK_NORMAL); //左妨害用ブロック右側
	SetBlock(D3DXVECTOR3(240.0f, 340.0f, 0.0f), 60.0f, 20.0f, D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0, BLOCK_ICE);		  //スタート地点左側の氷ブロック
	SetBlock(D3DXVECTOR3(280.0f, 120.0f, 0.0f), 800.0f, 20.0f, D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0, BLOCK_NORMAL);	  //スタート地点上の上側横長ブロック
	SetBlock(D3DXVECTOR3(560.0f, 20.0f, 0.0f), 20.0f, 40.0f, D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0, BLOCK_NORMAL);     //真ん中上左側上ブロック
	SetBlock(D3DXVECTOR3(560.0f, 100.0f, 0.0f), 20.0f, 40.0f, D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0, BLOCK_NORMAL);    //真ん中上左側下ブロック
	SetBlock(D3DXVECTOR3(600.0f, 30.0f, 0.0f), 20.0f, 80.0f, D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0, BLOCK_WARP);       //真ん中上左側ワープホール
	SetBlock(D3DXVECTOR3(1110.0f, 610.0f, 0.0f), 20.0f, 80.0f, D3DXVECTOR3(0.0f, 0.0f, 0.0f), 1, BLOCK_WARP);     //右下ワープホール
	SetBlock(D3DXVECTOR3(1080.0f, 600.0f, 0.0f), 20.0f, 100.0f, D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0, BLOCK_NORMAL);  //右下囲みブロック縦
	SetBlock(D3DXVECTOR3(1080.0f, 580.0f, 0.0f), 180.0f, 20.0f, D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0, BLOCK_NORMAL);  //右下囲みブロック横
	SetBlock(D3DXVECTOR3(640.0f, 20.0f, 0.0f), 20.0f, 100.0f, D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0, BLOCK_NORMAL);    //真ん中上壁
	SetBlock(D3DXVECTOR3(680.0f, 30.0f, 0.0f), 20.0f, 80.0f, D3DXVECTOR3(0.0f, 0.0f, 0.0f), 2, BLOCK_WARP);       //真ん中上右側ワープホール
	SetBlock(D3DXVECTOR3(90.0f, 490.0f, 0.0f), 20.0f, 80.0f, D3DXVECTOR3(0.0f, 0.0f, 0.0f), 3, BLOCK_WARP);       //左下ワープホール
	SetBlock(D3DXVECTOR3(80.0f, 620.0f, 0.0f), 20.0f, 80.0f, D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0, BLOCK_NORMAL);	  //キャリーブロックのストッパー
	SetBlock(D3DXVECTOR3(80.0f, 620.0f, 0.0f), 80.0f, 80.0f, D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0, BLOCK_CARRY);      //左下キャリーブロック
	SetBlock(D3DXVECTOR3(180.0f, 580.0f, 0.0f), 300.0f, 20.0f, D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0, BLOCK_NORMAL);   //左下横長
	SetBlock(D3DXVECTOR3(480.0f, 480.0f, 0.0f), 20.0f, 60.0f, D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0, BLOCK_NORMAL);    //真ん中下左入口と出口の区切り
	SetBlock(D3DXVECTOR3(620.0f, 480.0f, 0.0f), 20.0f, 220.0f, D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0, BLOCK_NORMAL);   //真ん中区切り
	SetBlock(D3DXVECTOR3(520.0f, 500.0f, 0.0f), 80.0f, 20.0f, D3DXVECTOR3(0.0f, 640.0f, 0.0f), 0, BLOCK_DOWN);    //真ん中下左側入口落ちるブロック
	SetBlock(D3DXVECTOR3(720.0f, 520.0f, 0.0f), 20.0f, 100.0f, D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0, BLOCK_NORMAL);   //真ん中下右縦左側
	SetBlock(D3DXVECTOR3(740.0f, 520.0f, 0.0f), 220.0f, 20.0f, D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0, BLOCK_NORMAL);   //真ん中下右横
	SetBlock(D3DXVECTOR3(940.0f, 540.0f, 0.0f), 20.0f, 80.0f, D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0, BLOCK_NORMAL);    //真ん中下右縦右側
	SetBlock(D3DXVECTOR3(860.0f, 620.0f, 0.0f), 20.0f, 80.0f, D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0, BLOCK_NORMAL);    //右下キャリーブロックを納める場所
	SetBlock(D3DXVECTOR3(780.0f, 620.0f, 0.0f), 20.0f, 80.0f, D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0, BLOCK_NORMAL);    //右下キャリーブロックのストッパー
	SetBlock(D3DXVECTOR3(720.0f, 620.0f, 0.0f), 80.0f, 80.0f, D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0, BLOCK_CARRY);     //右下キャリーブロック
	SetBlock(D3DXVECTOR3(960.0f, 520.0f, 0.0f), 80.0f, 20.0f, D3DXVECTOR3(0.0f, 320.0f, 0.0f), 0, BLOCK_UP);      //スタート地点すぐ右上がるブロック
	SetBlock(D3DXVECTOR3(420.0f, 240.0f, 0.0f), 500.0f, 20.0f, D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0, BLOCK_NORMAL);   //スタート地点の上の下側横長ブロック
	SetBlock(D3DXVECTOR3(1060.0f, 140.0f, 0.0f), 20.0f, 100.0f, D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0, BLOCK_NORMAL);  //スタート地点の上の縦右側
	SetBlock(D3DXVECTOR3(320.0f, 140.0f, 0.0f), 20.0f, 120.0f, D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0, BLOCK_NORMAL);   //スタート地点の上の縦左側
	SetBlock(D3DXVECTOR3(340.0f, 250.0f, 0.0f), 80.0f, 20.0f, D3DXVECTOR3(0.0f, 400.0f, 0.0f), 0, BLOCK_DOWN);    //真ん中下左側入口落ちるブロック
}