//{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{
// TEh [sound.cpp]
// Author :Oãq¢
//{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{
#include "sound.h"

//=============================================================================
// p[^\¢Ìè`
//=============================================================================
typedef struct
{
	char *filename;		// t@C¼
	int nCntLoop;		// [vJEg
} PARAM;

//=============================================================================
// vg^Cvé¾
//=============================================================================
HRESULT CheckChunk(HANDLE hFile, DWORD format, DWORD *pChunkSize, DWORD *pChunkDataPosition);
HRESULT ReadChunkData(HANDLE hFile, void *pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset);

//=============================================================================
// O[oÏ
//=============================================================================
IXAudio2 *g_pXAudio2 = NULL;								// XAudio2IuWFNgÖÌC^[tFCX
IXAudio2MasteringVoice *g_pMasteringVoice = NULL;			// }X^[{CX
IXAudio2SourceVoice *g_apSourceVoice[SOUND_LABEL_MAX] = {};	// \[X{CX
BYTE *g_apDataAudio[SOUND_LABEL_MAX] = {};					// I[fBIf[^
DWORD g_aSizeAudio[SOUND_LABEL_MAX] = {};					// I[fBIf[^TCY

// e¹fÞÌp[^
PARAM g_aParam[SOUND_LABEL_MAX] =
{
	{ "data/BGM/title.wav", -1 },			// ^CgæÊ
	{ "data/BGM/game.wav", -1 },			// Xe[WBGM
	{ "data/BGM/congratulations.wav", 0 },	// ¬÷BGM
	{ "data/BGM/failed.wav", 0 },			// ¸sBGM
	{ "data/SE/footstep.wav", 0 },			// «¹
	{ "data/SE/cancel.wav", 0 },			// LZ¹
	{ "data/SE/targetbreak.wav", 0 },		// Ijó¹
	{ "data/SE/throw.wav", 0 },				// è °¹
	{ "data/SE/crossblock.wav", 0 },		// è ªubNÉ½Á½¹
	{ "data/SE/warp.wav", 0 },				// [v¹
	{ "data/SE/pause.wav", 0 },				// |[Y¹
	{ "data/SE/yes.wav", 0 },				// è¹
	{ "data/SE/jump.wav", 0 },				// Wv¹
	{ "data/SE/blockclash.wav", 0 },		// ubNË¹
	{ "data/SE/choice.wav", 0 },			// Ið¹
	{ "data/SE/countdown.wav", 0 }			// cèÔª­È¢Æ«
};

//=============================================================================
// ú»
//=============================================================================
HRESULT InitSound(HWND hWnd)
{
	HRESULT hr;

	// COMCuÌú»
	CoInitializeEx(NULL, COINIT_MULTITHREADED);

	// XAudio2IuWFNgÌì¬
	hr = XAudio2Create(&g_pXAudio2, 0);
	if (FAILED(hr))
	{
		// COMCuÌI¹
		CoUninitialize();
		return E_FAIL;
	}

	// }X^[{CXÌ¶¬
	hr = g_pXAudio2->CreateMasteringVoice(&g_pMasteringVoice);
	if (FAILED(hr))
	{
		if (g_pXAudio2)
		{
			// XAudio2IuWFNgÌJú
			g_pXAudio2->Release();
		}

		// COMCuÌI¹
		CoUninitialize();
		return E_FAIL;
	}

	// TEhf[^Ìú»
	for (int nCntSound = 0; nCntSound < SOUND_LABEL_MAX; nCntSound++)
	{
		HANDLE hFile;
		DWORD dwChunkSize = 0;
		DWORD dwChunkPosition = 0;
		DWORD filetype;
		WAVEFORMATEXTENSIBLE wfx;
		XAUDIO2_BUFFER buffer;

		// obt@ÌNA
		memset(&wfx, 0, sizeof(WAVEFORMATEXTENSIBLE));
		memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));

		// TEhf[^t@CÌ¶¬
		hFile = CreateFile(g_aParam[nCntSound].filename, GENERIC_READ,
			FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
		if (hFile == INVALID_HANDLE_VALUE)
		{
			return HRESULT_FROM_WIN32(GetLastError());
		}
		if (SetFilePointer(hFile, 0, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
		{
			return HRESULT_FROM_WIN32(GetLastError());
		}

		// WAVEt@CÌ`FbN
		hr = CheckChunk(hFile, 'FFIR', &dwChunkSize, &dwChunkPosition);
		if (FAILED(hr))
		{
			return S_FALSE;
		}
		hr = ReadChunkData(hFile, &filetype, sizeof(DWORD), dwChunkPosition);
		if (FAILED(hr))
		{
			return S_FALSE;
		}
		if (filetype != 'EVAW')
		{
			return S_FALSE;
		}

		// tH[}bg`FbN
		hr = CheckChunk(hFile, ' tmf', &dwChunkSize, &dwChunkPosition);
		if (FAILED(hr))
		{
			return S_FALSE;
		}
		hr = ReadChunkData(hFile, &wfx, dwChunkSize, dwChunkPosition);
		if (FAILED(hr))
		{
			return S_FALSE;
		}

		// I[fBIf[^ÇÝÝ
		hr = CheckChunk(hFile, 'atad', &g_aSizeAudio[nCntSound], &dwChunkPosition);
		if (FAILED(hr))
		{
			return S_FALSE;
		}
		g_apDataAudio[nCntSound] = (BYTE*)malloc(g_aSizeAudio[nCntSound]);
		hr = ReadChunkData(hFile, g_apDataAudio[nCntSound], g_aSizeAudio[nCntSound], dwChunkPosition);
		if (FAILED(hr))
		{
			return S_FALSE;
		}

		g_pXAudio2->CreateSourceVoice(&g_apSourceVoice[nCntSound], &(wfx.Format));

		// obt@ÌÝè
		memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));
		buffer.AudioBytes = g_aSizeAudio[nCntSound];
		buffer.pAudioData = g_apDataAudio[nCntSound];
		buffer.Flags = XAUDIO2_END_OF_STREAM;
		buffer.LoopCount = g_aParam[nCntSound].nCntLoop;

		g_apSourceVoice[nCntSound]->SubmitSourceBuffer(&buffer);

		// t@CðN[Y
		CloseHandle(hFile);
	}

	return S_OK;
}

//=============================================================================
// I¹
//=============================================================================
void UninitSound(void)
{
	// êâ~
	for (int nCntSound = 0; nCntSound < SOUND_LABEL_MAX; nCntSound++)
	{
		if (g_apSourceVoice[nCntSound] != NULL)
		{
			// êâ~
			g_apSourceVoice[nCntSound]->Stop(0);

			// \[X{CXÌjü
			g_apSourceVoice[nCntSound]->DestroyVoice();

			// I[fBIf[^ÌJú
			free(g_apDataAudio[nCntSound]);
		}
	}

	// }X^[{CXÌjü
	g_pMasteringVoice->DestroyVoice();

	if (g_pXAudio2)
	{
		// XAudio2IuWFNgÌJú
		g_pXAudio2->Release();
	}

	// COMCuÌI¹
	CoUninitialize();
}

//=============================================================================
// ZOgÄ¶(â~)
//=============================================================================
HRESULT PlaySound(SOUND_LABEL label)
{
	XAUDIO2_VOICE_STATE xa2state;
	XAUDIO2_BUFFER       buffer;

	// obt@ÌÝè
	memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));
	buffer.AudioBytes = g_aSizeAudio[label];
	buffer.pAudioData = g_apDataAudio[label];
	buffer.Flags = XAUDIO2_END_OF_STREAM;
	buffer.LoopCount = g_aParam[label].nCntLoop;

	// óÔæ¾
	g_apSourceVoice[label]->GetState(&xa2state);
	if (xa2state.BuffersQueued != 0)
	{// Ä¶
	 // êâ~
		g_apSourceVoice[label]->Stop(0);

		// NA
		g_apSourceVoice[label]->FlushSourceBuffers();
	}

	// o^
	g_apSourceVoice[label]->SubmitSourceBuffer(&buffer);

	// Ä¶
	g_apSourceVoice[label]->Start(0);

	return S_OK;
}

//=============================================================================
// ZOgâ~
//=============================================================================
void StopSound(SOUND_LABEL label)
{
	XAUDIO2_VOICE_STATE xa2state;

	// óÔæ¾
	g_apSourceVoice[label]->GetState(&xa2state);
	if (xa2state.BuffersQueued != 0)
	{// Ä¶
	 // êâ~
		g_apSourceVoice[label]->Stop(0);

		// NA
		g_apSourceVoice[label]->FlushSourceBuffers();
	}
}

//=============================================================================
// ZOgâ~
//=============================================================================
void StopSound(void)
{
	for (int nCntSound = 0; nCntSound < SOUND_LABEL_MAX; nCntSound++)
	{
		if (g_apSourceVoice[nCntSound] != NULL)
		{
			// êâ~
			g_apSourceVoice[nCntSound]->Stop(0);

			// NA
			g_apSourceVoice[nCntSound]->FlushSourceBuffers();
		}
	}
}

//=============================================================================
// `NÌ`FbN
//=============================================================================
HRESULT CheckChunk(HANDLE hFile, DWORD format, DWORD *pChunkSize, DWORD *pChunkDataPosition)
{
	HRESULT hr = S_OK;
	DWORD dwRead;
	DWORD dwChunkType;
	DWORD dwChunkDataSize;
	DWORD dwRIFFDataSize = 0;
	DWORD dwFileType;
	DWORD dwBytesRead = 0;
	DWORD dwOffset = 0;

	if (SetFilePointer(hFile, 0, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
	{// t@C|C^ðæªÉÚ®
		return HRESULT_FROM_WIN32(GetLastError());
	}

	while (hr == S_OK)
	{
		if (ReadFile(hFile, &dwChunkType, sizeof(DWORD), &dwRead, NULL) == 0)
		{// `NÌÇÝÝ
			hr = HRESULT_FROM_WIN32(GetLastError());
		}

		if (ReadFile(hFile, &dwChunkDataSize, sizeof(DWORD), &dwRead, NULL) == 0)
		{// `Nf[^ÌÇÝÝ
			hr = HRESULT_FROM_WIN32(GetLastError());
		}

		switch (dwChunkType)
		{
		case 'FFIR':
			dwRIFFDataSize = dwChunkDataSize;
			dwChunkDataSize = 4;
			if (ReadFile(hFile, &dwFileType, sizeof(DWORD), &dwRead, NULL) == 0)
			{// t@C^CvÌÇÝÝ
				hr = HRESULT_FROM_WIN32(GetLastError());
			}
			break;

		default:
			if (SetFilePointer(hFile, dwChunkDataSize, NULL, FILE_CURRENT) == INVALID_SET_FILE_POINTER)
			{// t@C|C^ð`Nf[^ªÚ®
				return HRESULT_FROM_WIN32(GetLastError());
			}
		}

		dwOffset += sizeof(DWORD) * 2;
		if (dwChunkType == format)
		{
			*pChunkSize = dwChunkDataSize;
			*pChunkDataPosition = dwOffset;

			return S_OK;
		}

		dwOffset += dwChunkDataSize;
		if (dwBytesRead >= dwRIFFDataSize)
		{
			return S_FALSE;
		}
	}

	return S_OK;
}

//=============================================================================
// `Nf[^ÌÇÝÝ
//=============================================================================
HRESULT ReadChunkData(HANDLE hFile, void *pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset)
{
	DWORD dwRead;

	if (SetFilePointer(hFile, dwBufferoffset, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
	{// t@C|C^ðwèÊuÜÅÚ®
		return HRESULT_FROM_WIN32(GetLastError());
	}

	if (ReadFile(hFile, pBuffer, dwBuffersize, &dwRead, NULL) == 0)
	{// f[^ÌÇÝÝ
		return HRESULT_FROM_WIN32(GetLastError());
	}

	return S_OK;
}