//--------------------------
//Author:éOè„çqê¢
//ÉQÅ[ÉÄâÊñ ÇÃèàóù(game.cpp)
//--------------------------
#include "input.h"
#include "game.h"
#include "bg.h"
#include "bullet.h"
#include "enemy.h"
#include "explosion.h"
#include "aircraft.h"
#include "ui.h"
#include "sound.h"
#include "laser.h"
#include "lockon.h"
#include "missile.h"
#include "smoke.h"
#include "uav.h"
#include "barrier.h"
#include "fade.h"
#include "prize.h"
#include "pauseui.h"
#include "death.h"
#include "boss.h"

//ÉOÉçÅ[ÉoÉãïœêî
int g_nTime;
int g_nGameStage;
bool g_bStop;

//èâä˙âªèàóù
void InitGame(void)
{
	InitBullet();
	InitBarrier();
	InitEnemy();
	InitExplosion();
	InitAircraft();
	InitUi();
	InitLaser();
	InitLockon();
	InitMissile();
	InitSmoke();
	InitUav();
	InitPrize();
	InitpauseUi();
	InitDeath();
	InitBoss();
	g_nTime = 0;
	g_nGameStage = 0;
	g_bStop = false;
	SetBg(1,true);
	SetAircraft();
}

//ÉQÅ[ÉÄâÊñ ÇÃèIóπèàóù
void UninitGame(void)
{
	UninitBullet();
	UninitEnemy();
	UninitExplosion();
	UninitAircraft();
	UninitUi();
	UninitLaser();
	UninitLockon();
	UninitMissile();
	UninitSmoke();
	UninitUav();
	UninitBarrier();
	UninitPrize();
	UninitpauseUi();
	UninitDeath();
	UninitBoss();
}

//ÉQÅ[ÉÄâÊñ ÇÃçXêVèàóù
void UpdateGame(void)
{
	if (GetKeyboardTrigger(DIK_P) == true && GetKeyboardRelease(DIK_P) == false)
	{
		if (g_bStop == false)
		{
			PlaySound(SOUND_LABEL_PAUSE000);
			g_bStop = true;
		}
		else
		{
			PlaySound(SOUND_LABEL_CANCEL000);
			g_bStop = false;
		}
	}
	if(g_bStop == false)
	{
		g_nTime++;
		ScrollBg(true);
		UpdateBullet();
		UpdateEnemy();
		UpdateExplosion();
		UpdateAircraft();
		Aircraft *pAircraft;
		pAircraft = GetAircraft();
		UpdateUi();
		UpdateLaser();
		UpdateLockon();
		UpdateMissile();
		UpdateSmoke();
		UpdateUav(pAircraft->pos, pAircraft->nLag);
		UpdateBarrier();
		UpdatePrize();
		UpdateDeath();
		UpdateBoss();
		if (g_nTime > 9010)
		{
			g_nTime = 9010;
		}
		switch (g_nGameStage)
		{
		case 0:
			switch (g_nTime)
			{
			case 10:
				PlaySound(SOUND_LABEL_BGM004);
				break;
			case 100:
				SetEnemy(D3DXVECTOR3(100.0f, -40.0f, 0.0f), ENEMY_XU_17, D3DXVECTOR3(0.0f, 0.5f, 0.0f), false, WAY_DOWN);
				break;
			case 200:
				SetEnemy(D3DXVECTOR3(800.0f, -40.0f, 0.0f), ENEMY_XU_57, D3DXVECTOR3(-0.2f, 0.8f, 0.0f), false, WAY_DOWN);
				break;
			case 450:
				SetEnemy(D3DXVECTOR3(200.0f, -40.0f, 0.0f), ENEMY_XU_17, D3DXVECTOR3(0.4f, 2.0f, 0.0f), false, WAY_DOWN);
				SetEnemy(D3DXVECTOR3(1080.0f, -40.0f, 0.0f), ENEMY_XU_17, D3DXVECTOR3(-0.4f, 2.0f, 0.0f), false, WAY_DOWN);
				break;
			case 600:
				SetEnemy(D3DXVECTOR3(400.0f, -40.0f, 0.0f), ENEMY_XU_37, D3DXVECTOR3(0.5f, 1.0f, 0.0f), false, WAY_DOWN);
				break;
			case 720:
				SetEnemy(D3DXVECTOR3(100.0f, 760.0f, 0.0f), ENEMY_XU_57, D3DXVECTOR3(0.8f, -3.0f, 0.0f), true, WAY_UP);
				break;
			case 800:
				SetEnemy(D3DXVECTOR3(-40.0f, 300.0f, 0.0f), ENEMY_XU_5, D3DXVECTOR3(0.8f, 0.1f, 0.0f), false, WAY_RIGHT);
				SetEnemy(D3DXVECTOR3(1320.0f, 300.0f, 0.0f), ENEMY_XU_5, D3DXVECTOR3(-0.8f, 0.1f, 0.0f), false, WAY_LEFT);
				break;
			case 950:
				SetEnemy(D3DXVECTOR3(70.0f, -40.0f, 0.0f), ENEMY_XU_17, D3DXVECTOR3(0.2f, 1.2f, 0.0f), false, WAY_DOWN);
				break;
			case 1020:
				SetEnemy(D3DXVECTOR3(70.0f, -40.0f, 0.0f), ENEMY_XU_5, D3DXVECTOR3(0.2f, 1.2f, 0.0f), false, WAY_DOWN);
				break;
			case 1090:
				SetEnemy(D3DXVECTOR3(70.0f, -40.0f, 0.0f), ENEMY_XU_37, D3DXVECTOR3(0.2f, 1.2f, 0.0f), false, WAY_DOWN);
				break;
			case 1170:
				SetEnemy(D3DXVECTOR3(70.0f, -40.0f, 0.0f), ENEMY_XU_57, D3DXVECTOR3(0.2f, 1.2f, 0.0f), false, WAY_DOWN);
				break;
			case 1260:
				SetEnemy(D3DXVECTOR3(1070.0f, -40.0f, 0.0f), ENEMY_XU_17, D3DXVECTOR3(-0.2f, 1.2f, 0.0f), false, WAY_DOWN);
				break;
			case 1350:
				SetEnemy(D3DXVECTOR3(1070.0f, -40.0f, 0.0f), ENEMY_XU_5, D3DXVECTOR3(-0.2f, 1.2f, 0.0f), false, WAY_DOWN);
				break;
			case 1440:
				SetEnemy(D3DXVECTOR3(1070.0f, -40.0f, 0.0f), ENEMY_XU_37, D3DXVECTOR3(-0.2f, 1.2f, 0.0f), false, WAY_DOWN);
				break;
			case 1530:
				SetEnemy(D3DXVECTOR3(1070.0f, -40.0f, 0.0f), ENEMY_XU_57, D3DXVECTOR3(-0.2f, 1.2f, 0.0f), false, WAY_DOWN);
				break;
			case 1880:
				SetEnemy(D3DXVECTOR3(200.0f, 760.0f, 0.0f), ENEMY_XU_17, D3DXVECTOR3(0.2f, -3.0f, 0.0f), false, WAY_UP);
				break;
			case 2000:
				SetEnemy(D3DXVECTOR3(200.0f, -40.0f, 0.0f), ENEMY_XU_17, D3DXVECTOR3(0.2f, 1.5f, 0.0f), false, WAY_DOWN);
				SetEnemy(D3DXVECTOR3(1000.0f, -40.0f, 0.0f), ENEMY_XU_5, D3DXVECTOR3(-0.2f, 1.5f, 0.0f), false, WAY_DOWN);
				break;
			case 2140:
				SetEnemy(D3DXVECTOR3(1320.0f, 300.0f, 0.0f), ENEMY_XU_5, D3DXVECTOR3(2.0f, 0.8f, 0.0f), false, WAY_LEFT);
				break;
			case 2300:
				SetEnemy(D3DXVECTOR3(900.0f, -40.0f, 0.0f), ENEMY_XU_57, D3DXVECTOR3(0.2f, 2.0f, 0.0f), false, WAY_DOWN);
				break;
			case 2450:
				SetEnemy(D3DXVECTOR3(0.0f, -40.0f, 0.0f), ENEMY_XU_17, D3DXVECTOR3(0.1f, 1.0f, 0.0f), false, WAY_DOWN);
				break;
			case 2700:
				SetEnemy(D3DXVECTOR3(-40.0f, 40.0f, 0.0f), ENEMY_XU_57, D3DXVECTOR3(2.0f, 0.4f, 0.0f), false, WAY_RIGHT);
				break;
			case 2800:
				SetEnemy(D3DXVECTOR3(-40.0f, 40.0f, 0.0f), ENEMY_XU_37, D3DXVECTOR3(2.0f, 0.4f, 0.0f), false, WAY_RIGHT);
				break;
			case 2900:
				SetEnemy(D3DXVECTOR3(-40.0f, 40.0f, 0.0f), ENEMY_XU_5, D3DXVECTOR3(2.0f, 0.4f, 0.0f), false, WAY_RIGHT);
				break;
			case 3000:
				SetEnemy(D3DXVECTOR3(-40.0f, 40.0f, 0.0f), ENEMY_XU_17, D3DXVECTOR3(2.0f, 0.4f, 0.0f), true, WAY_RIGHT);
				break;
			case 3500:
				SetEnemy(D3DXVECTOR3(600.0f, -40.0f, 0.0f), ENEMY_XU_5, D3DXVECTOR3(0.0f, 4.0f, 0.0f), false, WAY_DOWN);
				break;
			case 3650:
				SetEnemy(D3DXVECTOR3(800.0f, 760.0f, 0.0f), ENEMY_XU_17, D3DXVECTOR3(0.0f, -2.5f, 0.0f), false, WAY_UP);
				break;
			case 3800:
				SetEnemy(D3DXVECTOR3(600.0f, -40.0f, 0.0f), ENEMY_XU_37, D3DXVECTOR3(0.3f, 1.0f, 0.0f), false, WAY_DOWN);
				break;
			case 3940:
				SetEnemy(D3DXVECTOR3(800.0f, -40.0f, 0.0f), ENEMY_XU_57, D3DXVECTOR3(0.0f, 1.5f, 0.0f), false, WAY_DOWN);
				break;
			case 4050:
				SetEnemy(D3DXVECTOR3(1320.0f, 460.0f, 0.0f), ENEMY_XU_5, D3DXVECTOR3(-3.0f, 0.2f, 0.0f), false, WAY_LEFT);
				break;
			case 4200:
				SetEnemy(D3DXVECTOR3(-40.0f, 330.0f, 0.0f), ENEMY_XU_5, D3DXVECTOR3(3.0f, 0.2f, 0.0f), false, WAY_RIGHT);
				break;
			case 4340:
				SetEnemy(D3DXVECTOR3(30.0f, -40.0f, 0.0f), ENEMY_XU_5, D3DXVECTOR3(0.0f, 2.0f, 0.0f), false, WAY_DOWN);
				break;
			case 4500:
				SetEnemy(D3DXVECTOR3(1250.0f, -40.0f, 0.0f), ENEMY_XU_5, D3DXVECTOR3(0.2f, 2.0f, 0.0f), false, WAY_DOWN);
				break;
			case 4550:
				SetEnemy(D3DXVECTOR3(660.0f, -40.0f, 0.0f), ENEMY_XU_5, D3DXVECTOR3(0.3f, 2.0f, 0.0f), false, WAY_DOWN);
				break;
			case 4800:
				SetEnemy(D3DXVECTOR3(1100.0f, -40.0f, 0.0f), ENEMY_XU_5, D3DXVECTOR3(0.2f, 3.0f, 0.0f), false, WAY_DOWN);
				break;
			case 4900:
				SetEnemy(D3DXVECTOR3(1100.0f, -40.0f, 0.0f), ENEMY_XU_5, D3DXVECTOR3(0.2f, 3.0f, 0.0f), false, WAY_DOWN);
				break;
			case 5000:
				SetEnemy(D3DXVECTOR3(1100.0f, -40.0f, 0.0f), ENEMY_XU_5, D3DXVECTOR3(0.2f, 3.0f, 0.0f), true, WAY_DOWN);
				break;
			case 5100:
				SetEnemy(D3DXVECTOR3(1100.0f, -40.0f, 0.0f), ENEMY_XU_5, D3DXVECTOR3(0.2f, 3.0f, 0.0f), false, WAY_DOWN);
				break;
			case 5200:
				SetEnemy(D3DXVECTOR3(1100.0f, -40.0f, 0.0f), ENEMY_XU_5, D3DXVECTOR3(0.2f, 3.0f, 0.0f), false, WAY_DOWN);
				break;
			case 5500:
				SetEnemy(D3DXVECTOR3(400.0f, -40.0f, 0.0f), ENEMY_XU_17, D3DXVECTOR3(0.3f, 1.0f, 0.0f), false, WAY_DOWN);
				break;
			case 5620:
				SetEnemy(D3DXVECTOR3(800.0f, -40.0f, 0.0f), ENEMY_XU_37, D3DXVECTOR3(-0.7f, 1.0f, 0.0f), false, WAY_DOWN);
				break;
			case 5770:
				SetEnemy(D3DXVECTOR3(-40.0f, 100.0f, 0.0f), ENEMY_XU_57, D3DXVECTOR3(1.5f, 0.3f, 0.0f), false, WAY_RIGHT);
				break;
			case 5900:
				SetEnemy(D3DXVECTOR3(-40.0f, 100.0f, 0.0f), ENEMY_XU_57, D3DXVECTOR3(1.5f, 0.3f, 0.0f), false, WAY_RIGHT);
				break;
			case 6050:
				SetEnemy(D3DXVECTOR3(300.0f, -40.0f, 0.0f), ENEMY_XU_37, D3DXVECTOR3(0.4f, 1.2f, 0.0f), false, WAY_DOWN);
				break;
			case 6150:
				SetEnemy(D3DXVECTOR3(900.0f, -40.0f, 0.0f), ENEMY_XU_37, D3DXVECTOR3(-0.4f, 1.2f, 0.0f), false, WAY_DOWN);
				break;
			case 6250:
				SetEnemy(D3DXVECTOR3(300.0f, -40.0f, 0.0f), ENEMY_XU_37, D3DXVECTOR3(0.4f, 1.2f, 0.0f), false, WAY_DOWN);
				break;
			case 6350:
				SetEnemy(D3DXVECTOR3(900.0f, -40.0f, 0.0f), ENEMY_XU_37, D3DXVECTOR3(-0.4f, 1.2f, 0.0f), false, WAY_DOWN);
				break;
			case 6450:
				SetEnemy(D3DXVECTOR3(300.0f, -40.0f, 0.0f), ENEMY_XU_37, D3DXVECTOR3(0.4f, 1.2f, 0.0f), false, WAY_DOWN);
				break;
			case 6550:
				SetEnemy(D3DXVECTOR3(900.0f, -40.0f, 0.0f), ENEMY_XU_37, D3DXVECTOR3(-0.4f, 1.2f, 0.0f), true, WAY_DOWN);
				break;
			case 6800:
				SetEnemy(D3DXVECTOR3(700.0f, -40.0f, 0.0f), ENEMY_XU_5, D3DXVECTOR3(0.0f, 3.0f, 0.0f), false, WAY_DOWN);
				break;
			case 7000:
				SetEnemy(D3DXVECTOR3(1280.0f, 760.0f, 0.0f), ENEMY_XU_17, D3DXVECTOR3(-0.5f, -3.0f, 0.0f), false, WAY_UP);
				break;
			case 7100:
				SetEnemy(D3DXVECTOR3(0.0f, 760.0f, 0.0f), ENEMY_XU_17, D3DXVECTOR3(0.5f, -3.0f, 0.0f), false, WAY_UP);
				break;
			case 7200:
				SetEnemy(D3DXVECTOR3(1280.0f, 760.0f, 0.0f), ENEMY_XU_5, D3DXVECTOR3(-0.5f, -3.0f, 0.0f), false, WAY_UP);
				break;
			case 7300:
				SetEnemy(D3DXVECTOR3(0.0f, 760.0f, 0.0f), ENEMY_XU_5, D3DXVECTOR3(0.5f, -3.0f, 0.0f), false, WAY_UP);
				break;
			case 7600:
				SetEnemy(D3DXVECTOR3(-40.0f, 60.0f, 0.0f), ENEMY_XU_57, D3DXVECTOR3(0.6f, 2.0f, 0.0f), false, WAY_DOWN);
				break;
			case 7750:
				SetEnemy(D3DXVECTOR3(1340.0f, 100.0f, 0.0f), ENEMY_XU_37, D3DXVECTOR3(-0.6f, 2.4f, 0.0f), false, WAY_DOWN);
				break;
			case 7900:
				SetEnemy(D3DXVECTOR3(-40.0f, 160.0f, 0.0f), ENEMY_XU_5, D3DXVECTOR3(1.8f, 0.5f, 0.0f), false, WAY_RIGHT);
				break;
			case 8000:
				SetEnemy(D3DXVECTOR3(540.0f, -40.0f, 0.0f), ENEMY_XU_37, D3DXVECTOR3(0.2f, 2.4f, 0.0f), false, WAY_DOWN);
				break;
			case 8100:
				SetEnemy(D3DXVECTOR3(740.0f, -40.0f, 0.0f), ENEMY_XU_57, D3DXVECTOR3(-0.2f, 2.4f, 0.0f), false, WAY_DOWN);
				break;
			case 8200:
				SetEnemy(D3DXVECTOR3(540.0f, -40.0f, 0.0f), ENEMY_XU_5, D3DXVECTOR3(0.2f, 2.4f, 0.0f), false, WAY_DOWN);
				break;
			case 8300:
				SetEnemy(D3DXVECTOR3(740.0f, -40.0f, 0.0f), ENEMY_XU_17, D3DXVECTOR3(-0.2f, 2.4f, 0.0f), true, WAY_DOWN);
				break;
			case 9000:
				StopSound(SOUND_LABEL_BGM004);
				SetBoss(D3DXVECTOR3(640.0f, -140.0f, 0.0f), BOSS_XU_44);
				PlaySound(SOUND_LABEL_BGM001);
				break;
			}
			break;
		case 1:
			switch (g_nTime)
			{
			case 10:
				PlaySound(SOUND_LABEL_BGM005);
				break;
			case 100:
				SetEnemy(D3DXVECTOR3(740.0f, -40.0f, 0.0f), ENEMY_XU_17, D3DXVECTOR3(-0.2f, 1.8f, 0.0f), false, WAY_DOWN);
				break;
			case 280:
				SetEnemy(D3DXVECTOR3(1130.0f, 760.0f, 0.0f), ENEMY_XU_5, D3DXVECTOR3(0.0f, -1.8f, 0.0f), false, WAY_UP);
				break;
			case 380:
				SetEnemy(D3DXVECTOR3(150.0f, 760.0f, 0.0f), ENEMY_XU_17, D3DXVECTOR3(0.0f, -1.8f, 0.0f), false, WAY_UP);
				break;
			case 480:
				SetEnemy(D3DXVECTOR3(1130.0f, 760.0f, 0.0f), ENEMY_XU_5, D3DXVECTOR3(0.0f, -1.8f, 0.0f), false, WAY_UP);
				break;
			case 580:
				SetEnemy(D3DXVECTOR3(150.0f, 760.0f, 0.0f), ENEMY_XU_17, D3DXVECTOR3(0.0f, -1.8f, 0.0f), false, WAY_UP);
				break;
			case 780:
				SetEnemy(D3DXVECTOR3(900.0f, -40.0f, 0.0f), ENEMY_XU_37, D3DXVECTOR3(-0.4f, 2.0f, 0.0f), false, WAY_DOWN);
				break;
			case 950:
				SetEnemy(D3DXVECTOR3(250.0f, -40.0f, 0.0f), ENEMY_XU_37, D3DXVECTOR3(0.3f, 2.4f, 0.0f), false, WAY_DOWN);
				break;
			case 1100:
				SetEnemy(D3DXVECTOR3(1320.0f, 80.0f, 0.0f), ENEMY_XU_57, D3DXVECTOR3(2.2f, 0.3f, 0.0f), false, WAY_LEFT);
				break;
			case 1200:
				SetEnemy(D3DXVECTOR3(1320.0f, 80.0f, 0.0f), ENEMY_XU_57, D3DXVECTOR3(2.2f, 0.3f, 0.0f), false, WAY_LEFT);
				break;
			case 1300:
				SetEnemy(D3DXVECTOR3(1320.0f, 80.0f, 0.0f), ENEMY_XU_57, D3DXVECTOR3(2.2f, 0.3f, 0.0f), false, WAY_LEFT);
				break;
			case 1400:
				SetEnemy(D3DXVECTOR3(1320.0f, 80.0f, 0.0f), ENEMY_XU_57, D3DXVECTOR3(2.2f, 0.3f, 0.0f), true, WAY_LEFT);
				break;
			case 1700:
				SetEnemy(D3DXVECTOR3(-40.0f, 440.0f, 0.0f), ENEMY_XU_5, D3DXVECTOR3(2.6f, 0.3f, 0.0f), false, WAY_RIGHT);
				break;
			case 1800:
				SetEnemy(D3DXVECTOR3(-40.0f, 440.0f, 0.0f), ENEMY_XU_5, D3DXVECTOR3(2.6f, 0.3f, 0.0f), false, WAY_RIGHT);
				break;
			case 1900:
				SetEnemy(D3DXVECTOR3(-40.0f, 440.0f, 0.0f), ENEMY_XU_5, D3DXVECTOR3(2.6f, 0.3f, 0.0f), false, WAY_RIGHT);
				break;
			case 2000:
				SetEnemy(D3DXVECTOR3(-40.0f, 440.0f, 0.0f), ENEMY_XU_5, D3DXVECTOR3(2.6f, 0.3f, 0.0f), false, WAY_RIGHT);
				break;
			case 2500:
				SetEnemy(D3DXVECTOR3(-40.0f, 240.0f, 0.0f), ENEMY_XU_17, D3DXVECTOR3(1.6f, -0.1f, 0.0f), false, WAY_RIGHT);
				break;
			case 2650:
				SetEnemy(D3DXVECTOR3(1040.0f, -40.0f, 0.0f), ENEMY_XU_37, D3DXVECTOR3(-0.2f, 2.5f, 0.0f), false, WAY_DOWN);
				break;
			case 2800:
				SetEnemy(D3DXVECTOR3(310.0f, -40.0f, 0.0f), ENEMY_XU_37, D3DXVECTOR3(0.2f, 1.8f, 0.0f), false, WAY_DOWN);
				break;
			case 3000:
				SetEnemy(D3DXVECTOR3(640.0f, -40.0f, 0.0f), ENEMY_XU_5, D3DXVECTOR3(0.0f, 3.0f, 0.0f), true, WAY_DOWN);
				break;
			case 3200:
				SetEnemy(D3DXVECTOR3(120.0f, 760.0f, 0.0f), ENEMY_XU_17, D3DXVECTOR3(0.2f, 2.0f, 0.0f), false, WAY_UP);
				break;
			case 3300:
				SetEnemy(D3DXVECTOR3(220.0f, -40.0f, 0.0f), ENEMY_XU_37, D3DXVECTOR3(0.2f, -2.0f, 0.0f), false, WAY_DOWN);
				break;
			case 3400:
				SetEnemy(D3DXVECTOR3(320.0f, 760.0f, 0.0f), ENEMY_XU_57, D3DXVECTOR3(0.2f, 2.0f, 0.0f), false, WAY_UP);
				break;
			case 3500:
				SetEnemy(D3DXVECTOR3(420.0f, -40.0f, 0.0f), ENEMY_XU_37, D3DXVECTOR3(0.2f, -2.0f, 0.0f), false, WAY_DOWN);
				break;
			case 3600:
				SetEnemy(D3DXVECTOR3(520.0f, 760.0f, 0.0f), ENEMY_XU_17, D3DXVECTOR3(0.2f, 2.0f, 0.0f), false, WAY_UP);
				break;
			case 3700:
				SetEnemy(D3DXVECTOR3(620.0f, -40.0f, 0.0f), ENEMY_XU_5, D3DXVECTOR3(0.2f, -2.0f, 0.0f), false, WAY_DOWN);
				break;
			case 4000:
				SetEnemy(D3DXVECTOR3(70.0f, -40.0f, 0.0f), ENEMY_XU_5, D3DXVECTOR3(0.5f, 2.2f, 0.0f), false, WAY_DOWN);
				break;
			case 4150:
				SetEnemy(D3DXVECTOR3(1200.0f, -40.0f, 0.0f), ENEMY_XU_5, D3DXVECTOR3(-0.3f, 1.5f, 0.0f), false, WAY_DOWN);
				break;
			case 4300:
				SetEnemy(D3DXVECTOR3(360.0f, -40.0f, 0.0f), ENEMY_XU_57, D3DXVECTOR3(0.2f, 3.0f, 0.0f), true, WAY_DOWN);
				break;
			case 4500:
				SetEnemy(D3DXVECTOR3(740.0f, -40.0f, 0.0f), ENEMY_XU_57, D3DXVECTOR3(-0.2f, 1.8f, 0.0f), false, WAY_DOWN);
				break;
			case 4700:
				SetEnemy(D3DXVECTOR3(1140.0f, -40.0f, 0.0f), ENEMY_XU_37, D3DXVECTOR3(-0.2f, 1.8f, 0.0f), false, WAY_DOWN);
				break;
			case 4900:
				SetEnemy(D3DXVECTOR3(140.0f, -40.0f, 0.0f), ENEMY_XU_37, D3DXVECTOR3(-0.2f, 1.2f, 0.0f), false, WAY_DOWN);
				break;
			case 5200:
				SetEnemy(D3DXVECTOR3(640.0f, -40.0f, 0.0f), ENEMY_XU_5, D3DXVECTOR3(0.0f, 2.5f, 0.0f), false, WAY_DOWN);
				break;
			case 5300:
				SetEnemy(D3DXVECTOR3(740.0f, -40.0f, 0.0f), ENEMY_XU_5, D3DXVECTOR3(0.0f, 2.5f, 0.0f), false, WAY_DOWN);
				break;
			case 5400:
				SetEnemy(D3DXVECTOR3(840.0f, -40.0f, 0.0f), ENEMY_XU_5, D3DXVECTOR3(0.0f, 2.5f, 0.0f), false, WAY_DOWN);
				break;
			case 5500:
				SetEnemy(D3DXVECTOR3(940.0f, -40.0f, 0.0f), ENEMY_XU_5, D3DXVECTOR3(0.0f, 2.5f, 0.0f), true, WAY_DOWN);
				break;
			case 5600:
				SetEnemy(D3DXVECTOR3(1040.0f, 760.0f, 0.0f), ENEMY_XU_57, D3DXVECTOR3(0.0f, -2.5f, 0.0f), false, WAY_UP);
				break;
			case 5700:
				SetEnemy(D3DXVECTOR3(1140.0f, 760.0f, 0.0f), ENEMY_XU_57, D3DXVECTOR3(0.0f, -2.5f, 0.0f), false, WAY_UP);
				break;
			case 5800:
				SetEnemy(D3DXVECTOR3(1240.0f, 760.0f, 0.0f), ENEMY_XU_57, D3DXVECTOR3(0.0f, -2.5f, 0.0f), false, WAY_UP);
				break;
			case 6100:
				SetEnemy(D3DXVECTOR3(-40.0f, 240.0f, 0.0f), ENEMY_XU_57, D3DXVECTOR3(2.3f, -0.2f, 0.0f), false, WAY_RIGHT);
				break;
			case 6200:
				SetEnemy(D3DXVECTOR3(1320.0f, 240.0f, 0.0f), ENEMY_XU_17, D3DXVECTOR3(2.3f, 0.2f, 0.0f), false, WAY_LEFT);
				break;
			case 6350:
				SetEnemy(D3DXVECTOR3(660.0f, -40.0f, 0.0f), ENEMY_XU_37, D3DXVECTOR3(0.2f, 2.1f, 0.0f), false, WAY_DOWN);
				break;
			case 6500:
				SetEnemy(D3DXVECTOR3(230.0f, -40.0f, 0.0f), ENEMY_XU_17, D3DXVECTOR3(-0.2f, 2.0f, 0.0f), false, WAY_DOWN);
				break;
			case 6650:
				SetEnemy(D3DXVECTOR3(890.0f, 760.0f, 0.0f), ENEMY_XU_57, D3DXVECTOR3(-0.4f, -1.8f, 0.0f), false, WAY_UP);
				break;
			case 6800:
				SetEnemy(D3DXVECTOR3(1090.0f, -40.0f, 0.0f), ENEMY_XU_17, D3DXVECTOR3(-0.2f, 1.6f, 0.0f), false, WAY_DOWN);
				break;
			case 7000:
				SetEnemy(D3DXVECTOR3(1090.0f, 760.0f, 0.0f), ENEMY_XU_37, D3DXVECTOR3(-0.3f, -2.0f, 0.0f), false, WAY_UP);
				break;
			case 7100:
				SetEnemy(D3DXVECTOR3(190.0f, 760.0f, 0.0f), ENEMY_XU_37, D3DXVECTOR3(0.3f, -2.0f, 0.0f), false, WAY_UP);
				break;
			case 7200:
				SetEnemy(D3DXVECTOR3(1090.0f, 760.0f, 0.0f), ENEMY_XU_37, D3DXVECTOR3(-0.3f, -2.0f, 0.0f), false, WAY_UP);
				break;
			case 7300:
				SetEnemy(D3DXVECTOR3(190.0f, 760.0f, 0.0f), ENEMY_XU_37, D3DXVECTOR3(0.3f, -2.0f, 0.0f), true, WAY_UP);
				break;
			case 7400:
				SetEnemy(D3DXVECTOR3(1090.0f, 760.0f, 0.0f), ENEMY_XU_37, D3DXVECTOR3(-0.3f, -2.0f, 0.0f), false, WAY_UP);
				break;
			case 7500:
				SetEnemy(D3DXVECTOR3(190.0f, 760.0f, 0.0f), ENEMY_XU_37, D3DXVECTOR3(0.3f, -2.0f, 0.0f), false, WAY_UP);
				break;
			case 8000:
				SetEnemy(D3DXVECTOR3(740.0f, -40.0f, 0.0f), ENEMY_XU_5, D3DXVECTOR3(0.1f, 2.0f, 0.0f), false, WAY_DOWN);
				SetEnemy(D3DXVECTOR3(540.0f, -40.0f, 0.0f), ENEMY_XU_5, D3DXVECTOR3(-0.1f, 2.0f, 0.0f), false, WAY_DOWN);
				break;
			case 8150:
				SetEnemy(D3DXVECTOR3(840.0f, -40.0f, 0.0f), ENEMY_XU_17, D3DXVECTOR3(0.1f, 1.8f, 0.0f), false, WAY_DOWN);
				SetEnemy(D3DXVECTOR3(440.0f, -40.0f, 0.0f), ENEMY_XU_17, D3DXVECTOR3(-0.1f, 1.8f, 0.0f), false, WAY_DOWN);
				break;
			case 8300:
				SetEnemy(D3DXVECTOR3(940.0f, -40.0f, 0.0f), ENEMY_XU_37, D3DXVECTOR3(0.1f, 1.6f, 0.0f), false, WAY_DOWN);
				SetEnemy(D3DXVECTOR3(340.0f, -40.0f, 0.0f), ENEMY_XU_37, D3DXVECTOR3(-0.1f, 1.6f, 0.0f), false, WAY_DOWN);
				break;
			case 8450:
				SetEnemy(D3DXVECTOR3(1040.0f, -40.0f, 0.0f), ENEMY_XU_57, D3DXVECTOR3(0.1f, 1.4f, 0.0f), false, WAY_DOWN);
				SetEnemy(D3DXVECTOR3(240.0f, -40.0f, 0.0f), ENEMY_XU_57, D3DXVECTOR3(-0.1f, 1.4f, 0.0f), true, WAY_DOWN);
				break;
			case 9000:
				StopSound(SOUND_LABEL_BGM005);
				SetBoss(D3DXVECTOR3(640.0f, -240.0f, 0.0f), BOSS_SEAGULL);
				PlaySound(SOUND_LABEL_BGM002);
				break;
			}
			break;
		case 2:
			switch (g_nTime)
			{
			case 10:
				PlaySound(SOUND_LABEL_BGM006);
				break;
			case 100:
				SetEnemy(D3DXVECTOR3(1040.0f, -40.0f, 0.0f), ENEMY_XU_57, D3DXVECTOR3(0.3f, 1.6f, 0.0f), false, WAY_DOWN);
				break;
			case 250:
				SetEnemy(D3DXVECTOR3(1040.0f, -40.0f, 0.0f), ENEMY_XU_37, D3DXVECTOR3(0.3f, 1.6f, 0.0f), false, WAY_DOWN);
				break;
			case 400:
				SetEnemy(D3DXVECTOR3(1040.0f, -40.0f, 0.0f), ENEMY_XU_17, D3DXVECTOR3(0.3f, 1.6f, 0.0f), false, WAY_DOWN);
				break;
			case 550:
				SetEnemy(D3DXVECTOR3(1040.0f, -40.0f, 0.0f), ENEMY_XU_5, D3DXVECTOR3(0.3f, 1.6f, 0.0f), false, WAY_DOWN);
				break;
			case 800:
				SetEnemy(D3DXVECTOR3(140.0f, -40.0f, 0.0f), ENEMY_XU_5, D3DXVECTOR3(-0.3f, 1.6f, 0.0f), false, WAY_DOWN);
				break;
			case 900:
				SetEnemy(D3DXVECTOR3(140.0f, -40.0f, 0.0f), ENEMY_XU_17, D3DXVECTOR3(-0.3f, 1.6f, 0.0f), false, WAY_DOWN);
				break;
			case 1000:
				SetEnemy(D3DXVECTOR3(140.0f, -40.0f, 0.0f), ENEMY_XU_37, D3DXVECTOR3(-0.3f, 1.6f, 0.0f), false, WAY_DOWN);
				break;
			case 1100:
				SetEnemy(D3DXVECTOR3(140.0f, -40.0f, 0.0f), ENEMY_XU_57, D3DXVECTOR3(-0.3f, 1.6f, 0.0f), true, WAY_DOWN);
				break;
			case 1300:
				SetEnemy(D3DXVECTOR3(-40.0f, 40.0f, 0.0f), ENEMY_XU_5, D3DXVECTOR3(2.5f, 0.6f, 0.0f), false, WAY_RIGHT);
				break;
			case 1400:
				SetEnemy(D3DXVECTOR3(1320.0f, 40.0f, 0.0f), ENEMY_XU_37, D3DXVECTOR3(-2.3f, 0.2f, 0.0f), false, WAY_LEFT);
				break;
			case 1550:
				SetEnemy(D3DXVECTOR3(720.0f, 760.0f, 0.0f), ENEMY_XU_5, D3DXVECTOR3(0.0f, -2.4f, 0.0f), false, WAY_UP);
				break;
			case 1650:
				SetEnemy(D3DXVECTOR3(840.0f, 760.0f, 0.0f), ENEMY_XU_17, D3DXVECTOR3(0.0f, -2.4f, 0.0f), false, WAY_UP);
				break;
			case 1750:
				SetEnemy(D3DXVECTOR3(960.0f, 760.0f, 0.0f), ENEMY_XU_37, D3DXVECTOR3(0.0f, -2.4f, 0.0f), false, WAY_UP);
				break;
			case 1850:
				SetEnemy(D3DXVECTOR3(1080.0f, 760.0f, 0.0f), ENEMY_XU_57, D3DXVECTOR3(0.0f, -2.4f, 0.0f), false, WAY_UP);
				break;
			case 2000:
				SetEnemy(D3DXVECTOR3(560.0f, 760.0f, 0.0f), ENEMY_XU_5, D3DXVECTOR3(0.0f, -2.4f, 0.0f), false, WAY_UP);
				break;
			case 2100:
				SetEnemy(D3DXVECTOR3(440.0f, 760.0f, 0.0f), ENEMY_XU_17, D3DXVECTOR3(0.0f, -2.4f, 0.0f), false, WAY_UP);
				break;
			case 2200:
				SetEnemy(D3DXVECTOR3(320.0f, 760.0f, 0.0f), ENEMY_XU_37, D3DXVECTOR3(0.0f, -2.4f, 0.0f), false, WAY_UP);
				break;
			case 2300:
				SetEnemy(D3DXVECTOR3(200.0f, 760.0f, 0.0f), ENEMY_XU_57, D3DXVECTOR3(0.0f, -2.4f, 0.0f), false, WAY_UP);
				break;
			case 2600:
				SetEnemy(D3DXVECTOR3(120.0f, -40.0f, 0.0f), ENEMY_XU_17, D3DXVECTOR3(-0.3f, 1.6f, 0.0f), true, WAY_DOWN);
				break;
			case 2750:
				SetEnemy(D3DXVECTOR3(-40.0f, 240.0f, 0.0f), ENEMY_XU_5, D3DXVECTOR3(4.0f, 0.0f, 0.0f), false, WAY_RIGHT);
				break;
			case 2850:
				SetEnemy(D3DXVECTOR3(1320.0f, 140.0f, 0.0f), ENEMY_XU_5, D3DXVECTOR3(-4.0f, 0.0f, 0.0f), false, WAY_LEFT);
				break;
			case 3000:
				SetEnemy(D3DXVECTOR3(640.0f, -40.0f, 0.0f), ENEMY_XU_5, D3DXVECTOR3(0.0f, 2.5f, 0.0f), false, WAY_DOWN);
				break;
			case 3100:
				SetEnemy(D3DXVECTOR3(520.0f, -40.0f, 0.0f), ENEMY_XU_17, D3DXVECTOR3(0.0f, 2.5f, 0.0f), false, WAY_DOWN);
				SetEnemy(D3DXVECTOR3(760.0f, -40.0f, 0.0f), ENEMY_XU_17, D3DXVECTOR3(0.0f, 2.5f, 0.0f), false, WAY_DOWN);
				break;
			case 3200:
				SetEnemy(D3DXVECTOR3(400.0f, -40.0f, 0.0f), ENEMY_XU_37, D3DXVECTOR3(0.0f, 2.5f, 0.0f), false, WAY_DOWN);
				SetEnemy(D3DXVECTOR3(880.0f, -40.0f, 0.0f), ENEMY_XU_37, D3DXVECTOR3(0.0f, 2.5f, 0.0f), false, WAY_DOWN);
				break;
			case 3300:
				SetEnemy(D3DXVECTOR3(280.0f, -40.0f, 0.0f), ENEMY_XU_57, D3DXVECTOR3(0.0f, 2.5f, 0.0f), false, WAY_DOWN);
				SetEnemy(D3DXVECTOR3(1000.0f, -40.0f, 0.0f), ENEMY_XU_57, D3DXVECTOR3(0.0f, 2.5f, 0.0f), true, WAY_DOWN);
				break;
			case 4000:
				StopSound(SOUND_LABEL_BGM006);
				SetBoss(D3DXVECTOR3(640.0f, -140.0f, 0.0f), BOSS_XU_80);
				PlaySound(SOUND_LABEL_BGM003);
				break;
			}
			break;
		}
	}
	else
	{
		UpdatepauseUi();
		ScrollBg(false);
	}
}

//ï`âÊèàóù
void DrawGame(void)
{
	DrawBullet();
	DrawExplosion();
	DrawLaser();
	DrawMissile();
	DrawSmoke();
	DrawUav();
	DrawEnemy();
	DrawBoss();
	DrawAircraft();
	DrawDeath();
	DrawBarrier();
	DrawPrize();
	DrawLockon();
	DrawUi();
	if (g_bStop == true)
	{
		DrawpauseUi();
	}
}

void StartGame(void)
{
	g_bStop = false;
}

void RestartGame(void)
{
	g_nTime = 0;
	ResetBarrier();
	ResetBullet();
	ResetAircraft();
	ResetEnemy();
	ResetExplosion();
	ResetLaser();
	ResetLockon();
	ResetMissile();
	ResetPrize();
	ResetSmoke();
	ResetUav();
	ResetUi();
	ResetDeath();
	ResetBoss();
	SetBg(1,true);
}

void NextGame(void)
{
	g_nTime = 0;
	g_nGameStage++;
	ResetBarrier();
	ResetBullet();
	SetAircraft();
	ResetEnemy();
	ResetExplosion();
	ResetLaser();
	ResetLockon();
	ResetMissile();
	ResetPrize();
	ResetSmoke();
	ResetUav();
	ResetUi();
	ResetDeath();
	ResetBoss();
	SetBg(1, true);
}