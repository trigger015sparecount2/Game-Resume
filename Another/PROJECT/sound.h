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
		SOUND_LABEL_GAMEBGM,		// 道中
		SOUND_LABEL_ATTACK,			// プレイヤーの攻撃
		SOUND_LABEL_CANCEL,			// キャンセル音
		SOUND_LABEL_CLEAR,			// クリア音
		SOUND_LABEL_DAMAGE,			// 敵の攻撃
		SOUND_LABEL_DODGE,			// 回避
		SOUND_LABEL_ENEMYAPP,		// 敵出現時
		SOUND_LABEL_ENEMYDEATH,		// 敵消滅時
		SOUND_LABEL_FOOTSTEP,		// 足音
		SOUND_LABEL_GO,				// GOサイン
		SOUND_LABEL_GUARD,			// ガード時
		SOUND_LABEL_LANDING,		// 着地
		SOUND_LABEL_OK,				// 決定音
		SOUND_LABEL_PAUSE,			// ポーズ
		SOUND_LABEL_PINCH,			// ピンチ時
		SOUND_LABEL_PLAYERDEATH,	// プレイヤー死亡時
		SOUND_LABEL_READY,			// READYサイン
		SOUND_LABEL_SELECT,			// 選択音
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
