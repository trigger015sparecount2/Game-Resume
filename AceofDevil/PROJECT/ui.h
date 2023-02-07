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

//マクロ定義
#define MAX_UIDATA (52)

class CScene2D;
class CUi : public CScene
{
public:
	CUi(PRIORITY Priority = PRIORITY_UI);
	~CUi();
	HRESULT Init(float fWidth, float fHeight, D3DXVECTOR3 pos, float fTexSize);	//初期化
	void Uninit(void);						//終了
	void Update(void);						//更新
	void Draw(void);						//描画
	bool GetDamage(void) { return false; }	//ダメージ受けるか
	void Damage(int) { ; }					//ダメージ処理

	static void SetLife(int nLife) { m_nLife = nLife; }	//体力設定
	static void SetScore(int nScore);					//スコア設定
	static int GetScore(void) { return m_nScore; }		//スコア取得
	static void SetKill(void);							//撃墜数設定
	static int GetKill(void) { return m_nKill; }		//撃墜数取得
	static void SetFireGauge(int nFire);				//火属性ゲージ設定
	static void SetIceGauge(int nIce);					//氷属性ゲージ設定
	static void SetThunderGauge(int nThunder);			//雷属性ゲージ設定
	static void SetWaterGauge(int nWater);				//水属性ゲージ設定
	static void SetWindGauge(int nWind);				//風属性ゲージ設定
	static void ChangeMagic(bool bPlus);				//魔法変える
	static void KillClear(D3DXVECTOR3 pos, float fWidth, float fHeight);	//撃墜数透過させる
	static void ScoreClear(D3DXVECTOR3 pos, float fWidth, float fHeight);	//スコア透過させる
	static void MagicClear(D3DXVECTOR3 pos, float fWidth, float fHeight);	//魔法透過させる
	static void DMGClear(D3DXVECTOR3 pos, float fWidth, float fHeight);		//ダメージ透過させる
	static void Damage(void);							//ダメージ処理
	static HRESULT Load(void);							//テクスチャ読み込み
	static void UnLoad(void);							//テクスチャ破棄
	static CUi *Create(void);							//作成

private:
	bool m_bClear;								//自機UIの点滅
	int m_nClearTime;							//自機UIの点滅の時間
	static int m_nDamageTime;					//プレイヤーがダメージくらってからの時間
	static int m_nLife;							//プレイヤーの体力
	static int m_nKill;							//撃墜数
	static int m_nScore;						//スコア
	static int m_nType;							//属性の種類
	static float m_fClear[4];					//UI4種の透明度
	static D3DXVECTOR3 m_apos[5];				//場所
	static LPDIRECT3DTEXTURE9 m_pTexture[23];	//テクスチャへのポインタ
	static CScene2D *m_apScene[MAX_UIDATA];		//シーン
};

#endif