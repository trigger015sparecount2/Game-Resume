//------------------------
//Author:三上航世
//ランキング処理(rank.h)
//------------------------
#ifndef _RANK_H_
#define _RANK_H_
#include "main.h"

//プロトタイプ宣言
void Rankmain(int nTimeSec, int nTimeCom, bool bDemo);	//メイン処理
void RankSave(void);	//セーブ
void RankLoad(void);	//読み込み
void RankData(int nTimeSec, int nTimeCom, bool bDemo);	//ランキング入れ替え
int TRankData(int nTRank);	//各ランクの数字取得
#endif _RANK_H_