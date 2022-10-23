//=============================================================================
//
// タイトル [title.h]
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

HRESULT CResult::Init(float fWidth, float fHeight, D3DXVECTOR3 pos, float fTexSize)
{
	CScene::SetObjType(CScene::OBJTYPE_NONE);
	CResultUi::Load();
	CResultUi::Create();
	CSound::Play(CSound::SOUND_LABEL_RESULTBGM);
	return S_OK;
}

void CResult::Uninit(void)
{
	CResultUi::UnLoad();
	Release();
}

void CResult::Update(void)
{
	
}

void CResult::Draw(void)
{

}

void CResult::Damage(int nDamage)
{

}

bool CResult::GetDamage(void)
{
	return false;
}