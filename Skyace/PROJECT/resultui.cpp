//-------------------------
//Author:三上航世
//リザルト画面UI処理(resultui.cpp)
//-------------------------
#include "resultui.h"
#include "rank.h"
#include "ui.h"

//グローバル変数
LPDIRECT3DTEXTURE9 g_pTextureresultUi[10] = {};    //テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffresultUi = NULL; //頂点バッファへのポインタ
D3DXVECTOR3 g_aresultPos[4 + 11 + (5 * 13)];   //各ポリゴンの位置(文字、今プレイのスコア等、各順位のスコア等)

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
	g_aresultPos[nCntUi] = D3DXVECTOR3(400.0f, 170.0f, 0.0f);//スコア（文字）
	nCntUi++;
	g_aresultPos[nCntUi] = D3DXVECTOR3(880.0f, 170.0f, 0.0f);//キル（文字）
	nCntUi++;
	g_aresultPos[nCntUi] = D3DXVECTOR3(240.0f, 230.0f, 0.0f);//you（文字）
	nCntUi++;
	for (nCntnumber = 0; nCntnumber < 11; nCntnumber++)//今プレイのスコアと撃墜数
	{
		if (nCntnumber < 8)
		{
			g_aresultPos[nCntUi] = D3DXVECTOR3(330.0f + nCntnumber * 30.0f, 230.0f, 0.0f);//スコア（数字）
		}
		else
		{
			g_aresultPos[nCntUi] = D3DXVECTOR3(850.0f + (nCntnumber - 8) * 30.0f, 230.0f, 0.0f);//撃墜数（数字）
		}
		nCntUi++;
	}
	for (nCntrank = 0; nCntrank < 5; nCntrank++)
	{
		g_aresultPos[nCntUi] = D3DXVECTOR3(240.0f, 330.0f + nCntrank * 80.0f, 0.0f);//順位（文字）
		nCntUi++;
		for (nCntnumber = 0; nCntnumber < 11; nCntnumber++)//スコアと撃墜数
		{
			if (nCntnumber < 8)
			{
				g_aresultPos[nCntUi] = D3DXVECTOR3(330.0f + nCntnumber * 30.0f, 330.0f + nCntrank * 80.0f, 0.0f);//スコア（数字）
			}
			else
			{
				if (nCntnumber == 8)
				{
					g_aresultPos[nCntUi] = D3DXVECTOR3(730.0f, 330.0f + nCntrank * 80.0f, 0.0f);//順位（文字）
					nCntUi++;
				}
				g_aresultPos[nCntUi] = D3DXVECTOR3(850.0f + (nCntnumber - 8) * 30.0f, 330.0f + nCntrank * 80.0f, 0.0f);//撃墜数（数字）
			}
			nCntUi++;
		}
	}
	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/Texture/result.png",
		&g_pTextureresultUi[0]);
	D3DXCreateTextureFromFile(pDevice,
		"data/Texture/score.png",
		&g_pTextureresultUi[1]);
	D3DXCreateTextureFromFile(pDevice,
		"data/Texture/kill.png",
		&g_pTextureresultUi[2]);
	D3DXCreateTextureFromFile(pDevice,
		"data/Texture/you.png",
		&g_pTextureresultUi[3]);
	D3DXCreateTextureFromFile(pDevice,
		"data/Texture/number.png",
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
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * 80,
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
	//スコア(文字)
	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(g_aresultPos[nCntUi].x - 54, g_aresultPos[nCntUi].y - 20, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_aresultPos[nCntUi].x + 54, g_aresultPos[nCntUi].y - 20, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_aresultPos[nCntUi].x - 54, g_aresultPos[nCntUi].y + 20, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_aresultPos[nCntUi].x + 54, g_aresultPos[nCntUi].y + 20, 0.0f);
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
	//キル(文字)
	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(g_aresultPos[nCntUi].x - 40, g_aresultPos[nCntUi].y - 20, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_aresultPos[nCntUi].x + 40, g_aresultPos[nCntUi].y - 20, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_aresultPos[nCntUi].x - 40, g_aresultPos[nCntUi].y + 20, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_aresultPos[nCntUi].x + 40, g_aresultPos[nCntUi].y + 20, 0.0f);
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
	nCntUi++;
	for (nCntnumber = 0; nCntnumber < 11; nCntnumber++)
	{
		//スコアと撃墜数(数字)
		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(g_aresultPos[nCntUi].x - 20, g_aresultPos[nCntUi].y - 30, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aresultPos[nCntUi].x + 20, g_aresultPos[nCntUi].y - 30, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aresultPos[nCntUi].x - 20, g_aresultPos[nCntUi].y + 30, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aresultPos[nCntUi].x + 20, g_aresultPos[nCntUi].y + 30, 0.0f);
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
		if (nCntnumber < 8)
		{
			nRank = pow(10, 8 - nCntnumber);
			nNumber = Score() % nRank / (nRank / 10);
		}
		else
		{
			nRank = pow(10, 11 - nCntnumber);
			nNumber = Kill() % nRank / (nRank / 10);
		}
		pVtx[0].tex = D3DXVECTOR2(nNumber * 0.1f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(nNumber * 0.1f + 0.1f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(nNumber * 0.1f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(nNumber * 0.1f + 0.1f, 1.0f);
		pVtx += 4;
		nCntUi++;
	}
	for (nCntrank = 0; nCntrank < 5; nCntrank++)
	{
		for (nCntnumber = 0; nCntnumber < 11; nCntnumber++)//スコアと撃墜数
		{
			if (nCntnumber == 0 || nCntnumber == 8)
			{
				//順位(数字)
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
			//スコアと撃墜数(数字)
			//頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(g_aresultPos[nCntUi].x - 20, g_aresultPos[nCntUi].y - 30, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aresultPos[nCntUi].x + 20, g_aresultPos[nCntUi].y - 30, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aresultPos[nCntUi].x - 20, g_aresultPos[nCntUi].y + 30, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aresultPos[nCntUi].x + 20, g_aresultPos[nCntUi].y + 30, 0.0f);
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
			if (nCntnumber < 8)
			{
				nRank = pow(10, 8 - nCntnumber);
				nNumber = SRankData(nCntrank) % nRank / (nRank / 10);
			}
			else
			{
				nRank = pow(10, 11 - nCntnumber);
				nNumber = KRankData(nCntrank) % nRank / (nRank / 10);
			}
			pVtx[0].tex = D3DXVECTOR2(nNumber * 0.1f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(nNumber * 0.1f + 0.1f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(nNumber * 0.1f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(nNumber * 0.1f + 0.1f, 1.0f);
			nCntUi++;
			pVtx += 4;
		}
	}
	//頂点バッファをアンロックする
	g_pVtxBuffresultUi->Unlock();
}

//UIの終了処理
void UninitresultUi(void)
{
	int nCntUi;
	for (nCntUi = 0; nCntUi < 10; nCntUi++)
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
	
}


//描画処理
void DrawresultUi(void)
{
	LPDIRECT3DDEVICE9 pDevice;//デバイスのポインタ
	int nCntUi = 0;
	int nCntletter;
	int nCntnumber;
	int nCntRank;
	int SRank = 0;
	int SRankLag = 1;
	int ntScore = 0;
	int KRank = 0;
	int KRankLag = 1;
	int ntKill = 0;
	pDevice = GetDevice();//デバイスを取得する
	//頂点バッファをデバイスのデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffresultUi, 0, sizeof(VERTEX_2D));
	pDevice->SetFVF(FVF_VERTEX_2D);               //頂点フォーマットの設定
	for (nCntletter = 0; nCntletter < 4; nCntletter++)
	{
		pDevice->SetTexture(0, g_pTextureresultUi[nCntletter]);//文字列
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
			nCntUi * 4,
			2);
		nCntUi++;
	}
	for (nCntnumber = 0; nCntnumber < 11; nCntnumber++)
	{
		pDevice->SetTexture(0, g_pTextureresultUi[4]);//スコアと撃墜数（数字）
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
			nCntUi * 4,
			2);
		nCntUi++;
	}
	for (nCntRank = 0; nCntRank < 5; nCntRank++)
	{
		for (nCntnumber = 0; nCntnumber < 11; nCntnumber++)
		{
			if (nCntnumber == 0)
			{
				if (ntScore == SRankData(nCntRank))
				{
					SRankLag++;
				}
				else
				{
					SRank += SRankLag;
					SRankLag = 1;
				}
				pDevice->SetTexture(0, g_pTextureresultUi[4 + SRank]);//スコア順位(数字)
				pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
					nCntUi * 4,
					2);
				nCntUi++;
			}
			if (nCntnumber == 8)
			{
				if (ntKill == KRankData(nCntRank))
				{
					KRankLag++;
				}
				else
				{
					KRank += KRankLag;
					KRankLag = 1;
				}
				pDevice->SetTexture(0, g_pTextureresultUi[4 + KRank]);//撃墜数順位(数字)
				pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
					nCntUi * 4,
					2);
				nCntUi++;
			}
			pDevice->SetTexture(0, g_pTextureresultUi[4]);//スコアと撃墜数(数字)
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
				nCntUi * 4,
				2);
			nCntUi++;
		}
		ntScore = SRankData(nCntRank);
		ntKill = KRankData(nCntRank);
	}
}
