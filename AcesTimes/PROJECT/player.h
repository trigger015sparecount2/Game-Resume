//=============================================================================
//
// ÉVÅ[Éìèàóù [player.h]
// Author : éOè„çqê¢
//
//=============================================================================
#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "main.h"
#include "scene2d.h"

class CKeyboard;
class CGamePad;
class CShadow;
class CUI;
class CPauseUI;
class CWarp;
class CTextUI;

class CPlayer : public CScene2D
{
	typedef struct
	{
		int nHP;
		D3DXVECTOR3 pos;
	} Status;

public:
	typedef enum
	{
		SHOTTYPE_FRONT = 0,
		SHOTTYPE_FRONTWIDE,
		SHOTTYPE_FRONTSIDE,
		SHOTTYPE_FRONTWIDESIDE,
		SHOTTYPE_FRONTWIDEBACKSIDE,
		SHOTTYPE_ALL,
		SHOTTYPE_MAX
	} SHOTTYPE;

	CPlayer(PRIORITY Priority);
	~CPlayer();
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR2 size, int nHP, float fSpeed, int nMaxSlow, int nMaxReturn, SHOTTYPE ShotType, int nShadowTex, int nHudTex);
	void Uninit();
	void Update();
	void Draw();
	OBJTYPE GetObjType() { return OBJECTTYPE_PLAYER; }
	void Damage(int nDamage);
	int Damage() { return 1; }
	int GetHP() { return m_Status.nHP; }
	int GetMaxHP() { return m_nMaxHP; }
	void AddScore(int nScore);
	
	static CPlayer *Create(D3DXVECTOR3 pos, D3DXVECTOR2 size, int nHP, float fSpeed, int nMaxSlow, int nMaxReturn, int nTexType, SHOTTYPE ShotType, int nShadowTex, int nHudTex);

private:
	void Move(float);
	void Stop();
	void CollisionBullet(D3DXVECTOR3);
	void CollisionUI(D3DXVECTOR3 pos);
	void PauseChange(int nAdd);
	void PauseSelect();
	void TimeKeep(D3DXVECTOR3 pos);
	void TimeReturn(D3DXVECTOR3 pos);
	D3DXVECTOR3 Return(D3DXVECTOR3 pos);
	void Shot(D3DXVECTOR3 pos);

	D3DXVECTOR3 m_PosOld;
	D3DXVECTOR3 m_move;
	D3DXVECTOR3 m_WarpMove;
	float m_fMove;
	CKeyboard *m_pKeyboard;
	CGamePad *m_pGamePad;
	CShadow *m_pShadow;
	CScene *m_pEnemy[16];
	Status m_Status;
	Status m_KeepStatus;
	int m_nMaxHP;
	int m_nCntDamage;
	bool m_bPinch;
	bool m_bUIClear;
	int m_nPauseSelect;
	int m_nCntPinch;
	int m_nCntShot;
	int m_nReturn;
	int m_nMaxReturn;
	int m_nSlow;
	int m_nMaxSlow;
	int m_nScore;
	bool m_bWarp;
	bool m_bDeath;
	int m_nCntDeath;
	int m_nCntWarpPoint;
	bool m_bWarpNow;
	int m_nHudTex;
	int m_nCntWarp;
	float m_fHUDColor;
	float m_fClear[3];
	char m_Str[3][32];
	SHOTTYPE m_ShotType;
	CUI *m_pHUD[4];
	CUI *m_pGauge[7];
	CUI *m_pScore[3];
	CPauseUI *m_pPauseUI[3];
	CWarp *m_pWarp;
	CTextUI *m_pTextUI[3];
};
#endif