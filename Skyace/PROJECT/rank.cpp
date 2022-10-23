//-------------------------
//Autohr;三上航世
//ランキング降順(main.cpp)
//-------------------------
#include "rank.h"

//マクロ定義
#define MAX_RANK (5)
#define SAVEFILENAME "shootingrank.txt"

//グローバル変数
int g_aScore[MAX_RANK];
int g_aKill[MAX_RANK];

void Rankmain(int nScore,int nKill)
{
	RankLoad(nScore,nKill);
	RankData();
	RankSave();
}

void RankSave(void)
{
	FILE *pFile;
	pFile = fopen(SAVEFILENAME, "w");
	int nCount;
	if (pFile != NULL)
	{
		for (nCount = 0; nCount < MAX_RANK; nCount++)
		{
			fprintf(pFile, "%d\n", g_aScore[nCount]);
		}
		for (nCount = 0; nCount < MAX_RANK; nCount++)
		{
			fprintf(pFile, "%d\n", g_aKill[nCount]);
		}
		fclose(pFile);
	}
}

void RankLoad(int nScore, int nKill)
{
	FILE *pFile;
	int nCount;
	pFile = fopen(SAVEFILENAME, "r");
	if (pFile != NULL)
	{
		for (nCount = 0; nCount < MAX_RANK; nCount++)
		{
			fscanf(pFile, "%d", &g_aScore[nCount]);
		}
		for (nCount = 0; nCount < MAX_RANK; nCount++)
		{
			fscanf(pFile, "%d", &g_aKill[nCount]);
		}
		fclose(pFile);
	}
	if (g_aScore[MAX_RANK - 1] < nScore)
	{
		g_aScore[MAX_RANK - 1] = nScore;
	}
	if (g_aKill[MAX_RANK - 1] < nKill)
	{
		g_aKill[MAX_RANK - 1] = nKill;
	}
}

void RankData(void)
{
	int nCount1;
	int nCount2;
	int nRank = 0;
	int ntScore = 0;
	int ntKill = 0;
	int nSamerank = 0;
	//スコア並び替え
	for (nCount1 = 0; nCount1 < MAX_RANK - 1; nCount1++)
	{
		for (nCount2 = nCount1 + 1; nCount2 < MAX_RANK; nCount2++)
		{
			if (g_aScore[nCount1] < g_aScore[nCount2])
			{
				nRank = g_aScore[nCount1];
				g_aScore[nCount1] = g_aScore[nCount2];
				g_aScore[nCount2] = nRank;
			}
		}
	}
	//撃墜数並び替え
	nRank = 0;
	for (nCount1 = 0; nCount1 < MAX_RANK - 1; nCount1++)
	{
		for (nCount2 = nCount1 + 1; nCount2 < MAX_RANK; nCount2++)
		{
			if (g_aKill[nCount1] < g_aKill[nCount2])
			{
				nRank = g_aKill[nCount1];
				g_aKill[nCount1] = g_aKill[nCount2];
				g_aKill[nCount2] = nRank;
			}
		}
	}
}

int SRankData(int nSRank)
{
	return g_aScore[nSRank];
}

int KRankData(int nKRank)
{
	return g_aKill[nKRank];
}