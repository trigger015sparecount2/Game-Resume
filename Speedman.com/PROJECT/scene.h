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

	//当たり判定の種類
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
	CScene *GetNext() { return *m_pNext; }

	static CScene *GetScene(int nPriority) { return m_pTop[nPriority]; }
	static void ReleaseAll();
	static void UpdateAll();
	static void DrawAll();

	//オブジェクトタイプの設定
	void SetObjType(OBJTYPE obj) { m_objType = obj; }
	OBJTYPE GetObjType() { return m_objType; }

	//posの設定
	D3DXVECTOR3 GetPos() { return m_pos; }
	void SetPos(D3DXVECTOR3 pos) { m_pos = pos; }

	//半径の設定
	void SetRadius(float fRadius) { m_fRadius = fRadius; }
	float GetRadius() { return m_fRadius; }

	//当たり判定設定
	COLLISION GetCollision() { return m_Collision; }
	void SetCollision(COLLISION collision) { m_Collision = collision; }

	//モデルの最大サイズ設定
	D3DXVECTOR3 GetVtxMax() { return m_vtxMaxModel; }
	void SetVtxMax(D3DXVECTOR3 VtxMax) { m_vtxMaxModel = VtxMax; }

	//モデルの最小サイズ設定
	D3DXVECTOR3 GetVtxMin() { return m_vtxMinModel; }
	void SetVtxMin(D3DXVECTOR3 VtxMin) { m_vtxMinModel = VtxMin; }

	//スラックラインの番号設定
	int GetSlack() { return m_nSlack; }
	void SetSlack(int nSlack) { m_nSlack = nSlack; }

	//カメラの視点設定
	D3DXVECTOR3 GetCameraPosV() { return m_CameraPosV; }
	void SetCameraPosV(D3DXVECTOR3 posV) { m_CameraPosV = posV; }

	//カメラの注視点設定
	D3DXVECTOR3 GetCameraPosR() { return m_CameraPosR; }
	void SetCameraPosR(D3DXVECTOR3 posR) { m_CameraPosR = posR; }

	//次のフレームで消す設定
	void SetDeath(bool bDeath) { m_bDeath = bDeath; }

private:
	static CScene *m_pTop[PRIORITY];	//先頭のオブジェクトのポインタ
	static CScene *m_pCur[PRIORITY];	//現在(一番後ろ)のオブジェクトのポインタ
	CScene *m_pPrev[PRIORITY];			//前のオブジェクトのポインタ
	CScene *m_pNext[PRIORITY];			//後ろのオブジェクトのポインタ

	static int m_nNumAll;		//何個シーンあるか
	//static bool m_bKey;
	D3DXVECTOR3 m_pos;			//場所
	bool m_bDeath;				//次消す
	float m_fRadius;			//半径
	COLLISION m_Collision;		//当たり判定
	D3DXVECTOR3 m_vtxMinModel;	//モデルの最小値
	D3DXVECTOR3 m_vtxMaxModel;	//モデルの最大値
	int m_nSlack;				//スラックラインの番号
	D3DXVECTOR3 m_CameraPosV;	//カメラ視点
	D3DXVECTOR3 m_CameraPosR;	//カメラ注視点

	int m_nPriority;			//優先順位

	OBJTYPE m_objType;			//オブジェクトタイプ

protected:
	void Release();				//削除
};
#endif