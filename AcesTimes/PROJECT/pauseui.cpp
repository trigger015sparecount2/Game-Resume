//=============================================================================
//�|�[�Y�Ǘ�[pauseui.cpp]
// Author : �O��@�q��
//=============================================================================
//*****************************************************************************
//�C���N���[�h�t�@�C��
//*****************************************************************************
#include "pauseui.h"
#include "manager.h"

//*****************************************************************************
//�}�N��
//*****************************************************************************
#define MAX_GAMESERECT (3)

//*****************************************************************************
//�R���X�g���N�^
//*****************************************************************************
CPauseUI::CPauseUI(PRIORITY Priority) : CUI::CUI(Priority)
{

}

//*****************************************************************************
// �f�X�g���N�^
//*****************************************************************************
CPauseUI::~CPauseUI()
{

}

//*****************************************************************************
// ������
//***************************************************************************** 
HRESULT CPauseUI::Init(D3DXVECTOR3 pos, D3DXVECTOR2 size, int nType, D3DXCOLOR col, D3DXVECTOR3 rot)
{
	CScene2D::Init(pos);
	CScene2D::SetTexture(nType);	//�I�������ԍ��̃e�N�X�`����\��
	CScene2D::SetSize(size);
	CScene2D::ColorChange(col);
	CScene2D::AddRot(rot.y);
	CScene2D::Set();
	return S_OK;
}

//*****************************************************************************
// �I��
//***************************************************************************** 
void CPauseUI::Uninit()
{
	CUI::Uninit();
}

//*****************************************************************************
// �X�V
//***************************************************************************** 
void CPauseUI::Update()
{

}

//*****************************************************************************
//�`��
//***************************************************************************** 
void CPauseUI::Draw()
{
	if (CManager::GetPause() == true)
	{
		CUI::Draw();
	}
}

//*****************************************************************************
//�쐬
//***************************************************************************** 
CPauseUI *CPauseUI::Create(D3DXVECTOR3 pos, D3DXVECTOR2 size, int nType, D3DXCOLOR col, D3DXVECTOR3 rot)
{
	CPauseUI *pPauseUI = NULL;
	pPauseUI = new CPauseUI(PRIORITY_PAUSEUI);	//�������m��
												//NULL�`�F�b�N
	if (pPauseUI != NULL)
	{
		pPauseUI->Init(pos, size, nType, col, rot);
	}

	return pPauseUI;
}