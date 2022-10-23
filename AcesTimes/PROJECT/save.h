//=============================================================================
// ëIëä«óù[select.h]
// Author : éOè„çqê¢
//=============================================================================
#ifndef _SAVE_H_
#define _SAVE_H_

#include "manager.h"
#include "scene.h"

//*****************************************************************************
//ëOï˚êÈåæ
//*****************************************************************************
class CGamePad;
class CKeyboard;
class CUI;

class CSave : public CScene
{
	typedef enum
	{
		TEXTTYPE_NONE = 0,
		TEXTTYPE_SAVE,
		TEXTTYPE_SAVED,
		TEXTTYPE_SAVEFAILED,
		TEXTTYPE_CLEAR,
		TEXTTYPE_CLEARED,
		TEXTTYPE_CLEARFAILED,
		TEXTTYPE_MAX
	} TEXTTYPE;

public:
	CSave(PRIORITY Priority);
	~CSave();

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

	static CSave *Create();

private:
	void Select(int nAdd);
	void Load();
	void Save();
	void DataReset();
	void TextChange(TEXTTYPE Texttype);

	CKeyboard *m_pKeyboard;
	CGamePad *m_pGamePad;
	CUI *m_pUI[30];
	bool m_aData[3];
	int m_nMoney[3];
	int m_nNumAircraft[3];
	int m_nTexAircraft[6];
	bool m_aAircraft[6][3];
	char m_Str[256];
	int m_nSelect;
	LPD3DXFONT m_pFont[4];
	bool m_bSave;
	bool m_bClear;
	int m_nCntChange;

	int m_nClear[3];
};

#endif // _TITLE_H_
