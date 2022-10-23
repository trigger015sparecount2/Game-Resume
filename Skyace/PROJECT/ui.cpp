//-------------------------
//Author:三上航世
//UI処理(ui.cpp)
//-------------------------
#include "ui.h"
#include "aircraft.h"
#include "enemy.h"

//マクロ定義
#define MAX_NUMBER (8)      //桁数
#define NUMBER_PATTERN (10) //数字のパターン数
#define NUMBER_WIDTH (40)   //数字の高さ
#define NUMBER_HEIGHT (60)  //数字の幅

//グローバル変数
LPDIRECT3DTEXTURE9 g_pTextureUi[13] = {};    //テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffUi = NULL; //頂点バッファへのポインタ
D3DXVECTOR3 g_aPos[9 + 4 + 1 + 1 + 3 + 4];   //各ポリゴンの位置(スコア,撃墜数,四角,HUD戦闘機,損傷度,特殊兵装)
int g_aUIclear[3];
int g_nATime;
int g_nScore = 0; //スコア
int g_nkill = 0; //撃墜数
int g_nReScore; //やり直し時のスコア
int g_nReKill;  //やり直し時の撃墜数

//初期化処理
void InitUi(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	int nCntUi = 0;
	int nCntcol;
	pDevice = GetDevice();
	for (nCntcol = 0; nCntcol < 3; nCntcol++)
	{
		g_aUIclear[nCntcol] = 200;
	}
	g_aPos[nCntUi] = D3DXVECTOR3(155.0f, 55.0f, 0.0f);//スコア四角
	for (nCntUi++; nCntUi < 9; nCntUi++)//スコア数値
	{
		g_aPos[nCntUi] = D3DXVECTOR3(45.0f + (nCntUi - 1) * 30.0f, 55.0f, 0.0f);
	}
	g_aPos[nCntUi] = D3DXVECTOR3(1200.0f, 50.0f, 0.0f);//撃墜数四角
	for (nCntUi++; nCntUi < 13; nCntUi++)//撃墜数数値
	{
		g_aPos[nCntUi] = D3DXVECTOR3(1170.0f + (nCntUi - 10) * 30.0f, 50.0f, 0.0f);
	}
	g_aPos[nCntUi] = D3DXVECTOR3(1180.0f, 540.0f, 0.0f);//戦闘機関連四角
	nCntUi++;
	g_aPos[nCntUi] = D3DXVECTOR3(1180.0f, 460.0f, 0.0f);//HUD戦闘機
	for (nCntUi++; nCntUi < 17; nCntUi++)//機体の損傷度(数値)
	{
		g_aPos[nCntUi] = D3DXVECTOR3(1140.0f + (nCntUi - 15) * 30.0f, 560.0f, 0.0f);
	}
	g_aPos[nCntUi] = D3DXVECTOR3(1210.0f, 560.0f, 0.0f);//「%」マーク
	nCntUi++;
	g_aPos[nCntUi] = D3DXVECTOR3(1130.0f, 650.0f, 0.0f);//HUD特殊兵装
	nCntUi++;
	g_aPos[nCntUi] = D3DXVECTOR3(1175.0f, 630.0f, 0.0f);//特殊兵装残弾数
	nCntUi++;
	g_aPos[nCntUi] = D3DXVECTOR3(1203.0f, 650.0f, 0.0f);//「/」マーク
	nCntUi++;
	g_aPos[nCntUi] = D3DXVECTOR3(1233.0f, 660.0f, 0.0f);//特殊兵装最大数
	g_nScore = 0;
	g_nkill = 0;
	g_nATime = 0;
	g_nReKill = 0;
	g_nReScore = 0;
	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/Texture/number.png",
		&g_pTextureUi[0]);
	D3DXCreateTextureFromFile(pDevice,
		"data/Texture/HUDeagle.png",
		&g_pTextureUi[1]);
	D3DXCreateTextureFromFile(pDevice,
		"data/Texture/HUDflanker.png",
		&g_pTextureUi[2]);
	D3DXCreateTextureFromFile(pDevice,
		"data/Texture/HUDuav.png",
		&g_pTextureUi[3]);
	D3DXCreateTextureFromFile(pDevice,
		"data/Texture/HUDmissile.png",
		&g_pTextureUi[4]);
	D3DXCreateTextureFromFile(pDevice,
		"data/Texture/HUDbarrier.png",
		&g_pTextureUi[5]);
	D3DXCreateTextureFromFile(pDevice,
		"data/Texture/HUDrailgun.png",
		&g_pTextureUi[6]);
	D3DXCreateTextureFromFile(pDevice,
		"data/Texture/HUDbombmissile.png",
		&g_pTextureUi[7]);
	D3DXCreateTextureFromFile(pDevice,
		"data/Texture/HUDlaser.png",
		&g_pTextureUi[8]);
	D3DXCreateTextureFromFile(pDevice,
		"data/Texture/%mark.png",
		&g_pTextureUi[9]);
	D3DXCreateTextureFromFile(pDevice,
		"data/Texture/slashmark.png",
		&g_pTextureUi[10]);
	D3DXCreateTextureFromFile(pDevice,
		"data/Texture/縦長四角.png",
		&g_pTextureUi[11]);
	D3DXCreateTextureFromFile(pDevice,
		"data/Texture/横長四角.png",
		&g_pTextureUi[12]);
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * 22,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffUi,
		NULL);
	VERTEX_2D *pVtx;
	nCntUi = 0;
	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffUi->Lock(0, 0, (void **)&pVtx, 0);
	//スコア四角
	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(g_aPos[nCntUi].x - 150, g_aPos[nCntUi].y - 50, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_aPos[nCntUi].x + 150, g_aPos[nCntUi].y - 50, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_aPos[nCntUi].x - 150, g_aPos[nCntUi].y + 50, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_aPos[nCntUi].x + 150, g_aPos[nCntUi].y + 50, 0.0f);
	//rhwの設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;
	//頂点カラー
	pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 200);
	pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 200);
	pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 200);
	pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 200);
	//テクスチャ座標
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
	pVtx += 4;
	for (nCntUi++; nCntUi < MAX_NUMBER + 1; nCntUi++)//スコア数値
	{
		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(g_aPos[nCntUi].x - NUMBER_WIDTH / 2, g_aPos[nCntUi].y - NUMBER_HEIGHT / 2, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aPos[nCntUi].x + NUMBER_WIDTH / 2, g_aPos[nCntUi].y - NUMBER_HEIGHT / 2, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aPos[nCntUi].x - NUMBER_WIDTH / 2, g_aPos[nCntUi].y + NUMBER_HEIGHT / 2, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aPos[nCntUi].x + NUMBER_WIDTH / 2, g_aPos[nCntUi].y + NUMBER_HEIGHT / 2, 0.0f);
		//rhwの設定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;
		//頂点カラー
		pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 200);
		pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 200);
		pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 200);
		pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 200);
		//テクスチャ座標
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.1f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f, 1.0f);
		pVtx += 4;
	}
	//撃墜数四角
	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(g_aPos[nCntUi].x - 70, g_aPos[nCntUi].y - 45, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_aPos[nCntUi].x + 70, g_aPos[nCntUi].y - 45, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_aPos[nCntUi].x - 70, g_aPos[nCntUi].y + 45, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_aPos[nCntUi].x + 70, g_aPos[nCntUi].y + 45, 0.0f);
	//rhwの設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;
	//頂点カラー
	pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 200);
	pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 200);
	pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 200);
	pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 200);
	//テクスチャ座標
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
	pVtx += 4;
	for (nCntUi++; nCntUi < 13; nCntUi++)//撃墜数数値
	{
		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(g_aPos[nCntUi].x - NUMBER_WIDTH / 2, g_aPos[nCntUi].y - NUMBER_HEIGHT / 2, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aPos[nCntUi].x + NUMBER_WIDTH / 2, g_aPos[nCntUi].y - NUMBER_HEIGHT / 2, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aPos[nCntUi].x - NUMBER_WIDTH / 2, g_aPos[nCntUi].y + NUMBER_HEIGHT / 2, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aPos[nCntUi].x + NUMBER_WIDTH / 2, g_aPos[nCntUi].y + NUMBER_HEIGHT / 2, 0.0f);
		//rhwの設定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;
		//頂点カラー
		pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 200);
		pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 200);
		pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 200);
		pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 200);
		//テクスチャ座標
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.1f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f, 1.0f);
		pVtx += 4;
	}
	//戦闘機関連四角
	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(g_aPos[nCntUi].x - 90, g_aPos[nCntUi].y - 170, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_aPos[nCntUi].x + 90, g_aPos[nCntUi].y - 170, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_aPos[nCntUi].x - 90, g_aPos[nCntUi].y + 170, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_aPos[nCntUi].x + 90, g_aPos[nCntUi].y + 170, 0.0f);
	//rhwの設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;
	//頂点カラー
	pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 200);
	pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 200);
	pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 200);
	pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 200);
	//テクスチャ座標
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
	pVtx += 4;
	nCntUi++;
	//HUD戦闘機
	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(g_aPos[nCntUi].x - AIRCRAFT_WIDTH / 2, g_aPos[nCntUi].y - AIRCRAFT_HEIGHT / 2, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_aPos[nCntUi].x + AIRCRAFT_WIDTH / 2, g_aPos[nCntUi].y - AIRCRAFT_HEIGHT / 2, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_aPos[nCntUi].x - AIRCRAFT_WIDTH / 2, g_aPos[nCntUi].y + AIRCRAFT_HEIGHT / 2, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_aPos[nCntUi].x + AIRCRAFT_WIDTH / 2, g_aPos[nCntUi].y + AIRCRAFT_HEIGHT / 2, 0.0f);
	//rhwの設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;
	//頂点カラー
	pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 200);
	pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 200);
	pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 200);
	pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 200);
	//テクスチャ座標
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
	pVtx += 4;
	for (nCntUi++; nCntUi < 17; nCntUi++)//機体の損傷度
	{
		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(g_aPos[nCntUi].x - NUMBER_WIDTH / 2, g_aPos[nCntUi].y - NUMBER_HEIGHT / 2, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aPos[nCntUi].x + NUMBER_WIDTH / 2, g_aPos[nCntUi].y - NUMBER_HEIGHT / 2, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aPos[nCntUi].x - NUMBER_WIDTH / 2, g_aPos[nCntUi].y + NUMBER_HEIGHT / 2, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aPos[nCntUi].x + NUMBER_WIDTH / 2, g_aPos[nCntUi].y + NUMBER_HEIGHT / 2, 0.0f);
		//rhwの設定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;
		//頂点カラー
		pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 200);
		pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 200);
		pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 200);
		pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 200);
		//テクスチャ座標
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
		pVtx += 4;
	}
	//「%」マーク
	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(g_aPos[nCntUi].x - 25, g_aPos[nCntUi].y - 25, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_aPos[nCntUi].x + 25, g_aPos[nCntUi].y - 25, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_aPos[nCntUi].x - 25, g_aPos[nCntUi].y + 25, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_aPos[nCntUi].x + 25, g_aPos[nCntUi].y + 25, 0.0f);
	//rhwの設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;
	//頂点カラー
	pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 200);
	pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 200);
	pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 200);
	pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 200);
	//テクスチャ座標
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
	pVtx += 4;
	nCntUi++;
	//特殊兵装HUD
	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(g_aPos[nCntUi].x - 27, g_aPos[nCntUi].y - 24, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_aPos[nCntUi].x + 27, g_aPos[nCntUi].y - 24, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_aPos[nCntUi].x - 27, g_aPos[nCntUi].y + 24, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_aPos[nCntUi].x + 27, g_aPos[nCntUi].y + 24, 0.0f);
	//rhwの設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;
	//頂点カラー
	pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 200);
	pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 200);
	pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 200);
	pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 200);
	//テクスチャ座標
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(0.5f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(0.5f, 1.0f);
	pVtx += 4;
	//特殊兵装残弾数
	nCntUi++;
	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(g_aPos[nCntUi].x - (NUMBER_WIDTH / 2 + 5), g_aPos[nCntUi].y - (NUMBER_HEIGHT / 2 + 5), 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_aPos[nCntUi].x + (NUMBER_WIDTH / 2 + 5), g_aPos[nCntUi].y - (NUMBER_HEIGHT / 2 + 5), 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_aPos[nCntUi].x - (NUMBER_WIDTH / 2 + 5), g_aPos[nCntUi].y + (NUMBER_HEIGHT / 2 + 5), 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_aPos[nCntUi].x + (NUMBER_WIDTH / 2 + 5), g_aPos[nCntUi].y + (NUMBER_HEIGHT / 2 + 5), 0.0f);
	//rhwの設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;
	//頂点カラー
	pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 200);
	pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 200);
	pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 200);
	pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 200);
	//テクスチャ座標
	pVtx[0].tex = D3DXVECTOR2(0.2f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(0.3f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.2f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(0.3f, 1.0f);
	pVtx += 4;
	//「/」マーク
	nCntUi++;
	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(g_aPos[nCntUi].x - 12, g_aPos[nCntUi].y - 24, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_aPos[nCntUi].x + 12, g_aPos[nCntUi].y - 24, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_aPos[nCntUi].x - 12, g_aPos[nCntUi].y + 24, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_aPos[nCntUi].x + 12, g_aPos[nCntUi].y + 24, 0.0f);
	//rhwの設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;
	//頂点カラー
	pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 200);
	pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 200);
	pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 200);
	pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 200);
	//テクスチャ座標
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
	pVtx += 4;
	//特殊兵装最大数(6)
	nCntUi++;
	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(g_aPos[nCntUi].x - NUMBER_WIDTH / 2, g_aPos[nCntUi].y - NUMBER_HEIGHT / 2, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_aPos[nCntUi].x + NUMBER_WIDTH / 2, g_aPos[nCntUi].y - NUMBER_HEIGHT / 2, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_aPos[nCntUi].x - NUMBER_WIDTH / 2, g_aPos[nCntUi].y + NUMBER_HEIGHT / 2, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_aPos[nCntUi].x + NUMBER_WIDTH / 2, g_aPos[nCntUi].y + NUMBER_HEIGHT / 2, 0.0f);
	//rhwの設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;
	//頂点カラー
	pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 200);
	pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 200);
	pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 200);
	pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 200);
	//テクスチャ座標
	pVtx[0].tex = D3DXVECTOR2(0.6f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(0.7f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.6f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(0.7f, 1.0f);
	//頂点バッファをアンロックする
	g_pVtxBuffUi->Unlock();
}

//UIの終了処理
void UninitUi(void)
{
	int nCntUi;
	for (nCntUi = 0; nCntUi < 13; nCntUi++)
	{
		//テクスチャの破棄
		if (g_pTextureUi[nCntUi] != NULL)
		{
			g_pTextureUi[nCntUi]->Release();
			g_pTextureUi[nCntUi] = NULL;
		}
	}
	//頂点バッファの破棄
	if (g_pVtxBuffUi != NULL)
	{
		g_pVtxBuffUi->Release();
		g_pVtxBuffUi = NULL;
	}
}

//UIの更新処理
void UpdateUi(void)
{
	int nCntUi;
	int nNumber;
	int nRank;
	VERTEX_2D *pVtx;
	g_pVtxBuffUi->Lock(0, 0, (void **)&pVtx, 0);
	Aircraft *pAircraft;
	pAircraft = GetAircraft();
	if (pAircraft->nLife == 1)
	{
		g_nATime++;
	}
	//スコア四角
	if (pAircraft->pos.x - (AIRCRAFT_WIDTH / 2) - 150 <= g_aPos[0].x && g_aPos[0].x <= pAircraft->pos.x + (AIRCRAFT_WIDTH / 2) + 150 &&
		pAircraft->pos.y - (AIRCRAFT_HEIGHT / 2) - 50 <= g_aPos[0].y && g_aPos[0].y <= pAircraft->pos.y + (AIRCRAFT_HEIGHT / 2) + 50) //自機が近づいたら透ける
	{
		g_aUIclear[0] -= 7;
		if (g_aUIclear[0] <= 60)
		{
			g_aUIclear[0] = 60;
		}
	}
	else
	{
		g_aUIclear[0] += 7;
		if (g_aUIclear[0] >= 200)
		{
			g_aUIclear[0] = 200;
		}
	}
	pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, g_aUIclear[0]);
	pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, g_aUIclear[0]);
	pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, g_aUIclear[0]);
	pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, g_aUIclear[0]);
	pVtx += 4;
	for (nCntUi = 1; nCntUi < MAX_NUMBER + 1; nCntUi++)//スコア数値
	{
		nRank = pow(10, MAX_NUMBER + 1 - nCntUi);
		nNumber = g_nScore % nRank / (nRank / 10);
		pVtx[0].tex = D3DXVECTOR2(nNumber * 0.1f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(nNumber * 0.1f + 0.1f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(nNumber * 0.1f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(nNumber * 0.1f + 0.1f, 1.0f);
		pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, g_aUIclear[0]);
		pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, g_aUIclear[0]);
		pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, g_aUIclear[0]);
		pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, g_aUIclear[0]);
		pVtx += 4;
	}
	//撃墜数四角
	if (pAircraft->pos.x - (AIRCRAFT_WIDTH / 2) - 70 <= g_aPos[9].x && g_aPos[9].x <= pAircraft->pos.x + (AIRCRAFT_WIDTH / 2) + 70 &&
		pAircraft->pos.y - (AIRCRAFT_HEIGHT / 2) - 45 <= g_aPos[9].y && g_aPos[9].y <= pAircraft->pos.y + (AIRCRAFT_HEIGHT / 2) + 45) //自機が近づいたら透ける
	{
		g_aUIclear[1] -= 7;
		if (g_aUIclear[1] <= 60)
		{
			g_aUIclear[1] = 60;
		}
	}
	else
	{
		g_aUIclear[1] += 7;
		if (g_aUIclear[1] >= 200)
		{
			g_aUIclear[1] = 200;
		}
	}
	pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, g_aUIclear[1]);
	pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, g_aUIclear[1]);
	pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, g_aUIclear[1]);
	pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, g_aUIclear[1]);
	pVtx += 4;
	for (nCntUi = 10; nCntUi < 13; nCntUi++)//撃墜数数値
	{
		nRank = pow(10, 13 - nCntUi);
		nNumber = g_nkill % nRank / (nRank / 10);
		pVtx[0].tex = D3DXVECTOR2(nNumber * 0.1f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(nNumber * 0.1f + 0.1f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(nNumber * 0.1f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(nNumber * 0.1f + 0.1f, 1.0f);
		pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, g_aUIclear[1]);
		pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, g_aUIclear[1]);
		pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, g_aUIclear[1]);
		pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, g_aUIclear[1]);
		pVtx += 4;
	}
	//戦闘機関連四角
	if (pAircraft->pos.x - (AIRCRAFT_WIDTH / 2) - 90 <= g_aPos[13].x && g_aPos[13].x <= pAircraft->pos.x + (AIRCRAFT_WIDTH / 2) + 90 &&
		pAircraft->pos.y - (AIRCRAFT_HEIGHT / 2) - 170 <= g_aPos[13].y && g_aPos[13].y <= pAircraft->pos.y + (AIRCRAFT_HEIGHT / 2) + 170) //自機が近づいたら透ける
	{
		g_aUIclear[2] -= 7;
		if (g_aUIclear[2] <= 60)
		{
			g_aUIclear[2] = 60;
		}
	}
	else
	{
		g_aUIclear[2] += 7;
		if (g_aUIclear[2] >= 200)
		{
			g_aUIclear[2] = 200;
		}
	}
	pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, g_aUIclear[2]);
	pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, g_aUIclear[2]);
	pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, g_aUIclear[2]);
	pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, g_aUIclear[2]);
	pVtx += 4;
	//HUD戦闘機
	if (pAircraft->nLife > 0)
	{
		pVtx[0].tex = D3DXVECTOR2((pAircraft->nMaxLife - pAircraft->nLife) * (1.0f / pAircraft->nMaxLife), 0.0f);
		pVtx[1].tex = D3DXVECTOR2((pAircraft->nMaxLife - pAircraft->nLife + 1.0f) * (1.0f / pAircraft->nMaxLife), 0.0f);
		pVtx[2].tex = D3DXVECTOR2((pAircraft->nMaxLife - pAircraft->nLife) * (1.0f / pAircraft->nMaxLife), 1.0f);
		pVtx[3].tex = D3DXVECTOR2((pAircraft->nMaxLife - pAircraft->nLife + 1.0f) * (1.0f / pAircraft->nMaxLife), 1.0f);
	}
	else
	{
		pVtx[0].tex = D3DXVECTOR2((pAircraft->nMaxLife - (pAircraft->nLife + 1)) * (1.0f / pAircraft->nMaxLife), 0.0f);
		pVtx[1].tex = D3DXVECTOR2((pAircraft->nMaxLife - (pAircraft->nLife + 1) + 1.0f) * (1.0f / pAircraft->nMaxLife), 0.0f);
		pVtx[2].tex = D3DXVECTOR2((pAircraft->nMaxLife - (pAircraft->nLife + 1)) * (1.0f / pAircraft->nMaxLife), 1.0f);
		pVtx[3].tex = D3DXVECTOR2((pAircraft->nMaxLife - (pAircraft->nLife + 1) + 1.0f) * (1.0f / pAircraft->nMaxLife), 1.0f);
	}
	if (g_nATime < 6)
	{
		pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, g_aUIclear[2]);
		pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, g_aUIclear[2]);
		pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, g_aUIclear[2]);
		pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, g_aUIclear[2]);
	}
	else
	{
		pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 0);
		pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 0);
		pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 0);
		pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 0);
		if (g_nATime >= 10)
		{
			g_nATime = 0;
		}
	}
	pVtx += 4;
	for (nCntUi = 15; nCntUi < 17; nCntUi++)//機体損傷度数値
	{
		nRank = pow(10, 17 - nCntUi);
		nNumber = (100 / pAircraft->nMaxLife) * (pAircraft->nMaxLife - pAircraft->nLife) % nRank / (nRank / 10);
		pVtx[0].tex = D3DXVECTOR2(nNumber * 0.1f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(nNumber * 0.1f + 0.1f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(nNumber * 0.1f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(nNumber * 0.1f + 0.1f, 1.0f);
		pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, g_aUIclear[2]);
		pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, g_aUIclear[2]);
		pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, g_aUIclear[2]);
		pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, g_aUIclear[2]);
		pVtx += 4;
	}
	nCntUi++;
	//「%」マーク
	pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, g_aUIclear[2]);
	pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, g_aUIclear[2]);
	pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, g_aUIclear[2]);
	pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, g_aUIclear[2]);
	pVtx += 4;
	//HUD特殊兵装
	if (pAircraft->nReload == 0 && pAircraft->nSpecial > 0)
	{
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.5f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.5f, 1.0f);
	}
	else
	{
		pVtx[0].tex = D3DXVECTOR2(0.5f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.5f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
	}
	pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, g_aUIclear[2]);
	pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, g_aUIclear[2]);
	pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, g_aUIclear[2]);
	pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, g_aUIclear[2]);
	pVtx += 4;
	//特殊兵装残弾数
	nNumber = pAircraft->nSpecial;
	pVtx[0].tex = D3DXVECTOR2(nNumber * 0.1f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(nNumber * 0.1f + 0.1f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(nNumber * 0.1f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(nNumber * 0.1f + 0.1f, 1.0f);
	pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, g_aUIclear[2]);
	pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, g_aUIclear[2]);
	pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, g_aUIclear[2]);
	pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, g_aUIclear[2]);
	pVtx += 4;
	//「/」マーク
	pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, g_aUIclear[2]);
	pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, g_aUIclear[2]);
	pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, g_aUIclear[2]);
	pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, g_aUIclear[2]);
	pVtx += 4;
	//特殊兵装最大数
	pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, g_aUIclear[2]);
	pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, g_aUIclear[2]);
	pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, g_aUIclear[2]);
	pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, g_aUIclear[2]);
	g_pVtxBuffUi->Unlock();
}


//描画処理
void DrawUi(void)
{
	LPDIRECT3DDEVICE9 pDevice;                    //デバイスのポインタ
	int nCntUi = 0;
	Aircraft *pAircraft;
	pAircraft = GetAircraft();
	pDevice = GetDevice();                        //デバイスを取得する
	//頂点バッファをデバイスのデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffUi, 0, sizeof(VERTEX_2D));
	pDevice->SetFVF(FVF_VERTEX_2D);               //頂点フォーマットの設定
	pDevice->SetTexture(0, g_pTextureUi[12]);//スコア四角
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
		nCntUi * 4,
		2);
	pDevice->SetTexture(0, g_pTextureUi[0]);//テクスチャの設定
	for (nCntUi++; nCntUi < 9; nCntUi++)//スコア
	{
		//ポリゴン描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
			nCntUi * 4,
			2);
	}
	pDevice->SetTexture(0, g_pTextureUi[12]);//撃墜数四角
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
		nCntUi * 4,
		2);
	pDevice->SetTexture(0, g_pTextureUi[0]);//テクスチャの設定
	for (nCntUi++; nCntUi < 13; nCntUi++)//撃墜数
	{
		//ポリゴン描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
			nCntUi * 4,
			2);
	}
	pDevice->SetTexture(0, g_pTextureUi[11]);//戦闘機関連四角
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
		nCntUi * 4,
		2);
	nCntUi++;
	pDevice->SetTexture(0, g_pTextureUi[pAircraft->nAType + 1]);//HUD戦闘機
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
		nCntUi * 4,
		2);
	pDevice->SetTexture(0, g_pTextureUi[0]);
	for (nCntUi++; nCntUi < 17; nCntUi++)//機体の損傷度
	{
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
			nCntUi * 4,
			2);
	}
	pDevice->SetTexture(0, g_pTextureUi[9]);//「%」マーク
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
		nCntUi * 4,
		2);
	nCntUi++;
	pDevice->SetTexture(0, g_pTextureUi[pAircraft->nAType * 3 + pAircraft->nSType + 3]);//HUD特殊兵装
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
		nCntUi * 4,
		2);
	nCntUi++;
	pDevice->SetTexture(0, g_pTextureUi[0]);//特殊兵装残弾数
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
		nCntUi * 4,
		2);
	nCntUi++;
	pDevice->SetTexture(0, g_pTextureUi[10]);//「/」マーク
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
		nCntUi * 4,
		2);
	nCntUi++;
	pDevice->SetTexture(0, g_pTextureUi[0]);//特殊兵装最大数
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
		nCntUi * 4,
		2);
}

//スコア加算
void AddScore(int nValue)
{
	g_nScore += nValue;
}

void AddKill(void)
{
	g_nkill++;
}

void ResetUi(void)
{
	int nCntClear;
	g_nScore = g_nReScore;
	g_nkill = g_nReKill;
	for (nCntClear = 0; nCntClear < 3; nCntClear++)
	{
		g_aUIclear[nCntClear] = 200;
	}
	g_nATime = 0;
}

int Score(void)
{
	return g_nScore;
}

int Kill(void)
{
	return g_nkill;
}

void NextStage(void)
{
	int nCntClear;
	g_nReScore = g_nScore;
	g_nReKill = g_nkill;
	for (nCntClear = 0; nCntClear < 3; nCntClear++)
	{
		g_aUIclear[nCntClear] = 200;
	}
	g_nATime = 0;
}