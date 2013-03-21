/*
 * =====================================================================
 *
 * FileName:    Block.h
 * Author:      Buck
 * Email:       Buck88@gmail.com
 * Created:     2013.3.21
 * Environment: Visual Studio 2008
 *
 * Description: 魔方中每个块的定义
 *
 * ======================================================================
 */

#pragma once

#include "Vertex.h"
#include "global.h"

#define BLOCK_VERTEX_NUM 24	// 总顶点数 = 6*4
#define BLOCK_INDICE_NUM 36	// 总三角形顶点数 = 6*2*3

class CBlock
{;
public:
	CBlock(void);
	~CBlock(void) {}
	
	typedef void (CBlock::*RotFunc)(float ang);

	/**
	x、y、z: 块所在坐标
	len: 边长
	*/
	void Init(float x, float y, float z, float len=2.0f);
	void SetColor(int index, int c);

	// 将块渲染信息添加到渲染缓存中
	int FillVertices(void *vertices, int startIndex);
	int FillIndices(WORD *indices, int startIndex);

	// 旋转相关函数
	void Rotate(RotateType type, float ang=D3DX_PI/2);
	void F(float ang=D3DX_PI/2);
	void B(float ang=D3DX_PI/2);
	void U(float ang=D3DX_PI/2);
	void D(float ang=D3DX_PI/2);
	void L(float ang=D3DX_PI/2);
	void R(float ang=D3DX_PI/2);

private:
	CubeVertex	m_CubeVertex[BLOCK_VERTEX_NUM];
	int			m_iIndex[BLOCK_INDICE_NUM];
	
	// 实现快速旋转
	RotFunc		m_pRotFunc[RT_NUM];

	// 魔方每个面的颜色
	static const D3DXCOLOR ms_FaceColor[7];
};