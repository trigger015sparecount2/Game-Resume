//=============================================================================
//
// シーン処理 [scene.h]
// Author : 三上航世
//
//=============================================================================
#ifndef _SCENE_H_
#define _SCENE_H_
#include "main.h"

//*****************************************************************************
//マクロ
//*****************************************************************************
#define FILE_SAVE1 "data/FILES/save1.txt"
#define FILE_SAVE2 "data/FILES/save2.txt"
#define FILE_SAVE3 "data/FILES/save3.txt"
#define FILE_AIRCRAFT "data/FILES/aircraft.txt"
#define FILE_RANK "data/FILES/rank.txt"

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
		OBJECTTYPE_BULLET,
		OBJECTTYPE_BG,
		OBJECTTYPE_BOMB,
		OBJECTTYPE_MAX
	} OBJTYPE;

	//描画順
	typedef enum
	{
		PRIORITY_PLANE = 0,
		PRIORITY_BULLET,
		PRIORITY_SHADOW,
		PRIORITY_CHARA,
		PRIORITY_EFFECT,
		PRIORITY_UI,
		PRIORITY_PAUSEUI,
		PRIORITY_MAX
	} PRIORITY;

	CScene(PRIORITY Priority);
	virtual ~CScene();
	virtual HRESULT Init(D3DXVECTOR3 pos) = 0;		//初期化
	virtual void Uninit() = 0;						//終了
	virtual void Update() = 0;						//更新
	virtual void Draw() = 0;						//描画
	virtual OBJTYPE GetObjType() = 0;				//種類
	virtual D3DXVECTOR3 GetPos() = 0;				//pos取得
	virtual D3DXVECTOR3 GetRot() = 0;				//rot取得
	virtual D3DXVECTOR2 GetSize() = 0;				//サイズ取得
	virtual float GetSizeRot() = 0;					//サイズ変更
	virtual void Damage(int nDamage) = 0;			//ダメージ種類
	virtual int Damage() = 0;						//ダメージ量
	virtual int GetHP() = 0;						//体力取得
	virtual int GetMaxHP() = 0;						//最大体力取得
	virtual bool GetEnemy() = 0;					//敵のものか
	virtual void AddScore(int nScore) = 0;			//スコア加算
	virtual CScene *GetNext() { return m_pNext; }	//次のシーン

	static CScene *GetScene(PRIORITY Priority) { return m_pTop[Priority]; }	//先頭のシーン取得
	static void ReleaseAll();	//全部消去
	static void UpdateAll();	//全部更新
	static void DrawAll();		//全部描画

	void SetDeath(bool bDeath) { m_bDeath = bDeath; }	//消去する
	bool GetDeath() { return m_bDeath; }				//消去するか

private:
	static CScene *m_pTop[PRIORITY_MAX]; //先頭のオブジェクトのポインタ
	static CScene *m_pCur[PRIORITY_MAX]; //現在(一番後ろ)のオブジェクトのポインタ
	CScene *m_pPrev;       //前のオブジェクトのポインタ
	CScene *m_pNext;       //後ろのオブジェクトのポインタ

	static int m_nNumAll;	//オブジェクト個数
	bool m_bDeath;			//消去するか

	PRIORITY m_Priority;	//優先順位

protected:
	void Release();
};
#endif