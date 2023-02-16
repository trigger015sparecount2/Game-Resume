//=============================================================================
//
// プレイヤー処理 [player.h]
// Author : 三上航世
//
//=============================================================================
#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "main.h"
#include "scene3d.h"

class CKeyboard;
class CGamePad;
class CMouse;
class CModel;
class CCamera;
class CShadow;

class CPlayer : public CScene3D
{
	//モーションの種類
	typedef enum
	{
		MOTIONTYPE_NEUTRAL = 0,
		MOTIONTYPE_RUN,
		MOTIONTYPE_JUMP,
		MOTIONTYPE_LANDING,
		MOTIONTYPE_ZIPNUETRAL,
		MOTIONTYPE_ZIPMOVE,
		MOTIONTYPE_RETURN, //ニュートラルモーションに戻る際のモーション。txtファイルに書く必要はない
		MOTIONTYPE_ZIPRETURN,
		MAX_MOTION
	} MOTIONTYPE;

	//キーの構造体
	typedef struct
	{
		float fPosX;	//位置X
		float fPosY;	//位置Y
		float fPosZ;	//位置Z
		float fRotX;	//向きX
		float fRotY;	//向きY
		float fRotZ;	//向きZ
	} KEY;

	//キー情報の構造体
	typedef struct
	{
		int nFrame;		//フレーム
		KEY aKey[32];	//各モデルのキー要素 []内はパーツの最大数
	} KEY_INFO;

	//モーション情報の構造体
	typedef struct
	{
		bool bLoop;		//ループするかどうか
		int nNumKey;	//キーの総数
		int nNonOpe;	//操作不能時間
		KEY_INFO aKeyInfo[16]; //キー情報 []内はキーの最大数
	} MOTION_INFO;

public:
	CPlayer(int nPriority);
	~CPlayer();

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, const char *aFileName);	//初期化
	void Uninit();	//終了
	void Update();	//更新
	void Draw();	//描画

	static CPlayer *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, const char *aFileName);	//作成
	static int GetControlNum() { return m_nControl; }	//操作説明の種類取得

	//ライフ設定
	static void SetLife(int Life) { m_nLife = Life; }
	static int GetLife() { return m_nLife; }

private:
	void Load(const char *aFileName);								//モデル読み込み
	void Move(float fRotY);											//移動
	void SlackMove(float fRotY);									//スラックライン移動
	void Stop();													//停止
	void Goal(D3DXVECTOR3 PlayerPos);								//ゴールとの当たり判定
	void UnderSearch(D3DXVECTOR3 pos);								//真下にある床やものを調べる
	void Trampoline(D3DXVECTOR3 *pPos, D3DXVECTOR3 posOld);			//トランポリンとの当たり判定
	void CollisionObject(D3DXVECTOR3 *pPos, D3DXVECTOR3 posOld);	//障害物との当たり判定
	bool Slackline(D3DXVECTOR3 *pPos);								//ロープ掴めるか
	bool Land(D3DXVECTOR3 *pPos, D3DXVECTOR3 posOld);				//着地判定
	bool CatchSlackline(D3DXVECTOR3 *pPos);							//ロープ掴む
	D3DXVECTOR3 SearchSlackline(int nNumber);						//対応するスラックライン検索
	void CameraWarp(D3DXVECTOR3 pos);								//カメラパネルの当たり判定
	void Motion();													//モーション
	void MotionChange(MOTIONTYPE motionType);						//モーション切り替え

	D3DXVECTOR3 m_PosOld;		//過去位置
	D3DXVECTOR3 m_move;			//移動ベクトル
	D3DXVECTOR3 m_rotDesh;		//目的の方向
	D3DXVECTOR3 m_RestartPos;	//リスタートする位置
	float m_fRot;				//ロープ掴んだ際の方向
	float m_fMove;				//移動量
	float m_fGravity;			//重力
	float m_fJump;				//ジャンプ力
	float m_fHeight;			//身長
	bool m_bJump;				//着地してるか
	bool m_bSlack;				//すぐ近くにスラックラインのポールがあるか
	bool m_bSlackMove;			//ロープ掴んでるか
	bool m_bLandObject;			//オブジェクトに乗ってるか
	int m_nNumModel;			//モデル何個使ってるか
	MOTIONTYPE m_motionType;	//モーションの種類
	int m_nCntMotion;			//モーションのフレーム
	MOTION_INFO m_aMotionInfo[MAX_MOTION];	//全モーションの情報
	MOTION_INFO m_MotionInfo;	//今のモーションの情報
	int m_nNeutralTime;			//操作してない時間
	KEY m_aKeyDiff[32];			//パーツごとのキーの加算量
	int m_nKey;					//今のキー数

	CModel *m_pModel[32];		//モデル（パーツ）
	CKeyboard *m_pKeyboard;		//キーボードのポインタ
	CGamePad *m_pGamePad;		//ゲームパッドのポインタ
	CMouse *m_pMouse;			//マウスのポインタ
	CCamera *m_pCamera;			//カメラのポインタ
	CShadow *m_pShadow;			//影

	bool m_bLanding;			//着地
	static int m_nControl;		//操作説明変更用
	static int m_nLife;			//体力
};
#endif