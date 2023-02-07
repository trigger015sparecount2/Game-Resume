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
		SOUND_LABEL_STAGE1BGM,		// 道中
		SOUND_LABEL_STAGE2BGM,		// 道中
		SOUND_LABEL_STAGE3BGM,		// 道中
		SOUND_LABEL_BOSS1BGM,		// ボス
		SOUND_LABEL_BOSS2BGM,		// ボス
		SOUND_LABEL_BOSS3BGM,		// ボス
		SOUND_LABEL_RESULTBGM,		// リザルト
		SOUND_LABEL_AIRCRAFTDAMAGE,	// プレイヤーのダメージ
		SOUND_LABEL_BOMB,			// 購入音
		SOUND_LABEL_BUY,			// ボム音
		SOUND_LABEL_CANCEL,			// キャンセル音
		SOUND_LABEL_CHOICE,			// 選択音
		SOUND_LABEL_ENEMYDAMAGE,	// 敵のダメージ
		SOUND_LABEL_ENEMYSHATDOWN,	// 敵消滅時
		SOUND_LABEL_GUN,			// 攻撃時
		SOUND_LABEL_PAUSE,			// ポーズ
		SOUND_LABEL_PINCH,			// ピンチ時
		SOUND_LABEL_RETURNSET,		// 巻き戻しセット
		SOUND_LABEL_RETURN,			// 巻き戻し
		SOUND_LABEL_YES,			// 決定音
		SOUND_LABEL_MAX
	} SOUND_LABEL;

	CSound();
	~CSound();

	static HRESULT Init(HWND hWnd);			//初期化
	static void Uninit();					//終了
	static HRESULT Play(SOUND_LABEL label);	//再生
	static void Stop(SOUND_LABEL label);	//選択した曲停止
	static void Stop();						//全停止

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
