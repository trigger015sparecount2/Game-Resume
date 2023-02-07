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
	SOUND_LABEL_BGM000 = 0,		//タイトルBGM
	SOUND_LABEL_BGM001,			//ボスBGM
	SOUND_LABEL_BGM002,			//ボスBGM
	SOUND_LABEL_BGM003,			//ボスBGM
	SOUND_LABEL_BGM004,			//道中BGM
	SOUND_LABEL_BGM005,			//道中BGM
	SOUND_LABEL_BGM006,			//道中BGM
	SOUND_LABEL_SHOT000,		//ショット音
	SOUND_LABEL_BARRIER000,		//バリア音
	SOUND_LABEL_CANCEL000,		//キャンセル音
	SOUND_LABEL_ENEMYDOWN000,	//敵機撃墜音
	SOUND_LABEL_EXPLOSION000,	//大爆発音
	SOUND_LABEL_LASER000,		//レーザー音
	SOUND_LABEL_MISSILE000,		//ミサイル爆発音
	SOUND_LABEL_PAUSE000,		//ポーズ音
	SOUND_LABEL_RAILGUN000,		//レールガン音
	SOUND_LABEL_YES000,		    //決定音
	SOUND_LABEL_CHOICE000,		//選択音
	SOUND_LABEL_ITEM000,		//アイテム取得
	SOUND_LABEL_PINCH000,		//ピンチ時
	SOUND_LABEL_DAMAGE000,		//ダメージ
	SOUND_LABEL_MAX,
} SOUND_LABEL;

//=============================================================================
// プロトタイプ宣言
//=============================================================================
HRESULT InitSound(HWND hWnd);			//初期化
void UninitSound(void);					//終了
HRESULT PlaySound(SOUND_LABEL label);	//再生
void StopSound(SOUND_LABEL label);		//1曲停止
void StopSound(void);					//全停止

#endif
