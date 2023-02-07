//=============================================================================
//
// リザルト [result.h]
// Author : 三上航世
//
//=============================================================================
#include "scene2d.h"
#include "sound.h"
#include "result.h"
#include "manager.h"
#include "fade.h"
#include "resultui.h"

CResult::CResult(PRIORITY Priority) : CScene::CScene(Priority)
{

}

CResult::~CResult(void)
{

}

//初期化
HRESULT CResult::Init(float, float, D3DXVECTOR3, float)
{
	CScene::SetObjType(CScene::OBJTYPE_NONE);
	CResultUi::Load();
	CResultUi::Create();
	CSound::Play(CSound::SOUND_LABEL_RESULTBGM);
	return S_OK;
}

//終了
void CResult::Uninit(void)
{
	CResultUi::UnLoad();
	Release();
}

//更新
void CResult::Update(void)
{
	
}

//描画
void CResult::Draw(void)
{

}