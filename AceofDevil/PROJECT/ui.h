//=============================================================================
//
// UI処理 [ui.h]
// Author : 三上航世
//
//=============================================================================
#ifndef _UI_H_
#define _UI_H_
#include "main.h"
#include "scene.h"

#define MAX_UIDATA (52)

class CScene2D;
class CUi : public CScene
{
public:
	CUi(PRIORITY Priority = PRIORITY_UI);
	~CUi();
	HRESULT Init(float fWidth, float fHeight, D3DXVECTOR3 pos, float fTexSize);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	bool GetDamage(void);
	void Damage(int nDamage);

	static void SetLife(int nLife);
	static void SetScore(int nScore);
	static int GetScore(void);
	static void SetKill(void);
	static int GetKill(void);
	static void SetFireGauge(int nFire);
	static void SetIceGauge(int nIce);
	static void SetThunderGauge(int nThunder);
	static void SetWaterGauge(int nWater);
	static void SetWindGauge(int nWind);
	static void ChangeMagic(bool bPlus);
	static void KillClear(D3DXVECTOR3 pos, float fWidth, float fHeight);
	static void ScoreClear(D3DXVECTOR3 pos, float fWidth, float fHeight);
	static void MagicClear(D3DXVECTOR3 pos, float fWidth, float fHeight);
	static void DMGClear(D3DXVECTOR3 pos, float fWidth, float fHeight);
	static void Damage(void);
	static HRESULT Load(void);
	static void UnLoad(void);
	static CUi *Create(void);

private:
	bool m_bClear;
	int m_nClearTime;
	static int m_nDamageTime;
	static int m_nLife;
	static int m_nKill;
	static int m_nScore;
	static int m_nType;
	static float m_fClear[4];
	static D3DXVECTOR3 m_apos[5];
	static LPDIRECT3DTEXTURE9 m_pTexture[23]; //テクスチャへのポインタ
	static CScene2D *m_apScene[MAX_UIDATA];
};

#endif