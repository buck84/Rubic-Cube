/*
* =====================================================================
*
* FileName:    RubicCubeDlg.h
* Author:      Buck
* Email:       Buck88@126.com
* Created:     2013.3.21
* Environment: Visual Studio 2008
*
* Description: Dialog��
*
* ======================================================================
*/

#pragma once
#include "afxwin.h"

class CCube3D;

// CRubicCubeDlg �Ի���
class CRubicCubeDlg : public CDialog
{
// ����
public:
	CRubicCubeDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_RUBICCUBE_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��

private:
	CCube3D	*m_pCube3D;

// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnDestroy();
	CComboBox m_comboRank;
	afx_msg void OnBnClickedButtonReset();
	afx_msg void OnCbnSelchangeComboRank();
	afx_msg void OnBnClickedButtonUpset();
	CString m_strFormula;
	int m_iExecuteNum;
	afx_msg void OnBnClickedButtonExecute();
};
