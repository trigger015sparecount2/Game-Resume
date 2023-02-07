//=============================================================================
//
// シーン処理 [player.h]
// Author : 三上航世
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
public:
	//巻き戻しする際に必要な情報
	typedef struct
	{
		int nHP;
		D3DXVECTOR3 pos;
	} Status;

	//ショットの撃ち方
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
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR2 size, int nHP, float fSpeed, int nMaxSlow, int nMaxReturn, SHOTTYPE ShotType, int nShadowTex, int nHudTex);	//初期化
	void Uninit();	//終了
	void Update();	//更新
	void Draw();	//描画
	OBJTYPE GetObjType() { return OBJECTTYPE_PLAYER; }	//種類
	void Damage(int nDamage);							//ダメージ処理
	int Damage() { return 1; }							//ダメージ量
	int GetHP() { return m_Status.nHP; }				//体力
	int GetMaxHP() { return m_nMaxHP; }					//最大体力
	void AddScore(int nScore);							//スコア増加
	
	static CPlayer *Create(D3DXVECTOR3 pos, D3DXVECTOR2 size, int nHP, float fSpeed, int nMaxSlow, int nMaxReturn, int nTexType, SHOTTYPE ShotType, int nShadowTex, int nHudTex);	//作成

private:
	void Move(float fRot);					//移動
	void Stop();							//停止
	void CollisionBullet(D3DXVECTOR3 pos);	//弾との当たり判定
	void CollisionUI(D3DXVECTOR3 pos);		//UIとの当たり判定
	void PauseChange(int nAdd);				//ポーズ選択肢変える
	void PauseSelect();						//ポーズ選ぶ
	void TimeKeep(D3DXVECTOR3 pos);			//リターンポイント設置
	void TimeReturn(D3DXVECTOR3 pos);		//時間の巻き戻し
	D3DXVECTOR3 Return(D3DXVECTOR3 pos);	//巻き戻し中の移動
	void Shot(D3DXVECTOR3 pos);				//撃つ

	D3DXVECTOR3 m_PosOld;		//前回位置
	D3DXVECTOR3 m_move;			//移動
	D3DXVECTOR3 m_WarpMove;		//巻き戻し中の移動量
	float m_fMove;				//最大移動量
	CKeyboard *m_pKeyboard;		//キーボード
	CGamePad *m_pGamePad;		//ゲームパッド
	CShadow *m_pShadow;			//影
	Status m_Status;			//現在ステータス
	Status m_KeepStatus;		//キープ中のステータス
	int m_nMaxHP;				//最大体力
	int m_nCntDamage;			//ダメージくらったカウント
	bool m_bPinch;				//ピンチか
	bool m_bUIClear;			//ピンチ時のUIの自機のシルエット点滅
	int m_nPauseSelect;			//ポーズの選択肢
	int m_nCntPinch;			//ピンチの点滅のカウント
	int m_nCntShot;				//ショットカウント
	int m_nReturn;				//現在巻き戻し値
	int m_nMaxReturn;			//最大巻き戻し値
	int m_nSlow;				//現在爆弾値
	int m_nMaxSlow;				//最大爆弾値
	int m_nScore;				//スコア
	bool m_bWarp;				//巻き戻し地点設置してるか
	bool m_bDeath;				//消すか
	int m_nCntDeath;			//やられたカウント
	int m_nCntWarpPoint;		//リターンポイントのカウント
	bool m_bWarpNow;			//今巻き戻しているか
	int m_nHudTex;				//巻き戻す際の残像処理のテクスチャ
	int m_nCntWarp;				//巻き戻しカウント
	float m_fHUDColor;			//自機のシルエットの色
	float m_fClear[3];			//各UIの透明度
	char m_Str[3][32];			//各文章
	SHOTTYPE m_ShotType;		//撃ち方
	CUI *m_pHUD[4];				//右下のUI
	CUI *m_pGauge[7];			//左下のUI
	CUI *m_pScore[3];			//左上のUI
	CPauseUI *m_pPauseUI[3];	//ポーズUI
	CWarp *m_pWarp;				//巻き戻し地点
	CTextUI *m_pTextUI[3];		//各UIのテキスト
};
#endif