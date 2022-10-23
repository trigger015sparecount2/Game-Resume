//*****************************************************************************
// スコア処理 [Score.h]
// Author : satou syusuke
//*****************************************************************************

#ifndef _SCORE_H_
#define _SCORE_H_

#include "Scene.h"

#define MAX_SCORE (10)

class CScene2D;
class CScore : public CScene
{
public:

	typedef enum
	{
		NUMBER_LIFE,	//ライフ表示
		NUMBER_TIME,	//最終スコア
		NUMBER_RANKING,		//ランキング
		NUMBER_SCORE,		//最終スコア
		NUMBER_MAX,
	} NUMBER_TYPE;		//表示する数字


	CScore();
	~CScore();

	HRESULT Init(D3DXVECTOR3 pos);
	void Uninit();
	void Update();
	void Draw();

	//ゲット・セット関数
	void SetScore(int nScore);
	void AddScore(int nValue);
	int GetScore();

	//桁数
	void SetDigit(int nDigit);
	int GetDigit();

	//テクスチャ番号
	void SetTexNum(int nNumtex);
	int GetTexNum();

	//大きさX
	void SetSizeX(float nSizeX);
	float GetSizeX();

	//大きさY
	void SetSizeY(float nSizeY);
	float GetSizeY();

	//表示する数字
	void SetNumtype(CScore::NUMBER_TYPE Numtype);
	CScore::NUMBER_TYPE GetNumtype();

	//何位のランク数字
	void SetRankNum(int nRankNum);
	int GetRankNum();

	//何位かリセット
	static void SetRankReset(void);

	//タイマー関連
	static void SetTime(int nNum);	//タイマーセット
	static int GetTime();		//タイマー取得

	static CScore * Create(D3DXVECTOR3 pos, float SizeX, float SizeY, int nType, int nDigit, CScore::NUMBER_TYPE Numtype);		//出現位置、サイズX、サイズY、テクスチャ、桁数、番号タイプ

private:
	CScene2D *m_apScene[MAX_SCORE];
	int m_aScoreNum[MAX_SCORE];

	int m_nScore;		//現在のスコア
	int m_AddScore;		//徐々に増やしていくやつ

	int m_Num;			//表示する数字

	int m_nDigit;		//桁数

	float m_fSizeX;
	float m_fSizeY;

	int m_nNumtex;

	CScore::NUMBER_TYPE m_aNumbertype;	//何の番号か

	bool m_bUninit;		//数字並びを消す
	static int m_nTime;			//時間
	static int m_nTimeCalc;		//フレーム計算用

	static int m_nNumRank;	//何番目か指定するために用意
	int m_RankNum;		//何番目のランクかどうか
};
#endif // !_SCORE_H_
