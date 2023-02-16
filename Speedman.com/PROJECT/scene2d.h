//*****************************************************************************
// シーン2D処理 [Scene2D.h]
// Author : 佐藤秀亮
//*****************************************************************************
#ifndef _SCENE2D_H_
#define _SCENE2D_H_

//*****************************************************************************
// インクルード
//*****************************************************************************
#include "main.h"
#include "Scene.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define FVF_VERTEX_2D	(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)
#define MAX_TEXTURE (64)		//最大で使える2D用の画像数

//*****************************************************************************
// 構造体定義
//*****************************************************************************
// 頂点フォーマットに合わせた構造体
typedef struct
{
	D3DXVECTOR3 pos;	// 頂点処理
	float rhw;			// 1.0で固定
	D3DCOLOR col;		// 頂点カラー
	D3DXVECTOR2 tex;	// テクスチャの座標
} VERTEX_2D;

//*****************************************************************************
//オブジェクトクラス
//*****************************************************************************
class CScene2D : public CScene
{
public:
	CScene2D(int nPriority = 3);
	~CScene2D();

	HRESULT Init(D3DXVECTOR3 pos);	//初期化
	void Uninit();					//終了
	void Update();					//更新
	void Draw();					//描画

	static void UninitTexture();

	static void CreateTexture();//テクスチャ読み込み

	void SetPosition(D3DXVECTOR3 pos);				//pos更新用
	D3DXVECTOR3 GetPosition() { return m_pos; }		//pos更新するためにゲット

	float GetHeight() { return m_Size.y; }			//大きさ更新するためにゲット
	float GetWidth() { return m_Size.x; }			//大きさ更新するためにゲット
	void SetHeight(float Hight);					//大きさ更新
	void SetWidth(float Whidth);					//大きさ更新

	void SetTexture(int nTex) { m_nType = nTex; }	//テクスチャタイプ


	void ColorChange(D3DCOLORVALUE color);			//カラー変更

	void FadeColorChange(int Alpha);

	void SetTex(int Texpos, float TexSize);			//テクスチャパターン番号、パターン１つのサイズ
	void TexMove(float TexMoveU, float TexMoveV);	//テクスチャ座標の更新

	void SetGaugeScele(float Whidth, float Hight);	//ゲージタイプのスケール変更

private:

	static LPDIRECT3DTEXTURE9 m_Texture[MAX_TEXTURE];	//テクスチャーポインタ
	static int m_nMaxTex;								//使用する最大テクスチャ

	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff = NULL;	//頂点バッファへのポインタ

	D3DXVECTOR3 m_pos;				//位置

	D3DXVECTOR2 m_Size;				//サイズ

	int m_nType;					//種類

	int m_Texpos;					//テクスチャ座標
	float m_TexSize;				//テクスチャサイズ

	float m_TexMoveU;				//テクスチャ移動値
	float m_TexMoveV;				//テクスチャ移動値

	float m_PosTexU;				//テクスチャU座標
	float m_PosTexV;				//テクスチャV座標

	//カラー値
	D3DCOLORVALUE m_color;
};
#endif
