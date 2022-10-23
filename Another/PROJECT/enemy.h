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
		MOTIONTYPE_NEUTRAL = 0,
		MOTIONTYPE_RUN,
		MOTIONTYPE_ACTION,
		MOTIONTYPE_DAMAGEBACK,
		MOTIONTYPE_DAMAGEFRONT,
		MOTIONTYPE_REFLECT,
		MOTIONTYPE_RETURN, //ニュートラルモーションに戻る際のモーション。txtファイルに書く必要はない
		MAX_MOTION
	} MOTIONTYPE;

	//キーの構造体
	typedef struct
	{
		D3DXVECTOR3 pos;
		D3DXVECTOR3 rot;
	} KEY;

	//キー情報の構造体
	typedef struct
	{
		int nFrame; //フレーム
		float fMove; //そのキーの水平移動量
		float fHeight; //そのキーの垂直移動量
		KEY aKey[32]; //各モデルのキー要素 []内はパーツの最大数
	} KEY_INFO;

	//当たり判定の構造体
	typedef struct
	{
		D3DXVECTOR3 pos; //位置
		D3DXMATRIX mtxWorld; //ワールドマトリックス
		int nIdxModelParent; //親モデルのインデックス
		float fRadius; //当たり判定の大きさの半径
	} Collision;

	//ダメージ関連の構造体
	typedef struct
	{
		int nDamageTime; //ヒット間隔
		int nStartTime; //当たり判定が発生する時間
		int nFinishTime; //当たり判定が消える時間
		int nDamage; //ダメージ量
		float fHClash; //水平方向の吹っ飛ばし力
		float fVClash; //垂直方向の吹っ飛ばし力
	} Damage;

	//軌跡の構造体
	typedef struct
	{
		D3DXVECTOR3 pos; //軌跡の先端の位置
		D3DXVECTOR3 TipposOld; //軌跡の先端の前回位置
		D3DXVECTOR3 posOld; //軌跡の根元の前回位置
		D3DXMATRIX TipmtxWorld;
		int nTime; //消えるまでの時間
		int nStartTime; //発生する時間
		int nFinishTime; //終了の時間
		int nIdxParent;
		float fDis; //消える速さ
		D3DXCOLOR col1;	//色先端
		D3DXCOLOR col2; //色根本
	} Orbit;

	//ステータスの構造体
	typedef struct
	{
		int nNumModel;
		int nHP;
		int nMaxHP;
		float fMove;
		float fGravity;
		float fJump;
		float fHeight;
		float fRadius;
	} Status;

	//モーション情報の構造体
	typedef struct
	{
		bool bLoop;	//ループするかどうか
		bool bGravity;
		int nNumKey;	//キーの総数
		Collision Collision[8];
		int nNumCol;	//当たり判定の球の数
		Damage Damage[8];
		int nNumDmg;
		Orbit Orbit[4];	//軌跡
		int nNumOrbit;
		int nNonOpe; //操作不能時間
		KEY_INFO aKeyInfo[16];	//キー情報 []内はキーの最大数
	} MOTION_INFO;

	//コピーするもの
	typedef struct
	{
		MOTION_INFO amotionInfo[MAX_MOTION];
		Status status;
		CModel *apModel[32];
	} Copies;

public:
	CEnemy(PRIORITY Priority);
	~CEnemy();
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot);
	void Uninit();
	void Update();
	void Draw();
	OBJTYPE GetObjType() { return OBJECTTYPE_ENEMY; }
	float GetRadius() { return m_Status.fRadius; }
	float GetHeight() { return m_Status.fHeight; }
	/*COLLISION GetCollision() { return COLLISION_SPHERE; }
	D3DXVECTOR3 GetVtxMax() { return m_VtxMax; }
	D3DXVECTOR3 GetVtxMin() { return m_VtxMin; }*/
	void Damage(int nDamage, D3DXVECTOR3 clash);
	bool CollisionDamage(D3DXVECTOR3 pos, float fRadius, float fHeight);
	int Damage() { return m_MotionInfo.Damage[m_nDmgNumNow].nDamage; }
	float HClash() { return m_MotionInfo.Damage[m_nDmgNumNow].fHClash; }
	float VClash() { return m_MotionInfo.Damage[m_nDmgNumNow].fVClash; }
	void SetParticle();
	bool GetAttack() { return m_bAttackMove; }
	void Reflect();
	int GetHP() { return m_Status.nHP; }
	int GetMaxHP() { return m_Status.nMaxHP; }
	D3DXMATRIX GetMatrix(int nPart) { return m_pModel[nPart]->GetMatrix(); }

	static CEnemy *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot);
	static void Load(const char *aFileName);
	static void UnLoad();
	static int GetEnemies() { return m_nEnemies; }

private:
	void Move(float fRotY);
	void MotionMove(float fMove, float fHeight);
	void Stop();
	void UnderSearch(D3DXVECTOR3 pos);
	void CollisionPos(int nCntCol);
	void CollisionObject(D3DXVECTOR3 *pPos, D3DXVECTOR3 posOld);
	void CollisionPlane(D3DXVECTOR3 *pPos, D3DXVECTOR3 posOld);
	bool CollisionLand(D3DXVECTOR3 *pPos, D3DXVECTOR3 posOld, CScene *pScene);
	void CollisionWall(D3DXVECTOR3 *pPos, D3DXVECTOR3 posOld, CScene *pScene);
	void CollisionEnemy(D3DXVECTOR3 *pPos);
	void Motion();
	void MotionChange(MOTIONTYPE motionType);
	void EnemyDamage(D3DXVECTOR3 pos, float fRadius, float fHeight);
	void EnemyAI();
	bool EnemyAttack();
	void AttackAI(D3DXVECTOR3 pos);
	D3DXVECTOR3 SearchPlayer(void);
	void DataCopy();

	D3DXVECTOR3 m_PosOld;
	D3DXVECTOR3 m_move;
	D3DXVECTOR3 m_motionMove;
	D3DXVECTOR3 m_rotDesh;
	Status m_Status;
	bool m_bJump;
	bool m_bLandObject;
	MOTIONTYPE m_motionType;
	int m_nCntMotion;
	MOTION_INFO m_aMotionInfo[MAX_MOTION];
	MOTION_INFO m_MotionInfo;
	int m_nNeutralTime;
	KEY m_aKeyDiff[32];
	int m_nKey;
	int m_nCntThinkTime;
	int m_nCntMoveTime;
	CModel *m_pModel[32];	//モデル（パーツ）
	CShadow *m_pShadow;
	bool m_bLanding;	//着地
	bool m_bAttackMove;
	bool m_bAttack;
	bool m_bHit;
	int m_nDmgNumNow;
	int m_nTotalTime;
	int m_nCntCol;
	int m_nCntDamage;

	static Copies m_Copies;
	static int m_nEnemies;
};
#endif