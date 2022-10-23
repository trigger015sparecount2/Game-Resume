//------------------------
//Author:三上航世
//ランキング処理(rank.h)
//------------------------
#ifndef _RANK_H_
#define _RANK_H_
#include "main.h"
//プロトタイプ宣言
void Rankmain(int nTimeMin, int nTimeSec, bool bDemo);
void RankSave(void);
void RankLoad(void);
void RankData(int nTimeMin, int nTimeSec, bool bDemo);
int TRankData(int nTRank);
#endif _RANK_H_