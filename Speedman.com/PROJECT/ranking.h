//=============================================================================
// ランキング管理[Ranking.h]
// Author : satou syusuke
//=============================================================================
#ifndef _RANKING_H_
#define _RANKING_H_

#include "main.h"


//=============================================================================
// マクロ
//=============================================================================
#define MAX_RANKING (5)	//順位

class CRanking
{
public:
	CRanking();
	~CRanking();

	static void SaveRanking(int nScore);	//ランキングの比較・セーブ
	static void LoadRanking();				//ランキングの読み込み

	static int GetRank(int nNum);			//引数番目の番号を返す
private:

	static int m_nRank[MAX_RANKING];		//ランキングのすうじ格納
	static int m_Score;						//今回スコア
};

#endif // !_RANKING_H_
