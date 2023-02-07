//---------------------------
//Author:三上航世
//プレイヤー(player.cpp)
//---------------------------
#include "player.h"
#include "input.h"
#include "camera.h"
#include "shadow.h"
#include "gamepad.h"
#include "bullet.h"
#include "particle.h"
#include "meshfield.h"
#include "object.h"
#include "enemy.h"
#include "orbit.h"
#include "rockon.h"
#include "sound.h"
#include "wall.h"

//マクロ定義
#define CHARAFILENAME "data/motion_flash.txt"

//プロトタイプ宣言
void PlayerMoveX(void);
void PlayerMoveZ(void);
void PlayerSlowX(void);
void PlayerSlowZ(void);
void PlayerMotion(void);
void MotionChange(MOTIONTYPE motiontype);
void CollisionPos(void);
void PlayerOrbit(void);
MOTIONTYPE AttackType(float fDistance, float fHeight);

//モデルの情報
typedef struct
{
	LPD3DXMESH pMesh;		//メッシュ（頂点情報）へのポインタ
	LPD3DXBUFFER pBuffMat;	//マテリアル（材質情報）へのポインタ
	DWORD nNumMat;			//マテリアルの数
	LPDIRECT3DTEXTURE9 TextureModel[4] = {};	//テクスチャ
	D3DXVECTOR3 pos;		//位置
	D3DXVECTOR3 posOri;		//元々の位置
	D3DXVECTOR3 rot;		//向き
	D3DXVECTOR3 rotOri;		//元々の向き
	D3DXMATRIX mtxWorld;	//ワールドマトリックス
	int nIdxModelParent;	//親モデルのインデックス
} Model;

//当たり判定
typedef struct
{
	D3DXVECTOR3 pos;		//位置
	D3DXMATRIX mtxWorld;	//ワールドマトリックス
	int nIdxModelParent;	//親モデルのインデックス
	float fRadius;			//当たり判定の大きさの半径
	int nStarttime;			//当たり判定が発生する時間
	int nFinishtime;		//当たり判定が消える時間
	int nDamage;			//ダメージ量
	int nDamageTime;		//ヒット間隔
	float fHClash;			//水平方向の吹っ飛ばし力
	float fVClash;			//垂直方向の吹っ飛ばし力
} Collision;

//軌跡設定
typedef struct
{
	D3DXVECTOR3 pos;		//軌跡の先端の位置
	D3DXMATRIX TipmtxWorld;	//軌跡先端のマトリックス
	D3DXVECTOR3 TipposOld;	//軌跡の先端の前回位置
	D3DXVECTOR3 posOld;		//軌跡の根元の前回位置
	int nTime;				//消えるまでの時間
	int nStartTime;			//発生する時間
	int nFinishTime;		//終了の時間
	int nIdxParent;			//親モデルの番号
} Orbit;

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
	int nFrame;		//フレーム
	float fMove;	//そのキーの水平移動量
	float fHeight;	//そのキーの垂直移動量
	KEY aKey[16];	//各モデルのキー要素 []内はパーツの最大数
} KEY_INFO;

//モーション情報の構造体
typedef struct
{
	bool bLoop;		//ループするかどうか
	int nNumKey;	//キーの総数
	Collision Collision[8];	//当たり判定
	int nNumCol;	//当たり判定の球の数
	int nComboTime;	//コンボが繋がる時間
	Orbit orbit;	//軌跡
	KEY_INFO aKeyInfo[16];	//キー情報 []内はキーの最大数
} MOTION_INFO;

//プレイヤーの情報
typedef struct
{
	D3DXVECTOR3 pos;		//位置
	D3DXVECTOR3 posOld;		//前回位置
	D3DXVECTOR3 rot;		//向き
	D3DXVECTOR3 rotDesh;	//目的の向き
	D3DXMATRIX mtxWorld;	//ワールドマトリックス
	D3DXVECTOR3 move;		//移動量
	Model aModel[16];		//モデル（パーツ）
	int nNumModel;			//モデル（パーツ）数
	int nShadow;			//影の番号
	bool bMove;				//動いているか
	bool bJump;				//ジャンプできるか(=地面と接してるか)
	bool bRock;				//ロックオンしてるか
	MOTION_INFO aMotionInfo[MAX_MOTION];	//モーション情報
	int nNumMotion;			//モーション数
	MOTIONTYPE motionType;	//モーションタイプ
	int nKey;				//キーNo.
	int nCounterMotion;		//モーションのカウンター
	KEY aKeyDiff[32];		//差分 []内はパーツ数
	KEY KeyMove;			//モーション開始から体がどれだけ動くか
	float fRadius;			//半径
	float fHeight;			//高さ
	float fMove;			//最大速度
	float fJump;			//ジャンプ力
	int nNeutralTime;		//移動キーから離した時間
	int nMoveParts;			//モデルの基準になる部位
	int nTotalTime;			//モーション開始から合計の時間
	MOTION_INFO MotionInfo;	//モーションの情報
	int nCol;				//何番目の当たり判定にヒットしたか
	int nEnemy;				//何番目の敵にロックオンしてるか
	int nCombo;				//今のコンボ数
	int nComboTime;			//攻撃してからの時間
	int nHP;				//現在HP
	int nDamageTime;		//ダメージ受けたカウント
	int nMaxHP;				//最大HP
	int nFoot;				//足音
	int nKill;				//倒した数
	MOTIONTYPE Oldmotion;	//過去のモーション
} Player;

//グローバル変数
Player g_player;	//プレイヤーの情報
Model g_model[16];	//モデルの情報
int g_nNumModel;	//キャラに使うモデルが何種類あるか

//初期化処理
HRESULT InitPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice; //デバイスのポインタ
	pDevice = GetDevice();     //デバイスを取得する
	int nCntParts = 0;
	int nCntMotion = 0;
	int nCntKey = 0;
	int nCntCol = 0;
	char aFile[256];
	bool bChara = false;
	bool bParts = false;
	bool bMotion = false;
	bool bKey = false;
	bool bKeySet = false;
	bool bColSet = false;
	bool bCol = false;
	int nIndex;
	int nLoop;
	FILE *pFile;
	g_player.fRadius = 20.0f;
	g_player.fJump = 10.0f;
	g_player.fMove = 5.0f;
	g_player.nMoveParts = -1;
	pFile = fopen(CHARAFILENAME, "r");
	if (pFile != NULL)
	{
		while (true)
		{
			fscanf(pFile, "%s", &aFile[0]); //fscanfを繰り返してファイルを読み取っていく
			if (strcmp(&aFile[0], "NUM_MODEL") == 0) //モデル数
			{
				fscanf(pFile, "%s", &aFile[0]);
				fscanf(pFile, "%d", &g_nNumModel);
			}
			if (strcmp(&aFile[0], "MODEL_FILENAME") == 0) //モデルファイル読み込み
			{
				fscanf(pFile, "%s", &aFile[0]);
				fscanf(pFile, "%s", &aFile[0]);
				D3DXLoadMeshFromX(&aFile[0],
					D3DXMESH_SYSTEMMEM,
					pDevice,
					NULL,
					&g_model[nCntParts].pBuffMat,
					NULL,
					&g_model[nCntParts].nNumMat,
					&g_model[nCntParts].pMesh);
				nCntParts++;
			}
			if (bChara == true) //CHARACTERSET時
			{
				if (strcmp(&aFile[0], "NUM_PARTS") == 0) //パーツ数
				{
					fscanf(pFile, "%s", &aFile[0]);
					fscanf(pFile, "%d", &g_player.nNumModel);
				}
				if (strcmp(&aFile[0], "MOVE") == 0) //移動速度
				{
					fscanf(pFile, "%s", &aFile[0]);
					fscanf(pFile, "%f", &g_player.fMove);
				}
				if (strcmp(&aFile[0], "JUMP") == 0) //ジャンプ力
				{
					fscanf(pFile, "%s", &aFile[0]);
					fscanf(pFile, "%f", &g_player.fJump);
				}
				if (strcmp(&aFile[0], "RADIUS") == 0) //半径
				{
					fscanf(pFile, "%s", &aFile[0]);
					fscanf(pFile, "%f", &g_player.fRadius);
				}
				if (strcmp(&aFile[0], "HEIGHT") == 0) //身長
				{
					fscanf(pFile, "%s", &aFile[0]);
					fscanf(pFile, "%f", &g_player.fHeight);
				}
				if (strcmp(&aFile[0], "HP") == 0) //身長
				{
					fscanf(pFile, "%s", &aFile[0]);
					fscanf(pFile, "%d", &g_player.nHP);
					g_player.nMaxHP = g_player.nHP;
				}
				if (strcmp(&aFile[0], "PARTSSET") == 0)
				{
					bParts = true;
				}
				if (strcmp(&aFile[0], "END_PARTSSET") == 0)
				{
					bParts = false;
					nCntParts++;
				}
				if (bParts == true) //PARTSSET時
				{
					if (strcmp(&aFile[0], "MOVEPARTS") == 0) //これが動くとg_playerのposも動く（いらないかも）
					{
						g_player.nMoveParts = nCntParts;
					}
					if (strcmp(&aFile[0], "INDEX") == 0) //モデルファイル読み込みの時の、何番目のモデルを呼び出すか
					{
						fscanf(pFile, "%s", &aFile[0]);
						fscanf(pFile, "%d", &nIndex);
						g_player.aModel[nCntParts].pBuffMat = g_model[nIndex].pBuffMat;
						g_player.aModel[nCntParts].nNumMat = g_model[nIndex].nNumMat;
						g_player.aModel[nCntParts].pMesh = g_model[nIndex].pMesh;
					}
					if (strcmp(&aFile[0], "PARENT") == 0) //親モデル
					{
						fscanf(pFile, "%s", &aFile[0]);
						fscanf(pFile, "%d", &g_player.aModel[nCntParts].nIdxModelParent);
					}
					if (strcmp(&aFile[0], "POS") == 0) //各パーツの場所
					{
						fscanf(pFile, "%s", &aFile[0]);
						fscanf(pFile, "%f %f %f", &g_player.aModel[nCntParts].posOri.x, &g_player.aModel[nCntParts].posOri.y, &g_player.aModel[nCntParts].posOri.z);
					}
					if (strcmp(&aFile[0], "ROT") == 0) //各パーツの角度
					{
						fscanf(pFile, "%s", &aFile[0]);
						fscanf(pFile, "%f %f %f", &g_player.aModel[nCntParts].rotOri.x, &g_player.aModel[nCntParts].rotOri.y, &g_player.aModel[nCntParts].rotOri.z);
					}
				}
			}
			if (bMotion == true) //MOTIONSET時
			{
				if (strcmp(&aFile[0], "LOOP") == 0) //そのモーションがループするか
				{
					fscanf(pFile, "%s", &aFile[0]);
					fscanf(pFile, "%d", &nLoop);
					if (nLoop == 0)
					{
						g_player.aMotionInfo[nCntMotion].bLoop = false;
					}
					else
					{
						g_player.aMotionInfo[nCntMotion].bLoop = true;
					}
				}
				if (strcmp(&aFile[0], "NUM_KEY") == 0) //いくつキーがあるか
				{
					fscanf(pFile, "%s", &aFile[0]);
					fscanf(pFile, "%d", &g_player.aMotionInfo[nCntMotion].nNumKey);
				}
				if (strcmp(&aFile[0], "ORBIT") == 0) //軌跡（場所、表示時間、発生時間）
				{
					fscanf(pFile, "%s", &aFile[0]);
					fscanf(pFile, "%d %f %f %f %d %d %d", &g_player.aMotionInfo[nCntMotion].orbit.nIdxParent, &g_player.aMotionInfo[nCntMotion].orbit.pos.x, &g_player.aMotionInfo[nCntMotion].orbit.pos.y, &g_player.aMotionInfo[nCntMotion].orbit.pos.z,
						&g_player.aMotionInfo[nCntMotion].orbit.nTime, &g_player.aMotionInfo[nCntMotion].orbit.nStartTime, &g_player.aMotionInfo[nCntMotion].orbit.nFinishTime);
				}
				if (strcmp(&aFile[0], "COMBO_TIME") == 0) //コンボができるまでの時間
				{
					fscanf(pFile, "%s", &aFile[0]);
					fscanf(pFile, "%d", &g_player.aMotionInfo[nCntMotion].nComboTime);
				}
				if (strcmp(&aFile[0], "COLLISIONSET") == 0)
				{
					bColSet = true;
					nCntCol = 0;
				}
				if (strcmp(&aFile[0], "END_COLLISIONSET") == 0)
				{
					g_player.aMotionInfo[nCntMotion].nNumCol = nCntCol;
					bColSet = false;
				}
				if (strcmp(&aFile[0], "KEYSET") == 0)
				{
					bKeySet = true;
					nCntParts = 0;
				}
				if (strcmp(&aFile[0], "END_KEYSET") == 0)
				{
					bKeySet = false;
					nCntKey++;
				}
				if (bColSet == true) //COLLISIONSET時
				{
					if (strcmp(&aFile[0], "COLLISION") == 0)
					{
						bCol = true;
					}
					if (strcmp(&aFile[0], "END_COLLISION") == 0)
					{
						bCol = false;
						nCntCol++;
					}
					if (bCol == true)
					{
						if (strcmp(&aFile[0], "COL") == 0) //当たり判定（親のインデックス、場所、大きさ、時間）
						{
							fscanf(pFile, "%s", &aFile[0]);
							fscanf(pFile, "%d %f %f %f %f %d %d", &g_player.aMotionInfo[nCntMotion].Collision[nCntCol].nIdxModelParent,
								&g_player.aMotionInfo[nCntMotion].Collision[nCntCol].pos.x, &g_player.aMotionInfo[nCntMotion].Collision[nCntCol].pos.y, &g_player.aMotionInfo[nCntMotion].Collision[nCntCol].pos.z,
								&g_player.aMotionInfo[nCntMotion].Collision[nCntCol].fRadius, &g_player.aMotionInfo[nCntMotion].Collision[nCntCol].nStarttime, &g_player.aMotionInfo[nCntMotion].Collision[nCntCol].nFinishtime);
						}
						if (strcmp(&aFile[0], "DMG") == 0) //当たり判定（水平吹っ飛ばし力、垂直吹っ飛ばし力、ダメージ量、ヒット間隔）
						{
							fscanf(pFile, "%s", &aFile[0]);
							fscanf(pFile, "%f %f %d %d", &g_player.aMotionInfo[nCntMotion].Collision[nCntCol].fHClash, &g_player.aMotionInfo[nCntMotion].Collision[nCntCol].fVClash,
								&g_player.aMotionInfo[nCntMotion].Collision[nCntCol].nDamage, &g_player.aMotionInfo[nCntMotion].Collision[nCntCol].nDamageTime);
						}
					}
				}
				if (bKeySet == true) //KEYSET時
				{
					if (strcmp(&aFile[0], "FRAME") == 0) //フレーム数
					{
						fscanf(pFile, "%s", &aFile[0]);
						fscanf(pFile, "%d", &g_player.aMotionInfo[nCntMotion].aKeyInfo[nCntKey].nFrame);
					}
					if (strcmp(&aFile[0], "MOVE") == 0) //移動量
					{
						fscanf(pFile, "%s", &aFile[0]);
						fscanf(pFile, "%f", &g_player.aMotionInfo[nCntMotion].aKeyInfo[nCntKey].fMove);
					}
					if (strcmp(&aFile[0], "HEIGHT") == 0) //垂直移動量
					{
						fscanf(pFile, "%s", &aFile[0]);
						fscanf(pFile, "%f", &g_player.aMotionInfo[nCntMotion].aKeyInfo[nCntKey].fHeight);
					}
					if (strcmp(&aFile[0], "KEY") == 0)
					{
						bKey = true;
					}
					if (strcmp(&aFile[0], "END_KEY") == 0)
					{
						bKey = false;
						nCntParts++;
					}
					if (bKey == true) //KEY時
					{
						if (strcmp(&aFile[0], "POS") == 0) //場所
						{
							fscanf(pFile, "%s", &aFile[0]);
							fscanf(pFile, "%f %f %f", &g_player.aMotionInfo[nCntMotion].aKeyInfo[nCntKey].aKey[nCntParts].fPosX, &g_player.aMotionInfo[nCntMotion].aKeyInfo[nCntKey].aKey[nCntParts].fPosY, &g_player.aMotionInfo[nCntMotion].aKeyInfo[nCntKey].aKey[nCntParts].fPosZ);
						}
						if (strcmp(&aFile[0], "ROT") == 0) //角度
						{
							fscanf(pFile, "%s", &aFile[0]);
							fscanf(pFile, "%f %f %f", &g_player.aMotionInfo[nCntMotion].aKeyInfo[nCntKey].aKey[nCntParts].fRotX, &g_player.aMotionInfo[nCntMotion].aKeyInfo[nCntKey].aKey[nCntParts].fRotY, &g_player.aMotionInfo[nCntMotion].aKeyInfo[nCntKey].aKey[nCntParts].fRotZ);
						}
					}
				}
			}
			if (strcmp(&aFile[0], "CHARACTERSET") == 0)
			{
				bChara = true;
				nCntParts = 0;
			}
			if (strcmp(&aFile[0], "END_CHARACTERSET") == 0)
			{
				bChara = false;
			}
			if (strcmp(&aFile[0], "MOTIONSET") == 0)
			{
				bMotion = true;
				nCntKey = 0;
				nCntCol = 0;
			}
			if (strcmp(&aFile[0], "END_MOTIONSET") == 0)
			{
				bMotion = false;
				nCntMotion++;
			}
			if (strcmp(&aFile[0], "END_SCRIPT") == 0) //終了の1文
			{
				break;
			}
		}
		fclose(pFile);
	}
	D3DXMATERIAL *pMat;
	//「MOTIONTYPE_RETURN」は、ニュートラルモーションに戻る際に経由するモーション
	g_player.aMotionInfo[MOTIONTYPE_RETURN].bLoop = false;
	g_player.aMotionInfo[MOTIONTYPE_RETURN].aKeyInfo[0].nFrame = 14;
	g_player.aMotionInfo[MOTIONTYPE_RETURN].nNumKey = 1;
	for (nCntParts = 0; nCntParts < g_player.nNumModel; nCntParts++)
	{
		g_player.aMotionInfo[MOTIONTYPE_RETURN].aKeyInfo[0].aKey[nCntParts].fPosX = g_player.aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[1].aKey[nCntParts].fPosX;
		g_player.aMotionInfo[MOTIONTYPE_RETURN].aKeyInfo[0].aKey[nCntParts].fPosY = g_player.aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[1].aKey[nCntParts].fPosY;
		g_player.aMotionInfo[MOTIONTYPE_RETURN].aKeyInfo[0].aKey[nCntParts].fPosZ = g_player.aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[1].aKey[nCntParts].fPosZ;
		g_player.aMotionInfo[MOTIONTYPE_RETURN].aKeyInfo[0].aKey[nCntParts].fRotX = g_player.aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[1].aKey[nCntParts].fRotX;
		g_player.aMotionInfo[MOTIONTYPE_RETURN].aKeyInfo[0].aKey[nCntParts].fRotY = g_player.aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[1].aKey[nCntParts].fRotY;
		g_player.aMotionInfo[MOTIONTYPE_RETURN].aKeyInfo[0].aKey[nCntParts].fRotZ = g_player.aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[1].aKey[nCntParts].fRotZ;
		//マテリアル情報に対するポインタを取得
		pMat = (D3DXMATERIAL*)g_player.aModel[nCntParts].pBuffMat->GetBufferPointer();
		int nCntTex = 0;
		for (int nCntMat = 0; nCntMat < (int)g_player.aModel[nCntParts].nNumMat; nCntMat++)
		{
			if (pMat[nCntMat].pTextureFilename != NULL)
			{
				D3DXCreateTextureFromFile(pDevice,
					pMat[nCntMat].pTextureFilename,
					&g_player.aModel[nCntParts].TextureModel[nCntTex]);
				nCntTex++;
			}
		}
	}
	g_player.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_player.posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_player.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_player.rotDesh = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_player.bJump = true;
	g_player.bMove = false;
	g_player.nNeutralTime = 0;
	g_player.nTotalTime = 0;
	g_player.nEnemy = -1;
	g_player.nDamageTime = 0;
	g_player.nKill = 0;
	MotionChange(MOTIONTYPE_RETURN);
	g_player.nShadow = SetShadow(g_player.pos, g_player.fRadius);
	g_player.nCombo = 0;
	g_player.nComboTime = 0;
	g_player.nFoot = 0;
	g_player.bRock = false;
	return S_OK;
}

//終了処理
void UninitPlayer(void)
{
	int nCntModel;
	for (nCntModel = 0; nCntModel < g_nNumModel; nCntModel++)
	{
		//テクスチャの破棄
		if (g_model[nCntModel].pMesh != NULL)
		{
			g_model[nCntModel].pMesh->Release();
			g_model[nCntModel].pMesh = NULL;
		}
		//頂点バッファの破棄
		if (g_model[nCntModel].pBuffMat != NULL)
		{
			g_model[nCntModel].pBuffMat->Release();
			g_model[nCntModel].pBuffMat = NULL;
		}
	}
}

//更新処理
void UpdatePlayer(void)
{
	bool bLand;
	bLand = g_player.bJump;
	g_player.posOld = g_player.pos;
	//攻撃してないかつダメージくらってない
	if (g_player.nComboTime == 0 && g_player.nDamageTime == 0)
	{
		//移動
		if (GetKeyboardPress(DIK_A) == true || LeftStickX() < 0.0f)
		{
			if (GetKeyboardPress(DIK_W) == true || LeftStickY() < 0.0f)
			{
				g_player.rotDesh.y = D3DX_PI * 0.75f + CameraRot();
			}
			else if (GetKeyboardPress(DIK_S) == true || LeftStickY() > 0.0f)
			{
				g_player.rotDesh.y = D3DX_PI * 0.25f + CameraRot();
			}
			else
			{
				g_player.rotDesh.y = D3DX_PI * 0.5f + CameraRot();
			}
			PlayerMoveX();
			PlayerMoveZ();
		}
		else if (GetKeyboardPress(DIK_D) == true || LeftStickX() > 0.0f)
		{
			if (GetKeyboardPress(DIK_W) == true || LeftStickY() < 0.0f)
			{
				g_player.rotDesh.y = D3DX_PI * -0.75f + CameraRot();
			}
			else if (GetKeyboardPress(DIK_S) == true || LeftStickY() > 0.0f)
			{
				g_player.rotDesh.y = D3DX_PI * -0.25f + CameraRot();
			}
			else
			{
				g_player.rotDesh.y = D3DX_PI * -0.5f + CameraRot();
			}
			PlayerMoveX();
			PlayerMoveZ();
		}
		else if (GetKeyboardPress(DIK_W) == true || LeftStickY() < 0.0f)
		{
			g_player.rotDesh.y = D3DX_PI + CameraRot();
			PlayerMoveX();
			PlayerMoveZ();
		}
		else if (GetKeyboardPress(DIK_S) == true || LeftStickY() > 0.0f)
		{
			g_player.rotDesh.y = CameraRot();
			PlayerMoveX();
			PlayerMoveZ();
		}
		else
		{
			//移動速度低下
			PlayerSlowX();
			PlayerSlowZ();
		}
	}
	else
	{
		//移動速度低下
		PlayerSlowX();
		PlayerSlowZ();
	}

	//歩いてるかつモーションが歩いてないかつ着地してる
	if (g_player.bMove == true && g_player.motionType != MOTIONTYPE_WALK && g_player.bJump == true)
	{
		MotionChange(MOTIONTYPE_WALK);
	}

	//歩いでないかつモーションが歩いてるかつ歩くのやめてからカウント経ってる
	if(g_player.bMove == false && g_player.nNeutralTime == 8 && g_player.motionType == MOTIONTYPE_WALK)
	{
		MotionChange(MOTIONTYPE_RETURN);
	}

	//方向が狂わないようにする
	if (g_player.rot.y >= D3DX_PI)
	{
		g_player.rot.y -= D3DX_PI * 2;
	}
	if (g_player.rot.y < -D3DX_PI)
	{
		g_player.rot.y += D3DX_PI * 2;
	}
	if (g_player.rotDesh.y >= D3DX_PI)
	{
		g_player.rotDesh.y -= D3DX_PI * 2;
	}
	if (g_player.rotDesh.y < -D3DX_PI)
	{
		g_player.rotDesh.y += D3DX_PI * 2;
	}

	//方向加算
	if (g_player.rot.y != g_player.rotDesh.y)
	{
		if (g_player.rotDesh.y - g_player.rot.y < -3.14f)
		{
			g_player.rot.y += (g_player.rotDesh.y - g_player.rot.y + 6.28f) * 0.14f;
		}
		else if (g_player.rotDesh.y - g_player.rot.y > 3.14f)
		{
			g_player.rot.y += (g_player.rotDesh.y - g_player.rot.y - 6.28f) * 0.14f;
		}
		else
		{
			g_player.rot.y += (g_player.rotDesh.y - g_player.rot.y) * 0.14f;
		}
	}

	//ジャンプ
	if (GetKeyboardKey(DIK_I) == true || GetGamePadButton(DIP_A) == true)
	{
		//着地してるかつコンボしてない
		if (g_player.bJump == true && g_player.nComboTime == 0)
		{
			//ジャンプさせる
			g_player.move.y += g_player.fJump;
			MotionChange(MOTIONTYPE_JUMP);
			g_player.nCombo = 0;
		}
	}

	//攻撃してなかったら、重力働かせる
	if (g_player.motionType == MOTIONTYPE_NEUTRAL || g_player.motionType == MOTIONTYPE_JUMP || g_player.motionType == MOTIONTYPE_LANDING ||
		g_player.motionType == MOTIONTYPE_RETURN || g_player.motionType == MOTIONTYPE_WALK)
	{
		g_player.move.y -= 0.4f;
	}
	else
	{
		g_player.move.y = 0.0f;
	}

	PlayerMotion(); //プレイヤーのモーション

	//移動
	g_player.pos.x += g_player.move.x;
	g_player.pos.y += g_player.move.y;
	g_player.pos.z += g_player.move.z;
	MoveShadow(g_player.nShadow, g_player.pos);

	//ロックオンしてない
	if (g_player.bRock == false)
	{
		//オートロックオンさせる
		g_player.nEnemy = RockOn(g_player.pos);
	}

	//ロックオン
	if (GetKeyboardKey(DIK_E) == true || GetGamePadButton(DIP_R1) == true)
	{
		if (g_player.bRock == true)
		{
			g_player.bRock = false;
		}
		else if(g_player.nEnemy != -1)
		{
			g_player.bRock = true;
		}
	}

	//ロックオンしてる敵いないかつロックオンしてないロックオン外す
	if (g_player.nEnemy == -1 && g_player.bRock == false)
	{
		RemoveRockon();
	}

	//カウント類進める
	if (g_player.nDamageTime > 0)
	{
		g_player.nDamageTime--;
	}
	if (g_player.nComboTime > 0)
	{
		g_player.nComboTime--;
	}
	//ダメージカウント進んでる
	else if (g_player.nDamageTime == 0)
	{
		//攻撃
		if (GetKeyboardKey(DIK_J) == true || GetGamePadButton(DIP_B) == true)
		{
			//敵をロックオンしてる
			if (g_player.nEnemy != -1)
			{
				//敵の方向を向く
				g_player.rotDesh.y = RockAttack(g_player.pos, g_player.nEnemy);
				if (g_player.nCombo < 2)
				{
					//敵との位置関係に応じて攻撃変わる
					MotionChange(AttackType(RockHDistance(g_player.pos, g_player.nEnemy), RockVDistance(g_player.pos, g_player.nEnemy, g_player.fHeight)));
				}
				else
				{
					//コンボフィニッシュ
					MotionChange(MOTIONTYPE_FINISH);
					g_player.nCombo = 0;
				}
			}
			//ロックオンしてない
			else
			{
				//コンボ数に応じて攻撃変わる
				switch (g_player.nCombo)
				{
				case 0:
					MotionChange(MOTIONTYPE_NORMAL);	//近距離
					g_player.nCombo++;
					break;
				case 1:
					MotionChange(MOTIONTYPE_RUSH);		//突進
					g_player.nCombo++;
					break;
				case 2:
					MotionChange(MOTIONTYPE_FINISH);	//フィニッシュ
					g_player.nCombo = 0;
					break;
				}
			}
		}
	}

	//当たり判定
	g_player.bJump = CollisionField(&g_player.pos, &g_player.posOld, &g_player.move, g_player.fRadius); //フィールドとの当たり判定
	CollisionObject(&g_player.pos, g_player.fRadius, g_player.fHeight); //オブジェクトとの当たり判定(上に乗ることはできない)
	CoillisionEnemy(g_player.fRadius, &g_player.pos, g_player.fHeight);
	CollisionWall(&g_player.pos, g_player.fRadius);
	//弾との当たり判定
	if (CollisionBullet(&g_player.pos, &g_player.move, g_player.fRadius, g_player.nDamageTime, &g_player.rotDesh.y) == true)
	{
		//HP減らす
		g_player.rot.y = g_player.rotDesh.y;
		g_player.nHP -= BulletDamage();

		//まだ生きてる
		if (g_player.nHP > 0)
		{
			//ダメージモーション
			PlaySound(SOUND_LABEL_DAMAGE);
			MotionChange(MOTIONTYPE_DAMAGE);
		}
		else
		{
			//やられた
			MotionChange(MOTIONTYPE_DIE);
		}
	}

	//前フレームで着地してないかつ今フレームで着地する
	if (bLand == false && g_player.bJump == true)
	{
		if (g_player.motionType == MOTIONTYPE_NEUTRAL || g_player.motionType == MOTIONTYPE_JUMP || g_player.motionType == MOTIONTYPE_RETURN || g_player.motionType == MOTIONTYPE_WALK)
		{
			MotionChange(MOTIONTYPE_LANDING);
			g_player.nCombo = 0;
		}
	}
}

//描画処理
void DrawPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice;   //デバイスのポインタ
	D3DXMATRIX mtxRot, mtxTrans; //計算用マトリックス
	D3DMATERIAL9 matDef;		 //現在のマテリアル保存用
	D3DXMATERIAL *pMat;			 //マテリアルデータへのポインタ
	int nCntModel;
	pDevice = GetDevice();		 //デバイスを取得する

	//プレイヤーのワールドマトリックスの初期化
	D3DXMatrixIdentity(&g_player.mtxWorld);

	//プレイヤーの向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_player.rot.y, g_player.rot.x, g_player.rot.z);
	D3DXMatrixMultiply(&g_player.mtxWorld, &g_player.mtxWorld, &mtxRot);

	//プレイヤーの位置を反映
	D3DXMatrixTranslation(&mtxTrans, g_player.pos.x, g_player.pos.y, g_player.pos.z);
	D3DXMatrixMultiply(&g_player.mtxWorld, &g_player.mtxWorld, &mtxTrans);

	//プレイヤーのワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &g_player.mtxWorld);

	for (nCntModel = 0; nCntModel < g_player.nNumModel; nCntModel++)
	{
		D3DXMATRIX mtxRotParts, mtxTransParts; //パーツ用計算用マトリックス
		D3DXMATRIX mtxParent; //親のマトリックス

		//パーツのワールドマトリックスの初期化
		D3DXMatrixIdentity(&g_player.aModel[nCntModel].mtxWorld);

		//パーツの向きを反映
		D3DXMatrixRotationYawPitchRoll(&mtxRotParts, g_player.aModel[nCntModel].rot.y + g_player.aModel[nCntModel].rotOri.y, g_player.aModel[nCntModel].rot.x + g_player.aModel[nCntModel].rotOri.x, g_player.aModel[nCntModel].rot.z + g_player.aModel[nCntModel].rotOri.z);
		D3DXMatrixMultiply(&g_player.aModel[nCntModel].mtxWorld, &g_player.aModel[nCntModel].mtxWorld, &mtxRotParts);
		
		//パーツの位置を反映
		D3DXMatrixTranslation(&mtxTransParts, g_player.aModel[nCntModel].pos.x + g_player.aModel[nCntModel].posOri.x, g_player.aModel[nCntModel].pos.y + g_player.aModel[nCntModel].posOri.y, g_player.aModel[nCntModel].pos.z + g_player.aModel[nCntModel].posOri.z);
		D3DXMatrixMultiply(&g_player.aModel[nCntModel].mtxWorld, &g_player.aModel[nCntModel].mtxWorld, &mtxTransParts);
		
		//各パーツの親のマトリックスを設定
		if (g_player.aModel[nCntModel].nIdxModelParent != -1)
		{
			mtxParent = g_player.aModel[g_player.aModel[nCntModel].nIdxModelParent].mtxWorld;
		}
		else
		{
			mtxParent = g_player.mtxWorld; //プレイヤーのマトリックスを設定
		}
		
		//算出した各パーツのワールドマトリックスと親のマトリックスを掛け合わせる
		D3DXMatrixMultiply(&g_player.aModel[nCntModel].mtxWorld,
			&g_player.aModel[nCntModel].mtxWorld,
			&mtxParent);
		
		//各パーツのワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &g_player.aModel[nCntModel].mtxWorld);
		
		//パーツの現在のマテリアルを取得
		pDevice->GetMaterial(&matDef);
		
		//パーツのマテリアルデータへのポインタの反映
		pMat = (D3DXMATERIAL*)g_player.aModel[nCntModel].pBuffMat->GetBufferPointer();
		
		int nCntMat;
		for (nCntMat = 0; nCntMat < (int)g_player.aModel[nCntModel].nNumMat; nCntMat++)
		{
			int nCntTex = 0;
			//プレイヤーのマテリアルの設定
			pDevice->SetMaterial(&pMat[nCntMat].MatD3D);
			for (int nCntMat = 0; nCntMat < (int)g_player.aModel[nCntModel].nNumMat; nCntMat++)
			{
				if (pMat[nCntMat].pTextureFilename != NULL)
				{
					//テクスチャの設定
					pDevice->SetTexture(0, g_player.aModel[nCntModel].TextureModel[nCntTex]);
					nCntTex++;
				}
				else
				{
					pDevice->SetTexture(0, NULL);
				}
			}
			//プレイヤーのモデル（パーツ）の描画
			g_player.aModel[nCntModel].pMesh->DrawSubset(nCntMat);
		}
		//保存していたマテリアルを戻す
		pDevice->SetMaterial(&matDef);
	}
	//プレイヤーの軌跡
	PlayerOrbit();

	//攻撃の当たり判定の場所設定
	CollisionPos();
}

//移動速度横
void PlayerMoveX(void)
{
	if (g_player.bJump == true)
	{
		g_player.move.x -= (sinf(g_player.rotDesh.y) * g_player.fMove + g_player.move.x) * 0.1f;
	}
	else
	{
		g_player.move.x -= (sinf(g_player.rotDesh.y) * g_player.fMove + g_player.move.x) * 0.05f;
	}

	//コンボ0にする
	g_player.nCombo = 0;

	//カウント進める、進んだら足音鳴らす
	g_player.nFoot++;
	if (g_player.nFoot == 20)
	{
		PlaySound(SOUND_LABEL_FOOTSTEP);
		g_player.nFoot = 0;
	}
	g_player.nNeutralTime = 0;
	g_player.bMove = true;
}

//移動速度奥
void PlayerMoveZ(void)
{
	if (g_player.bJump == true)
	{
		g_player.move.z -= (cosf(g_player.rotDesh.y) * g_player.fMove + g_player.move.z) * 0.1f;
	}
	else
	{
		g_player.move.z -= (cosf(g_player.rotDesh.y) * g_player.fMove + g_player.move.z) * 0.05f;
	}
}

//移動速度低下横
void PlayerSlowX(void)
{
	if (g_player.bJump == true)
	{
		g_player.move.x -= g_player.move.x * 0.1f;
	}
	else
	{
		g_player.move.x -= g_player.move.x * 0.05f;
	}

	//ニュートラルカウント進める
	g_player.nNeutralTime++;
	if (g_player.nNeutralTime > 100)
	{
		g_player.nNeutralTime = 100;
	}
	g_player.bMove = false;
}

//移動速度低下縦
void PlayerSlowZ(void)
{
	if (g_player.bJump == true)
	{
		g_player.move.z -= g_player.move.z * 0.1f;
	}
	else
	{
		g_player.move.z -= g_player.move.z * 0.05f;
	}
}

//モーションを切り替える関数
void MotionChange(MOTIONTYPE motiontype)
{
	g_player.Oldmotion = g_player.motionType;
	g_player.motionType = motiontype;
	g_player.MotionInfo = g_player.aMotionInfo[motiontype];
	g_player.nCounterMotion = 0;
	g_player.nKey = 0;
	g_player.KeyMove.fPosX = 0.0f;
	g_player.KeyMove.fPosY = 0.0f;
	g_player.KeyMove.fPosZ = 0.0f;
	g_player.nTotalTime = 0;
	g_player.nComboTime = g_player.aMotionInfo[motiontype].nComboTime;
}

//モーションの動作
void PlayerMotion(void)
{
	int nCntParts;
	float fDistance;
	float fHeight;
	g_player.nCounterMotion++;
	g_player.nTotalTime++;
	for (nCntParts = 0; nCntParts < g_player.nNumModel; nCntParts++)
	{
		if (g_player.nCounterMotion == 1) //差分を求める(対応するキーの各パーツのpos(rot) - 今の各パーツのpos(rot))
		{
			if (nCntParts != g_player.nMoveParts)
			{
				g_player.aKeyDiff[nCntParts].fPosX = g_player.aMotionInfo[g_player.motionType].aKeyInfo[g_player.nKey].aKey[nCntParts].fPosX - g_player.aModel[nCntParts].pos.x;
				g_player.aKeyDiff[nCntParts].fPosY = g_player.aMotionInfo[g_player.motionType].aKeyInfo[g_player.nKey].aKey[nCntParts].fPosY - g_player.aModel[nCntParts].pos.y;
				g_player.aKeyDiff[nCntParts].fPosZ = g_player.aMotionInfo[g_player.motionType].aKeyInfo[g_player.nKey].aKey[nCntParts].fPosZ - g_player.aModel[nCntParts].pos.z;
			}
			else
			{
				//ロックオンしてる
				if (g_player.nEnemy != -1)
				{
					//水平方向の距離計算
					fDistance = RockHDistance(g_player.pos, g_player.nEnemy);
					if (fDistance * 1.2f < g_player.aMotionInfo[g_player.motionType].aKeyInfo[g_player.nKey].fMove)
					{
						g_player.KeyMove.fPosX = sinf(D3DX_PI + g_player.rot.y) * (fDistance * 1.2f);
						g_player.KeyMove.fPosZ = cosf(D3DX_PI + g_player.rot.y) * (fDistance * 1.2f);
					}
					else
					{
						g_player.KeyMove.fPosX = sinf(D3DX_PI + g_player.rot.y) * g_player.aMotionInfo[g_player.motionType].aKeyInfo[g_player.nKey].fMove;
						g_player.KeyMove.fPosZ = cosf(D3DX_PI + g_player.rot.y) * g_player.aMotionInfo[g_player.motionType].aKeyInfo[g_player.nKey].fMove;
					}

					//垂直方向の距離計算
					fHeight = RockVDistance(g_player.pos, g_player.nEnemy, g_player.fHeight);
					if (fabsf(fHeight) < g_player.aMotionInfo[g_player.motionType].aKeyInfo[g_player.nKey].fHeight)
					{
						g_player.KeyMove.fPosY = -fHeight;
					}
					else if(fHeight < 0.0f)
					{
						g_player.KeyMove.fPosY = g_player.aMotionInfo[g_player.motionType].aKeyInfo[g_player.nKey].fHeight;
					}
					else
					{
						g_player.KeyMove.fPosY = -g_player.aMotionInfo[g_player.motionType].aKeyInfo[g_player.nKey].fHeight;
					}
				}
				else
				{
					g_player.KeyMove.fPosX = sinf(D3DX_PI + g_player.rot.y) * g_player.aMotionInfo[g_player.motionType].aKeyInfo[g_player.nKey].fMove;
					g_player.KeyMove.fPosY = 0.0f;
					g_player.KeyMove.fPosZ = cosf(D3DX_PI + g_player.rot.y) * g_player.aMotionInfo[g_player.motionType].aKeyInfo[g_player.nKey].fMove;
				}
			}
			//方向計算
			g_player.aKeyDiff[nCntParts].fRotX = g_player.aMotionInfo[g_player.motionType].aKeyInfo[g_player.nKey].aKey[nCntParts].fRotX - g_player.aModel[nCntParts].rot.x;
			if (g_player.aKeyDiff[nCntParts].fRotX <= -D3DX_PI)
			{
				g_player.aKeyDiff[nCntParts].fRotX += D3DX_PI * 2;
			}
			else if (g_player.aKeyDiff[nCntParts].fRotX > D3DX_PI)
			{
				g_player.aKeyDiff[nCntParts].fRotX -= D3DX_PI * 2;
			}
			g_player.aKeyDiff[nCntParts].fRotY = g_player.aMotionInfo[g_player.motionType].aKeyInfo[g_player.nKey].aKey[nCntParts].fRotY - g_player.aModel[nCntParts].rot.y;
			if (g_player.aKeyDiff[nCntParts].fRotY <= -D3DX_PI)
			{
				g_player.aKeyDiff[nCntParts].fRotY += D3DX_PI * 2;
			}
			else if (g_player.aKeyDiff[nCntParts].fRotY > D3DX_PI)
			{
				g_player.aKeyDiff[nCntParts].fRotY -= D3DX_PI * 2;
			}
			g_player.aKeyDiff[nCntParts].fRotZ = g_player.aMotionInfo[g_player.motionType].aKeyInfo[g_player.nKey].aKey[nCntParts].fRotZ - g_player.aModel[nCntParts].rot.z;
			if (g_player.aKeyDiff[nCntParts].fRotZ <= -D3DX_PI)
			{
				g_player.aKeyDiff[nCntParts].fRotZ += D3DX_PI * 2;
			}
			else if (g_player.aKeyDiff[nCntParts].fRotZ > D3DX_PI)
			{
				g_player.aKeyDiff[nCntParts].fRotZ -= D3DX_PI * 2;
			}
		}
		if (nCntParts != g_player.nMoveParts)
		{
			g_player.aModel[nCntParts].pos.x += g_player.aKeyDiff[nCntParts].fPosX / g_player.aMotionInfo[g_player.motionType].aKeyInfo[g_player.nKey].nFrame;
			g_player.aModel[nCntParts].pos.y += g_player.aKeyDiff[nCntParts].fPosY / g_player.aMotionInfo[g_player.motionType].aKeyInfo[g_player.nKey].nFrame;
			g_player.aModel[nCntParts].pos.z += g_player.aKeyDiff[nCntParts].fPosZ / g_player.aMotionInfo[g_player.motionType].aKeyInfo[g_player.nKey].nFrame;
		}
		else
		{
			if (g_player.KeyMove.fPosX != 0.0f)
			{
				g_player.move.x = g_player.KeyMove.fPosX / g_player.aMotionInfo[g_player.motionType].aKeyInfo[g_player.nKey].nFrame;
			}
			if (g_player.KeyMove.fPosY != 0.0f)
			{
				g_player.move.y = g_player.KeyMove.fPosY / g_player.aMotionInfo[g_player.motionType].aKeyInfo[g_player.nKey].nFrame;
			}
			if (g_player.KeyMove.fPosZ != 0.0f)
			{
				g_player.move.z = g_player.KeyMove.fPosZ / g_player.aMotionInfo[g_player.motionType].aKeyInfo[g_player.nKey].nFrame;
			}
		}
		//方向加算
		g_player.aModel[nCntParts].rot.x += g_player.aKeyDiff[nCntParts].fRotX / g_player.aMotionInfo[g_player.motionType].aKeyInfo[g_player.nKey].nFrame;
		g_player.aModel[nCntParts].rot.y += g_player.aKeyDiff[nCntParts].fRotY / g_player.aMotionInfo[g_player.motionType].aKeyInfo[g_player.nKey].nFrame;
		g_player.aModel[nCntParts].rot.z += g_player.aKeyDiff[nCntParts].fRotZ / g_player.aMotionInfo[g_player.motionType].aKeyInfo[g_player.nKey].nFrame;
		if (g_player.aModel[nCntParts].rot.x >= D3DX_PI)
		{
			g_player.aModel[nCntParts].rot.x -= D3DX_PI * 2;
		}
		else if (g_player.aModel[nCntParts].rot.x < -D3DX_PI)
		{
			g_player.aModel[nCntParts].rot.x += D3DX_PI * 2;
		}
		if (g_player.aModel[nCntParts].rot.y >= D3DX_PI)
		{
			g_player.aModel[nCntParts].rot.y -= D3DX_PI * 2;
		}
		else if (g_player.aModel[nCntParts].rot.y < -D3DX_PI)
		{
			g_player.aModel[nCntParts].rot.y += D3DX_PI * 2;
		}
		if (g_player.aModel[nCntParts].rot.z >= D3DX_PI)
		{
			g_player.aModel[nCntParts].rot.z -= D3DX_PI * 2;
		}
		else if (g_player.aModel[nCntParts].rot.z < -D3DX_PI)
		{
			g_player.aModel[nCntParts].rot.z += D3DX_PI * 2;
		}
	}

	//キーのフレーム数上限
	if (g_player.nCounterMotion == g_player.aMotionInfo[g_player.motionType].aKeyInfo[g_player.nKey].nFrame)
	{
		//キーのモーション数リセット、キー加算
		g_player.nCounterMotion = 0;
		g_player.nKey++;

		//キー上限
		if (g_player.nKey == g_player.MotionInfo.nNumKey)
		{
			//ループしない
			if (g_player.MotionInfo.bLoop == false)
			{
				//リターンモーションじゃない
				if (g_player.motionType != MOTIONTYPE_RETURN)
				{
					MotionChange(MOTIONTYPE_RETURN);
				}
				//リターンモーション
				else
				{
					MotionChange(MOTIONTYPE_NEUTRAL);
					g_player.nCombo = 0;
				}
			}
			//キー数リセット
			g_player.nKey = 0;
		}
	}
}

//プレイヤーの場所
D3DXVECTOR3 PlayerPos(void)
{
	return g_player.pos;
}

//プレイヤーの向き
float PlayerRot(void)
{
	float fRoty;
	fRoty = g_player.rot.y + D3DX_PI;
	if (fRoty > D3DX_PI)
	{
		fRoty -= D3DX_PI * 2;
	}
	return fRoty;
}

//プレイヤーの目的の向き
float PlayerRotDesh(void)
{
	return g_player.rotDesh.y;
}

//プレイヤーの移動量取得
float PlayerMove(int nRot)
{
	switch (nRot)
	{
	case 0:
		return g_player.move.x;
		break;
	case 1:
		return g_player.move.y;
		break;
	case 2:
		return g_player.move.z;
		break;
	}
	return 0.0f;
}

//プレイヤーのマトリックス取得
D3DXMATRIX Playermtx(void)
{
	return g_player.mtxWorld;
}

//プレイヤーの攻撃の当たり判定の場所取得
float CollisionPos(int nNumber, int nRot)
{
	switch (nRot)
	{
	case 0:
		return g_player.MotionInfo.Collision[nNumber].mtxWorld._41;
		break;
	case 1:
		return g_player.MotionInfo.Collision[nNumber].mtxWorld._42;
		break;
	case 2:
		return g_player.MotionInfo.Collision[nNumber].mtxWorld._43;
		break;
	}
	return 0.0f;
}

//ロックオンしてるか取得
bool TargetRockon(void)
{
	return g_player.bRock;
}

//ロックオン外す
void RemovePlayerRock(void)
{
	g_player.bRock = false;
}

//プレイヤーの攻撃の当たり判定何個か取得
int NumParts(void)
{
	return g_player.MotionInfo.nNumCol;
}

//攻撃の当たり判定の場所設定
void CollisionPos(void)
{
	int nCntCol;
	for (nCntCol = 0; nCntCol < g_player.MotionInfo.nNumCol; nCntCol++)
	{
		if (g_player.MotionInfo.Collision[nCntCol].nStarttime <= g_player.nTotalTime &&
			g_player.nTotalTime < g_player.MotionInfo.Collision[nCntCol].nFinishtime)
		{
			D3DXMATRIX mtxTrans; //パーツ用計算用マトリックス
			D3DXMATRIX mtxParent; //親のマトリックス

			//当たり判定のワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_player.MotionInfo.Collision[nCntCol].mtxWorld);

			//当たり判定の位置を反映
			D3DXMatrixTranslation(&mtxTrans, g_player.MotionInfo.Collision[nCntCol].pos.x, g_player.MotionInfo.Collision[nCntCol].pos.y, g_player.MotionInfo.Collision[nCntCol].pos.z);
			D3DXMatrixMultiply(&g_player.MotionInfo.Collision[nCntCol].mtxWorld, &g_player.MotionInfo.Collision[nCntCol].mtxWorld, &mtxTrans);
			
			//親のマトリックス取得
			mtxParent = g_player.aModel[g_player.MotionInfo.Collision[nCntCol].nIdxModelParent].mtxWorld;
			
			//算出した各当たり判定のワールドマトリックスと親のマトリックスを掛け合わせる
			D3DXMatrixMultiply(&g_player.MotionInfo.Collision[nCntCol].mtxWorld,
				&g_player.MotionInfo.Collision[nCntCol].mtxWorld,
				&mtxParent);
		}
	}
}

//攻撃の当たり判定
bool CollisionDamage(D3DXVECTOR3 pos, float fRadius)
{
	int nCntCol;
	float fLengthX; //当たり判定と敵のX方向の長さ
	float fLengthY; //当たり判定と敵のY方向の長さ
	float fLengthZ; //当たり判定と敵のZ方向の長さ
	float fHDistance; //当たり判定と敵の水平方向の距離
	float fDistance; //当たり判定と敵の距離
	for (nCntCol = 0; nCntCol < g_player.MotionInfo.nNumCol; nCntCol++)
	{
		if (g_player.MotionInfo.Collision[nCntCol].nStarttime <= g_player.nTotalTime &&
			g_player.nTotalTime < g_player.MotionInfo.Collision[nCntCol].nFinishtime)
		{
			fLengthX = powf(pos.x - g_player.MotionInfo.Collision[nCntCol].mtxWorld._41, 2);
			fLengthY = powf(pos.y + (fRadius / 2) - g_player.MotionInfo.Collision[nCntCol].mtxWorld._42, 2);
			fLengthZ = powf(pos.z - g_player.MotionInfo.Collision[nCntCol].mtxWorld._43, 2);
			fHDistance = fLengthX + fLengthZ;
			fDistance = sqrtf(fLengthY + fHDistance);

			//距離計算、半径より短い
			if (fDistance <= fRadius + g_player.MotionInfo.Collision[nCntCol].fRadius)
			{
				g_player.nCol = nCntCol;
				return true;
			}
		}
	}
	return false;
}

//プレイヤーの軌跡設定
void PlayerOrbit(void)
{
	if (g_player.MotionInfo.orbit.nStartTime <= g_player.nTotalTime &&
		g_player.nTotalTime < g_player.MotionInfo.orbit.nFinishTime)
	{
		D3DXMATRIX mtxTrans; //パーツ用計算用マトリックス
		D3DXMATRIX mtxParent; //親のマトリックス
		//当たり判定のワールドマトリックスの初期化
		D3DXMatrixIdentity(&g_player.MotionInfo.orbit.TipmtxWorld);

		//当たり判定の位置を反映
		D3DXMatrixTranslation(&mtxTrans, g_player.MotionInfo.orbit.pos.x, g_player.MotionInfo.orbit.pos.y, g_player.MotionInfo.orbit.pos.z);
		D3DXMatrixMultiply(&g_player.MotionInfo.orbit.TipmtxWorld, &g_player.MotionInfo.orbit.TipmtxWorld, &mtxTrans);
		
		//親のマトリックス取得
		mtxParent = g_player.aModel[g_player.MotionInfo.orbit.nIdxParent].mtxWorld;
		
		//算出した各当たり判定のワールドマトリックスと親のマトリックスを掛け合わせる
		D3DXMatrixMultiply(&g_player.MotionInfo.orbit.TipmtxWorld,
			&g_player.MotionInfo.orbit.TipmtxWorld,
			&mtxParent);
		
		//軌跡を設定する時間だったら
		if (g_player.MotionInfo.orbit.nStartTime < g_player.nTotalTime &&
			g_player.nTotalTime < g_player.MotionInfo.orbit.nFinishTime)
		{
			//軌跡設定
			SetOrbit(D3DXVECTOR3(g_player.aModel[g_player.MotionInfo.orbit.nIdxParent].mtxWorld._41, g_player.aModel[g_player.MotionInfo.orbit.nIdxParent].mtxWorld._42, g_player.aModel[g_player.MotionInfo.orbit.nIdxParent].mtxWorld._43),
				D3DXVECTOR3(g_player.MotionInfo.orbit.TipmtxWorld._41, g_player.MotionInfo.orbit.TipmtxWorld._42, g_player.MotionInfo.orbit.TipmtxWorld._43),
				g_player.MotionInfo.orbit.posOld, g_player.MotionInfo.orbit.TipposOld, g_player.MotionInfo.orbit.nTime);
		}
		g_player.MotionInfo.orbit.posOld = D3DXVECTOR3(g_player.aModel[g_player.MotionInfo.orbit.nIdxParent].mtxWorld._41, g_player.aModel[g_player.MotionInfo.orbit.nIdxParent].mtxWorld._42, g_player.aModel[g_player.MotionInfo.orbit.nIdxParent].mtxWorld._43);
		g_player.MotionInfo.orbit.TipposOld = D3DXVECTOR3(g_player.MotionInfo.orbit.TipmtxWorld._41, g_player.MotionInfo.orbit.TipmtxWorld._42, g_player.MotionInfo.orbit.TipmtxWorld._43);
	}
}

//敵の方を見る
float PlayerLookEnemy(void)
{
	float fRot = 0.0f;
	fRot = RockCamera() - atan2f(g_player.pos.x - EnemyPos(0, g_player.nEnemy), g_player.pos.z - EnemyPos(2, g_player.nEnemy));
	
	if (fRot > D3DX_PI)
	{
		fRot -= D3DX_PI * 2;
	}
	if (fRot < -D3DX_PI)
	{
		fRot += D3DX_PI * 2;
	}
	
	return fRot;
}

//敵吹き飛ばす
D3DXVECTOR3 EnemyClash(D3DXVECTOR3 pos)
{
	float fRot = 0.0f;
	D3DXVECTOR3 clash;
	fRot = atan2f(pos.x - g_player.pos.x, pos.z - g_player.pos.z);
	clash.x = sinf(fRot) * g_player.MotionInfo.Collision[g_player.nCol].fHClash;
	clash.y = g_player.MotionInfo.Collision[g_player.nCol].fVClash;
	clash.z = cosf(fRot) * g_player.MotionInfo.Collision[g_player.nCol].fHClash;
	return clash;
}

//吹き飛ばす方向
float ClashRot(D3DXVECTOR3 pos)
{
	float fRot = 0.0f;
	fRot = atan2f(pos.x - g_player.pos.x, pos.z - g_player.pos.z);
	return fRot;
}

//プレイヤーが吹き飛ぶ方向
float PlayerClashRot(D3DXVECTOR3 pos)
{
	float fRot = 0.0f;
	fRot = atan2f(g_player.pos.x - pos.x, g_player.pos.z - pos.z);
	return fRot;
}

//当たり判定の時間
int DamageTime(void)
{
	return g_player.MotionInfo.Collision[g_player.nCol].nDamageTime;
}

//ダメージ量
int Damage(void)
{
	return g_player.MotionInfo.Collision[g_player.nCol].nDamage;
}

//ロックオンしてる敵
int PlayerRockEnemy(void)
{
	return g_player.nEnemy;
}

//位置関係に応じて攻撃が変わる
MOTIONTYPE AttackType(float fDistance, float fHeight)
{
	g_player.nCombo++;

	//距離長い
	if (fDistance > 120.0f)
	{
		//直前のモーションが突進
		if (g_player.Oldmotion == MOTIONTYPE_RUSH)
		{
			return MOTIONTYPE_NORMAL;
		}
		//それ以外
		else
		{
			return MOTIONTYPE_RUSH;
		}
	}
	//上にいる
	else if(fHeight < -60.0f)
	{
		return MOTIONTYPE_ARIAIL;
	}
	//下にいる
	else if (fHeight > 100.0f)
	{
		return MOTIONTYPE_GROUND;
	}
	//周辺に敵がいる
	else if (EnemyNear(g_player.pos, g_player.fHeight) == true)
	{
		//直前の攻撃が周辺攻撃
		if (g_player.Oldmotion == MOTIONTYPE_ROUND)
		{
			return MOTIONTYPE_NORMAL;
		}
		//それ以外
		else
		{
			return MOTIONTYPE_ROUND;
		}
	}
	//どの条件も満たさない
	else
	{
		//直前のモーションが普通
		if (g_player.Oldmotion == MOTIONTYPE_NORMAL)
		{
			return MOTIONTYPE_RUSH;
		}
		//それ以外
		else
		{
			return MOTIONTYPE_NORMAL;
		}
	}
	return MOTIONTYPE_NORMAL;
}

//プレイヤーの体の向き
float PlayerPartsRot(void)
{
	return g_player.aModel[0].rot.y;
}

//プレイヤーの現在キー
int PlayerKey(void)
{
	return g_player.nKey;
}

//プレイヤーとの距離
float PlayerDistance(D3DXVECTOR3 pos)
{
	float fLengthX;
	float fLengthZ;
	float fDistance;
	fLengthX = powf(pos.x - g_player.pos.x, 2);
	fLengthZ = powf(pos.z - g_player.pos.z, 2);
	fDistance = sqrtf(fLengthX + fLengthZ);
	return fDistance;
}

//プレイヤーの高さ
float PlayerHeight(void)
{
	return g_player.fHeight;
}

//プレイヤーの無敵時間設定
void PlayerDamageTime(int nDamageTime)
{
	g_player.nDamageTime = nDamageTime;
}

//プレイヤーのダメージ判定
bool PlayerDamage(D3DXVECTOR3 pos, float fRadius)
{
	if (g_player.nDamageTime > 0)
	{
		return false;
	}
	float fLengthX; //当たり判定と敵のX方向の長さ
	float fLengthY; //当たり判定と敵のY方向の長さ
	float fLengthZ; //当たり判定と敵のZ方向の長さ
	float fHDistance; //当たり判定と敵の水平方向の距離
	float fDistance; //当たり判定と敵の距離
	fLengthX = powf(pos.x - g_player.pos.x, 2);
	fLengthY = powf(pos.y - g_player.pos.y + (g_player.fHeight / 2), 2);
	fLengthZ = powf(pos.z - g_player.pos.z, 2);
	fHDistance = fLengthX + fLengthZ;
	fDistance = sqrtf(fLengthY + fHDistance);
	if (fDistance <= fRadius + g_player.fRadius)
	{
		return true;
	}
	return false;
}

//ダメージ処理
void PlayerDamage(int nDamage, int nDamageTime, float fRoty, D3DXVECTOR3 Clash)
{
	g_player.nHP -= nDamage;
	g_player.nDamageTime = nDamageTime;
	g_player.rot.y = fRoty;
	g_player.rotDesh.y = fRoty;
	g_player.move = Clash;
	if (g_player.nHP > 0)
	{
		MotionChange(MOTIONTYPE_DAMAGE);
	}
	else
	{
		MotionChange(MOTIONTYPE_DIE);
	}
}

//現在HP
int PlayerHP(void)
{
	return g_player.nHP;
}

//最大HPと現在HPの割合
float PlayerHPRate(void)
{
	float fHPRate;
	fHPRate = (float)g_player.nHP / (float)g_player.nMaxHP;
	if (fHPRate <= 0)
	{
		return 0.0f;
	}
	return fHPRate;
}

//倒した数加算
void PlayerKillAdd(void)
{
	g_player.nKill++;
}

//倒した数取得
int PlayerKill(void)
{
	return g_player.nKill;
}