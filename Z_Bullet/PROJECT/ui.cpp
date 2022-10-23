//-------------------------
//Author:三上航世
//UI処理(ui.cpp)
//-------------------------
#include "ui.h"
#include "player.h"
#include "input.h"
#include "game.h"
#include "fade.h"

//マクロ定義
#define MAX_UI (13)

typedef struct
{
	D3DXVECTOR3 Pos; //画像の中心、貼られる場所
	int nTex;
} UI;

//グローバル変数
LPDIRECT3DTEXTURE9 g_pTextureUi[MAX_UI] = {};	 //テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffUI = NULL;//頂点バッファへのポインタ
UI g_UI[32];
bool g_bChainClear;
int g_nChainClear;
int g_nFrame;
int g_nTime;

//初期化処理
HRESULT InitUi(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = GetDevice();
	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/Texture/HPgaugeGw.png",
		&g_pTextureUi[0]);
	D3DXCreateTextureFromFile(pDevice,
		"data/Texture/HPgauge.png",
		&g_pTextureUi[1]);
	D3DXCreateTextureFromFile(pDevice,
		"data/Texture/SPgaugeGw.png",
		&g_pTextureUi[2]);
	D3DXCreateTextureFromFile(pDevice,
		"data/Texture/SPgauge.png",
		&g_pTextureUi[3]);
	D3DXCreateTextureFromFile(pDevice,
		"data/Texture/bulleticon.png",
		&g_pTextureUi[4]);
	D3DXCreateTextureFromFile(pDevice,
		"data/Texture/itemicon.png",
		&g_pTextureUi[5]);
	D3DXCreateTextureFromFile(pDevice,
		"data/Texture/slashmark.png",
		&g_pTextureUi[6]);
	D3DXCreateTextureFromFile(pDevice,
		"data/Texture/number.png",
		&g_pTextureUi[7]);
	D3DXCreateTextureFromFile(pDevice,
		"data/Texture/timer.png",
		&g_pTextureUi[8]);
	D3DXCreateTextureFromFile(pDevice,
		"data/Texture/comma.png",
		&g_pTextureUi[9]);
	D3DXCreateTextureFromFile(pDevice,
		"data/Texture/chain.png",
		&g_pTextureUi[10]);
	D3DXCreateTextureFromFile(pDevice,
		"data/Texture/bossHPgaugeGw.png",
		&g_pTextureUi[11]);
	D3DXCreateTextureFromFile(pDevice,
		"data/Texture/bossHPgauge.png",
		&g_pTextureUi[12]);

	g_bChainClear = false;
	g_nChainClear = 0;

	g_nFrame = 60;
	g_nTime = 300;

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * 32,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffUI,
		NULL);

	int nCntScore;

	//HPゲージ
	int nCntUi = 0;
	g_UI[nCntUi].Pos = D3DXVECTOR3(120.0f, 35.0f, 0.0f);
	g_UI[nCntUi].nTex = 0;

	nCntUi++;
	g_UI[nCntUi].Pos = D3DXVECTOR3(140.0f, 35.0f, 0.0f);
	g_UI[nCntUi].nTex = 1;
	
	//SPゲージ
	nCntUi++;
	g_UI[nCntUi].Pos = D3DXVECTOR3(120.0f, 85.0f, 0.0f);
	g_UI[nCntUi].nTex = 2;

	nCntUi++;
	g_UI[nCntUi].Pos = D3DXVECTOR3(140.0f, 85.0f, 0.0f);
	g_UI[nCntUi].nTex = 3;

	//銃関連
	nCntUi++;
	g_UI[nCntUi].Pos = D3DXVECTOR3(70.0f, 60.0f, 0.0f);
	g_UI[nCntUi].nTex = 4;

	//アイテム関連
	nCntUi++;
	g_UI[nCntUi].Pos = D3DXVECTOR3(88.0f, 170.0f, 0.0f);
	g_UI[nCntUi].nTex = 5;

	//残弾数
	nCntUi++;
	g_UI[nCntUi].Pos = D3DXVECTOR3(34.0f, 93.0f, 0.0f);
	g_UI[nCntUi].nTex = 7;

	nCntUi++;
	g_UI[nCntUi].Pos = D3DXVECTOR3(52.0f, 93.0f, 0.0f);
	g_UI[nCntUi].nTex = 7;

	//「/」
	nCntUi++;
	g_UI[nCntUi].Pos = D3DXVECTOR3(70.0f, 93.0f, 0.0f);
	g_UI[nCntUi].nTex = 6;

	//最大装填数
	nCntUi++;
	g_UI[nCntUi].Pos = D3DXVECTOR3(88.0f, 93.0f, 0.0f);
	g_UI[nCntUi].nTex = 7;

	nCntUi++;
	g_UI[nCntUi].Pos = D3DXVECTOR3(106.0f, 93.0f, 0.0f);
	g_UI[nCntUi].nTex = 7;

	//アイテム数
	nCntUi++;
	g_UI[nCntUi].Pos = D3DXVECTOR3(115.0f, 200.0f, 0.0f);
	g_UI[nCntUi].nTex = 7;

	//スコア四角
	nCntUi++;
	g_UI[nCntUi].Pos = D3DXVECTOR3(1080.0f, 45.0f, 0.0f);
	g_UI[nCntUi].nTex = 8;

	//スコア
	for (nCntScore = 0; nCntScore < 8; nCntScore++)
	{
		nCntUi++;
		g_UI[nCntUi].Pos = D3DXVECTOR3(940.0f + nCntScore * 40.0f, 45.0f, 0.0f);
		g_UI[nCntUi].nTex = 7;
	}

	//タイマー四角
	nCntUi++;
	g_UI[nCntUi].Pos = D3DXVECTOR3(640.0f, 50.0f, 0.0f);
	g_UI[nCntUi].nTex = 8;

	//タイマー分
	nCntUi++;
	g_UI[nCntUi].Pos = D3DXVECTOR3(580.0f, 50.0f, 0.0f);
	g_UI[nCntUi].nTex = 7;

	//コンマ
	nCntUi++;
	g_UI[nCntUi].Pos = D3DXVECTOR3(616.0f, 50.0f, 0.0f);
	g_UI[nCntUi].nTex = 9;

	//タイマー秒
	nCntUi++;
	g_UI[nCntUi].Pos = D3DXVECTOR3(652.0f, 50.0f, 0.0f);
	g_UI[nCntUi].nTex = 7;

	//タイマー秒
	nCntUi++;
	g_UI[nCntUi].Pos = D3DXVECTOR3(700.0f, 50.0f, 0.0f);
	g_UI[nCntUi].nTex = 7;

	//チェイン
	nCntUi++;
	g_UI[nCntUi].Pos = D3DXVECTOR3(970.0f, 145.0f, 0.0f);
	g_UI[nCntUi].nTex = 7;

	//チェイン
	nCntUi++;
	g_UI[nCntUi].Pos = D3DXVECTOR3(1030.0f, 145.0f, 0.0f);
	g_UI[nCntUi].nTex = 7;

	//チェイン
	nCntUi++;
	g_UI[nCntUi].Pos = D3DXVECTOR3(1090.0f, 145.0f, 0.0f);
	g_UI[nCntUi].nTex = 7;

	//チェイン文字
	nCntUi++;
	g_UI[nCntUi].Pos = D3DXVECTOR3(1180.0f, 180.0f, 0.0f);
	g_UI[nCntUi].nTex = 10;

	//ボスHP下地
	nCntUi++;
	g_UI[nCntUi].Pos = D3DXVECTOR3(1270.0f, 675.0f, 0.0f);
	g_UI[nCntUi].nTex = 11;

	//ボスHP
	nCntUi++;
	g_UI[nCntUi].Pos = D3DXVECTOR3(1188.0f, 672.5f, 0.0f);
	g_UI[nCntUi].nTex = 12;

	nCntUi = 0;
	VERTEX_2D *pVtx;//頂点情報へのポインタ

	//頂点バッファをロックし、頂点データ絵のポインタを取得
	g_pVtxBuffUI->Lock(0, 0, (void**)&pVtx, 0);
	//座標の設定(x,y)float HPゲージ
	pVtx[0].pos = D3DXVECTOR3(g_UI[nCntUi].Pos.x,          g_UI[nCntUi].Pos.y - 25.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_UI[nCntUi].Pos.x + 310.0f, g_UI[nCntUi].Pos.y - 25.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_UI[nCntUi].Pos.x,          g_UI[nCntUi].Pos.y + 25.0f, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_UI[nCntUi].Pos.x + 310.0f, g_UI[nCntUi].Pos.y + 25.0f, 0.0f);

	//rhwの設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//カラーの設定
	pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

	//テクスチャの設定(u,v)float
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
	pVtx += 4;
	nCntUi++;

	//座標の設定(x,y)float　HPゲージ
	pVtx[0].pos = D3DXVECTOR3(g_UI[nCntUi].Pos.x,		   g_UI[nCntUi].Pos.y - 24.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_UI[nCntUi].Pos.x + 289.0f, g_UI[nCntUi].Pos.y - 24.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_UI[nCntUi].Pos.x,		   g_UI[nCntUi].Pos.y + 24.0f, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_UI[nCntUi].Pos.x + 289.0f, g_UI[nCntUi].Pos.y + 24.0f, 0.0f);

	//rhwの設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//カラーの設定
	pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

	//テクスチャの設定(u,v)float
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
	pVtx += 4;
	nCntUi++;

	//座標の設定(x,y)float SPゲージ
	pVtx[0].pos = D3DXVECTOR3(g_UI[nCntUi].Pos.x,		   g_UI[nCntUi].Pos.y - 25.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_UI[nCntUi].Pos.x + 200.0f, g_UI[nCntUi].Pos.y - 25.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_UI[nCntUi].Pos.x,		   g_UI[nCntUi].Pos.y + 25.0f, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_UI[nCntUi].Pos.x + 200.0f, g_UI[nCntUi].Pos.y + 25.0f, 0.0f);

	//rhwの設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//カラーの設定
	pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

	//テクスチャの設定(u,v)float
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
	pVtx += 4;
	nCntUi++;

	//座標の設定(x,y)float SPゲージ
	pVtx[0].pos = D3DXVECTOR3(g_UI[nCntUi].Pos.x,		   g_UI[nCntUi].Pos.y - 24.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_UI[nCntUi].Pos.x + 179.0f, g_UI[nCntUi].Pos.y - 24.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_UI[nCntUi].Pos.x,		   g_UI[nCntUi].Pos.y + 24.0f, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_UI[nCntUi].Pos.x + 179.0f, g_UI[nCntUi].Pos.y + 24.0f, 0.0f);

	//rhwの設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//カラーの設定
	pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

	//テクスチャの設定(u,v)float
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
	pVtx += 4;
	nCntUi++;

	//座標の設定(x,y)float 銃弾
	pVtx[0].pos = D3DXVECTOR3(g_UI[nCntUi].Pos.x - 50.0f, g_UI[nCntUi].Pos.y - 50.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_UI[nCntUi].Pos.x + 50.0f, g_UI[nCntUi].Pos.y - 50.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_UI[nCntUi].Pos.x - 50.0f, g_UI[nCntUi].Pos.y + 50.0f, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_UI[nCntUi].Pos.x + 50.0f, g_UI[nCntUi].Pos.y + 50.0f, 0.0f);

	//rhwの設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//カラーの設定
	pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

	//テクスチャの設定(u,v)float
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
	pVtx += 4;
	nCntUi++;

	//座標の設定(x,y)float アイテム
	pVtx[0].pos = D3DXVECTOR3(g_UI[nCntUi].Pos.x - 50.0f, g_UI[nCntUi].Pos.y - 50.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_UI[nCntUi].Pos.x + 50.0f, g_UI[nCntUi].Pos.y - 50.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_UI[nCntUi].Pos.x - 50.0f, g_UI[nCntUi].Pos.y + 50.0f, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_UI[nCntUi].Pos.x + 50.0f, g_UI[nCntUi].Pos.y + 50.0f, 0.0f);

	//rhwの設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//カラーの設定
	pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

	//テクスチャの設定(u,v)float
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
	pVtx += 4;
	nCntUi++;

	//座標の設定(x,y)float　装填数
	pVtx[0].pos = D3DXVECTOR3(g_UI[nCntUi].Pos.x - 10.0f, g_UI[nCntUi].Pos.y - 15.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_UI[nCntUi].Pos.x + 10.0f, g_UI[nCntUi].Pos.y - 15.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_UI[nCntUi].Pos.x - 10.0f, g_UI[nCntUi].Pos.y + 15.0f, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_UI[nCntUi].Pos.x + 10.0f, g_UI[nCntUi].Pos.y + 15.0f, 0.0f);

	//rhwの設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//カラーの設定
	pVtx[0].col = D3DCOLOR_RGBA(0, 255, 0, 255);
	pVtx[1].col = D3DCOLOR_RGBA(0, 255, 0, 255);
	pVtx[2].col = D3DCOLOR_RGBA(0, 255, 0, 255);
	pVtx[3].col = D3DCOLOR_RGBA(0, 255, 0, 255);

	//テクスチャの設定(u,v)float
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
	pVtx += 4;
	nCntUi++;

	//座標の設定(x,y)float　装填数
	pVtx[0].pos = D3DXVECTOR3(g_UI[nCntUi].Pos.x - 10.0f, g_UI[nCntUi].Pos.y - 15.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_UI[nCntUi].Pos.x + 10.0f, g_UI[nCntUi].Pos.y - 15.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_UI[nCntUi].Pos.x - 10.0f, g_UI[nCntUi].Pos.y + 15.0f, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_UI[nCntUi].Pos.x + 10.0f, g_UI[nCntUi].Pos.y + 15.0f, 0.0f);

	//rhwの設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//カラーの設定
	pVtx[0].col = D3DCOLOR_RGBA(0, 255, 0, 255);
	pVtx[1].col = D3DCOLOR_RGBA(0, 255, 0, 255);
	pVtx[2].col = D3DCOLOR_RGBA(0, 255, 0, 255);
	pVtx[3].col = D3DCOLOR_RGBA(0, 255, 0, 255);

	//テクスチャの設定(u,v)float
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
	pVtx += 4;
	nCntUi++;

	//座標の設定(x,y)float　スラッシュ
	pVtx[0].pos = D3DXVECTOR3(g_UI[nCntUi].Pos.x - 15.0f, g_UI[nCntUi].Pos.y - 15.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_UI[nCntUi].Pos.x + 15.0f, g_UI[nCntUi].Pos.y - 15.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_UI[nCntUi].Pos.x - 15.0f, g_UI[nCntUi].Pos.y + 15.0f, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_UI[nCntUi].Pos.x + 15.0f, g_UI[nCntUi].Pos.y + 15.0f, 0.0f);

	//rhwの設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//カラーの設定
	pVtx[0].col = D3DCOLOR_RGBA(0, 255, 0, 255);
	pVtx[1].col = D3DCOLOR_RGBA(0, 255, 0, 255);
	pVtx[2].col = D3DCOLOR_RGBA(0, 255, 0, 255);
	pVtx[3].col = D3DCOLOR_RGBA(0, 255, 0, 255);

	//テクスチャの設定(u,v)float
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
	pVtx += 4;
	nCntUi++;

	//座標の設定(x,y)float　最大装填数
	pVtx[0].pos = D3DXVECTOR3(g_UI[nCntUi].Pos.x - 10.0f, g_UI[nCntUi].Pos.y - 15.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_UI[nCntUi].Pos.x + 10.0f, g_UI[nCntUi].Pos.y - 15.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_UI[nCntUi].Pos.x - 10.0f, g_UI[nCntUi].Pos.y + 15.0f, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_UI[nCntUi].Pos.x + 10.0f, g_UI[nCntUi].Pos.y + 15.0f, 0.0f);

	//rhwの設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//カラーの設定
	pVtx[0].col = D3DCOLOR_RGBA(0, 255, 0, 255);
	pVtx[1].col = D3DCOLOR_RGBA(0, 255, 0, 255);
	pVtx[2].col = D3DCOLOR_RGBA(0, 255, 0, 255);
	pVtx[3].col = D3DCOLOR_RGBA(0, 255, 0, 255);

	//テクスチャの設定(u,v)float
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
	pVtx += 4;
	nCntUi++;

	//座標の設定(x,y)float　最大装填数
	pVtx[0].pos = D3DXVECTOR3(g_UI[nCntUi].Pos.x - 10.0f, g_UI[nCntUi].Pos.y - 15.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_UI[nCntUi].Pos.x + 10.0f, g_UI[nCntUi].Pos.y - 15.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_UI[nCntUi].Pos.x - 10.0f, g_UI[nCntUi].Pos.y + 15.0f, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_UI[nCntUi].Pos.x + 10.0f, g_UI[nCntUi].Pos.y + 15.0f, 0.0f);

	//rhwの設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//カラーの設定
	pVtx[0].col = D3DCOLOR_RGBA(0, 255, 0, 255);
	pVtx[1].col = D3DCOLOR_RGBA(0, 255, 0, 255);
	pVtx[2].col = D3DCOLOR_RGBA(0, 255, 0, 255);
	pVtx[3].col = D3DCOLOR_RGBA(0, 255, 0, 255);

	//テクスチャの設定(u,v)float
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
	pVtx += 4;
	nCntUi++;

	//座標の設定(x,y)float　アイテム所持数
	pVtx[0].pos = D3DXVECTOR3(g_UI[nCntUi].Pos.x - 12.0f, g_UI[nCntUi].Pos.y - 18.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_UI[nCntUi].Pos.x + 12.0f, g_UI[nCntUi].Pos.y - 18.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_UI[nCntUi].Pos.x - 12.0f, g_UI[nCntUi].Pos.y + 18.0f, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_UI[nCntUi].Pos.x + 12.0f, g_UI[nCntUi].Pos.y + 18.0f, 0.0f);

	//rhwの設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//カラーの設定
	pVtx[0].col = D3DCOLOR_RGBA(0, 255, 0, 255);
	pVtx[1].col = D3DCOLOR_RGBA(0, 255, 0, 255);
	pVtx[2].col = D3DCOLOR_RGBA(0, 255, 0, 255);
	pVtx[3].col = D3DCOLOR_RGBA(0, 255, 0, 255);

	//テクスチャの設定(u,v)float
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
	pVtx += 4;
	nCntUi++;

	//座標の設定(x,y)float　スコア四角
	pVtx[0].pos = D3DXVECTOR3(g_UI[nCntUi].Pos.x - 180.0f, g_UI[nCntUi].Pos.y - 40.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_UI[nCntUi].Pos.x + 180.0f, g_UI[nCntUi].Pos.y - 40.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_UI[nCntUi].Pos.x - 180.0f, g_UI[nCntUi].Pos.y + 40.0f, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_UI[nCntUi].Pos.x + 180.0f, g_UI[nCntUi].Pos.y + 40.0f, 0.0f);

	//rhwの設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//カラーの設定
	pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

	//テクスチャの設定(u,v)float
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
	pVtx += 4;
	nCntUi++;

	for (nCntScore = 0; nCntScore < 8; nCntScore++)
	{
		//座標の設定(x,y)float　スコア
		pVtx[0].pos = D3DXVECTOR3(g_UI[nCntUi].Pos.x - 20.0f, g_UI[nCntUi].Pos.y - 30.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_UI[nCntUi].Pos.x + 20.0f, g_UI[nCntUi].Pos.y - 30.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_UI[nCntUi].Pos.x - 20.0f, g_UI[nCntUi].Pos.y + 30.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_UI[nCntUi].Pos.x + 20.0f, g_UI[nCntUi].Pos.y + 30.0f, 0.0f);

		//rhwの設定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//カラーの設定
		pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

		//テクスチャの設定(u,v)float
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.1f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f, 1.0f);
		pVtx += 4;
		nCntUi++;
	}

	//座標の設定(x,y)float　タイマー四角
	pVtx[0].pos = D3DXVECTOR3(g_UI[nCntUi].Pos.x - 100.0f, g_UI[nCntUi].Pos.y - 45.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_UI[nCntUi].Pos.x + 100.0f, g_UI[nCntUi].Pos.y - 45.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_UI[nCntUi].Pos.x - 100.0f, g_UI[nCntUi].Pos.y + 45.0f, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_UI[nCntUi].Pos.x + 100.0f, g_UI[nCntUi].Pos.y + 45.0f, 0.0f);

	//rhwの設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//カラーの設定
	pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

	//テクスチャの設定(u,v)float
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
	pVtx += 4;
	nCntUi++;

	//座標の設定(x,y)float　タイマー分
	pVtx[0].pos = D3DXVECTOR3(g_UI[nCntUi].Pos.x - 24.0f, g_UI[nCntUi].Pos.y - 36.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_UI[nCntUi].Pos.x + 24.0f, g_UI[nCntUi].Pos.y - 36.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_UI[nCntUi].Pos.x - 24.0f, g_UI[nCntUi].Pos.y + 36.0f, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_UI[nCntUi].Pos.x + 24.0f, g_UI[nCntUi].Pos.y + 36.0f, 0.0f);

	//rhwの設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//カラーの設定
	pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

	//テクスチャの設定(u,v)float
	pVtx[0].tex = D3DXVECTOR2(0.5f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(0.6f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.5f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(0.6f, 1.0f);
	pVtx += 4;
	nCntUi++;

	//座標の設定(x,y)float　タイマーコンマ
	pVtx[0].pos = D3DXVECTOR3(g_UI[nCntUi].Pos.x - 12.0f, g_UI[nCntUi].Pos.y - 36.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_UI[nCntUi].Pos.x + 12.0f, g_UI[nCntUi].Pos.y - 36.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_UI[nCntUi].Pos.x - 12.0f, g_UI[nCntUi].Pos.y + 36.0f, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_UI[nCntUi].Pos.x + 12.0f, g_UI[nCntUi].Pos.y + 36.0f, 0.0f);

	//rhwの設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//カラーの設定
	pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

	//テクスチャの設定(u,v)float
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
	pVtx += 4;
	nCntUi++;

	//座標の設定(x,y)float　タイマー秒
	pVtx[0].pos = D3DXVECTOR3(g_UI[nCntUi].Pos.x - 24.0f, g_UI[nCntUi].Pos.y - 36.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_UI[nCntUi].Pos.x + 24.0f, g_UI[nCntUi].Pos.y - 36.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_UI[nCntUi].Pos.x - 24.0f, g_UI[nCntUi].Pos.y + 36.0f, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_UI[nCntUi].Pos.x + 24.0f, g_UI[nCntUi].Pos.y + 36.0f, 0.0f);

	//rhwの設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//カラーの設定
	pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

	//テクスチャの設定(u,v)float
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(0.1f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(0.1f, 1.0f);
	pVtx += 4;
	nCntUi++;

	//座標の設定(x,y)float　タイマー秒
	pVtx[0].pos = D3DXVECTOR3(g_UI[nCntUi].Pos.x - 24.0f, g_UI[nCntUi].Pos.y - 36.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_UI[nCntUi].Pos.x + 24.0f, g_UI[nCntUi].Pos.y - 36.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_UI[nCntUi].Pos.x - 24.0f, g_UI[nCntUi].Pos.y + 36.0f, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_UI[nCntUi].Pos.x + 24.0f, g_UI[nCntUi].Pos.y + 36.0f, 0.0f);

	//rhwの設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//カラーの設定
	pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

	//テクスチャの設定(u,v)float
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(0.1f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(0.1f, 1.0f);
	pVtx += 4;
	nCntUi++;

	//座標の設定(x,y)float　チェイン数
	pVtx[0].pos = D3DXVECTOR3(g_UI[nCntUi].Pos.x - 30.0f, g_UI[nCntUi].Pos.y - 45.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_UI[nCntUi].Pos.x + 30.0f, g_UI[nCntUi].Pos.y - 45.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_UI[nCntUi].Pos.x - 30.0f, g_UI[nCntUi].Pos.y + 45.0f, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_UI[nCntUi].Pos.x + 30.0f, g_UI[nCntUi].Pos.y + 45.0f, 0.0f);

	//rhwの設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//カラーの設定
	pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

	//テクスチャの設定(u,v)float
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(0.1f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(0.1f, 1.0f);
	pVtx += 4;
	nCntUi++;

	//座標の設定(x,y)float　チェイン数
	pVtx[0].pos = D3DXVECTOR3(g_UI[nCntUi].Pos.x - 30.0f, g_UI[nCntUi].Pos.y - 45.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_UI[nCntUi].Pos.x + 30.0f, g_UI[nCntUi].Pos.y - 45.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_UI[nCntUi].Pos.x - 30.0f, g_UI[nCntUi].Pos.y + 45.0f, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_UI[nCntUi].Pos.x + 30.0f, g_UI[nCntUi].Pos.y + 45.0f, 0.0f);

	//rhwの設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//カラーの設定
	pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

	//テクスチャの設定(u,v)float
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(0.1f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(0.1f, 1.0f);
	pVtx += 4;
	nCntUi++;

	//座標の設定(x,y)float　チェイン数
	pVtx[0].pos = D3DXVECTOR3(g_UI[nCntUi].Pos.x - 30.0f, g_UI[nCntUi].Pos.y - 45.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_UI[nCntUi].Pos.x + 30.0f, g_UI[nCntUi].Pos.y - 45.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_UI[nCntUi].Pos.x - 30.0f, g_UI[nCntUi].Pos.y + 45.0f, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_UI[nCntUi].Pos.x + 30.0f, g_UI[nCntUi].Pos.y + 45.0f, 0.0f);

	//rhwの設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//カラーの設定
	pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

	//テクスチャの設定(u,v)float
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(0.1f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(0.1f, 1.0f);
	pVtx += 4;
	nCntUi++;

	//座標の設定(x,y)float　チェイン文字
	pVtx[0].pos = D3DXVECTOR3(g_UI[nCntUi].Pos.x - 50.0f, g_UI[nCntUi].Pos.y - 24.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_UI[nCntUi].Pos.x + 50.0f, g_UI[nCntUi].Pos.y - 24.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_UI[nCntUi].Pos.x - 50.0f, g_UI[nCntUi].Pos.y + 24.0f, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_UI[nCntUi].Pos.x + 50.0f, g_UI[nCntUi].Pos.y + 24.0f, 0.0f);

	//rhwの設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//カラーの設定
	pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

	//テクスチャの設定(u,v)float
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
	pVtx += 4;
	nCntUi++;

	//座標の設定(x,y)float　ボスHP下地
	pVtx[0].pos = D3DXVECTOR3(g_UI[nCntUi].Pos.x - 400.0f, g_UI[nCntUi].Pos.y - 35.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_UI[nCntUi].Pos.x		 , g_UI[nCntUi].Pos.y - 35.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_UI[nCntUi].Pos.x - 400.0f, g_UI[nCntUi].Pos.y + 35.0f, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_UI[nCntUi].Pos.x		 , g_UI[nCntUi].Pos.y + 35.0f, 0.0f);

	//rhwの設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//カラーの設定
	pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 0);
	pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 0);
	pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 0);
	pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 0);

	//テクスチャの設定(u,v)float
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
	pVtx += 4;
	nCntUi++;

	//座標の設定(x,y)float　ボスHPゲージ
	pVtx[0].pos = D3DXVECTOR3(g_UI[31].Pos.x - 305.0f, g_UI[31].Pos.y - 22.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_UI[31].Pos.x		 , g_UI[31].Pos.y - 22.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_UI[31].Pos.x - 305.0f, g_UI[31].Pos.y + 22.0f, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_UI[31].Pos.x		 , g_UI[31].Pos.y + 22.0f, 0.0f);

	//rhwの設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//カラーの設定
	pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 0);
	pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 0);
	pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 0);
	pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 0);

	//テクスチャの設定(u,v)float
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
	
	//頂点バッファをアンロックする
	g_pVtxBuffUI->Unlock();
	return S_OK;
}

//UIの終了処理
void UninitUi(void)
{
	int nCntTex;
	for (nCntTex = 0; nCntTex < MAX_UI; nCntTex++)
	{
		//テクスチャの破棄
		if (g_pTextureUi[nCntTex] != NULL)
		{
			g_pTextureUi[nCntTex]->Release();
			g_pTextureUi[nCntTex] = NULL;
		}
	}
	//頂点バッファの破棄
	if (g_pVtxBuffUI != NULL)
	{
		g_pVtxBuffUI->Release();
		g_pVtxBuffUI = NULL;
	}
}

//UIの更新処理
void UpdateUi(void)
{
	if (GetTuto() == false)
	{
		g_nFrame--;
	}
	if (g_nFrame == 30)
	{
		//22,23,24,25
		VERTEX_2D *pVtx;//頂点情報へのポインタ
		//頂点バッファをロックし、頂点データ絵のポインタを取得
		g_pVtxBuffUI->Lock(0, 0, (void**)&pVtx, 0);
		pVtx += 23 * 4;
		pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 0);
		pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 0);
		pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 0);
		pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 0);
		//頂点バッファをアンロックする
		g_pVtxBuffUI->Unlock();
	}
	if (g_nFrame == 0)
	{
		g_nFrame = 60;
		g_nTime--;
		int nNumber;
		int nMinute;
		int nCntTime;
		int nRank;
		nNumber = g_nTime / 60;
		VERTEX_2D *pVtx;//頂点情報へのポインタ
		//頂点バッファをロックし、頂点データ絵のポインタを取得
		g_pVtxBuffUI->Lock(0, 0, (void**)&pVtx, 0);
		pVtx += 22 * 4;
		pVtx[0].tex = D3DXVECTOR2((nNumber * 0.1f) + 0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2((nNumber * 0.1f) + 0.1f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2((nNumber * 0.1f) + 0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2((nNumber * 0.1f) + 0.1f, 1.0f);

		pVtx += 4;
		pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

		nMinute = g_nTime % 60;
		for (nCntTime = 0; nCntTime < 2; nCntTime++)
		{
			nRank = pow(10, 2 - nCntTime);
			nNumber = nMinute % nRank / (nRank / 10);
			pVtx += 4;
			pVtx[0].tex = D3DXVECTOR2((nNumber * 0.1f) + 0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2((nNumber * 0.1f) + 0.1f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2((nNumber * 0.1f) + 0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2((nNumber * 0.1f) + 0.1f, 1.0f);
		}
		//頂点バッファをアンロックする
		g_pVtxBuffUI->Unlock();
		if (g_nTime <= 0)
		{
			if (GetTuto() == false)
			{
				SetFade(MODE_RESULT);
			}
			else
			{
				SetFade();
			}
		}
	}
}


//描画処理
void DrawUi(void)
{
	int nCntUi;
	LPDIRECT3DDEVICE9 pDevice;//デバイスへのポインタ

	pDevice = GetDevice();//デバイスの取得

						  //頂点バッファをデバイスのデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffUI, 0, sizeof(VERTEX_2D));

	pDevice->SetFVF(FVF_VERTEX_2D);//頂点フォーマットの設定
	for (nCntUi = 0; nCntUi < 32; nCntUi++)
	{
		pDevice->SetTexture(0, g_pTextureUi[g_UI[nCntUi].nTex]);//テクスチャの設定
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntUi * 4, 2);//ポリゴンの描画
	}
}

void MaxBulletUI(int nMaxBullet)
{
	int nCntBullet;
	int nRank;
	int nNumber;
	for (nCntBullet = 0; nCntBullet < 2; nCntBullet++)
	{
		nRank = pow(10, 2 - nCntBullet);
		nNumber = nMaxBullet % nRank / (nRank / 10);
		VERTEX_2D *pVtx;//頂点情報へのポインタ
		//頂点バッファをロックし、頂点データ絵のポインタを取得
		g_pVtxBuffUI->Lock(0, 0, (void**)&pVtx, 0);
		pVtx += (9 + nCntBullet) * 4;
		pVtx[0].tex = D3DXVECTOR2((nNumber * 0.1f) + 0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2((nNumber * 0.1f) + 0.1f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2((nNumber * 0.1f) + 0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2((nNumber * 0.1f) + 0.1f, 1.0f);
		//頂点バッファをアンロックする
		g_pVtxBuffUI->Unlock();
	}
}

void BulletUI(int nBullet)
{
	int nCntBullet;
	int nRank;
	int nNumber;
	for (nCntBullet = 0; nCntBullet < 2; nCntBullet++)
	{
		nRank = pow(10, 2 - nCntBullet);
		nNumber = nBullet % nRank / (nRank / 10);
		VERTEX_2D *pVtx;//頂点情報へのポインタ
		//頂点バッファをロックし、頂点データ絵のポインタを取得
		g_pVtxBuffUI->Lock(0, 0, (void**)&pVtx, 0);
		pVtx += (6 + nCntBullet) * 4;
		pVtx[0].tex = D3DXVECTOR2((nNumber * 0.1f) + 0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2((nNumber * 0.1f) + 0.1f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2((nNumber * 0.1f) + 0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2((nNumber * 0.1f) + 0.1f, 1.0f);
		//頂点バッファをアンロックする
		g_pVtxBuffUI->Unlock();
	}
}

void ScoreUI(int nScore)
{
	int nCntScore;
	int nRank;
	int nNumber;
	for (nCntScore = 0; nCntScore < 8; nCntScore++)
	{
		nRank = pow(10, 8 - nCntScore);
		nNumber = nScore % nRank / (nRank / 10);
		VERTEX_2D *pVtx;//頂点情報へのポインタ
		//頂点バッファをロックし、頂点データ絵のポインタを取得
		g_pVtxBuffUI->Lock(0, 0, (void**)&pVtx, 0);
		pVtx += (13 + nCntScore) * 4;
		pVtx[0].tex = D3DXVECTOR2((nNumber * 0.1f) + 0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2((nNumber * 0.1f) + 0.1f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2((nNumber * 0.1f) + 0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2((nNumber * 0.1f) + 0.1f, 1.0f);
		//頂点バッファをアンロックする
		g_pVtxBuffUI->Unlock();
	}
}

void ItemUI(int nItem)
{
	VERTEX_2D *pVtx;//頂点情報へのポインタ
	//頂点バッファをロックし、頂点データ絵のポインタを取得
	g_pVtxBuffUI->Lock(0, 0, (void**)&pVtx, 0);
	pVtx += 11 * 4;
	pVtx[0].tex = D3DXVECTOR2((nItem * 0.1f) + 0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2((nItem * 0.1f) + 0.1f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2((nItem * 0.1f) + 0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2((nItem * 0.1f) + 0.1f, 1.0f);
	//頂点バッファをアンロックする
	g_pVtxBuffUI->Unlock();
}

void ChainUI(int nChain)
{
	int nCntChain;
	int nRank;
	int nNumber;
	bool bzero = true;
	for (nCntChain = 0; nCntChain < 3; nCntChain++)
	{
		nRank = pow(10, 3 - nCntChain);
		nNumber = nChain % nRank / (nRank / 10);
		if (nNumber > 0)
		{
			bzero = false;
		}
		VERTEX_2D *pVtx;//頂点情報へのポインタ
		//頂点バッファをロックし、頂点データ絵のポインタを取得
		g_pVtxBuffUI->Lock(0, 0, (void**)&pVtx, 0);
		pVtx += (26 + nCntChain) * 4;
		pVtx[0].tex = D3DXVECTOR2((nNumber * 0.1f) + 0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2((nNumber * 0.1f) + 0.1f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2((nNumber * 0.1f) + 0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2((nNumber * 0.1f) + 0.1f, 1.0f);

		if (bzero == true)
		{
			pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 0);
			pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 0);
			pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 0);
			pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 0);
		}
		else
		{
			pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		}
		//頂点バッファをアンロックする
		g_pVtxBuffUI->Unlock();
	}
}

void ChainTimeUI(int nChainTime)
{
	if (180 < nChainTime && nChainTime <= 360)
	{
		if (g_bChainClear == true)
		{
			g_nChainClear -= 15;
			if (g_nChainClear <= 0)
			{
				g_nChainClear = 0;
				g_bChainClear = false;
			}
		}
		else
		{
			g_nChainClear += 15;
			if (g_nChainClear >= 255)
			{
				g_nChainClear = 255;
				g_bChainClear = true;
			}
		}
	}
	if (nChainTime <= 180)
	{
		if (g_bChainClear == true)
		{
			g_nChainClear -= 30;
			if (g_nChainClear <= 0)
			{
				g_nChainClear = 0;
				g_bChainClear = false;
			}
		}
		else
		{
			g_nChainClear += 30;
			if (g_nChainClear >= 255)
			{
				g_nChainClear = 255;
				g_bChainClear = true;
			}
		}
	}
	if (nChainTime == 480)
	{
		g_nChainClear = 255;
	}
	if (nChainTime == 0)
	{
		g_nChainClear = 0;
	}
	VERTEX_2D *pVtx;//頂点情報へのポインタ
	//頂点バッファをロックし、頂点データ絵のポインタを取得
	g_pVtxBuffUI->Lock(0, 0, (void**)&pVtx, 0);
	pVtx += 29 * 4;
	pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, g_nChainClear);
	pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, g_nChainClear);
	pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, g_nChainClear);
	pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, g_nChainClear);
	//頂点バッファをアンロックする
	g_pVtxBuffUI->Unlock();
}

void HPUI(float fRate)
{
	VERTEX_2D *pVtx;//頂点情報へのポインタ
	//頂点バッファをロックし、頂点データ絵のポインタを取得
	g_pVtxBuffUI->Lock(0, 0, (void**)&pVtx, 0);
	pVtx += 4;
	pVtx[0].pos = D3DXVECTOR3(g_UI[1].Pos.x					, g_UI[1].Pos.y - 24.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_UI[1].Pos.x + 289.0f * fRate, g_UI[1].Pos.y - 24.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_UI[1].Pos.x					, g_UI[1].Pos.y + 24.0f, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_UI[1].Pos.x + 289.0f * fRate, g_UI[1].Pos.y + 24.0f, 0.0f);
	//頂点バッファをアンロックする
	g_pVtxBuffUI->Unlock();
}

void SPUI(float fRate)
{
	VERTEX_2D *pVtx;//頂点情報へのポインタ
	//頂点バッファをロックし、頂点データ絵のポインタを取得
	g_pVtxBuffUI->Lock(0, 0, (void**)&pVtx, 0);
	pVtx += 4 * 3;
	pVtx[0].pos = D3DXVECTOR3(g_UI[3].Pos.x					, g_UI[3].Pos.y - 24.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_UI[3].Pos.x + 179.0f * fRate, g_UI[3].Pos.y - 24.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_UI[3].Pos.x					, g_UI[3].Pos.y + 24.0f, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_UI[3].Pos.x + 179.0f * fRate, g_UI[3].Pos.y + 24.0f, 0.0f);
	//頂点バッファをアンロックする
	g_pVtxBuffUI->Unlock();
}

void BossHPUI(float fRate)
{
	VERTEX_2D *pVtx;//頂点情報へのポインタ
	//頂点バッファをロックし、頂点データ絵のポインタを取得
	g_pVtxBuffUI->Lock(0, 0, (void**)&pVtx, 0);
	pVtx += 4 * 31;
	pVtx[0].pos = D3DXVECTOR3(g_UI[31].Pos.x - 305.0f * fRate, g_UI[31].Pos.y - 22.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_UI[31].Pos.x				 , g_UI[31].Pos.y - 22.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_UI[31].Pos.x - 305.0f * fRate, g_UI[31].Pos.y + 22.0f, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_UI[31].Pos.x				 , g_UI[31].Pos.y + 22.0f, 0.0f);
	//頂点バッファをアンロックする
	g_pVtxBuffUI->Unlock();
}

void BossHPUISet(void)
{
	VERTEX_2D *pVtx;//頂点情報へのポインタ
					//頂点バッファをロックし、頂点データ絵のポインタを取得
	g_pVtxBuffUI->Lock(0, 0, (void**)&pVtx, 0);
	pVtx += 4 * 30;
	pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

	pVtx += 4;
	pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	//頂点バッファをアンロックする
	g_pVtxBuffUI->Unlock();
}

void ResetUI(void)
{
	g_nFrame = 60;
	g_nTime = 300;
	int nNumber;
	int nMinute;
	int nCntTime;
	int nRank;
	nNumber = g_nTime / 60;
	VERTEX_2D *pVtx;//頂点情報へのポインタ
					//頂点バッファをロックし、頂点データ絵のポインタを取得
	g_pVtxBuffUI->Lock(0, 0, (void**)&pVtx, 0);
	g_pVtxBuffUI->Lock(0, 0, (void**)&pVtx, 0);
	pVtx += 22 * 4;
	pVtx[0].tex = D3DXVECTOR2((nNumber * 0.1f) + 0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2((nNumber * 0.1f) + 0.1f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2((nNumber * 0.1f) + 0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2((nNumber * 0.1f) + 0.1f, 1.0f);
	pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

	pVtx += 4;
	pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

	nMinute = g_nTime % 60;
	for (nCntTime = 0; nCntTime < 2; nCntTime++)
	{
		nRank = pow(10, 2 - nCntTime);
		nNumber = nMinute % nRank / (nRank / 10);
		pVtx += 4;
		pVtx[0].tex = D3DXVECTOR2((nNumber * 0.1f) + 0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2((nNumber * 0.1f) + 0.1f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2((nNumber * 0.1f) + 0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2((nNumber * 0.1f) + 0.1f, 1.0f);
		pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	}
	//頂点バッファをアンロックする
	g_pVtxBuffUI->Unlock();
	//頂点バッファをアンロックする
	g_pVtxBuffUI->Unlock();
}