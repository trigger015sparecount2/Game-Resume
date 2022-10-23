//＋＋＋＋＋＋＋＋＋＋＋＋＋＋＋＋＋＋＋＋＋＋＋＋＋＋＋＋＋＋＋＋＋＋＋＋＋＋
// サウンド処理 [sound.h]
// Author :三上航世
//＋＋＋＋＋＋＋＋＋＋＋＋＋＋＋＋＋＋＋＋＋＋＋＋＋＋＋＋＋＋＋＋＋＋＋＋＋＋
#ifndef _SOUND_H_
#define _SOUND_H_

#include "main.h"

class CSound
{
public:
	typedef enum
	{
		SOUND_LABEL_TITLEBGM = 0,	// タイトルBGM
		//SOUND_LABEL_TUTORIALBGM,	// チュートリアル
		SOUND_LABEL_GAMEBGM,		// 道中
		SOUND_LABEL_RESULTBGM,		// リザルト画面
		SOUND_LABEL_CANCEL,			// キャンセル音
		SOUND_LABEL_CHEERS,			// 歓声
		SOUND_LABEL_COUNTDOWN,		// カウントダウン
		SOUND_LABEL_SELECT,			// 選択音
		SOUND_LABEL_SIREN,			// 警告音
		SOUND_LABEL_SIRENCANCEL,	// 
		SOUND_LABEL_CLICK,			// 決定音
		SOUND_LABEL_GAMEOVER,		// ゲームオーバーSE
		SOUND_LABEL_PAUSE,			// ポーズ解除SE
		SOUND_LABEL_COUNTDOWNLAST,	// カウントダウンラストSE
		SOUND_LABEL_FALL,			//落下音
		SOUND_LABEL_GRAB,			//掴む音
		SOUND_LABEL_JUMP,			//ジャンプ
		SOUND_LABEL_JUMPPAD,		//ジャンプパッド
		SOUND_LABEL_CHECKPOINT,		//チェックポイント
		SOUND_LABEL_LANDING,		//着地

		SOUND_LABEL_MAX
	} SOUND_LABEL;

	CSound();
	~CSound();

	static HRESULT Init(HWND hWnd);
	static void Uninit();
	static HRESULT Play(SOUND_LABEL label);
	static void Stop(SOUND_LABEL label);
	static void Stop();

	//音量調整
	static void ControlVoice(SOUND_LABEL label, float fVolume)
	{
		m_apSourceVoice[label]->SetVolume(fVolume);
	}

private:
	typedef struct
	{
		char *filename;		// ファイル名
		int nCntLoop;		// ループカウント
	} PARAM;
	static HRESULT CheckChunk(HANDLE hFile, DWORD format, DWORD *pChunkSize, DWORD *pChunkDataPosition);
	static HRESULT ReadChunkData(HANDLE hFile, void *pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset);
	static IXAudio2 *m_pXAudio2;									// XAudio2オブジェクトへのインターフェイス
	static IXAudio2MasteringVoice *m_pMasteringVoice;				// マスターボイス
	static IXAudio2SourceVoice *m_apSourceVoice[SOUND_LABEL_MAX];	// ソースボイス
	static BYTE *m_apDataAudio[SOUND_LABEL_MAX];					// オーディオデータ
	static DWORD m_aSizeAudio[SOUND_LABEL_MAX];						// オーディオデータサイズ

																	// 各音素材のパラメータ
	static PARAM m_aParam[SOUND_LABEL_MAX];
};
#endif
