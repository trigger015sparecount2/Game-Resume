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

	HRESULT Init(D3DXVECTOR3 pos);	//初期化
	void Uninit();	//終了
	void Update();	//更新
	void Draw();	//描画
	OBJTYPE GetObjType() { return OBJECTTYPE_NONE; }	//種類

	//使わない↓
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
	//使わない↑

	static CResult *Create();	//作成

private:
	//ランキングのロード、セーブ
	void RankLoad();
	void RankSave();

	CKeyboard *m_pKeyboard;	//キーボード
	CGamePad *m_pGamePad;	//ゲームパッド
	LPD3DXFONT m_pFont[4];	//文字
	int m_nScore[3];		//スコア
	CUI *m_pUI;				//点滅UI
	bool m_bClear;			//点滅
	float m_fClear;			//透明度
};

#endif // _RESULT_H_
