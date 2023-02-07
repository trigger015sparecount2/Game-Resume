//-------------------------
//Author:三上航世
//リザルト画面UI処理(resultui.cpp)
//-------------------------
#include "resultui.h"
#include "rank.h"
#include "gameui.h"
#include "result.h"

//グローバル変数
LPDIRECT3DTEXTURE9 g_pTextureresultUi[12] = {};    //テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffresultUi = NULL; //頂点バッファへのポインタ
D3DXVECTOR3 g_aresultPos[3 + 5 + (5 * 6) + 1];   //各ポリゴンの位置(文字、今プレイのクリア時間、各順位のクリア時間、pressenter)
int g_nClearTime;
int g_nPressClearresult;
bool g_bPressClearresult;

//初期化処理
void InitresultUi(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	int nCntUi = 0;
	int nCntrank;
	int nRank;
	int nNumber;
	int nCntnumber;
	pDevice = GetDevice();
	g_aresultPos[nCntUi] = D3DXVECTOR3(640.0f, 70.0f, 0.0f);//リザルト
	nCntUi++;
	g_aresultPos[nCntUi] = D3DXVECTOR3(790.0f, 150.0f, 0.0f);//クリア時間（文字）
	nCntUi++;
	g_aresultPos[nCntUi] = D3DXVECTOR3(340.0f, 230.0f, 0.0f);//you（文字）
	nCntUi++;
	for (nCntnumber = 0; nCntnumber < 5; nCntnumber++)//今プレイクリア時間
	{
		if (nCntnumber < 2)
		{
			g_aresultPos[nCntUi] = D3DXVECTOR3(730.0f + nCntnumber * 30.0f, 230.0f, 0.0f);//クリア時間（数字）
		}
		else if(nCntnumber == 2)
		{
			g_aresultPos[nCntUi] = D3DXVECTOR3(722.0f + nCntnumber * 30.0f, 230.0f, 0.0f);//コンマ
		}
		else if(nCntnumber > 2)
		{
			g_aresultPos[nCntUi] = D3DXVECTOR3(714.0f + nCntnumber * 30.0f, 230.0f, 0.0f);//コンマ秒数(数字)
		}
		nCntUi++;
	}
	for (nCntrank = 0; nCntrank < 5; nCntrank++)
	{
		g_aresultPos[nCntUi] = D3DXVECTOR3(340.0f, 330.0f + nCntrank * 80.0f, 0.0f);//順位（文字）
		nCntUi++;
		for (nCntnumber = 0; nCntnumber < 5; nCntnumber++)//残り時間
		{
			if (nCntnumber < 2)
			{
				g_aresultPos[nCntUi] = D3DXVECTOR3(730.0f + nCntnumber * 30.0f, 330.0f + nCntrank * 80.0f, 0.0f);//クリア時間（数字）
			}
			else if(nCntnumber == 2)
			{
				g_aresultPos[nCntUi] = D3DXVECTOR3(722.0f + nCntnumber * 30.0f, 330.0f + nCntrank * 80.0f, 0.0f);//コンマ
			}
			else if (nCntnumber > 2)
			{
				g_aresultPos[nCntUi] = D3DXVECTOR3(714.0f + nCntnumber * 30.0f, 330.0f + nCntrank * 80.0f, 0.0f);//コンマ秒数(数字)
			}
			nCntUi++;
		}
	}
	g_aresultPos[nCntUi] = D3DXVECTOR3(1020.0f, 690.0f, 0.0f);//press
	g_nClearTime = 0;
	g_nPressClearresult = 255;
	g_bPressClearresult = true;

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/Texture/result.png",
		&g_pTextureresultUi[0]);
	D3DXCreateTextureFromFile(pDevice,
		"data/Texture/cleartime.png",
		&g_pTextureresultUi[1]);
	D3DXCreateTextureFromFile(pDevice,
		"data/Texture/you.png",
		&g_pTextureresultUi[2]);
	D3DXCreateTextureFromFile(pDevice,
		"data/Texture/number.png",
		&g_pTextureresultUi[3]);
	D3DXCreateTextureFromFile(pDevice,
		"data/Texture/comma.png",
		&g_pTextureresultUi[4]);
	D3DXCreateTextureFromFile(pDevice,
		"data/Texture/1st.png",
		&g_pTextureresultUi[5]);
	D3DXCreateTextureFromFile(pDevice,
		"data/Texture/2nd.png",
		&g_pTextureresultUi[6]);
	D3DXCreateTextureFromFile(pDevice,
		"data/Texture/3rd.png",
		&g_pTextureresultUi[7]);
	D3DXCreateTextureFromFile(pDevice,
		"data/Texture/4th.png",
		&g_pTextureresultUi[8]);
	D3DXCreateTextureFromFile(pDevice,
		"data/Texture/5th.png",
		&g_pTextureresultUi[9]);
	D3DXCreateTextureFromFile(pDevice,
		"data/Texture/pressanybutton.png",
		&g_pTextureresultUi[10]);
	D3DXCreateTextureFromFile(pDevice,
		"data/Texture/-mark.png",
		&g_pTextureresultUi[11]);

	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * 39,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffresultUi,
		NULL);

	VERTEX_2D *pVtx;
	nCntUi = 0;
	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffresultUi->Lock(0, 0, (void **)&pVtx, 0);
	//リザルト(文字)
	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(g_aresultPos[nCntUi].x - 105, g_aresultPos[nCntUi].y - 30, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_aresultPos[nCntUi].x + 105, g_aresultPos[nCntUi].y - 30, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_aresultPos[nCntUi].x - 105, g_aresultPos[nCntUi].y + 30, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_aresultPos[nCntUi].x + 105, g_aresultPos[nCntUi].y + 30, 0.0f);

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
	nCntUi++;
	pVtx += 4;

	//クリア時間(文字)
	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(g_aresultPos[nCntUi].x - 130, g_aresultPos[nCntUi].y - 30, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_aresultPos[nCntUi].x + 130, g_aresultPos[nCntUi].y - 30, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_aresultPos[nCntUi].x - 130, g_aresultPos[nCntUi].y + 30, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_aresultPos[nCntUi].x + 130, g_aresultPos[nCntUi].y + 30, 0.0f);

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
	nCntUi++;
	pVtx += 4;

	//you(文字)
	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(g_aresultPos[nCntUi].x - 60, g_aresultPos[nCntUi].y - 30, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_aresultPos[nCntUi].x + 60, g_aresultPos[nCntUi].y - 30, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_aresultPos[nCntUi].x - 60, g_aresultPos[nCntUi].y + 30, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_aresultPos[nCntUi].x + 60, g_aresultPos[nCntUi].y + 30, 0.0f);

	//rhwの設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//頂点カラー
	if (Demo() == true)
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

	//テクスチャ座標
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
	pVtx += 4;
	nCntUi++;

	for (nCntnumber = 0; nCntnumber < 5; nCntnumber++)
	{
		//残り時間(数字)
		//頂点座標の設定
		if (nCntnumber != 2)
		{
			pVtx[0].pos = D3DXVECTOR3(g_aresultPos[nCntUi].x - 20, g_aresultPos[nCntUi].y - 30, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aresultPos[nCntUi].x + 20, g_aresultPos[nCntUi].y - 30, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aresultPos[nCntUi].x - 20, g_aresultPos[nCntUi].y + 30, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aresultPos[nCntUi].x + 20, g_aresultPos[nCntUi].y + 30, 0.0f);
		}
		else
		{
			pVtx[0].pos = D3DXVECTOR3(g_aresultPos[nCntUi].x - 12, g_aresultPos[nCntUi].y - 30, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aresultPos[nCntUi].x + 12, g_aresultPos[nCntUi].y - 30, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aresultPos[nCntUi].x - 12, g_aresultPos[nCntUi].y + 30, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aresultPos[nCntUi].x + 12, g_aresultPos[nCntUi].y + 30, 0.0f);
		}

		//rhwの設定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//頂点カラー
		if (Demo() == true)
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

		//テクスチャ座標
		if (nCntnumber < 2)
		{
			nRank = pow(10, 2 - nCntnumber);
			nNumber = TimeMin() % nRank / (nRank / 10);//残り〇秒
		}
		else if (nCntnumber > 2)
		{
			nRank = pow(10, 5 - nCntnumber);
			nNumber = floor((float)TimeSec() * 5.0f / 3.0f);
			nNumber = nNumber % nRank / (nRank / 10);
		}
		if (nCntnumber == 2 || TimeMin() == TIME_LIMIT_SEC)
		{
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
		}
		else
		{
			pVtx[0].tex = D3DXVECTOR2(nNumber * 0.1f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(nNumber * 0.1f + 0.1f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(nNumber * 0.1f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(nNumber * 0.1f + 0.1f, 1.0f);
		}
		pVtx += 4;
		nCntUi++;
	}
	for (nCntrank = 0; nCntrank < 5; nCntrank++)
	{
		for (nCntnumber = 0; nCntnumber < 5; nCntnumber++)//残り時間
		{
			if (nCntnumber == 0)
			{
				//順位(数字)
				//頂点座標の設定
				pVtx[0].pos = D3DXVECTOR3(g_aresultPos[nCntUi].x - 50, g_aresultPos[nCntUi].y - 30, 0.0f);
				pVtx[1].pos = D3DXVECTOR3(g_aresultPos[nCntUi].x + 50, g_aresultPos[nCntUi].y - 30, 0.0f);
				pVtx[2].pos = D3DXVECTOR3(g_aresultPos[nCntUi].x - 50, g_aresultPos[nCntUi].y + 30, 0.0f);
				pVtx[3].pos = D3DXVECTOR3(g_aresultPos[nCntUi].x + 50, g_aresultPos[nCntUi].y + 30, 0.0f);

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
				nCntUi++;
				pVtx += 4;
			}

			//残り時間(数字)
			//頂点座標の設定
			if (nCntnumber != 2)
			{
				pVtx[0].pos = D3DXVECTOR3(g_aresultPos[nCntUi].x - 20, g_aresultPos[nCntUi].y - 30, 0.0f);
				pVtx[1].pos = D3DXVECTOR3(g_aresultPos[nCntUi].x + 20, g_aresultPos[nCntUi].y - 30, 0.0f);
				pVtx[2].pos = D3DXVECTOR3(g_aresultPos[nCntUi].x - 20, g_aresultPos[nCntUi].y + 30, 0.0f);
				pVtx[3].pos = D3DXVECTOR3(g_aresultPos[nCntUi].x + 20, g_aresultPos[nCntUi].y + 30, 0.0f);
			}
			else
			{
				pVtx[0].pos = D3DXVECTOR3(g_aresultPos[nCntUi].x - 12, g_aresultPos[nCntUi].y - 30, 0.0f);
				pVtx[1].pos = D3DXVECTOR3(g_aresultPos[nCntUi].x + 12, g_aresultPos[nCntUi].y - 30, 0.0f);
				pVtx[2].pos = D3DXVECTOR3(g_aresultPos[nCntUi].x - 12, g_aresultPos[nCntUi].y + 30, 0.0f);
				pVtx[3].pos = D3DXVECTOR3(g_aresultPos[nCntUi].x + 12, g_aresultPos[nCntUi].y + 30, 0.0f);
			}

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
			if (nCntnumber < 2)
			{
				nRank = pow(10, 2 - nCntnumber);
				nNumber = (TRankData(nCntrank) / 60) % nRank / (nRank / 10);
			}
			else if (nCntnumber > 2)
			{
				nRank = pow(10, 5 - nCntnumber);
				nNumber = floor((float)(TRankData(nCntrank) % 60) * 5.0f / 3.0f);
				nNumber = nNumber % nRank / (nRank / 10);
			}
			if (nCntnumber == 2 || TRankData(nCntrank) == TIME_LIMIT_SEC * TIME_LIMIT_COM)
			{
				pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
				pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
				pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
				pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
			}
			else
			{
				pVtx[0].tex = D3DXVECTOR2(nNumber * 0.1f, 0.0f);
				pVtx[1].tex = D3DXVECTOR2(nNumber * 0.1f + 0.1f, 0.0f);
				pVtx[2].tex = D3DXVECTOR2(nNumber * 0.1f, 1.0f);
				pVtx[3].tex = D3DXVECTOR2(nNumber * 0.1f + 0.1f, 1.0f);
			}
			nCntUi++;
			pVtx += 4;
		}
	}
	//press
	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(g_aresultPos[nCntUi].x - 230, g_aresultPos[nCntUi].y - 25, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_aresultPos[nCntUi].x + 230, g_aresultPos[nCntUi].y - 25, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_aresultPos[nCntUi].x - 230, g_aresultPos[nCntUi].y + 25, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_aresultPos[nCntUi].x + 230, g_aresultPos[nCntUi].y + 25, 0.0f);

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

	//頂点バッファをアンロックする
	g_pVtxBuffresultUi->Unlock();
}

//UIの終了処理
void UninitresultUi(void)
{
	int nCntUi;
	for (nCntUi = 0; nCntUi < 12; nCntUi++)
	{
		//テクスチャの破棄
		if (g_pTextureresultUi[nCntUi] != NULL)
		{
			g_pTextureresultUi[nCntUi]->Release();
			g_pTextureresultUi[nCntUi] = NULL;
		}
	}
	//頂点バッファの破棄
	if (g_pVtxBuffresultUi != NULL)
	{
		g_pVtxBuffresultUi->Release();
		g_pVtxBuffresultUi = NULL;
	}
}

//UIの更新処理
void UpdateresultUi(void)
{
	//カウント進める
	g_nClearTime++;
	int nCntRank;
	int nCntTime;
	if (g_nClearTime == 3)
	{
		//対応する文字消す
		for (nCntRank = 0; nCntRank < 5; nCntRank++)
		{
			if (TRankData(nCntRank) == TimeMin() * 60 + TimeSec() && TimeMin() != TIME_LIMIT_SEC && Demo() == false)
			{
				VERTEX_2D *pVtx;
				g_pVtxBuffresultUi->Lock(0, 0, (void **)&pVtx, 0);
				pVtx += 4 * 8 + 4 * nCntRank * 6 + 4;
				for (nCntTime = 0; nCntTime < 5; nCntTime++)
				{
					pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 0);
					pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 0);
					pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 0);
					pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 0);
					pVtx += 4;
				}
				g_pVtxBuffresultUi->Unlock();
				break;
			}
		}
	}
	if (g_nClearTime == 6)
	{
		//対応する文字点ける
		g_nClearTime = 0;
		for (nCntRank = 0; nCntRank < 5; nCntRank++)
		{
			if (TRankData(nCntRank) == TimeMin() * 60 + TimeSec() && TimeMin() != TIME_LIMIT_SEC && Demo() == false)
			{
				VERTEX_2D *pVtx;
				g_pVtxBuffresultUi->Lock(0, 0, (void **)&pVtx, 0);
				pVtx += 4 * 8 + 4 * nCntRank * 6 + 4;
				for (nCntTime = 0; nCntTime < 5; nCntTime++)
				{
					pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
					pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
					pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
					pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);
					pVtx += 4;
				}
				g_pVtxBuffresultUi->Unlock();
				break;
			}
		}
	}

	//press透明度減らす
	if (g_bPressClearresult == true)
	{
		g_nPressClearresult -= 15;
		if (g_nPressClearresult <= 0)
		{
			g_nPressClearresult = 0;
			g_bPressClearresult = false;
		}
	}
	else
	{
		//press透明度加算
		g_nPressClearresult += 15;
		if (g_nPressClearresult >= 255)
		{
			g_nPressClearresult = 255;
			g_bPressClearresult = true;
		}
	}

	//頂点情報いじる
	VERTEX_2D *pVtx;
	g_pVtxBuffresultUi->Lock(0, 0, (void **)&pVtx, 0);
	pVtx += 4 * 38;
	if (ResultTime() > 300 && Demo() == false || Demo() == true)
	{
		pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, g_nPressClearresult);
		pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, g_nPressClearresult);
		pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, g_nPressClearresult);
		pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, g_nPressClearresult);
	}
	else
	{
		pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 0);
		pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 0);
		pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 0);
		pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 0);
	}
	g_pVtxBuffresultUi->Unlock();
}


//描画処理
void DrawresultUi(void)
{
	LPDIRECT3DDEVICE9 pDevice;//デバイスのポインタ
	int nCntUi = 0;
	int nCntletter;
	int nCntnumber;
	int nCntRank;
	int nRank = 0;
	int nRankLag = 1;
	int ntTime = -1;
	pDevice = GetDevice();//デバイスを取得する
	//頂点バッファをデバイスのデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffresultUi, 0, sizeof(VERTEX_2D));
	pDevice->SetFVF(FVF_VERTEX_2D);               //頂点フォーマットの設定

	//テクスチャ設定
	for (nCntletter = 0; nCntletter < 3; nCntletter++)
	{
		pDevice->SetTexture(0, g_pTextureresultUi[nCntletter]);//文字列
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
			nCntUi * 4,
			2);
		nCntUi++;
	}

	for (nCntnumber = 0; nCntnumber < 5; nCntnumber++)
	{
		if (nCntnumber != 2)
		{
			if (TimeMin() == TIME_LIMIT_SEC)
			{
				pDevice->SetTexture(0, g_pTextureresultUi[11]);
			}
			else
			{
				pDevice->SetTexture(0, g_pTextureresultUi[3]);//クリア時間（数字）
			}
		}
		else
		{
			pDevice->SetTexture(0, g_pTextureresultUi[4]);//コンマ
		}
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
			nCntUi * 4,
			2);
		nCntUi++;
	}
	for (nCntRank = 0; nCntRank < 5; nCntRank++)
	{
		for (nCntnumber = 0; nCntnumber < 5; nCntnumber++)
		{
			if (nCntnumber == 0)
			{
				if (ntTime == TRankData(nCntRank))
				{
					nRankLag++;
				}
				else
				{
					nRank += nRankLag;
					nRankLag = 1;
				}
				pDevice->SetTexture(0, g_pTextureresultUi[4 + nRank]);//クリア時間順位(数字)
				pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
					nCntUi * 4,
					2);
				nCntUi++;
			}
			if (nCntnumber != 2)
			{
				if (TRankData(nCntRank) == TIME_LIMIT_SEC * TIME_LIMIT_COM)
				{
					pDevice->SetTexture(0, g_pTextureresultUi[11]);//未クリア
				}
				else
				{
					pDevice->SetTexture(0, g_pTextureresultUi[3]);//クリア時間(数字)
				}
			}
			else
			{
				pDevice->SetTexture(0, g_pTextureresultUi[4]);//コンマ
			}
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
				nCntUi * 4,
				2);
			nCntUi++;
		}
		ntTime = TRankData(nCntRank);
	}
	pDevice->SetTexture(0, g_pTextureresultUi[10]);//press
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
		nCntUi * 4,
		2);
}