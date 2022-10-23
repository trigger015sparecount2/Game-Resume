//--------------------------
//Author:三上航世
//自機の処理(plyer.cpp)
//--------------------------
#include "player.h"
#include "input.h"
#include "block.h"
#include "knife.h"
#include "padinput.h"
#include "sound.h"

//グローバル変数
LPDIRECT3DTEXTURE9 g_pTexturePlayer = NULL; //テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffPlayer = NULL; //頂点バッファへのポインタ
Player g_aPlayer;

//初期化処理
void InitPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice; //デバイスのポインタ
	pDevice = GetDevice();     //デバイスを取得する
	ResetPlayer();
	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/Texture/player000.png",
		&g_pTexturePlayer);
	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffPlayer,
		NULL);
	VERTEX_2D *pVtx; //頂点情報へのポインタ
					 //頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffPlayer->Lock(0, 0, (void **)&pVtx, 0);
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
	pVtx[1].tex = D3DXVECTOR2(0.25f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 0.5f);
	pVtx[3].tex = D3DXVECTOR2(0.25f, 0.5f);
	//頂点バッファをアンロックする
	g_pVtxBuffPlayer->Unlock();
}

//終了処理
void UninitPlayer(void)
{
	//テクスチャの破棄
	if (g_pTexturePlayer != NULL)
	{
		g_pTexturePlayer->Release();
		g_pTexturePlayer = NULL;
	}
	//頂点バッファの破棄
	if (g_pVtxBuffPlayer != NULL)
	{
		g_pVtxBuffPlayer->Release();
		g_pVtxBuffPlayer = NULL;
	}
}

//更新処理
void UpdatePlayer(void)
{
	g_aPlayer.posOld.x = g_aPlayer.pos.x;
	g_aPlayer.posOld.y = g_aPlayer.pos.y;
	g_aPlayer.nknife--;
	if (g_aPlayer.nknife < 0)
	{
		g_aPlayer.nknife = 0;
	}
	if (GetKeyboardPress(DIK_A) == true || LeftStickX() < 0.0f) //Aキーを押すか、左スティックを倒したとき
	{
		if (g_aPlayer.nStep == 0 && g_aPlayer.bJump == true)
		{
			PlaySound(SOUND_LABEL_FOOTSTEP);
		}
		g_aPlayer.nStep++;
		if (g_aPlayer.nStep >= 15)
		{
			g_aPlayer.nStep = 0;
		}
		g_aPlayer.fLeft = 0.5f;
		g_aPlayer.nCounterAnim++;
		if (g_aPlayer.nCounterAnim == 6)
		{
			g_aPlayer.nPatternAnim++;
			g_aPlayer.nCounterAnim = 0;
			if (g_aPlayer.nPatternAnim == 4)
			{
				g_aPlayer.nPatternAnim = 0;
			}
		}
		if (g_aPlayer.move.x > 0.0f) //方向転換するとき
		{
			if (LeftStickX() < -0.6f || GetKeyboardPress(DIK_A) == true) //深く左スティックを左に倒したときかAキーを押したとき
			{
				if (g_aPlayer.bJump == true)
				{
					g_aPlayer.move.x -= 1.6f * g_aPlayer.fIce; //地上
				}
				else
				{
					g_aPlayer.move.x -= 1.0f * g_aPlayer.fIce; //空中
				}
			}
			else //浅く左スティックを左に倒したとき
			{
				if (g_aPlayer.bJump == true)
				{
					g_aPlayer.move.x -= 0.5f * g_aPlayer.fIce; //地上
				}
				else
				{
					g_aPlayer.move.x -= 0.3f * g_aPlayer.fIce; //空中
				}
			}
		}
		else
		{
			if (LeftStickX() < -0.6f || GetKeyboardPress(DIK_A) == true) //深く左スティックを左に倒したとき
			{
				if (g_aPlayer.bJump == true)
				{
					g_aPlayer.move.x -= 0.8f * g_aPlayer.fIce; //地上
				}
				else
				{
					g_aPlayer.move.x -= 0.5f * g_aPlayer.fIce; //空中
				}
			}
			else
			{
				if (g_aPlayer.bJump == true) //浅く左スティックを左に倒したとき
				{
					g_aPlayer.move.x -= 0.25f * g_aPlayer.fIce; //地上
				}
				else
				{
					g_aPlayer.move.x -= 0.16f * g_aPlayer.fIce; //空中
				}
				if (g_aPlayer.move.x <= -4.0f)
				{
					g_aPlayer.move.x += 1.0f; //歩きが速すぎる場合、徐々に遅くなる
				}
				else if (g_aPlayer.move.x <= -3.0f)
				{
					g_aPlayer.move.x = -3.0f; //歩いているときの最高速度
				}
			}
		}
		if (g_aPlayer.move.x <= -8.0f)
		{
			g_aPlayer.move.x = -8.0f; //通常時の最高速度
		}
	}
	else if (GetKeyboardPress(DIK_D) == true || LeftStickX() > 0.0f)
	{
		if (g_aPlayer.nStep == 0 && g_aPlayer.bJump == true)
		{
			PlaySound(SOUND_LABEL_FOOTSTEP);
		}
		g_aPlayer.nStep++;
		if (g_aPlayer.nStep >= 15)
		{
			g_aPlayer.nStep = 0;
		}
		g_aPlayer.fLeft = 0.0f;
		g_aPlayer.nCounterAnim++;
		if (g_aPlayer.nCounterAnim == 6)
		{
			g_aPlayer.nPatternAnim++;
			g_aPlayer.nCounterAnim = 0;
			if (g_aPlayer.nPatternAnim == 4)
			{
				g_aPlayer.nPatternAnim = 0;
			}
		}
		if (g_aPlayer.move.x < 0.0f) //方向転換するとき
		{
			if (LeftStickX() > 0.6f || GetKeyboardPress(DIK_D) == true) //深く左スティックを右に倒したとき
			{
				if (g_aPlayer.bJump == true)
				{
					g_aPlayer.move.x += 1.6f * g_aPlayer.fIce; //地上
				}
				else
				{
					g_aPlayer.move.x += 1.0f * g_aPlayer.fIce; //空中
				}
			}
			else
			{
				if (g_aPlayer.bJump == true) //浅く左スティックを倒したとき
				{
					g_aPlayer.move.x += 0.5f * g_aPlayer.fIce; //地上
				}
				else
				{
					g_aPlayer.move.x += 0.3f * g_aPlayer.fIce; //空中
				}
			}
		}
		else
		{
			if (LeftStickX() > 0.6f || GetKeyboardPress(DIK_D) == true) //深く左スティックを右に倒したときかDキーを押したとき
			{
				if (g_aPlayer.bJump == true)
				{
					g_aPlayer.move.x += 0.8f * g_aPlayer.fIce; //地上
				}
				else
				{
					g_aPlayer.move.x += 0.5f * g_aPlayer.fIce; //空中
				}
			}
			else
			{
				if (g_aPlayer.bJump == true) //浅く左スティックを右に倒したとき
				{
					g_aPlayer.move.x += 0.25f * g_aPlayer.fIce; //地上
				}
				else
				{
					g_aPlayer.move.x += 0.16f * g_aPlayer.fIce; //空中
				}
				if (g_aPlayer.move.x >= 4.0f)
				{
					g_aPlayer.move.x -= 1.0f; //歩きが速すぎるとき、徐々に遅くなる
				}
				else if (g_aPlayer.move.x >= 3.0f)
				{
					g_aPlayer.move.x = 3.0f; //歩きの最高速度
				}
			}
		}
		if (g_aPlayer.move.x >= 8.0f)
		{
			g_aPlayer.move.x = 8.0f; //走りの最高速度
		}
	}
	else
	{
		g_aPlayer.nCounterAnim = 0;
		g_aPlayer.nPatternAnim = 0;
		g_aPlayer.nStep = 0;
		if (g_aPlayer.move.x > 0.0f)
		{
			g_aPlayer.move.x -= 1.2f * g_aPlayer.fIce;
			if (g_aPlayer.move.x < 0.0f)
			{
				g_aPlayer.move.x = 0.0f;
			}
		}
		else if (g_aPlayer.move.x < 0.0f)
		{
			g_aPlayer.move.x += 1.2f * g_aPlayer.fIce;
			if (g_aPlayer.move.x > 0.0f)
			{
				g_aPlayer.move.x = 0.0f;
			}
		}
	}
	if (GetKeyboardKey(DIK_I) == true && g_aPlayer.nknife == 0 || GetGamePadButton(5) == true && g_aPlayer.nknife == 0)
	{
		if (GetKeyboardPress(DIK_W) == true || RightStickY() < -0.9f && GamePad() == true) //上
		{
			SetKnife(g_aPlayer.pos, KNIFE_UP);
		}
		else if (GetKeyboardPress(DIK_S) == true || RightStickY() > 0.9f && GamePad() == true) //下
		{
			SetKnife(g_aPlayer.pos, KNIFE_DOWN);
		}
		else if (GetKeyboardPress(DIK_A) == true || RightStickX() < 0.0f && GamePad() == true) //左
		{
			g_aPlayer.fLeft = 0.5f;
			SetKnife(g_aPlayer.pos, KNIFE_LEFT);
		}
		else if (GetKeyboardPress(DIK_D) == true || RightStickX() > 0.0f && GamePad() == true) //右
		{
			g_aPlayer.fLeft = 0.0f;
			SetKnife(g_aPlayer.pos, KNIFE_RIGHT);
		}
		else
		{
			if (g_aPlayer.fLeft == 0.0f) //右を向いている
			{
				SetKnife(g_aPlayer.pos, KNIFE_RIGHT);
			}
			else //左を向いている
			{
				SetKnife(g_aPlayer.pos, KNIFE_LEFT);
			}
		}
		g_aPlayer.nknife = 15;
	}
	if (GetKeyboardKey(DIK_J) == true && g_aPlayer.bJump == true || GetGamePadButton(2) == true && g_aPlayer.bJump == true) //ジャンプ
	{
		PlaySound(SOUND_LABEL_JUMP);
		g_aPlayer.move.y = -20.0f;
	}
	g_aPlayer.move.y += 1.3f;
	if (g_aPlayer.bJump == false)
	{
		g_aPlayer.nPatternAnim = 1;
		g_aPlayer.nStep = 13;
	}
	g_aPlayer.pos.x += g_aPlayer.move.x + g_aPlayer.Blockmove.x;
	g_aPlayer.pos.y += g_aPlayer.move.y + g_aPlayer.Blockmove.y;
	if (g_aPlayer.pos.x <= -PLAYER_WIDTH / 2) //左行った場合
	{
		ResetPlayer();
	}
	if (g_aPlayer.pos.y <= -PLAYER_HEIGHT) //上行った場合
	{
		ResetPlayer();
	}
	if (g_aPlayer.pos.x >= SCREEN_WIDTH + (PLAYER_WIDTH / 2)) //右行った場合
	{
		ResetPlayer();
	}
	if (g_aPlayer.pos.y >= SCREEN_HEIGHT + PLAYER_HEIGHT) //下行った場合
	{
		ResetPlayer();
	}
	g_aPlayer.bJump = CollisionBlock(&g_aPlayer.pos, &g_aPlayer.posOld, &g_aPlayer.move, PLAYER_WIDTH / 2, PLAYER_HEIGHT, &g_aPlayer.Blockmove, 0);
	VERTEX_2D *pVtx; //頂点情報へのポイント
	g_pVtxBuffPlayer->Lock(0, 0, (void **)&pVtx, 0);
	pVtx[0].pos = D3DXVECTOR3((float)(g_aPlayer.pos.x - PLAYER_WIDTH / 2), (float)(g_aPlayer.pos.y - PLAYER_HEIGHT), g_aPlayer.pos.z);
	pVtx[1].pos = D3DXVECTOR3((float)(g_aPlayer.pos.x + PLAYER_WIDTH / 2), (float)(g_aPlayer.pos.y - PLAYER_HEIGHT), g_aPlayer.pos.z);
	pVtx[2].pos = D3DXVECTOR3((float)(g_aPlayer.pos.x - PLAYER_WIDTH / 2), (float)(g_aPlayer.pos.y),				 g_aPlayer.pos.z);
	pVtx[3].pos = D3DXVECTOR3((float)(g_aPlayer.pos.x + PLAYER_WIDTH / 2), (float)(g_aPlayer.pos.y),				 g_aPlayer.pos.z);
	pVtx[0].tex = D3DXVECTOR2((float)g_aPlayer.nPatternAnim / 4        , g_aPlayer.fLeft);
	pVtx[1].tex = D3DXVECTOR2((float)g_aPlayer.nPatternAnim / 4 + 0.25f, g_aPlayer.fLeft);
	pVtx[2].tex = D3DXVECTOR2((float)g_aPlayer.nPatternAnim / 4        , g_aPlayer.fLeft + 0.5f);
	pVtx[3].tex = D3DXVECTOR2((float)g_aPlayer.nPatternAnim / 4 + 0.25f, g_aPlayer.fLeft + 0.5f);
	g_pVtxBuffPlayer->Unlock();
}

//描画処理
void DrawPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice;                    //デバイスのポインタ
	pDevice = GetDevice();                        //デバイスを取得する
												  //頂点バッファをデバイスのデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffPlayer, 0, sizeof(VERTEX_2D));
	pDevice->SetFVF(FVF_VERTEX_2D);               //頂点フォーマットの設定
	pDevice->SetTexture(0, g_pTexturePlayer);    //テクスチャの設定
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
		0,  //開始する始点のインデックス
		2); //描画するプリミティブ数
}

Player *GetPlayer(void)
{
	return &g_aPlayer;
}

void ResetPlayer(void)
{
	g_aPlayer.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_aPlayer.pos = D3DXVECTOR3(640.0f, 460.0f, 0.0f);
	g_aPlayer.posOld = D3DXVECTOR3(640.0f, 460.0f, 0.0f);
	g_aPlayer.Blockmove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_aPlayer.nCounterAnim = 0;
	g_aPlayer.nPatternAnim = 0;
	g_aPlayer.nknife = 0;
	g_aPlayer.fLeft = 0.0f;
	g_aPlayer.nStep = 0;
	g_aPlayer.fIce = 1.0f;
	g_aPlayer.bJump = true;
}

void IcePlayer(bool bIce)
{
	if (bIce == true)
	{
		g_aPlayer.fIce = 0.06f;
	}
	else
	{
		g_aPlayer.fIce = 1.0f;
	}
}