//--------------------------
//Author:三上航世
//ゲーム画面の処理(game.cpp)
//--------------------------
#include "input.h"
#include "gamepad.h"
#include "game.h"
#include "bullet.h"
#include "enemy.h"
#include "explosion.h"
#include "ui.h"
#include "fade.h"
#include "rockon.h"
#include "shadow.h"
#include "light.h"
#include "wall.h"
#include "billboard.h"
#include "meshfield.h"
#include "particle.h"
#include "player.h"
#include "object.h"
#include "orbit.h"
#include "camera.h"
#include "sound.h"
#include "pauseui.h"

//グローバル宣言
D3DXVECTOR3 g_resporn[4];
bool g_bStop;

//初期化処理
void InitGame(void)
{
	ResetCamera();
	ResetShadow();
	InitBullet();
	InitEnemy();
	InitExplosion();
	InitUi();
	InitRockon();
	InitShadow();
	InitLight();
	InitWall();
	InitBillboard();
	InitMeshfield();
	InitParticle();
	InitPlayer();
	InitObject();
	InitOrbit();
	InitpauseUi();
	g_bStop = false;
	g_resporn[0] = D3DXVECTOR3(0.0f, 0.0f, 1200.0f);
	g_resporn[1] = D3DXVECTOR3(1200.0f, 0.0f, 0.0f);
	g_resporn[2] = D3DXVECTOR3(0.0f, 0.0f, -1200.0f);
	g_resporn[3] = D3DXVECTOR3(-1200.0f, 0.0f, 0.0f);
	PlaySound(SOUND_LABEL_BGM2);
}

//ゲーム画面の終了処理
void UninitGame(void)
{
	UninitLight();
	UninitShadow();
	UninitWall();
	UninitBillboard();
	UninitMeshfield();
	UninitBullet();
	UninitExplosion();
	UninitParticle();
	UninitPlayer();
	UninitObject();
	UninitEnemy();
	UninitUi();
	UninitOrbit();
	UninitRockon();
	UninitpauseUi();
}

//ゲーム画面の更新処理
void UpdateGame(void)
{
	//ポーズ
	if (GetKeyboardKey(DIK_O) == true || GetGamePadButton(DIP_START) == true)
	{
		if (g_bStop == true)
		{
			g_bStop = false;
		}
		else
		{
			Setpause();
			g_bStop = true;
		}
	}

	if (g_bStop == false)
	{
		UpdateLight();
		UpdateShadow();
		UpdateWall();
		UpdateBillboard();
		UpdateMeshfield();
		UpdateBullet();
		UpdateExplosion();
		UpdateParticle();
		UpdateObject();
		UpdateEnemy();
		UpdatePlayer();
		UpdateUi();
		UpdateOrbit();
		UpdateRockon();
	}
	else
	{
		UpdatepauseUi();
	}

	//プレイヤーの倒した数が100未満かつ敵の数が500未満
	if (PlayerKill() + EnemyNum() < 100 && EnemyNum() < 50)
	{
		SetEnemy(g_resporn[rand() % 4], rand() % 2);
	}

	//プレイヤーが100体倒す、またはプレイヤーのHPが0になる
	if (PlayerKill() >= 100 || PlayerHP() <= 0)
	{
		SetFade(MODE_RESULT);
	}
}

//描画処理
void DrawGame(void)
{
	DrawWall();
	DrawMeshfield();
	DrawShadow();
	DrawExplosion();
	DrawBullet();
	DrawParticle();
	DrawPlayer();
	DrawEnemy();
	DrawOrbit();
	DrawObject();
	DrawRockon();
	if (g_bStop == true)
	{
		DrawpauseUi();
	}
}