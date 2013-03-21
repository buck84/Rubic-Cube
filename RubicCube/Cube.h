/*
 * =====================================================================
 *
 * FileName:    Cube.h
 * Author:      Buck
 * Email:       Buck88@gmail.com
 * Created:     2013.3.21
 * Environment: Visual Studio 2008
 *
 * Description: 魔方定义类
 *
 * ======================================================================
 */

#pragma once

#include <vector>
#include "global.h"

class CBlock;

class CCube
{
	struct RotateAnimInfo
	{
		int index;
		CBlock *pBlock;
	};

	struct RotateInfo
	{
		RotateType type;
		int index;
	};
public:
	CCube(void);
	~CCube(void);

	// rank: 几阶魔方
	// len: cube边长
	// speed: 多长时间转动90°，单位ms
	void Init(int rank=3, float len=6.0f, float speed=200);
	void Release();

	void Upset() { srand((unsigned int)time(NULL)); m_bUpseting=true; m_iUpsetNum=0; }
	void ExeFormula(CString str, int num);
	bool CanOpt() { return !m_bUpseting && m_vecRotateInfo.size()==0; }

	// 将魔方渲染信息添加到渲染缓存中
	void FillCubeVertexInfo(void *vertices, WORD *indices, int &vNum, int &iNum);

	// 开始播放转动动画，播放完成进行实际转动
	void StartRot(RotateType type, int index=0);

	// index:第几层块
	// 实际块转动函数
	void F(int index = 0);
	void B(int index = 0);
	void U(int index = 0);
	void D(int index = 0);
	void L(int index = 0);
	void R(int index = 0);

	// 定时执行的逻辑
	// 目前只是播放转动动画
	void Tick(int iTime);

	// 鼠标点击操作相关函数
	float GetLen() { return m_fLength; }
	int GetFIndex(float z);
	int GetBIndex(float z);
	int GetUIndex(float y);
	int GetDIndex(float y);
	int GetLIndex(float x);
	int GetRIndex(float x);

	int GetRank() { return m_iRank; }
	float GetSpeed() { return m_fSpeed; }

private:
	CBlock* GetBlock(int i, int j, int k);
	CBlock* ReplaceBlock(int i, int j, int k, CBlock *b);
	// 所在位置块是否正在转动，如果是则返回，否则返回空
	CBlock* GetRotBlock(int x, int y, int z);
private:

	/* 组成魔方的方块
	 * 里面方块指针为空，因为不需要显示
	 * 采用z轴正方向朝里，
	 * 前左下角为GetBlock(      0,      0,      0)
	 * 前右下角为GetBlock(m_iRank,      0,      0)
	 * 前右上角为GetBlock(m_iRank,m_iRank,      0)
	 * 后右上角为GetBlock(m_iRank,m_iRank,m_iRank)
	y     z
	|    /
	|   /
	|  /
	| /
	|/
	------------x
    0->

	左下(0, 0, 0)
	右上(m_iRank-1, m_iRank-1, m_iRank-1)
	*/
	CBlock		**m_pBlocks;
	int			m_iRank;		// 几阶魔方
	float		m_fLength;		// 魔方显示大小

	// 转动动画相关
	float		m_fSpeed;
	std::vector<RotateAnimInfo>	m_vecRotateAnimInfo;
	RotateType	m_iRotType;			// 转动哪一面
	int			m_iRotIndex;		// 转动一面的第几层(用于高阶魔方)
	float		m_fRotAngle;		// 当前旋转角度(播放转动动画)

	bool		m_bUpseting;
	int			m_iUpsetNum;
	std::vector<RotateInfo>		m_vecRotateInfo;
};
