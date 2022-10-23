//-------------------------
//Author:三上航世
//リザルト画面UI処理(resultui.cpp)
//-------------------------
#include "resultui.h"
#include "player.h"
#include "rank.h"

#define MAX_SCORE (8)
#define MAX_RANK (5)

typedef struct
{
	D3DXVECTOR3 pos;//位置
	int nTexType;//種類
} ResultUI;

//グローバル変数
LPDIRECT3DTEXTURE9 g_pTextureresultUi[8] = {};    //テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffScoreResult = NULL;//頂点バッファへのポインタ
ResultUI g_resultui[MAX_RANK + 1][MAX_SCORE + 1];
int g_nYourRank;
int g_nRankTime;
bool g_bRankClear;

//初期化処理
void InitresultUi(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	int nCntUi;
	int nCntRank;
	int nScore;
	int nRank;
	int NumRank = 1;
	int nTScore = -1;
	int nRag = 0;
	int nNumber;
	g_bRankClear = true;
	pDevice = GetDevice();
	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/Texture/result.png",
		&g_pTextureresultUi[0]);
	D3DXCreateTextureFromFile(pDevice,
		"data/Texture/you.png",
		&g_pTextureresultUi[1]);
	D3DXCreateTextureFromFile(pDevice,
		"data/Texture/Rank1st.png",
		&g_pTextureresultUi[2]);
	D3DXCreateTextureFromFile(pDevice,
		"data/Texture/Rank2nd.png",
		&g_pTextureresultUi[3]);
	D3DXCreateTextureFromFile(pDevice,
		"data/Texture/Rank3rd.png",
		&g_pTextureresultUi[4]);
	D3DXCreateTextureFromFile(pDevice,
		"data/Texture/Rank4th.png",
		&g_pTextureresultUi[5]);
	D3DXCreateTextureFromFile(pDevice,
		"data/Texture/Rank5th.png",
		&g_pTextureresultUi[6]);
	D3DXCreateTextureFromFile(pDevice,
		"data/Texture/number.png",
		&g_pTextureresultUi[7]);

	g_nYourRank = 0;

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * (1 + (MAX_SCORE + 1) * (MAX_RANK + 1)),
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffScoreResult,
		NULL);

	VERTEX_2D *pVtx;//頂点情報へのポインタ

	//頂点バッファをロックし、頂点データ絵のポインタを取得
	g_pVtxBuffScoreResult->Lock(0, 0, (void**)&pVtx, 0);

	//テクスチャの設定(u,v)float
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//座標の設定(x,y)float
	pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(1280.0f, 0.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(0.0f, 720.0f, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(1280.0f, 720.0f, 0.0f);

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
	pVtx += 4;

	
	//テクスチャの設定(u,v)float
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//座標の中心
	g_resultui[0][0].pos = D3DXVECTOR3(300.0f, 200.0f, 0.0f);

	//座標の設定(x,y)float
	pVtx[0].pos = D3DXVECTOR3(g_resultui[0][0].pos.x - 50.0f, g_resultui[0][0].pos.y - 30.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_resultui[0][0].pos.x + 50.0f, g_resultui[0][0].pos.y - 30.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_resultui[0][0].pos.x - 50.0f, g_resultui[0][0].pos.y + 30.0f, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_resultui[0][0].pos.x + 50.0f, g_resultui[0][0].pos.y + 30.0f, 0.0f);

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

	g_resultui[0][0].nTexType = 1;

	pVtx += 4;

	nScore = PlayerScore();
	
	for (nCntUi = 0; nCntUi < MAX_SCORE; nCntUi++)
	{
		nRank = powf(10, MAX_SCORE - nCntUi);
		nNumber = nScore % nRank / (nRank / 10);

		//テクスチャの設定(u,v)float
		pVtx[0].tex = D3DXVECTOR2((nNumber * 0.1f) + 0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2((nNumber * 0.1f) + 0.1f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2((nNumber * 0.1f) + 0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2((nNumber * 0.1f) + 0.1f, 1.0f);

		//座標の中心
		g_resultui[0][nCntUi + 1].pos = D3DXVECTOR3(800.0f, 200.0f, 0.0f);

		//座標の設定(x,y)float
		pVtx[0].pos = D3DXVECTOR3(g_resultui[0][nCntUi + 1].pos.x - 20.0f + nCntUi * 40.0f, g_resultui[0][nCntUi + 1].pos.y - 30.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_resultui[0][nCntUi + 1].pos.x + 20.0f + nCntUi * 40.0f, g_resultui[0][nCntUi + 1].pos.y - 30.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_resultui[0][nCntUi + 1].pos.x - 20.0f + nCntUi * 40.0f, g_resultui[0][nCntUi + 1].pos.y + 30.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_resultui[0][nCntUi + 1].pos.x + 20.0f + nCntUi * 40.0f, g_resultui[0][nCntUi + 1].pos.y + 30.0f, 0.0f);

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

		g_resultui[0][nCntUi + 1].nTexType = 7;

		pVtx += 4;
	}
	
	for (nCntRank = 0; nCntRank < MAX_RANK; nCntRank++)
	{
		//テクスチャの設定(u,v)float
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		//座標の中心
		g_resultui[nCntRank + 1][0].pos = D3DXVECTOR3(300.0f, 220.0f + (nCntRank + 1) * 80.0f, 0.0f);

		//座標の設定(x,y)float
		pVtx[0].pos = D3DXVECTOR3(g_resultui[nCntRank + 1][0].pos.x - 50.0f, g_resultui[nCntRank + 1][0].pos.y - 30.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_resultui[nCntRank + 1][0].pos.x + 50.0f, g_resultui[nCntRank + 1][0].pos.y - 30.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_resultui[nCntRank + 1][0].pos.x - 50.0f, g_resultui[nCntRank + 1][0].pos.y + 30.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_resultui[nCntRank + 1][0].pos.x + 50.0f, g_resultui[nCntRank + 1][0].pos.y + 30.0f, 0.0f);

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

		pVtx += 4;

		nScore = SRankData(nCntRank);

		if (nScore == PlayerScore() && g_nYourRank == 0 && nScore > 0)
		{
			g_nYourRank = NumRank;
		}

		if (nScore == nTScore)
		{
			nRag++;
		}
		else
		{
			NumRank += nRag + 1;
			nRag = 0;
		}

		g_resultui[nCntRank + 1][0].nTexType = NumRank;

		for (nCntUi = 0; nCntUi < MAX_SCORE; nCntUi++)
		{
			nRank = powf(10, MAX_SCORE - nCntUi);
			nNumber = nScore % nRank / (nRank / 10);

			//テクスチャの設定(u,v)float
			pVtx[0].tex = D3DXVECTOR2((nNumber * 0.1f) + 0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2((nNumber * 0.1f) + 0.1f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2((nNumber * 0.1f) + 0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2((nNumber * 0.1f) + 0.1f, 1.0f);

			//座標の中心
			g_resultui[nCntRank + 1][nCntUi + 1].pos = D3DXVECTOR3(800.0f, 220.0f + (nCntRank + 1) * 80.0f, 0.0f);

			//座標の設定(x,y)float
			pVtx[0].pos = D3DXVECTOR3(g_resultui[nCntRank + 1][nCntUi + 1].pos.x - 20.0f + nCntUi * 40.0f, g_resultui[nCntRank + 1][nCntUi + 1].pos.y - 30.0f, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_resultui[nCntRank + 1][nCntUi + 1].pos.x + 20.0f + nCntUi * 40.0f, g_resultui[nCntRank + 1][nCntUi + 1].pos.y - 30.0f, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_resultui[nCntRank + 1][nCntUi + 1].pos.x - 20.0f + nCntUi * 40.0f, g_resultui[nCntRank + 1][nCntUi + 1].pos.y + 30.0f, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_resultui[nCntRank + 1][nCntUi + 1].pos.x + 20.0f + nCntUi * 40.0f, g_resultui[nCntRank + 1][nCntUi + 1].pos.y + 30.0f, 0.0f);

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

			g_resultui[nCntRank + 1][nCntUi + 1].nTexType = 7;

			pVtx += 4;
		}
		nTScore = nScore;
	}
	
	//頂点バッファをアンロックする
	g_pVtxBuffScoreResult->Unlock();

}

//UIの終了処理
void UninitresultUi(void)
{
	int nCntUi;
	for (nCntUi = 0; nCntUi < 8; nCntUi++)
	{
		//テクスチャの破棄
		if (g_pTextureresultUi[nCntUi] != NULL)
		{
			g_pTextureresultUi[nCntUi]->Release();
			g_pTextureresultUi[nCntUi] = NULL;
		}
	}
	//頂点バッファの破棄
	if (g_pVtxBuffScoreResult != NULL)
	{
		g_pVtxBuffScoreResult->Release();
		g_pVtxBuffScoreResult = NULL;
	}
}

//UIの更新処理
void UpdateresultUi(void)
{
	if (g_nYourRank > 0)
	{
		g_nRankTime++;
		if (g_nRankTime >= 5)
		{
			if (g_bRankClear == true)
			{
				VERTEX_2D *pVtx;//頂点情報へのポインタ

				//頂点バッファをロックし、頂点データ絵のポインタを取得
				g_pVtxBuffScoreResult->Lock(0, 0, (void**)&pVtx, 0);
				pVtx += (g_nYourRank * 9 + 1) * 4;
				//カラーの設定
				pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
				pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
				pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
				pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);
				//頂点バッファをアンロックする
				g_pVtxBuffScoreResult->Unlock();
				g_bRankClear = false;
			}
			else
			{
				VERTEX_2D *pVtx;//頂点情報へのポインタ

								//頂点バッファをロックし、頂点データ絵のポインタを取得
				g_pVtxBuffScoreResult->Lock(0, 0, (void**)&pVtx, 0);
				pVtx += (g_nYourRank * 9 + 1) * 4;
				//カラーの設定
				pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 0);
				pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 0);
				pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 0);
				pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 0);
				//頂点バッファをアンロックする
				g_pVtxBuffScoreResult->Unlock();
				g_bRankClear = true;
			}
			g_nRankTime = 0;
		}
	}
}


//描画処理
void DrawresultUi(void)
{
	int nCntScore;//for文用
	int nCntRank;
	int nCntUI = 0;

	LPDIRECT3DDEVICE9 pDevice;//デバイスへのポインタ

	pDevice = GetDevice();//デバイスの取得

	//頂点バッファをデバイスのデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffScoreResult, 0, sizeof(VERTEX_2D));

	pDevice->SetFVF(FVF_VERTEX_2D);//頂点フォーマットの設定

	pDevice->SetTexture(0, g_pTextureresultUi[0]);//テクスチャの設定
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntUI * 4, 2);//ポリゴンの描画
	nCntUI++;
	for (nCntRank = 0; nCntRank < MAX_RANK + 1; nCntRank++)
	{
		for (nCntScore = 0; nCntScore < MAX_SCORE + 1; nCntScore++)
		{
			pDevice->SetTexture(0, g_pTextureresultUi[g_resultui[nCntRank][nCntScore].nTexType]);//テクスチャの設定
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntUI * 4, 2);//ポリゴンの描画
			nCntUI++;
		}
	}
}
