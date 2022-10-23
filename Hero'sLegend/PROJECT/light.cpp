//---------------------------
//Author:三上航世
//ライト(light.cpp)
//---------------------------
#include "light.h"

//グローバル変数
D3DLIGHT9 g_light[3]; //ライトの情報

//初期化処理
HRESULT InitLight(void)
{
	LPDIRECT3DDEVICE9 pDevice; //デバイスのポインタ
	D3DXVECTOR3 vecDir; //設定用方向ベクトル
	pDevice = GetDevice();     //デバイスを取得する
	//ライトをクリアする
	ZeroMemory(&g_light, sizeof(D3DLIGHT9));
	//ライトの種類を設定
	g_light[0].Type = D3DLIGHT_DIRECTIONAL; //偏向光源
	//ライトの拡散先（色）を設定
	g_light[0].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	//ライトの方向を設定
	vecDir = D3DXVECTOR3(0.2f, -0.8f, -0.4f);
	D3DXVec3Normalize(&vecDir, &vecDir); //正規化する
	g_light[0].Direction = vecDir;
	//ライトを設定する
	pDevice->SetLight(0, &g_light[0]);
	//ライトを有効にする
	pDevice->LightEnable(0, TRUE);
	//ライトの種類を設定
	g_light[1].Type = D3DLIGHT_DIRECTIONAL; //偏向光源
	//ライトの拡散先（色）を設定
	g_light[1].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	//ライトの方向を設定
	vecDir = D3DXVECTOR3(0.5f, -0.4f, 0.5f);
	D3DXVec3Normalize(&vecDir, &vecDir); //正規化する
	g_light[1].Direction = vecDir;
	//ライトを設定する
	pDevice->SetLight(1, &g_light[1]);
	//ライトを有効にする
	pDevice->LightEnable(1, TRUE);
	//ライトの種類を設定
	g_light[2].Type = D3DLIGHT_DIRECTIONAL; //偏向光源
	//ライトの拡散先（色）を設定
	g_light[2].Diffuse = D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f);
	//ライトの方向を設定
	vecDir = D3DXVECTOR3(-0.5f, -0.4f, -0.5f);
	D3DXVec3Normalize(&vecDir, &vecDir); //正規化する
	g_light[2].Direction = vecDir;
	//ライトを設定する
	pDevice->SetLight(2, &g_light[2]);
	//ライトを有効にする
	pDevice->LightEnable(2, TRUE);
	return S_OK;
}

void UninitLight(void)
{

}

void UpdateLight(void)
{

}

void SetLight(int nCntLight, D3DXCOLOR col, D3DXVECTOR3 vec)
{
	LPDIRECT3DDEVICE9 pDevice; //デバイスのポインタ
	D3DXVECTOR3 vecDir; //設定用方向ベクトル
	pDevice = GetDevice();     //デバイスを取得する
	//ライトをクリアする
	ZeroMemory(&g_light, sizeof(D3DLIGHT9));
	//ライトの種類を設定
	g_light[nCntLight].Type = D3DLIGHT_DIRECTIONAL; //偏向光源
	//ライトの拡散先（色）を設定
	g_light[nCntLight].Diffuse = D3DXCOLOR(col.r, col.g, col.b, 1.0f);
	//ライトの方向を設定
	vecDir = D3DXVECTOR3(vec.x, vec.y, vec.z);
	D3DXVec3Normalize(&vecDir, &vecDir); //正規化する
	g_light[nCntLight].Direction = vecDir;
	//ライトを設定する
	pDevice->SetLight(nCntLight, &g_light[nCntLight]);
	//ライトを有効にする
	pDevice->LightEnable(nCntLight, TRUE);
}