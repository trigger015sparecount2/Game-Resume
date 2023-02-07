//-------------------------
//Author:三上航世
//ゲーム中のUI処理(gameui.cpp)
//-------------------------
#include "gameui.h"
#include "player.h"
#include "fade.h"
#include "target.h"
#include "sound.h"

//マクロ定義
#define MAX_NUMBER (2)		//桁数
#define NUMBER_PATTERN (10)	//数字のパターン数
#define NUMBER_WIDTH (40)	//数字の高さ
#define NUMBER_HEIGHT (60)	//数字の幅

//グローバル変数
LPDIRECT3DTEXTURE9 g_pTextureGameUi[6] = {};	 //テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffGameUi = NULL; //頂点バッファへのポインタ
D3DXVECTOR3 g_aPos[5 + 5];   //各ポリゴンの位置(的、時間)
int g_aGameUIclear[2]; //UIの透け具合
int g_nTimeSec; //残り時間（秒）
int g_nTimeCom; //残り時間（コンマ）
bool g_bStopTime; //時間が止まっているかどうか
bool g_bGameOver; //ゲームの終了する条件を満たしたかどうか
float g_fRot[4];  //時間の針の向き

//初期化処理
void InitGameUi(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	int nCntUi;
	pDevice = GetDevice();
	g_aPos[0] = D3DXVECTOR3(115.0f, 50.0f, 0.0f); //的の四角
	g_aPos[1] = D3DXVECTOR3(60.0f, 50.0f, 0.0f); //的マーク
	g_aPos[2] = D3DXVECTOR3(110.0f, 50.0f, 0.0f); //×マーク
	for (nCntUi = 0; nCntUi < 2; nCntUi++) //的の残り枚数数値
	{
		g_aPos[nCntUi + 3] = D3DXVECTOR3(150.0f + nCntUi * 30.0f, 50.0f, 0.0f);
	}
	g_aPos[5] = D3DXVECTOR3(1190.0f, 50.0f, 0.0f);//時計の四角
	for (nCntUi = 0; nCntUi < 2; nCntUi++) //時計マーク
	{
		g_aPos[nCntUi + 6] = D3DXVECTOR3(1150.0f, 50.0f, 0.0f);
	}
	for (nCntUi = 0; nCntUi < 2; nCntUi++) //残り時間
	{
		g_aPos[nCntUi + 8] = D3DXVECTOR3(1210.0f + nCntUi * 30.0f, 50.0f, 0.0f);
	}
	ResetGameUi();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/Texture/number.png",
		&g_pTextureGameUi[0]);
	D3DXCreateTextureFromFile(pDevice,
		"data/Texture/target.png",
		&g_pTextureGameUi[1]);
	D3DXCreateTextureFromFile(pDevice,
		"data/Texture/×mark.png",
		&g_pTextureGameUi[2]);
	D3DXCreateTextureFromFile(pDevice,
		"data/Texture/clock.png",
		&g_pTextureGameUi[3]);
	D3DXCreateTextureFromFile(pDevice,
		"data/Texture/clockneedle.png",
		&g_pTextureGameUi[4]);
	D3DXCreateTextureFromFile(pDevice,
		"data/Texture/slongsquare.png",
		&g_pTextureGameUi[5]);
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * 10,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffGameUi,
		NULL);
	VERTEX_2D *pVtx;
	nCntUi = 0;
	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffGameUi->Lock(0, 0, (void **)&pVtx, 0);
	//的四角
	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(g_aPos[0].x - 110, g_aPos[0].y - 50, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_aPos[0].x + 110, g_aPos[0].y - 50, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_aPos[0].x - 110, g_aPos[0].y + 50, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_aPos[0].x + 110, g_aPos[0].y + 50, 0.0f);
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
	//テクスチャ座標
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
	pVtx += 4;
	//的マーク
	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(g_aPos[1].x - 30, g_aPos[1].y - 30, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_aPos[1].x + 30, g_aPos[1].y - 30, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_aPos[1].x - 30, g_aPos[1].y + 30, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_aPos[1].x + 30, g_aPos[1].y + 30, 0.0f);
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
	//テクスチャ座標
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
	pVtx += 4;
	//×マーク
	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(g_aPos[2].x - 20, g_aPos[2].y - 20, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_aPos[2].x + 20, g_aPos[2].y - 20, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_aPos[2].x - 20, g_aPos[2].y + 20, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_aPos[2].x + 20, g_aPos[2].y + 20, 0.0f);
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
	//テクスチャ座標
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
	pVtx += 4;
	for (nCntUi = 0; nCntUi < MAX_NUMBER; nCntUi++) //的の残り枚数数値
	{
		pVtx[0].pos = D3DXVECTOR3(g_aPos[nCntUi + 3].x - NUMBER_WIDTH / 2, g_aPos[nCntUi + 3].y - NUMBER_HEIGHT / 2, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aPos[nCntUi + 3].x + NUMBER_WIDTH / 2, g_aPos[nCntUi + 3].y - NUMBER_HEIGHT / 2, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aPos[nCntUi + 3].x - NUMBER_WIDTH / 2, g_aPos[nCntUi + 3].y + NUMBER_HEIGHT / 2, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aPos[nCntUi + 3].x + NUMBER_WIDTH / 2, g_aPos[nCntUi + 3].y + NUMBER_HEIGHT / 2, 0.0f);
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
		//テクスチャ座標
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.1f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f, 1.0f);
		pVtx += 4;
	}
	//残り時間四角
	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(g_aPos[5].x - 90, g_aPos[5].y - 50, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_aPos[5].x + 90, g_aPos[5].y - 50, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_aPos[5].x - 90, g_aPos[5].y + 50, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_aPos[5].x + 90, g_aPos[5].y + 50, 0.0f);
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
	//テクスチャ座標
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
	pVtx += 4;
	for (nCntUi = 0; nCntUi < 2; nCntUi++)//時計
	{
		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(g_aPos[nCntUi + 6].x - 30, g_aPos[nCntUi + 6].y - 30, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aPos[nCntUi + 6].x + 30, g_aPos[nCntUi + 6].y - 30, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aPos[nCntUi + 6].x - 30, g_aPos[nCntUi + 6].y + 30, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aPos[nCntUi + 6].x + 30, g_aPos[nCntUi + 6].y + 30, 0.0f);
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
		//テクスチャ座標
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
		pVtx += 4;
	}
	for (nCntUi = 0; nCntUi < 2; nCntUi++)//残り時間時間数値
	{
		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(g_aPos[nCntUi + 8].x - (NUMBER_WIDTH / 2), g_aPos[nCntUi + 8].y - (NUMBER_HEIGHT / 2), 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aPos[nCntUi + 8].x + (NUMBER_WIDTH / 2), g_aPos[nCntUi + 8].y - (NUMBER_HEIGHT / 2), 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aPos[nCntUi + 8].x - (NUMBER_WIDTH / 2), g_aPos[nCntUi + 8].y + (NUMBER_HEIGHT / 2), 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aPos[nCntUi + 8].x + (NUMBER_WIDTH / 2), g_aPos[nCntUi + 8].y + (NUMBER_HEIGHT / 2), 0.0f);
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
		//テクスチャ座標
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.1f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f, 1.0f);
		pVtx += 4;
	}
	//頂点バッファをアンロックする
	g_pVtxBuffGameUi->Unlock();
}

//UIの終了処理
void UninitGameUi(void)
{
	int nCntUi;
	for (nCntUi = 0; nCntUi < 6; nCntUi++)
	{
		//テクスチャの破棄
		if (g_pTextureGameUi[nCntUi] != NULL)
		{
			g_pTextureGameUi[nCntUi]->Release();
			g_pTextureGameUi[nCntUi] = NULL;
		}
	}
	//頂点バッファの破棄
	if (g_pVtxBuffGameUi != NULL)
	{
		g_pVtxBuffGameUi->Release();
		g_pVtxBuffGameUi = NULL;
	}
}

//UIの更新処理
void UpdateGameUi(void)
{
	int nCntUi;
	int nNumber;
	int nRank;
	if (g_bStopTime == false)
	{
		g_nTimeCom--;
		if (g_nTimeCom <= 0)
		{
			g_nTimeSec--;
			g_nTimeCom = TIME_LIMIT_COM;
			if (g_nTimeSec <= 10)
			{
				PlaySound(SOUNT_LABEL_COUNTDOWN);
			}
		}
		for (nCntUi = 0; nCntUi < 4; nCntUi++)
		{
			g_fRot[nCntUi] += 2.0f / (float)(TIME_LIMIT_COM * TIME_LIMIT_SEC);
			if (g_fRot[nCntUi] <= -1.0f)
			{
				g_fRot[nCntUi] += 2.0f;
			}
		}
	}

	VERTEX_2D *pVtx;
	g_pVtxBuffGameUi->Lock(0, 0, (void **)&pVtx, 0);
	Player *pPlayer;
	pPlayer = GetPlayer();
	if (pPlayer->pos.x - (PLAYER_WIDTH / 2) <= g_aPos[0].x + 110 && g_aPos[0].x - 110 <= pPlayer->pos.x + (PLAYER_WIDTH / 2) &&
		pPlayer->pos.y - PLAYER_HEIGHT <= g_aPos[0].y + 50 && g_aPos[0].y - 50 <= pPlayer->pos.y) //透過処理
	{
		g_aGameUIclear[0] -= 7;
		if (g_aGameUIclear[0] <= 60)
		{
			g_aGameUIclear[0] = 60;
		}
	}
	else
	{
		g_aGameUIclear[0] += 7;
		if (g_aGameUIclear[0] >= 255)
		{
			g_aGameUIclear[0] = 255;
		}
	}

	//的四角とマーク
	for(nCntUi = 0; nCntUi < 3; nCntUi++)
	{
		pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, g_aGameUIclear[0]);
		pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, g_aGameUIclear[0]);
		pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, g_aGameUIclear[0]);
		pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, g_aGameUIclear[0]);
		pVtx += 4;
	}

	for (nCntUi = 0; nCntUi < MAX_NUMBER; nCntUi++) //的の残り枚数数値
	{
		nRank = pow(10, MAX_NUMBER - nCntUi);
		nNumber = CountTarget() % nRank / (nRank / 10);
		pVtx[0].tex = D3DXVECTOR2(nNumber * 0.1f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(nNumber * 0.1f + 0.1f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(nNumber * 0.1f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(nNumber * 0.1f + 0.1f, 1.0f);
		pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, g_aGameUIclear[0]);
		pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, g_aGameUIclear[0]);
		pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, g_aGameUIclear[0]);
		pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, g_aGameUIclear[0]);
		pVtx += 4;
	}

	//時間に当たっているか
	if (pPlayer->pos.x - (PLAYER_WIDTH / 2) <= g_aPos[5].x + 90 && g_aPos[5].x - 90 <= pPlayer->pos.x + (PLAYER_WIDTH / 2) &&
	    pPlayer->pos.y - PLAYER_HEIGHT <= g_aPos[5].y + 50 && g_aPos[5].y - 50 <= pPlayer->pos.y) //透過処理
	{
		g_aGameUIclear[1] -= 7;
		if (g_aGameUIclear[1] <= 60)
		{
			g_aGameUIclear[1] = 60;
		}
	}
	else
	{
		g_aGameUIclear[1] += 7;
		if (g_aGameUIclear[1] >= 255)
		{
			g_aGameUIclear[1] = 255;
		}

	}
	//残り時間四角と時計
	for(nCntUi = 0; nCntUi < 2; nCntUi++)
	{
		pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, g_aGameUIclear[1]);
		pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, g_aGameUIclear[1]);
		pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, g_aGameUIclear[1]);
		pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, g_aGameUIclear[1]);
		pVtx += 4;
	}
	//時計の針
	pVtx[0].pos = D3DXVECTOR3(g_aPos[7].x + 30 * sinf(D3DX_PI * g_fRot[0]), g_aPos[7].y + 30 * cosf(D3DX_PI * g_fRot[0]), 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_aPos[7].x + 30 * sinf(D3DX_PI * g_fRot[1]), g_aPos[7].y + 30 * cosf(D3DX_PI * g_fRot[1]), 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_aPos[7].x + 30 * sinf(D3DX_PI * g_fRot[2]), g_aPos[7].y + 30 * cosf(D3DX_PI * g_fRot[2]), 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_aPos[7].x + 30 * sinf(D3DX_PI * g_fRot[3]), g_aPos[7].y + 30 * cosf(D3DX_PI * g_fRot[3]), 0.0f);
	pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, g_aGameUIclear[1]);
	pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, g_aGameUIclear[1]);
	pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, g_aGameUIclear[1]);
	pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, g_aGameUIclear[1]);
	pVtx += 4;
	for (nCntUi = 0; nCntUi < 2; nCntUi++) //残り時間数値
	{
		nRank = pow(10, 2 - nCntUi);
		nNumber = g_nTimeSec % nRank / (nRank / 10);
		pVtx[0].tex = D3DXVECTOR2(nNumber * 0.1f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(nNumber * 0.1f + 0.1f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(nNumber * 0.1f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(nNumber * 0.1f + 0.1f, 1.0f);
		if (g_nTimeSec > 10)
		{
			pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, g_aGameUIclear[1]);
			pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, g_aGameUIclear[1]);
			pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, g_aGameUIclear[1]);
			pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, g_aGameUIclear[1]);
		}
		else
		{
			pVtx[0].col = D3DCOLOR_RGBA(255, 0, 0, g_aGameUIclear[1]);
			pVtx[1].col = D3DCOLOR_RGBA(255, 0, 0, g_aGameUIclear[1]);
			pVtx[2].col = D3DCOLOR_RGBA(255, 0, 0, g_aGameUIclear[1]);
			pVtx[3].col = D3DCOLOR_RGBA(255, 0, 0, g_aGameUIclear[1]);
		}
		pVtx += 4;
	}
	g_pVtxBuffGameUi->Unlock();
	if (g_nTimeSec <= 0 && GetFade() == FADE_NONE)
	{
		g_nTimeSec = 0;
		g_bGameOver = true;
		StopTime();
		SetFade(MODE_RESULT);
	}
}


//描画処理
void DrawGameUi(void)
{
	LPDIRECT3DDEVICE9 pDevice;                    //デバイスのポインタ
	int nCntUi = 0;
	pDevice = GetDevice();                        //デバイスを取得する
	//頂点バッファをデバイスのデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffGameUi, 0, sizeof(VERTEX_2D));
	pDevice->SetFVF(FVF_VERTEX_2D);               //頂点フォーマットの設定
	pDevice->SetTexture(0, g_pTextureGameUi[5]); //枚数四角
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
		nCntUi * 4,
		2);
	nCntUi = 1;
	pDevice->SetTexture(0, g_pTextureGameUi[1]); //的マーク
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
		nCntUi * 4,
		2);
	nCntUi = 2;
	pDevice->SetTexture(0, g_pTextureGameUi[2]); //×マーク
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
		nCntUi * 4,
		2);
	for (nCntUi = 3; nCntUi < 5; nCntUi++) //的の残り枚数
	{
		pDevice->SetTexture(0, g_pTextureGameUi[0]);//的の残り枚数(数値)
		//ポリゴン描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
			nCntUi * 4,
			2);
	}
	nCntUi = 5;
	pDevice->SetTexture(0, g_pTextureGameUi[5]);//残り時間四角
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
		nCntUi * 4,
		2);
	nCntUi = 6;
	pDevice->SetTexture(0, g_pTextureGameUi[3]);//時計
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
		nCntUi * 4,
		2);
	nCntUi = 7;
	pDevice->SetTexture(0, g_pTextureGameUi[4]);//時計の針
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
		nCntUi * 4,
		2);
	for (nCntUi = 8; nCntUi < 10; nCntUi++)//残り時間数値
	{
		pDevice->SetTexture(0, g_pTextureGameUi[0]);//残り時間数値
		//ポリゴン描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
			nCntUi * 4,
			2);
	}
}

//UIリセット
void ResetGameUi(void)
{
	g_nTimeCom = TIME_LIMIT_COM;
	g_nTimeSec = TIME_LIMIT_SEC;
	int nCntUi;
	for (nCntUi = 0; nCntUi < 2; nCntUi++)
	{
		g_aGameUIclear[nCntUi] = 255;
	}
	g_fRot[0] = -0.75;
	g_fRot[1] = 0.75;
	g_fRot[2] = -0.25;
	g_fRot[3] = 0.25;
	g_bStopTime = false;
	g_bGameOver = false;
}

//残りの秒取得
int TimeMin(void)
{
	return TIME_LIMIT_COM - g_nTimeCom;
}

//残りコンマ取得
int TimeSec(void)
{
	return TIME_LIMIT_SEC - g_nTimeSec;
}

//時間止める
void StopTime(void)
{
	g_bStopTime = true;
}

//ゲームオーバー取得
bool GameOver(void)
{
	return g_bGameOver;
}

//ゲームクリア―
void GameClear(void)
{
	g_bGameOver = true;
}