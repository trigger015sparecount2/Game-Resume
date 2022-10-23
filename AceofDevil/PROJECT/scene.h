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
	typedef enum
	{
		PRIORITY_BG = 0,
		PRIORITY_EFFECT,
		PRIORITY_BULLET,
		PRIORITY_CHARA,
		PRIORITY_UI,
		PRIORITY_MAX
	} PRIORITY;
	typedef enum
	{
		ELEMENT_FIRE = 0,
		ELEMENT_ICE,
		ELEMENT_THUNDER,
		ELEMENT_WATER,
		ELEMENT_WIND,
		ELEMENT_MAX
	} ELEMENT;
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
	virtual HRESULT Init(float fWidth, float fHeight, D3DXVECTOR3 pos, float fTexSize) = 0;
	virtual void Uninit(void) = 0;
	virtual void Update(void) = 0;
	virtual void Draw(void) = 0;
	virtual bool GetDamage(void) = 0;
	virtual void Damage(int nDamage) = 0;

	ELEMENT GetElement(void);
	void SetElement(ELEMENT element);
	void SetObjType(OBJTYPE objType);
	OBJTYPE GetObjType(void);
	void SetPos(D3DXVECTOR3 pos);
	D3DXVECTOR3 GetPos(void);
	void SetEnemy(bool bEnemy);
	bool GetEnemy(void);
	CScene *GetNext(void);

	static bool GetbEnemy(int nEnemy);
	static D3DXVECTOR3 *GetPointerPos(int nCntScene);
	static D3DXVECTOR3 *GetPlayerPos(void);
	static CScene *GetScene(PRIORITY Priority, int nCntScene);
	static CScene *GetScene(PRIORITY Priority);
	static void ReleaseAll(void);
	static void UpdateAll(void);
	static void DrawAll(void);

private:
	static CScene *m_apScene[PRIORITY_MAX][MAX_POLYGON];
	static CScene *m_pTop[PRIORITY_MAX]; //先頭のオブジェクトのポインタ
	static CScene *m_pCur[PRIORITY_MAX]; //現在(一番後ろ)のオブジェクトのポインタ
	CScene *m_pPrev;       //前のオブジェクトのポインタ
	CScene *m_pNext;       //後ろのオブジェクトのポインタ
	static int m_nNumAll;
	OBJTYPE m_objType;
	D3DXVECTOR3 m_pos;
	int m_nID;
	int m_nPriority;
	bool m_bEnemy;
	bool m_bDeath;
	ELEMENT m_Element;

protected:
	void Release(void);
};

#endif