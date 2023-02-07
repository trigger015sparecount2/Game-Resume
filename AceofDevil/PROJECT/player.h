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
	HRESULT Init(D3DXVECTOR3 pos);	//初期化
	void Uninit(void);				//終了
	void Update(void);				//更新
	void Draw(void);				//描画
	bool GetDamage(void);			//ダメージを受けるか
	void Damage(int nDamage);		//ダメージ処理

	static HRESULT Load(void);								//テクスチャ読み込み
	static void UnLoad(void);								//テクスチャ破棄
	static CPlayer *Create(D3DXVECTOR3 pos);				//作成
	static void GetPrize(int nType, int nHeal);				//プライズ取得
	static bool GetGameOver(void) { return m_bDeath; }		//ゲームオーバーか
	static void SetTuto(bool bTuto) { m_bTuto = bTuto; }	//チュートリアルか設定

private:
	void Move(float fRot);			//移動
	void Stop(void);				//止まる
	int SetBulletTime(int nType);	//次に撃つまでのインターバルの設定
	void SetGauge(void);			//魔力のゲージ設定
	void Shot(D3DXVECTOR3 pos);		//撃つ

	CInputKeyboard *m_pInputKeyboard;		//キーボードのポインタ
	CInputGamePad *m_pInputGamePad;			//ゲームパッドのポインタ
	CWater *m_pWater;						//水のポインタ
	CWind *m_pWind;							//風のポインタ
	D3DXVECTOR3 m_move;						//移動速度
	D3DXVECTOR3 m_Maxmove;					//最大移動速度
	CScene2D *m_pShadowScene;				//影
	int m_nBullet;							//弾撃ってからのカウンタ
	int m_nBulletTime;						//発射間隔
	int m_nHP;								//体力
	int m_nDamageTime;						//無敵時間
	int m_nDTime;							//ダメージを食らってからの時間
	int m_nType;							//攻撃の種類
	int m_nMagicTime;						//魔力の回復の間隔
	int m_nSlide;							//やられたとき、振動するカウント
	bool m_bSlide;							//やられたとき、左右どっちに振動するか
	float m_fScale;							//大きさ
	bool m_bUse;							//水か風を使ってるか
	static bool m_bDeath;					//ゲームオーバーか
	static int m_aResource[MAX_TYPE];		//魔力のリソース
	static LPDIRECT3DTEXTURE9 m_pTexture;	//テクスチャ
	static bool m_bTuto;					//チュートリアルか
};

#endif