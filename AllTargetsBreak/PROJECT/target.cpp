//--------------------------
//Author:三上航世
//的の処理(target.cpp)
//--------------------------
#include "target.h"
#include "player.h"
#include "sound.h"
#include "gameui.h"
#include "fade.h"

//マクロ定義
#define MAX_TARGET (8) //的の最大数
#define TARGET_SIZE (50) //的のサイズ

//的の構造体定義
typedef struct
{
	D3DXVECTOR3 pos;  //位置
	D3DXVECTOR3 move; //動き
	D3DXVECTOR3 Maxmove;
	int nTime;
	int nMaxTime;
	bool bUse;        //使用しているかどうか
} Target;

//グローバル変数
LPDIRECT3DTEXTURE9 g_pTextureTarget = NULL; //テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTarget = NULL; //頂点バッファへのポインタ
Target g_aTarget[MAX_TARGET];

//初期化処理
void InitTarget(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	int nCntTarget;
	pDevice = GetDevice();
	//的の初期化
	for (nCntTarget = 0; nCntTarget < MAX_TARGET; nCntTarget++)
	{
		g_aTarget[nCntTarget].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aTarget[nCntTarget].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aTarget[nCntTarget].Maxmove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aTarget[nCntTarget].nTime = 0;
		g_aTarget[nCntTarget].nMaxTime = 0;
		g_aTarget[nCntTarget].bUse = false;
	}

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/Texture/target.png",
		&g_pTextureTarget);

	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_TARGET,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffTarget,
		NULL);

	VERTEX_2D *pVtx;
	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffTarget->Lock(0, 0, (void **)&pVtx, 0);
	for (nCntTarget = 0; nCntTarget < MAX_TARGET; nCntTarget++)
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

		//テクスチャ座標
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
		pVtx += 4;
	}
	//頂点バッファをアンロックする
	g_pVtxBuffTarget->Unlock();
}

//的の終了処理
void UninitTarget(void)
{
	//テクスチャの破棄
	if (g_pTextureTarget != NULL)
	{
		g_pTextureTarget->Release();
		g_pTextureTarget = NULL;
	}
	//頂点バッファの破棄
	if (g_pVtxBuffTarget != NULL)
	{
		g_pVtxBuffTarget->Release();
		g_pVtxBuffTarget = NULL;
	}
}

//的の更新処理
void UpdateTarget(void)
{
	int nCntTarget;
	VERTEX_2D *pVtx;
	g_pVtxBuffTarget->Lock(0, 0, (void **)&pVtx, 0);
	for (nCntTarget = 0; nCntTarget < MAX_TARGET; nCntTarget++)
	{
		//的使っている
		if (g_aTarget[nCntTarget].bUse == true)
		{
			//時間を進め、位置が変わっていた場合は変える
			g_aTarget[nCntTarget].nTime++;
			pVtx[0].pos = D3DXVECTOR3(g_aTarget[nCntTarget].pos.x               , g_aTarget[nCntTarget].pos.y, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aTarget[nCntTarget].pos.x + TARGET_SIZE, g_aTarget[nCntTarget].pos.y, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aTarget[nCntTarget].pos.x               , g_aTarget[nCntTarget].pos.y + TARGET_SIZE, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aTarget[nCntTarget].pos.x + TARGET_SIZE, g_aTarget[nCntTarget].pos.y + TARGET_SIZE, 0.0f);
		}
		pVtx += 4;
	}
	g_pVtxBuffTarget->Unlock();
}

//描画処理
void DrawTarget(void)
{
	LPDIRECT3DDEVICE9 pDevice;                    //デバイスのポインタ
	int nCntTarget;
	pDevice = GetDevice();                        //デバイスを取得する
												  //頂点バッファをデバイスのデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffTarget, 0, sizeof(VERTEX_2D));
	pDevice->SetFVF(FVF_VERTEX_2D);               //頂点フォーマットの設定
	for (nCntTarget = 0; nCntTarget < MAX_TARGET; nCntTarget++)
	{
		if (g_aTarget[nCntTarget].bUse == true)
		{
			pDevice->SetTexture(0, g_pTextureTarget);    //テクスチャの設定
														//ポリゴン描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
				nCntTarget * 4,
				2);
		}
	}
}

//的の設定処理
void SetTarget(D3DXVECTOR3 pos)
{
	int nCntTarget;
	VERTEX_2D *pVtx;
	g_pVtxBuffTarget->Lock(0, 0, (void **)&pVtx, 0);
	for (nCntTarget = 0; nCntTarget < MAX_TARGET; nCntTarget++)
	{
		if (g_aTarget[nCntTarget].bUse == false)
		{
			//位置設定
			g_aTarget[nCntTarget].pos = pos;
			pVtx[0].pos = D3DXVECTOR3(g_aTarget[nCntTarget].pos.x               , g_aTarget[nCntTarget].pos.y, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aTarget[nCntTarget].pos.x + TARGET_SIZE, g_aTarget[nCntTarget].pos.y, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aTarget[nCntTarget].pos.x               , g_aTarget[nCntTarget].pos.y + TARGET_SIZE, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aTarget[nCntTarget].pos.x + TARGET_SIZE, g_aTarget[nCntTarget].pos.y + TARGET_SIZE, 0.0f);
			g_aTarget[nCntTarget].bUse = true;
			break;
		}
		pVtx += 4;
	}
	g_pVtxBuffTarget->Unlock();
}

//的壊れたか
bool BreakTarget(D3DXVECTOR3 pos, float fHeight, float fWidth)
{
	int nCntTarget;
	bool bBreak = false;
	for (nCntTarget = 0; nCntTarget < MAX_TARGET; nCntTarget++)
	{
		if (g_aTarget[nCntTarget].bUse == true)
		{
			//的に当たったか
			if (pos.x <= g_aTarget[nCntTarget].pos.x + TARGET_SIZE && g_aTarget[nCntTarget].pos.x <= pos.x &&
				pos.y <= g_aTarget[nCntTarget].pos.y + TARGET_SIZE && g_aTarget[nCntTarget].pos.y <= pos.y)
			{
				//的を消す
				g_aTarget[nCntTarget].bUse = false;
				bBreak = true;
			}
		}
	}
	return bBreak;
}

//全部消す
void ResetTarget(void)
{
	int nCntTarget;
	for (nCntTarget = 0; nCntTarget < MAX_TARGET; nCntTarget++)
	{
		g_aTarget[nCntTarget].bUse = false;
	}
}

//今何個的があるか
int CountTarget(void)
{
	int nCntTarget;
	int nTarget = 0;
	//的が今何個あるか調べる
	for (nCntTarget = 0; nCntTarget < MAX_TARGET; nCntTarget++)
	{
		if (g_aTarget[nCntTarget].bUse == true)
		{
			nTarget++;
		}
	}

	//0個の場合かつフェード動いてない
	if (nTarget == 0 && GetFade() == FADE_NONE)
	{
		//リザルトに移行、時間止める、ゲームクリア処理をする
		SetFade(MODE_RESULT);
		StopTime();
		GameClear();
	}
	return nTarget;
}