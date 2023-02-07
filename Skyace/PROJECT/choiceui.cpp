//-------------------------
//Author:三上航世
//選択画面UI処理(choiceui.cpp)
//-------------------------
#include "choiceui.h"
#include "input.h"
#include "aircraft.h"
#include "fade.h"
#include "sound.h"

//グローバル変数
LPDIRECT3DTEXTURE9 g_pTexturechoiceUi[18] = {};    //テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffchoiceUi = NULL; //頂点バッファへのポインタ
D3DXVECTOR3 g_achoicePos[2 + 6 + 2 + 6 + 1];   //各ポリゴンの位置(HUD戦闘機、特殊兵装、*2、説明文)
int g_nAtype;	//戦闘機の種類
int g_nStype;	//特殊兵装の種類
int g_nStage;	//枠何番目か
int g_nlight;	//どれだけ白くするか
bool g_blight;	//明るくするか暗くするか

//初期化処理
void InitchoiceUi(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	int nCntUi = 0;
	int nCntspecial;
	int nCntsquare;
	g_nAtype = 0;
	g_nStype = 0;
	g_nStage = 0;
	g_nlight = 0;
	g_blight = true;
	pDevice = GetDevice();
	g_achoicePos[nCntUi] = D3DXVECTOR3(400.0f, 270.0f, 0.0f);//イーグル四角
	nCntUi++;
	g_achoicePos[nCntUi] = D3DXVECTOR3(400.0f, 200.0f, 0.0f);//イーグル
	nCntUi++;
	for (nCntspecial = 0; nCntspecial < 3; nCntspecial++)
	{
		for (nCntsquare = 0; nCntsquare < 2; nCntsquare++)
		{
			g_achoicePos[nCntUi] = D3DXVECTOR3(300.0f + nCntspecial * 100.0f, 400.0f, 0.0f);//イーグル特殊兵装
			nCntUi++;
		}
	}
	g_achoicePos[nCntUi] = D3DXVECTOR3(800.0f, 270.0f, 0.0f);//フランカー四角
	nCntUi++;
	g_achoicePos[nCntUi] = D3DXVECTOR3(800.0f, 200.0f, 0.0f);//フランカー
	nCntUi++;
	for (nCntspecial = 0; nCntspecial < 3; nCntspecial++)
	{
		for (nCntsquare = 0; nCntsquare < 2; nCntsquare++)
		{
			g_achoicePos[nCntUi] = D3DXVECTOR3(700.0f + nCntspecial * 100.0f, 400.0f, 0.0f);//フランカー特殊兵装
			nCntUi++;
		}
	}
	g_achoicePos[nCntUi] = D3DXVECTOR3(640.0f, 630.0f, 0.0f);//説明文四角
	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/Texture/HUDeagle.png",
		&g_pTexturechoiceUi[0]);
	D3DXCreateTextureFromFile(pDevice,
		"data/Texture/HUDflanker.png",
		&g_pTexturechoiceUi[1]);
	D3DXCreateTextureFromFile(pDevice,
		"data/Texture/HUDuav.png",
		&g_pTexturechoiceUi[2]);
	D3DXCreateTextureFromFile(pDevice,
		"data/Texture/HUDmissile.png",
		&g_pTexturechoiceUi[3]);
	D3DXCreateTextureFromFile(pDevice,
		"data/Texture/HUDbarrier.png",
		&g_pTexturechoiceUi[4]);
	D3DXCreateTextureFromFile(pDevice,
		"data/Texture/HUDrailgun.png",
		&g_pTexturechoiceUi[5]);
	D3DXCreateTextureFromFile(pDevice,
		"data/Texture/HUDbombmissile.png",
		&g_pTexturechoiceUi[6]);
	D3DXCreateTextureFromFile(pDevice,
		"data/Texture/HUDlaser.png",
		&g_pTexturechoiceUi[7]);
	D3DXCreateTextureFromFile(pDevice,
		"data/Texture/縦長四角.png",
		&g_pTexturechoiceUi[8]);
	D3DXCreateTextureFromFile(pDevice,
		"data/Texture/square.png",
		&g_pTexturechoiceUi[9]);
	D3DXCreateTextureFromFile(pDevice,
		"data/Texture/イーグル説明文.png",
		&g_pTexturechoiceUi[10]);
	D3DXCreateTextureFromFile(pDevice,
		"data/Texture/フランカー説明文.png",
		&g_pTexturechoiceUi[11]);
	D3DXCreateTextureFromFile(pDevice,
		"data/Texture/UAV説明文.png",
		&g_pTexturechoiceUi[12]);
	D3DXCreateTextureFromFile(pDevice,
		"data/Texture/マルチロックミサイル説明文.png",
		&g_pTexturechoiceUi[13]);
	D3DXCreateTextureFromFile(pDevice,
		"data/Texture/バリア説明文.png",
		&g_pTexturechoiceUi[14]);
	D3DXCreateTextureFromFile(pDevice,
		"data/Texture/レールガン説明文.png",
		&g_pTexturechoiceUi[15]);
	D3DXCreateTextureFromFile(pDevice,
		"data/Texture/ボムミサイル説明文.png",
		&g_pTexturechoiceUi[16]);
	D3DXCreateTextureFromFile(pDevice,
		"data/Texture/レーザー説明文.png",
		&g_pTexturechoiceUi[17]);

	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * 17,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffchoiceUi,
		NULL);

	VERTEX_2D *pVtx;
	nCntUi = 0;
	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffchoiceUi->Lock(0, 0, (void **)&pVtx, 0);
	for (nCntsquare = 0; nCntsquare < 2; nCntsquare++)
	{
		//戦闘機大枠四角
		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(g_achoicePos[nCntUi].x - 160.0f, g_achoicePos[nCntUi].y - 260.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_achoicePos[nCntUi].x + 160.0f, g_achoicePos[nCntUi].y - 260.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_achoicePos[nCntUi].x - 160.0f, g_achoicePos[nCntUi].y + 260.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_achoicePos[nCntUi].x + 160.0f, g_achoicePos[nCntUi].y + 260.0f, 0.0f);
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
		//戦闘機
		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(g_achoicePos[nCntUi].x - 100.0f, g_achoicePos[nCntUi].y - 130.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_achoicePos[nCntUi].x + 100.0f, g_achoicePos[nCntUi].y - 130.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_achoicePos[nCntUi].x - 100.0f, g_achoicePos[nCntUi].y + 130.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_achoicePos[nCntUi].x + 100.0f, g_achoicePos[nCntUi].y + 130.0f, 0.0f);
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
		if (nCntsquare == 0)//イーグル
		{
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(0.2f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(0.2f, 1.0f);
		}
		else//フランカー
		{
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(0.25f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(0.25f, 1.0f);
		}
		pVtx += 4;
		nCntUi++;
		for (nCntspecial = 0; nCntspecial < 3; nCntspecial++)//特殊兵装
		{
			//頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(g_achoicePos[nCntUi].x - 40.0f, g_achoicePos[nCntUi].y - 40.0f, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_achoicePos[nCntUi].x + 40.0f, g_achoicePos[nCntUi].y - 40.0f, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_achoicePos[nCntUi].x - 40.0f, g_achoicePos[nCntUi].y + 40.0f, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_achoicePos[nCntUi].x + 40.0f, g_achoicePos[nCntUi].y + 40.0f, 0.0f);
			//rhwの設定
			pVtx[0].rhw = 1.0f;
			pVtx[1].rhw = 1.0f;
			pVtx[2].rhw = 1.0f;
			pVtx[3].rhw = 1.0f;
			//頂点カラー
			pVtx[0].col = D3DCOLOR_RGBA(120, 120, 120, 255);
			pVtx[1].col = D3DCOLOR_RGBA(120, 120, 120, 255);
			pVtx[2].col = D3DCOLOR_RGBA(120, 120, 120, 255);
			pVtx[3].col = D3DCOLOR_RGBA(120, 120, 120, 255);
			//テクスチャ座標
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
			pVtx += 4;
			nCntUi++;
			//頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(g_achoicePos[nCntUi].x - 30.0f, g_achoicePos[nCntUi].y - 30.0f, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_achoicePos[nCntUi].x + 30.0f, g_achoicePos[nCntUi].y - 30.0f, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_achoicePos[nCntUi].x - 30.0f, g_achoicePos[nCntUi].y + 30.0f, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_achoicePos[nCntUi].x + 30.0f, g_achoicePos[nCntUi].y + 30.0f, 0.0f);
			//rhwの設定
			pVtx[0].rhw = 1.0f;
			pVtx[1].rhw = 1.0f;
			pVtx[2].rhw = 1.0f;
			pVtx[3].rhw = 1.0f;
			//頂点カラー
			pVtx[0].col = D3DCOLOR_RGBA(120, 120, 120, 255);
			pVtx[1].col = D3DCOLOR_RGBA(120, 120, 120, 255);
			pVtx[2].col = D3DCOLOR_RGBA(120, 120, 120, 255);
			pVtx[3].col = D3DCOLOR_RGBA(120, 120, 120, 255);
			//テクスチャ座標
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(0.5f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(0.5f, 1.0f);
			pVtx += 4;
			nCntUi++;
		}
	}
	//説明文四角
	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(g_achoicePos[nCntUi].x - 630.0f, g_achoicePos[nCntUi].y - 80.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_achoicePos[nCntUi].x + 630.0f, g_achoicePos[nCntUi].y - 80.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_achoicePos[nCntUi].x - 630.0f, g_achoicePos[nCntUi].y + 80.0f, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_achoicePos[nCntUi].x + 630.0f, g_achoicePos[nCntUi].y + 80.0f, 0.0f);
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
	g_pVtxBuffchoiceUi->Unlock();
}

//UIの終了処理
void UninitchoiceUi(void)
{
	int nCntUi;
	for (nCntUi = 0; nCntUi < 18; nCntUi++)
	{
		//テクスチャの破棄
		if (g_pTexturechoiceUi[nCntUi] != NULL)
		{
			g_pTexturechoiceUi[nCntUi]->Release();
			g_pTexturechoiceUi[nCntUi] = NULL;
		}
	}
	//頂点バッファの破棄
	if (g_pVtxBuffchoiceUi != NULL)
	{
		g_pVtxBuffchoiceUi->Release();
		g_pVtxBuffchoiceUi = NULL;
	}
}

//UIの更新処理
void UpdatechoiceUi(void)
{
	int nCntchoiceUi;
	int nCntSquare;

	//明るくする
	if (g_blight == true)
	{
		g_nlight++;
		if (g_nlight >= 30)
		{
			g_nlight = 30;
			g_blight = false;
		}
	}
	//暗くする
	else
	{
		g_nlight--;
		if (g_nlight <= 0)
		{
			g_nlight = 0;
			g_blight = true;
		}
	}

	//機体選択
	if (g_nStage == 0)
	{
		g_nStype = 0;
		
		//左右に選択
		if (GetKeyboardTrigger(DIK_D) == true && GetKeyboardRelease(DIK_D) == false)
		{
			PlaySound(SOUND_LABEL_CHOICE000);
			g_nAtype = (g_nAtype + 1) % 2;
		}
		else if (GetKeyboardTrigger(DIK_A) == true && GetKeyboardRelease(DIK_A) == false)
		{
			PlaySound(SOUND_LABEL_CHOICE000);
			g_nAtype--;
			if (g_nAtype == -1)
			{
				g_nAtype = 1;
			}
		}

		//決定
		if (GetKeyboardTrigger(DIK_J) == true && GetKeyboardRelease(DIK_J) == false)
		{
			g_nStage++;
			PlaySound(SOUND_LABEL_YES000);
		}

		//全部やや暗く描画
		for (nCntchoiceUi = 0; nCntchoiceUi < 16; nCntchoiceUi++)
		{
			VERTEX_2D *pVtx;
			g_pVtxBuffchoiceUi->Lock(0, 0, (void **)&pVtx, 0);
			pVtx += nCntchoiceUi * 4;
			pVtx[0].col = D3DCOLOR_RGBA(120, 120, 120, 255);
			pVtx[1].col = D3DCOLOR_RGBA(120, 120, 120, 255);
			pVtx[2].col = D3DCOLOR_RGBA(120, 120, 120, 255);
			pVtx[3].col = D3DCOLOR_RGBA(120, 120, 120, 255);
			g_pVtxBuffchoiceUi->Unlock();
		}

		//選択したほうを明るく描画
		VERTEX_2D *pVtx;
		g_pVtxBuffchoiceUi->Lock(0, 0, (void **)&pVtx, 0);
		pVtx += g_nAtype * 4 * 8;
		for (nCntchoiceUi = 0; nCntchoiceUi < 2; nCntchoiceUi++)
		{
			pVtx[0].col = D3DCOLOR_RGBA(225 + g_nlight, 225 + g_nlight, 225 + g_nlight, 255);
			pVtx[1].col = D3DCOLOR_RGBA(225 + g_nlight, 225 + g_nlight, 225 + g_nlight, 255);
			pVtx[2].col = D3DCOLOR_RGBA(225 + g_nlight, 225 + g_nlight, 225 + g_nlight, 255);
			pVtx[3].col = D3DCOLOR_RGBA(225 + g_nlight, 225 + g_nlight, 225 + g_nlight, 255);
			pVtx += 4;
		}
		g_pVtxBuffchoiceUi->Unlock();
	}
	//特殊兵装選択
	else
	{
		//特殊兵装を全部暗くする
		for (nCntSquare = 0; nCntSquare < 2; nCntSquare++)
		{
			for (nCntchoiceUi = 0; nCntchoiceUi < 6; nCntchoiceUi++)
			{
				VERTEX_2D *pVtx;
				g_pVtxBuffchoiceUi->Lock(0, 0, (void **)&pVtx, 0);
				pVtx += nCntchoiceUi * 4 + nCntSquare * 32 + 8;
				pVtx[0].col = D3DCOLOR_RGBA(120, 120, 120, 255);
				pVtx[1].col = D3DCOLOR_RGBA(120, 120, 120, 255);
				pVtx[2].col = D3DCOLOR_RGBA(120, 120, 120, 255);
				pVtx[3].col = D3DCOLOR_RGBA(120, 120, 120, 255);
				g_pVtxBuffchoiceUi->Unlock();
			}
		}

		//選択したやつ全部明るくする
		VERTEX_2D *pVtx;
		g_pVtxBuffchoiceUi->Lock(0, 0, (void **)&pVtx, 0);
		pVtx += g_nAtype * 32 + g_nStype * 8 + 8;
		for (nCntchoiceUi = 0; nCntchoiceUi < 2; nCntchoiceUi++)
		{
			pVtx[0].col = D3DCOLOR_RGBA(225 + g_nlight, 225 + g_nlight, 225 + g_nlight, 255);
			pVtx[1].col = D3DCOLOR_RGBA(225 + g_nlight, 225 + g_nlight, 225 + g_nlight, 255);
			pVtx[2].col = D3DCOLOR_RGBA(225 + g_nlight, 225 + g_nlight, 225 + g_nlight, 255);
			pVtx[3].col = D3DCOLOR_RGBA(225 + g_nlight, 225 + g_nlight, 225 + g_nlight, 255);
			pVtx += 4;
		}
		g_pVtxBuffchoiceUi->Unlock();

		//左右に選択
		if (GetKeyboardTrigger(DIK_D) == true && GetKeyboardRelease(DIK_D) == false)
		{
			PlaySound(SOUND_LABEL_CHOICE000);
			g_nStype = (g_nStype + 1) % 3;
		}
		else if (GetKeyboardTrigger(DIK_A) == true && GetKeyboardRelease(DIK_A) == false)
		{
			PlaySound(SOUND_LABEL_CHOICE000);
			g_nStype--;
			if (g_nStype == -1)
			{
				g_nStype = 2;
			}
		}

		//決定
		if (GetKeyboardTrigger(DIK_J) == true && GetKeyboardRelease(DIK_J) == false)
		{
			PlaySound(SOUND_LABEL_YES000);
			SetFade(MODE_GAME, true);
			SetTypeAircraft(g_nAtype, g_nStype);
		}
		//キャンセル
		else if (GetKeyboardTrigger(DIK_I) == true && GetKeyboardRelease(DIK_I) == false)
		{
			PlaySound(SOUND_LABEL_CANCEL000);
			g_nStage--;
		}
	}
}


//描画処理
void DrawchoiceUi(void)
{
	LPDIRECT3DDEVICE9 pDevice;                    //デバイスのポインタ
	int nCntUi = 0;
	int nCntSpecial;
	pDevice = GetDevice();                        //デバイスを取得する
	//頂点バッファをデバイスのデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffchoiceUi, 0, sizeof(VERTEX_2D));
	pDevice->SetFVF(FVF_VERTEX_2D);               //頂点フォーマットの設定
	pDevice->SetTexture(0, g_pTexturechoiceUi[8]);//イーグル四角
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
		nCntUi * 4,
		2);
	nCntUi++;
	pDevice->SetTexture(0, g_pTexturechoiceUi[0]);//イーグル
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
		nCntUi * 4,
		2);
	nCntUi++;
	for (nCntSpecial = 0; nCntSpecial < 3; nCntSpecial++)
	{
		pDevice->SetTexture(0, g_pTexturechoiceUi[9]);//特殊兵装四角
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
			nCntUi * 4,
			2);
		nCntUi++;
		pDevice->SetTexture(0, g_pTexturechoiceUi[nCntSpecial + 2]);//特殊兵装
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
			nCntUi * 4,
			2);
		nCntUi++;
	}
	pDevice->SetTexture(0, g_pTexturechoiceUi[8]);//フランカー四角
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
		nCntUi * 4,
		2);
	nCntUi++;
	pDevice->SetTexture(0, g_pTexturechoiceUi[1]);//フランカー
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
		nCntUi * 4,
		2);
	nCntUi++;
	for (nCntSpecial = 0; nCntSpecial < 3; nCntSpecial++)
	{
		pDevice->SetTexture(0, g_pTexturechoiceUi[9]);//特殊兵装四角
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
			nCntUi * 4,
			2);
		nCntUi++;
		pDevice->SetTexture(0, g_pTexturechoiceUi[nCntSpecial + 5]);//特殊兵装
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
			nCntUi * 4,
			2);
		nCntUi++;
	}
	if (g_nStage == 0)
	{
		pDevice->SetTexture(0, g_pTexturechoiceUi[10 + g_nAtype]);//機体説明文
	}
	else
	{
		pDevice->SetTexture(0, g_pTexturechoiceUi[12 + (g_nAtype * 3) + g_nStype]);//兵装説明文
	}
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
		nCntUi * 4,
		2);
}
