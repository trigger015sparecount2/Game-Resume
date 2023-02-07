//=============================================================================
//
// 敵処理 [enemy.h]
// Author : 三上航世
//
//=============================================================================
#ifndef _ENEMY_H_
#define _ENEMY_H_

#include "main.h"
#include "scene3d.h"
#include "model.h"

class CShadow;

class CEnemy : public CScene3D
{
	//モーションの種類
	typedef enum
	{
		MOTIONTYPE_NEUTRAL = 0,	//ニュートラル
		MOTIONTYPE_RUN,			//走る
		MOTIONTYPE_ACTION,		//攻撃
		MOTIONTYPE_DAMAGEBACK,	//後方からダメージ
		MOTIONTYPE_DAMAGEFRONT,	//前方からダメージ
		MOTIONTYPE_REFLECT,		//防がれた
		MOTIONTYPE_RETURN,		//ニュートラルモーションに戻る際のモーション。txtファイルに書く必要はない
		MAX_MOTION
	} MOTIONTYPE;

	//キーの構造体
	typedef struct
	{
		D3DXVECTOR3 pos;	//場所
		D3DXVECTOR3 rot;	//方向
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
		D3DXVECTOR3 TipposOld;	//軌跡の先端の前回位置
		D3DXVECTOR3 posOld;		//軌跡の根元の前回位置
		D3DXMATRIX TipmtxWorld;	//軌跡の先端のマトリックス
		int nTime;				//消えるまでの時間
		int nStartTime;			//発生する時間
		int nFinishTime;		//終了の時間
		int nIdxParent;			//親モデルの番号
		float fDis;				//消える速さ
		D3DXCOLOR col1;			//色先端
		D3DXCOLOR col2;			//色根本
	} Orbit;

	//ステータスの構造体
	typedef struct
	{
		int nNumModel;	//モデルの個数
		int nHP;		//体力
		int nMaxHP;		//最大体力
		float fMove;	//最大移動量
		float fGravity;	//重力
		float fJump;	//ジャンプ力
		float fHeight;	//当たり判定の高さ
		float fRadius;	//当たり判定の半径
	} Status;

	//モーション情報の構造体
	typedef struct
	{
		bool bLoop;				//ループするかどうか
		bool bGravity;			//重力受けるか
		int nNumKey;			//キーの総数
		Collision Collision[8];	//攻撃の当たり判定
		int nNumCol;			//当たり判定の球の数
		Damage Damage[8];		//ダメージ
		int nNumDmg;			//ダメージの種類
		Orbit Orbit[4];			//軌跡
		int nNumOrbit;			//軌跡の種類
		int nNonOpe;			//操作不能時間
		KEY_INFO aKeyInfo[16];	//キー情報 []内はキーの最大数
	} MOTION_INFO;

	//コピーするもの
	typedef struct
	{
		MOTION_INFO amotionInfo[MAX_MOTION];	//モーションの情報
		Status status;			//ステータス
		CModel *apModel[32];	//モデル
	} Copies;

public:
	CEnemy(PRIORITY Priority);
	~CEnemy();
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot);	//初期化
	void Uninit();	//終了
	void Update();	//更新
	void Draw();	//描画

	OBJTYPE GetObjType() { return OBJECTTYPE_ENEMY; }	//種類
	float GetRadius() { return m_Status.fRadius; }		//当たり判定半径
	float GetHeight() { return m_Status.fHeight; }		//当たり判定高さ
	/*COLLISION GetCollision() { return COLLISION_SPHERE; }
	D3DXVECTOR3 GetVtxMax() { return m_VtxMax; }
	D3DXVECTOR3 GetVtxMin() { return m_VtxMin; }*/
	void Damage(int nDamage, D3DXVECTOR3 clash);	//ダメージ
	bool CollisionDamage(D3DXVECTOR3 pos, float fRadius, float fHeight);	//当たり判定くらうか
	int Damage() { return m_MotionInfo.Damage[m_nDmgNumNow].nDamage; }		//ダメージ量
	float HClash() { return m_MotionInfo.Damage[m_nDmgNumNow].fHClash; }	//攻撃のノックバック水平
	float VClash() { return m_MotionInfo.Damage[m_nDmgNumNow].fVClash; }	//攻撃のノックバック高さ
	void SetParticle();							//パーティクル作る
	bool GetAttack() { return m_bAttackMove; }	//攻撃しようとしてるか取得
	void Reflect();								//攻撃弾かれた
	int GetHP() { return m_Status.nHP; }		//現在HP
	int GetMaxHP() { return m_Status.nMaxHP; }	//最大HP
	D3DXMATRIX GetMatrix(int nPart) { return m_pModel[nPart]->GetMatrix(); }	//モデルパーツのマトリックス

	static CEnemy *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot);	//作成
	static void Load(const char *aFileName);					//ファイル読み込み
	static void UnLoad();										//モデル破棄
	static int GetEnemies() { return m_nEnemies; }				//敵の数

private:
	void Move(float fRotY);							//移動
	void MotionMove(float fMove, float fHeight);	//モーションの移動
	void Stop();									//停止
	void UnderSearch(D3DXVECTOR3 pos);				//影の設定
	void CollisionPos(int nCntCol);					//攻撃の当たり判定設定
	void CollisionObject(D3DXVECTOR3 *pPos, D3DXVECTOR3 posOld);				//障害物との当たり判定
	void CollisionPlane(D3DXVECTOR3 *pPos, D3DXVECTOR3 posOld);					//平面の当たり判定
	bool CollisionLand(D3DXVECTOR3 *pPos, D3DXVECTOR3 posOld, CScene *pScene);	//地面との当たり判定
	void CollisionWall(D3DXVECTOR3 *pPos, D3DXVECTOR3 posOld, CScene *pScene);	//壁との当たり判定
	void CollisionEnemy(D3DXVECTOR3 *pPos);										//敵との当たり判定
	void Motion();														//モーション
	void MotionChange(MOTIONTYPE motionType);							//モーション変更
	void EnemyDamage(D3DXVECTOR3 pos, float fRadius, float fHeight);	//敵のダメージ
	void EnemyAI();					//敵のAI
	bool EnemyAttack();				//敵が複数体攻撃するか
	void AttackAI(D3DXVECTOR3 pos);	//攻撃時のAI
	D3DXVECTOR3 SearchPlayer(void);	//プレイヤー探す
	void DataCopy();				//モデルなどのコピー

	D3DXVECTOR3 m_PosOld;		//過去場所
	D3DXVECTOR3 m_move;			//移動
	D3DXVECTOR3 m_motionMove;	//モーションの移動量
	D3DXVECTOR3 m_rotDesh;		//目的の向き
	Status m_Status;			//ステータス
	bool m_bJump;				//着地してるか
	bool m_bLandObject;			//障害物に乗ってるか
	MOTIONTYPE m_motionType;	//モーションの種類
	int m_nCntMotion;			//キーごとのカウント
	MOTION_INFO m_aMotionInfo[MAX_MOTION];	//モーション全種
	MOTION_INFO m_MotionInfo;	//現在モーション
	KEY m_aKeyDiff[32];			//キーごとの差分
	int m_nKey;					//今のキー数
	int m_nCntThinkTime;		//考える時間
	int m_nCntMoveTime;			//移動時間
	CModel *m_pModel[32];		//モデル（パーツ）
	CShadow *m_pShadow;			//影
	bool m_bLanding;			//着地
	bool m_bAttackMove;			//攻撃するために移動してるか
	bool m_bAttack;				//攻撃中か
	bool m_bHit;				//当たったか
	int m_nDmgNumNow;			//ダメージの種類
	int m_nTotalTime;			//モーションのトータルカウント
	int m_nCntCol;				//ダメージを与えた当たり判定の種類
	int m_nCntDamage;			//モーションの当たり判定のカウント

	static Copies m_Copies;		//コピーする情報
	static int m_nEnemies;		//敵の数
};
#endif