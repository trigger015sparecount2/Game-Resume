//=============================================================================
//
// �V�[������ [scene.h]
// Author : �O��q��
//
//=============================================================================
#ifndef _SCENE_H_
#define _SCENE_H_
#include "main.h"

//*****************************************************************************
//�}�N��
//*****************************************************************************
#define FILE_SAVE1 "data/FILES/save1.txt"
#define FILE_SAVE2 "data/FILES/save2.txt"
#define FILE_SAVE3 "data/FILES/save3.txt"
#define FILE_AIRCRAFT "data/FILES/aircraft.txt"
#define FILE_RANK "data/FILES/rank.txt"

class CScene
{
public:

	//�I�u�W�F�N�g�̎��
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

	//�`�揇
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
	virtual HRESULT Init(D3DXVECTOR3 pos) = 0;
	virtual void Uninit() = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;
	virtual OBJTYPE GetObjType() = 0;
	virtual D3DXVECTOR3 GetPos() = 0;
	virtual D3DXVECTOR3 GetRot() = 0;
	virtual D3DXVECTOR2 GetSize() = 0;
	virtual float GetSizeRot() = 0;
	virtual void Damage(int nDamage) = 0;
	virtual int Damage() = 0;
	virtual int GetHP() = 0;
	virtual int GetMaxHP() = 0;
	virtual bool GetEnemy() = 0;
	virtual void AddScore(int nScore) = 0;
	virtual CScene *GetNext() { return m_pNext; }

	static CScene *GetScene(PRIORITY Priority) { return m_pTop[Priority]; }
	static void ReleaseAll();
	static void UpdateAll();
	static void DrawAll();

	void SetDeath(bool bDeath) { m_bDeath = bDeath; }
	bool GetDeath() { return m_bDeath; }

private:
	static CScene *m_pTop[PRIORITY_MAX]; //�擪�̃I�u�W�F�N�g�̃|�C���^
	static CScene *m_pCur[PRIORITY_MAX]; //����(��Ԍ��)�̃I�u�W�F�N�g�̃|�C���^
	CScene *m_pPrev;       //�O�̃I�u�W�F�N�g�̃|�C���^
	CScene *m_pNext;       //���̃I�u�W�F�N�g�̃|�C���^

	static int m_nNumAll;
	bool m_bDeath;

	PRIORITY m_Priority;//�D�揇��

protected:
	void Release();
};
#endif