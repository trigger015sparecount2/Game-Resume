//=============================================================================
//
// 弾丸処理 [bullet.h]
// Author : 三上航世
//
//=============================================================================
#include "manager.h"
#include "bullet.h"
#include "renderer.h"
#include "enemy.h"
#include "effect.h"
#include "player.h"
#include "pauseui.h"

//静的メンバ変数
LPDIRECT3DTEXTURE9 CBullet::m_pTexture = NULL;

//コンストラクタ
CBullet::CBullet(PRIORITY Priority) : CScene2D::CScene2D(Priority)
{

}

//デストラクタ
CBullet::~CBullet(void)
{

}

//初期化処理
HRESULT CBullet::Init(D3DXVECTOR3 pos, D3DXVECTOR3 move, bool bEnemy, CScene::ELEMENT element)
{
	CScene2D::Init(BULLET_SIZE, BULLET_SIZE, pos, 1.0f);
	CScene::SetObjType(CScene::OBJTYPE_BULLET);
	SetPos(pos);
	CScene::SetEnemy(bEnemy);		//敵のものか設定
	CScene::SetElement(element);	//属性設定
	m_move = move;
	m_bUninit = false;
	m_nEffect = 0;

	//属性によって色を変える
	switch (element)
	{
	case ELEMENT_FIRE:
		ChangeColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
		break;
	case ELEMENT_ICE:
		ChangeColor(D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f));
		break;
	case ELEMENT_THUNDER:
		ChangeColor(D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f));
		break;
	case ELEMENT_WATER:
		ChangeColor(D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f));
		break;
	default:
		break;
	}
	return S_OK;
}

//終了処理
void CBullet::Uninit(void)
{
	CScene2D::Uninit();
}

//更新処理
void CBullet::Update(void)
{
	if (CPauseUi::GetPause() == false)
	{
		if (m_bUninit == false)
		{
			D3DXVECTOR3 pos = GetPos();
			pos += m_move;
			Set(BULLET_SIZE, BULLET_SIZE, pos);
			m_nEffect++;
			//エフェクトカウントが一定になるたびに、エフェクトを作成する
			if (m_nEffect >= 4)
			{
				switch (GetElement())
				{
				case ELEMENT_FIRE:
					CEffect::Create(pos, CEffect::COLORTYPE_RED);
					break;
				case ELEMENT_ICE:
					CEffect::Create(pos, CEffect::COLORTYPE_LIGHTBLUE);
					break;
				case ELEMENT_THUNDER:
					CEffect::Create(pos, CEffect::COLORTYPE_YELLOW);
					break;
				case ELEMENT_WATER:
					CEffect::Create(pos, CEffect::COLORTYPE_BLUE);
					break;
				}
				m_nEffect = 0;
			}

			//画面外にいったら消える
			if (pos.x <= -BULLET_SIZE || SCREEN_WIDTH + BULLET_SIZE <= pos.x ||
				pos.y <= -BULLET_SIZE || SCREEN_HEIGHT + BULLET_SIZE <= pos.y)
			{
				m_bUninit = true;
			}

			int nCntScene;
			//プレイヤーが発射したものなら
			if (CScene::GetEnemy() == false)
			{
				for (nCntScene = 0; nCntScene < MAX_POLYGON; nCntScene++)
				{
					CScene *pScene;
					pScene = GetScene(PRIORITY_CHARA, nCntScene);
					if (pScene != NULL)
					{
						OBJTYPE objType;
						objType = pScene->GetObjType();
						//敵だったら
						if (objType == OBJTYPE_ENEMY)
						{
							D3DXVECTOR3 posEnemy;
							posEnemy = pScene->GetPos();
							//敵に当たったら
							if (posEnemy.x - ENEMY_SIZE * sinf(0.25f * D3DX_PI) <= pos.x + BULLET_SIZE * sinf(0.25f * D3DX_PI) && pos.x - BULLET_SIZE * sinf(0.25f * D3DX_PI) <= posEnemy.x + ENEMY_SIZE * sinf(0.25f * D3DX_PI) &&
								posEnemy.y - ENEMY_SIZE * cosf(0.25f * D3DX_PI) <= pos.y + BULLET_SIZE * cosf(0.25f * D3DX_PI) && pos.y - BULLET_SIZE * cosf(0.25f * D3DX_PI) <= posEnemy.y + ENEMY_SIZE * cosf(0.25f * D3DX_PI))
							{
								//ダメージ、弾を消す
								pScene->Damage(1);
								m_bUninit = true;
							}
						}
					}
				}
			}
			//敵が発射したものなら
			else
			{
				for (nCntScene = 0; nCntScene < MAX_POLYGON; nCntScene++)
				{
					CScene *pScene;
					pScene = GetScene(PRIORITY_CHARA, nCntScene);
					if (pScene != NULL)
					{
						OBJTYPE objType;
						objType = pScene->GetObjType();
						//プレイヤーだったら
						if (objType == OBJTYPE_PLAYER)
						{
							D3DXVECTOR3 posPlayer;
							posPlayer = pScene->GetPos();
							//プレイヤーに当たったら
							if (posPlayer.x - POLYGON_WIDTH * sinf(0.25f * D3DX_PI) * 0.5f <= pos.x + BULLET_SIZE * sinf(0.25f * D3DX_PI) && pos.x - BULLET_SIZE * sinf(0.25f * D3DX_PI) <= posPlayer.x + POLYGON_WIDTH * sinf(0.25f * D3DX_PI) * 0.5f &&
								posPlayer.y - POLYGON_HEIGHT * cosf(0.25f * D3DX_PI) * 0.5f <= pos.y + BULLET_SIZE * cosf(0.25f * D3DX_PI) && pos.y - BULLET_SIZE * cosf(0.25f * D3DX_PI) <= posPlayer.y + POLYGON_HEIGHT * cosf(0.25f * D3DX_PI) * 0.5f)
							{
								//当たり判定があるならダメージを与える、弾を消す
								if (pScene->GetDamage() == true)
								{
									pScene->Damage(1);
								}
								m_bUninit = true;
							}
							break;
						}
					}
				}
			}
		}
		else
		{
			Uninit();
		}
	}
}

//描画処理
void CBullet::Draw(void)
{
	CScene2D::Draw();
}

//作成
CBullet *CBullet::Create(D3DXVECTOR3 pos, D3DXVECTOR3 move, bool bEnemy, CScene::ELEMENT element)
{
	CBullet *pBullet;
	pBullet = new CBullet(PRIORITY_BULLET);
	if (pBullet != NULL)
	{
		pBullet->Init(pos, move, bEnemy, element);
		pBullet->BindTexture(m_pTexture);
	}
	return pBullet;
}

//テクスチャ読み込み
HRESULT CBullet::Load(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = CManager::GetRenderer()->GetDevice();
	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/bullet.png",
		&m_pTexture);
	return S_OK;
}

//テクスチャ破棄
void CBullet::UnLoad(void)
{
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
}