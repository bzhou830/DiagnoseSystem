/**********************************************************************************
  Copyright @ Robin 2015-2025
  File name:  ImgInfo.cpp
  Author: Robin   Version: V1.0   Date:2015.11.05 
  Description:  Displayed layers about data besides image pixles 
				dicom image's relative data such as: Patient name 
***********************************************************************************/
#include "StdAfx.h"
#include "ImgInfoLayer.h"


CImgInfoLayer::CImgInfoLayer(void)
{
	//memset(&m_info, 0, sizeof(m_info));
	isEmpty = TRUE;
}

CImgInfoLayer::~CImgInfoLayer(void)
{
}

/***************************************************************************************
Function: ����ͼ��������ݲ�����
Input:    ͼ��������ݽṹ��
***************************************************************************************/
void CImgInfoLayer::SetLayerInfoData(sOneImgInfo info)
{
	m_info = info;
	isEmpty = FALSE;
}

/***************************************************************************************
Function: ��ȡ��ǰͼ��������ݲ�����
Input:    NULL
Output:  ͼ��������ݽṹ��
***************************************************************************************/
sOneImgInfo CImgInfoLayer::GetLayerInfoData()
{
	return m_info;
}

/***************************************************************************************
Function: ͼ��������ݻ���
Input:    CDC* ���Ƶ�Ŀ��DC  CRect ����DC���������
***************************************************************************************/
void CImgInfoLayer::OnDraw(CDC* pDC, CRect rect)
{
	if(!m_info.isDisplay)
		return;
	CRect rc = rect;
	pDC->SetTextColor(RGB(255, 128, 0));
	pDC->SetBkMode(TRANSPARENT);
	CFont ft;
	ft.CreateFont(12, 0, 0, 0, FW_MEDIUM, FALSE, FALSE, 0, GB2312_CHARSET, OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, "����");  //��������
	pDC->SelectObject(ft);
	if (isEmpty)				//ͼ�����ݲ�Ϊ��ʱ��д����
		return;
	//д������Ϣ
	CRect rc1(3, 3, 300, 300);												//���Ͻ�
	CRect rc2(rc.Width() - 303, 3, rc.Width() - 3, 300);					//���Ͻ�
	CRect rc3(3, rc.Height() - 15*3, 300, rc.Height());						//���½�
	CRect rc4(rc.Width()-303, rc.Height()- 15*2, rc.Width(), rc.Height());  //���½�

	CString txt1 = "����豸: " + m_info.sInstitutionName + "\r\n" +
		           "����: " + m_info.sName + "\r\n" +
				   "�Ա�: " + m_info.sSex + "\r\n" +
				   "ID��: " + m_info.sId + "\r\n" +
				   "����: " + m_info.sAge + "\r\n";

	CString tmp;
	tmp.Format("%.2lf",m_info.slicesickness);
	CString txt2 = "����: " + m_info.sWindowWidth + " \r\n" +
		           "��λ: " + m_info.sWindowCenter + " \r\n" +
				   "���: " + tmp + "mm\r\n";

	CString txt3 = "RL: " + CType::double2CString(m_info.nPositionX) 
		           + "\r\nAP: " + CType::double2CString(m_info.nPositionY)  
				   + "\r\nHF: " + CType::double2CString(m_info.nPositionZ)
				   + "\r\n";

	CString txt4 = "\nUESTC-Robin\n";

	DrawMultLineText(pDC, rc1, 2, DT_LEFT, txt1);
	DrawMultLineText(pDC, rc2, 2, DT_RIGHT, txt2);
	DrawMultLineText(pDC, rc3, 2, DT_LEFT, txt3);
	pDC->DrawText(txt4, &rc4, DT_RIGHT);
}

/*===========================================================================*
* ��ָ��DC��д��������
*============================================================================*/
void CImgInfoLayer::DrawMultLineText(CDC *pDC, CRect rect, int nRowDis, UINT nFromat, CString strText)
{
	int len = strText.GetLength();
	int j = 0;
	for(int i = 0; i < len; i++)
	{
		if(strText[i] != '\n')
			continue;
		CString str  = strText.Mid(j, i - j);
		str += '\n';
		pDC->DrawText(str, &rect, nFromat);
		rect.top += (12 + nRowDis);
		j = i+1;
	}
}


/***************************************************************************************
Function: ������������Ϣ��Ӧ
Input:    �����λ�ü������Ϣ
***************************************************************************************/
void CImgInfoLayer::OnLButtonDown(UINT nFlags, CPoint point)
{
}


/***************************************************************************************
Function: ������̧����Ϣ��Ӧ
Input:    ���̧��ʱλ��
***************************************************************************************/
void CImgInfoLayer::OnLButtonUp(UINT nFlags, CPoint point)
{
}


/***************************************************************************************
Function: �ַ����ָ��
Input:    ������ָ���ַ��� �ָ��ַ� �ָ�õ��Ľṹ��Ŀ
Output:   �ָ�õ����ַ�������
***************************************************************************************/
CString* CImgInfoLayer::SplitString(CString str, char split, int& iSubStrs)
{
	int iPos = 0;				//�ָ��λ��
	int iNums = 0;				//�ָ��������
	CString strTemp = str;
	CString strRight;
	//�ȼ������ַ���������
	while (iPos != -1)
	{
		iPos = strTemp.Find(split);
		if (iPos == -1)
		{
			break;
		}
		strRight = strTemp.Mid(iPos + 1, str.GetLength());
		strTemp = strRight;
		iNums++;
	}
	if (iNums == 0)				//û���ҵ��ָ��
	{
		//���ַ����������ַ�������
		iSubStrs = 1; 
		return NULL;
	}
	//���ַ�������
	iSubStrs = iNums + 1; //�Ӵ������� = �ָ������ + 1
	CString* pStrSplit;
	pStrSplit = new CString[iSubStrs];
	strTemp = str;
	CString strLeft;
	for (int i = 0; i < iNums; i++)
	{
		iPos = strTemp.Find(split);
		//���Ӵ�
		strLeft = strTemp.Left(iPos);
		//���Ӵ�
		strRight = strTemp.Mid(iPos + 1, strTemp.GetLength());
		strTemp = strRight;
		pStrSplit[i] = strLeft;
	}
	pStrSplit[iNums] = strTemp;
	return pStrSplit;
}


