/*
 * =====================================================================
 *
 * FileName:    Block.cpp
 * Author:      Buck
 * Email:       Buck88@gmail.com
 * Created:     2013.3.21
 * Environment: Visual Studio 2008
 *
 * Description: CBlock实现
 *
 * ======================================================================
 */

#include "StdAfx.h"
#include "Block.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

const D3DXCOLOR CBlock::ms_FaceColor[7] = {
	D3DCOLOR_XRGB(  0,   0, 255),
	D3DCOLOR_XRGB(  0, 255,   0),

	D3DCOLOR_XRGB(255, 255,   0),
	D3DCOLOR_XRGB(255, 255, 255),

	D3DCOLOR_XRGB(255,  80,   0),
	D3DCOLOR_XRGB(255,   0,   0),
	D3DCOLOR_XRGB(  0,   0,   0),
};

CBlock::CBlock(void)
{
	int index = 0;

	// 初始化三角形index
	// front 0-3
	m_iIndex[index++] = 0; m_iIndex[index++] = 1; m_iIndex[index++] = 2;
	m_iIndex[index++] = 0; m_iIndex[index++] = 2; m_iIndex[index++] = 3;

	// back 4-7
	m_iIndex[index++] = 4; m_iIndex[index++] = 6; m_iIndex[index++] = 5;
	m_iIndex[index++] = 4; m_iIndex[index++] = 7; m_iIndex[index++] = 6;

	// up 8-11
	m_iIndex[index++] = 8; m_iIndex[index++] = 9; m_iIndex[index++] = 10;
	m_iIndex[index++] = 8; m_iIndex[index++] = 10; m_iIndex[index++] = 11;

	// down 12-15
	m_iIndex[index++] = 12; m_iIndex[index++] = 14; m_iIndex[index++] = 13;
	m_iIndex[index++] = 12; m_iIndex[index++] = 15; m_iIndex[index++] = 14;

	// left 16-19
	m_iIndex[index++] = 16; m_iIndex[index++] = 17; m_iIndex[index++] = 18;
	m_iIndex[index++] = 16; m_iIndex[index++] = 18; m_iIndex[index++] = 19;

	// right 20-23
	m_iIndex[index++] = 20; m_iIndex[index++] = 22; m_iIndex[index++] = 21;
	m_iIndex[index++] = 20; m_iIndex[index++] = 23; m_iIndex[index++] = 22;

	m_pRotFunc[RT_F] = &CBlock::F;
	m_pRotFunc[RT_B] = &CBlock::B;
	m_pRotFunc[RT_U] = &CBlock::U;
	m_pRotFunc[RT_D] = &CBlock::D;
	m_pRotFunc[RT_L] = &CBlock::L;
	m_pRotFunc[RT_R] = &CBlock::R;
}

void CBlock::Init(float x, float y, float z, float len)
{
	int index = 0;
	// 初始化顶点位置
	// front
	m_CubeVertex[index++] = CubeVertex(-0.9f*len/2+x,  0.9f*len/2+y, -1.0f*len/2+z, ms_FaceColor[6]);
	m_CubeVertex[index++] = CubeVertex( 0.9f*len/2+x,  0.9f*len/2+y, -1.0f*len/2+z, ms_FaceColor[6]);
	m_CubeVertex[index++] = CubeVertex( 0.9f*len/2+x, -0.9f*len/2+y, -1.0f*len/2+z, ms_FaceColor[6]);
	m_CubeVertex[index++] = CubeVertex(-0.9f*len/2+x, -0.9f*len/2+y, -1.0f*len/2+z, ms_FaceColor[6]);
	
	// back
	m_CubeVertex[index++] = CubeVertex(-0.9f*len/2+x,  0.9f*len/2+y,  1.0f*len/2+z, ms_FaceColor[6]);
	m_CubeVertex[index++] = CubeVertex( 0.9f*len/2+x,  0.9f*len/2+y,  1.0f*len/2+z, ms_FaceColor[6]);
	m_CubeVertex[index++] = CubeVertex( 0.9f*len/2+x, -0.9f*len/2+y,  1.0f*len/2+z, ms_FaceColor[6]);
	m_CubeVertex[index++] = CubeVertex(-0.9f*len/2+x, -0.9f*len/2+y,  1.0f*len/2+z, ms_FaceColor[6]);
	
	// up
	m_CubeVertex[index++] = CubeVertex(-0.9f*len/2+x,  1.0f*len/2+y,  0.9f*len/2+z, ms_FaceColor[6]);
	m_CubeVertex[index++] = CubeVertex( 0.9f*len/2+x,  1.0f*len/2+y,  0.9f*len/2+z, ms_FaceColor[6]);
	m_CubeVertex[index++] = CubeVertex( 0.9f*len/2+x,  1.0f*len/2+y, -0.9f*len/2+z, ms_FaceColor[6]);
	m_CubeVertex[index++] = CubeVertex(-0.9f*len/2+x,  1.0f*len/2+y, -0.9f*len/2+z, ms_FaceColor[6]);
	
	// down
	m_CubeVertex[index++] = CubeVertex(-0.9f*len/2+x, -1.0f*len/2+y,  0.9f*len/2+z, ms_FaceColor[6]);
	m_CubeVertex[index++] = CubeVertex( 0.9f*len/2+x, -1.0f*len/2+y,  0.9f*len/2+z, ms_FaceColor[6]);
	m_CubeVertex[index++] = CubeVertex( 0.9f*len/2+x, -1.0f*len/2+y, -0.9f*len/2+z, ms_FaceColor[6]);
	m_CubeVertex[index++] = CubeVertex(-0.9f*len/2+x, -1.0f*len/2+y, -0.9f*len/2+z, ms_FaceColor[6]);

	// left	
	m_CubeVertex[index++] = CubeVertex(-1.0f*len/2+x,  0.9f*len/2+y,  0.9f*len/2+z, ms_FaceColor[6]);
	m_CubeVertex[index++] = CubeVertex(-1.0f*len/2+x,  0.9f*len/2+y, -0.9f*len/2+z, ms_FaceColor[6]);
	m_CubeVertex[index++] = CubeVertex(-1.0f*len/2+x, -0.9f*len/2+y, -0.9f*len/2+z, ms_FaceColor[6]);
	m_CubeVertex[index++] = CubeVertex(-1.0f*len/2+x, -0.9f*len/2+y,  0.9f*len/2+z, ms_FaceColor[6]);
	
	// right
	m_CubeVertex[index++] = CubeVertex( 1.0f*len/2+x,  0.9f*len/2+y,  0.9f*len/2+z, ms_FaceColor[6]);
	m_CubeVertex[index++] = CubeVertex( 1.0f*len/2+x,  0.9f*len/2+y, -0.9f*len/2+z, ms_FaceColor[6]);
	m_CubeVertex[index++] = CubeVertex( 1.0f*len/2+x, -0.9f*len/2+y, -0.9f*len/2+z, ms_FaceColor[6]);
	m_CubeVertex[index++] = CubeVertex( 1.0f*len/2+x, -0.9f*len/2+y,  0.9f*len/2+z, ms_FaceColor[6]);
}

void CBlock::SetColor(int index, int c)
{
	int i;
	for(i=0; i<4; ++i)
		m_CubeVertex[index*4+i]._color = ms_FaceColor[c];
}

int CBlock::FillVertices(void *vertices, int startIndex)
{
	CubeVertex *pCubeVertex = (CubeVertex*)vertices;
	int i;
	for (i=0; i<BLOCK_VERTEX_NUM; ++i)
	{
		pCubeVertex[i] = m_CubeVertex[i];
	}
	return BLOCK_VERTEX_NUM;
}

int CBlock::FillIndices(WORD *indices, int startIndex)
{
	int i;
	for (i=0; i<BLOCK_INDICE_NUM; ++i)
	{
		indices[i] = startIndex+m_iIndex[i];
	}
	return BLOCK_INDICE_NUM;
}

void CBlock::Rotate(RotateType type, float ang)
{
	(this->*m_pRotFunc[type])(ang);
}

void CBlock::F(float ang)
{
	D3DXMATRIX mat;
	D3DXMatrixRotationZ(&mat, D3DX_PI*2-ang);
	int i;
	for (i=0; i<BLOCK_VERTEX_NUM; ++i)
	{
		D3DXVECTOR3 vec3(m_CubeVertex[i]._x, m_CubeVertex[i]._y, m_CubeVertex[i]._z);
		D3DXVECTOR3 vecOut;
		D3DXVec3TransformCoord(&vecOut, &vec3, &mat);
		m_CubeVertex[i]._x = vecOut.x;
		m_CubeVertex[i]._y = vecOut.y;
		m_CubeVertex[i]._z = vecOut.z;
	}
}

void CBlock::B(float ang)
{
	D3DXMATRIX mat;
	D3DXMatrixRotationZ(&mat, ang);
	int i;
	for (i=0; i<BLOCK_VERTEX_NUM; ++i)
	{
		D3DXVECTOR3 vec3(m_CubeVertex[i]._x, m_CubeVertex[i]._y, m_CubeVertex[i]._z);
		D3DXVECTOR3 vecOut;
		D3DXVec3TransformCoord(&vecOut, &vec3, &mat);
		m_CubeVertex[i]._x = vecOut.x;
		m_CubeVertex[i]._y = vecOut.y;
		m_CubeVertex[i]._z = vecOut.z;
	}
}

void CBlock::U(float ang)
{
	D3DXMATRIX mat;
	D3DXMatrixRotationY(&mat, ang);
	int i;
	for (i=0; i<BLOCK_VERTEX_NUM; ++i)
	{
		D3DXVECTOR3 vec3(m_CubeVertex[i]._x, m_CubeVertex[i]._y, m_CubeVertex[i]._z);
		D3DXVECTOR3 vecOut;
		D3DXVec3TransformCoord(&vecOut, &vec3, &mat);
		m_CubeVertex[i]._x = vecOut.x;
		m_CubeVertex[i]._y = vecOut.y;
		m_CubeVertex[i]._z = vecOut.z;
	}
}

void CBlock::D(float ang)
{
	D3DXMATRIX mat;
	D3DXMatrixRotationY(&mat, D3DX_PI*2-ang);
	int i;
	for (i=0; i<BLOCK_VERTEX_NUM; ++i)
	{
		D3DXVECTOR3 vec3(m_CubeVertex[i]._x, m_CubeVertex[i]._y, m_CubeVertex[i]._z);
		D3DXVECTOR3 vecOut;
		D3DXVec3TransformCoord(&vecOut, &vec3, &mat);
		m_CubeVertex[i]._x = vecOut.x;
		m_CubeVertex[i]._y = vecOut.y;
		m_CubeVertex[i]._z = vecOut.z;
	}
}

void CBlock::L(float ang)
{
	D3DXMATRIX mat;
	D3DXMatrixRotationX(&mat, D3DX_PI*2-ang);
	int i;
	for (i=0; i<BLOCK_VERTEX_NUM; ++i)
	{
		D3DXVECTOR3 vec3(m_CubeVertex[i]._x, m_CubeVertex[i]._y, m_CubeVertex[i]._z);
		D3DXVECTOR3 vecOut;
		D3DXVec3TransformCoord(&vecOut, &vec3, &mat);
		m_CubeVertex[i]._x = vecOut.x;
		m_CubeVertex[i]._y = vecOut.y;
		m_CubeVertex[i]._z = vecOut.z;
	}

}

void CBlock::R(float ang)
{
	D3DXMATRIX mat;
	D3DXMatrixRotationX(&mat, ang);
	int i;
	for (i=0; i<BLOCK_VERTEX_NUM; ++i)
	{
		D3DXVECTOR3 vec3(m_CubeVertex[i]._x, m_CubeVertex[i]._y, m_CubeVertex[i]._z);
		D3DXVECTOR3 vecOut;
		D3DXVec3TransformCoord(&vecOut, &vec3, &mat);
		m_CubeVertex[i]._x = vecOut.x;
		m_CubeVertex[i]._y = vecOut.y;
		m_CubeVertex[i]._z = vecOut.z;
	}

}