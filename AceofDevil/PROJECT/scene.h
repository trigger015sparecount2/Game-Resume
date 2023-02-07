//=============================================================================
//
// シーン処理 [scene.h]
// Author : 三上航世
//
//=============================================================================
#ifndef _SCENE_H_
#define _SCENE_H_
#include "main.h"
#define POLYGON_WIDTH (60.0f)
#define POLYGON_HEIGHT (70.0f)
#define MAX_POLYGON (256)
class CScene
{
public:
	//オブジェクトタイプ
	typedef enum
	{
		OBJTYPE_NONE = 0,
		OBJTYPE_PLAYER,
		OBJTYPE_ENEMY,
		OBJTYPE_BULLET,
		OBJTYPE_EXPLOSION,
		OBJTYPE_LIFE,
		OBJTYPE_SHADOW,
		OBJTYPE_PRIZE,
		OBJTYPE_MAX
	}OBJTYPE;

	//描画順
	typedef enum
	{
		PRIORITY_BG = 0,
		PRIORITY_EFFECT,
		PRIORITY_BULLET,
		PRIORITY_CHARA,
		PRIORITY_UI,
		PRIORITY_MAX
	} PRIORITY;

	//属性
	typedef enum
	{
		ELEMENT_FIRE = 0,
		ELEMENT_ICE,
		ELEMENT_THUNDER,
		ELEMENT_WATER,
		ELEMENT_WIND,
		ELEMENT_MAX
	} ELEMENT;

	//影の種類
	typedef enum
	{
		SHADOW_F_15S_MTD = 0,
		SHADOW_F_14,
		SHADOW_MIG_31,
		SHADOW_F_35,
		SHADOW_F_2,
		SHADOW_RAFALE,
		MAX_SHADOW
	}SHADOW;


	CScene(PRIORITY Priority);
	virtual ~CScene();
	virtual HRESULT Init(float fWidth, float fHeight, D3DXVECTOR3 pos, float fTexSize) = 0;	//初期化
	virtual void Uninit(void) = 0;			//終了
	virtual void Update(void) = 0;			//更新
	virtual void Draw(void) = 0;			//描画
	virtual bool GetDamage(void) = 0;		//ダメージ受けるか
	virtual void Damage(int nDamage) = 0;	//ダメージ処理

	void SetElement(ELEMENT element) { m_Element = element; }
	ELEMENT GetElement(void) { return m_Element; }
	void SetObjType(OBJTYPE objType) { m_objType = objType; }
	OBJTYPE GetObjType(void) { return m_objType; }
	void SetPos(D3DXVECTOR3 pos) { m_pos = pos; }
	D3DXVECTOR3 GetPos(void) { return m_pos; }
	void SetEnemy(bool bEnemy) { m_bEnemy = bEnemy; }
	bool GetEnemy(void) { return m_bEnemy; }
	CScene *GetNext(void) { return m_pNext; }

	static bool GetbEnemy(int nEnemy);	//敵が存在してるかつダメージを受ける
	static D3DXVECTOR3 *GetPointerPos(int nCntScene) { return &m_apScene[PRIORITY_CHARA][nCntScene]->m_pos; }	//posのポインタ返す
	static D3DXVECTOR3 *GetPlayerPos(void);	//プレイヤーのposのポインタ返す
	static CScene *GetScene(PRIORITY Priority, int nCntScene) { return m_apScene[Priority][nCntScene]; }
	//static CScene *GetScene(PRIORITY Priority) { return m_apScene[Priority][0]; }
	static void ReleaseAll(void);
	static void UpdateAll(void);
	static void DrawAll(void);

private:
	static CScene *m_apScene[PRIORITY_MAX][MAX_POLYGON];	//シーン全部
	static CScene *m_pTop[PRIORITY_MAX];	//先頭のオブジェクトのポインタ
	static CScene *m_pCur[PRIORITY_MAX];	//現在(一番後ろ)のオブジェクトのポインタ
	CScene *m_pPrev;		//前のオブジェクトのポインタ
	CScene *m_pNext;		//後ろのオブジェクトのポインタ
	static int m_nNumAll;	//オブジェクト今どれだけあるか
	OBJTYPE m_objType;		//オブジェクトの種類
	D3DXVECTOR3 m_pos;		//場所
	int m_nID;				//ID
	int m_nPriority;		//描画順
	bool m_bEnemy;			//敵か否か
	bool m_bDeath;			//死ぬか
	ELEMENT m_Element;		//属性

protected:
	void Release(void);		//削除
};

#endif