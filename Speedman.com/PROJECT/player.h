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
		float fPosX; //位置X
		float fPosY; //位置Y
		float fPosZ; //位置Z
		float fRotX; //向きX
		float fRotY; //向きY
		float fRotZ; //向きZ
	} KEY;

	//キー情報の構造体
	typedef struct
	{
		int nFrame; //フレーム
		KEY aKey[32]; //各モデルのキー要素 []内はパーツの最大数
	} KEY_INFO;

	//モーション情報の構造体
	typedef struct
	{
		bool bLoop; //ループするかどうか
		int nNumKey; //キーの総数
		int nNonOpe;
		KEY_INFO aKeyInfo[16]; //キー情報 []内はキーの最大数
	} MOTION_INFO;

public:
	CPlayer(int nPriority);
	~CPlayer();
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, const char *aFileName);
	void Uninit();
	void Update();
	void Draw();

	static CPlayer *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, const char *aFileName);
	static int GetControlNum() { return m_nControl; }
	static void SetLife(int Life) { m_nLife = Life; }
	static int GetLife() { return m_nLife; }

private:
	void Load(const char *aFileName);
	void Move(float fRotY);
	void SlackMove(float fRotY);
	void Stop();
	void Goal(D3DXVECTOR3 PlayerPos);
	void UnderSearch(D3DXVECTOR3 pos);
	void Trampoline(D3DXVECTOR3 *pPos, D3DXVECTOR3 posOld);
	void CollisionObject(D3DXVECTOR3 *pPos, D3DXVECTOR3 posOld);
	bool Slackline(D3DXVECTOR3 *pPos);
	bool Land(D3DXVECTOR3 *pPos, D3DXVECTOR3 posOld);
	bool CatchSlackline(D3DXVECTOR3 *pPos);
	D3DXVECTOR3 SearchSlackline(int nNumber);
	void CameraWarp(D3DXVECTOR3 pos);
	void Motion();
	void MotionChange(MOTIONTYPE motionType);

	D3DXVECTOR3 m_PosOld;
	D3DXVECTOR3 m_move;
	D3DXVECTOR3 m_rotDesh;
	D3DXVECTOR3 m_RestartPos;
	float m_fRot;
	float m_fMove;
	float m_fGravity;
	float m_fJump;
	float m_fHeight;
	bool m_bJump;
	bool m_bSlack;
	bool m_bSlackMove;
	bool m_bLandObject;
	int m_nNumModel;
	MOTIONTYPE m_motionType;
	int m_nCntMotion;
	MOTION_INFO m_aMotionInfo[MAX_MOTION];
	MOTION_INFO m_MotionInfo;
	int m_nNeutralTime;
	KEY m_aKeyDiff[32];
	int m_nKey;
	CModel *m_pModel[32]; //モデル（パーツ）
	CKeyboard *m_pKeyboard;
	CGamePad *m_pGamePad;
	CMouse *m_pMouse;
	CCamera *m_pCamera;
	CShadow *m_pShadow;

	bool m_bLanding;		//着地
	static int m_nControl;	//操作説明変更用
	static int m_nLife;		//体力
};
#endif