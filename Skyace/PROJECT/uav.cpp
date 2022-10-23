//--------------------------
//Author:三上航世
//味方無人機の処理(uav.cpp)
//--------------------------
#include "input.h"
#include "uav.h"
#include "aircraft.h"
#include "bullet.h"
#include "death.h"

//マクロ定義
#define UAV_WIDTH (60)  //ポリゴンの幅
#define UAV_HEIGHT (40) //ポリゴンの高さ
#define SHADOW_UAV_WIDTH (20)  //ポリゴンの幅
#define SHADOW_UAV_HEIGHT (15) //ポリゴンの高さ
#define MAX_UAV (2)     //UAVの最大数

//味方無人機の構造体定義
typedef struct
{
	D3DXVECTOR3 pos;  //位置
	D3DXVECTOR3 move; //移動量
	int nCounterAnim; //アニメーションカウンタ
	int nPatternAnim; //アニメーションパターンNo
	int nLife;        //寿命
	bool bUse;        //使用中か否か
} Uav;

//グローバル変数
LPDIRECT3DTEXTURE9 g_pTextureUav[2] = {}; //テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffUav = NULL; //頂点バッファへのポインタ
Uav g_aUav[MAX_UAV];

//初期化処理
void InitUav(void)
{
	int nCntUav;
	LPDIRECT3DDEVICE9 pDevice; //デバイスのポインタ
	pDevice = GetDevice();     //デバイスを取得する
	for (nCntUav = 0; nCntUav < MAX_UAV; nCntUav++)
	{
		g_aUav[nCntUav].bUse = false;
		g_aUav[nCntUav].nCounterAnim = 0;
		g_aUav[nCntUav].nPatternAnim = 0;
		g_aUav[nCntUav].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aUav[nCntUav].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aUav[nCntUav].nLife = 0;
	}
	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/Texture/UAV.png",
		&g_pTextureUav[0]);
	D3DXCreateTextureFromFile(pDevice,
		"data/Texture/UAVshadow.png",
		&g_pTextureUav[1]);
	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_UAV * 2,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffUav,
		NULL);
	VERTEX_2D *pVtx; //頂点情報へのポインタ
	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffUav->Lock(0, 0, (void **)&pVtx, 0);
	for (nCntUav = 0; nCntUav < MAX_UAV * 2; nCntUav++)
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
		if (nCntUav % 2 == 0)
	    {
			pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		}
		else
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
	g_pVtxBuffUav->Unlock();
}

//終了処理
void UninitUav(void)
{
	int nCntUav;
	for (nCntUav = 0; nCntUav < 2; nCntUav++)
	{
		//テクスチャの破棄
		if (g_pTextureUav[nCntUav] != NULL)
		{
			g_pTextureUav[nCntUav]->Release();
			g_pTextureUav[nCntUav] = NULL;
		}
	}
	//頂点バッファの破棄
	if (g_pVtxBuffUav != NULL)
	{
		g_pVtxBuffUav->Release();
		g_pVtxBuffUav = NULL;
	}
}

//更新処理
void UpdateUav(D3DXVECTOR3 pos, int nLag)
{
	int nCntUav;
	for (nCntUav = 0; nCntUav < MAX_UAV; nCntUav++)
	{
		if (g_aUav[nCntUav].bUse == true)
		{
			VERTEX_2D *pVtx; //頂点情報へのポインタ
			if (GetKeyboardTrigger(DIK_J) == true && GetKeyboardRelease(DIK_J) == false) //弾発射
			{
				SetBullet(g_aUav[nCntUav].pos, D3DXVECTOR3(0.0f, -12.0f, 0.0f), 0, true);
			}
			if (g_aUav[nCntUav].nLife > 870)
			{
				if (nCntUav == 0)
				{
					g_aUav[nCntUav].pos.x -= g_aUav[nCntUav].move.x;
				}
				else
				{
					g_aUav[nCntUav].pos.x += g_aUav[nCntUav].move.x;
				}
			}
			else if(nLag >= 4 || nLag == 0)
			{
				if (nCntUav == 0)//左にいるUAV
				{
					if (pos.x - g_aUav[nCntUav].pos.x < 80.0f)//左のUAVが自機に近い、あるいは自機より右にいる場合(=左に行きたい)
					{
						g_aUav[nCntUav].pos.x -= g_aUav[nCntUav].move.x * 1.5f;
					}
					else if(pos.x - g_aUav[nCntUav].pos.x > 200.0f)//左のUAVが自機よりも大幅に左にいる場合
					{
						g_aUav[nCntUav].pos.x += g_aUav[nCntUav].move.x * 1.5f;
					}
					else if (pos.x - g_aUav[nCntUav].pos.x > 120.0f)//左のUAVが自機よりも少し離れて左にいる場合
					{
						g_aUav[nCntUav].pos.x += g_aUav[nCntUav].move.x;
					}
				}
				else//右にいるUAV
				{
					if (g_aUav[nCntUav].pos.x - pos.x < 80.0f)//右のUAVが自機に近い、あるいは自機より左にいる場合
					{
						g_aUav[nCntUav].pos.x += g_aUav[nCntUav].move.x * 1.5f;
					}
					else if (g_aUav[nCntUav].pos.x - pos.x > 200.0f)//右のUAVが自機よりも大幅に右にいる場合
					{
						g_aUav[nCntUav].pos.x -= g_aUav[nCntUav].move.x * 1.5f;
					}
					else if (g_aUav[nCntUav].pos.x - pos.x > 120.0f)//右のUAVが自機よりも少し離れて右にいる場合
					{
						g_aUav[nCntUav].pos.x -= g_aUav[nCntUav].move.x;
					}
				}
				if (g_aUav[nCntUav].pos.y - pos.y > 10.0f)//自機がかなり上にいる場合
				{
					g_aUav[nCntUav].pos.y -= g_aUav[nCntUav].move.y * 1.5f;
				}
				else if (g_aUav[nCntUav].pos.y - pos.y > 5.0f)//自機が上にいる場合
				{
					g_aUav[nCntUav].pos.y -= g_aUav[nCntUav].move.y;
				}
				else if (pos.y - g_aUav[nCntUav].pos.y > 10.0f)//自機がかなり下にいる場合
				{
					g_aUav[nCntUav].pos.y += g_aUav[nCntUav].move.y * 1.5f;
				}
				else if (pos.y - g_aUav[nCntUav].pos.y > 5.0f)//自機が下にいる場合
				{
					g_aUav[nCntUav].pos.y += g_aUav[nCntUav].move.y;
				}
			}
			g_pVtxBuffUav->Lock(0, 0, (void **)&pVtx, 0);
			pVtx += nCntUav * 8;
			pVtx[0].pos = D3DXVECTOR3((float)(g_aUav[nCntUav].pos.x - UAV_WIDTH / 2), (float)(g_aUav[nCntUav].pos.y - UAV_HEIGHT / 2), 0.0f);
			pVtx[1].pos = D3DXVECTOR3((float)(g_aUav[nCntUav].pos.x + UAV_WIDTH / 2), (float)(g_aUav[nCntUav].pos.y - UAV_HEIGHT / 2), 0.0f);
			pVtx[2].pos = D3DXVECTOR3((float)(g_aUav[nCntUav].pos.x - UAV_WIDTH / 2), (float)(g_aUav[nCntUav].pos.y + UAV_HEIGHT / 2), 0.0f);
			pVtx[3].pos = D3DXVECTOR3((float)(g_aUav[nCntUav].pos.x + UAV_WIDTH / 2), (float)(g_aUav[nCntUav].pos.y + UAV_HEIGHT / 2), 0.0f);
			pVtx += 4;
			pVtx[0].pos = D3DXVECTOR3((float)(g_aUav[nCntUav].pos.x + 80.0f - SHADOW_UAV_WIDTH / 2), (float)(g_aUav[nCntUav].pos.y + 80.0f - SHADOW_UAV_HEIGHT / 2), 0.0f);
			pVtx[1].pos = D3DXVECTOR3((float)(g_aUav[nCntUav].pos.x + 80.0f + SHADOW_UAV_WIDTH / 2), (float)(g_aUav[nCntUav].pos.y + 80.0f - SHADOW_UAV_HEIGHT / 2), 0.0f);
			pVtx[2].pos = D3DXVECTOR3((float)(g_aUav[nCntUav].pos.x + 80.0f - SHADOW_UAV_WIDTH / 2), (float)(g_aUav[nCntUav].pos.y + 80.0f + SHADOW_UAV_HEIGHT / 2), 0.0f);
			pVtx[3].pos = D3DXVECTOR3((float)(g_aUav[nCntUav].pos.x + 80.0f + SHADOW_UAV_WIDTH / 2), (float)(g_aUav[nCntUav].pos.y + 80.0f + SHADOW_UAV_HEIGHT / 2), 0.0f);
			g_pVtxBuffUav->Unlock();
			g_aUav[nCntUav].nLife--;
			if (g_aUav[nCntUav].nLife <= 0)
			{
				g_aUav[nCntUav].bUse = false;
				SetDeath(g_aUav[nCntUav].pos, true);
			}
		}
	}
}

//描画処理
void DrawUav(void)
{
	LPDIRECT3DDEVICE9 pDevice;                    //デバイスのポインタ
	int nCntUav;
	pDevice = GetDevice();                        //デバイスを取得する
	//頂点バッファをデバイスのデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffUav, 0, sizeof(VERTEX_2D));
	pDevice->SetFVF(FVF_VERTEX_2D);               //頂点フォーマットの設定
	for (nCntUav = 0; nCntUav < MAX_UAV; nCntUav++)
	{
		if (g_aUav[nCntUav].bUse == true)
		{
			pDevice->SetTexture(0, g_pTextureUav[0]);    //テクスチャの設定
			//ポリゴン描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
				nCntUav * 8,
				2);
			pDevice->SetTexture(0, g_pTextureUav[1]);    //テクスチャの設定
														 //ポリゴン描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
				nCntUav * 8 + 4,
				2);
		}
	}
}

//味方無人機の設定処理
void SetUav(D3DXVECTOR3 pos, D3DXVECTOR3 move)
{
	int nCntUav;
	for(nCntUav = 0; nCntUav < MAX_UAV; nCntUav++)
	{
		if (g_aUav[nCntUav].bUse == false)
		{
			VERTEX_2D *pVtx;
			g_pVtxBuffUav->Lock(0, 0, (void **)&pVtx, 0);
			pVtx += nCntUav * 8;
			g_aUav[nCntUav].pos = pos;
			pVtx[0].pos = D3DXVECTOR3((float)(g_aUav[nCntUav].pos.x - (UAV_WIDTH / 2)), (float)(g_aUav[nCntUav].pos.y - (UAV_HEIGHT / 2)), 0.0f);
			pVtx[1].pos = D3DXVECTOR3((float)(g_aUav[nCntUav].pos.x + (UAV_WIDTH / 2)), (float)(g_aUav[nCntUav].pos.y - (UAV_HEIGHT / 2)), 0.0f);
			pVtx[2].pos = D3DXVECTOR3((float)(g_aUav[nCntUav].pos.x - (UAV_WIDTH / 2)), (float)(g_aUav[nCntUav].pos.y + (UAV_HEIGHT / 2)), 0.0f);
			pVtx[3].pos = D3DXVECTOR3((float)(g_aUav[nCntUav].pos.x + (UAV_WIDTH / 2)), (float)(g_aUav[nCntUav].pos.y + (UAV_HEIGHT / 2)), 0.0f);
			g_aUav[nCntUav].nLife = 900;
			g_aUav[nCntUav].move = move;
			g_aUav[nCntUav].nCounterAnim = 0;
			g_aUav[nCntUav].nPatternAnim = 0;
			g_aUav[nCntUav].bUse = true;
			pVtx += 4;
			pVtx[0].pos = D3DXVECTOR3((float)(g_aUav[nCntUav].pos.x + 80.0f - SHADOW_UAV_WIDTH / 2), (float)(g_aUav[nCntUav].pos.y + 80.0f - SHADOW_UAV_HEIGHT / 2), 0.0f);
			pVtx[1].pos = D3DXVECTOR3((float)(g_aUav[nCntUav].pos.x + 80.0f + SHADOW_UAV_WIDTH / 2), (float)(g_aUav[nCntUav].pos.y + 80.0f - SHADOW_UAV_HEIGHT / 2), 0.0f);
			pVtx[2].pos = D3DXVECTOR3((float)(g_aUav[nCntUav].pos.x + 80.0f - SHADOW_UAV_WIDTH / 2), (float)(g_aUav[nCntUav].pos.y + 80.0f + SHADOW_UAV_HEIGHT / 2), 0.0f);
			pVtx[3].pos = D3DXVECTOR3((float)(g_aUav[nCntUav].pos.x + 80.0f + SHADOW_UAV_WIDTH / 2), (float)(g_aUav[nCntUav].pos.y + 80.0f + SHADOW_UAV_HEIGHT / 2), 0.0f);
			g_pVtxBuffUav->Unlock();
		}
	}
}

void ResetUav(void)
{
	int nCntUav;
	for (nCntUav = 0; nCntUav < MAX_UAV; nCntUav++)
	{
		g_aUav[nCntUav].bUse = false;
	}
}