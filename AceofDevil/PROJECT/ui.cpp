//=============================================================================
//
// UI処理 [ui.h]
// Author : 三上航世
//
//=============================================================================
#include "ui.h"
#include "manager.h"
#include "renderer.h"
#include "scene2d.h"
#include "input.h"

//静的メンバ変数
LPDIRECT3DTEXTURE9 CUi::m_pTexture[23] = {};
CScene2D *CUi::m_apScene[MAX_UIDATA] = {};
int CUi::m_nDamageTime = 0;
int CUi::m_nLife = 5;
int CUi::m_nKill = 0;
int CUi::m_nScore = 0;
int CUi::m_nType = 0;
D3DXVECTOR3 CUi::m_apos[5] = {};
float CUi::m_fClear[4] = {};

CUi::CUi(PRIORITY Priority) : CScene::CScene(Priority)
{

}

CUi::~CUi(void)
{

}

HRESULT CUi::Init(float fWidth, float fHeight, D3DXVECTOR3 pos, float fTexSize)
{
	m_nLife = 5;
	m_nDamageTime = 0;
	m_nKill = 0;
	m_nScore = 0;
	m_bClear = true;
	m_nClearTime = 5;
	int nCntLife = 0;
	//撃墜数四角
	//端
	m_apScene[nCntLife] = CScene2D::Create(14.0f, 40.0f, D3DXVECTOR3(20.0f, 40.0f, 0.0f), PRIORITY_UI);
	m_apScene[nCntLife]->BindTexture(m_pTexture[0]);
	m_apScene[nCntLife]->ChangeColor(D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f));
	nCntLife++;
	//真ん中
	m_apScene[nCntLife] = CScene2D::Create(104.0f, 40.0f, D3DXVECTOR3(103.0f, 40.0f, 0.0f), PRIORITY_UI);
	m_apScene[nCntLife]->ChangeColor(D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f));
	m_apScene[nCntLife]->BindTexture(m_pTexture[1]);
	nCntLife++;
	//端
	m_apScene[nCntLife] = CScene2D::Create(14.0f, 40.0f, D3DXVECTOR3(186.0f, 40.0f, 0.0f), PRIORITY_UI);
	m_apScene[nCntLife]->BindTexture(m_pTexture[0]);
	m_apScene[nCntLife]->SetRot(1.0f);
	m_apScene[nCntLife]->ChangeColor(D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f));
	m_apScene[nCntLife]->Set(14.0f, 40.0f, D3DXVECTOR3(186.0f, 40.0f, 0.0f));
	nCntLife++;

	//撃墜数
	//文字
	m_apScene[nCntLife] = CScene2D::Create(60.0f, 30.0f, D3DXVECTOR3(65.0f, 40.0f, 0.0f), PRIORITY_UI);
	m_apScene[nCntLife]->ChangeColor(D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f));
	m_apScene[nCntLife]->BindTexture(m_pTexture[3]);
	nCntLife++;
	//コンマ
	m_apScene[nCntLife] = CScene2D::Create(6.0f, 20.0f, D3DXVECTOR3(110.0f, 40.0f, 0.0f), PRIORITY_UI);
	m_apScene[nCntLife]->BindTexture(m_pTexture[5]);
	m_apScene[nCntLife]->ChangeColor(D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f));
	nCntLife++;
	//数字
	int nLag = nCntLife;
	for (nCntLife; nCntLife < nLag + 3; nCntLife++)
	{
		m_apScene[nCntLife] = CScene2D::Create(20.0f, 30.0f, D3DXVECTOR3(128.0f + 20.0f * (nCntLife - nLag), 40.0f, 0.0f), PRIORITY_UI);
		m_apScene[nCntLife]->BindTexture(m_pTexture[2]);
		m_apScene[nCntLife]->SetTexture(0, 10);
		m_apScene[nCntLife]->ChangeColor(D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f));
	}

	//スコア四角
	//端
	m_apScene[nCntLife] = CScene2D::Create(14.0f, 40.0f, D3DXVECTOR3(962.0f, 40.0f, 0.0f), PRIORITY_UI);
	m_apScene[nCntLife]->ChangeColor(D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f));
	m_apScene[nCntLife]->BindTexture(m_pTexture[0]);
	nCntLife++;
	//真ん中
	m_apScene[nCntLife] = CScene2D::Create(196.0f, 40.0f, D3DXVECTOR3(1110.0f, 40.0f, 0.0f), PRIORITY_UI);
	m_apScene[nCntLife]->ChangeColor(D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f));
	m_apScene[nCntLife]->BindTexture(m_pTexture[1]);
	nCntLife++;
	//端
	m_apScene[nCntLife] = CScene2D::Create(14.0f, 40.0f, D3DXVECTOR3(1258.0f, 40.0f, 0.0f), PRIORITY_UI);
	m_apScene[nCntLife]->ChangeColor(D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f));
	m_apScene[nCntLife]->BindTexture(m_pTexture[0]);
	m_apScene[nCntLife]->SetRot(1.0f);
	m_apScene[nCntLife]->Set(14.0f, 40.0f, D3DXVECTOR3(1258.0f, 40.0f, 0.0f));
	nCntLife++;

	//スコア
	//文字
	m_apScene[nCntLife] = CScene2D::Create(80.0f, 30.0f, D3DXVECTOR3(1020.0f, 40.0f, 0.0f), PRIORITY_UI);
	m_apScene[nCntLife]->ChangeColor(D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f));
	m_apScene[nCntLife]->BindTexture(m_pTexture[4]);
	nCntLife++;
	//コンマ
	m_apScene[nCntLife] = CScene2D::Create(9.0f, 20.0f, D3DXVECTOR3(1082.0f, 40.0f, 0.0f), PRIORITY_UI);
	m_apScene[nCntLife]->BindTexture(m_pTexture[5]);
	m_apScene[nCntLife]->ChangeColor(D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f));
	nCntLife++;
	//数字
	nLag = nCntLife;
	for (nCntLife; nCntLife < nLag + 8; nCntLife++)
	{
		m_apScene[nCntLife] = CScene2D::Create(20.0f, 30.0f, D3DXVECTOR3(1100.0f + 20.0f * (nCntLife - nLag), 40.0f, 0.0f), PRIORITY_UI);
		m_apScene[nCntLife]->BindTexture(m_pTexture[2]);
		m_apScene[nCntLife]->SetTexture(0, 10);
		m_apScene[nCntLife]->ChangeColor(D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f));
	}

	//自機関連四角
	//端
	m_apScene[nCntLife] = CScene2D::Create(100.0f, 24.0f, D3DXVECTOR3(1200.0f, 530.0f, 0.0f), PRIORITY_UI);
	m_apScene[nCntLife]->ChangeColor(D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f));
	m_apScene[nCntLife]->BindTexture(m_pTexture[9]);
	nCntLife++;
	//真ん中
	m_apScene[nCntLife] = CScene2D::Create(100.0f, 90.0f, D3DXVECTOR3(1200.0f, 610.0f, 0.0f), PRIORITY_UI);
	m_apScene[nCntLife]->ChangeColor(D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f));
	m_apScene[nCntLife]->BindTexture(m_pTexture[10]);
	nCntLife++;
	//端
	m_apScene[nCntLife] = CScene2D::Create(100.0f, 24.0f, D3DXVECTOR3(1200.0f, 680.0f, 0.0f), PRIORITY_UI);
	m_apScene[nCntLife]->ChangeColor(D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f));
	m_apScene[nCntLife]->BindTexture(m_pTexture[9]);
	m_apScene[nCntLife]->SetRot(1.0f);
	m_apScene[nCntLife]->Set(100.0f, 24.0f, D3DXVECTOR3(1200.0f, 690.0f, 0.0f));
	nCntLife++;

	//自機関連
	//自機HUD
	m_apScene[nCntLife] = CScene2D::Create(74.0f, 80.0f, D3DXVECTOR3(1200.0f, 590.0f, 0.0f), PRIORITY_UI);
	m_apScene[nCntLife]->ChangeColor(D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f));
	m_apScene[nCntLife]->BindTexture(m_pTexture[6]);
	nCntLife++;
	//DMG
	m_apScene[nCntLife] = CScene2D::Create(40.0f, 20.0f, D3DXVECTOR3(1170.0f, 665.0f, 0.0f), PRIORITY_UI);
	m_apScene[nCntLife]->ChangeColor(D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f));
	m_apScene[nCntLife]->BindTexture(m_pTexture[7]);
	nCntLife++;
	//コンマ
	m_apScene[nCntLife] = CScene2D::Create(8.0f, 15.0f, D3DXVECTOR3(1200.0f, 665.0f, 0.0f), PRIORITY_UI);
	m_apScene[nCntLife]->BindTexture(m_pTexture[5]);
	m_apScene[nCntLife]->ChangeColor(D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f));
	nCntLife++;
	//数字
	nLag = nCntLife;
	for (nCntLife; nCntLife < nLag + 2; nCntLife++)
	{
		m_apScene[nCntLife] = CScene2D::Create(15.0f, 20.0f, D3DXVECTOR3(1212.0f + 15.0f * (nCntLife - nLag), 665.0f, 0.0f), PRIORITY_UI);
		m_apScene[nCntLife]->BindTexture(m_pTexture[2]);
		m_apScene[nCntLife]->SetTexture(0, 10);
		m_apScene[nCntLife]->ChangeColor(D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f));
	}
	//%
	m_apScene[nCntLife] = CScene2D::Create(14.0f, 14.0f, D3DXVECTOR3(1247.0f, 665.0f, 0.0f), PRIORITY_UI);
	m_apScene[nCntLife]->BindTexture(m_pTexture[8]);
	m_apScene[nCntLife]->ChangeColor(D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f));
	nCntLife++;

	//魔法関連
	//四角
	//端
	m_apScene[nCntLife] = CScene2D::Create(28.0f, 80.0f, D3DXVECTOR3(30.0f, 660.0f, 0.0f), PRIORITY_UI);
	m_apScene[nCntLife]->ChangeColor(D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f));
	m_apScene[nCntLife]->BindTexture(m_pTexture[0]);
	nCntLife++;
	//真ん中
	m_apScene[nCntLife] = CScene2D::Create(220.0f, 80.0f, D3DXVECTOR3(205.0f, 660.0f, 0.0f), PRIORITY_UI);
	m_apScene[nCntLife]->ChangeColor(D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f));
	m_apScene[nCntLife]->BindTexture(m_pTexture[1]);
	nCntLife++;
	//端
	m_apScene[nCntLife] = CScene2D::Create(28.0f, 80.0f, D3DXVECTOR3(380.0f, 660.0f, 0.0f), PRIORITY_UI);
	m_apScene[nCntLife]->ChangeColor(D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f));
	m_apScene[nCntLife]->BindTexture(m_pTexture[0]);
	m_apScene[nCntLife]->SetRot(1.0f);
	m_apScene[nCntLife]->Set(28.0f, 80.0f, D3DXVECTOR3(380.0f, 660.0f, 0.0f));
	nCntLife++;

	//矢印
	m_apScene[nCntLife] = CScene2D::Create(18.0f, 35.0f, D3DXVECTOR3(265.0f, 660.0f, 0.0f), PRIORITY_UI);
	m_apScene[nCntLife]->ChangeColor(D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f));
	m_apScene[nCntLife]->BindTexture(m_pTexture[18]);
	m_apScene[nCntLife]->SetRot(1.0f);
	m_apScene[nCntLife]->Set(18.0f, 35.0f, D3DXVECTOR3(265.0f, 660.0f, 0.0f));
	nCntLife++;
	if (CInputGamePad::GetGamePad() == true)
	{
		m_apScene[nCntLife] = CScene2D::Create(9.0f, 7.0f, D3DXVECTOR3(262.0f, 660.0f, 0.0f), PRIORITY_UI);
		m_apScene[nCntLife]->ChangeColor(D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f));
		m_apScene[nCntLife]->BindTexture(m_pTexture[19]);
	}
	else
	{
		m_apScene[nCntLife] = CScene2D::Create(8.0f, 14.0f, D3DXVECTOR3(262.0f, 660.0f, 0.0f), PRIORITY_UI);
		m_apScene[nCntLife]->ChangeColor(D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f));
		m_apScene[nCntLife]->BindTexture(m_pTexture[21]);
	}
	nCntLife++;
	m_apScene[nCntLife] = CScene2D::Create(18.0f, 35.0f, D3DXVECTOR3(145.0f, 660.0f, 0.0f), PRIORITY_UI);
	m_apScene[nCntLife]->ChangeColor(D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f));
	m_apScene[nCntLife]->BindTexture(m_pTexture[18]);
	nCntLife++;
	if (CInputGamePad::GetGamePad() == true)
	{
		m_apScene[nCntLife] = CScene2D::Create(9.0f, 7.0f, D3DXVECTOR3(148.0f, 660.0f, 0.0f), PRIORITY_UI);
		m_apScene[nCntLife]->ChangeColor(D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f));
		m_apScene[nCntLife]->BindTexture(m_pTexture[20]);
	}
	else
	{
		m_apScene[nCntLife] = CScene2D::Create(8.0f, 14.0f, D3DXVECTOR3(148.0f, 660.0f, 0.0f), PRIORITY_UI);
		m_apScene[nCntLife]->ChangeColor(D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f));
		m_apScene[nCntLife]->BindTexture(m_pTexture[22]);
	}
	nCntLife++;

	m_nType = 0;
	int nMagic = 0;
	//下地
	//火属性
	m_apos[nMagic] = D3DXVECTOR3(205.0f, 660.0f, 0.0f);
	m_apScene[nCntLife] = CScene2D::Create(62.0f, 62.0f, m_apos[nMagic], PRIORITY_UI);
	m_apScene[nCntLife]->BindTexture(m_pTexture[16]);
	m_apScene[nCntLife]->ChangeColor(D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f));
	nCntLife++;
	//氷属性
	nMagic++;
	m_apos[nMagic] = D3DXVECTOR3(305.0f, 675.0f, 0.0f);
	m_apScene[nCntLife] = CScene2D::Create(32.0f, 32.0f, m_apos[nMagic], PRIORITY_UI);
	m_apScene[nCntLife]->BindTexture(m_pTexture[16]);
	m_apScene[nCntLife]->ChangeColor(D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f));
	nCntLife++;
	//雷属性
	nMagic++;
	m_apos[nMagic] = D3DXVECTOR3(355.0f, 675.0f, 0.0f);
	m_apScene[nCntLife] = CScene2D::Create(32.0f, 32.0f, m_apos[nMagic], PRIORITY_UI);
	m_apScene[nCntLife]->BindTexture(m_pTexture[16]);
	m_apScene[nCntLife]->ChangeColor(D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f));
	nCntLife++;
	//水属性
	nMagic++;
	m_apos[nMagic] = D3DXVECTOR3(55.0f, 675.0f, 0.0f);
	m_apScene[nCntLife] = CScene2D::Create(32.0f, 32.0f, m_apos[nMagic], PRIORITY_UI);
	m_apScene[nCntLife]->BindTexture(m_pTexture[16]);
	m_apScene[nCntLife]->ChangeColor(D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f));
	nCntLife++;
	//風属性
	nMagic++;
	m_apos[nMagic] = D3DXVECTOR3(105.0f, 675.0f, 0.0f);
	m_apScene[nCntLife] = CScene2D::Create(32.0f, 32.0f, m_apos[nMagic], PRIORITY_UI);
	m_apScene[nCntLife]->BindTexture(m_pTexture[16]);
	m_apScene[nCntLife]->ChangeColor(D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f));
	nCntLife++;
	//ゲージ
	//火
	m_apScene[nCntLife] = CScene2D::Create(60.0f, 60.0f, m_apos[0], PRIORITY_UI);
	m_apScene[nCntLife]->BindTexture(m_pTexture[17]);
	m_apScene[nCntLife]->ChangeColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
	nCntLife++;
	//氷
	m_apScene[nCntLife] = CScene2D::Create(30.0f, 30.0f, m_apos[1], PRIORITY_UI);
	m_apScene[nCntLife]->BindTexture(m_pTexture[17]);
	m_apScene[nCntLife]->ChangeColor(D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f));
	nCntLife++;
	//雷
	m_apScene[nCntLife] = CScene2D::Create(30.0f, 30.0f, m_apos[2], PRIORITY_UI);
	m_apScene[nCntLife]->BindTexture(m_pTexture[17]);
	m_apScene[nCntLife]->ChangeColor(D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f));
	nCntLife++;
	//水
	m_apScene[nCntLife] = CScene2D::Create(30.0f, 30.0f, m_apos[3], PRIORITY_UI);
	m_apScene[nCntLife]->BindTexture(m_pTexture[17]);
	m_apScene[nCntLife]->ChangeColor(D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f));
	nCntLife++;
	//風
	m_apScene[nCntLife] = CScene2D::Create(30.0f, 30.0f, m_apos[4], PRIORITY_UI);
	m_apScene[nCntLife]->BindTexture(m_pTexture[17]);
	m_apScene[nCntLife]->ChangeColor(D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f));
	nCntLife++;
	//属性マーク
	//火
	m_apScene[nCntLife] = CScene2D::Create(60.0f, 60.0f, m_apos[0], PRIORITY_UI);
	m_apScene[nCntLife]->BindTexture(m_pTexture[11]); //45
	nCntLife++;
	//氷
	m_apScene[nCntLife] = CScene2D::Create(30.0f, 30.0f, m_apos[1], PRIORITY_UI);
	m_apScene[nCntLife]->BindTexture(m_pTexture[12]);
	nCntLife++;
	//雷
	m_apScene[nCntLife] = CScene2D::Create(30.0f, 30.0f, m_apos[2], PRIORITY_UI);
	m_apScene[nCntLife]->BindTexture(m_pTexture[13]);
	nCntLife++;
	//水
	m_apScene[nCntLife] = CScene2D::Create(30.0f, 30.0f, m_apos[3], PRIORITY_UI);
	m_apScene[nCntLife]->BindTexture(m_pTexture[14]);
	nCntLife++;
	//風
	m_apScene[nCntLife] = CScene2D::Create(30.0f, 30.0f, m_apos[4], PRIORITY_UI);
	m_apScene[nCntLife]->BindTexture(m_pTexture[15]);
	//nCntLife++;

	int nCntClear;
	for (nCntClear = 0; nCntClear < 4; nCntClear++)
	{
		m_fClear[nCntClear] = 1.0f;
	}
	return S_OK;
}

void CUi::Uninit(void)
{
	int nCntScene;
	for (nCntScene = 0; nCntScene < MAX_UIDATA; nCntScene++)
	{
		if (m_apScene[nCntScene] != NULL)
		{
			m_apScene[nCntScene] = NULL;
		}
	}
	Release();
}

void CUi::Update(void)
{
	if (m_nLife > 0)
	{
		if (m_nDamageTime > 0)
		{
			m_nDamageTime--;
			if (m_nDamageTime <= 0)
			{
				int nCntScene;
				for (nCntScene = 0; nCntScene < MAX_UIDATA - 16; nCntScene++)
				{
					if (m_apScene[nCntScene] != NULL)
					{
						if (nCntScene == 24)
						{
							switch (m_nLife)
							{
							case 0:
								m_apScene[nCntScene]->ChangeColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, m_fClear[2]));
								break;
							case 1:
								m_apScene[nCntScene]->ChangeColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, m_fClear[2]));
								break;
							case 2:
								m_apScene[nCntScene]->ChangeColor(D3DXCOLOR(1.0f, 0.5f, 0.0f, m_fClear[2]));
								break;
							case 3:
								m_apScene[nCntScene]->ChangeColor(D3DXCOLOR(1.0f, 1.0f, 0.0f, m_fClear[2]));
								break;
							case 4:
								m_apScene[nCntScene]->ChangeColor(D3DXCOLOR(0.7f, 1.0f, 0.0f, m_fClear[2]));
								break;
							case 5:
								m_apScene[nCntScene]->ChangeColor(D3DXCOLOR(0.0f, 1.0f, 0.0f, m_fClear[2]));
								break;
							default:
								break;
							}
						}
						else
						{
							//Kill
							if (nCntScene < 8)
							{
								m_apScene[nCntScene]->ChangeColor(D3DXCOLOR(0.0f, 1.0f, 0.0f, m_fClear[0]));
							} 
							//Score
							else if (nCntScene < 21)
							{
								m_apScene[nCntScene]->ChangeColor(D3DXCOLOR(0.0f, 1.0f, 0.0f, m_fClear[1]));
							}
							//DMG
							else if (nCntScene < 30)
							{
								m_apScene[nCntScene]->ChangeColor(D3DXCOLOR(0.0f, 1.0f, 0.0f, m_fClear[2]));
							}
							//左下四角と矢印
							else if (nCntScene < 37)
							{
								m_apScene[nCntScene]->ChangeColor(D3DXCOLOR(0.0f, 1.0f, 0.0f, m_fClear[3]));
							}
							//ゲージ下地
							else if (nCntScene < 42)
							{
								m_apScene[nCntScene]->ChangeColor(D3DXCOLOR(0.0f, 0.0f, 0.0f, m_fClear[3]));
							}
							//ゲージ
							else if (nCntScene < 48)
							{
								switch (nCntScene)
								{
								case 42:
									//火
									m_apScene[nCntScene]->ChangeColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, m_fClear[3]));
									break;
								case 43:
									//氷
									m_apScene[nCntScene]->ChangeColor(D3DXCOLOR(0.0f, 1.0f, 1.0f, m_fClear[3]));
									break;
								case 44:
									//雷
									m_apScene[nCntScene]->ChangeColor(D3DXCOLOR(1.0f, 1.0f, 0.0f, m_fClear[3]));
									break;
								case 45:
									//水
									m_apScene[nCntScene]->ChangeColor(D3DXCOLOR(0.0f, 0.0f, 1.0f, m_fClear[3]));
									break;
								case 46:
									//風
									m_apScene[nCntScene]->ChangeColor(D3DXCOLOR(0.0f, 1.0f, 0.0f, m_fClear[3]));
									break;
								default:
									break;
								}
							}
							//マーク
							else if (nCntScene < MAX_UIDATA)
							{
								m_apScene[nCntScene]->ChangeColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, m_fClear[3]));
							}
						}
					}
				}
			}
		}
		else if (m_nLife == 1)
		{
			m_nClearTime--;
			if (m_nClearTime <= 0)
			{
				if (m_bClear == true)
				{
					m_apScene[24]->ChangeColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, 0.0f));
					m_bClear = false;
				}
				else
				{
					m_apScene[24]->ChangeColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, m_fClear[2]));
					m_bClear = true;
				}
				m_nClearTime = 5;
			}
		}
	}
	else
	{
		if (m_nDamageTime > 0)
		{
			m_nDamageTime--;
			if (m_nDamageTime <= 0)
			{
				int nCntScene;
				for (nCntScene = 0; nCntScene < MAX_UIDATA; nCntScene++)
				{
					if (m_apScene[nCntScene] != NULL)
					{
						m_apScene[nCntScene]->ChangeColor(D3DXCOLOR(0.0f, 1.0f, 0.0f, 0.0f));
					}
				}
			}
		}
	}
}

void CUi::Draw(void)
{

}

HRESULT CUi::Load(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = CManager::GetRenderer()->GetDevice();
	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/squareside.png",
		&m_pTexture[0]);
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/squarecenter.png",
		&m_pTexture[1]);
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/number.png",
		&m_pTexture[2]);
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/kill.png",
		&m_pTexture[3]);
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/score.png",
		&m_pTexture[4]);
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/comma.png",
		&m_pTexture[5]);
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/HUDF-15S_MTD.png",
		&m_pTexture[6]);
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/DMG.png",
		&m_pTexture[7]);
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/%mark.png",
		&m_pTexture[8]);
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/bigsquareside.png",
		&m_pTexture[9]);
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/bigsquarecenter.png",
		&m_pTexture[10]);
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/firemark.png",
		&m_pTexture[11]);
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/icemark.png",
		&m_pTexture[12]);
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/thundermark.png",
		&m_pTexture[13]);
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/watermark.png",
		&m_pTexture[14]);
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/windmark.png",
		&m_pTexture[15]);
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/whitebg.png",
		&m_pTexture[16]);
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/Gauge.png",
		&m_pTexture[17]);
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/Arrow.png",
		&m_pTexture[18]);
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/RT.png",
		&m_pTexture[19]);
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/LT.png",
		&m_pTexture[20]);
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/E.png",
		&m_pTexture[21]);
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/Q.png",
		&m_pTexture[22]);
	return S_OK;
}

void CUi::UnLoad(void)
{
	int nCntLife;
	for (nCntLife = 0; nCntLife < 23; nCntLife++)
	{
		if (m_pTexture[nCntLife] != NULL)
		{
			m_pTexture[nCntLife]->Release();
			m_pTexture[nCntLife] = NULL;
		}
	}
}

CUi *CUi::Create(void)
{
	CUi *pUi;
	pUi = new CUi(PRIORITY_UI);
	if (pUi != NULL)
	{
		pUi->Init(0.0f, 0.0f, D3DXVECTOR3(0.0f, 0.0f, 0.0f), 1.0f);
	}
	return pUi;
}

void CUi::Damage(void)
{
	if (m_nLife > 0)
	{
		m_nDamageTime = 10;
	}
	else
	{
		m_nDamageTime = 150;
	}
	int nCntLife = 0;
	int nLag;
	//撃墜数四角
	//端
	m_apScene[nCntLife]->ChangeColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, m_fClear[0]));
	nCntLife++;
	//真ん中
	m_apScene[nCntLife]->ChangeColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, m_fClear[0]));
	nCntLife++;
	//端
	m_apScene[nCntLife]->ChangeColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, m_fClear[0]));
	nCntLife++;

	//撃墜数
	if (m_nLife <= 0)
	{
		//文字
		m_apScene[nCntLife]->ChangeColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, 0.0f));
		nCntLife++;
		//コンマ
		m_apScene[nCntLife]->ChangeColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, 0.0f));
		nCntLife++;
		//数字
		int nLag = nCntLife;
		for (nCntLife; nCntLife < nLag + 3; nCntLife++)
		{
			m_apScene[nCntLife]->ChangeColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, 0.0f));
		}
	}
	else
	{
		//文字
		m_apScene[nCntLife]->ChangeColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, m_fClear[0]));
		nCntLife++;
		//コンマ
		m_apScene[nCntLife]->ChangeColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, m_fClear[0]));
		nCntLife++;
		//数字
		int nLag = nCntLife;
		for (nCntLife; nCntLife < nLag + 3; nCntLife++)
		{
			m_apScene[nCntLife]->ChangeColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, m_fClear[0]));
		}
	}

	//スコア四角
	//端
	m_apScene[nCntLife]->ChangeColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, m_fClear[1]));
	nCntLife++;
	//真ん中
	m_apScene[nCntLife]->ChangeColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, m_fClear[1]));
	nCntLife++;
	//端
	m_apScene[nCntLife]->ChangeColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, m_fClear[1]));
	nCntLife++;

	//スコア
	if (m_nLife <= 0)
	{
		//文字
		m_apScene[nCntLife]->ChangeColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, 0.0f));
		nCntLife++;
		//コンマ
		m_apScene[nCntLife]->ChangeColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, 0.0f));
		nCntLife++;
		//数字
		nLag = nCntLife;
		for (nCntLife; nCntLife < nLag + 8; nCntLife++)
		{
			m_apScene[nCntLife]->ChangeColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, 0.0f));
		}
	}
	else
	{
		//文字
		m_apScene[nCntLife]->ChangeColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, m_fClear[1]));
		nCntLife++;
		//コンマ
		m_apScene[nCntLife]->ChangeColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, m_fClear[1]));
		nCntLife++;
		//数字
		nLag = nCntLife;
		for (nCntLife; nCntLife < nLag + 8; nCntLife++)
		{
			m_apScene[nCntLife]->ChangeColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, m_fClear[1]));
		}
	}

	//自機関連四角
	//端
	m_apScene[nCntLife]->ChangeColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, m_fClear[2]));
	nCntLife++;
	//真ん中
	m_apScene[nCntLife]->ChangeColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, m_fClear[2]));
	nCntLife++;
	//端
	m_apScene[nCntLife]->ChangeColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, m_fClear[2]));
	nCntLife++;


	//自機関連
	if(m_nLife <= 0)
	{ 
		//自機HUD
		m_apScene[nCntLife]->ChangeColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, 0.0f));
		nCntLife++;
		//DMG
		m_apScene[nCntLife]->ChangeColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, 0.0f));
		nCntLife++;
		//コンマ
		m_apScene[nCntLife]->ChangeColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, 0.0f));
		nCntLife++;
		//数字
		nLag = nCntLife;
		int nRank;
		int nNumber;
		for (nCntLife; nCntLife < nLag + 2; nCntLife++)
		{
			nRank = pow(10, nLag + 2 - nCntLife);
			nNumber = ((5 - m_nLife) * 20) % nRank / (nRank / 10);
			m_apScene[nCntLife]->SetTexture(nNumber, 10);
			m_apScene[nCntLife]->ChangeColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, 0.0f));
		}
		//%
		m_apScene[nCntLife]->ChangeColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, 0.0f));
		nCntLife++;
	}
	else
	{
		//自機HUD
		m_apScene[nCntLife]->ChangeColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, m_fClear[2]));
		nCntLife++;
		//DMG
		m_apScene[nCntLife]->ChangeColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, m_fClear[2]));
		nCntLife++;
		//コンマ
		m_apScene[nCntLife]->ChangeColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, m_fClear[2]));
		nCntLife++;
		//数字
		nLag = nCntLife;
		int nRank;
		int nNumber;
		for (nCntLife; nCntLife < nLag + 2; nCntLife++)
		{
			nRank = pow(10, nLag + 2 - nCntLife);
			nNumber = ((5 - m_nLife) * 20) % nRank / (nRank / 10);
			m_apScene[nCntLife]->SetTexture(nNumber, 10);
			m_apScene[nCntLife]->ChangeColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, m_fClear[2]));
		}
		//%
		m_apScene[nCntLife]->ChangeColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, m_fClear[2]));
		nCntLife++;
	}
	//属性関連
	//四角
	m_apScene[nCntLife]->ChangeColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, m_fClear[3]));
	nCntLife++;
	m_apScene[nCntLife]->ChangeColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, m_fClear[3]));
	nCntLife++;
	m_apScene[nCntLife]->ChangeColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, m_fClear[3]));
	nCntLife++;

	if (m_nLife <= 0)
	{
		for (nCntLife = 33; nCntLife < MAX_UIDATA; nCntLife++)
		{
			m_apScene[nCntLife]->ChangeColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, 0.0f));
		}
	}
	else
	{
		//矢印
		m_apScene[nCntLife]->ChangeColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, m_fClear[3]));
		nCntLife++;
		m_apScene[nCntLife]->ChangeColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, m_fClear[3]));
		nCntLife++;
		m_apScene[nCntLife]->ChangeColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, m_fClear[3]));
		nCntLife++;
		m_apScene[nCntLife]->ChangeColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, m_fClear[3]));
		nCntLife++;
	}
}

void CUi::Damage(int nDamage)
{

}

bool CUi::GetDamage(void)
{
	return false;
}

void CUi::SetLife(int nLife)
{
	m_nLife = nLife;
}

void CUi::SetScore(int nScore)
{
	m_nScore += nScore;
	//数字
	int nRank;
	int nNumber;
	int nCntScene = 13;
	int nLag = nCntScene;
	for (nCntScene; nCntScene < nLag + 8; nCntScene++)
	{
		nRank = pow(10, nLag + 8 - nCntScene);
		nNumber = m_nScore % nRank / (nRank / 10);
		m_apScene[nCntScene]->SetTexture(nNumber, 10);
	}
}

void CUi::SetKill(void)
{
	m_nKill++;
	//数字
	int nRank;
	int nNumber;
	int nCntScene = 5;
	int nLag = nCntScene;
	for (nCntScene; nCntScene < nLag + 3; nCntScene++)
	{
		nRank = pow(10, nLag + 3 - nCntScene);
		nNumber = m_nKill % nRank / (nRank / 10);
		m_apScene[nCntScene]->SetTexture(nNumber, 10);
	}
}

int CUi::GetKill(void)
{
	return m_nKill;
}

int CUi::GetScore(void)
{
	return m_nScore;
}

void CUi::ChangeMagic(bool bPlus)
{
	int nCntMagic;
	int nCntScene;
	D3DXVECTOR3 Lagpos;
	if (bPlus == true)
	{
		for (nCntMagic = 0; nCntMagic < 4; nCntMagic++)
		{
			Lagpos = m_apos[nCntMagic + 1];
			m_apos[nCntMagic + 1] = m_apos[nCntMagic];
			m_apos[nCntMagic] = Lagpos;
		}
		m_nType--;
		if (m_nType < 0)
		{
			m_nType = 4;
		}
	}
	else
	{
		for (nCntMagic = 1; nCntMagic < 5; nCntMagic++)
		{
			Lagpos = m_apos[5 - nCntMagic];
			m_apos[5 - nCntMagic] = m_apos[4 - nCntMagic];
			m_apos[4 - nCntMagic] = Lagpos;
		}
		m_nType = (m_nType + 1) % 5;
	}
	for (nCntScene = 0; nCntScene < 5; nCntScene++)
	{
		if (m_nType == nCntScene)
		{
			m_apScene[47 + nCntScene]->Set(60.0f, 60.0f, m_apos[nCntScene]);
		}
		else
		{
			m_apScene[47 + nCntScene]->Set(30.0f, 30.0f, m_apos[nCntScene]);
		}
	}
}

void CUi::SetFireGauge(int nFire)
{
	int nUi = 42;
	if (m_nType == 0)
	{
		m_apScene[nUi]->Set(60.0f, ((float)nFire / 1000.0f) * 60.0f, D3DXVECTOR3(m_apos[0].x, m_apos[0].y + (1000.0f - (float)nFire) / 1000.0f * 60.0f * sinf(0.25f * D3DX_PI), 0.0f));
	}
	else
	{
		m_apScene[nUi]->Set(30.0f, ((float)nFire / 1000.0f) * 30.0f, D3DXVECTOR3(m_apos[0].x, m_apos[0].y + (1000.0f - (float)nFire) / 1000.0f * 30.0f * sinf(0.25f * D3DX_PI), 0.0f));
	}
}

void CUi::SetIceGauge(int nIce)
{
	int nUi = 43;
	if (m_nType == 1)
	{
		m_apScene[nUi]->Set(60.0f, ((float)nIce / 1000.0f) * 60.0f, D3DXVECTOR3(m_apos[1].x, m_apos[1].y + (1000.0f - (float)nIce) / 1000.0f * 60.0f * sinf(0.25f * D3DX_PI), 0.0f));
	}
	else
	{
		m_apScene[nUi]->Set(30.0f, ((float)nIce / 1000.0f) * 30.0f, D3DXVECTOR3(m_apos[1].x, m_apos[1].y + (1000.0f - (float)nIce) / 1000.0f * 30.0f * sinf(0.25f * D3DX_PI), 0.0f));
	}
}

void CUi::SetThunderGauge(int nThunder)
{
	int nUi = 44;
	if (m_nType == 2)
	{
		m_apScene[nUi]->Set(60.0f, ((float)nThunder / 1000.0f) * 60.0f, D3DXVECTOR3(m_apos[2].x, m_apos[2].y + (1000.0f - (float)nThunder) / 1000.0f * 60.0f * sinf(0.25f * D3DX_PI), 0.0f));
	}
	else
	{
		m_apScene[nUi]->Set(30.0f, ((float)nThunder / 1000.0f) * 30.0f, D3DXVECTOR3(m_apos[2].x, m_apos[2].y + (1000.0f - (float)nThunder) / 1000.0f * 30.0f * sinf(0.25f * D3DX_PI), 0.0f));
	}
}

void CUi::SetWaterGauge(int nWater)
{
	int nUi = 45;
	if (m_nType == 3)
	{
		m_apScene[nUi]->Set(60.0f, ((float)nWater / 1000.0f) * 60.0f, D3DXVECTOR3(m_apos[3].x, m_apos[3].y + (1000.0f - (float)nWater) / 1000.0f * 60.0f * sinf(0.25f * D3DX_PI), 0.0f));
	}
	else
	{
		m_apScene[nUi]->Set(30.0f, ((float)nWater / 1000.0f) * 30.0f, D3DXVECTOR3(m_apos[3].x, m_apos[3].y + (1000.0f - (float)nWater) / 1000.0f * 30.0f * sinf(0.25f * D3DX_PI), 0.0f));
	}
}

void CUi::SetWindGauge(int nWind)
{
	int nUi = 46;
	if (m_nType == 4)
	{
		m_apScene[nUi]->Set(60.0f, ((float)nWind / 1000.0f) * 60.0f, D3DXVECTOR3(m_apos[4].x, m_apos[4].y + (1000.0f - (float)nWind) / 1000.0f * 60.0f * sinf(0.25f * D3DX_PI), 0.0f));
	}
	else
	{
		m_apScene[nUi]->Set(30.0f, ((float)nWind / 1000.0f) * 30.0f, D3DXVECTOR3(m_apos[4].x, m_apos[4].y + (1000.0f - (float)nWind) / 1000.0f * 30.0f * sinf(0.25f * D3DX_PI), 0.0f));
	}
}

void CUi::KillClear(D3DXVECTOR3 pos, float fWidth, float fHeight)
{
	int nCntUi = 0;
	if (pos.x - fWidth <= 186.0f + 14.0f * sinf(0.25f * D3DX_PI) && 20.0f - 14.0f * sinf(0.25f * D3DX_PI) <= pos.x + fWidth &&
		pos.y - fHeight <= 40.0f + 40.0f * cosf(0.25f * D3DX_PI) && 40.0f - 40.0f * cosf(0.25f * D3DX_PI) <= pos.y + fHeight)
	{
		if (m_fClear[0] > 0.3f)
		{
			m_fClear[0] -= 0.1f;
			if (m_fClear[0] <= 0.3f)
			{
				m_fClear[0] = 0.3f;
			}
			for (nCntUi = 0; nCntUi < 8; nCntUi++)
			{
				if (m_nDamageTime > 0)
				{
					m_apScene[nCntUi]->ChangeColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, m_fClear[0]));
				}
				else
				{
					m_apScene[nCntUi]->ChangeColor(D3DXCOLOR(0.0f, 1.0f, 0.0f, m_fClear[0]));
				}
			}
		}
	}
	else
	{
		if (m_fClear[0] < 1.0f)
		{
			m_fClear[0] += 0.1f;
			if (m_fClear[0] >= 1.0f)
			{
				m_fClear[0] = 1.0f;
			}
			for (nCntUi = 0; nCntUi < 8; nCntUi++)
			{
				if (m_nDamageTime > 0)
				{
					m_apScene[nCntUi]->ChangeColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, m_fClear[0]));
				}
				else
				{
					m_apScene[nCntUi]->ChangeColor(D3DXCOLOR(0.0f, 1.0f, 0.0f, m_fClear[0]));
				}
			}
		}
	}
}

void CUi::ScoreClear(D3DXVECTOR3 pos, float fWidth, float fHeight)
{
	int nCntUi = 0;
	if (pos.x - fWidth <= 1258.0f + 14.0f * sinf(0.25f * D3DX_PI) && 962.0f - 14.0f * sinf(0.25f * D3DX_PI) <= pos.x + fWidth &&
		pos.y - fHeight <= 40.0f + 40.0f * cosf(0.25f * D3DX_PI) && 40.0f - 40.0f * cosf(0.25f * D3DX_PI) <= pos.y + fHeight)
	{
		if (m_fClear[1] > 0.3f)
		{
			m_fClear[1] -= 0.1f;
			if (m_fClear[1] <= 0.3f)
			{
				m_fClear[1] = 0.3f;
			}
			for (nCntUi = 8; nCntUi < 21; nCntUi++)
			{
				if (m_nDamageTime > 0)
				{
					m_apScene[nCntUi]->ChangeColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, m_fClear[1]));
				}
				else
				{
					m_apScene[nCntUi]->ChangeColor(D3DXCOLOR(0.0f, 1.0f, 0.0f, m_fClear[1]));
				}
			}
		}
	}
	else
	{
		if (m_fClear[1] < 1.0f)
		{
			m_fClear[1] += 0.1f;
			if (m_fClear[1] >= 1.0f)
			{
				m_fClear[1] = 1.0f;
			}
			for (nCntUi = 8; nCntUi < 21; nCntUi++)
			{
				if (m_nDamageTime > 0)
				{
					m_apScene[nCntUi]->ChangeColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, m_fClear[1]));
				}
				else
				{
					m_apScene[nCntUi]->ChangeColor(D3DXCOLOR(0.0f, 1.0f, 0.0f, m_fClear[1]));
				}
			}
		}
	}
}

void CUi::DMGClear(D3DXVECTOR3 pos, float fWidth, float fHeight)
{
	int nCntUi = 0;
	if (pos.x - fWidth <= 1200.0f + 100.0f * sinf(0.25f * D3DX_PI) && 1200.0f - 100.0f * sinf(0.25f * D3DX_PI) <= pos.x + fWidth &&
		pos.y - fHeight <= 680.0f + 24.0f * cosf(0.25f * D3DX_PI) && 530.0f - 24.0f * cosf(0.25f * D3DX_PI) <= pos.y + fHeight)
	{
		if (m_fClear[2] > 0.3f)
		{
			m_fClear[2] -= 0.1f;
			if (m_fClear[2] <= 0.3f)
			{
				m_fClear[2] = 0.3f;
			}
			for (nCntUi = 21; nCntUi < 30; nCntUi++)
			{
				if (m_nDamageTime > 0)
				{
					m_apScene[nCntUi]->ChangeColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, m_fClear[2]));
				}
				else
				{
					if (nCntUi == 24)
					{
						switch (m_nLife)
						{
						case 0:
							m_apScene[nCntUi]->ChangeColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, m_fClear[2]));
							break;
						case 1:
							m_apScene[nCntUi]->ChangeColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, m_fClear[2]));
							break;
						case 2:
							m_apScene[nCntUi]->ChangeColor(D3DXCOLOR(1.0f, 0.5f, 0.0f, m_fClear[2]));
							break;
						case 3:
							m_apScene[nCntUi]->ChangeColor(D3DXCOLOR(1.0f, 1.0f, 0.0f, m_fClear[2]));
							break;
						case 4:
							m_apScene[nCntUi]->ChangeColor(D3DXCOLOR(0.7f, 1.0f, 0.0f, m_fClear[2]));
							break;
						case 5:
							m_apScene[nCntUi]->ChangeColor(D3DXCOLOR(0.0f, 1.0f, 0.0f, m_fClear[2]));
							break;
						default:
							break;
						}
					}
					else
					{
						m_apScene[nCntUi]->ChangeColor(D3DXCOLOR(0.0f, 1.0f, 0.0f, m_fClear[2]));
					}
				}
			}
		}
	}
	else
	{
		if (m_fClear[2] < 1.0f)
		{
			m_fClear[2] += 0.1f;
			if (m_fClear[2] >= 1.0f)
			{
				m_fClear[2] = 1.0f;
			}
			for (nCntUi = 21; nCntUi < 30; nCntUi++)
			{
				if (m_nDamageTime > 0)
				{
					m_apScene[nCntUi]->ChangeColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, m_fClear[2]));
				}
				else
				{
					if (nCntUi == 24)
					{
						switch (m_nLife)
						{
						case 0:
							m_apScene[nCntUi]->ChangeColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, m_fClear[2]));
							break;
						case 1:
							m_apScene[nCntUi]->ChangeColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, m_fClear[2]));
							break;
						case 2:
							m_apScene[nCntUi]->ChangeColor(D3DXCOLOR(1.0f, 0.5f, 0.0f, m_fClear[2]));
							break;
						case 3:
							m_apScene[nCntUi]->ChangeColor(D3DXCOLOR(1.0f, 1.0f, 0.0f, m_fClear[2]));
							break;
						case 4:
							m_apScene[nCntUi]->ChangeColor(D3DXCOLOR(0.7f, 1.0f, 0.0f, m_fClear[2]));
							break;
						case 5:
							m_apScene[nCntUi]->ChangeColor(D3DXCOLOR(0.0f, 1.0f, 0.0f, m_fClear[2]));
							break;
						default:
							break;
						}
					}
					else
					{
						m_apScene[nCntUi]->ChangeColor(D3DXCOLOR(0.0f, 1.0f, 0.0f, m_fClear[2]));
					}
				}
			}
		}
	}
}

void CUi::MagicClear(D3DXVECTOR3 pos, float fWidth, float fHeight)
{
	int nCntUi = 0;
	if (pos.x - fWidth <= 380.0f + 28.0f * sinf(0.25f * D3DX_PI) && 30.0f - 28.0f * sinf(0.25f * D3DX_PI) <= pos.x + fWidth &&
		pos.y - fHeight <= 660.0f + 80.0f * cosf(0.25f * D3DX_PI) && 660.0f - 80.0f * cosf(0.25f * D3DX_PI) <= pos.y + fHeight)
	{
		if (m_fClear[3] > 0.3f)
		{
			m_fClear[3] -= 0.1f;
			if (m_fClear[3] <= 0.3f)
			{
				m_fClear[3] = 0.3f;
			}
			for (nCntUi = 30; nCntUi < MAX_UIDATA; nCntUi++)
			{
				if (nCntUi < 37)
				{
					if (m_nDamageTime > 0)
					{
						m_apScene[nCntUi]->ChangeColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, m_fClear[3]));
					}
					else
					{
						m_apScene[nCntUi]->ChangeColor(D3DXCOLOR(0.0f, 1.0f, 0.0f, m_fClear[3]));
					}
				}
				else if(nCntUi < 42)
				{
					m_apScene[nCntUi]->ChangeColor(D3DXCOLOR(0.0f, 0.0f, 0.0f, m_fClear[3]));
				}
				else if (nCntUi < 47)
				{
					switch (nCntUi)
					{
					case 42:
						//火
						m_apScene[nCntUi]->ChangeColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, m_fClear[3]));
						break;
					case 43:
						//氷
						m_apScene[nCntUi]->ChangeColor(D3DXCOLOR(0.0f, 1.0f, 1.0f, m_fClear[3]));
						break;
					case 44:
						//雷
						m_apScene[nCntUi]->ChangeColor(D3DXCOLOR(1.0f, 1.0f, 0.0f, m_fClear[3]));
						break;
					case 45:
						//水
						m_apScene[nCntUi]->ChangeColor(D3DXCOLOR(0.0f, 0.0f, 1.0f, m_fClear[3]));
						break;
					case 46:
						//風
						m_apScene[nCntUi]->ChangeColor(D3DXCOLOR(0.0f, 1.0f, 0.0f, m_fClear[3]));
						break;
					default:
						break;
					}
				}
				else if (nCntUi < MAX_UIDATA)
				{
					m_apScene[nCntUi]->ChangeColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, m_fClear[3]));
				}
			}
		}
	}
	else
	{
		if (m_fClear[3] < 1.0f)
		{
			m_fClear[3] += 0.1f;
			if (m_fClear[3] >= 1.0f)
			{
				m_fClear[3] = 1.0f;
			}
			for (nCntUi = 30; nCntUi < MAX_UIDATA; nCntUi++)
			{
				if (nCntUi < 37)
				{
					if (m_nDamageTime > 0)
					{
						m_apScene[nCntUi]->ChangeColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, m_fClear[3]));
					}
					else
					{
						m_apScene[nCntUi]->ChangeColor(D3DXCOLOR(0.0f, 1.0f, 0.0f, m_fClear[3]));
					}
				}
				else if (nCntUi < 42)
				{
					m_apScene[nCntUi]->ChangeColor(D3DXCOLOR(0.0f, 0.0f, 0.0f, m_fClear[3]));
				}
				else if (nCntUi < 47)
				{
					switch (nCntUi)
					{
					case 42:
						//火
						m_apScene[nCntUi]->ChangeColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, m_fClear[3]));
						break;
					case 43:
						//氷
						m_apScene[nCntUi]->ChangeColor(D3DXCOLOR(0.0f, 1.0f, 1.0f, m_fClear[3]));
						break;
					case 44:
						//雷
						m_apScene[nCntUi]->ChangeColor(D3DXCOLOR(1.0f, 1.0f, 0.0f, m_fClear[3]));
						break;
					case 45:
						//水
						m_apScene[nCntUi]->ChangeColor(D3DXCOLOR(0.0f, 0.0f, 1.0f, m_fClear[3]));
						break;
					case 46:
						//風
						m_apScene[nCntUi]->ChangeColor(D3DXCOLOR(0.0f, 1.0f, 0.0f, m_fClear[3]));
						break;
					default:
						break;
					}
				}
				else if (nCntUi < MAX_UIDATA)
				{
					m_apScene[nCntUi]->ChangeColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, m_fClear[3]));
				}
			}
		}
	}
}