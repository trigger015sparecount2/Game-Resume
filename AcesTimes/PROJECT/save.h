//=============================================================================
// 選択管理[select.h]
// Author : 三上航世
//=============================================================================
#ifndef _SAVE_H_
#define _SAVE_H_

#include "manager.h"
#include "scene.h"

//*****************************************************************************
//前方宣言
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

	HRESULT Init(D3DXVECTOR3 pos);	//初期化
	void Uninit();	//終了
	void Update();	//更新
	void Draw();	//描画
	OBJTYPE GetObjType() { return OBJECTTYPE_NONE; }	//種類

	//使わない↓
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
	//使わない↑

	static CSave *Create();	//作成

private:
	void Select(int nAdd);				//選択肢変更
	void Load();						//最初の各データ読み込み
	void Save();						//セーブ
	void DataReset();					//データリセット
	void TextChange(TEXTTYPE Texttype);	//文章変更

	CKeyboard *m_pKeyboard;	//キーボード
	CGamePad *m_pGamePad;	//ゲームパッド
	CUI *m_pUI[30];			//UI
	bool m_aData[3];		//各データ読み込み可能か
	int m_nMoney[3];		//各データの金
	int m_nNumAircraft[3];	//各データの使える戦闘機の数
	int m_nTexAircraft[6];	//戦闘機のテクスチャ数字
	bool m_aAircraft[6][3];	//各データの各戦闘機使えるか
	char m_Str[256];		//文章
	int m_nSelect;			//選択肢何選んでるか
	LPD3DXFONT m_pFont[4];	//フォント
	bool m_bSave;			//セーブするか
	bool m_bClear;			//消去するか
	int m_nCntChange;		//文章
	int m_nClear[3];		//各データの文章の透明度
};

#endif // _TITLE_H_
