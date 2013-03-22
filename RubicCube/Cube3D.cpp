/*
 * =====================================================================
 *
 * FileName:    Cube3D.cpp
 * Author:      Buck
 * Email:       Buck88@126.com
 * Created:     2013.3.21
 * Environment: Visual Studio 2008
 *
 * Description: 3DäÖÈ¾ÊµÏÖÀà
 *
 * ======================================================================
 */

#include "StdAfx.h"
#include "Cube3D.h"
#include "Vertex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CCube3D::CCube3D(void)
{
	D3DXMatrixIdentity(&m_MatrixWorld);
	m_bRotCube = false;
	m_bRotFace = false;
}

CCube3D::~CCube3D(void)
{
}

bool CCube3D::Init(HWND hwnd, int w, int h)
{
	m_iWidth = w;
	m_iHeight = h;
	D3DDEVTYPE deviceType = D3DDEVTYPE_HAL;
	// Step 1: Create the IDirect3D9 object.
	IDirect3D9 *d3d9 = Direct3DCreate9(D3D_SDK_VERSION);
	if (!d3d9)
	{
		::MessageBox(0, L"Direct3DCreate9 - FAILED", 0, 0);
		return false;
	}

	// Step 2: Check for hardware vp.
	D3DCAPS9 caps;
	d3d9->GetDeviceCaps(D3DADAPTER_DEFAULT, deviceType, &caps);

	int vp = 0;
	if( caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT )
		vp = D3DCREATE_HARDWARE_VERTEXPROCESSING;
	else
		vp = D3DCREATE_SOFTWARE_VERTEXPROCESSING;

	// Step 3: Fill out the D3DPRESENT_PARAMETERS structure.

	D3DPRESENT_PARAMETERS d3dpp;
	d3dpp.BackBufferWidth            = w;
	d3dpp.BackBufferHeight           = h;
	d3dpp.BackBufferFormat           = D3DFMT_A8R8G8B8;
	d3dpp.BackBufferCount            = 1;
	d3dpp.MultiSampleType            = D3DMULTISAMPLE_2_SAMPLES; //D3DMULTISAMPLE_NONE;
	d3dpp.MultiSampleQuality         = 0;
	d3dpp.SwapEffect                 = D3DSWAPEFFECT_DISCARD; 
	d3dpp.hDeviceWindow              = hwnd;
	d3dpp.Windowed                   = true;
	d3dpp.EnableAutoDepthStencil     = true; 
	d3dpp.AutoDepthStencilFormat     = D3DFMT_D24S8;
	d3dpp.Flags                      = 0;
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
	d3dpp.PresentationInterval       = D3DPRESENT_INTERVAL_IMMEDIATE;

	// Step 4: Create the device.

	HRESULT hr = 0;
	hr = d3d9->CreateDevice(
		D3DADAPTER_DEFAULT, // primary adapter
		deviceType,         // device type
		hwnd,               // window associated with device
		vp,                 // CubeVertex processing
		&d3dpp,             // present parameters
		&m_pD3DDevice9);            // return created device

	if( FAILED(hr) )
	{
		// try again using a 16-bit depth buffer
		d3dpp.AutoDepthStencilFormat = D3DFMT_D16;

		hr = d3d9->CreateDevice(
			D3DADAPTER_DEFAULT,
			deviceType,
			hwnd,
			vp,
			&d3dpp,
			&m_pD3DDevice9);

		if( FAILED(hr) )
		{
			d3d9->Release(); // done with d3d9 object
			::MessageBox(0, L"CreateDevice() - FAILED", 0, 0);
			return false;
		}
	}

	d3d9->Release(); // done with d3d9 object


	D3DXMATRIX proj;
	D3DXMatrixPerspectiveFovLH(
		&proj,
		D3DX_PI * 0.25f, // 45 - degree
		(float)w / (float)h,
		1.0f,
		1000.0f);
	m_pD3DDevice9->SetTransform(D3DTS_PROJECTION, &proj);
	m_pD3DDevice9->SetRenderState(D3DRS_LIGHTING, false);

	m_pD3DDevice9->CreateVertexBuffer(
		40960* sizeof(CubeVertex), 
		D3DUSAGE_WRITEONLY,
		CubeVertex::FVF,
		D3DPOOL_MANAGED,
		&VB,
		0);

	m_pD3DDevice9->CreateIndexBuffer(
		40960 * sizeof(WORD),
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&IB,
		0);

	m_Camera.walk(-20.0f);

	int i;
	for (i=0; i<RANK_MAX-1; ++i)
		m_pCubesAll[i] = new CCube();

	m_pCubesAll[0]->Init(2, 5.0f);
	m_pCubesAll[1]->Init(3, 6.0f);
	m_pCubesAll[2]->Init(4, 6.2f);
	m_pCubesAll[3]->Init(5, 6.4f);
	m_pCubesAll[4]->Init(6, 6.6f);
	m_pCubesAll[5]->Init(7, 6.8f);
	m_pCubesAll[6]->Init(8, 7.0f);
	m_pCubesAll[7]->Init(9, 7.2f);
	m_pCubesAll[8]->Init(10, 7.4f);
	m_pCubesAll[9]->Init(11, 7.6f);
	m_pCubesAll[10]->Init(12, 7.8f);

	m_pCube = m_pCubesAll[1];
	return true;
}

void CCube3D::Release()
{
	int i;
	for (i=0; i<RANK_MAX-1; ++i)
	{
		m_pCubesAll[i]->Release();
		delete m_pCubesAll[i];
	}

	VB->Release();
	IB->Release();
	m_pD3DDevice9->Release();
}

void CCube3D::Tick(int iTime)
{
	m_pCube->Tick(iTime);
}

void CCube3D::Render()
{
	float timeDelta = .1f;

	if( ::GetAsyncKeyState(VK_UP) & 0x8000f )
		m_Camera.walk(2.0f * timeDelta);

	if( ::GetAsyncKeyState(VK_DOWN) & 0x8000f )
		m_Camera.walk(-2.0f * timeDelta);

	if( ::GetAsyncKeyState(VK_LEFT) & 0x8000f )
		m_Camera.strafe(-2.0f * timeDelta);

	if( ::GetAsyncKeyState(VK_RIGHT) & 0x8000f )
		m_Camera.strafe(2.0f * timeDelta);

	FillCubeInfo();

	D3DXMATRIX V;
	m_Camera.getViewMatrix(&V);
	m_pD3DDevice9->SetTransform(D3DTS_VIEW, &V);

	D3DRECT rc = { 0, 0, m_iWidth, m_iHeight};
	m_pD3DDevice9->Clear(1, &rc, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0xff555555, 1.0f, 0);


	m_pD3DDevice9->BeginScene();

	m_pD3DDevice9->SetTransform(D3DTS_WORLD, &m_MatrixWorld);

	m_pD3DDevice9->SetFVF(CubeVertex::FVF);
	m_pD3DDevice9->SetStreamSource(0, VB, 0, sizeof(CubeVertex));
	m_pD3DDevice9->SetIndices(IB);
	m_pD3DDevice9->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, m_iRenderVertexNum, 0, m_iRenderIndiceNum/3);

	m_pD3DDevice9->EndScene();


	RECT rc1 = { 0, 0, m_iWidth, m_iHeight};
	m_pD3DDevice9->Present(&rc1, &rc1, 0, 0);
}

void CCube3D::FillCubeInfo()
{
	CubeVertex* vertices;
	WORD* indices = 0;
	VB->Lock(0, 0, (void**)&vertices, 0);
	IB->Lock(0, 0, (void**)&indices, 0);

	m_pCube->FillCubeVertexInfo(vertices, indices, m_iRenderVertexNum, m_iRenderIndiceNum);


	VB->Unlock();
	IB->Unlock();
}

void CCube3D::OnLButtonDown(UINT nFlags, CPoint point)
{
	if( m_pCube->CanOpt() && HitTest(point) && (nFlags&MK_CONTROL)==0 )
	{
		m_bRotFace = true;
	}
	else
		m_bRotCube = true;

	m_ptLast = point;
}

void CCube3D::OnLButtonUp(UINT nFlags, CPoint point)
{
	m_bRotCube = false;
	m_bRotFace = false;
}

bool CCube3D::pt2vec(CPoint pt, D3DXVECTOR3 &vec)
{
	vec.x = ((float)pt.x-m_iWidth/2)/m_iWidth/2;
	vec.y = ((float)m_iHeight/2-pt.y)/m_iHeight/2;
	vec.z = -sqrt(1-vec.x*vec.x-vec.y*vec.y);

	return true;
}

void CCube3D::RotateJudge(CPoint pt)
{
	Ray rayLast = GetRay(m_ptLast);
	Ray rayNow = GetRay(pt);

	int r1, r2;
	float x1, y1, z1;
	float x2, y2, z2;
	r1 = GenInfoFromRay(rayLast, x1, y1, z1);
	r2 = GenInfoFromRay(rayNow, x2, y2, z2);

	if(r1==r2 && r1>=0)
	{
	switch(r1)
	{
	case 0:
		if(abs(x2-x1)>abs(y2-y1))
		{
			if(x2>x1)
			{
				m_pCube->StartRot(RT_D, m_pCube->GetDIndex(y1));
			}
			else
			{
				m_pCube->StartRot(RT_U, m_pCube->GetUIndex(y1));
			}
		}
		else
		{
			if(y2>y1)
			{
				m_pCube->StartRot(RT_R, m_pCube->GetRIndex(x1));
			}
			else
			{
				m_pCube->StartRot(RT_L, m_pCube->GetLIndex(x1));
			}
		}
		break;
	case 1:
		if(abs(x2-x1)>abs(y2-y1))
		{
			if(x2<x1)
			{
				m_pCube->StartRot(RT_D, m_pCube->GetDIndex(y1));
			}
			else
			{
				m_pCube->StartRot(RT_U, m_pCube->GetUIndex(y1));
			}
		}
		else
		{
			if(y2<y1)
			{
				m_pCube->StartRot(RT_R, m_pCube->GetRIndex(x1));
			}
			else
			{
				m_pCube->StartRot(RT_L, m_pCube->GetLIndex(x1));
			}
		}
		break;
	case 2:
		if(abs(x2-x1)>abs(z2-z1))
		{
			if(x2>x1)
			{
				m_pCube->StartRot(RT_F, m_pCube->GetDIndex(z1));
			}
			else
			{
				m_pCube->StartRot(RT_B, m_pCube->GetUIndex(z1));
			}
		}
		else
		{
			if(z2>z1)
			{
				m_pCube->StartRot(RT_R, m_pCube->GetRIndex(x1));
			}
			else
			{
				m_pCube->StartRot(RT_L, m_pCube->GetLIndex(x1));
			}
		}
		break;
	case 3:
		if(abs(x2-x1)>abs(z2-z1))
		{
			if(x2<x1)
			{
				m_pCube->StartRot(RT_F, m_pCube->GetDIndex(z1));
			}
			else
			{
				m_pCube->StartRot(RT_B, m_pCube->GetUIndex(z1));
			}
		}
		else
		{
			if(z2<z1)
			{
				m_pCube->StartRot(RT_R, m_pCube->GetRIndex(x1));
			}
			else
			{
				m_pCube->StartRot(RT_L, m_pCube->GetLIndex(x1));
			}
		}
		break;
	case 4:
		if(abs(z2-z1)>abs(y2-y1))
		{
			if(z2>z1)
			{
				m_pCube->StartRot(RT_U, m_pCube->GetUIndex(y1));
			}
			else
			{
				m_pCube->StartRot(RT_D, m_pCube->GetDIndex(y1));
			}
		}
		else
		{
			if(y2>y1)
			{
				m_pCube->StartRot(RT_F, m_pCube->GetFIndex(z1));
			}
			else
			{
				m_pCube->StartRot(RT_B, m_pCube->GetBIndex(z1));
			}
		}
		break;
	case 5:
		if(abs(z2-z1)>abs(y2-y1))
		{
			if(z2<z1)
			{
				m_pCube->StartRot(RT_U, m_pCube->GetUIndex(y1));
			}
			else
			{
				m_pCube->StartRot(RT_D, m_pCube->GetDIndex(y1));
			}
		}
		else
		{
			if(y2<y1)
			{
				m_pCube->StartRot(RT_F, m_pCube->GetFIndex(z1));
			}
			else
			{
				m_pCube->StartRot(RT_B, m_pCube->GetBIndex(z1));
			}
		}
		break;
	}
	}
}

void CCube3D::OnMouseMove(UINT nFlags, CPoint point)
{
	if (m_bRotCube)
	{
		D3DXVECTOR3 v1, v2, vR;
		pt2vec(m_ptLast, v1);
		pt2vec(point, v2);
		D3DXVec3Cross(&vR, &v1, &v2);

		D3DXVECTOR3 vDelt = v1-v2;
		float len = D3DXVec3Length(&vDelt);

		D3DXMATRIX matOut;
		D3DXMatrixRotationAxis(&matOut, &vR, len*10);
		m_MatrixWorld *= matOut;

		m_ptLast = point;
	}
	else if (m_bRotFace)
	{
		CPoint ptDelta = point - m_ptLast;

		if( (ptDelta.x*ptDelta.x + ptDelta.y*ptDelta.y)>100 )
		{
			RotateJudge(point);
			
			m_ptLast = point;
			m_bRotFace = false;
		}
	}
}

bool CCube3D::KeyDown(UINT nChar)
{
	switch (nChar)
	{
	case 'f':
	case 'F':
		m_pCube->StartRot(RT_F);
		break;
	case 'b':
	case 'B':
		m_pCube->StartRot(RT_B);
		break;
	case 'u':
	case 'U':
		m_pCube->StartRot(RT_U);
		break;
	case 'd':
	case 'D':
		m_pCube->StartRot(RT_D);
		break;
	case 'l':
	case 'L':
		m_pCube->StartRot(RT_L);
		break;
	case 'r':
	case 'R':
		m_pCube->StartRot(RT_R);
		break;
	default:
		return false;
	}

	return true;
}

bool CCube3D::HitTest(CPoint pt)
{
	Ray ray = GetRay(pt);
	float x, y, z;
	int r = GenInfoFromRay(ray, x, y, z);
	return r>=0;
}

CCube3D::Ray CCube3D::GetRay(CPoint pt)
{
	Ray ray;	
	
	D3DVIEWPORT9 vp;
	m_pD3DDevice9->GetViewport(&vp);
	D3DXMATRIX proj;
	m_pD3DDevice9->GetTransform(D3DTS_PROJECTION, &proj);

	float px = ((( 2.0f*pt.x) / vp.Width)  - 1.0f) / proj(0, 0);
	float py = (((-2.0f*pt.y) / vp.Height) + 1.0f) / proj(1, 1);

	ray._origin    = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	ray._direction = D3DXVECTOR3(px, py, 1.0f);

	D3DXMATRIX view;	
	m_Camera.getViewMatrix(&view);

	D3DXMATRIX viewInverse;
	D3DXMatrixInverse(&viewInverse,	0, &view);
	
	D3DXVec3TransformCoord(
		&ray._origin,
		&ray._origin,
		&viewInverse);

	// transform the ray's direction, w = 0.
	D3DXVec3TransformNormal(
		&ray._direction,
		&ray._direction,
		&viewInverse);

	D3DXMATRIX worldInverse;
	D3DXMatrixInverse(&worldInverse, 0, &m_MatrixWorld);
	
	D3DXVec3TransformCoord(
		&ray._origin,
		&ray._origin,
		&worldInverse);

	// transform the ray's direction, w = 0.
	D3DXVec3TransformNormal(
		&ray._direction,
		&ray._direction,
		&worldInverse);

	// normalize the direction
	D3DXVec3Normalize(&ray._direction, &ray._direction);

	return ray;
}

int CCube3D::GenInfoFromRay(Ray ray, float &xRet, float &yRet, float &zRet)
{
	D3DXVECTOR3 vecOrigin = ray._origin;
	D3DXVECTOR3 vecDir = ray._direction;
	int rRet = -1;
	float tBest = 9999.0f;
	float fLen = m_pCube->GetLen();
	float t;
	float x, y, z;
	// front
	t = (-fLen/2-vecOrigin.z) / vecDir.z;
	x = vecOrigin.x + t*vecDir.x;
	y = vecOrigin.y + t*vecDir.y;
	if(x>-fLen/2 && x<fLen/2 && y>-fLen/2 && y<fLen/2 && t<tBest)
	{
		tBest = t;
		xRet = x;
		yRet = y;
		zRet = 9999.0f;

		rRet = 0;
	}
	// back
	t = (fLen/2-vecOrigin.z) / vecDir.z;
	x = vecOrigin.x + t*vecDir.x;
	y = vecOrigin.y + t*vecDir.y;
	if(x>-fLen/2 && x<fLen/2 && y>-fLen/2 && y<fLen/2 && t<tBest)
	{
		tBest = t;
		xRet = x;
		yRet = y;
		zRet = 9999.0f;

		rRet = 1;
	}
	// up
	t = (fLen/2-vecOrigin.y) / vecDir.y;
	x = vecOrigin.x + t*vecDir.x;
	z = vecOrigin.z + t*vecDir.z;
	if(x>-fLen/2 && x<fLen/2 && z>-fLen/2 && z<fLen/2 && t<tBest)
	{
		tBest = t;
		xRet = x;
		yRet = 9999.0f;
		zRet = z;

		rRet = 2;
	}
	// down
	t = (-fLen/2-vecOrigin.y) / vecDir.y;
	x = vecOrigin.x + t*vecDir.x;
	z = vecOrigin.z + t*vecDir.z;
	if(x>-fLen/2 && x<fLen/2 && z>-fLen/2 && z<fLen/2 && t<tBest)
	{
		tBest = t;
		xRet = x;
		yRet = 9999.0f;
		zRet = z;

		rRet = 3;
	}
	// left
	t = (-fLen/2-vecOrigin.x) / vecDir.x;
	y = vecOrigin.y + t*vecDir.y;
	z = vecOrigin.z + t*vecDir.z;
	if(y>-fLen/2 && y<fLen/2 && z>-fLen/2 && z<fLen/2 && t<tBest)
	{
		tBest = t;
		xRet = 9999.0f;
		yRet = y;
		zRet = z;

		rRet = 4;
	}
	// right
	t = (fLen/2-vecOrigin.x) / vecDir.x;
	y = vecOrigin.y + t*vecDir.y;
	z = vecOrigin.z + t*vecDir.z;
	if(y>-fLen/2 && y<fLen/2 && z>-fLen/2 && z<fLen/2 && t<tBest)
	{
		tBest = t;
		xRet = 9999.0f;
		yRet = y;
		zRet = z;

		rRet = 5;
	}

	return rRet;
}

void CCube3D::Reset()
{
	m_pCube->Release();
	m_pCube->Init(m_pCube->GetRank(), m_pCube->GetLen(), m_pCube->GetSpeed());
	D3DXMatrixIdentity(&m_MatrixWorld);
}