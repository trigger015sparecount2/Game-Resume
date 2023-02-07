//---------------------------
//Author:三上航世
//カメラ(camera.cpp)
//---------------------------
#include "camera.h"
#include "input.h"
#include "mouse.h"
#include "model.h"
#include "player.h"
#include "enemy.h"
#include "gamepad.h"

//グローバル変数
Camera g_camera; //カメラの情報

//初期化処理
HRESULT InitCamera(void)
{
	g_camera.posV = D3DXVECTOR3(0.0f, 140.0f, -200.0f);
	g_camera.posVDest = D3DXVECTOR3(0.0f, 140.0f, -200.0f);
	g_camera.posR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_camera.posRDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_camera.vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	g_camera.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_camera.rotDesh = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_camera.fLength = 350.0f;
	g_camera.nTime = 0;
	g_camera.bRot = false;
	return S_OK;
}

//終了処理
void UninitCamera(void)
{

}

//更新処理
void UpdateCamera(void)
{
	LPDIRECT3DDEVICE9 pDevice; //デバイスのポインタ
	pDevice = GetDevice();     //デバイスを取得する

	//マウス左押す
	if (GetMousePress(DIM_L) == true)
	{
		//回転
		g_camera.rot.y += MouseX() * 0.01f;
		g_camera.rotDesh.y = g_camera.rot.y;
	}

	//マウスホイール押すかゲームパッドの右スティック押す
	if (GetMouseButton(DIM_W) == true || GetGamePadButton(DIP_RS) == true)
	{
		//カメラ反転
		g_camera.rotDesh.y = PlayerRot() + 3.14f;
	}

	//ゲームパッド繋がってる
	if (GamePad() == true)
	{
		//右スティック左
		if (RightStickX() > 0.0f)
		{
			g_camera.rot.y += 0.02f;
			g_camera.rotDesh.y = g_camera.rot.y;
		}
		//右スティック右
		else if (RightStickX() < 0.0f)
		{
			g_camera.rot.y -= 0.02f;
			g_camera.rotDesh.y = g_camera.rot.y;
		}
	}

	//ロックオンしてる
	if (TargetRockon() == true)
	{
		//ロックオンしてる敵に注視点を置く、方向をロックオンの敵に向ける
		g_camera.posRDest.x = EnemyPos(0, PlayerRockEnemy());
		g_camera.posRDest.y = EnemyPos(1, PlayerRockEnemy()) + RockEnemyHeight(PlayerRockEnemy());
		g_camera.posRDest.z = EnemyPos(2, PlayerRockEnemy());
		g_camera.rotDesh.y = atan2f(PlayerPos().x - EnemyPos(0, PlayerRockEnemy()), PlayerPos().z - EnemyPos(2, PlayerRockEnemy())) + 3.14f;
	}
	//ロックオンしてない
	else
	{
		//注視点をプレイヤーに置く
		g_camera.posRDest = D3DXVECTOR3(PlayerPos().x, PlayerPos().y + PlayerHeight(), PlayerPos().z);
	}

	//目的の注視点と現在の注視点が違ったら、足していく
	if (g_camera.posRDest.x != g_camera.posR.x)
	{
		g_camera.posR.x += (g_camera.posRDest.x - g_camera.posR.x) * 0.2f;
	}
	if (g_camera.posRDest.y != g_camera.posR.y)
	{
		g_camera.posR.y += (g_camera.posRDest.y - g_camera.posR.y) * 0.2f;
	}
	if (g_camera.posRDest.z != g_camera.posR.z)
	{
		g_camera.posR.z += (g_camera.posRDest.z - g_camera.posR.z) * 0.2f;
	}

	//角度が狂わないようにする
	if (g_camera.rot.y >= D3DX_PI)
	{
		g_camera.rot.y -= D3DX_PI * 2;
	}
	if (g_camera.rot.y < -D3DX_PI)
	{
		g_camera.rot.y += D3DX_PI * 2;
	}
	if (g_camera.rotDesh.y >= D3DX_PI)
	{
		g_camera.rotDesh.y -= D3DX_PI * 2;
	}
	if (g_camera.rotDesh.y < -D3DX_PI)
	{
		g_camera.rotDesh.y += D3DX_PI * 2;
	}

	//目的の向きを現在の向きが違ったら、足していく
	if (g_camera.rotDesh.y != g_camera.rot.y)
	{
		if (g_camera.rotDesh.y - g_camera.rot.y < -3.14f)
		{
			g_camera.rot.y += (g_camera.rotDesh.y - g_camera.rot.y + 6.28f) * 0.2f;
		}
		else if (g_camera.rotDesh.y - g_camera.rot.y > 3.14f)
		{
			g_camera.rot.y += (g_camera.rotDesh.y - g_camera.rot.y - 6.28f) * 0.2f;
		}
		else
		{
			g_camera.rot.y += (g_camera.rotDesh.y - g_camera.rot.y) * 0.2f;
		}
	}

	//視点をプレイヤーのposから距離引いた場所にする
	g_camera.posV.x = PlayerPos().x - sinf(g_camera.rot.y) * g_camera.fLength;
	g_camera.posV.y = PlayerPos().y + PlayerHeight() + g_camera.fLength * (3.0f / 10.0f);
	g_camera.posV.z = PlayerPos().z - cosf(g_camera.rot.y) * g_camera.fLength;

	//ビューマトリックスの初期化
	D3DXMatrixIdentity(&g_camera.mtxView);

	//ビューマトリックスの作成
	D3DXMatrixLookAtLH(&g_camera.mtxView, &g_camera.posV, &g_camera.posR, &g_camera.vecU);

	//ビューマトリックスの設定
	pDevice->SetTransform(D3DTS_VIEW, &g_camera.mtxView);

}

//設定処理
void SetCamera(void)
{
	LPDIRECT3DDEVICE9 pDevice; //デバイスのポインタ
	pDevice = GetDevice();     //デバイスを取得する
	//プロジェクションマトリックスの初期化
	D3DXMatrixIdentity(&g_camera.mtxProjection);
	//プロジェクションマトリックス作成
	D3DXMatrixPerspectiveFovLH(&g_camera.mtxProjection, D3DXToRadian(45.0f), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 10.0f, 5000.0f);
	//プロジェクションマトリックスの設定
	pDevice->SetTransform(D3DTS_PROJECTION, &g_camera.mtxProjection);

	//ビューマトリックスの初期化
	D3DXMatrixIdentity(&g_camera.mtxView);
	//ビューマトリックスの作成
	D3DXMatrixLookAtLH(&g_camera.mtxView, &g_camera.posV, &g_camera.posR, &g_camera.vecU);
	//ビューマトリックスの設定
	pDevice->SetTransform(D3DTS_VIEW, &g_camera.mtxView);
}

//カメラの方向
float CameraRot(void)
{
	return g_camera.rot.y;
}

//視点の場所
D3DXVECTOR3 CameraPosV(void)
{
	return g_camera.posV;
}

//注視点の場所
D3DXVECTOR3 CameraPosR(void)
{
	return g_camera.posR;
}

//視点と注視点の距離
float CameraDistance(void)
{
	float fDistance;
	float fHDistance;
	float fLengthX;
	float fLengthY;
	float fLengthZ;
	fLengthX = powf(g_camera.posR.x - g_camera.posV.x, 2);
	fLengthY = powf(g_camera.posR.y - g_camera.posV.y, 2);
	fLengthZ = powf(g_camera.posR.z - g_camera.posV.z, 2);
	fHDistance = fLengthX + fLengthZ;
	fDistance = sqrtf(fLengthY + fHDistance);
	return fDistance;
}

//カメラが向いてる方向から反転
float RockCamera(void)
{
	float fRot;
	fRot = g_camera.rot.y - D3DX_PI;
	if (fRot < -D3DX_PI)
	{
		fRot += D3DX_PI * 2;
	}
	return fRot;
}

//プロジェクションマトリックス
D3DXMATRIX CameraMtxPro(void)
{
	return g_camera.mtxProjection;
}

//ビューマトリックス
D3DXMATRIX CameraMtxView(void)
{
	return g_camera.mtxView;
}

//カメラの方向水平
float CameraRotH(void)
{
	float fRot = 0.0f;
	fRot = atan2f(g_camera.posR.x - g_camera.posV.x, g_camera.posR.z - g_camera.posV.z);
	return fRot;
}

//カメラの方向垂直
float CameraRotV(void)
{
	float fRot = 0.0f;
	float fLengthX;
	float fLengthY;
	float fLengthZ;
	float fDistance;
	fLengthX = powf(g_camera.posR.x - g_camera.posV.x, 2);
	fLengthY = g_camera.posR.y - g_camera.posV.y;
	fLengthZ = powf(g_camera.posR.z - g_camera.posV.z, 2);
	fDistance = sqrtf(fLengthX + fLengthZ);
	fRot = atan2f(fLengthY, fDistance);
	return fRot;
}

//カメラリセット
void ResetCamera(void)
{
	g_camera.posV = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_camera.posR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_camera.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}

//カメラの目的の方向取得
float CameraRotDesh(void)
{
	return g_camera.rotDesh.y;
}