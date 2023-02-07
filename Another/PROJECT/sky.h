//=============================================================================
//
// 空処理 [sky.h]
// Author : 三上航世
//
//=============================================================================
#ifndef _SKY_H_
#define _SKY_H_

#include "main.h"
#include "scene.h"

class CSky : public CScene
{
public:
	CSky(PRIORITY Priority);
	~CSky();
	HRESULT Init(D3DXVECTOR3 pos);	//初期化
	void Uninit(void);	//終了
	void Update(void);	//更新
	void Draw(void);	//描画

	//↓--------使わない---------↓
	OBJTYPE GetObjType() { return OBJECTTYPE_NONE; }
	//void SetPos(D3DXVECTOR3) { ; }
	D3DXVECTOR3 GetPos() { return m_pos; }
	D3DXVECTOR3 GetRot() { return D3DXVECTOR3(0.0f, 0.0f, 0.0f); }
	float GetRadius() { return 0.0f; }
	float GetHeight() { return 0.0f; }
	COLLISION GetCollision() { return COLLISION_SPHERE; }
	D3DXVECTOR3 GetVtxMax() { return D3DXVECTOR3(0.0f, 0.0f, 0.0f); }
	D3DXVECTOR3 GetVtxMin() { return D3DXVECTOR3(0.0f, 0.0f, 0.0f); }
	void Damage(int, D3DXVECTOR3) { ; }
	bool GetRotX() { return false; }
	bool CollisionDamage(D3DXVECTOR3, float, float) { return false; }
	bool SetEnemy(CScene *pEnemy) { return false; }
	int Damage() { return 0; }
	float HClash() { return 0.0f; }
	float VClash() { return 0.0f; }
	void SetParticle() { ; }
	bool GetAttack() { return false; }
	void Reflect() { ; }
	int GetHP() { return 0; }
	int GetMaxHP() { return 0; }
	D3DXMATRIX GetMatrix(int) { return m_mtxWorld; }
	//↑--------使わない---------↑

	void SetTexture(LPDIRECT3DTEXTURE9 *pTexture) { m_pTexture = *pTexture; }			//テクスチャ格納
	void SetScroll(float fScroll) { m_fScroll = fScroll; }								//スクロールの速さ格納
	static CSky *Create(D3DXVECTOR3 pos, float fScroll, LPDIRECT3DTEXTURE9 *Texture);	//作成

private:
	static LPDIRECT3DTEXTURE9 m_pTexture;		//テクスチャ
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff = NULL;	//頂点バッファへのポインタ
	LPDIRECT3DINDEXBUFFER9 m_pIdxBuff = NULL;	//インデックスバッファ
	D3DXMATRIX m_mtxWorld;						//ワールドマトリックス
	D3DXVECTOR3 m_pos;	//場所
	D3DXVECTOR3 m_rot;	//方向
	float m_fScroll;	//スクロールの速さ
};

#endif