//=============================================================================
//ゲーム管理[Game.h]
// Author : 三上航世
//=============================================================================
#ifndef _GAME_H_
#define _GAME_H_

#include "manager.h"
#include "scene.h"

//*****************************************************************************
//前方宣言
//*****************************************************************************

class CGame : public CScene
{
	typedef struct
	{
		int nTex;			//テクスチャの番号
		int nShadowTex;		//影テクスチャの番号
		D3DXVECTOR2 size;	//大きさ
		int nHP;			//体力
		int nScore;			//スコア
		int nMoney;			//金
	} TypeSpec;

	typedef struct
	{
		int nType;
		D3DXVECTOR3 pos;	//出現位置
		float fRotY;		//向いてる方向
		float fInterval;	//最初撃つまでの時間
		float fSpeed;		//速さ
		float fBulletMax;	//2発目以降撃つまでの時間
		int nTime;			//出てくる時間
		int nBulletType;	//弾の撃ち方
		float fBulletSpeed;	//弾速
	} EnemySpec;

	typedef struct
	{
		int nTex;			//テクスチャの番号
		int nShadowTex;		//影テクスチャの番号
		D3DXVECTOR3 pos;	//出現位置
		D3DXVECTOR2 size;	//大きさ
		int nHP;			//体力
		float fInterval;	//最初撃つまでの時間
		float fBulletMax;	//2発目以降撃つまでの時間
		int nScore;			//スコア
		int nMoney;			//金
		int nTime;			//出てくる時間
		bool bParts;		//パーツ持ちか
		int nBGMType;
	} BossSpec;

public:
	CGame(PRIORITY Priority);
	~CGame();

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

	static CGame *Create();

private:
	void Load();
	TypeSpec m_Type[6];
	EnemySpec m_Enemy[64];
	BossSpec m_Boss;
	int m_nBgTex;
	int m_nTime;
	int m_nCntEnemy;
	bool m_bBoss;
};

#endif // _GAME_H_
