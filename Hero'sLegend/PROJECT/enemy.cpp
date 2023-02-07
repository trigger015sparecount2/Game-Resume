//---------------------------
//Author:三上航世
//モデル(player.cpp)
//---------------------------
#include "enemy.h"
#include "input.h"
#include "camera.h"
#include "shadow.h"
#include "gamepad.h"
#include "bullet.h"
#include "particle.h"
#include "meshfield.h"
#include "object.h"
#include "rockon.h"
#include "player.h"
#include "sound.h"
#include "wall.h"

//マクロ定義
#define CHARAFILENAME "data/motion_armoredge.txt"
#define CHARAFILENAME2 "data/motion_helipod.txt"
#define MAX_ENEMY (64)

//プロトタイプ宣言
void EnemyMoveX(int nCntEnemy);
void EnemyMoveZ(int nCntEnemy);
void EnemySlowX(int nCntEnemy);
void EnemySlowZ(int nCntEnemy);
void EnemyMotionChange(int nCntEnemy, ENEMYMOTION enemymotion);
void EnemyMotion(int nCntEnemy);
void CollisionPos(int nCntEnemy);
void EnemyDamage(int nCntEnemy);
void EnemyAI(int nCntEnemy);
bool EnemyAtkNum(void);

//モデル
typedef struct
{
	LPD3DXMESH pMesh;							//メッシュ（頂点情報）へのポインタ
	LPD3DXBUFFER pBuffMat;						//マテリアル（材質情報）へのポインタ
	DWORD nNumMat;								//マテリアルの数
	LPDIRECT3DTEXTURE9 TextureModel[4] = {};	//テクスチャ
	D3DXVECTOR3 pos;							//位置
	D3DXVECTOR3 posOri;							//元々の位置
	D3DXVECTOR3 rot;							//向き
	D3DXVECTOR3 rotOri;							//元々の向き
	D3DXMATRIX mtxWorld;						//ワールドマトリックス
	int nIdxModelParent;						//親モデルのインデックス
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

//軌跡
typedef struct
{
	D3DXVECTOR3 pos;		//軌跡の先端の位置
	D3DXMATRIX TipmtxWorld;	//軌跡の先端のマトリックス
	D3DXVECTOR3 TipposOld;	//軌跡の先端の前回位置
	D3DXVECTOR3 posOld;		//軌跡の根元の前回位置
	int nTime;				//消えるまでの時間
	int nStartTime;			//発生する時間
	int nFinishTime;		//終了の時間
	int nIdxParent;			//親モデルの番号
} Orbit;

//銃弾
typedef struct
{
	D3DXVECTOR3 pos;		//発射根
	D3DXMATRIX mtxWorld;	//ワールドマトリックス
	int nLag;				//発射間隔
	int nBullet;			//
	int nStartBul;			//撃ち始めの時間
	int nFinishBul;			//終わりの時間
	int nIdxParent;			//親インデックス
	int nDamage;			//ダメージ量
	int nDamageTime;		//ヒット間隔
	float fHClash;			//水平方向の吹っ飛ばし力
	float fVClash;			//垂直方向の吹っ飛ばし力
} Bang;

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
	float fMove;	//そのキーの移動量
	KEY aKey[8];	//各モデルのキー要素 []内はパーツの最大数
} KEY_INFO;

//モーション情報の構造体
typedef struct
{
	bool bLoop;				//ループするかどうか
	int nNumKey;			//キーの総数
	Collision Collision[4];	//当たり判定
	int nNumCol;			//当たり判定の球の数
	Orbit orbit[4];			//軌跡
	int nNumOrbit;			//軌跡の数
	Bang Bang[4];			//弾発射
	int nNumBul;			//弾の発射根
	KEY_INFO aKeyInfo[16];	//キー情報 []内はキーの最大数
} MOTION_INFO;

typedef struct
{
	D3DXVECTOR3 pos;		//位置
	D3DXVECTOR3 posOld;		//前回位置
	D3DXVECTOR3 rot;		//向き
	D3DXVECTOR3 rotDesh;	//目的の向き
	D3DXMATRIX mtxWorld;	//ワールドマトリックス
	D3DXVECTOR3 move;		//移動量
	Model aModel[8];		//モデル（パーツ）
	int nNumModel;			//モデル（パーツ）数
	int nShadow;			//影の番号
	bool bMove;				//動いているか
	bool bLand;				//着地してるか
	MOTION_INFO aMotionInfo[MAX_ENEMYMOTION]; //モーション情報
	int nNumMotion;				//モーション数
	ENEMYMOTION enemymotion;	//モーションタイプ
	bool bLoopMotion;			//ループするかどうか
	int nNumKey;				//キー数
	int nKey;					//キーNo.
	int nCounterMotion;			//モーションのカウンター
	KEY aKeyDiff[8];			//差分 []内はパーツ数
	KEY KeyMove;				//モーション開始から体がどれだけ動いたか
	float fRadius;				//半径
	float fHeight;				//高さ
	float fMove;				//最大速度
	float fJump;				//ジャンプ力
	int nMoveParts;				//モデルの基準になる部位
	int nTotalTime;				//モーション開始から合計の時間
	int nDamageTime;			//被弾してからの時間
	int nThinkTime;				//棒立ち時間
	D3DXVECTOR3 posDesh;		//目的位置
	MOTION_INFO motionInfo;		//モーション情報
	int nHp;					//HP
	int nType;					//種類
	bool bUse;					//使ってるか
} Enemy;

//グローバル変数
Enemy g_enemy[MAX_ENEMY];	//敵
Enemy g_enemymodel[4];		//敵の情報
Model g_Emodel[8];			//モデルの情報 []内はパーツ数
int g_nNumEnemyModel;		//キャラに使うモデルが何種類あるか

//初期化処理
HRESULT InitEnemy(void)
{
	LPDIRECT3DDEVICE9 pDevice; //デバイスのポインタ
	pDevice = GetDevice();     //デバイスを取得する
	int nCntParts = 0;
	int nCntModelParts = 0;
	int nCntMotion = 0;
	int nCntKey = 0;
	int nCntCol = 0;
	int nCntBullet = 0;
	int nNumEnemy = 0;
	int nCntOrbit = 0;
	char aFile[256];
	bool bChara = false;
	bool bParts = false;
	bool bMotion = false;
	bool bKey = false;
	bool bKeySet = false;
	bool bColSet = false;
	bool bCol = false;
	bool bBulletSet = false;
	bool bBullet = false;
	bool bOrbit = false;
	bool bOrbitSet = false;
	int nIndex;
	int nLoop;

	//ファイル読み込み
	FILE *pFile;
	pFile = fopen(CHARAFILENAME, "r");
	if (pFile != NULL)
	{
		while (true)
		{
			fscanf(pFile, "%s", &aFile[0]); //fscanfを繰り返してファイルを読み取っていく
			if (strcmp(&aFile[0], "NUM_MODEL") == 0) //モデル数
			{
				fscanf(pFile, "%s", &aFile[0]);
				fscanf(pFile, "%d", &g_nNumEnemyModel);
			}
			if (strcmp(&aFile[0], "MODEL_FILENAME") == 0) //モデルファイル読み込み
			{
				fscanf(pFile, "%s", &aFile[0]);
				fscanf(pFile, "%s", &aFile[0]);
				D3DXLoadMeshFromX(&aFile[0],
					D3DXMESH_SYSTEMMEM,
					pDevice,
					NULL,
					&g_Emodel[nCntParts].pBuffMat,
					NULL,
					&g_Emodel[nCntParts].nNumMat,
					&g_Emodel[nCntParts].pMesh);
				nCntParts++;
			}
			if (bChara == true) //CHARACTERSET時
			{
				if (strcmp(&aFile[0], "NUM_PARTS") == 0) //パーツ数
				{
					fscanf(pFile, "%s", &aFile[0]);
					fscanf(pFile, "%d", &g_enemymodel[nNumEnemy].nNumModel);
				}
				if (strcmp(&aFile[0], "MOVE") == 0) //移動速度
				{
					fscanf(pFile, "%s", &aFile[0]);
					fscanf(pFile, "%f", &g_enemymodel[nNumEnemy].fMove);
				}
				if (strcmp(&aFile[0], "JUMP") == 0) //ジャンプ力
				{
					fscanf(pFile, "%s", &aFile[0]);
					fscanf(pFile, "%f", &g_enemymodel[nNumEnemy].fJump);
				}
				if (strcmp(&aFile[0], "RADIUS") == 0) //半径
				{
					fscanf(pFile, "%s", &aFile[0]);
					fscanf(pFile, "%f", &g_enemymodel[nNumEnemy].fRadius);
				}
				if (strcmp(&aFile[0], "HEIGHT") == 0) //身長
				{
					fscanf(pFile, "%s", &aFile[0]);
					fscanf(pFile, "%f", &g_enemymodel[nNumEnemy].fHeight);
				}
				if (strcmp(&aFile[0], "HP") == 0) //体力
				{
					fscanf(pFile, "%s", &aFile[0]);
					fscanf(pFile, "%d", &g_enemymodel[nNumEnemy].nHp);
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
						g_enemymodel[nNumEnemy].nMoveParts = nCntParts;
					}
					if (strcmp(&aFile[0], "INDEX") == 0) //モデルファイル読み込みの時の、何番目のモデルを呼び出すか
					{
						fscanf(pFile, "%s", &aFile[0]);
						fscanf(pFile, "%d", &nIndex);
						g_enemymodel[nNumEnemy].aModel[nCntParts].pBuffMat = g_Emodel[nIndex].pBuffMat;
						g_enemymodel[nNumEnemy].aModel[nCntParts].nNumMat = g_Emodel[nIndex].nNumMat;
						g_enemymodel[nNumEnemy].aModel[nCntParts].pMesh = g_Emodel[nIndex].pMesh;
					}
					if (strcmp(&aFile[0], "PARENT") == 0) //親モデル
					{
						fscanf(pFile, "%s", &aFile[0]);
						fscanf(pFile, "%d", &g_enemymodel[nNumEnemy].aModel[nCntParts].nIdxModelParent);
					}
					if (strcmp(&aFile[0], "POS") == 0) //各パーツの場所
					{
						fscanf(pFile, "%s", &aFile[0]);
						fscanf(pFile, "%f %f %f", &g_enemymodel[nNumEnemy].aModel[nCntParts].posOri.x, &g_enemymodel[nNumEnemy].aModel[nCntParts].posOri.y, &g_enemymodel[nNumEnemy].aModel[nCntParts].posOri.z);
					}
					if (strcmp(&aFile[0], "ROT") == 0) //各パーツの角度
					{
						fscanf(pFile, "%s", &aFile[0]);
						fscanf(pFile, "%f %f %f", &g_enemymodel[nNumEnemy].aModel[nCntParts].rotOri.x, &g_enemymodel[nNumEnemy].aModel[nCntParts].rotOri.y, &g_enemymodel[nNumEnemy].aModel[nCntParts].rotOri.z);
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
						g_enemymodel[nNumEnemy].aMotionInfo[nCntMotion].bLoop = false;
					}
					else
					{
						g_enemymodel[nNumEnemy].aMotionInfo[nCntMotion].bLoop = true;
					}
				}
				if (strcmp(&aFile[0], "NUM_KEY") == 0) //いくつキーがあるか
				{
					fscanf(pFile, "%s", &aFile[0]);
					fscanf(pFile, "%d", &g_enemymodel[nNumEnemy].aMotionInfo[nCntMotion].nNumKey);
				}
				if (strcmp(&aFile[0], "COLLISIONSET") == 0)
				{
					bColSet = true;
					nCntCol = 0;
				}
				if (strcmp(&aFile[0], "END_COLLISIONSET") == 0)
				{
					g_enemymodel[nNumEnemy].aMotionInfo[nCntMotion].nNumCol = nCntCol;
					bColSet = false;
				}
				if (strcmp(&aFile[0], "ORBITSET") == 0)
				{
					bOrbitSet = true;
					nCntCol = 0;
				}
				if (strcmp(&aFile[0], "END_ORBITSET") == 0)
				{
					g_enemymodel[nNumEnemy].aMotionInfo[nCntMotion].nNumOrbit = nCntOrbit;
					bOrbitSet = false;
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
							fscanf(pFile, "%d %f %f %f %f %d %d", &g_enemymodel[nNumEnemy].aMotionInfo[nCntMotion].Collision[nCntCol].nIdxModelParent,
								&g_enemymodel[nNumEnemy].aMotionInfo[nCntMotion].Collision[nCntCol].pos.x, &g_enemymodel[nNumEnemy].aMotionInfo[nCntMotion].Collision[nCntCol].pos.y, &g_enemymodel[nNumEnemy].aMotionInfo[nCntMotion].Collision[nCntCol].pos.z,
								&g_enemymodel[nNumEnemy].aMotionInfo[nCntMotion].Collision[nCntCol].fRadius, &g_enemymodel[nNumEnemy].aMotionInfo[nCntMotion].Collision[nCntCol].nStarttime, &g_enemymodel[nNumEnemy].aMotionInfo[nCntMotion].Collision[nCntCol].nFinishtime);
						}
						if (strcmp(&aFile[0], "DMG") == 0) //当たり判定（水平吹っ飛ばし力、垂直吹っ飛ばし力、ダメージ量、ヒット間隔）
						{
							fscanf(pFile, "%s", &aFile[0]);
							fscanf(pFile, "%f %f %d %d", &g_enemymodel[nNumEnemy].aMotionInfo[nCntMotion].Collision[nCntCol].fHClash, &g_enemymodel[nNumEnemy].aMotionInfo[nCntMotion].Collision[nCntCol].fVClash,
								&g_enemymodel[nNumEnemy].aMotionInfo[nCntMotion].Collision[nCntCol].nDamage, &g_enemymodel[nNumEnemy].aMotionInfo[nCntMotion].Collision[nCntCol].nDamageTime);
						}
					}
				}
				if (bOrbitSet == true) //ORBITSET時
				{
					if (strcmp(&aFile[0], "ORBIT") == 0)
					{
						bOrbit = true;
					}
					if (strcmp(&aFile[0], "END_ORBIT") == 0)
					{
						bOrbit = false;
						nCntOrbit++;
					}
					if (strcmp(&aFile[0], "OBT") == 0) //軌跡（場所、表示時間、発生時間）
					{
						fscanf(pFile, "%s", &aFile[0]);
						fscanf(pFile, "%d %f %f %f %d %d %d", &g_enemymodel[nNumEnemy].aMotionInfo[nCntMotion].orbit[nCntOrbit].nIdxParent,
							&g_enemymodel[nNumEnemy].aMotionInfo[nCntMotion].orbit[nCntOrbit].pos.x, &g_enemymodel[nNumEnemy].aMotionInfo[nCntMotion].orbit[nCntOrbit].pos.y,
							&g_enemymodel[nNumEnemy].aMotionInfo[nCntMotion].orbit[nCntOrbit].pos.z, &g_enemymodel[nNumEnemy].aMotionInfo[nCntMotion].orbit[nCntOrbit].nTime,
							&g_enemymodel[nNumEnemy].aMotionInfo[nCntMotion].orbit[nCntOrbit].nStartTime, &g_enemymodel[nNumEnemy].aMotionInfo[nCntMotion].orbit[nCntOrbit].nFinishTime);
					}
				}
				if (bKeySet == true) //KEYSET時
				{
					if (strcmp(&aFile[0], "FRAME") == 0) //フレーム数
					{
						fscanf(pFile, "%s", &aFile[0]);
						fscanf(pFile, "%d", &g_enemymodel[nNumEnemy].aMotionInfo[nCntMotion].aKeyInfo[nCntKey].nFrame);
					}
					if (strcmp(&aFile[0], "MOVE") == 0) //移動量
					{
						fscanf(pFile, "%s", &aFile[0]);
						fscanf(pFile, "%f", &g_enemymodel[nNumEnemy].aMotionInfo[nCntMotion].aKeyInfo[nCntKey].fMove);
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
							fscanf(pFile, "%f %f %f", &g_enemymodel[nNumEnemy].aMotionInfo[nCntMotion].aKeyInfo[nCntKey].aKey[nCntParts].fPosX, &g_enemymodel[nNumEnemy].aMotionInfo[nCntMotion].aKeyInfo[nCntKey].aKey[nCntParts].fPosY, &g_enemymodel[nNumEnemy].aMotionInfo[nCntMotion].aKeyInfo[nCntKey].aKey[nCntParts].fPosZ);
						}
						if (strcmp(&aFile[0], "ROT") == 0) //角度
						{
							fscanf(pFile, "%s", &aFile[0]);
							fscanf(pFile, "%f %f %f", &g_enemymodel[nNumEnemy].aMotionInfo[nCntMotion].aKeyInfo[nCntKey].aKey[nCntParts].fRotX, &g_enemymodel[nNumEnemy].aMotionInfo[nCntMotion].aKeyInfo[nCntKey].aKey[nCntParts].fRotY, &g_enemymodel[nNumEnemy].aMotionInfo[nCntMotion].aKeyInfo[nCntKey].aKey[nCntParts].fRotZ);
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
	g_enemymodel[nNumEnemy].aMotionInfo[ENEMYMOTION_RETURN].bLoop = false;
	g_enemymodel[nNumEnemy].aMotionInfo[ENEMYMOTION_RETURN].aKeyInfo[0].nFrame = 14;
	g_enemymodel[nNumEnemy].aMotionInfo[ENEMYMOTION_RETURN].nNumKey = 1;
	g_enemymodel[nNumEnemy].KeyMove.fPosX = 0.0f;
	g_enemymodel[nNumEnemy].KeyMove.fPosY = 0.0f;
	g_enemymodel[nNumEnemy].KeyMove.fPosZ = 0.0f;
	g_enemymodel[nNumEnemy].nDamageTime = 0;
	for (nCntParts = 0; nCntParts < g_enemymodel[nNumEnemy].nNumModel; nCntParts++)
	{
		g_enemymodel[nNumEnemy].aMotionInfo[ENEMYMOTION_RETURN].aKeyInfo[0].aKey[nCntParts].fPosX = g_enemymodel[nNumEnemy].aMotionInfo[ENEMYMOTION_NEUTRAL].aKeyInfo[1].aKey[nCntParts].fPosX;
		g_enemymodel[nNumEnemy].aMotionInfo[ENEMYMOTION_RETURN].aKeyInfo[0].aKey[nCntParts].fPosY = g_enemymodel[nNumEnemy].aMotionInfo[ENEMYMOTION_NEUTRAL].aKeyInfo[1].aKey[nCntParts].fPosY;
		g_enemymodel[nNumEnemy].aMotionInfo[ENEMYMOTION_RETURN].aKeyInfo[0].aKey[nCntParts].fPosZ = g_enemymodel[nNumEnemy].aMotionInfo[ENEMYMOTION_NEUTRAL].aKeyInfo[1].aKey[nCntParts].fPosZ;
		g_enemymodel[nNumEnemy].aMotionInfo[ENEMYMOTION_RETURN].aKeyInfo[0].aKey[nCntParts].fRotX = g_enemymodel[nNumEnemy].aMotionInfo[ENEMYMOTION_NEUTRAL].aKeyInfo[1].aKey[nCntParts].fRotX;
		g_enemymodel[nNumEnemy].aMotionInfo[ENEMYMOTION_RETURN].aKeyInfo[0].aKey[nCntParts].fRotY = g_enemymodel[nNumEnemy].aMotionInfo[ENEMYMOTION_NEUTRAL].aKeyInfo[1].aKey[nCntParts].fRotY;
		g_enemymodel[nNumEnemy].aMotionInfo[ENEMYMOTION_RETURN].aKeyInfo[0].aKey[nCntParts].fRotZ = g_enemymodel[nNumEnemy].aMotionInfo[ENEMYMOTION_NEUTRAL].aKeyInfo[1].aKey[nCntParts].fRotZ;
		//マテリアル情報に対するポインタを取得
		pMat = (D3DXMATERIAL*)g_enemymodel[nNumEnemy].aModel[nCntParts].pBuffMat->GetBufferPointer();
		int nCntTex = 0;
		for (int nCntMat = 0; nCntMat < (int)g_enemymodel[nNumEnemy].aModel[nCntParts].nNumMat; nCntMat++)
		{
			if (pMat[nCntMat].pTextureFilename != NULL)
			{
				D3DXCreateTextureFromFile(pDevice,
					pMat[nCntMat].pTextureFilename,
					&g_enemymodel[nNumEnemy].aModel[nCntParts].TextureModel[nCntTex]);
				nCntTex++;
			}
		}
	}
	g_enemymodel[nNumEnemy].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_enemymodel[nNumEnemy].posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_enemymodel[nNumEnemy].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_enemymodel[nNumEnemy].rotDesh = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_enemymodel[nNumEnemy].bMove = false;
	g_enemymodel[nNumEnemy].bLand = true;
	g_enemymodel[nNumEnemy].nTotalTime = 0;
	g_enemymodel[nNumEnemy].nThinkTime = 120;
	g_enemymodel[nNumEnemy].posDesh = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_enemymodel[nNumEnemy].bUse = true;

	nNumEnemy++;
	nCntParts = 0;
	nCntMotion = 0;
	nCntKey = 0;
	nCntCol = 0;
	nCntOrbit = 0;
	bOrbit = false;
	bOrbitSet = false;
	bChara = false;
	bParts = false;
	bMotion = false;
	bKey = false;
	bKeySet = false;
	bColSet = false;
	bCol = false;

	pFile = fopen(CHARAFILENAME2, "r");
	if (pFile != NULL)
	{
		while (true)
		{
			fscanf(pFile, "%s", &aFile[0]); //fscanfを繰り返してファイルを読み取っていく
			if (strcmp(&aFile[0], "NUM_MODEL") == 0) //モデル数
			{
				fscanf(pFile, "%s", &aFile[0]);
				fscanf(pFile, "%d", &g_nNumEnemyModel);
			}
			if (strcmp(&aFile[0], "MODEL_FILENAME") == 0) //モデルファイル読み込み
			{
				fscanf(pFile, "%s", &aFile[0]);
				fscanf(pFile, "%s", &aFile[0]);
				D3DXLoadMeshFromX(&aFile[0],
					D3DXMESH_SYSTEMMEM,
					pDevice,
					NULL,
					&g_Emodel[nCntParts].pBuffMat,
					NULL,
					&g_Emodel[nCntParts].nNumMat,
					&g_Emodel[nCntParts].pMesh);
				nCntParts++;
			}
			if (bChara == true) //CHARACTERSET時
			{
				if (strcmp(&aFile[0], "NUM_PARTS") == 0) //パーツ数
				{
					fscanf(pFile, "%s", &aFile[0]);
					fscanf(pFile, "%d", &g_enemymodel[nNumEnemy].nNumModel);
				}
				if (strcmp(&aFile[0], "MOVE") == 0) //移動速度
				{
					fscanf(pFile, "%s", &aFile[0]);
					fscanf(pFile, "%f", &g_enemymodel[nNumEnemy].fMove);
				}
				if (strcmp(&aFile[0], "JUMP") == 0) //ジャンプ力
				{
					fscanf(pFile, "%s", &aFile[0]);
					fscanf(pFile, "%f", &g_enemymodel[nNumEnemy].fJump);
				}
				if (strcmp(&aFile[0], "RADIUS") == 0) //半径
				{
					fscanf(pFile, "%s", &aFile[0]);
					fscanf(pFile, "%f", &g_enemymodel[nNumEnemy].fRadius);
				}
				if (strcmp(&aFile[0], "HEIGHT") == 0) //身長
				{
					fscanf(pFile, "%s", &aFile[0]);
					fscanf(pFile, "%f", &g_enemymodel[nNumEnemy].fHeight);
				}
				if (strcmp(&aFile[0], "HP") == 0) //体力
				{
					fscanf(pFile, "%s", &aFile[0]);
					fscanf(pFile, "%d", &g_enemymodel[nNumEnemy].nHp);
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
						g_enemymodel[nNumEnemy].nMoveParts = nCntParts;
					}
					if (strcmp(&aFile[0], "INDEX") == 0) //モデルファイル読み込みの時の、何番目のモデルを呼び出すか
					{
						fscanf(pFile, "%s", &aFile[0]);
						fscanf(pFile, "%d", &nIndex);
						g_enemymodel[nNumEnemy].aModel[nCntParts].pBuffMat = g_Emodel[nIndex].pBuffMat;
						g_enemymodel[nNumEnemy].aModel[nCntParts].nNumMat = g_Emodel[nIndex].nNumMat;
						g_enemymodel[nNumEnemy].aModel[nCntParts].pMesh = g_Emodel[nIndex].pMesh;
					}
					if (strcmp(&aFile[0], "PARENT") == 0) //親モデル
					{
						fscanf(pFile, "%s", &aFile[0]);
						fscanf(pFile, "%d", &g_enemymodel[nNumEnemy].aModel[nCntParts].nIdxModelParent);
					}
					if (strcmp(&aFile[0], "POS") == 0) //各パーツの場所
					{
						fscanf(pFile, "%s", &aFile[0]);
						fscanf(pFile, "%f %f %f", &g_enemymodel[nNumEnemy].aModel[nCntParts].posOri.x, &g_enemymodel[nNumEnemy].aModel[nCntParts].posOri.y, &g_enemymodel[nNumEnemy].aModel[nCntParts].posOri.z);
					}
					if (strcmp(&aFile[0], "ROT") == 0) //各パーツの角度
					{
						fscanf(pFile, "%s", &aFile[0]);
						fscanf(pFile, "%f %f %f", &g_enemymodel[nNumEnemy].aModel[nCntParts].rotOri.x, &g_enemymodel[nNumEnemy].aModel[nCntParts].rotOri.y, &g_enemymodel[nNumEnemy].aModel[nCntParts].rotOri.z);
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
						g_enemymodel[nNumEnemy].aMotionInfo[nCntMotion].bLoop = false;
					}
					else
					{
						g_enemymodel[nNumEnemy].aMotionInfo[nCntMotion].bLoop = true;
					}
				}
				if (strcmp(&aFile[0], "NUM_KEY") == 0) //いくつキーがあるか
				{
					fscanf(pFile, "%s", &aFile[0]);
					fscanf(pFile, "%d", &g_enemymodel[nNumEnemy].aMotionInfo[nCntMotion].nNumKey);
				}
				if (strcmp(&aFile[0], "COLLISIONSET") == 0)
				{
					bColSet = true;
					nCntCol = 0;
				}
				if (strcmp(&aFile[0], "END_COLLISIONSET") == 0)
				{
					g_enemymodel[nNumEnemy].aMotionInfo[nCntMotion].nNumCol = nCntCol;
					bColSet = false;
				}
				if (strcmp(&aFile[0], "ORBITSET") == 0)
				{
					bOrbitSet = true;
					nCntCol = 0;
				}
				if (strcmp(&aFile[0], "END_ORBITSET") == 0)
				{
					g_enemymodel[nNumEnemy].aMotionInfo[nCntMotion].nNumOrbit = nCntOrbit;
					bOrbitSet = false;
				}
				if (strcmp(&aFile[0], "BULLETSET") == 0)
				{
					bBulletSet = true;
					nCntBullet = 0;
				}
				if (strcmp(&aFile[0], "END_BULLETSET") == 0)
				{
					g_enemymodel[nNumEnemy].aMotionInfo[nCntMotion].nNumBul = nCntBullet;
					bBulletSet = false;
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
							fscanf(pFile, "%d %f %f %f %f %d %d", &g_enemymodel[nNumEnemy].aMotionInfo[nCntMotion].Collision[nCntCol].nIdxModelParent,
								&g_enemymodel[nNumEnemy].aMotionInfo[nCntMotion].Collision[nCntCol].pos.x, &g_enemymodel[nNumEnemy].aMotionInfo[nCntMotion].Collision[nCntCol].pos.y, &g_enemymodel[nNumEnemy].aMotionInfo[nCntMotion].Collision[nCntCol].pos.z,
								&g_enemymodel[nNumEnemy].aMotionInfo[nCntMotion].Collision[nCntCol].fRadius, &g_enemymodel[nNumEnemy].aMotionInfo[nCntMotion].Collision[nCntCol].nStarttime, &g_enemymodel[nNumEnemy].aMotionInfo[nCntMotion].Collision[nCntCol].nFinishtime);
						}
						if (strcmp(&aFile[0], "DMG") == 0) //当たり判定（水平吹っ飛ばし力、垂直吹っ飛ばし力、ダメージ量、ヒット間隔）
						{
							fscanf(pFile, "%s", &aFile[0]);
							fscanf(pFile, "%f %f %d %d", &g_enemymodel[nNumEnemy].aMotionInfo[nCntMotion].Collision[nCntCol].fHClash, &g_enemymodel[nNumEnemy].aMotionInfo[nCntMotion].Collision[nCntCol].fVClash,
								&g_enemymodel[nNumEnemy].aMotionInfo[nCntMotion].Collision[nCntCol].nDamage, &g_enemymodel[nNumEnemy].aMotionInfo[nCntMotion].Collision[nCntCol].nDamageTime);
						}
					}
				}
				if (bOrbitSet == true) //ORBITSET時
				{
					if (strcmp(&aFile[0], "ORBIT") == 0)
					{
						bOrbit = true;
					}
					if (strcmp(&aFile[0], "END_ORBIT") == 0)
					{
						bOrbit = false;
						nCntOrbit++;
					}
					if (strcmp(&aFile[0], "OBT") == 0) //軌跡（場所、表示時間、発生時間）
					{
						fscanf(pFile, "%s", &aFile[0]);
						fscanf(pFile, "%d %f %f %f %d %d %d", &g_enemymodel[nNumEnemy].aMotionInfo[nCntMotion].orbit[nCntOrbit].nIdxParent,
							&g_enemymodel[nNumEnemy].aMotionInfo[nCntMotion].orbit[nCntOrbit].pos.x, &g_enemymodel[nNumEnemy].aMotionInfo[nCntMotion].orbit[nCntOrbit].pos.y,
							&g_enemymodel[nNumEnemy].aMotionInfo[nCntMotion].orbit[nCntOrbit].pos.z, &g_enemymodel[nNumEnemy].aMotionInfo[nCntMotion].orbit[nCntOrbit].nTime,
							&g_enemymodel[nNumEnemy].aMotionInfo[nCntMotion].orbit[nCntOrbit].nStartTime, &g_enemymodel[nNumEnemy].aMotionInfo[nCntMotion].orbit[nCntOrbit].nFinishTime);
					}
				}
				if (bBulletSet == true) //BULLETSET時
				{
					if (strcmp(&aFile[0], "BULLET") == 0)
					{
						bBullet = true;
					}
					if (strcmp(&aFile[0], "END_BULLET") == 0)
					{
						bBullet = false;
						nCntBullet++;
					}
					if (bBullet == true)
					{
						if (strcmp(&aFile[0], "BUL") == 0) //弾丸（親のインデックス、場所、大きさ、時間）
						{
							fscanf(pFile, "%s", &aFile[0]);
							fscanf(pFile, "%d %f %f %f %d %d %d", &g_enemymodel[nNumEnemy].aMotionInfo[nCntMotion].Bang[nCntBullet].nIdxParent,
								&g_enemymodel[nNumEnemy].aMotionInfo[nCntMotion].Bang[nCntBullet].pos.x, &g_enemymodel[nNumEnemy].aMotionInfo[nCntMotion].Bang[nCntBullet].pos.y,
								&g_enemymodel[nNumEnemy].aMotionInfo[nCntMotion].Bang[nCntBullet].pos.z, &g_enemymodel[nNumEnemy].aMotionInfo[nCntMotion].Bang[nCntBullet].nStartBul,
								&g_enemymodel[nNumEnemy].aMotionInfo[nCntMotion].Bang[nCntBullet].nFinishBul, &g_enemymodel[nNumEnemy].aMotionInfo[nCntMotion].Bang[nCntBullet].nLag);
						}
						if (strcmp(&aFile[0], "DMG") == 0) //弾丸（水平吹っ飛ばし力、垂直吹っ飛ばし力、ダメージ量、ヒット間隔）
						{
							fscanf(pFile, "%s", &aFile[0]);
							fscanf(pFile, "%f %f %d %d", &g_enemymodel[nNumEnemy].aMotionInfo[nCntMotion].Bang[nCntBullet].fHClash, &g_enemymodel[nNumEnemy].aMotionInfo[nCntMotion].Bang[nCntBullet].fVClash,
								&g_enemymodel[nNumEnemy].aMotionInfo[nCntMotion].Bang[nCntBullet].nDamage, &g_enemymodel[nNumEnemy].aMotionInfo[nCntMotion].Bang[nCntBullet].nDamageTime);
						}
					}
				}
				if (bKeySet == true) //KEYSET時
				{
					if (strcmp(&aFile[0], "FRAME") == 0) //フレーム数
					{
						fscanf(pFile, "%s", &aFile[0]);
						fscanf(pFile, "%d", &g_enemymodel[nNumEnemy].aMotionInfo[nCntMotion].aKeyInfo[nCntKey].nFrame);
					}
					if (strcmp(&aFile[0], "MOVE") == 0) //移動量
					{
						fscanf(pFile, "%s", &aFile[0]);
						fscanf(pFile, "%f", &g_enemymodel[nNumEnemy].aMotionInfo[nCntMotion].aKeyInfo[nCntKey].fMove);
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
							fscanf(pFile, "%f %f %f", &g_enemymodel[nNumEnemy].aMotionInfo[nCntMotion].aKeyInfo[nCntKey].aKey[nCntParts].fPosX, &g_enemymodel[nNumEnemy].aMotionInfo[nCntMotion].aKeyInfo[nCntKey].aKey[nCntParts].fPosY, &g_enemymodel[nNumEnemy].aMotionInfo[nCntMotion].aKeyInfo[nCntKey].aKey[nCntParts].fPosZ);
						}
						if (strcmp(&aFile[0], "ROT") == 0) //角度
						{
							fscanf(pFile, "%s", &aFile[0]);
							fscanf(pFile, "%f %f %f", &g_enemymodel[nNumEnemy].aMotionInfo[nCntMotion].aKeyInfo[nCntKey].aKey[nCntParts].fRotX, &g_enemymodel[nNumEnemy].aMotionInfo[nCntMotion].aKeyInfo[nCntKey].aKey[nCntParts].fRotY, &g_enemymodel[nNumEnemy].aMotionInfo[nCntMotion].aKeyInfo[nCntKey].aKey[nCntParts].fRotZ);
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
	//「MOTIONTYPE_RETURN」は、ニュートラルモーションに戻る際に経由するモーション
	g_enemymodel[nNumEnemy].aMotionInfo[ENEMYMOTION_RETURN].bLoop = false;
	g_enemymodel[nNumEnemy].aMotionInfo[ENEMYMOTION_RETURN].aKeyInfo[0].nFrame = 14;
	g_enemymodel[nNumEnemy].aMotionInfo[ENEMYMOTION_RETURN].nNumKey = 1;
	g_enemymodel[nNumEnemy].KeyMove.fPosX = 0.0f;
	g_enemymodel[nNumEnemy].KeyMove.fPosY = 0.0f;
	g_enemymodel[nNumEnemy].KeyMove.fPosZ = 0.0f;
	g_enemymodel[nNumEnemy].nDamageTime = 0;
	for (nCntParts = 0; nCntParts < g_enemymodel[nNumEnemy].nNumModel; nCntParts++)
	{
		g_enemymodel[nNumEnemy].aMotionInfo[ENEMYMOTION_RETURN].aKeyInfo[0].aKey[nCntParts].fPosX = g_enemymodel[nNumEnemy].aMotionInfo[ENEMYMOTION_NEUTRAL].aKeyInfo[1].aKey[nCntParts].fPosX;
		g_enemymodel[nNumEnemy].aMotionInfo[ENEMYMOTION_RETURN].aKeyInfo[0].aKey[nCntParts].fPosY = g_enemymodel[nNumEnemy].aMotionInfo[ENEMYMOTION_NEUTRAL].aKeyInfo[1].aKey[nCntParts].fPosY;
		g_enemymodel[nNumEnemy].aMotionInfo[ENEMYMOTION_RETURN].aKeyInfo[0].aKey[nCntParts].fPosZ = g_enemymodel[nNumEnemy].aMotionInfo[ENEMYMOTION_NEUTRAL].aKeyInfo[1].aKey[nCntParts].fPosZ;
		g_enemymodel[nNumEnemy].aMotionInfo[ENEMYMOTION_RETURN].aKeyInfo[0].aKey[nCntParts].fRotX = g_enemymodel[nNumEnemy].aMotionInfo[ENEMYMOTION_NEUTRAL].aKeyInfo[1].aKey[nCntParts].fRotX;
		g_enemymodel[nNumEnemy].aMotionInfo[ENEMYMOTION_RETURN].aKeyInfo[0].aKey[nCntParts].fRotY = g_enemymodel[nNumEnemy].aMotionInfo[ENEMYMOTION_NEUTRAL].aKeyInfo[1].aKey[nCntParts].fRotY;
		g_enemymodel[nNumEnemy].aMotionInfo[ENEMYMOTION_RETURN].aKeyInfo[0].aKey[nCntParts].fRotZ = g_enemymodel[nNumEnemy].aMotionInfo[ENEMYMOTION_NEUTRAL].aKeyInfo[1].aKey[nCntParts].fRotZ;
		//マテリアル情報に対するポインタを取得
		pMat = (D3DXMATERIAL*)g_enemymodel[nNumEnemy].aModel[nCntParts].pBuffMat->GetBufferPointer();
		int nCntTex = 0;
		for (int nCntMat = 0; nCntMat < (int)g_enemymodel[nNumEnemy].aModel[nCntParts].nNumMat; nCntMat++)
		{
			if (pMat[nCntMat].pTextureFilename != NULL)
			{
				D3DXCreateTextureFromFile(pDevice,
					pMat[nCntMat].pTextureFilename,
					&g_enemymodel[nNumEnemy].aModel[nCntParts].TextureModel[nCntTex]);
				nCntTex++;
			}
		}
	}
	g_enemymodel[nNumEnemy].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_enemymodel[nNumEnemy].posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_enemymodel[nNumEnemy].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_enemymodel[nNumEnemy].rotDesh = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_enemymodel[nNumEnemy].bMove = false;
	g_enemymodel[nNumEnemy].bLand = true;
	g_enemymodel[nNumEnemy].nTotalTime = 0;
	g_enemymodel[nNumEnemy].nThinkTime = 120;
	g_enemymodel[nNumEnemy].posDesh = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_enemymodel[nNumEnemy].bUse = true;
	return S_OK;
}

//終了処理
void UninitEnemy(void)
{
	int nCntModel;
	for (nCntModel = 0; nCntModel < g_nNumEnemyModel; nCntModel++)
	{
		//テクスチャの破棄
		if (g_Emodel[nCntModel].pMesh != NULL)
		{
			g_Emodel[nCntModel].pMesh->Release();
			g_Emodel[nCntModel].pMesh = NULL;
		}
		//頂点バッファの破棄
		if (g_Emodel[nCntModel].pBuffMat != NULL)
		{
			g_Emodel[nCntModel].pBuffMat->Release();
			g_Emodel[nCntModel].pBuffMat = NULL;
		}
	}
	for (nCntModel = 0; nCntModel < MAX_ENEMY; nCntModel++)
	{
		if (g_enemy[nCntModel].bUse == true)
		{
			g_enemy[nCntModel].bUse = false;
		}
	}
}

//更新処理
void UpdateEnemy(void)
{
	int nCntEnemy;
	bool bLand;
	for (nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		//使っている
		if (g_enemy[nCntEnemy].bUse == true)
		{
			g_enemy[nCntEnemy].posOld = g_enemy[nCntEnemy].pos;
			bLand = g_enemy[nCntEnemy].bLand;
			
			//考える
			EnemyAI(nCntEnemy);

			//方向狂わないようにする
			if (g_enemy[nCntEnemy].rot.y >= D3DX_PI)
			{
				g_enemy[nCntEnemy].rot.y -= D3DX_PI * 2;
			}
			if (g_enemy[nCntEnemy].rot.y < -D3DX_PI)
			{
				g_enemy[nCntEnemy].rot.y += D3DX_PI * 2;
			}
			if (g_enemy[nCntEnemy].rotDesh.y >= D3DX_PI)
			{
				g_enemy[nCntEnemy].rotDesh.y -= D3DX_PI * 2;
			}
			if (g_enemy[nCntEnemy].rotDesh.y < -D3DX_PI)
			{
				g_enemy[nCntEnemy].rotDesh.y += D3DX_PI * 2;
			}

			//目的の向きを向いてない場合、足していく
			if (g_enemy[nCntEnemy].rot.y != g_enemy[nCntEnemy].rotDesh.y)
			{
				if (g_enemy[nCntEnemy].rotDesh.y - g_enemy[nCntEnemy].rot.y < -3.14f)
				{
					g_enemy[nCntEnemy].rot.y += (g_enemy[nCntEnemy].rotDesh.y - g_enemy[nCntEnemy].rot.y + 6.28f) * 0.14f;
				}
				else if (g_enemy[nCntEnemy].rotDesh.y - g_enemy[nCntEnemy].rot.y > 3.14f)
				{
					g_enemy[nCntEnemy].rot.y += (g_enemy[nCntEnemy].rotDesh.y - g_enemy[nCntEnemy].rot.y - 6.28f) * 0.14f;
				}
				else
				{
					g_enemy[nCntEnemy].rot.y += (g_enemy[nCntEnemy].rotDesh.y - g_enemy[nCntEnemy].rot.y) * 0.14f;
				}
			}

			//移動中じゃない
			if (g_enemy[nCntEnemy].bMove == false)
			{
				//遅くしていく
				EnemySlowX(nCntEnemy);
				EnemySlowZ(nCntEnemy);
			}
			EnemyMotion(nCntEnemy); //敵のモーション

			//移動
			g_enemy[nCntEnemy].pos.x += g_enemy[nCntEnemy].move.x;
			g_enemy[nCntEnemy].pos.y += g_enemy[nCntEnemy].move.y;
			g_enemy[nCntEnemy].pos.z += g_enemy[nCntEnemy].move.z;

			//特定の種類の場合、重力強くする
			if (g_enemy[nCntEnemy].nType == 0)
			{
				g_enemy[nCntEnemy].move.y -= 0.4f;
			}
			//もう一種類の場合、ふよふよさせる
			else if(g_enemy[nCntEnemy].pos.y <= 120.0f)
			{
				g_enemy[nCntEnemy].move.y = 0.2f;
			}
			else if (g_enemy[nCntEnemy].pos.y >= 130.0f)
			{
				g_enemy[nCntEnemy].move.y = -0.2f;
			}

			//カウント0より大きい場合、減らしてく
			if (g_enemy[nCntEnemy].nDamageTime > 0)
			{
				g_enemy[nCntEnemy].nDamageTime--;
			}
			//当たり判定当たってる場合、ダメージ受ける
			else if(CollisionDamage(g_enemy[nCntEnemy].pos, g_enemy[nCntEnemy].fHeight) == true)
			{
				EnemyDamage(nCntEnemy);
			}

			//ロックオンされてる敵の場合
			if (nCntEnemy == RockNumber())
			{
				//ロックオンする
				MoveRockon(D3DXVECTOR3(g_enemy[nCntEnemy].pos.x, g_enemy[nCntEnemy].pos.y + g_enemy[nCntEnemy].fHeight / 2, g_enemy[nCntEnemy].pos.z));
			}

			//影移動
			MoveShadow(g_enemy[nCntEnemy].nShadow, g_enemy[nCntEnemy].pos);
			
			//壁との当たり判定
			CollisionWall(&g_enemy[nCntEnemy].pos, g_enemy[nCntEnemy].fRadius);
			
			//敵の種類によっては床との当たり判定をし、着地させる
			if (g_enemy[nCntEnemy].nType == 0)
			{
				g_enemy[nCntEnemy].bLand = CollisionField(&g_enemy[nCntEnemy].pos, &g_enemy[nCntEnemy].posOld, &g_enemy[nCntEnemy].move, g_enemy[nCntEnemy].fRadius);
			}

			//前フレームで着地してなくて、今フレームで着地する
			if (bLand == false && g_enemy[nCntEnemy].bLand == true)
			{
				EnemyMotionChange(nCntEnemy, ENEMYMOTION_LANDING);
			}
		}
	}
}

//描画処理
void DrawEnemy(void)
{
	LPDIRECT3DDEVICE9 pDevice;   //デバイスのポインタ
	D3DXMATRIX mtxRot, mtxTrans; //計算用マトリックス
	D3DMATERIAL9 matDef;		 //現在のマテリアル保存用
	D3DXMATERIAL *pMat;			 //マテリアルデータへのポインタ
	int nCntModel;
	pDevice = GetDevice();		 //デバイスを取得する
	int nCntEnemy;
	for (nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		if (g_enemy[nCntEnemy].bUse == true && CameraDistance() < 3000.0f)
		{
			//敵のワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_enemy[nCntEnemy].mtxWorld);

			//敵の向きを反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_enemy[nCntEnemy].rot.y, g_enemy[nCntEnemy].rot.x, g_enemy[nCntEnemy].rot.z);
			D3DXMatrixMultiply(&g_enemy[nCntEnemy].mtxWorld, &g_enemy[nCntEnemy].mtxWorld, &mtxRot);

			//敵の位置を反映
			D3DXMatrixTranslation(&mtxTrans, g_enemy[nCntEnemy].pos.x, g_enemy[nCntEnemy].pos.y, g_enemy[nCntEnemy].pos.z);
			D3DXMatrixMultiply(&g_enemy[nCntEnemy].mtxWorld, &g_enemy[nCntEnemy].mtxWorld, &mtxTrans);

			//敵のワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_enemy[nCntEnemy].mtxWorld);

			for (nCntModel = 0; nCntModel < g_enemy[nCntEnemy].nNumModel; nCntModel++)
			{
				D3DXMATRIX mtxRotParts, mtxTransParts; //パーツ用計算用マトリックス
				D3DXMATRIX mtxParent; //親のマトリックス

				//パーツのワールドマトリックスの初期化
				D3DXMatrixIdentity(&g_enemy[nCntEnemy].aModel[nCntModel].mtxWorld);
				
				//パーツの向きを反映
				D3DXMatrixRotationYawPitchRoll(&mtxRotParts, g_enemy[nCntEnemy].aModel[nCntModel].rot.y + g_enemy[nCntEnemy].aModel[nCntModel].rotOri.y,
					g_enemy[nCntEnemy].aModel[nCntModel].rot.x + g_enemy[nCntEnemy].aModel[nCntModel].rotOri.x, g_enemy[nCntEnemy].aModel[nCntModel].rot.z + g_enemy[nCntEnemy].aModel[nCntModel].rotOri.z);
				D3DXMatrixMultiply(&g_enemy[nCntEnemy].aModel[nCntModel].mtxWorld, &g_enemy[nCntEnemy].aModel[nCntModel].mtxWorld, &mtxRotParts);
				
				//パーツの位置を反映
				D3DXMatrixTranslation(&mtxTransParts, g_enemy[nCntEnemy].aModel[nCntModel].pos.x + g_enemy[nCntEnemy].aModel[nCntModel].posOri.x, g_enemy[nCntEnemy].aModel[nCntModel].pos.y + g_enemy[nCntEnemy].aModel[nCntModel].posOri.y, g_enemy[nCntEnemy].aModel[nCntModel].pos.z + g_enemy[nCntEnemy].aModel[nCntModel].posOri.z);
				D3DXMatrixMultiply(&g_enemy[nCntEnemy].aModel[nCntModel].mtxWorld, &g_enemy[nCntEnemy].aModel[nCntModel].mtxWorld, &mtxTransParts);
				
				//各パーツの親のマトリックスを設定
				if (g_enemy[nCntEnemy].aModel[nCntModel].nIdxModelParent != -1)
				{
					mtxParent = g_enemy[nCntEnemy].aModel[g_enemy[nCntEnemy].aModel[nCntModel].nIdxModelParent].mtxWorld;
				}
				else
				{
					mtxParent = g_enemy[nCntEnemy].mtxWorld; //敵のマトリックスを設定
				}
				
				//算出した各パーツのワールドマトリックスと親のマトリックスを掛け合わせる
				D3DXMatrixMultiply(&g_enemy[nCntEnemy].aModel[nCntModel].mtxWorld,
					&g_enemy[nCntEnemy].aModel[nCntModel].mtxWorld,
					&mtxParent);
				
				//各パーツのワールドマトリックスの設定
				pDevice->SetTransform(D3DTS_WORLD, &g_enemy[nCntEnemy].aModel[nCntModel].mtxWorld);
				
				//パーツの現在のマテリアルを取得
				pDevice->GetMaterial(&matDef);
				
				//パーツのマテリアルデータへのポインタの反映
				pMat = (D3DXMATERIAL*)g_enemy[nCntEnemy].aModel[nCntModel].pBuffMat->GetBufferPointer();
				
				int nCntMat;
				for (nCntMat = 0; nCntMat < (int)g_enemy[nCntEnemy].aModel[nCntModel].nNumMat; nCntMat++)
				{
					int nCntTex = 0;
					
					//敵のマテリアルの設定
					pDevice->SetMaterial(&pMat[nCntMat].MatD3D);
					
					for (int nCntMat = 0; nCntMat < (int)g_enemy[nCntEnemy].aModel[nCntModel].nNumMat; nCntMat++)
					{
						if (pMat[nCntMat].pTextureFilename != NULL)
						{
							//テクスチャの設定
							pDevice->SetTexture(0, g_enemy[nCntEnemy].aModel[nCntModel].TextureModel[nCntTex]);
							nCntTex++;
						}
						else
						{
							//テクスチャの設定
							pDevice->SetTexture(0, NULL);
						}
					}
					//敵のモデル（パーツ）の描画
					g_enemy[nCntEnemy].aModel[nCntModel].pMesh->DrawSubset(nCntMat);
				}
			}
			if (g_enemy[nCntEnemy].nType == 1)
			{
				int nCntBullet;
				for (nCntBullet = 0; nCntBullet < g_enemy[nCntEnemy].motionInfo.nNumBul; nCntBullet++)
				{
					if (g_enemy[nCntEnemy].motionInfo.Bang[nCntBullet].nStartBul < g_enemy[nCntEnemy].nTotalTime &&
						g_enemy[nCntEnemy].nTotalTime <= g_enemy[nCntEnemy].motionInfo.Bang[nCntBullet].nFinishBul)
					{
						
						g_enemy[nCntEnemy].motionInfo.Bang[nCntBullet].nBullet--;
						
						if (g_enemy[nCntEnemy].motionInfo.Bang[nCntBullet].nBullet <= 0)
						{
							D3DXMATRIX mtxTrans; //パーツ用計算用マトリックス
							D3DXMATRIX mtxParent; //親のマトリックス
							
												  //当たり判定のワールドマトリックスの初期化
							D3DXMatrixIdentity(&g_enemy[nCntEnemy].motionInfo.Bang[nCntBullet].mtxWorld);
							
							//当たり判定の位置を反映
							D3DXMatrixTranslation(&mtxTrans, g_enemy[nCntEnemy].motionInfo.Bang[nCntBullet].pos.x, g_enemy[nCntEnemy].motionInfo.Bang[nCntBullet].pos.y, g_enemy[nCntEnemy].motionInfo.Bang[nCntBullet].pos.z);
							D3DXMatrixMultiply(&g_enemy[nCntEnemy].motionInfo.Bang[nCntBullet].mtxWorld, &g_enemy[nCntEnemy].motionInfo.Bang[nCntBullet].mtxWorld, &mtxTrans);
							
							//親のマトリックス取得
							mtxParent = g_enemy[nCntEnemy].aModel[g_enemy[nCntEnemy].motionInfo.Bang[nCntBullet].nIdxParent].mtxWorld;
							
							//算出した各当たり判定のワールドマトリックスと親のマトリックスを掛け合わせる
							D3DXMatrixMultiply(&g_enemy[nCntEnemy].motionInfo.Bang[nCntBullet].mtxWorld,
								&g_enemy[nCntEnemy].motionInfo.Bang[nCntBullet].mtxWorld,
								&mtxParent);
							
							//弾発射
							SetBullet(D3DXVECTOR3(g_enemy[nCntEnemy].motionInfo.Bang[nCntBullet].mtxWorld._41, g_enemy[nCntEnemy].motionInfo.Bang[nCntBullet].mtxWorld._42, g_enemy[nCntEnemy].motionInfo.Bang[nCntBullet].mtxWorld._43),
								D3DXVECTOR3(sinf(g_enemy[nCntEnemy].rot.y) * -10.0f, sinf(g_enemy[nCntEnemy].aModel[g_enemy[nCntEnemy].motionInfo.Bang[nCntBullet].nIdxParent].rot.x) * 10.0f, cosf(g_enemy[nCntEnemy].rot.y) * -10.0f),
								g_enemy[nCntEnemy].motionInfo.Bang[nCntBullet].nDamage, g_enemy[nCntEnemy].motionInfo.Bang[nCntBullet].nDamageTime, g_enemy[nCntEnemy].motionInfo.Bang[nCntBullet].fVClash,
								g_enemy[nCntEnemy].motionInfo.Bang[nCntBullet].fHClash, g_enemy[nCntEnemy].rot.y);
							g_enemy[nCntEnemy].motionInfo.Bang[nCntBullet].nBullet = g_enemy[nCntEnemy].motionInfo.Bang[nCntBullet].nLag;
						}
					}
				}
			}
			//当たり判定設定
			CollisionPos(nCntEnemy);
		}
	}
	//保存していたマテリアルを戻す
	pDevice->SetMaterial(&matDef);
}

//移動速度横
void EnemyMoveX(int nCntEnemy)
{
	g_enemy[nCntEnemy].move.x -= (sinf(g_enemy[nCntEnemy].rotDesh.y) * g_enemy[nCntEnemy].fMove + g_enemy[nCntEnemy].move.x) * 0.1f;
}

//移動速度奥
void EnemyMoveZ(int nCntEnemy)
{
	g_enemy[nCntEnemy].move.z -= (cosf(g_enemy[nCntEnemy].rotDesh.y) * g_enemy[nCntEnemy].fMove + g_enemy[nCntEnemy].move.z) * 0.1f;
}

//移動速度低下横
void EnemySlowX(int nCntEnemy)
{
	g_enemy[nCntEnemy].move.x -= g_enemy[nCntEnemy].move.x * 0.1f;
}

//移動速度低下縦
void EnemySlowZ(int nCntEnemy)
{
	g_enemy[nCntEnemy].move.z -= g_enemy[nCntEnemy].move.z * 0.1f;
}

//モーションを切り替える関数
void EnemyMotionChange(int nCntEnemy, ENEMYMOTION enemymotion)
{
	int nCntBullet;
	g_enemy[nCntEnemy].enemymotion = enemymotion;
	g_enemy[nCntEnemy].nNumKey = g_enemy[nCntEnemy].aMotionInfo[enemymotion].nNumKey;
	g_enemy[nCntEnemy].nCounterMotion = 0;
	g_enemy[nCntEnemy].bLoopMotion = g_enemy[nCntEnemy].aMotionInfo[enemymotion].bLoop;
	g_enemy[nCntEnemy].KeyMove.fPosX = 0.0f;
	g_enemy[nCntEnemy].KeyMove.fPosY = 0.0f;
	g_enemy[nCntEnemy].KeyMove.fPosZ = 0.0f;
	g_enemy[nCntEnemy].nTotalTime = 0;
	g_enemy[nCntEnemy].motionInfo = g_enemy[nCntEnemy].aMotionInfo[enemymotion];
	g_enemy[nCntEnemy].nKey = 0;
	for (nCntBullet = 0; nCntBullet < g_enemy[nCntEnemy].aMotionInfo[enemymotion].nNumBul; nCntBullet++)
	{
		g_enemy[nCntEnemy].motionInfo.Bang[nCntBullet].nBullet = 0;
	}
}

//モーションの動作
void EnemyMotion(int nCntEnemy)
{
	int nCntParts;
	g_enemy[nCntEnemy].nCounterMotion++;
	g_enemy[nCntEnemy].nTotalTime++;
	for (nCntParts = 0; nCntParts < g_enemy[nCntEnemy].nNumModel; nCntParts++)
	{
		if (g_enemy[nCntEnemy].nCounterMotion == 1) //差分を求める(対応するキーの各パーツのpos(rot) - 今の各パーツのpos(rot))
		{
			if (nCntParts != g_enemy[nCntEnemy].nMoveParts)
			{
				g_enemy[nCntEnemy].aKeyDiff[nCntParts].fPosX = g_enemy[nCntEnemy].aMotionInfo[g_enemy[nCntEnemy].enemymotion].aKeyInfo[g_enemy[nCntEnemy].nKey].aKey[nCntParts].fPosX - g_enemy[nCntEnemy].aModel[nCntParts].pos.x;
				g_enemy[nCntEnemy].aKeyDiff[nCntParts].fPosY = g_enemy[nCntEnemy].aMotionInfo[g_enemy[nCntEnemy].enemymotion].aKeyInfo[g_enemy[nCntEnemy].nKey].aKey[nCntParts].fPosY - g_enemy[nCntEnemy].aModel[nCntParts].pos.y;
				g_enemy[nCntEnemy].aKeyDiff[nCntParts].fPosZ = g_enemy[nCntEnemy].aMotionInfo[g_enemy[nCntEnemy].enemymotion].aKeyInfo[g_enemy[nCntEnemy].nKey].aKey[nCntParts].fPosZ - g_enemy[nCntEnemy].aModel[nCntParts].pos.z;
			}
			else
			{
				g_enemy[nCntEnemy].KeyMove.fPosX = sinf(D3DX_PI + g_enemy[nCntEnemy].rot.y) * g_enemy[nCntEnemy].aMotionInfo[g_enemy[nCntEnemy].enemymotion].aKeyInfo[g_enemy[nCntEnemy].nKey].fMove;
				g_enemy[nCntEnemy].KeyMove.fPosZ = cosf(D3DX_PI + g_enemy[nCntEnemy].rot.y) * g_enemy[nCntEnemy].aMotionInfo[g_enemy[nCntEnemy].enemymotion].aKeyInfo[g_enemy[nCntEnemy].nKey].fMove;
			}
			//方向は狂わないようにする
			g_enemy[nCntEnemy].aKeyDiff[nCntParts].fRotX = g_enemy[nCntEnemy].aMotionInfo[g_enemy[nCntEnemy].enemymotion].aKeyInfo[g_enemy[nCntEnemy].nKey].aKey[nCntParts].fRotX - g_enemy[nCntEnemy].aModel[nCntParts].rot.x;
			if (g_enemy[nCntEnemy].aKeyDiff[nCntParts].fRotX < -3.14f)
			{
				g_enemy[nCntEnemy].aKeyDiff[nCntParts].fRotX += 6.28f;
			}
			else if (g_enemy[nCntEnemy].aKeyDiff[nCntParts].fRotX > 3.14f)
			{
				g_enemy[nCntEnemy].aKeyDiff[nCntParts].fRotX -= 6.28f;
			}
			
			g_enemy[nCntEnemy].aKeyDiff[nCntParts].fRotY = g_enemy[nCntEnemy].aMotionInfo[g_enemy[nCntEnemy].enemymotion].aKeyInfo[g_enemy[nCntEnemy].nKey].aKey[nCntParts].fRotY - g_enemy[nCntEnemy].aModel[nCntParts].rot.y;
			if (g_enemy[nCntEnemy].aKeyDiff[nCntParts].fRotY < -3.14f)
			{
				g_enemy[nCntEnemy].aKeyDiff[nCntParts].fRotY += 6.28f;
			}
			else if (g_enemy[nCntEnemy].aKeyDiff[nCntParts].fRotY > 3.14f)
			{
				g_enemy[nCntEnemy].aKeyDiff[nCntParts].fRotY -= 6.28f;
			}
			
			g_enemy[nCntEnemy].aKeyDiff[nCntParts].fRotZ = g_enemy[nCntEnemy].aMotionInfo[g_enemy[nCntEnemy].enemymotion].aKeyInfo[g_enemy[nCntEnemy].nKey].aKey[nCntParts].fRotZ - g_enemy[nCntEnemy].aModel[nCntParts].rot.z;
			if (g_enemy[nCntEnemy].aKeyDiff[nCntParts].fRotZ < -3.14f)
			{
				g_enemy[nCntEnemy].aKeyDiff[nCntParts].fRotZ += 6.28f;
			}
			else if (g_enemy[nCntEnemy].aKeyDiff[nCntParts].fRotZ > 3.14f)
			{
				g_enemy[nCntEnemy].aKeyDiff[nCntParts].fRotZ -= 6.28f;
			}
		}
		if (nCntParts != g_enemy[nCntEnemy].nMoveParts)
		{
			g_enemy[nCntEnemy].aModel[nCntParts].pos.x += g_enemy[nCntEnemy].aKeyDiff[nCntParts].fPosX / g_enemy[nCntEnemy].aMotionInfo[g_enemy[nCntEnemy].enemymotion].aKeyInfo[g_enemy[nCntEnemy].nKey].nFrame;
			g_enemy[nCntEnemy].aModel[nCntParts].pos.y += g_enemy[nCntEnemy].aKeyDiff[nCntParts].fPosY / g_enemy[nCntEnemy].aMotionInfo[g_enemy[nCntEnemy].enemymotion].aKeyInfo[g_enemy[nCntEnemy].nKey].nFrame;
			g_enemy[nCntEnemy].aModel[nCntParts].pos.z += g_enemy[nCntEnemy].aKeyDiff[nCntParts].fPosZ / g_enemy[nCntEnemy].aMotionInfo[g_enemy[nCntEnemy].enemymotion].aKeyInfo[g_enemy[nCntEnemy].nKey].nFrame;
		}
		else
		{
			g_enemy[nCntEnemy].pos.x += g_enemy[nCntEnemy].KeyMove.fPosX / g_enemy[nCntEnemy].aMotionInfo[g_enemy[nCntEnemy].enemymotion].aKeyInfo[g_enemy[nCntEnemy].nKey].nFrame;
			g_enemy[nCntEnemy].pos.z += g_enemy[nCntEnemy].KeyMove.fPosZ / g_enemy[nCntEnemy].aMotionInfo[g_enemy[nCntEnemy].enemymotion].aKeyInfo[g_enemy[nCntEnemy].nKey].nFrame;
		}
		g_enemy[nCntEnemy].aModel[nCntParts].rot.x += g_enemy[nCntEnemy].aKeyDiff[nCntParts].fRotX / g_enemy[nCntEnemy].aMotionInfo[g_enemy[nCntEnemy].enemymotion].aKeyInfo[g_enemy[nCntEnemy].nKey].nFrame;
		g_enemy[nCntEnemy].aModel[nCntParts].rot.y += g_enemy[nCntEnemy].aKeyDiff[nCntParts].fRotY / g_enemy[nCntEnemy].aMotionInfo[g_enemy[nCntEnemy].enemymotion].aKeyInfo[g_enemy[nCntEnemy].nKey].nFrame;
		g_enemy[nCntEnemy].aModel[nCntParts].rot.z += g_enemy[nCntEnemy].aKeyDiff[nCntParts].fRotZ / g_enemy[nCntEnemy].aMotionInfo[g_enemy[nCntEnemy].enemymotion].aKeyInfo[g_enemy[nCntEnemy].nKey].nFrame;
	}

	//特定の種類の場合、特定のパーツを回転させる
	if (g_enemy[nCntEnemy].nType == 1)
	{
		g_enemy[nCntEnemy].aModel[1].rot.y += 0.6f;
		if (g_enemy[nCntEnemy].aModel[1].rot.y > D3DX_PI)
		{
			g_enemy[nCntEnemy].aModel[1].rot.y -= D3DX_PI * 2;
		}
	}

	//モーションカウントと想定フレーム数が一致
	if (g_enemy[nCntEnemy].nCounterMotion == g_enemy[nCntEnemy].aMotionInfo[g_enemy[nCntEnemy].enemymotion].aKeyInfo[g_enemy[nCntEnemy].nKey].nFrame)
	{
		//カウントリセット、キー数加算
		g_enemy[nCntEnemy].nCounterMotion = 0;
		g_enemy[nCntEnemy].nKey++;

		//キー数と想定キー数が一致
		if (g_enemy[nCntEnemy].nKey == g_enemy[nCntEnemy].nNumKey)
		{
			//ループしない
			if (g_enemy[nCntEnemy].bLoopMotion == false)
			{
				//リターンモーションじゃないならリターン、リターンだったらニュートラルにする
				if (g_enemy[nCntEnemy].enemymotion != ENEMYMOTION_RETURN)
				{
					EnemyMotionChange(nCntEnemy, ENEMYMOTION_RETURN);
				}
				else
				{
					EnemyMotionChange(nCntEnemy, ENEMYMOTION_NEUTRAL);
				}
			}
			//キー数リセット
			g_enemy[nCntEnemy].nKey = 0;
		}
	}
}

//敵同士の当たり判定
void CoillisionEnemy(float fRadius, D3DXVECTOR3 *pPos, float fHeight)
{
	float fLengthX; //誰かとオブジェクトのX方向の長さ
	float fLengthY;
	float fLengthZ; //誰かとオブジェクトのZ方向の長さ
	bool bPlusX; //fLengthXがプラスか
	bool bPlusZ; //fLengthZがプラスか
	float fDistance; //誰かとオブジェクトの距離
	float fshort; //誰かとオブジェクトが当たっている場合の、足りない長さの倍率
	int nCntEnemy;
	for (nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		//使ってる
		if (g_enemy[nCntEnemy].bUse == true)
		{
			//それぞれの方向の距離計算
			fLengthX = powf(pPos->x - g_enemy[nCntEnemy].pos.x, 2);
			//fLengthY = pPos->y + (fHeight / 2) - g_enemy[nCntEnemy].pos.y + g_enemy[nCntEnemy].fHeight;
			fLengthZ = powf(pPos->z - g_enemy[nCntEnemy].pos.z, 2);
			if (pPos->x - g_enemy[nCntEnemy].pos.x < 0.0f)
			{
				bPlusX = false;
			}
			else
			{
				bPlusX = true;
			}
			if (pPos->z - g_enemy[nCntEnemy].pos.z < 0.0f)
			{
				bPlusZ = false;
			}
			else
			{
				bPlusZ = true;
			}

			//距離
			fDistance = sqrtf(fLengthX + fLengthZ);
			if (fDistance <= fRadius + g_enemy[nCntEnemy].fRadius) //オブジェクトに当たったとき
			{
				//Yも当たってる
				if (pPos->y + fHeight > g_enemy[nCntEnemy].pos.y && g_enemy[nCntEnemy].pos.y + g_enemy[nCntEnemy].fHeight > pPos->y)
				{
					fLengthX = sqrtf(fLengthX);
					fLengthZ = sqrtf(fLengthZ);
					fshort = (fRadius + g_enemy[nCntEnemy].fRadius) / fDistance - 1.0f;
					fLengthX *= fshort;
					fLengthZ *= fshort;

					//めり込んだ分だけ押しのける
					if (bPlusX == true)
					{
						pPos->x += fLengthX;
					}
					else
					{
						pPos->x -= fLengthX;
					}
					if (bPlusZ == true)
					{
						pPos->z += fLengthZ;
					}
					else
					{
						pPos->z -= fLengthZ;
					}
				}
			}
		}
	}
}

//敵のダメージ判定
void EnemyDamage(int nCntEnemy)
{
	//色々リセット
	g_enemy[nCntEnemy].move = EnemyClash(g_enemy[nCntEnemy].pos);
	g_enemy[nCntEnemy].rotDesh.y = ClashRot(g_enemy[nCntEnemy].pos);
	g_enemy[nCntEnemy].rot.y = ClashRot(g_enemy[nCntEnemy].pos);
	EnemyMotionChange(nCntEnemy, ENEMYMOTION_DAMAGE);
	g_enemy[nCntEnemy].nDamageTime = DamageTime();
	g_enemy[nCntEnemy].bMove = false;
	g_enemy[nCntEnemy].nThinkTime = 80;
	g_enemy[nCntEnemy].nHp -= Damage();
	PlaySound(SOUND_LABEL_DAMAGE);
	if (g_enemy[nCntEnemy].nHp <= 0)
	{
		SetParticle(g_enemy[nCntEnemy].pos);
		RemoveShadow(g_enemy[nCntEnemy].nShadow);
		PlaySound(SOUND_LABEL_DEATH);
		g_enemy[nCntEnemy].bUse = false;
		if (PlayerRockEnemy() == nCntEnemy)
		{
			RemoveRockon();
		}
	}
}

//敵の場所取得
float EnemyPos(int nRot, int nCntEnemy)
{
	switch (nRot)
	{
	case 0:
		return g_enemy[nCntEnemy].pos.x;
		break;
	case 1:
		return g_enemy[nCntEnemy].pos.y;
		break;
	case 2:
		return g_enemy[nCntEnemy].pos.z;
		break;
	}
	return 0.0f;
}

//ロックオン
int RockOn(D3DXVECTOR3 pos)
{
	int nCntEnemy;
	int nMinEnemy = -1;
	int nMaxEnemy = -1;
	float fRot;
	float fMinRot = 30.0f;
	float fMaxRot = 0.0f;
	for (nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		//使ってる
		if (g_enemy[nCntEnemy].bUse == true)
		{
			//方向計算
			fRot = RockCamera() - atan2f(pos.x - g_enemy[nCntEnemy].pos.x, pos.z - g_enemy[nCntEnemy].pos.z);
			if (fRot > D3DX_PI)
			{
				fRot -= D3DX_PI * 2;
			}
			if (fRot < -D3DX_PI)
			{
				fRot += D3DX_PI * 2;
			}

			//最小、最大の値更新
			if (fabsf(fRot) < fMinRot)
			{
				fMinRot = fabsf(fRot);
				nMinEnemy = nCntEnemy;
			}
			if (fabsf(fRot) > fMaxRot)
			{
				fMaxRot = fabsf(fRot);
				nMaxEnemy = nCntEnemy;
			}
		}
	}

	//角度が小さい(カメラ真正面)
	if (fMinRot < D3DX_PI * (20.0f / 180.0f))
	{
		SetRockon(D3DXVECTOR3(g_enemy[nMinEnemy].pos.x, g_enemy[nMinEnemy].pos.y + g_enemy[nMinEnemy].fHeight / 2, g_enemy[nMinEnemy].pos.z), nMinEnemy);
		return nMinEnemy;
	}
	
	//角度が大きい(カメラ正反対)
	if (fMaxRot > D3DX_PI * (160.0f / 180.0f))
	{
		SetRockon(D3DXVECTOR3(g_enemy[nMaxEnemy].pos.x, g_enemy[nMaxEnemy].pos.y + g_enemy[nMaxEnemy].fHeight / 2, g_enemy[nMaxEnemy].pos.z), nMaxEnemy);
		return nMaxEnemy;
	}
	
	return -1;
}

//ロックオン中の攻撃
float RockAttack(D3DXVECTOR3 pos, int nCntEnemy)
{
	float fRot = 0.0f;
	fRot = atan2f(pos.x - g_enemy[nCntEnemy].pos.x, pos.z - g_enemy[nCntEnemy].pos.z);
	return fRot;
}

//敵設定
void SetEnemy(D3DXVECTOR3 pos, int nType)
{
	int nCntEnemy;
	for (nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		//使ってない
		if (g_enemy[nCntEnemy].bUse == false)
		{
			//色々設定
			g_enemy[nCntEnemy] = g_enemymodel[nType];
			g_enemy[nCntEnemy].pos = pos;
			g_enemy[nCntEnemy].nShadow = SetShadow(pos, g_enemymodel[nType].fRadius);
			g_enemy[nCntEnemy].nType = nType;
			EnemyMotionChange(nCntEnemy, ENEMYMOTION_RETURN);
			break;
		}
	}
}

//戦いが終わってるか
bool EndBattle(void)
{
	int nCntEnemy;
	for (nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		if (g_enemy[nCntEnemy].bUse == true)
		{
			return false;
		}
	}
	return true;
}

//特定の敵との水平方向の距離
float RockHDistance(D3DXVECTOR3 pos, int nEnemy)
{
	float fLengthX;
	float fLengthZ;
	float fDistance;
	fLengthX = powf(pos.x - g_enemy[nEnemy].pos.x, 2);
	fLengthZ = powf(pos.z - g_enemy[nEnemy].pos.z, 2);
	fDistance = sqrtf(fLengthX + fLengthX);
	return fDistance;
}

//特定の敵との垂直方向の距離
float RockVDistance(D3DXVECTOR3 pos, int nEnemy, float fHeight)
{
	float fLengthY;
	fLengthY = pos.y + (fHeight / 2) - g_enemy[nEnemy].pos.y + (g_enemy[nEnemy].fHeight / 2);
	return fLengthY;
}

//敵の高さ
float RockEnemyHeight(int nCntEnemy)
{
	return g_enemy[nCntEnemy].fHeight;
}

//敵が近くにいるか
bool EnemyNear(D3DXVECTOR3 pos, float fHeight)
{
	int nCntEnemy;
	float fLengthX;
	float fLengthY;
	float fLengthZ;
	float fHDistance;
	int nEnemy = 0;
	for (nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		if (g_enemy[nCntEnemy].bUse == true)
		{
			fLengthX = powf(pos.x - g_enemy[nCntEnemy].pos.x, 2);
			fLengthY = fabsf(pos.y + (fHeight / 2) - g_enemy[nCntEnemy].pos.y + (g_enemy[nCntEnemy].fHeight / 2));
			fLengthZ = powf(pos.z - g_enemy[nCntEnemy].pos.z, 2);
			fHDistance = sqrtf(fLengthX + fLengthZ);

			//周辺に敵がいる
			if (fHDistance < 120.0f && fLengthY < 50.0f)
			{
				//カウント加算、2体以上いたらtrue
				nEnemy++;
				if (nEnemy == 2)
				{
					return true;
				}
			}
		}
	}
	return false;
}

//敵のAI
void EnemyAI(int nCntEnemy)
{
	//考えるカウント減らす
	g_enemy[nCntEnemy].nThinkTime--;

	//カウントが0以下になる
	if (g_enemy[nCntEnemy].nThinkTime <= 0)
	{
		//特定の種類かつプレイヤーとの距離が少し長い
		if (g_enemy[nCntEnemy].nType == 0 && PlayerDistance(g_enemy[nCntEnemy].pos) > 200.0f)
		{
			//プレイヤーに向かって走る
			g_enemy[nCntEnemy].posDesh = PlayerPos();
			g_enemy[nCntEnemy].rotDesh.y = atan2f(g_enemy[nCntEnemy].pos.x - g_enemy[nCntEnemy].posDesh.x, g_enemy[nCntEnemy].pos.z - g_enemy[nCntEnemy].posDesh.z);
			g_enemy[nCntEnemy].bMove = true;
			g_enemy[nCntEnemy].nThinkTime = 600;
			EnemyMotionChange(nCntEnemy, ENEMYMOTION_WALK);
		}
		//特定の種類かつ距離がより長い
		else if (g_enemy[nCntEnemy].nType == 1 && PlayerDistance(g_enemy[nCntEnemy].pos) > 600.0f)
		{
			//プレイヤーに向かって走る
			g_enemy[nCntEnemy].posDesh = PlayerPos();
			g_enemy[nCntEnemy].rotDesh.y = atan2f(g_enemy[nCntEnemy].pos.x - g_enemy[nCntEnemy].posDesh.x, g_enemy[nCntEnemy].pos.z - g_enemy[nCntEnemy].posDesh.z);
			g_enemy[nCntEnemy].bMove = true;
			g_enemy[nCntEnemy].nThinkTime = 600;
			EnemyMotionChange(nCntEnemy, ENEMYMOTION_WALK);
		}
		//近い
		else
		{
			//攻撃する敵の数が少ない
			if (EnemyAtkNum() == false)
			{
				//プレイヤーの方向を向いて攻撃
				g_enemy[nCntEnemy].rotDesh.y = atan2f(g_enemy[nCntEnemy].pos.x - PlayerPos().x, g_enemy[nCntEnemy].pos.z - PlayerPos().z);
				g_enemy[nCntEnemy].nThinkTime = 900 + (rand() % 80) * 2;
				if (g_enemy[nCntEnemy].nType == 1)
				{
					g_enemy[nCntEnemy].nThinkTime += 160;
				}
				EnemyMotionChange(nCntEnemy, ENEMYMOTION_ACTION);
			}
			//攻撃する敵の数が多い
			else
			{
				//さらに考える
				g_enemy[nCntEnemy].nThinkTime = 100 + rand() % 100;
			}
		}
	}
	//まだ考え中
	else
	{
		//移動する
		if (g_enemy[nCntEnemy].bMove == true)
		{
			//プレイヤーの方向を向く
			g_enemy[nCntEnemy].rotDesh.y = atan2f(g_enemy[nCntEnemy].pos.x - PlayerPos().x, g_enemy[nCntEnemy].pos.z - PlayerPos().z);
			g_enemy[nCntEnemy].posDesh = PlayerPos();
			EnemyMoveX(nCntEnemy);
			EnemyMoveZ(nCntEnemy);

			//距離が一定未満
			if (PlayerDistance(g_enemy[nCntEnemy].pos) < 150.0f)
			{
				//移動やめさせる
				g_enemy[nCntEnemy].bMove = false;
				EnemyMotionChange(nCntEnemy, ENEMYMOTION_RETURN);
				g_enemy[nCntEnemy].nThinkTime = 700 + (rand() % 40) * 2;
			}
		}
	}
}


//攻撃の当たり判定設定
void CollisionPos(int nCntEnemy)
{
	int nCntCol;
	for (nCntCol = 0; nCntCol < g_enemy[nCntEnemy].motionInfo.nNumCol; nCntCol++)
	{
		//攻撃時間中
		if (g_enemy[nCntEnemy].motionInfo.Collision[nCntCol].nStarttime <= g_enemy[nCntEnemy].nTotalTime &&
			g_enemy[nCntEnemy].nTotalTime < g_enemy[nCntEnemy].motionInfo.Collision[nCntCol].nFinishtime)
		{
			D3DXMATRIX mtxTrans; //パーツ用計算用マトリックス
			D3DXMATRIX mtxParent; //親のマトリックス

			//当たり判定のワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_enemy[nCntEnemy].motionInfo.Collision[nCntCol].mtxWorld);

			//当たり判定の位置を反映
			D3DXMatrixTranslation(&mtxTrans, g_enemy[nCntEnemy].motionInfo.Collision[nCntCol].pos.x, g_enemy[nCntEnemy].motionInfo.Collision[nCntCol].pos.y, g_enemy[nCntEnemy].motionInfo.Collision[nCntCol].pos.z);
			D3DXMatrixMultiply(&g_enemy[nCntEnemy].motionInfo.Collision[nCntCol].mtxWorld, &g_enemy[nCntEnemy].motionInfo.Collision[nCntCol].mtxWorld, &mtxTrans);
			
			//親のマトリックス取得
			mtxParent = g_enemy[nCntEnemy].aModel[g_enemy[nCntEnemy].motionInfo.Collision[nCntCol].nIdxModelParent].mtxWorld;
			
			//算出した各当たり判定のワールドマトリックスと親のマトリックスを掛け合わせる
			D3DXMatrixMultiply(&g_enemy[nCntEnemy].motionInfo.Collision[nCntCol].mtxWorld,
				&g_enemy[nCntEnemy].motionInfo.Collision[nCntCol].mtxWorld,
				&mtxParent);
			
			//プレイヤーとの当たり判定
			if (PlayerDamage(D3DXVECTOR3(g_enemy[nCntEnemy].motionInfo.Collision[nCntCol].mtxWorld._41, g_enemy[nCntEnemy].motionInfo.Collision[nCntCol].mtxWorld._42, g_enemy[nCntEnemy].motionInfo.Collision[nCntCol].mtxWorld._43),
				g_enemy[nCntEnemy].motionInfo.Collision[nCntCol].fRadius) == true)
			{
				//ダメージくらわせる
				float fRoty;
				D3DXVECTOR3 Clash;
				fRoty = PlayerClashRot(g_enemy[nCntEnemy].pos);
				Clash.x = sinf(fRoty) * g_enemy[nCntEnemy].motionInfo.Collision[nCntCol].fHClash;
				Clash.y = g_enemy[nCntEnemy].motionInfo.Collision[nCntCol].fVClash;
				Clash.z = cosf(fRoty) * g_enemy[nCntEnemy].motionInfo.Collision[nCntCol].fHClash;
				PlaySound(SOUND_LABEL_DAMAGE);
				PlayerDamage(g_enemy[nCntEnemy].motionInfo.Collision[nCntCol].nDamage, g_enemy[nCntEnemy].motionInfo.Collision[nCntCol].nDamageTime, fRoty, Clash);
			}

		}
	}
}

//敵の攻撃の当たり判定のマトリックス取得
D3DXVECTOR3 ECollisionPos(void)
{
	return D3DXVECTOR3(g_enemy[0].motionInfo.Collision[1].mtxWorld._41, g_enemy[0].motionInfo.Collision[1].mtxWorld._42, g_enemy[0].motionInfo.Collision[1].mtxWorld._43);
}

//敵が何体一度に攻撃してくるか
bool EnemyAtkNum(void)
{
	int nCntEnemy;
	int nAtkEnemy = 0;
	for (nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		if (g_enemy[nCntEnemy].bUse == true && g_enemy[nCntEnemy].enemymotion == ENEMYMOTION_ACTION)
		{
			nAtkEnemy++;
			if (nAtkEnemy >= 3)
			{
				return true;
			}
		}
	}
	return false;
}

//敵リセット
void ResetEnemy(void)
{
	int nCntEnemy;
	for (nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		g_enemy[nCntEnemy].bUse = false;
	}
}

//敵が何体いるか
int EnemyNum(void)
{
	int nCntEnemy;
	int nEnemy = 0;
	for (nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		if (g_enemy[nCntEnemy].bUse == true)
		{
			nEnemy++;
		}
	}
	return nEnemy;
}