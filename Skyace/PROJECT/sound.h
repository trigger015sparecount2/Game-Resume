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
	SOUND_LABEL_BGM001,			// ƒ{ƒXBGM
	SOUND_LABEL_BGM002,			// ƒ{ƒXBGM
	SOUND_LABEL_BGM003,			// ƒ{ƒXBGM
	SOUND_LABEL_BGM004,			// “¹’†BGM
	SOUND_LABEL_BGM005,			// “¹’†BGM
	SOUND_LABEL_BGM006,			// “¹’†BGM
	SOUND_LABEL_SHOT000,		// ƒVƒ‡ƒbƒg‰¹
	SOUND_LABEL_BARRIER000,		// ƒoƒŠƒA‰¹
	SOUND_LABEL_CANCEL000,		// ƒLƒƒƒ“ƒZƒ‹‰¹
	SOUND_LABEL_ENEMYDOWN000,	// “G‹@Œ‚’Ä‰¹
	SOUND_LABEL_EXPLOSION000,	// ‘å”š”­‰¹
	SOUND_LABEL_LASER000,		// ƒŒ[ƒU[‰¹
	SOUND_LABEL_MISSILE000,		// ƒ~ƒTƒCƒ‹”š”­‰¹
	SOUND_LABEL_PAUSE000,		// ƒ|[ƒY‰¹
	SOUND_LABEL_RAILGUN000,		// ƒŒ[ƒ‹ƒKƒ“‰¹
	SOUND_LABEL_YES000,		    // Œˆ’è‰¹
	SOUND_LABEL_CHOICE000,
	SOUND_LABEL_ITEM000,
	SOUND_LABEL_PINCH000,
	SOUND_LABEL_DAMAGE000,
	SOUND_LABEL_MAX,
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
