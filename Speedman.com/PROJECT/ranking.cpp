//=============================================================================
// ランキング管理[ranking.cpp]
// Author : satou syusuke
//=============================================================================
#include "ranking.h"

//=============================================================================
// マクロ
//=============================================================================
#define RANK_TEXT "data/Rank.txt"	//ランキングファイル

//*****************************************************************************
//静的メンバ変数
//*****************************************************************************
int CRanking::m_nRank[MAX_RANKING] = {};	//ランキングのすうじ格納
int CRanking::m_Score = 0;					//今回スコア

//*****************************************************************************
//コンストラクタ
//*****************************************************************************
CRanking::CRanking()
{
	m_nRank[MAX_RANKING] = 0;
	m_Score = 0;
}

//*****************************************************************************
//デストラクタ
//*****************************************************************************
CRanking::~CRanking()
{

}

//*****************************************************************************
//ランキングの比較・セーブ
//*****************************************************************************
void CRanking::SaveRanking(int nScore)
{
	int nCntRank, nCntRank2, SubData;
	//int nCntNum = 0;		//何番目のランクか

							//ランキングロード
	LoadRanking();
	m_Score = nScore;

	//スコア比較
	for (nCntRank = 0; nCntRank < MAX_RANKING; nCntRank++)
	{
		for (nCntRank2 = nCntRank; nCntRank2 < MAX_RANKING; nCntRank2++)
		{
			if (m_nRank[nCntRank] < m_Score)
			{
				SubData = m_Score;
				m_Score = m_nRank[nCntRank];
				m_nRank[nCntRank] = SubData;
			}
		}
	}

	FILE *pFile = fopen(RANK_TEXT, "w");
	if (pFile != NULL)
	{
		for (int nCnt = 0; nCnt < MAX_RANKING; nCnt++)
		{
			fprintf(pFile, "RANK_NUMBER = %d\n", m_nRank[nCnt]);
		}
		fprintf(pFile, "\n");
		fprintf(pFile, "END_SCRIPT");
	}
	fclose(pFile);
}

//*****************************************************************************
//ランキングのロード
//*****************************************************************************
void CRanking::LoadRanking()
{
	int nCntNum = 0;		//何番目のランクか

							//ファイル読み込み
	char aFile[256];
	FILE *pFile = fopen(RANK_TEXT, "r");

	if (pFile != NULL)
	{
		while (true)
		{
			fscanf(pFile, "%s", &aFile[0]);

			if (strcmp(&aFile[0], "RANK_NUMBER") == 0) //TEXTURE_FILENAMEの文字列
			{
				fscanf(pFile, "%s", &aFile[0]);
				fscanf(pFile, "%d", &m_nRank[nCntNum]);
				nCntNum++;		//何番目のランクか

			}

			if (strcmp(&aFile[0], "END_SCRIPT") == 0) //END_SCRIPTの文字列を見つけたら
			{
				break;
			}
		}
	}
	fclose(pFile);

}

//*****************************************************************************
//ランキング取得
//*****************************************************************************
int CRanking::GetRank(int nNum)
{
	return m_nRank[nNum];
}