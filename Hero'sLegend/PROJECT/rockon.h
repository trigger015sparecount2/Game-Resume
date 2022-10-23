//---------------------------
//Author:�O��q��
//���b�N�I��(rockon.h)
//---------------------------
#ifndef _ROCKON_H_
#define _ROCKON_H_
#include "main.h"
typedef struct
{
	D3DXVECTOR3 pos; //�ʒu
	D3DXMATRIX mtxWorld; //���[���h�}�g���b�N�X
	int nNumber;
	bool bUse;
} Rockon;
//�v���g�^�C�v�錾
HRESULT InitRockon(void);
void UninitRockon(void);
void UpdateRockon(void);
void DrawRockon(void);
void SetRockon(D3DXVECTOR3 pos, int nNumber);
void MoveRockon(D3DXVECTOR3 pos);
int RemoveRockon(void);
int RockNumber(void);
#endif _ROCKON_H_