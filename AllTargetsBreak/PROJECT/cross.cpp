//--------------------------
//Author:三上航世
//手裏剣の処理(cross.cpp)
//--------------------------
#include "cross.h"
#include "sound.h"
#include "block.h"
#include "target.h"

//マクロ定義
#define MAX_CROSS (16)   //手裏剣の最大数
#define CROSS_SIZE (40)  //手裏剣のサイズ

//ナイフの構造体定義
typedef struct
{
	D3DXVECTOR3 pos;  //位置
	D3DXVECTOR3 posOld;
	D3DXVECTOR3 move; //移動量
	D3DXVECTOR3 BlockMove;
	CROSSROT kniferot;
	float fRot[4];
	int nTime;
	bool bLand;
	bool bLand2;
	bool bSound;
	bool bUse;        //使用しているかどうか
} Cross;

//グローバル変数
LPDIRECT3DTEXTURE9 g_pTextureKnife = NULL; //テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffKnife = NULL; //頂点バッファへのポインタ
Cross g_aKnife[MAX_CROSS];

//初期化処理
void InitCross(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	int nCntKnife;
	int nCntRot;
	pDevice = GetDevice();

	//手裏剣の初期化
	for (nCntKnife = 0; nCntKnife < MAX_CROSS; nCntKnife++)
	{
		g_aKnife[nCntKnife].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aKnife[nCntKnife].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aKnife[nCntKnife].BlockMove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aKnife[nCntKnife].posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aKnife[nCntKnife].nTime = 0;
		g_aKnife[nCntKnife].kniferot = CROSS_UP;
		g_aKnife[nCntKnife].bUse = false;
		g_aKnife[nCntKnife].bSound = false;
		for (nCntRot = 0; nCntRot < 4; nCntRot++)
		{
			g_aKnife[nCntKnife].fRot[nCntRot] = 0.0f;
		}
		g_aKnife[nCntKnife].bLand = false;
	}
	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/Texture/closs.png",
		&g_pTextureKnife);

	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_CROSS,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffKnife,
		NULL);
	VERTEX_2D *pVtx;
	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffKnife->Lock(0, 0, (void **)&pVtx, 0);
	for (nCntKnife = 0; nCntKnife < MAX_CROSS; nCntKnife++)
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
	g_pVtxBuffKnife->Unlock();
}

//手裏剣の終了処理
void UninitCross(void)
{
	//テクスチャの破棄
	if (g_pTextureKnife != NULL)
	{
		g_pTextureKnife->Release();
		g_pTextureKnife = NULL;
	}
	//頂点バッファの破棄
	if (g_pVtxBuffKnife != NULL)
	{
		g_pVtxBuffKnife->Release();
		g_pVtxBuffKnife = NULL;
	}
}

//手裏剣の更新処理
void UpdateCross(void)
{
	int nCntKnife;
	VERTEX_2D *pVtx;
	g_pVtxBuffKnife->Lock(0, 0, (void **)&pVtx, 0);
	for (nCntKnife = 0; nCntKnife < MAX_CROSS; nCntKnife++)
	{
		//使ってる
		if (g_aKnife[nCntKnife].bUse == true)
		{
			g_aKnife[nCntKnife].posOld = g_aKnife[nCntKnife].pos;
			//移動させる
			g_aKnife[nCntKnife].pos += g_aKnife[nCntKnife].move;
			int nCntRot;
			if (g_aKnife[nCntKnife].bLand2 == false) //まだ飛んでいる
			{
				//回転させる
				for (nCntRot = 0; nCntRot < 4; nCntRot++)
				{
					g_aKnife[nCntKnife].fRot[nCntRot] -= 0.1f;
					if (g_aKnife[nCntKnife].fRot[nCntRot] <= -1.0f)
					{
						g_aKnife[nCntKnife].fRot[nCntRot] += 2.0f;
					}
				}
			}
			else //どこかに刺さった
			{
				//時間がたつと使用してない状態にする
				g_aKnife[nCntKnife].nTime++;
				if (g_aKnife[nCntKnife].nTime >= 20)
				{
					g_aKnife[nCntKnife].bUse = false;
				}
			}

			//的に当たると、消す
			if (BreakTarget(g_aKnife[nCntKnife].pos, CROSS_SIZE / 2, CROSS_SIZE / 2) == true)
			{
				g_aKnife[nCntKnife].bUse = false;
				PlaySound(SOUND_LABEL_TARGETBREAK);
			}

			//ブロックとの当たり判定
			g_aKnife[nCntKnife].bLand = CollisionBlock(&g_aKnife[nCntKnife].pos, &g_aKnife[nCntKnife].posOld, &g_aKnife[nCntKnife].move, CROSS_SIZE / 2, CROSS_SIZE / 2, &g_aKnife[nCntKnife].BlockMove, 1);
			
			//当たったとき、音を鳴らす
			if (g_aKnife[nCntKnife].bLand == true)
			{
				g_aKnife[nCntKnife].bLand2 = true;
				if (g_aKnife[nCntKnife].bSound == false)
				{
					PlaySound(SOUND_LABEL_CROSSBLOCK);
					g_aKnife[nCntKnife].bSound = true;
				}
			}

			//画面外にいったら消す
			if (g_aKnife[nCntKnife].pos.x <= -CROSS_SIZE / 2) //左行った場合
			{
				g_aKnife[nCntKnife].bUse = false;
			}
			if (g_aKnife[nCntKnife].pos.y <= -CROSS_SIZE) //上行った場合
			{
				g_aKnife[nCntKnife].bUse = false;
			}
			if (g_aKnife[nCntKnife].pos.x >= SCREEN_WIDTH + (CROSS_SIZE / 2)) //右行った場合
			{
				g_aKnife[nCntKnife].bUse = false;
			}
			if (g_aKnife[nCntKnife].pos.y >= SCREEN_HEIGHT + CROSS_SIZE) //下行った場合
			{
				g_aKnife[nCntKnife].bUse = false;
			}


			pVtx[0].pos = D3DXVECTOR3(g_aKnife[nCntKnife].pos.x + (CROSS_SIZE / 2) * sinf(D3DX_PI * g_aKnife[nCntKnife].fRot[0]), g_aKnife[nCntKnife].pos.y + (CROSS_SIZE / 2) * cosf(D3DX_PI * g_aKnife[nCntKnife].fRot[0]), 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aKnife[nCntKnife].pos.x + (CROSS_SIZE / 2) * sinf(D3DX_PI * g_aKnife[nCntKnife].fRot[1]), g_aKnife[nCntKnife].pos.y + (CROSS_SIZE / 2) * cosf(D3DX_PI * g_aKnife[nCntKnife].fRot[1]), 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aKnife[nCntKnife].pos.x + (CROSS_SIZE / 2) * sinf(D3DX_PI * g_aKnife[nCntKnife].fRot[2]), g_aKnife[nCntKnife].pos.y + (CROSS_SIZE / 2) * cosf(D3DX_PI * g_aKnife[nCntKnife].fRot[2]), 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aKnife[nCntKnife].pos.x + (CROSS_SIZE / 2) * sinf(D3DX_PI * g_aKnife[nCntKnife].fRot[3]), g_aKnife[nCntKnife].pos.y + (CROSS_SIZE / 2) * cosf(D3DX_PI * g_aKnife[nCntKnife].fRot[3]), 0.0f);
		}
		pVtx += 4;
	}
	g_pVtxBuffKnife->Unlock();
}

//描画処理
void DrawCross(void)
{
	LPDIRECT3DDEVICE9 pDevice;                    //デバイスのポインタ
	int nCntKnife;
	pDevice = GetDevice();                        //デバイスを取得する
												  //頂点バッファをデバイスのデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffKnife, 0, sizeof(VERTEX_2D));
	pDevice->SetFVF(FVF_VERTEX_2D);               //頂点フォーマットの設定
	for (nCntKnife = 0; nCntKnife < CROSS_SIZE; nCntKnife++)
	{
		if (g_aKnife[nCntKnife].bUse == true)
		{
			pDevice->SetTexture(0, g_pTextureKnife);    //テクスチャの設定
			//ポリゴン描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
				nCntKnife * 4,
				2);
		}
	}
}

//手裏剣の設定処理
void SetCross(D3DXVECTOR3 pos, CROSSROT Crossrot)
{
	int nCntKnife;
	VERTEX_2D *pVtx;
	g_pVtxBuffKnife->Lock(0, 0, (void **)&pVtx, 0);
	for (nCntKnife = 0; nCntKnife < CROSS_SIZE; nCntKnife++)
	{
		if (g_aKnife[nCntKnife].bUse == false)
		{
			g_aKnife[nCntKnife].pos = D3DXVECTOR3(pos.x, pos.y - 40.0f, pos.z);
			g_aKnife[nCntKnife].bUse = true;
			g_aKnife[nCntKnife].kniferot = Crossrot;
			g_aKnife[nCntKnife].nTime = 0;
			g_aKnife[nCntKnife].bLand = false;
			g_aKnife[nCntKnife].bLand2 = false;
			g_aKnife[nCntKnife].bSound = false;
			g_aKnife[nCntKnife].BlockMove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			g_aKnife[nCntKnife].posOld = pos;
			g_aKnife[nCntKnife].fRot[0] = -0.75f;
			g_aKnife[nCntKnife].fRot[1] = 0.75f;
			g_aKnife[nCntKnife].fRot[2] = -0.25f;
			g_aKnife[nCntKnife].fRot[3] = 0.25f;
			switch (Crossrot)
			{
			case CROSS_UP:
				g_aKnife[nCntKnife].move = D3DXVECTOR3(sinf(D3DX_PI * 1.0f) * 15.0f, cosf(D3DX_PI * 1.0f) * 15.0f, 0.0f);
				break;
			case CROSS_RIGHT:
				g_aKnife[nCntKnife].move = D3DXVECTOR3(sinf(D3DX_PI * 0.5f) * 15.0f, cosf(D3DX_PI * 0.5f) * 15.0f, 0.0f);
				break;
			case CROSS_DOWN:
				g_aKnife[nCntKnife].move = D3DXVECTOR3(sinf(D3DX_PI * 0.0f) * 15.0f, cosf(D3DX_PI * 0.0f) * 15.0f, 0.0f);
				break;
			case CROSS_LEFT:
				g_aKnife[nCntKnife].move = D3DXVECTOR3(sinf(D3DX_PI * -0.5f) * 15.0f, cosf(D3DX_PI * -0.5f) * 15.0f, 0.0f);
				break;
			}
			pVtx[0].pos = D3DXVECTOR3(g_aKnife[nCntKnife].pos.x + (CROSS_SIZE / 2) * sinf(D3DX_PI * g_aKnife[nCntKnife].fRot[0]), g_aKnife[nCntKnife].pos.y + (CROSS_SIZE / 2) * cosf(D3DX_PI * g_aKnife[nCntKnife].fRot[0]), 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aKnife[nCntKnife].pos.x + (CROSS_SIZE / 2) * sinf(D3DX_PI * g_aKnife[nCntKnife].fRot[1]), g_aKnife[nCntKnife].pos.y + (CROSS_SIZE / 2) * cosf(D3DX_PI * g_aKnife[nCntKnife].fRot[1]), 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aKnife[nCntKnife].pos.x + (CROSS_SIZE / 2) * sinf(D3DX_PI * g_aKnife[nCntKnife].fRot[2]), g_aKnife[nCntKnife].pos.y + (CROSS_SIZE / 2) * cosf(D3DX_PI * g_aKnife[nCntKnife].fRot[2]), 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aKnife[nCntKnife].pos.x + (CROSS_SIZE / 2) * sinf(D3DX_PI * g_aKnife[nCntKnife].fRot[3]), g_aKnife[nCntKnife].pos.y + (CROSS_SIZE / 2) * cosf(D3DX_PI * g_aKnife[nCntKnife].fRot[3]), 0.0f);
			PlaySound(SOUND_LABEL_THROW);
			break;
		}
		pVtx += 4;
	}
	g_pVtxBuffKnife->Unlock();
}

//全部消す
void ResetCross(void)
{
	int nCntKnife;
	for (nCntKnife = 0; nCntKnife < CROSS_SIZE; nCntKnife++)
	{
		g_aKnife[nCntKnife].bUse = false;
	}
}