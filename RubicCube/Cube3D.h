/*
 * =====================================================================
 *
 * FileName:    Cube3D.h
 * Author:      Buck
 * Email:       Buck88@126.com
 * Created:     2013.3.21
 * Environment: Visual Studio 2008
 *
 * Description: 3D��Ⱦ�࣬�洢���н׵�ħ������Ӧ������
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
	// ׼����Ⱦ��Ϣ
	void FillCubeInfo();
	// ��תħ����������
	bool pt2vec(CPoint pt, D3DXVECTOR3 &vec);

	// �ж�ת��һ��
	void RotateJudge(CPoint pt);
	// �ж�������Ƿ����ħ��
	bool HitTest(CPoint pt);

	// ������Ļ����ʰȡ����
	Ray GetRay(CPoint pt);
	// ����ʰȡ���߻�û���ħ���漰λ��
	int GenInfoFromRay(Ray ray, float &xRet, float &yRet, float &zRet);

private:
	// ��Ⱦ����
	IDirect3DDevice9	*m_pD3DDevice9;
	Camera				m_Camera;
	int					m_iWidth;
	int					m_iHeight;

	IDirect3DVertexBuffer9* VB;
	IDirect3DIndexBuffer9*  IB;
	int					m_iRenderVertexNum;
	int					m_iRenderIndiceNum;
	D3DXMATRIX			m_MatrixWorld;

	// ħ��
	CCube			*m_pCubesAll[RANK_MAX];
	CCube			*m_pCube;

	// �������
	bool		m_bRotCube;
	bool		m_bRotFace;
	CPoint		m_ptLast;
};
