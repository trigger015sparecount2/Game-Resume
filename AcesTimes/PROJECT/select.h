//=============================================================================
// 選択管理[select.h]
// Author : 三上航世
//=============================================================================
#ifndef _SELECT_H_
#define _SELECT_H_

#include "manager.h"
#include "scene.h"
#include "player.h"

//*****************************************************************************
//前方宣言
//*****************************************************************************
class CGamePad;
class CKeyboard;
class CUI;

class CSelect : public CScene
{
public:
	typedef struct
	{
		char aName[16];
		char aNickName[32];
		float fSpeed;
		int nHP;
		CPlayer::SHOTTYPE ShotType;
		int nTexType;
		int nHudTexType;
		int nShadowTex;
		int nMaxSlow;
		int nMaxReturn;
		int nMoney;
		int nExplainLines;
		char aExplain[4][128];
	} Aircraft;

	typedef enum
	{
		SELECT_NONE = 0,
		SELECT_LEFT,
		SELECT_RIGHT,
		SELECT_MAX
	} SELECT;

	CSelect(PRIORITY Priority);
	~CSelect();

	HRESULT Init(D3DXVECTOR3 pos);
	void Uninit();
	void Update();
	void Draw();
	OBJTYPE GetObjType() { return OBJECTTYPE_NONE; }
	D3DXVECTOR3 GetPos() { return D3DXVECTOR3(0.0f, 0.0f, 0.0f); }
	D3DXVECTOR3 GetRot() { return D3DXVECTOR3(0.0f, 0.0f, 0.0f); }
	D3DXVECTOR2 GetSize() { return D3DXVECTOR2(0.0f, 0.0f); }
	float GetSizeRot() { return 0.0f; }
	void Damage(int) { ; }
	int Damage() { return 0; }
	int GetHP() { return 0; }
	int GetMaxHP() { return 0; }
	bool GetEnemy() { return false; }
	void AddScore(int) { ; }

	static CSelect *Create();
	static float GetSpeed() { return m_aAircraft[CManager::GetType()].fSpeed; }
	static int GetSelectHP() { return m_aAircraft[CManager::GetType()].nHP; }
	static CPlayer::SHOTTYPE GetShotType() { return m_aAircraft[CManager::GetType()].ShotType; }
	static int GetTexType() { return m_aAircraft[CManager::GetType()].nTexType; }
	static int GetHudTexType() { return m_aAircraft[CManager::GetType()].nHudTexType; }
	static int GetShadowTexType() { return m_aAircraft[CManager::GetType()].nShadowTex; }
	static int GetMaxSlow() { return m_aAircraft[CManager::GetType()].nMaxSlow; }
	static int GetMaxReturn() { return m_aAircraft[CManager::GetType()].nMaxReturn; }

	static Aircraft m_aAircraft[6];

private:
	void Load();
	void RankLoad();
	void Select(int nAdd);
	void StageSelect();
	void StageCancel();
	void Buy();
	void Text();

	CKeyboard *m_pKeyboard;
	CGamePad *m_pGamePad;
	CUI *m_pUI[4 * 6 + 2 + 5];
	CUI *m_pStageUI[5 * 3];
	char m_Name[128];
	char m_Str[512];
	int m_nClear;
	bool m_bClear;
	int m_nSelect[2];
	int m_nCntSelect;
	int m_nCntGauge;
	int m_nCntChange;
	float m_fStageClear;
	bool m_bBuy;
	bool m_bStage;
	int m_nRank[3];
	LPD3DXFONT m_pFont[4]; // フォントへのポインタ
	SELECT m_Select;
};

#endif // _TITLE_H_
