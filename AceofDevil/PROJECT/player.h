//=============================================================================
//
// プレイヤー処理 [player.h]
// Author : 三上航世
//
//=============================================================================
#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "main.h"
#include "scene2d.h"

#define MAX_TYPE (5)

class CInputKeyboard;
class CInputGamePad;
class CWater;
class CWind;

class CPlayer : public CScene2D
{
public:
	CPlayer(PRIORITY Priority = PRIORITY_CHARA);
	~CPlayer();
	HRESULT Init(D3DXVECTOR3 pos);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	bool GetDamage(void);
	void Damage(int nDamage);

	static HRESULT Load(void);
	static void UnLoad(void);
	static CPlayer *Create(D3DXVECTOR3 pos);
	static void GetPrize(int nType, int nHeal);
	static bool GetGameOver(void);
	static void SetTuto(bool bTuto);

private:
	void Move(float fRot);
	void Stop(void);
	int SetBulletTime(int nType);
	void SetGauge(void);
	void Shot(D3DXVECTOR3 pos);

	CInputKeyboard *m_pInputKeyboard;
	CInputGamePad *m_pInputGamePad;
	CWater *m_pWater;
	CWind *m_pWind;
	D3DXVECTOR3 m_move; //移動速度
	D3DXVECTOR3 m_Maxmove;
	CScene2D *m_pShadowScene;
	int m_nBullet; //弾撃ってからのカウンタ
	int m_nBulletTime; //発射間隔
	int m_nHP; //体力
	int m_nDamageTime; //無敵時間
	int m_nDTime; //ダメージを食らってからの時間
	int m_nType;
	int m_nMagicTime; //魔力の回復の間隔
	int m_nSlide;
	bool m_bSlide;
	float m_fScale;
	bool m_bUse;
	static bool m_bDeath;
	static int m_aResource[MAX_TYPE];
	static LPDIRECT3DTEXTURE9 m_pTexture;
	static bool m_bTuto;
};

#endif