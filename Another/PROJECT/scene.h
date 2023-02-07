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
//#define POLYGON_SIZE (70.0f)
//#define PRIORITY (7)
//#define CIRCLE (float(rand() % 324) / 100.0f - float(rand() % 324) / 100.0f) //円

class CScene
{
public:

	//オブジェクトの種類
	typedef enum
	{
		OBJECTTYPE_NONE = 0,
		OBJECTTYPE_UI,
		OBJECTTYPE_PLAYER,
		OBJECTTYPE_ENEMY,
		OBJECTTYPE_FIELD,
		OBJECTTYPE_WALL,
		OBJECTTYPE_BLOCK,
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

	//優先順位
	typedef enum
	{
		PRIORITY_PLANE = 0,
		PRIORITY_OBJECT,
		PRIORITY_CHARA,
		PRIORITY_EFFECT,
		PRIORITY_ORBIT,
		PRIORITY_UIEFFECT,
		PRIORITY_UI,
		PRIORITY_PAUSEUI,
		PRIORITY_MAX
	} PRIORITY;

	CScene(PRIORITY Priority);
	virtual ~CScene();
	virtual HRESULT Init(D3DXVECTOR3 pos) = 0;	//初期化
	virtual void Uninit() = 0;				//終了
	virtual void Update() = 0;				//更新
	virtual void Draw() = 0;				//描画

	virtual OBJTYPE GetObjType() = 0;		//種類
	virtual D3DXVECTOR3 GetPos() = 0;		//場所取得
	virtual D3DXVECTOR3 GetRot() = 0;		//方向取得
	virtual float GetRadius() = 0;			//半径取得
	virtual float GetHeight() = 0;			//高さ取得
	virtual COLLISION GetCollision() = 0;	//当たり判定の種類取得
	virtual D3DXVECTOR3 GetVtxMax() = 0;	//最大の大きさ取得
	virtual D3DXVECTOR3 GetVtxMin() = 0;	//最小の大きさ取得
	virtual void Damage(int nDamage, D3DXVECTOR3 clash) = 0;	//ダメージ処理
	virtual bool GetRotX() = 0;	//床がどの方向に傾いているか
	virtual bool CollisionDamage(D3DXVECTOR3 pos, float fRadius, float fHeight) = 0;	//ダメージ判定との当たり判定
	virtual bool SetEnemy(CScene *pEnemy) = 0;	//敵をリスト内に入れる
	virtual int Damage() = 0;		//ダメージ量
	virtual float VClash() = 0;		//垂直方向のノックバック
	virtual float HClash() = 0;		//水平方向のノックバック
	virtual D3DXMATRIX GetMatrix(int nPart) = 0;	//モデルパーツのマトリックス
	virtual void SetParticle() = 0;	//パーティクル作る
	virtual bool GetAttack() = 0;	//攻撃中か
	virtual void Reflect() = 0;		//ガードされた
	virtual int GetHP() = 0;		//現在HP取得
	virtual int GetMaxHP() = 0;		//最大HP取得
	virtual CScene *GetNext() { return m_pNext; }	//次のシーン取得

	static CScene *GetScene(PRIORITY Priority) { return m_pTop[Priority]; }	//最初のシーン取得
	static void ReleaseAll();	//全部削除
	static void UpdateAll();	//更新
	static void DrawAll();		//描画

	//削除の格納・取得
	void SetDeath(bool bDeath) { m_bDeath = bDeath; }
	bool GetDeath() { return m_bDeath; }

private:
	static CScene *m_pTop[PRIORITY_MAX]; //先頭のオブジェクトのポインタ
	static CScene *m_pCur[PRIORITY_MAX]; //現在(一番後ろ)のオブジェクトのポインタ
	CScene *m_pPrev;       //前のオブジェクトのポインタ
	CScene *m_pNext;       //後ろのオブジェクトのポインタ

	static int m_nNumAll;	//今今何個オブジェクトあるか
	bool m_bDeath;			//削除するか

	PRIORITY m_Priority;	//優先順位

protected:
	void Release();
};
#endif