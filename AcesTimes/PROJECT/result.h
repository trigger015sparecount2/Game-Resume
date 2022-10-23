//=============================================================================
//リザルト管理[Result.h]
// Author : 三上航世
//=============================================================================
#ifndef _RESULT_H_
#define _RESULT_H_

#include "manager.h"
#include "scene.h"
//*****************************************************************************
//前方宣言
//*****************************************************************************
class CGamePad;
class CUI;

class CResult : public CScene
{
public:
	CResult(PRIORITY Priority);
	~CResult();

	HRESULT Init(D3DXVECTOR3 pos);
	void Uninit();
	void Update();
	void Draw();
	OBJTYPE GetObjType() { return OBJECTTYPE_NONE; }
	D3DXVECTOR3 GetPos() { return D3DXVECTOR3(0.0f, 0.0f, 0.0f); }
	D3DXVECTOR3 GetRot() { return D3DXVECTOR3(0.0f, 0.0f, 0.0f); }
	D3DXVECTOR2 GetSize() { return D3DXVECTOR2(0.0f, 0.0f); }
	void Damage(int, D3DXVECTOR3) { ; }
	void Damage(int) { ; }
	float GetSizeRot() { return 0.0f; }
	int Damage() { return 0; }
	int GetHP() { return 0; }
	int GetMaxHP() { return 0; }
	bool GetEnemy() { return false; }
	void AddScore(int) { ; }

	static CResult *Create();

private:
	void RankLoad();
	void RankSave();

	CKeyboard *m_pKeyboard;
	CGamePad *m_pGamePad;
	LPD3DXFONT m_pFont[4];
	int m_nScore[3];
	CUI *m_pUI;
	bool m_bClear;
	float m_fClear;
};

#endif // _RESULT_H_
