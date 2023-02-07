//=============================================================================
// 選択管理[select.h]
// Author : 三上航世
//=============================================================================
#ifndef _LOAD_H_
#define _LOAD_H_

#include "manager.h"
#include "scene.h"
#include "player.h"

//*****************************************************************************
//前方宣言
//*****************************************************************************
class CGamePad;
class CKeyboard;
class CUI;

class CLoad : public CScene
{
	typedef enum
	{
		TEXTTYPE_NONE = 0,
		TEXTTYPE_LOAD,
		TEXTTYPE_NODATA,
		TEXTTYPE_MAX
	} TEXTTYPE;


public:
	CLoad(PRIORITY Priority);
	~CLoad();

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

	static CLoad *Create();	//作成

private:
	void Select(int nAdd);				//選択肢変更
	void Load();						//最初の各データの読み込み
	HRESULT DataLoad();					//データ読み込み(今のデータを読み込んだデータにする)
	void TextChange(TEXTTYPE Texttype);	//文章変更

	CKeyboard *m_pKeyboard;	//キーボード
	CGamePad *m_pGamePad;	//ゲームパッド
	CUI *m_pUI[30];			//UI
	bool m_aData[3];		//データが存在してるか
	int m_nMoney[3];		//各データの金
	int m_nNumAircraft[3];	//各データの戦闘機の数
	int m_nTexAircraft[6];	//各戦闘機のテクスチャ
	bool m_aAircraft[6][3];	//各データの戦闘機何使えるか
	char m_Str[256];		//下の説明文章
	int m_nSelect;			//何選んでるか
	LPD3DXFONT m_pFont[4];	//各文字
	bool m_bLoad;			//ロードするか
	int m_nCntChange;		//文章変えてから何フレーム経ったか
	int m_nClear[3];		//データの文章の透明度
};

#endif // _TITLE_H_
