//--------------------------
//Author:三上航世
//自機の処理(aircraft.cpp)
//--------------------------
#include "aircraft.h"
#include "input.h"
#include "bullet.h"
#include "uav.h"
#include "missile.h"
#include "barrier.h"
#include "laser.h"
#include "sound.h"
#include "death.h"
#include "fade.h"

//マクロ定義
#define MAX_AIRCRAFT (2)      //機体の種類
#define MAX_SPECIAL (6)       //特殊兵装最大弾数

//グローバル変数
LPDIRECT3DTEXTURE9 g_pTextureAircraft[MAX_AIRCRAFT * 2] = {}; //テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffAircraft = NULL; //頂点バッファへのポインタ
Aircraft g_aAircraft;

//初期化処理
void InitAircraft(void)
{
	int nCntAircraft;
	LPDIRECT3DDEVICE9 pDevice; //デバイスのポインタ
	pDevice = GetDevice();     //デバイスを取得する
	g_aAircraft.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_aAircraft.normalmove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_aAircraft.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_aAircraft.nLife = 0;
	g_aAircraft.nMaxLife = 0;
	g_aAircraft.nLag = 0;
	g_aAircraft.nReload = 0;
	g_aAircraft.nSpecial = 0;
	g_aAircraft.nDTime = 0;
	g_aAircraft.state = AIRCRAFTSTATE_NORMAL;
	g_aAircraft.nClear = 0;
	g_aAircraft.bDeath = false;
	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/Texture/eagle.png",
		&g_pTextureAircraft[0]);
	D3DXCreateTextureFromFile(pDevice,
		"data/Texture/flanker.png",
		&g_pTextureAircraft[1]);
	D3DXCreateTextureFromFile(pDevice,
		"data/Texture/eagleshadow.png",
		&g_pTextureAircraft[2]);
	D3DXCreateTextureFromFile(pDevice,
		"data/Texture/flankershadow.png",
		&g_pTextureAircraft[3]);
	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * 2,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffAircraft,
		NULL);
	VERTEX_2D *pVtx; //頂点情報へのポインタ
	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffAircraft->Lock(0, 0, (void **)&pVtx, 0);
	for (nCntAircraft = 0; nCntAircraft < 2; nCntAircraft++)
	{
		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		//rhwの設定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;
		//頂点カラー
		pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		if (nCntAircraft == 1)
		{
			pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 120);
			pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 120);
			pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 120);
			pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 120);
		}
		//テクスチャ座標
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
		pVtx += 4;
	}
	//頂点バッファをアンロックする
	g_pVtxBuffAircraft->Unlock();
}

//終了処理
void UninitAircraft(void)
{
	int nCntAircraft;
	//テクスチャの破棄
	for (nCntAircraft = 0; nCntAircraft < MAX_AIRCRAFT * 2; nCntAircraft++)
	{
		if (g_pTextureAircraft[nCntAircraft] != NULL)
		{
			g_pTextureAircraft[nCntAircraft]->Release();
			g_pTextureAircraft[nCntAircraft] = NULL;
		}
	}
	//頂点バッファの破棄
	if (g_pVtxBuffAircraft != NULL)
	{
		g_pVtxBuffAircraft->Release();
		g_pVtxBuffAircraft = NULL;
	}
}

//更新処理
void UpdateAircraft(void)
{
	if (g_aAircraft.bDeath == false)
	{
		VERTEX_2D *pVtx; //頂点情報へのポイント
		g_pVtxBuffAircraft->Lock(0, 0, (void **)&pVtx, 0);
		switch (g_aAircraft.state)
		{
		case AIRCRAFTSTATE_NORMAL:
			break;
		case AIRCRAFTSTATE_DAMAGE:
			g_aAircraft.nDTime--;
			g_aAircraft.nClear--;
			if (g_aAircraft.nDTime <= 475)
			{
				g_aAircraft.state = AIRCRAFTSTATE_NODAMAGE;
			}
			break;
		case AIRCRAFTSTATE_NODAMAGE:
			g_aAircraft.nDTime--;
			g_aAircraft.nClear--;
			if (g_aAircraft.nDTime > 140)
			{
				if (g_aAircraft.nClear > 10)
				{
					pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
					pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
					pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
					pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);
				}
				else
				{
					pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 0);
					pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 0);
					pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 0);
					pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 0);
					if (g_aAircraft.nClear < 0)
					{
						g_aAircraft.nClear = 20;
					}
				}
			}
			else
			{
				if (g_aAircraft.nClear > 5)
				{
					pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
					pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
					pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
					pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);
				}
				else
				{
					pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 0);
					pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 0);
					pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 0);
					pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 0);
					if (g_aAircraft.nClear < 0)
					{
						g_aAircraft.nClear = 10;
					}
				}
			}
			if (g_aAircraft.nDTime <= 0)
			{
				g_aAircraft.state = AIRCRAFTSTATE_NORMAL;
				pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
				pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
				pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
				pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			}
			break;
		case AIRCRAFTSTATE_BARRIER:
			break;
		}
		//shiftでダッシュ
		if (GetKeyboardPress(DIK_O) == true)
		{
			g_aAircraft.move = g_aAircraft.normalmove * 2;
		}
		else
		{
			g_aAircraft.move = g_aAircraft.normalmove;
		}
		if (GetKeyboardPress(DIK_A) == true)
		{
			g_aAircraft.nLag++;
			if (GetKeyboardPress(DIK_W) == true) //左上
			{
				g_aAircraft.pos.x += sinf(-D3DX_PI * 0.75f) * g_aAircraft.move.x;
				g_aAircraft.pos.y += cosf(-D3DX_PI * 0.75f) * g_aAircraft.move.y;
			}
			else if (GetKeyboardPress(DIK_S) == true) //左下
			{
				g_aAircraft.pos.x += sinf(-D3DX_PI * 0.25f) * g_aAircraft.move.x;
				g_aAircraft.pos.y += cosf(-D3DX_PI * 0.25f) * g_aAircraft.move.y;
			}
			else //左
			{
				g_aAircraft.pos.x += sinf(-D3DX_PI * 0.5f) * g_aAircraft.move.x;
				g_aAircraft.pos.y += cosf(-D3DX_PI * 0.5f) * g_aAircraft.move.y;
			}
		}
		else if (GetKeyboardPress(DIK_D) == true)
		{
			g_aAircraft.nLag++;
			if (GetKeyboardPress(DIK_W) == true) //右上
			{
				g_aAircraft.pos.x += sinf(D3DX_PI * 0.75f) * g_aAircraft.move.x;
				g_aAircraft.pos.y += cosf(D3DX_PI * 0.75f) * g_aAircraft.move.y;
			}
			else if (GetKeyboardPress(DIK_S) == true) //右下
			{
				g_aAircraft.pos.x += sinf(D3DX_PI * 0.25f) * g_aAircraft.move.x;
				g_aAircraft.pos.y += cosf(D3DX_PI * 0.25f) * g_aAircraft.move.y;
			}
			else //右
			{
				g_aAircraft.pos.x += sinf(D3DX_PI * 0.5f) * g_aAircraft.move.x;
				g_aAircraft.pos.y += cosf(D3DX_PI * 0.5f) * g_aAircraft.move.y;
			}
		}
		else if (GetKeyboardPress(DIK_W) == true) //上
		{
			g_aAircraft.nLag++;
			g_aAircraft.pos.x += sinf(D3DX_PI) * g_aAircraft.move.x;
			g_aAircraft.pos.y += cosf(D3DX_PI) * g_aAircraft.move.y;
		}
		else if (GetKeyboardPress(DIK_S) == true) //下
		{
			g_aAircraft.nLag++;
			g_aAircraft.pos.x += sinf(0.0f) * g_aAircraft.move.x;
			g_aAircraft.pos.y += cosf(0.0f) * g_aAircraft.move.y;
		}
		else
		{
			g_aAircraft.nLag = 0;
		}
		if (g_aAircraft.pos.x <= AIRCRAFT_WIDTH / 2) //左行った場合
		{
			g_aAircraft.pos.x = AIRCRAFT_WIDTH / 2;
		}
		if (g_aAircraft.pos.y <= AIRCRAFT_HEIGHT / 2) //上行った場合
		{
			g_aAircraft.pos.y = AIRCRAFT_HEIGHT / 2;
		}
		if (g_aAircraft.pos.x >= SCREEN_WIDTH - (AIRCRAFT_WIDTH / 2)) //右行った場合
		{
			g_aAircraft.pos.x = SCREEN_WIDTH - (AIRCRAFT_WIDTH / 2);
		}
		if (g_aAircraft.pos.y >= SCREEN_HEIGHT - (AIRCRAFT_HEIGHT / 2)) //下行った場合
		{
			g_aAircraft.pos.y = SCREEN_HEIGHT - (AIRCRAFT_HEIGHT / 2);
		}
		if (GetKeyboardTrigger(DIK_J) == true && GetKeyboardRelease(DIK_J) == false) //弾発射
		{
			SetBullet(g_aAircraft.pos, D3DXVECTOR3(0.0f, -12.0f, 0.0f), 0, false);
		}
		if (g_aAircraft.nReload > 0)
		{
			g_aAircraft.nReload--;
		}
		if (GetKeyboardTrigger(DIK_I) == true && g_aAircraft.nReload <= 0 && g_aAircraft.nSpecial > 0) //特殊兵装
		{
			g_aAircraft.nReload = 1800;
			g_aAircraft.nSpecial--;
			switch (g_aAircraft.nAType)
			{
			case 0: //イーグル
				switch (g_aAircraft.nSType)
				{
				case 0:
					SetUav(g_aAircraft.pos, g_aAircraft.move); //UAV
					break;
				case 1:
					SetMissile(g_aAircraft.pos, D3DXVECTOR3(0.0f, -30.0f, 0.0f), 0); //マルチロックミサイル
					break;
				case 2:
					if (g_aAircraft.state != AIRCRAFTSTATE_BARRIER)
					{
						Barrier *pBarrier;
						pBarrier = GetBarrier();
						int nCntBarrier;
						for (nCntBarrier = 0; nCntBarrier < MAX_BARRIER; nCntBarrier++, pBarrier++)
						{
							if (pBarrier->bUse == false)
							{
								SetBarrier(g_aAircraft.pos, 3, 0); //バリア
								g_aAircraft.state = AIRCRAFTSTATE_BARRIER;
								pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
								pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
								pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
								pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);
								break;
							}
						}
					}
					else
					{
						g_aAircraft.nReload = 0;
						g_aAircraft.nSpecial++;
					}
				}
				break;
			case 1: //フランカー
				switch (g_aAircraft.nSType)
				{
				case 0:
					SetBullet(g_aAircraft.pos, D3DXVECTOR3(0.0f, -60.0f, 0.0f), 1, false); //レールガン
					break;
				case 1:
					SetMissile(g_aAircraft.pos, D3DXVECTOR3(0.0f, -30.0f, 0.0f), 1); //ボムミサイル
					break;
				case 2:
					SetLaser(g_aAircraft.pos, 0); //レーザー
					break;
				}
				break;
			}
		}
		pVtx[0].pos = D3DXVECTOR3((float)(g_aAircraft.pos.x - AIRCRAFT_WIDTH / 2), (float)(g_aAircraft.pos.y - AIRCRAFT_HEIGHT / 2), g_aAircraft.pos.z);
		pVtx[1].pos = D3DXVECTOR3((float)(g_aAircraft.pos.x + AIRCRAFT_WIDTH / 2), (float)(g_aAircraft.pos.y - AIRCRAFT_HEIGHT / 2), g_aAircraft.pos.z);
		pVtx[2].pos = D3DXVECTOR3((float)(g_aAircraft.pos.x - AIRCRAFT_WIDTH / 2), (float)(g_aAircraft.pos.y + AIRCRAFT_HEIGHT / 2), g_aAircraft.pos.z);
		pVtx[3].pos = D3DXVECTOR3((float)(g_aAircraft.pos.x + AIRCRAFT_WIDTH / 2), (float)(g_aAircraft.pos.y + AIRCRAFT_HEIGHT / 2), g_aAircraft.pos.z);
		pVtx += 4;
		pVtx[0].pos = D3DXVECTOR3((float)(g_aAircraft.pos.x + 80.0f - SHADOW_AIRCRAFT_WIDTH / 2), (float)(g_aAircraft.pos.y + 80.0f - SHADOW_AIRCRAFT_HEIGHT / 2), 0.0f);
		pVtx[1].pos = D3DXVECTOR3((float)(g_aAircraft.pos.x + 80.0f + SHADOW_AIRCRAFT_WIDTH / 2), (float)(g_aAircraft.pos.y + 80.0f - SHADOW_AIRCRAFT_HEIGHT / 2), 0.0f);
		pVtx[2].pos = D3DXVECTOR3((float)(g_aAircraft.pos.x + 80.0f - SHADOW_AIRCRAFT_WIDTH / 2), (float)(g_aAircraft.pos.y + 80.0f + SHADOW_AIRCRAFT_HEIGHT / 2), 0.0f);
		pVtx[3].pos = D3DXVECTOR3((float)(g_aAircraft.pos.x + 80.0f + SHADOW_AIRCRAFT_WIDTH / 2), (float)(g_aAircraft.pos.y + 80.0f + SHADOW_AIRCRAFT_HEIGHT / 2), 0.0f);
		g_pVtxBuffAircraft->Unlock();
	}
	else
	{
		g_aAircraft.nDTime--;
		if (g_aAircraft.nDTime % 2 == 0)
		{
			g_aAircraft.pos.x += 3.0f;
		}
		else
		{
			g_aAircraft.pos.x -= 3.0f;
		}
		if (g_aAircraft.nDTime % 10 == 0)
		{
			D3DXVECTOR3 pos;
			pos = D3DXVECTOR3(g_aAircraft.pos.x - 45 + rand() % 90, g_aAircraft.pos.y - 60 + rand() % 120, 0.0f);
			SetDeath(pos, true);
		}
	}
}

//描画処理
void DrawAircraft(void)
{
	LPDIRECT3DDEVICE9 pDevice;                    //デバイスのポインタ
	pDevice = GetDevice();                        //デバイスを取得する
	//頂点バッファをデバイスのデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffAircraft, 0, sizeof(VERTEX_2D));
	pDevice->SetFVF(FVF_VERTEX_2D);               //頂点フォーマットの設定
	pDevice->SetTexture(0, g_pTextureAircraft[g_aAircraft.nAType]);    //テクスチャの設定
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
		    0,  //開始する始点のインデックス
		    2); //描画するプリミティブ数
	pDevice->SetTexture(0, g_pTextureAircraft[g_aAircraft.nAType + 2]);    //テクスチャの設定
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
			4,  //開始する始点のインデックス
			2); //描画するプリミティブ数
}

//自機の設定処理
void SetAircraft(void)
{
	VERTEX_2D *pVtx;
	g_pVtxBuffAircraft->Lock(0, 0, (void **)&pVtx, 0);
	g_aAircraft.pos = D3DXVECTOR3(640.0f, 700.0f, 0.0f);
	pVtx[0].pos = D3DXVECTOR3((float)(g_aAircraft.pos.x - (AIRCRAFT_WIDTH / 2)), (float)(g_aAircraft.pos.y - (AIRCRAFT_HEIGHT / 2)), 0.0f);
	pVtx[1].pos = D3DXVECTOR3((float)(g_aAircraft.pos.x + (AIRCRAFT_WIDTH / 2)), (float)(g_aAircraft.pos.y - (AIRCRAFT_HEIGHT / 2)), 0.0f);
	pVtx[2].pos = D3DXVECTOR3((float)(g_aAircraft.pos.x - (AIRCRAFT_WIDTH / 2)), (float)(g_aAircraft.pos.y + (AIRCRAFT_HEIGHT / 2)), 0.0f);
	pVtx[3].pos = D3DXVECTOR3((float)(g_aAircraft.pos.x + (AIRCRAFT_WIDTH / 2)), (float)(g_aAircraft.pos.y + (AIRCRAFT_HEIGHT / 2)), 0.0f);
	pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	switch (g_aAircraft.nAType)
	{
	case 0:
		g_aAircraft.nLife = 5;
		g_aAircraft.move = D3DXVECTOR3(7.0f, 7.0f, 0.0f);
		break;
	case 1:
		g_aAircraft.nLife = 4;
		g_aAircraft.move = D3DXVECTOR3(10.0f, 10.0f, 0.0f);
		break;
	}
	g_aAircraft.normalmove = g_aAircraft.move;
	g_aAircraft.nMaxLife = g_aAircraft.nLife;
	pVtx += 4;
	pVtx[0].pos = D3DXVECTOR3((float)(g_aAircraft.pos.x + 80.0f - SHADOW_AIRCRAFT_WIDTH / 2), (float)(g_aAircraft.pos.y + 80.0f - SHADOW_AIRCRAFT_HEIGHT / 2), 0.0f);
	pVtx[1].pos = D3DXVECTOR3((float)(g_aAircraft.pos.x + 80.0f + SHADOW_AIRCRAFT_WIDTH / 2), (float)(g_aAircraft.pos.y + 80.0f - SHADOW_AIRCRAFT_HEIGHT / 2), 0.0f);
	pVtx[2].pos = D3DXVECTOR3((float)(g_aAircraft.pos.x + 80.0f - SHADOW_AIRCRAFT_WIDTH / 2), (float)(g_aAircraft.pos.y + 80.0f + SHADOW_AIRCRAFT_HEIGHT / 2), 0.0f);
	pVtx[3].pos = D3DXVECTOR3((float)(g_aAircraft.pos.x + 80.0f + SHADOW_AIRCRAFT_WIDTH / 2), (float)(g_aAircraft.pos.y + 80.0f + SHADOW_AIRCRAFT_HEIGHT / 2), 0.0f);
	g_aAircraft.nDTime = 0;
	g_aAircraft.nLag = 0;
	g_aAircraft.nReload = 0;
	g_aAircraft.nClear = 0;
	g_aAircraft.bDeath = false;
	g_aAircraft.state = AIRCRAFTSTATE_NORMAL;
	if (g_aAircraft.nSpecial < 2)
	{
		g_aAircraft.nSpecial = 2;
	}
	g_aAircraft.nReSpecial = g_aAircraft.nSpecial;
	g_pVtxBuffAircraft->Unlock();
}

//自機の取得
Aircraft *GetAircraft(void)
{
	return &g_aAircraft;
}

//自機の種類設定
void SetTypeAircraft(int nAType, int nSType)
{
	g_aAircraft.nAType = nAType;
	g_aAircraft.nSType = nSType;
}

void HitAircraft(void)
{
	g_aAircraft.nLife--;
	g_aAircraft.nDTime = 480;
	g_aAircraft.nClear = 20;
	PlaySound(SOUND_LABEL_DAMAGE000);
	if (g_aAircraft.nLife <= 0)
	{
		g_aAircraft.bDeath = true;
		SetDeath(g_aAircraft.pos, true);
		SetFade(MODE_RESULT, true);
	}
	if (g_aAircraft.nLife == 1)
	{
		PlaySound(SOUND_LABEL_PINCH000);
	}
	g_aAircraft.state = AIRCRAFTSTATE_DAMAGE;
	VERTEX_2D *pVtx;
	g_pVtxBuffAircraft->Lock(0, 0, (void **)&pVtx, 0);
	pVtx[0].col = D3DCOLOR_RGBA(255, 0, 0, 255);
	pVtx[1].col = D3DCOLOR_RGBA(255, 0, 0, 255);
	pVtx[2].col = D3DCOLOR_RGBA(255, 0, 0, 255);
	pVtx[3].col = D3DCOLOR_RGBA(255, 0, 0, 255);
	g_pVtxBuffAircraft->Unlock();
}

void ResetAircraft(void)
{
	VERTEX_2D *pVtx;
	g_pVtxBuffAircraft->Lock(0, 0, (void **)&pVtx, 0);
	g_aAircraft.pos = D3DXVECTOR3(640.0f, 700.0f, 0.0f);
	pVtx[0].pos = D3DXVECTOR3((float)(g_aAircraft.pos.x - (AIRCRAFT_WIDTH / 2)), (float)(g_aAircraft.pos.y - (AIRCRAFT_HEIGHT / 2)), 0.0f);
	pVtx[1].pos = D3DXVECTOR3((float)(g_aAircraft.pos.x + (AIRCRAFT_WIDTH / 2)), (float)(g_aAircraft.pos.y - (AIRCRAFT_HEIGHT / 2)), 0.0f);
	pVtx[2].pos = D3DXVECTOR3((float)(g_aAircraft.pos.x - (AIRCRAFT_WIDTH / 2)), (float)(g_aAircraft.pos.y + (AIRCRAFT_HEIGHT / 2)), 0.0f);
	pVtx[3].pos = D3DXVECTOR3((float)(g_aAircraft.pos.x + (AIRCRAFT_WIDTH / 2)), (float)(g_aAircraft.pos.y + (AIRCRAFT_HEIGHT / 2)), 0.0f);
	pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	switch (g_aAircraft.nAType)
	{
	case 0:
		g_aAircraft.nLife = 5;
		g_aAircraft.move = D3DXVECTOR3(7.0f, 7.0f, 0.0f);
		break;
	case 1:
		g_aAircraft.nLife = 4;
		g_aAircraft.move = D3DXVECTOR3(10.0f, 10.0f, 0.0f);
		break;
	}
	g_aAircraft.normalmove = g_aAircraft.move;
	g_aAircraft.nMaxLife = g_aAircraft.nLife;
	pVtx += 4;
	pVtx[0].pos = D3DXVECTOR3((float)(g_aAircraft.pos.x + 80.0f - SHADOW_AIRCRAFT_WIDTH / 2), (float)(g_aAircraft.pos.y + 80.0f - SHADOW_AIRCRAFT_HEIGHT / 2), 0.0f);
	pVtx[1].pos = D3DXVECTOR3((float)(g_aAircraft.pos.x + 80.0f + SHADOW_AIRCRAFT_WIDTH / 2), (float)(g_aAircraft.pos.y + 80.0f - SHADOW_AIRCRAFT_HEIGHT / 2), 0.0f);
	pVtx[2].pos = D3DXVECTOR3((float)(g_aAircraft.pos.x + 80.0f - SHADOW_AIRCRAFT_WIDTH / 2), (float)(g_aAircraft.pos.y + 80.0f + SHADOW_AIRCRAFT_HEIGHT / 2), 0.0f);
	pVtx[3].pos = D3DXVECTOR3((float)(g_aAircraft.pos.x + 80.0f + SHADOW_AIRCRAFT_WIDTH / 2), (float)(g_aAircraft.pos.y + 80.0f + SHADOW_AIRCRAFT_HEIGHT / 2), 0.0f);
	g_aAircraft.nDTime = 0;
	g_aAircraft.state = AIRCRAFTSTATE_NORMAL;
	g_aAircraft.nClear = 0;
	g_aAircraft.nLag = 0;
	g_aAircraft.nReload = 0;
	g_aAircraft.bDeath = false;
	g_aAircraft.nSpecial = g_aAircraft.nReSpecial;
	g_aAircraft.state = AIRCRAFTSTATE_NORMAL;
	g_pVtxBuffAircraft->Unlock();
}