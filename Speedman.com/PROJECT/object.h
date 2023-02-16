//=============================================================================
//
// オブジェクト処理 [object.h]
// Author : 三上航世
//
//=============================================================================
#ifndef _OBJECT_H_
#define _OBJECT_H_

#include "main.h"
#include "scene3d.h"

class CModel;

class CObject : public CScene3D
{
public:
	CObject(int nPriority);
	~CObject();
	HRESULT Init(D3DXVECTOR3 pos, int nType, COLLISION collision, D3DXVECTOR3 rot, int nMoveTime, D3DXVECTOR3 move, bool bDrop);	//初期化
	void Uninit();	//終了
	void Update();	//更新
	void Draw();	//描画

	static CObject *Create(D3DXVECTOR3 pos, int nType, COLLISION collision, D3DXVECTOR3 rot, int nMoveTime, D3DXVECTOR3 move, bool bDrop);	//作成
	static void Load(int nCnt, const char *aModelName);	//モデル読み込み
	static void UnLoad();	//モデル破棄

private:
	bool Land(D3DXVECTOR3 *pPos, D3DXVECTOR3 posOld);	//床との当たり判定

	CModel *m_pModel;				//モデル
	D3DXVECTOR3 m_move;				//移動量
	D3DXVECTOR3 m_MaxMove;			//最大移動量
	D3DXVECTOR3 m_pos;				//pos
	int m_nTime;					//時間
	int m_nMoveTime;				//移動時間
	bool m_bPlus;					//加算するか減算するか
	bool m_bDrop[2];				//落ちるか

	static CModel *m_paModel[8];	//モデル元
};

#endif