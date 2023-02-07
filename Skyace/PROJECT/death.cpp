//--------------------------
//Author:三上航世
//やられたとき処理(death.cpp)
//--------------------------
#include "death.h"
#include "sound.h"

//マクロ定義
#define DEATH_BLOCK (80.0f)  //爆発の大きさ

//爆発の構造体定義
typedef struct
{
	D3DXVECTOR3 pos; //位置
	int nCounterAnim;       //寿命
	int nPatternAnim;
	bool bUse;       //使用しているかどうか
} Death;

//グローバル変数
LPDIRECT3DTEXTURE9 g_pTextureDeath = NULL; //テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffDeath = NULL; //頂点バッファへのポインタ
Death g_aDeath[128];

//初期化処理
void InitDeath(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = GetDevice();
	int nCntDeath;
	//爆発の初期化
	for (nCntDeath = 0; nCntDeath < 128; nCntDeath++)
	{
		g_aDeath[nCntDeath].bUse = false;
		g_aDeath[nCntDeath].nCounterAnim = 0;
		g_aDeath[nCntDeath].nPatternAnim = 0;
		g_aDeath[nCntDeath].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}
	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/Texture/death.png",
		&g_pTextureDeath);
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * 128,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffDeath,
		NULL);

	VERTEX_2D *pVtx;
	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffDeath->Lock(0, 0, (void **)&pVtx, 0);
	for (nCntDeath = 0; nCntDeath < 128; nCntDeath++)
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
		pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 220);
		pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 220);
		pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 220);
		pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 220);
		//テクスチャ座標
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.125f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.125f, 1.0f);
		pVtx += 4;
	}
	//頂点バッファをアンロックする
	g_pVtxBuffDeath->Unlock();
}

//爆発の終了処理
void UninitDeath(void)
{
	//テクスチャの破棄
	if (g_pTextureDeath != NULL)
	{
		g_pTextureDeath->Release();
		g_pTextureDeath = NULL;
	}
	//頂点バッファの破棄
	if (g_pVtxBuffDeath != NULL)
	{
		g_pVtxBuffDeath->Release();
		g_pVtxBuffDeath = NULL;
	}
}

//爆発の更新処理
void UpdateDeath(void)
{
	int nCntDeath;
	for (nCntDeath = 0; nCntDeath < 128; nCntDeath++)
	{
		//使ってる
		if (g_aDeath[nCntDeath].bUse == true)
		{
			//アニメーションカウント進める
			g_aDeath[nCntDeath].nCounterAnim++;
			if (g_aDeath[nCntDeath].nCounterAnim == 6)
			{
				//アニメーションパターン進める、カウントリセット
				g_aDeath[nCntDeath].nPatternAnim++;
				g_aDeath[nCntDeath].nCounterAnim = 0;
				//パターン進む
				if (g_aDeath[nCntDeath].nPatternAnim == 8)
				{
					//消す
					g_aDeath[nCntDeath].bUse = false;
				}
			}

			//パターンに応じてテクスチャの描画個所を切り替える
			VERTEX_2D *pVtx;
			g_pVtxBuffDeath->Lock(0, 0, (void **)&pVtx, 0);
			pVtx += nCntDeath * 4;
			pVtx[0].tex = D3DXVECTOR2((float)g_aDeath[nCntDeath].nPatternAnim / 8, 0.0f);
			pVtx[1].tex = D3DXVECTOR2((float)(g_aDeath[nCntDeath].nPatternAnim + 1) / 8, 0.0f);
			pVtx[2].tex = D3DXVECTOR2((float)g_aDeath[nCntDeath].nPatternAnim / 8, 1.0f);
			pVtx[3].tex = D3DXVECTOR2((float)(g_aDeath[nCntDeath].nPatternAnim + 1) / 8, 1.0f);
			g_pVtxBuffDeath->Unlock();
		}
	}
}

//描画処理
void DrawDeath(void)
{
	int nCntDeath;
	LPDIRECT3DDEVICE9 pDevice;                    //デバイスのポインタ
	pDevice = GetDevice();                        //デバイスを取得する
	//頂点バッファをデバイスのデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffDeath, 0, sizeof(VERTEX_2D));
	pDevice->SetFVF(FVF_VERTEX_2D);               //頂点フォーマットの設定
	pDevice->SetTexture(0, g_pTextureDeath);    //テクスチャの設定
	for (nCntDeath = 0; nCntDeath < 128; nCntDeath++)
	{
		//使用中のみ描画
		if (g_aDeath[nCntDeath].bUse == true)
		{
			//ポリゴン描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
				nCntDeath * 4,
				2);
		}
	}
}

//爆発の設定処理
void SetDeath(D3DXVECTOR3 pos, bool bSound)
{
	int nCntDeath;
	VERTEX_2D *pVtx;
	g_pVtxBuffDeath->Lock(0, 0, (void **)&pVtx, 0);
	for (nCntDeath = 0; nCntDeath < 128; nCntDeath++)
	{
		//使ってない
		if (g_aDeath[nCntDeath].bUse == false)
		{
			//色々設定
			g_aDeath[nCntDeath].pos = pos;
			pVtx[0].pos = D3DXVECTOR3(g_aDeath[nCntDeath].pos.x - (DEATH_BLOCK / 2.0f), g_aDeath[nCntDeath].pos.y - (DEATH_BLOCK / 2.0f), 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aDeath[nCntDeath].pos.x + (DEATH_BLOCK / 2.0f), g_aDeath[nCntDeath].pos.y - (DEATH_BLOCK / 2.0f), 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aDeath[nCntDeath].pos.x - (DEATH_BLOCK / 2.0f), g_aDeath[nCntDeath].pos.y + (DEATH_BLOCK / 2.0f), 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aDeath[nCntDeath].pos.x + (DEATH_BLOCK / 2.0f), g_aDeath[nCntDeath].pos.y + (DEATH_BLOCK / 2.0f), 0.0f);
			g_aDeath[nCntDeath].nCounterAnim = 0;
			g_aDeath[nCntDeath].nPatternAnim = 0;
			g_aDeath[nCntDeath].bUse = true;
			if (bSound == true)
			{
				PlaySound(SOUND_LABEL_ENEMYDOWN000);
			}
			break;
		}
		pVtx += 4;
	}
	g_pVtxBuffDeath->Unlock();
}

//リセット
void ResetDeath(void)
{
	int nCntDeath;
	for (nCntDeath = 0; nCntDeath < 128; nCntDeath++)
	{
		g_aDeath[nCntDeath].bUse = false;
	}
}