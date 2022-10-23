//=============================================================================
//
// シーン処理 [scene.h]
// Author : 三上航世
//
//=============================================================================
#ifndef _SCENE_H_
#define _SCENE_H_
#include "main.h"

//マクロ定義
#define POLYGON_SIZE (70.0f)
#define PRIORITY (7)
#define CIRCLE (float(rand() % 324) / 100.0f - float(rand() % 324) / 100.0f) //円

class CCameraPanel;

class CScene
{
public:

	//オブジェクトの種類
	typedef enum
	{
		OBJECTTYPE_NONE = 0,
		OBJECTTYPE_UI,
		OBJECTTYPE_EFFECT,
		OBJECTTYPE_ZIPLINE,
		OBJECTTYPE_FIELD,
		OBJECTTYPE_BLOCK,
		OBJCTTYPE_GOAL,
		OBJCTTYPE_TRAMPOLINE,
		OBJCTTYPE_SLACKLINE,
		OBJCTTYPE_CAMERAPANEL,
		OBJCTTYPE_CHECKPOINT,
		OBJECTTYPE_SERECTUI,
		OBJECTTYPE_MAX
	} OBJTYPE;

	typedef enum
	{
		COLLISION_SPHERE = 0,
		COLLISION_PALL,
		COLLISION_SQUARE,
		COLLISION_MAX
	} COLLISION;

	CScene(int nPriority = 3);
	virtual ~CScene();
	virtual HRESULT Init(D3DXVECTOR3 pos) = 0;
	virtual void Uninit() = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;
	CScene *GetNext();

	static CScene *GetScene(int nPriority);
	static void ReleaseAll();
	static void UpdateAll();
	static void DrawAll();

	void SetObjType(OBJTYPE obj);
	OBJTYPE GetObjType();
	D3DXVECTOR3 GetPos();
	void SetPos(D3DXVECTOR3 pos);
	void SetRadius(float fRadius);
	float GetRadius();
	COLLISION GetCollision();
	void SetCollision(COLLISION collision);
	D3DXVECTOR3 GetVtxMax() { return m_vtxMaxModel; }
	void SetVtxMax(D3DXVECTOR3 VtxMax);
	D3DXVECTOR3 GetVtxMin() { return m_vtxMinModel; }
	void SetVtxMin(D3DXVECTOR3 VtxMin);
	int GetSlack() { return m_nSlack; }
	void SetSlack(int nSlack);
	D3DXVECTOR3 GetCameraPosV() { return m_CameraPosV; }
	void SetCameraPosV(D3DXVECTOR3 posV);
	D3DXVECTOR3 GetCameraPosR() { return m_CameraPosR; }
	void SetCameraPosR(D3DXVECTOR3 posR);
	void SetDeath(bool bDeath);

private:
	static CScene *m_pTop[PRIORITY]; //先頭のオブジェクトのポインタ
	static CScene *m_pCur[PRIORITY]; //現在(一番後ろ)のオブジェクトのポインタ
	CScene *m_pPrev[PRIORITY];       //前のオブジェクトのポインタ
	CScene *m_pNext[PRIORITY];       //後ろのオブジェクトのポインタ

	static int m_nNumAll;
	static bool m_bKey;
	D3DXVECTOR3 m_pos;
	bool m_bDeath;
	float m_fRadius;
	COLLISION m_Collision;
	D3DXVECTOR3 m_vtxMinModel; //モデルの最小値
	D3DXVECTOR3 m_vtxMaxModel; //モデルの最大値
	int m_nSlack;
	D3DXVECTOR3 m_CameraPosV;
	D3DXVECTOR3 m_CameraPosR;

	int m_nPriority;//優先順位

	OBJTYPE m_objType;							//オブジェクトタイプ

protected:
	void Release();
};
#endif