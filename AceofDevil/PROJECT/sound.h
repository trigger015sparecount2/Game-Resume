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
		SOUND_LABEL_TUTORIALBGM,    // チュートリアル
		SOUND_LABEL_GAMEBGM,        // 道中
		SOUND_LABEL_BOSSBGM,        // ボス戦
		SOUND_LABEL_RESULTBGM,      // リザルト画面
		SOUND_LABEL_SHATDOWNBGM,	// 自機かボス撃墜
		SOUND_LABEL_PLAYERDAMAGE,	// 自機ダメージ
		SOUND_LABEL_CANCEL,         // キャンセル音
		SOUND_LABEL_CHOICE,         // 選択音
		SOUND_LABEL_ENEMYDAMAGE,    // 敵のダメージ
		SOUND_LABEL_ENEMYSHATDOWN,  // 敵機撃墜
		SOUND_LABEL_FIRE,           // 火の弾
		SOUND_LABEL_GUN,            // 敵弾
		SOUND_LABEL_ICE,            // 氷弾
		SOUND_LABEL_LIGHTNING,      // 電撃
		SOUND_LABEL_MAGICCHANGE,    // 属性切り替え
		SOUND_LABEL_PAUSE,          // ポーズ音
		SOUND_LABEL_PINCH,          // ピンチ
		SOUND_LABEL_PRIZE,          // プライズ取得音
		SOUND_LABEL_THUNDER,        // 雷弾
		SOUND_LABEL_WATER,          // 水レーザー
		SOUND_LABEL_WIND,           // 風のバリア
		SOUND_LABEL_YES,            // 決定音
		SOUND_LABEL_MAX
	} SOUND_LABEL;
	CSound();
	~CSound();
	static HRESULT Init(HWND hWnd);			//初期化
	static void Uninit(void);				//終了
	static HRESULT Play(SOUND_LABEL label);	//再生
	static void Stop(SOUND_LABEL label);	//停止
	static void Stop(void);					//全停止

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
