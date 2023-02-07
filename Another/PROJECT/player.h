//=============================================================================
//
// シーン処理 [player.h]
// Author : 三上航世
//
//=============================================================================
#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "main.h"
#include "scene3d.h"
#include "model.h"

//前方宣言
class CKeyboard;
class CGamePad;
class CMouse;
class CCamera;
class CShadow;
class COrbit;
class CRockon;
class CUI;
class CPauseUI;

class CPlayer : public CScene3D
{
	//モーションの種類
	typedef enum
	{
		MOTIONTYPE_NEUTRAL = 0,	//ニュートラル
		MOTIONTYPE_AIRNEUTRAL,	//空中ニュートラル
		MOTIONTYPE_RUN,			//走る
		MOTIONTYPE_JUMP,		//ジャンプ
		MOTIONTYPE_SLAP,		//至近距離攻撃
		MOTIONTYPE_RUSH,		//突進攻撃
		MOTIONTYPE_ROUND,		//周辺攻撃
		MOTIONTYPE_ARIAL,		//対空攻撃
		MOTIONTYPE_DIVE,		//対地攻撃
		MOTIONTYPE_FINISH,		//フィニッシュ攻撃
		MOTIONTYPE_LANDING,		//着地
		MOTIONTYPE_GROUNDDODGE,	//地上回避
		MOTIONTYPE_AIRDODGE,	//空中回避
		MOTIONTYPE_GUARD,		//ガード
		MOTIONTYPE_COUNTER,		//カウンター
		MOTIONTYPE_DAMAGEFRONT,	//前方からダメージ
		MOTIONTYPE_DAMAGEBACK,	//後ろからダメージ
		MOTIONTYPE_DEATH,		//やられた
		MOTIONTYPE_CLEAR,		//クリア
		MOTIONTYPE_RETURN,		//ニュートラルモーションに戻る際のモーション。txtファイルに書く必要はない
		MOTIONTYPE_AIRRETURN,	//空中ニュートラルモーションに戻る際のモーション。txtファイルに書く必要はない
		MAX_MOTION
	} MOTIONTYPE;

	//モーションの上下動き方
	typedef enum
	{
		MOTIONUPDN_NONE = 0,
		MOTIONUPDN_UNDER,
		MOTIONUPDN_UPPER,
		MOTIONUPDN_UPDN,
		MAX_MOTIONUPDN
	} MOTIONUPDN;

	//キーの構造体
	typedef struct
	{
		D3DXVECTOR3 pos;
		D3DXVECTOR3 rot;
	} KEY;

	//キー情報の構造体
	typedef struct
	{
		int nFrame;		//フレーム
		float fMove;	//そのキーの水平移動量
		float fHeight;	//そのキーの垂直移動量
		KEY aKey[32];	//各モデルのキー要素 []内はパーツの最大数
	} KEY_INFO;

	//当たり判定の構造体
	typedef struct
	{
		D3DXVECTOR3 pos;		//位置
		D3DXMATRIX mtxWorld;	//ワールドマトリックス
		int nIdxModelParent;	//親モデルのインデックス
		float fRadius;			//当たり判定の大きさの半径
	} Collision;

	//ダメージ関連の構造体
	typedef struct
	{
		int nDamageTime;	//ヒット間隔
		int nStartTime;		//当たり判定が発生する時間
		int nFinishTime;	//当たり判定が消える時間
		int nDamage;		//ダメージ量
		float fHClash;		//水平方向の吹っ飛ばし力
		float fVClash;		//垂直方向の吹っ飛ばし力
	} Damage;

	//軌跡の構造体
	typedef struct
	{
		D3DXVECTOR3 pos;		//軌跡の先端の位置
		D3DXMATRIX TipmtxWorld;	//軌跡のマトリックス
		float fDis;				//消える速さ
		D3DXCOLOR col1;			//色先端
		D3DXCOLOR col2;			//色根本
		int nStartTime;			//発生する時間
		int nFinishTime;		//終了の時間
		int nIdxParent;			//親モデルの番号
	} Orbit;

	//モーション情報の構造体
	typedef struct
	{
		bool bLoop;				//ループするかどうか
		bool bHoming;			//突進力などに追尾性があるか(途中で止まるか)
		bool bGravity;			//重力が働くか
		int nNumKey;			//キーの総数
		Collision Collision[8];	//当たり判定
		int nNumCol;			//当たり判定の球の数
		int nComboTime;			//コンボ猶予時間
		Damage Damage[8];		//ダメージ
		int nNumDmg;			//ダメージの数
		Orbit Orbit[4];			//軌跡
		int nNumOrbit;			//軌跡の数
		int nNonOpe;			//操作不能時間
		KEY_INFO aKeyInfo[16];	//キー情報 []内はキーの最大数
		int nNoDmg;				//無敵か
		MOTIONUPDN MotionUpdn;	//モーション上下
	} MOTION_INFO;

public:
	CPlayer(PRIORITY Priority);
	~CPlayer();
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, const char *aFileName);	//初期化
	void Uninit();	//終了
	void Update();	//更新
	void Draw();	//描画
	OBJTYPE GetObjType() { return OBJECTTYPE_PLAYER; }	//種類
	float GetRadius() { return m_fRadius; }				//半径
	float GetHeight() { return m_fHeight; }				//高さ
	//COLLISION GetCollision() { return COLLISION_SPHERE; }
	//D3DXVECTOR3 GetVtxMax() { return D3DXVECTOR3(0.0f, 0.0f, 0.0f); }
	//D3DXVECTOR3 GetVtxMin() { return D3DXVECTOR3(0.0f, 0.0f, 0.0f); }
	void Damage(int nDamage, D3DXVECTOR3 clash);								//ダメージ処理
	bool CollisionDamage(D3DXVECTOR3 pos, float fRadius, float fHeight);		//攻撃の当たり判定に当たったか
	bool SetEnemy(CScene *pEnemy);												//敵をリストに収める
	int Damage() { return m_MotionInfo.Damage[m_nDmgNumNow].nDamage; }			//ダメージ量
	float HClash() { return m_MotionInfo.Damage[m_nDmgNumNow].fHClash; }		//水平方向のノックバック
	float VClash() { return m_MotionInfo.Damage[m_nDmgNumNow].fVClash; }		//垂直方向のノックバック
	D3DXMATRIX GetMatrix(int nPart) { return m_pModel[nPart]->GetMatrix(); }	//モデルのマトリックス
	void SetParticle();					//攻撃ヒット時、パーティクル作る
	int GetHP() { return m_nHP; }		//現在体力
	int GetMaxHP() { return m_nMaxHP; }	//最大体力
	static CPlayer *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, const char *aFileName);	//作成

private:
	void Load(const char *aFileName);			//ファイル読み込み
	bool Move(float fRotY);						//移動
	void MotionMove(float fRotY, float fMove);	//モーションの移動量水平
	void MotionStopMove();						//モーションの移動量水平減少
	void MotionHeight(float fHeight);			//モーションの移動量垂直
	void MotionStopHeight();					//モーションの移動量垂直減少
	bool Stop();								//止まる
	void Jump();								//ジャンプ
	void UnderSearch(D3DXVECTOR3 pos);			//影設定
	void CollisionObject(D3DXVECTOR3 *pPos, D3DXVECTOR3 posOld);				//障害物との当たり判定
	void CollisionPlane(D3DXVECTOR3 *pPos, D3DXVECTOR3 posOld);					//平面との当たり判定
	bool CollisionLand(D3DXVECTOR3 *pPos, D3DXVECTOR3 posOld, CScene *pScene);	//地面との当たり判定
	void CollisionWall(D3DXVECTOR3 *pPos, D3DXVECTOR3 posOld, CScene *pScene);	//壁との当たり判定
	void Motion();								//モーション
	void HasteMotion();							//速いモーション
	void MotionChange(MOTIONTYPE motionType);	//モーション変更
	void CollisionPos(int nCntCol);				//攻撃判定のモーション
	void CollisionEnemy(D3DXVECTOR3 *pPos);		//敵との当たり判定
	void Orbit();								//軌跡
	void Rockon();								//ロックオン
	void UnRockon();							//ロックオン解除
	void ResetEnemy();							//敵リスト消す
	void RockCameraMove(D3DXVECTOR3 pos);		//ロックオン中のカメラ移動
	MOTIONTYPE RockAttack(D3DXVECTOR3 pos);		//ロックオン中の攻撃
	bool AroundEnemy(D3DXVECTOR3 pos);			//周囲に敵いるか
	void SearchRock(D3DXVECTOR3 pos);			//ロックオン中の敵サーチ
	MOTIONTYPE NoRockAttack();					//ロックオンしてない時の攻撃
	void PlayerDamage(D3DXVECTOR3 pos, float fHeight, float fRadius);	//プレイヤーのダメージ
	void PauseChange(int nAdd);	//ポーズ選択肢変更
	void PauseSelect();			//ポーズ決定
	void EndChange(int nAdd);	//終わった後の選択肢変更
	void EndSelect();			//終わった後の選択肢決定
	void GameClear();			//ゲームクリア

	D3DXVECTOR3 m_PosOld;			//過去位置
	D3DXVECTOR3 m_move;				//移動
	D3DXVECTOR3 m_rotDesh;			//目的の向き
	D3DXVECTOR3 m_RestartPos;		//壁などを抜けたときの戻ってくる位置
	float m_fMove;					//最大水平移動量
	float m_fGravity;				//重力
	float m_fJump;					//ジャンプ力
	float m_fHeight;				//高さ
	float m_fRadius;				//半径
	bool m_bJump;					//着地してるか
	bool m_bLandObject;				//オブジェクトに乗ってるか
	int m_nNumModel;				//モデル数
	int m_nTotalTime;				//モーションのトータルカウント
	MOTIONTYPE m_motionType;		//現在モーション
	MOTIONTYPE m_motionTypeOld;		//過去モーション
	int m_nCntMotion;				//キーごとのカウント
	MOTION_INFO m_aMotionInfo[32];	//モーションの情報全種
	MOTION_INFO m_MotionInfo;		//現在モーションの情報
	int m_nNeutralTime;				//ニュートラルの時間
	KEY m_aKeyDiff[32];				//キーの差分
	int m_nKey;						//現在キー
	CModel *m_pModel[32];			//モデル（パーツ）
	CKeyboard *m_pKeyboard;			//キーボード
	CGamePad *m_pGamePad;			//ゲームパッド
	CMouse *m_pMouse;				//マウス
	CCamera *m_pCamera;				//カメラ
	CShadow *m_pShadow;				//影
	CRockon *m_pRockon;				//ロックオン
	CScene *m_pEnemy[16];			//敵のリスト
	COrbit *m_pOrbit[4];			//軌跡
	int m_nHP;						//現在HP
	int m_nMaxHP;					//最大HP
	int m_nRunStop;					//走るのやめたカウント
	int m_nCntDamage;				//攻撃モーションのダメージカウント
	int m_nDmgNumNow;				//ダメージを与えたときのダメージの種類
	int m_nCombo;					//現在コンボ数
	int m_nNonOpe;					//操作不能カウント
	int m_nComboTime;				//コンボするまでの時間
	int m_nCntRun;					//走ってるカウント
	D3DXVECTOR3 m_MotionMove;		//モーションの移動量
	bool m_bRockon;					//ロックオンしてるか
	bool m_bLanding;				//着地
	bool m_bHit;					//攻撃当たったか
	bool m_bAttack;					//攻撃判定出ているか
	bool m_bCounter;				//カウンター可能か
	bool m_bGameClear;				//クリアしたか
	bool m_bEndSelect;				//選択肢出ているか
	bool m_bPinch;					//ピンチか
	int m_nCntCol;					//何番目の当たり判定に当たったか
	int m_nCntNoDmg;				//無敵時間
	int m_nPauseSelect;				//ポーズ中の選択肢番号
	int m_nCntRoundChange;			//ラウンドが変更されてからどれだけかかったかカウント
	int m_nCntDeath;				//やられたカウント
	int m_nCntPinch;				//ピンチ中のカウント
	CUI *m_pCounterUI;				//カウンターUI
	CUI *m_pHPUI;					//HPゲージ
	CUI *m_pEnemyHPUI[3];			//敵のHP
	CPauseUI *m_pPauseUI[3];		//ポーズの選択肢UI
	CUI *m_pRoundUI[4];				//ラウンド数周りのUI
	CUI *m_pEndUI[2];				//終わった後の選択肢UI
};
#endif