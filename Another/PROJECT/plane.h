//=============================================================================
//
// シーン処理 [field.h]
// Author : 三上航世
//
//=============================================================================
#ifndef _PLANE_H_
#define _PLANE_H_

#include "main.h"
#include "scene3d.h"

//マクロ定義
#define MAX_TEXTURE_FILED (16)

class CPlane : public CScene3D
{
public:
	CPlane(PRIORITY Priority);
	~CPlane();
	HRESULT Init(D3DXVECTOR3 size, D3DXVECTOR3 pos, D3DXVECTOR2 Tex, bool bRotX);	//初期化
	void Uninit();	//終了
	void Update();	//更新
	void Draw();	//描画
	//OBJTYPE GetObjType() { return OBJECTTYPE_NONE; }
	//void SetPos(D3DXVECTOR3 pos);
	//D3DXVECTOR3 GetPos() { return m_pos; }
	/*float GetRadius() { return 0.0f; }
	COLLISION GetCollision() { return COLLISION_SPHERE; }
	D3DXVECTOR3 GetVtxMax() { return D3DXVECTOR3(0.0f, 0.0f, 0.0f); }
	D3DXVECTOR3 GetVtxMin() { return D3DXVECTOR3(0.0f, 0.0f, 0.0f); }*/
	//void Damage(int, D3DXVECTOR3, float);

	//サイズ格納・取得
	D3DXVECTOR3 GetSize() { return m_size; }
	void SetSize(D3DXVECTOR3 size);
	
	LPDIRECT3DVERTEXBUFFER9 GetVtx() { return m_pVtxBuff; }	//頂点バッファへのポインタ取得
	void SetTexture(int nTex) { m_nTexType = nTex; }		//指定された番号のテクスチャを貼れるように
	void ChangeColor(D3DXCOLOR col);						//色変える

	//エフェクト用関数
	void ColorChange(D3DXCOLOR color);	//色変更
	void ChangeSize(D3DXVECTOR3 size);	//サイズ変更
	static CPlane *Create(D3DXVECTOR3 size, D3DXVECTOR3 pos, D3DXVECTOR2 Tex, bool bRotX);	//作成
	static void CreateTextureFiled();	//テクスチャ読み込み

	static LPDIRECT3DTEXTURE9 *GetTexture(int nTex) { return &m_pTexture[nTex]; }	//テクスチャ取得

	//テクスチャ破棄
	static void UninitTexture();

private:
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff = NULL;	//頂点バッファへのポインタ
	D3DXVECTOR3 m_size;							//サイズ
	static int m_nMaxTex;						//使用する最大テクスチャ

protected:
	static LPDIRECT3DTEXTURE9 m_pTexture[MAX_TEXTURE_FILED];	//テクスチャへのポインタ
	int m_nTexType;												//貼るテクスチャ
};

#endif