//=============================================================================
//
// スラックラインの支柱処理 [slack.h]
// Author : 三上航世
//
//=============================================================================
#ifndef _SLACK_H_
#define _SLACK_H_

#include "main.h"
#include "object.h"

class CModel;

class CSlack : public CObject
{
public:
	CSlack(int nPriority);
	~CSlack();

	HRESULT Init(D3DXVECTOR3 pos, int nType, COLLISION collision, D3DXVECTOR3 rot, int nSlack);	//初期化
	void Uninit();	//終了
	void Update();	//更新
	void Draw();	//描画

	static CSlack *Create(D3DXVECTOR3 pos, int nType, COLLISION collision, D3DXVECTOR3 rot, int nSlack);	//作成
private:

};

#endif