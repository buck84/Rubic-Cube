/*
 * =====================================================================
 *
 * FileName:    Cube.h
 * Author:      Buck
 * Email:       Buck88@gmail.com
 * Created:     2013.3.21
 * Environment: Visual Studio 2008
 *
 * Description: ħ��������
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

	// rank: ����ħ��
	// len: cube�߳�
	// speed: �೤ʱ��ת��90�㣬��λms
	void Init(int rank=3, float len=6.0f, float speed=200);
	void Release();

	void Upset() { srand((unsigned int)time(NULL)); m_bUpseting=true; m_iUpsetNum=0; }
	void ExeFormula(CString str, int num);
	bool CanOpt() { return !m_bUpseting && m_vecRotateInfo.size()==0; }

	// ��ħ����Ⱦ��Ϣ��ӵ���Ⱦ������
	void FillCubeVertexInfo(void *vertices, WORD *indices, int &vNum, int &iNum);

	// ��ʼ����ת��������������ɽ���ʵ��ת��
	void StartRot(RotateType type, int index=0);

	// index:�ڼ����
	// ʵ�ʿ�ת������
	void F(int index = 0);
	void B(int index = 0);
	void U(int index = 0);
	void D(int index = 0);
	void L(int index = 0);
	void R(int index = 0);

	// ��ʱִ�е��߼�
	// Ŀǰֻ�ǲ���ת������
	void Tick(int iTime);

	// �����������غ���
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
	// ����λ�ÿ��Ƿ�����ת����������򷵻أ����򷵻ؿ�
	CBlock* GetRotBlock(int x, int y, int z);
private:

	/* ���ħ���ķ���
	 * ���淽��ָ��Ϊ�գ���Ϊ����Ҫ��ʾ
	 * ����z���������
	 * ǰ���½�ΪGetBlock(      0,      0,      0)
	 * ǰ���½�ΪGetBlock(m_iRank,      0,      0)
	 * ǰ���Ͻ�ΪGetBlock(m_iRank,m_iRank,      0)
	 * �����Ͻ�ΪGetBlock(m_iRank,m_iRank,m_iRank)
	y     z
	|    /
	|   /
	|  /
	| /
	|/
	------------x
    0->

	����(0, 0, 0)
	����(m_iRank-1, m_iRank-1, m_iRank-1)
	*/
	CBlock		**m_pBlocks;
	int			m_iRank;		// ����ħ��
	float		m_fLength;		// ħ����ʾ��С

	// ת���������
	float		m_fSpeed;
	std::vector<RotateAnimInfo>	m_vecRotateAnimInfo;
	RotateType	m_iRotType;			// ת����һ��
	int			m_iRotIndex;		// ת��һ��ĵڼ���(���ڸ߽�ħ��)
	float		m_fRotAngle;		// ��ǰ��ת�Ƕ�(����ת������)

	bool		m_bUpseting;
	int			m_iUpsetNum;
	std::vector<RotateInfo>		m_vecRotateInfo;
};
