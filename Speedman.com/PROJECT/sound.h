//{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{
// TEh [sound.h]
// Author :Oãq¢
//{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{
#ifndef _SOUND_H_
#define _SOUND_H_

#include "main.h"

class CSound
{
public:
	typedef enum
	{
		SOUND_LABEL_TITLEBGM = 0,	// ^CgBGM
		//SOUND_LABEL_TUTORIALBGM,	// `[gA
		SOUND_LABEL_GAMEBGM,		// ¹
		SOUND_LABEL_RESULTBGM,		// UgæÊ
		SOUND_LABEL_CANCEL,			// LZ¹
		SOUND_LABEL_CHEERS,			// ½º
		SOUND_LABEL_COUNTDOWN,		// JEg_E
		SOUND_LABEL_SELECT,			// Ið¹
		SOUND_LABEL_SIREN,			// x¹
		SOUND_LABEL_SIRENCANCEL,	// 
		SOUND_LABEL_CLICK,			// è¹
		SOUND_LABEL_GAMEOVER,		// Q[I[o[SE
		SOUND_LABEL_PAUSE,			// |[YðSE
		SOUND_LABEL_COUNTDOWNLAST,	// JEg_EXgSE
		SOUND_LABEL_FALL,			//º¹
		SOUND_LABEL_GRAB,			//ÍÞ¹
		SOUND_LABEL_JUMP,			//Wv
		SOUND_LABEL_JUMPPAD,		//Wvpbh
		SOUND_LABEL_CHECKPOINT,		//`FbN|Cg
		SOUND_LABEL_LANDING,		//n

		SOUND_LABEL_MAX
	} SOUND_LABEL;

	CSound();
	~CSound();

	static HRESULT Init(HWND hWnd);
	static void Uninit();
	static HRESULT Play(SOUND_LABEL label);
	static void Stop(SOUND_LABEL label);
	static void Stop();

	//¹Ê²®
	static void ControlVoice(SOUND_LABEL label, float fVolume)
	{
		m_apSourceVoice[label]->SetVolume(fVolume);
	}

private:
	typedef struct
	{
		char *filename;		// t@C¼
		int nCntLoop;		// [vJEg
	} PARAM;
	static HRESULT CheckChunk(HANDLE hFile, DWORD format, DWORD *pChunkSize, DWORD *pChunkDataPosition);
	static HRESULT ReadChunkData(HANDLE hFile, void *pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset);
	static IXAudio2 *m_pXAudio2;									// XAudio2IuWFNgÖÌC^[tFCX
	static IXAudio2MasteringVoice *m_pMasteringVoice;				// }X^[{CX
	static IXAudio2SourceVoice *m_apSourceVoice[SOUND_LABEL_MAX];	// \[X{CX
	static BYTE *m_apDataAudio[SOUND_LABEL_MAX];					// I[fBIf[^
	static DWORD m_aSizeAudio[SOUND_LABEL_MAX];						// I[fBIf[^TCY

																	// e¹fÞÌp[^
	static PARAM m_aParam[SOUND_LABEL_MAX];
};
#endif
