//{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{
// ƒTƒEƒ“ƒhˆ— [sound.h]
// Author :ŽOãq¢
//{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{
#ifndef _SOUND_H_
#define _SOUND_H_

#include "main.h"

//=================================
// ƒTƒEƒ“ƒhƒtƒ@ƒCƒ‹
//=================================
typedef enum
{
	SOUND_LABEL_BGM000 = 0,		// ƒ^ƒCƒgƒ‹BGM
	SOUND_LABEL_BGM001,			// ƒXƒe[ƒWBGM
	SOUND_LABEL_BGM002,			// ¬Œ÷BGM
	SOUND_LABEL_BGM003,			// Ž¸”sBGM
	SOUND_LABEL_FOOTSTEP,		//‘«‰¹
	SOUND_LABEL_CANCEL,			// ƒLƒƒƒ“ƒZƒ‹‰¹
	SOUND_LABEL_TARGETBREAK,	//“I”j‰ó‰¹
	SOUND_LABEL_THROW,			////Žè— Œ•“Š‚°‰¹
	SOUND_LABEL_CROSSBLOCK,		//Žè— Œ•‚ªƒuƒƒbƒN‚ÉŽh‚³‚Á‚½‚Æ‚«‚Ì‰¹
	SOUND_LABEL_WARP,			//ƒ[ƒv‰¹
	SOUND_LABEL_PAUSE,			// ƒ|[ƒY‰¹
	SOUND_LABEL_YES,			// Œˆ’è‰¹
	SOUND_LABEL_JUMP,			// ƒWƒƒƒ“ƒv‰¹
	SOUND_LABEL_BLOCKCLASH,		// ƒuƒƒbƒNŒƒ“Ë‰¹
	SOUND_LABEL_CHOICE,			//‘I‘ð‰¹
	SOUNT_LABEL_COUNTDOWN,		//ŽžŠÔ‚ª­‚È‚¢‚Æ‚«
	SOUND_LABEL_MAX
} SOUND_LABEL;

//=============================================================================
// ƒvƒƒgƒ^ƒCƒvéŒ¾
//=============================================================================
HRESULT InitSound(HWND hWnd);
void UninitSound(void);
HRESULT PlaySound(SOUND_LABEL label);
void StopSound(SOUND_LABEL label);
void StopSound(void);

#endif
