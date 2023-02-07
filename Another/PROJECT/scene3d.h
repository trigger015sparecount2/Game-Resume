//=============================================================================
//
// シーン処理 [scene3d.h]
// Author : 三上航世
//
//=============================================================================
#ifndef _SCENE3D_H_
#define _SCENE3D_H_

#include "main.h"
#include "scene.h"

//*****************************************************************************
// 構造体定義
//*****************************************************************************
// 頂点フォーマットに合わせた構造体
struct VERTEX_3D
{
	D3DXVECTOR3 pos; //頂点座標
	D3DXVECTOR3 nor; //法線ベクトル
	D3DCOLOR col;    //頂点カラー
	D3DXVECTOR2 tex; //テクスチャ座標
};

class CScene3D : public CScene
{
public:
	CScene3D(PRIORITY Priority);
	~CScene3D();
	HRESULT Init(D3DXVECTOR3 pos);	//初期化
	void Uninit();	//終了
	void Update();	//更新
	void Draw();	//描画
	
	//posの格納・取得
	void SetPos(const D3DXVECTOR3& pos) { m_pos = pos; }
	D3DXVECTOR3 GetPos() { return m_pos; }

	//マトリックス格納・取得
	D3DXMATRIX GetMatrix() const { return m_mtxWorld; }
	void SetMatrix(const D3DXMATRIX& mtx) { m_mtxWorld = mtx; }

	//方向の格納・取得
	D3DXVECTOR3 GetRot() { return m_rot; }
	void SetRot(const D3DXVECTOR3& rot) { m_rot = rot; }

	//↓--------使わない---------↓
	OBJTYPE GetObjType() { return OBJECTTYPE_NONE; }
	float GetRadius() { return 0.0f; }
	float GetHeight() { return 0.0f; }
	COLLISION GetCollision() { return COLLISION_SPHERE; }
	D3DXVECTOR3 GetVtxMax() { return D3DXVECTOR3(0.0f, 0.0f, 0.0f); }
	D3DXVECTOR3 GetVtxMin() { return D3DXVECTOR3(0.0f, 0.0f, 0.0f); }
	void Damage(int, D3DXVECTOR3) { ; }
	bool GetRotX() { return false; }
	bool CollisionDamage(D3DXVECTOR3, float, float) { return false; }
	bool SetEnemy(CScene *pScene) { return false; }
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

	static CScene3D *Create(const D3DXVECTOR3& pos);	//作成


private:
	D3DXVECTOR3 m_rot;		//向き
	D3DXVECTOR3 m_pos;		//場所
	D3DXMATRIX m_mtxWorld;	//ワールドマトリックス
};
#endif
