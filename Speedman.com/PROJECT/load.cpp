//---------------------------
//Author:三上航世
//ロード処理(load.cpp)
//---------------------------
#include "load.h"
#include "manager.h"
#include "renderer.h"
#include "player.h"
#include "field.h"
#include "light.h"
#include "billboard.h"
#include "fade.h"
#include "goal.h"
#include "object.h"
#include "trampoline.h"
#include "slack.h"
#include "camerapanel.h"
#include "camera.h"
#include "scene.h"
#include "wall.h"
#include "player.h"
#include "checkpoint.h"
#include "sky.h"

CLoad::CLoad()
{

}

CLoad::~CLoad(void)
{

}

void CLoad::Load(const char *aFileName)
{
	FILE *pFile;
	pFile = fopen(aFileName, "r");
	char aFile[256];
	char aPlayerFile[256];
	int nNumModel;
	int nCntModel = 0;
	bool bField = false;
	bool bWall = false;
	bool bModel = false;
	bool bLight = false;
	bool bCamera = false;
	bool bBillboard = false;
	bool bjump = false;
	bool bCheck = false;
	bool bCameraPanel = false;
	bool bPlayer = false;
	bool bGoal = false;
	D3DXVECTOR3 pos;
	D3DXVECTOR3 rot;
	int nTex;
	int nBlock[2];
	D3DXVECTOR3 size;
	D3DXVECTOR3 move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	int nMoveTime = 0;
	int nType;
	CScene::COLLISION collision;
	int nCollision;
	int nCntLight = 0;
	D3DXCOLOR col;
	D3DXVECTOR3 vec;
	D3DXVECTOR2 Tex;
	D3DXVECTOR3 posV;
	D3DXVECTOR3 posR;
	int nJump = -1;
	int nZipLine = -1;
	int nCheck = -1;
	int nDrop = -1;
	bool bDrop = false;
	bool bSky = false;
	float fScroll;
	if (pFile != NULL)
	{
		while (true)
		{
			fscanf(pFile, "%s", &aFile[0]); //fscanfを繰り返してファイルを読み取っていく
			if (strcmp(&aFile[0], "NUM_MODEL") == 0) //モデル数
			{
				fscanf(pFile, "%s", &aFile[0]);
				fscanf(pFile, "%d", &nNumModel);
			}
			if (strcmp(&aFile[0], "MODEL_FILENAME") == 0) //モデル名
			{
				fscanf(pFile, "%s", &aFile[0]);
				fscanf(pFile, "%s", &aFile[0]);
				CObject::Load(nCntModel, aFile);
				nCntModel++;
			}

			if (strcmp(&aFile[0], "FIELDSET") == 0) //地面
			{
				bField = true;
			}
			if (strcmp(&aFile[0], "END_FIELDSET") == 0) //地面
			{
				size.x = Tex.x * nBlock[0];
				size.z = Tex.y * nBlock[1];
				size.y = -sinf(rot.x) * size.z;
				//size.y = 0.0f;
				CField::Create(size, pos, nTex, Tex);
				bField = false;
			}
			if (strcmp(&aFile[0], "WALLSET") == 0) //壁
			{
				bWall = true;
			}
			if (strcmp(&aFile[0], "END_WALLSET") == 0) //壁
			{
				size.x = Tex.x * nBlock[0];
				size.y = Tex.y * nBlock[1];
				size.z = 0.0f;
				CWall::Create(size, pos, nTex, Tex, rot);
				bWall = false;
			}
			if (strcmp(&aFile[0], "MODELSET") == 0) //オブジェクト
			{
				bModel = true;
			}
			if (strcmp(&aFile[0], "END_MODELSET") == 0) //オブジェクト
			{
				if (nZipLine != -1)
				{
					CSlack::Create(pos, nType, collision, rot, nZipLine);
					nZipLine = -1;
					bModel = false;
				}
				else if (bjump == true)
				{
					CTrampoline::Create(D3DXVECTOR3(22.0f, 0.0f, 22.0f), D3DXVECTOR3(pos.x, pos.y + 8.0f, pos.z));
					bjump = false;
					nJump = -1;
					CObject::Create(pos, nType, collision, rot, nMoveTime, move, false);
					bModel = false;
				}
				else if (bCheck == true)
				{
					CCheckPoint::Create(pos, nType, CScene::COLLISION_PALL, rot);
					bCheck = false;
					bModel = false;
				}
				else
				{
					CObject::Create(pos, nType, collision, rot, nMoveTime, move, bDrop);
					nMoveTime = 0;
					move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
					bDrop = false;
					nDrop = -1;
					bModel = false;
				}
			}
			if (strcmp(&aFile[0], "LIGHTSET") == 0) //ライト
			{
				bLight = true;
			}
			if (strcmp(&aFile[0], "END_LIGHTSET") == 0) //ライト
			{
				CLight::Create(col, vec, nCntLight);
				nCntLight++;
				bLight = false;
			}
			if (strcmp(&aFile[0], "BILLBOARDSET") == 0) //ビルボード
			{
				bBillboard = true;
			}
			if (strcmp(&aFile[0], "END_BILLBOARDSET") == 0) //ビルボード
			{
				CBillboard::Create(size, pos);
				bBillboard = false;
			}
			if (strcmp(&aFile[0], "CAMERASET") == 0) //カメラ
			{
				bCamera = true;
			}
			if (strcmp(&aFile[0], "END_CAMERASET") == 0) //カメラ
			{
				bCamera = false;
			}
			if (strcmp(&aFile[0], "CAMERAPANELSET") == 0) //カメラパネル
			{
				bCameraPanel = true;
			}
			if (strcmp(&aFile[0], "END_CAMERAPANELSET") == 0) //カメラパネル
			{
				CCameraPanel::Create(size, pos, posV, posR);
				bCameraPanel = false;
			}
			if (strcmp(&aFile[0], "PLAYERSET") == 0) //プレイヤー
			{
				bPlayer = true;
			}
			if (strcmp(&aFile[0], "END_PLAYERSET") == 0) //プレイヤー
			{
				CPlayer::Create(pos, rot, aPlayerFile);
				bPlayer = false;
			}
			if (strcmp(&aFile[0], "GOALSET") == 0) //ゴール
			{
				bGoal = true;
			}
			if (strcmp(&aFile[0], "END_GOALSET") == 0) //ゴール
			{
				CGoal::Create(size, pos);
				bGoal = false;
			}
			if (strcmp(&aFile[0], "SKYSET") == 0) //空
			{
				bSky = true;
			}
			if (strcmp(&aFile[0], "END_SKYSET") == 0) //空
			{
				CSky::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0.00002f, CPlane::GetTexture(6));
				bSky = false;
			}
			if (bField == true)
			{
				if (strcmp(&aFile[0], "POS") == 0) //場所
				{
					fscanf(pFile, "%s", &aFile[0]);
					fscanf(pFile, "%f %f %f", &pos.x, &pos.y, &pos.z);
				}
				if (strcmp(&aFile[0], "ROT") == 0) //角度
				{
					fscanf(pFile, "%s", &aFile[0]);
					fscanf(pFile, "%f %f %f", &rot.x, &rot.y, &rot.z);
					rot.x = rot.x / 180.0f * D3DX_PI;
					rot.y = rot.y / 180.0f * D3DX_PI;
					rot.z = rot.z / 180.0f * D3DX_PI;
				}
				if (strcmp(&aFile[0], "BLOCK") == 0) //テクスチャの細かさ
				{
					fscanf(pFile, "%s", &aFile[0]);
					fscanf(pFile, "%f %f", &Tex.x, &Tex.y);
				}
				if (strcmp(&aFile[0], "SIZE") == 0) //大きさ
				{
					fscanf(pFile, "%s", &aFile[0]);
					fscanf(pFile, "%d %d", &nBlock[0], &nBlock[1]);
				}
				if (strcmp(&aFile[0], "TEXTYPE") == 0) //テクスチャ
				{
					fscanf(pFile, "%s", &aFile[0]);
					fscanf(pFile, "%d", &nTex);
				}
			}
			if (bWall == true)
			{
				if (strcmp(&aFile[0], "POS") == 0) //場所
				{
					fscanf(pFile, "%s", &aFile[0]);
					fscanf(pFile, "%f %f %f", &pos.x, &pos.y, &pos.z);
				}
				if (strcmp(&aFile[0], "ROT") == 0) //回転
				{
					fscanf(pFile, "%s", &aFile[0]);
					fscanf(pFile, "%f %f %f", &rot.x, &rot.y, &rot.z);
					rot.x = rot.x / 180.0f * D3DX_PI;
					rot.y = rot.y / 180.0f * D3DX_PI;
					rot.z = rot.z / 180.0f * D3DX_PI;
				}
				if (strcmp(&aFile[0], "BLOCK") == 0) //テクスチャの細かさ
				{
					fscanf(pFile, "%s", &aFile[0]);
					fscanf(pFile, "%f %f", &Tex.x, &Tex.y);
				}
				if (strcmp(&aFile[0], "SIZE") == 0) //大きさ
				{
					fscanf(pFile, "%s", &aFile[0]);
					fscanf(pFile, "%d %d", &nBlock[0], &nBlock[1]);
				}
				if (strcmp(&aFile[0], "TEXTYPE") == 0) //テクスチャ
				{
					fscanf(pFile, "%s", &aFile[0]);
					fscanf(pFile, "%d", &nTex);
				}
			}
			if (bBillboard == true)
			{
				if (strcmp(&aFile[0], "POS") == 0) //場所
				{
					fscanf(pFile, "%s", &aFile[0]);
					fscanf(pFile, "%f %f %f", &pos.x, &pos.y, &pos.z);
				}
				if (strcmp(&aFile[0], "SIZE") == 0) //大きさ
				{
					fscanf(pFile, "%s", &aFile[0]);
					fscanf(pFile, "%f %f", &size.x, &size.y);
					size.z = 0.0f;
				}
				if (strcmp(&aFile[0], "TEXTYPE") == 0) //テクスチャ
				{
					fscanf(pFile, "%s", &aFile[0]);
					fscanf(pFile, "%d", &nTex);
				}
			}
			if (bModel == true)
			{
				if (strcmp(&aFile[0], "JUMP") == 0) //トランポリン用
				{
					fscanf(pFile, "%s", &aFile[0]);
					fscanf(pFile, "%d", &nJump);
					if (nJump == 1)
					{
						bjump = true;
					}
				}
				if (strcmp(&aFile[0], "CHECK") == 0) //チェックポイント用
				{
					fscanf(pFile, "%s", &aFile[0]);
					fscanf(pFile, "%d", &nCheck);
					if (nCheck == 1)
					{
						bCheck = true;
						nCheck = -1;
					}
				}
				if (strcmp(&aFile[0], "ZIP") == 0) //スラックライン用
				{
					fscanf(pFile, "%s", &aFile[0]);
					fscanf(pFile, "%d", &nZipLine);
				}
				if (strcmp(&aFile[0], "MOVETIME") == 0) //移動時間
				{
					fscanf(pFile, "%s", &aFile[0]);
					fscanf(pFile, "%d", &nMoveTime);
				}
				if (strcmp(&aFile[0], "MOVE") == 0) //移動距離
				{
					fscanf(pFile, "%s", &aFile[0]);
					fscanf(pFile, "%f %f %f", &move.x, &move.y, &move.z);
				}
				if (strcmp(&aFile[0], "TYPE") == 0) //モデルの種類
				{
					fscanf(pFile, "%s", &aFile[0]);
					fscanf(pFile, "%d", &nType);
				}
				if (strcmp(&aFile[0], "POS") == 0) //モデルの場所
				{
					fscanf(pFile, "%s", &aFile[0]);
					fscanf(pFile, "%f %f %f", &pos.x, &pos.y, &pos.z);
				}
				if (strcmp(&aFile[0], "ROT") == 0) //モデルの方向
				{
					fscanf(pFile, "%s", &aFile[0]);
					fscanf(pFile, "%f %f %f", &rot.x, &rot.y, &rot.z);
					rot.x = rot.x / 180.0f * D3DX_PI;
					rot.y = rot.y / 180.0f * D3DX_PI;
					rot.z = rot.z / 180.0f * D3DX_PI;
				}
				if (strcmp(&aFile[0], "COL") == 0) //モデルの当たり判定
				{
					fscanf(pFile, "%s", &aFile[0]);
					fscanf(pFile, "%d", &nCollision);
					switch (nCollision)
					{
					case 0:
						collision = CScene::COLLISION_SPHERE;
						break;
					case 1:
						collision = CScene::COLLISION_SQUARE;
						break;
					default:
						break;
					}
				}
				if (strcmp(&aFile[0], "DROP") == 0) //落下するか
				{
					fscanf(pFile, "%s", &aFile[0]);
					fscanf(pFile, "%d", &nDrop);
					if (nDrop == 1)
					{
						bDrop = true;
					}
				}
			}
			if (bLight == true)
			{
				if (strcmp(&aFile[0], "DIRECTION") == 0) //光の方向
				{
					fscanf(pFile, "%s", &aFile[0]);
					fscanf(pFile, "%f %f %f", &vec.x, &vec.y, &vec.z);
				}
				if (strcmp(&aFile[0], "DIFFUSE") == 0) //光の色
				{
					fscanf(pFile, "%s", &aFile[0]);
					fscanf(pFile, "%f %f %f", &col.r, &col.g, &col.b);
					col.a = 1.0f;
				}
			}
			if (bCamera == true)
			{
				if (strcmp(&aFile[0], "POS") == 0) //視点
				{
					fscanf(pFile, "%s", &aFile[0]);
					fscanf(pFile, "%f %f %f", &pos.x, &pos.y, &pos.z);
					CManager::GetRenderer()->GetCamera()->SetPosV(pos);
				}
				if (strcmp(&aFile[0], "REF") == 0) //注視点
				{
					fscanf(pFile, "%s", &aFile[0]);
					fscanf(pFile, "%f %f %f", &pos.x, &pos.y, &pos.z);
					CManager::GetRenderer()->GetCamera()->SetPosR(pos);
				}
			}
			if (bSky == true)
			{
				if (strcmp(&aFile[0], "TEXTYPE") == 0) //テクスチャ
				{
					fscanf(pFile, "%s", &aFile[0]);
					fscanf(pFile, "%d", &nTex);
				}
				if (strcmp(&aFile[0], "MOVE") == 0) //動き
				{
					fscanf(pFile, "%s", &aFile[0]);
					fscanf(pFile, "%f", &fScroll);
				}
			}
			if (bCameraPanel == true)
			{
				if (strcmp(&aFile[0], "POS") == 0) //パネルの設置場所
				{
					fscanf(pFile, "%s", &aFile[0]);
					fscanf(pFile, "%f %f %f", &pos.x, &pos.y, &pos.z);
				}
				if (strcmp(&aFile[0], "SIZE") == 0) //大きさ
				{
					fscanf(pFile, "%s", &aFile[0]);
					fscanf(pFile, "%f %f %f", &size.x, &size.y, &size.z);
				}
				if (strcmp(&aFile[0], "POSV") == 0) //カメラの視点
				{
					fscanf(pFile, "%s", &aFile[0]);
					fscanf(pFile, "%f %f %f", &posV.x, &posV.y, &posV.z);
				}
				if (strcmp(&aFile[0], "POSR") == 0) //カメラの注視点
				{
					fscanf(pFile, "%s", &aFile[0]);
					fscanf(pFile, "%f %f %f", &posR.x, &posR.y, &posR.z);
				}
			}
			if (bPlayer == true)
			{
				if (strcmp(&aFile[0], "MOTION_FILENAME") == 0) //モデルの場所
				{
					fscanf(pFile, "%s", &aFile[0]);
					fscanf(pFile, "%s", &aPlayerFile[0]);
				}
				if (strcmp(&aFile[0], "POS") == 0) //モデルの場所
				{
					fscanf(pFile, "%s", &aFile[0]);
					fscanf(pFile, "%f %f %f", &pos.x, &pos.y, &pos.z);
				}
				if (strcmp(&aFile[0], "ROT") == 0) //モデルの方向
				{
					fscanf(pFile, "%s", &aFile[0]);
					fscanf(pFile, "%f %f %f", &rot.x, &rot.y, &rot.z);
					rot.x = rot.x / 180.0f * D3DX_PI;
					rot.y = rot.y / 180.0f * D3DX_PI;
					rot.z = rot.z / 180.0f * D3DX_PI;
				}
			}
			if (bGoal == true)
			{
				if (strcmp(&aFile[0], "POS") == 0) //ゴールの場所
				{
					fscanf(pFile, "%s", &aFile[0]);
					fscanf(pFile, "%f %f %f", &pos.x, &pos.y, &pos.z);
				}
				if (strcmp(&aFile[0], "SIZE") == 0) //ゴールの大きさ
				{
					fscanf(pFile, "%s", &aFile[0]);
					fscanf(pFile, "%f %f", &size.x, &size.z);
					size.y = 0.0f;
				}
			}
			if (strcmp(&aFile[0], "END_SCRIPT") == 0) //終わり
			{
				break;
			}
		}
	}
}