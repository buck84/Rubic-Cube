/*
 * =====================================================================
 *
 * FileName:    Cube.cpp.
 * Author:      Buck
 * Email:       Buck88@gmail.com
 * Created:     2013.3.21
 * Environment: Visual Studio 2008
 *
 * Description: 魔方类实现
 *
 * ======================================================================
 */

#include "StdAfx.h"
#include "Cube.h"
#include "Block.h"
#include "Vertex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CCube::CCube(void)
{
	m_iRotIndex = -1;
	m_bUpseting = false;
	m_iUpsetNum = 0;
}

CCube::~CCube(void)
{
}

void CCube::Init(int rank, float len, float speed)
{
	if(rank>RANK_MAX)
		return;
	m_iRank = rank;
	m_fLength = len;
	m_fSpeed = speed;

	m_pBlocks = new CBlock*[rank*rank*rank];
	m_vecRotateAnimInfo.clear();

	float fBlockLen = len/rank;
	int i, j, k;
	for(i=0; i<rank; ++i)	// z
	{
		for(j=0; j<rank; ++j)	// y
		{
			for(k=0; k<rank; ++k)	// x
			{
				if( i==0 || i==(rank-1) || j==0 || j==(rank-1) || k==0 || k==(rank-1) )
				{
					m_pBlocks[i*rank*rank + j*rank + k] = new CBlock;
					m_pBlocks[i*rank*rank + j*rank + k]->Init((k-(m_iRank-1.0f)/2.0f)*fBlockLen, (j-(m_iRank-1.0f)/2.0f)*fBlockLen, (i-(m_iRank-1.0f)/2.0f)*fBlockLen, fBlockLen);
				}
				else
					m_pBlocks[i*rank*rank + j*rank + k] = 0;
			}
		}
	}

	for (i=0; i<rank; ++i)
	{
		for (j=0; j<rank; ++j)
		{
			// front back
			m_pBlocks[       0*rank*rank + i*rank + j]->SetColor(0, CS_BLUE);
			m_pBlocks[(rank-1)*rank*rank + i*rank + j]->SetColor(1, CS_GREEN);

			// up down
			m_pBlocks[i*rank*rank + (rank-1)*rank + j]->SetColor(2, CS_YELLOW);
			m_pBlocks[i*rank*rank +        0*rank + j]->SetColor(3, CS_WHITE);

			// left right
			m_pBlocks[i*rank*rank + j*rank +        0]->SetColor(4, CS_ORANGE);
			m_pBlocks[i*rank*rank + j*rank + (rank-1)]->SetColor(5, CS_RED);
		}
	}

	// create rotate info
	for(i=0; i<m_iRank*m_iRank; ++i)
	{
		RotateAnimInfo rInfo;
		rInfo.pBlock = new CBlock();
		m_vecRotateAnimInfo.push_back(rInfo);
	}
}

void CCube::Release()
{
	int i;
	for (i=0; i<m_iRank*m_iRank*m_iRank; ++i)
	{
		delete m_pBlocks[i];
	}
	delete [] m_pBlocks;
	
	for (i=0; i<(int)m_vecRotateAnimInfo.size(); ++i)
		delete m_vecRotateAnimInfo[i].pBlock;

	m_vecRotateAnimInfo.clear();
}

void CCube::FillCubeVertexInfo(void *vertices, WORD *indices, int &vNum, int &iNum)
{
	CubeVertex *pCubeVertex = (CubeVertex*)vertices;
	vNum = 0;
	iNum = 0;
	int i, j, k;
	for (i=0; i<m_iRank; ++i)
	{
		for (j=0; j<m_iRank; ++j)
		{
			for (k=0; k<m_iRank; ++k)
			{				
				if( (i==0||i==(m_iRank-1)) || (j==0||j==(m_iRank-1)) || (k==0||k==(m_iRank-1)) )
				{
					CBlock *pBlock = GetRotBlock(i, j, k);
					if(!pBlock)
						pBlock = GetBlock(i, j, k);
					
					int num = pBlock->FillIndices(indices+iNum, vNum);
					iNum += num;
					num = pBlock->FillVertices(pCubeVertex+vNum, vNum);
					vNum += num;
				}
			}
		}
	}
}

void CCube::StartRot(RotateType type, int index)
{
	if(m_iRotIndex>=0)
		return;

	m_iRotType = type;
	m_iRotIndex = index;
	m_fRotAngle = 0;

	int i, j;
	for(i=0; i<m_iRank; ++i)
	{
		for(j=0; j<m_iRank; ++j)
		{
			int x, y, z;
			switch(m_iRotType)
			{
			case RT_F:
				x = i;
				y = j;
				z = index;
				break;
			case RT_B:
				x = i;
				y = j;
				z = m_iRank-1-index;
				break;
			case RT_U:
				x = i;
				y = (m_iRank-1-index);
				z = j;
				break;
			case RT_D:
				x = i;
				y = index;
				z = j;
				break;
			case RT_L:
				x = index;
				y = i;
				z = j;
				break;
			case RT_R:
				x = m_iRank-1-index;
				y = i;
				z = j;
				break;
			default:
				break;
			}
			m_vecRotateAnimInfo[i*m_iRank+j].index = z*m_iRank*m_iRank + y*m_iRank + x;
			if(GetBlock(x, y, z))
			*m_vecRotateAnimInfo[i*m_iRank+j].pBlock = *GetBlock(x, y, z);
		}
	}
}

void CCube::F(int index)
{
	int i, j;
	// 旋转相应方块
	for (i=0; i<m_iRank; ++i)
	{
		for (j=0; j<m_iRank; ++j)
		{
			CBlock *pBlock = GetBlock(i, j, index);
			if(pBlock)
				pBlock->F();
		}
	}

	/* 顺序替换4条边

	- -  |
         |
   |
   |  -  -

	*/
	// 调整在m_pBlocks中位置
	// 从外到里一圈一圈替换
	for(i=0; i<m_iRank/2; ++i)
	{
		CBlock *pBlockTmp;
		for(j=0; j<m_iRank-1-i*2; ++j)
		{
			pBlockTmp = GetBlock(i, i+j, index);			// 取左下
			pBlockTmp = ReplaceBlock(i+j, m_iRank-1-i, index, pBlockTmp);
			pBlockTmp = ReplaceBlock(m_iRank-1-i, m_iRank-1-i-j, index, pBlockTmp);
			pBlockTmp = ReplaceBlock(m_iRank-1-i-j, i, index, pBlockTmp);
			ReplaceBlock(i, i+j, index, pBlockTmp);
		}

		// 如果不是最外层，只需要交换一次
		if(index!=0 && index!=m_iRank-1)
			break;
	}
}

void CCube::B(int index)
{
	int i, j;
	for (i=0; i<m_iRank; ++i)
	{
		for (j=0; j<m_iRank; ++j)
		{
			CBlock *pBlock = GetBlock(i, j, m_iRank-1-index);
			if(pBlock)
				pBlock->B();
		}
	}
	
	for(i=0; i<m_iRank/2; ++i)
	{
		CBlock *pBlockTmp;
		for(j=0; j<m_iRank-1-i*2; ++j)
		{
			pBlockTmp = GetBlock(i, i+j, m_iRank-1-index);
			pBlockTmp = ReplaceBlock(m_iRank-1-i-j, i, m_iRank-1-index, pBlockTmp);
			pBlockTmp = ReplaceBlock(m_iRank-1-i, m_iRank-1-i-j, m_iRank-1-index, pBlockTmp);
			pBlockTmp = ReplaceBlock(i+j, m_iRank-1-i, m_iRank-1-index, pBlockTmp);
			ReplaceBlock(i, i+j, m_iRank-1-index, pBlockTmp);
		}

		// 如果不是最外层，只需要交换一次
		if(index!=0 && index!=m_iRank-1)
			break;
	}
}

void CCube::U(int index)
{
	int i, j;
	for (i=0; i<m_iRank; ++i)
	{
		for (j=0; j<m_iRank; ++j)
		{
			CBlock *pBlock = GetBlock(i, m_iRank-1-index, j);
			if(pBlock)
				pBlock->U();
		}
	}
	
	for(i=0; i<m_iRank/2; ++i)
	{
		CBlock *pBlockTmp;
		for(j=0; j<m_iRank-1-i*2; ++j)
		{
			pBlockTmp = GetBlock(i, m_iRank-1-index, i+j);
			pBlockTmp = ReplaceBlock(i+j, m_iRank-1-index, m_iRank-1-i, pBlockTmp);
			pBlockTmp = ReplaceBlock(m_iRank-1-i, m_iRank-1-index, m_iRank-1-i-j, pBlockTmp);
			pBlockTmp = ReplaceBlock(m_iRank-1-i-j, m_iRank-1-index, i, pBlockTmp);
			ReplaceBlock(i, m_iRank-1-index, i+j, pBlockTmp);
		}

		// 如果不是最外层，只需要交换一次
		if(index!=0 && index!=m_iRank-1)
			break;
	}
}

void CCube::D(int index)
{
	int i, j;
	for (i=0; i<m_iRank; ++i)
	{
		for (j=0; j<m_iRank; ++j)
		{
			CBlock *pBlock = GetBlock(i, index, j);
			if(pBlock)
				pBlock->D();
		}
	}
	
	for(i=0; i<m_iRank/2; ++i)
	{
		CBlock *pBlockTmp;
		for(j=0; j<m_iRank-1-i*2; ++j)
		{
			pBlockTmp = GetBlock(i, index, i+j);
			pBlockTmp = ReplaceBlock(m_iRank-1-i-j, index, i, pBlockTmp);
			pBlockTmp = ReplaceBlock(m_iRank-1-i, index, m_iRank-1-i-j, pBlockTmp);
			pBlockTmp = ReplaceBlock(i+j, index, m_iRank-1-i, pBlockTmp);
			ReplaceBlock(i, index, i+j, pBlockTmp);
		}

		// 如果不是最外层，只需要交换一次
		if(index!=0 && index!=m_iRank-1)
			break;
	}
}

void CCube::L(int index)
{
	int i, j;
	for (i=0; i<m_iRank; ++i)
	{
		for (j=0; j<m_iRank; ++j)
		{
			CBlock *pBlock = GetBlock(index, i, j);
			if(pBlock)
				pBlock->L();
		}
	}
	
	for(i=0; i<m_iRank/2; ++i)
	{
		CBlock *pBlockTmp;
		for(j=0; j<m_iRank-1-i*2; ++j)
		{
			pBlockTmp = GetBlock(index, i, i+j);
			pBlockTmp = ReplaceBlock(index, i+j, m_iRank-1-i, pBlockTmp);
			pBlockTmp = ReplaceBlock(index, m_iRank-1-i, m_iRank-1-i-j, pBlockTmp);
			pBlockTmp = ReplaceBlock(index, m_iRank-1-i-j, i, pBlockTmp);
			ReplaceBlock(index, i, i+j, pBlockTmp);
		}

		// 如果不是最外层，只需要交换一次
		if(index!=0 && index!=m_iRank-1)
			break;
	}
}

void CCube::R(int index)
{
	int i, j;
	for (i=0; i<m_iRank; ++i)
	{
		for (j=0; j<m_iRank; ++j)
		{
			CBlock *pBlock = GetBlock(m_iRank-1-index, i, j);
			if(pBlock)
				pBlock->R();
		}
	}
	
	for(i=0; i<m_iRank/2; ++i)
	{
		CBlock *pBlockTmp;
		for(j=0; j<m_iRank-1-i*2; ++j)
		{
			pBlockTmp = GetBlock(m_iRank-1-index, i, i+j);
			pBlockTmp = ReplaceBlock(m_iRank-1-index, m_iRank-1-i-j, i, pBlockTmp);
			pBlockTmp = ReplaceBlock(m_iRank-1-index, m_iRank-1-i, m_iRank-1-i-j, pBlockTmp);
			pBlockTmp = ReplaceBlock(m_iRank-1-index, i+j, m_iRank-1-i, pBlockTmp);
			ReplaceBlock(m_iRank-1-index, i, i+j, pBlockTmp);
		}

		// 如果不是最外层，只需要交换一次
		if(index!=0 && index!=m_iRank-1)
			break;
	}
}

CBlock* CCube::GetBlock(int i, int j, int k)
{
	return m_pBlocks[k*m_iRank*m_iRank + j*m_iRank + i];
}

CBlock* CCube::ReplaceBlock(int i, int j, int k, CBlock *b)
{
	CBlock *pBlock = m_pBlocks[k*m_iRank*m_iRank + j*m_iRank + i];
	m_pBlocks[k*m_iRank*m_iRank + j*m_iRank + i] = b;

	return pBlock;
}

CBlock* CCube::GetRotBlock(int x, int y, int z)
{
	if(m_iRotIndex<0)
		return (CBlock*)0;

	unsigned int i;
	for(i=0; i<m_vecRotateAnimInfo.size(); ++i)
	{
		if((x+y*m_iRank+z*m_iRank*m_iRank)==m_vecRotateAnimInfo[i].index)
			return m_vecRotateAnimInfo[i].pBlock;

	}
	return (CBlock*)0;
}

void CCube::Tick(int iTime)
{
	if (m_bUpseting)
	{
		if (m_iRotIndex<0)
		{
			int iRandRotType = rand() % 6;
			int iRandIndex = rand() % m_iRank;
			StartRot((RotateType)iRandRotType, iRandIndex);
			m_iUpsetNum++;
			if (m_iUpsetNum>5*m_iRank)
				m_bUpseting = false;
		}
	}

	if (m_vecRotateInfo.size()>0)
	{
		if (m_iRotIndex<0)
		{
			StartRot(m_vecRotateInfo[0].type, m_vecRotateInfo[0].index);
			m_vecRotateInfo.erase(m_vecRotateInfo.begin());
		}
	}

	if(m_iRotIndex<0)
		return;

	m_fRotAngle += D3DX_PI*iTime/m_fSpeed/2;
	if(m_fRotAngle>D3DX_PI/2)
	{		
		switch(m_iRotType)
		{
		case RT_F:
			F(m_iRotIndex);
			break;
		case RT_B:
			B(m_iRotIndex);
			break;
		case RT_U:
			U(m_iRotIndex);
			break;
		case RT_D:
			D(m_iRotIndex);
			break;
		case RT_L:
			L(m_iRotIndex);
			break;
		case RT_R:
			R(m_iRotIndex);
			break;
		default:
			break;
		}

		m_iRotIndex = -1;
		return;
	}
	unsigned int i;
	for(i=0; i<m_vecRotateAnimInfo.size(); ++i)
		m_vecRotateAnimInfo[i].pBlock->Rotate(m_iRotType, D3DX_PI*iTime/m_fSpeed/2);
}

int CCube::GetFIndex(float z)
{
	float fBlockLen = m_fLength/m_iRank;
	return int((z+m_fLength/2)/fBlockLen);
}

int CCube::GetBIndex(float z)
{
	float fBlockLen = m_fLength/m_iRank;
	return int((m_fLength/2-z)/fBlockLen);
}

int CCube::GetUIndex(float y)
{
	float fBlockLen = m_fLength/m_iRank;
	return int((m_fLength/2-y)/fBlockLen);
}

int CCube::GetDIndex(float y)
{
	float fBlockLen = m_fLength/m_iRank;
	return int((y+m_fLength/2)/fBlockLen);
}

int CCube::GetLIndex(float x)
{
	float fBlockLen = m_fLength/m_iRank;
	return int((x+m_fLength/2)/fBlockLen);
}

int CCube::GetRIndex(float x)
{
	float fBlockLen = m_fLength/m_iRank;
	return int((m_fLength/2-x)/fBlockLen);
}

void CCube::ExeFormula(CString str, int num)
{
	if (!CanOpt())
		return;

	int i;
	for (i=0; i<str.GetLength();)
	{
		RotateInfo ri;
		switch (str[i])
		{
		case 'f':
		case 'F':
			ri.type = RT_F;
			break;
		case 'b':
		case 'B':
			ri.type = RT_B;
			break;
		case 'u':
		case 'U':
			ri.type = RT_U;
			break;
		case 'd':
		case 'D':
			ri.type = RT_D;
			break;
		case 'l':
		case 'L':
			ri.type = RT_L;
			break;
		case 'r':
		case 'R':
			ri.type = RT_R;
			break;
		}
		if (i<(str.GetLength()-1) && str[i+1]>='1' && str[i+1]<='9')
		{
			ri.index = str[i+1]-'0';
			i++;
		}
		else
		{
			ri.index = 0;
		}

		if (i<(str.GetLength()-1) &&str[i+1]=='\'')
		{
			int tmp = ri.type%2;
			if(tmp==0)
				ri.type = (RotateType)((int)ri.type+1);
			else
				ri.type = (RotateType)((int)ri.type-1);
			ri.index = m_iRank-1;
			i++;
		}

		i++;
		m_vecRotateInfo.push_back(ri);
	}

	std::vector<RotateInfo> vecTmp = m_vecRotateInfo;
	for (i=0; i<num-1; ++i)
	{
		unsigned int j;
		for (j=0; j<vecTmp.size(); ++j)
		{
			m_vecRotateInfo.push_back(vecTmp[j]);
		}
	}

}
