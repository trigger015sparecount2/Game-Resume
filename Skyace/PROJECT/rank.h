//------------------------
//Author:三上航世
//ランキング処理(rank.h)
//------------------------
#ifndef _RANK_H_
#define _RANK_H_
#include "main.h"
//プロトタイプ宣言
void Rankmain(int nScore,int nKill);
void RankSave(void);
void RankLoad(int nScore, int nKill);
void RankData(void);
int SRankData(int nSRank);
int KRankData(int nKRank);
#endif _RANK_H_
