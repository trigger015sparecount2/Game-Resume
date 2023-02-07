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
	//戦闘機の情報
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

	//どっち選んだか
	typedef enum
	{
		SELECT_NONE = 0,
		SELECT_LEFT,
		SELECT_RIGHT,
		SELECT_MAX
	} SELECT;

	CSelect(PRIORITY Priority);
	~CSelect();

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

	static CSelect *Create();	//作成

	//戦闘機の情報取得
	static float GetSpeed() { return m_aAircraft[CManager::GetType()].fSpeed; }
	static int GetSelectHP() { return m_aAircraft[CManager::GetType()].nHP; }
	static CPlayer::SHOTTYPE GetShotType() { return m_aAircraft[CManager::GetType()].ShotType; }
	static int GetTexType() { return m_aAircraft[CManager::GetType()].nTexType; }
	static int GetHudTexType() { return m_aAircraft[CManager::GetType()].nHudTexType; }
	static int GetShadowTexType() { return m_aAircraft[CManager::GetType()].nShadowTex; }
	static int GetMaxSlow() { return m_aAircraft[CManager::GetType()].nMaxSlow; }
	static int GetMaxReturn() { return m_aAircraft[CManager::GetType()].nMaxReturn; }

	static Aircraft m_aAircraft[6];	//戦闘機の情報

private:
	void Load();			//戦闘機読み込み
	void RankLoad();		//ランキング読み込み
	void Select(int nAdd);	//選択肢変更
	void StageSelect();		//ステージ選択開始
	void StageCancel();		//ステージ選択解除
	void Buy();				//機体購入
	void Text();			//文章変更

	CKeyboard *m_pKeyboard;		//キーボード
	CGamePad *m_pGamePad;		//ゲームパッド
	CUI *m_pUI[4 * 6 + 2 + 5];	//機体UI
	CUI *m_pStageUI[5 * 3];		//ステージUI
	char m_Name[128];			//名前文章
	char m_Str[512];			//説明文章
	int m_nClear;				//文字の透明度
	bool m_bClear;				//透明度濃くするか薄くするか
	int m_nSelect[2];			//機体、ステージどれ選んでるか
	int m_nCntSelect;			//選んだカウント
	int m_nCntGauge;			//ゲージ伸びるカウント
	int m_nCntChange;			//文章変更カウント
	float m_fStageClear;		//ステージ選択UI透明度
	bool m_bBuy;				//購入するか
	bool m_bStage;				//ステージ選択中か
	int m_nRank[3];				//各ステージのランキング
	LPD3DXFONT m_pFont[4];		//フォントへのポインタ
	SELECT m_Select;			//どっち選択したか
};

#endif // _TITLE_H_
