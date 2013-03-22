/*
 * =====================================================================
 *
 * FileName:    Cube3D.h
 * Author:      Buck
 * Email:       Buck88@126.com
 * Created:     2013.3.21
 * Environment: Visual Studio 2008
 *
 * Description: 3D渲染类，存储所有阶的魔方，响应鼠标操作
 *
 * ======================================================================
 */

#pragma once

#include <d3dx9.h>

#include "camera.h"
#include "Cube.h"

class CCube3D
{	
	struct Ray
	{
		D3DXVECTOR3 _origin;
		D3DXVECTOR3 _direction;
	};

public:
	CCube3D(void);
	~CCube3D(void);

	bool Init(HWND hwnd, int w, int h);
	void Release();

	void Tick(int iTime);
	void Render();

	void OnLButtonDown(UINT nFlags, CPoint point);
	void OnLButtonUp(UINT nFlags, CPoint point);
	void OnMouseMove(UINT nFlags, CPoint point);
	bool KeyDown(UINT nChar);

	CCube* GetCube() { return m_pCube; }
	void Reset();

	// index: 2-RANK_MAX
	void ChangeCube(int index) { m_pCube = m_pCubesAll[index-2]; }

private:
	// 准备渲染信息
	void FillCubeInfo();
	// 旋转魔方辅助函数
	bool pt2vec(CPoint pt, D3DXVECTOR3 &vec);

	// 判断转哪一面
	void RotateJudge(CPoint pt);
	// 判断鼠标点击是否击中魔方
	bool HitTest(CPoint pt);

	// 根据屏幕点获得拾取射线
	Ray GetRay(CPoint pt);
	// 根据拾取射线获得击中魔方面及位置
	int GenInfoFromRay(Ray ray, float &xRet, float &yRet, float &zRet);

private:
	// 渲染参数
	IDirect3DDevice9	*m_pD3DDevice9;
	Camera				m_Camera;
	int					m_iWidth;
	int					m_iHeight;

	IDirect3DVertexBuffer9* VB;
	IDirect3DIndexBuffer9*  IB;
	int					m_iRenderVertexNum;
	int					m_iRenderIndiceNum;
	D3DXMATRIX			m_MatrixWorld;

	// 魔方
	CCube			*m_pCubesAll[RANK_MAX];
	CCube			*m_pCube;

	// 操作相关
	bool		m_bRotCube;
	bool		m_bRotFace;
	CPoint		m_ptLast;
};
