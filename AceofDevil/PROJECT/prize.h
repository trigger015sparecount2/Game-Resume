//=============================================================================
//
// プライズ処理 [prize.h]
// Author : 三上航世
//
//=============================================================================
#ifndef _PRIZE_H_
#define _PRIZE_H_

#include "main.h"
#include "scene2d.h"

#define PRIZE_SIZE (12.0f)

class CPrize : public CScene2D
{
public:
	CPrize(PRIORITY Priority = PRIORITY_BG);
	~CPrize();
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 move, ELEMENT element, int nHeal);	//初期化
	void Uninit(void);	//終了
	void Update(void);	//更新
	void Draw(void);	//描画
	bool GetDamage(void) { return false; }
	void Damage(int) { ; }

	static HRESULT Load(void);
	static void UnLoad(void);
	static CPrize *Create(D3DXVECTOR3 pos, D3DXVECTOR3 move, ELEMENT element, int nHeal);

private:
	D3DXVECTOR3 m_move;		//移動速度
	D3DXVECTOR3 *m_pPos;	//プレイヤーの場所のポインタ
	float m_fRotY;			//最初飛び散る方向
	float m_fSize;			//大きさ
	ELEMENT m_Element;		//属性
	static LPDIRECT3DTEXTURE9 m_pTexture;	//テクスチャ
	int m_nTime;			//生成されてからの時間
	int m_nHeal;			//どれだけ魔力を回復させるか
	bool m_bUninit;			//削除するか
};

#endif