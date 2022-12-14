//＋＋＋＋＋＋＋＋＋＋＋＋＋＋＋＋＋＋＋＋＋＋＋＋＋＋＋＋＋＋＋＋＋＋＋＋＋＋
// サウンド処理 [sound.h]
// Author :三上航世
//＋＋＋＋＋＋＋＋＋＋＋＋＋＋＋＋＋＋＋＋＋＋＋＋＋＋＋＋＋＋＋＋＋＋＋＋＋＋
#ifndef _SOUND_H_
#define _SOUND_H_

#include "main.h"

//=================================
// サウンドファイル
//=================================
typedef enum
{
	SOUND_LABEL_BGM000 = 0,		// タイトルBGM
	SOUND_LABEL_BGM001,			// ステージBGM
	SOUND_LABEL_BGM002,			// 成功BGM
	SOUND_LABEL_BGM003,			// 失敗BGM
	SOUND_LABEL_FOOTSTEP,		//足音
	SOUND_LABEL_CANCEL,			// キャンセル音
	SOUND_LABEL_TARGETBREAK,	//的破壊音
	SOUND_LABEL_THROW,			////手裏剣投げ音
	SOUND_LABEL_CROSSBLOCK,		//手裏剣がブロックに刺さったときの音
	SOUND_LABEL_WARP,			//ワープ音
	SOUND_LABEL_PAUSE,			// ポーズ音
	SOUND_LABEL_YES,			// 決定音
	SOUND_LABEL_JUMP,			// ジャンプ音
	SOUND_LABEL_BLOCKCLASH,		// ブロック激突音
	SOUND_LABEL_CHOICE,			//選択音
	SOUNT_LABEL_COUNTDOWN,		//時間が少ないとき
	SOUND_LABEL_MAX
} SOUND_LABEL;

//=============================================================================
// プロトタイプ宣言
//=============================================================================
HRESULT InitSound(HWND hWnd);
void UninitSound(void);
HRESULT PlaySound(SOUND_LABEL label);
void StopSound(SOUND_LABEL label);
void StopSound(void);

#endif
