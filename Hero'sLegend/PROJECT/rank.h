//------------------------
//Author:三上航世
//ランキング処理(rank.h)
//------------------------
#ifndef _RANK_H_
#define _RANK_H_
#include "main.h"

//プロトタイプ宣言
void Rankmain(int nScore);	//メイン
void RankSave(void);		//書き込み
void RankLoad(int nScore);	//読み込み
void RankData(void);		//ランキング入れ替え
#endif _RANK_H_