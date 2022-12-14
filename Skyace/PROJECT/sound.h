//{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{
// TEh [sound.h]
// Author :Oãq¢
//{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{
#ifndef _SOUND_H_
#define _SOUND_H_

#include "main.h"

//=================================
// TEht@C
//=================================
typedef enum
{
	SOUND_LABEL_BGM000 = 0,		// ^CgBGM
	SOUND_LABEL_BGM001,			// {XBGM
	SOUND_LABEL_BGM002,			// {XBGM
	SOUND_LABEL_BGM003,			// {XBGM
	SOUND_LABEL_BGM004,			// ¹BGM
	SOUND_LABEL_BGM005,			// ¹BGM
	SOUND_LABEL_BGM006,			// ¹BGM
	SOUND_LABEL_SHOT000,		// Vbg¹
	SOUND_LABEL_BARRIER000,		// oA¹
	SOUND_LABEL_CANCEL000,		// LZ¹
	SOUND_LABEL_ENEMYDOWN000,	// G@Ä¹
	SOUND_LABEL_EXPLOSION000,	// å­¹
	SOUND_LABEL_LASER000,		// [U[¹
	SOUND_LABEL_MISSILE000,		// ~TC­¹
	SOUND_LABEL_PAUSE000,		// |[Y¹
	SOUND_LABEL_RAILGUN000,		// [K¹
	SOUND_LABEL_YES000,		    // è¹
	SOUND_LABEL_CHOICE000,
	SOUND_LABEL_ITEM000,
	SOUND_LABEL_PINCH000,
	SOUND_LABEL_DAMAGE000,
	SOUND_LABEL_MAX,
} SOUND_LABEL;

//=============================================================================
// vg^Cvé¾
//=============================================================================
HRESULT InitSound(HWND hWnd);
void UninitSound(void);
HRESULT PlaySound(SOUND_LABEL label);
void StopSound(SOUND_LABEL label);
void StopSound(void);

#endif
