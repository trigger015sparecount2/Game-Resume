//=============================================================================
//ゲーム管理[Game.h]
// Author : 三上航世
//=============================================================================
#ifndef _GAME_H_
#define _GAME_H_

#include "manager.h"
#include "Scene.h"

#define MAX_ROUND (8)

//*****************************************************************************
//前方宣言
//*****************************************************************************
class CGamePad;

class CGame : public CScene
{
public:
	CGame(PRIORITY Priority);
	~CGame();

	HRESULT Init(D3DXVECTOR3 pos);	//初期化
	void Uninit();	//終了
	void Update();	//更新
	void Draw();	//描画

	//使わない↓
	OBJTYPE GetObjType() { return OBJECTTYPE_NONE; }
	D3DXVECTOR3 GetPos() { return D3DXVECTOR3(0.0f, 0.0f, 0.0f); }
	D3DXVECTOR3 GetRot() { return D3DXVECTOR3(0.0f, 0.0f, 0.0f); }
	void SetPos(D3DXVECTOR3) { ; }
	float GetRadius() { return 0.0f; }
	float GetHeight() { return 0.0f; }
	COLLISION GetCollision() { return COLLISION_SPHERE; }
	D3DXVECTOR3 GetVtxMax() { return D3DXVECTOR3(0.0f, 0.0f, 0.0f); }
	D3DXVECTOR3 GetVtxMin() { return D3DXVECTOR3(0.0f, 0.0f, 0.0f); }
	void Damage(int, D3DXVECTOR3) { ; }
	bool GetRotX() { return false; }
	bool CollisionDamage(D3DXVECTOR3, float, float) { return false; }
	bool SetEnemy(CScene *pEnemy) { return false; }
	bool Land() { return false; }
	int Damage() { return 0; }
	float HClash() { return 0.0f; }
	float VClash() { return 0.0f; }
	void SetParticle() { ; }
	bool GetAttack() { return false; }
	void Reflect() { ; }
	int GetHP() { return 0; }
	int GetMaxHP() { return 0; }
	D3DXMATRIX GetMatrix(int) { return D3DXMATRIX(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f); }
	//使わない↑

	static CGame *Create();	//作成

	static int GetRound();											//ラウンド数
	static void SetCountFlag(bool bFlag) { m_bCountFlag = bFlag; }	//カウントダウン中
	static bool GetRoundChange() { return m_bRoundChange; }			//ラウンド数更新したか
	static int GetSelectNum() { return m_SelectNum; };				//選択番号取得

private:
	static bool m_bCountFlag;		//カウントダウン中かどうか
	static int m_SelectNum;			//選択番号
	static int m_nRound;			//ラウンド数
	static bool m_bRoundChange;		//ラウント数更新したか
};

#endif // _GAME_H_
