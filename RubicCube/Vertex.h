/*
 * =====================================================================
 *
 * FileName:    Vertex.h
 * Author:      Buck
 * Email:       Buck88@126.com
 * Created:     2013.3.21
 * Environment: Visual Studio 2008
 *
 * Description: 顶点定义，包括位置和颜色
 *
 * ======================================================================
 */

#ifndef _CUBE_VERTEX_
#define _CUBE_VERTEX_

#include <d3dx9.h>

struct CubeVertex
{
	CubeVertex(){}

	CubeVertex(float x, float y, float z, D3DCOLOR c = D3DCOLOR_XRGB(  0, 255,   0))
	{
		_x = x;	 _y = y;  _z = z;  _color = c;
	}

	float _x, _y, _z;
	D3DCOLOR _color;

	static const DWORD FVF;
};

#endif // _CUBE_VERTEX_