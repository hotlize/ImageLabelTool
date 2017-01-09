// CPaintDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "CPaintDlg.h"
#include "afxdialogex.h"
#include "CVideoLabelImageFile.h"

// CPaintDlg �Ի���

IMPLEMENT_DYNAMIC(CPaintDlg, CDialogEx)

CPaintDlg::CPaintDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CPaintDlg::IDD, pParent)
{

}

CPaintDlg::~CPaintDlg()
{
}
BOOL CPaintDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	SetWindowLong(this->GetSafeHwnd(), GWL_EXSTYLE, GetWindowLong(this->GetSafeHwnd(), GWL_EXSTYLE) ^ WS_EX_LAYERED);
	//SetWindowPos(this->m_hWnd,HWND_TOPMOST,10,10,10,10,SWP_NOMOVE|SWP_NOSIZE);
	COLORREF clr = RGB(0, 0, 0);
	//LWA_COLORKEY:��ɫΪclr�ĵط������͸���������ط�������ʾ��AlphaֵʧЧ��������˼�ǳ��˺�ɫ����������ʾ
	SetLayeredWindowAttributes(clr, 128, LWA_COLORKEY);//LWA_COLORKEY LWA_ALPHA     //ͨ�����������������ô���͸���̶�
	isMouseDown = FALSE;
	m_toolTip.Create(this);
	m_toolTip.SetTipTextColor(0x00FFCC);
	EnableToolTips();
	//m_toolTip.SetDelayTime(2000);
	return TRUE;  // return TRUE unless you set the focus to a control
}

inline void CPaintDlg::DrawLabel()
{
	//�����߼�����
	CDC * pDC = GetDC();
	CPen newPen;
	BYTE usR = abs(rand()) % 256;
	BYTE usG = abs(rand()) % 256;
	BYTE usB = abs(rand()) % 256;

	
	list<CLabel>::iterator it;
	if (!m_newRect.IsRectEmpty())
	{
		newPen.CreatePen(PS_SOLID, 1, RGB(usR, usG, usB));
		pDC->SelectObject(&newPen);
		pDC->MoveTo(m_newRect.left, m_newRect.top);
		pDC->LineTo(m_newRect.left, m_newRect.bottom);
		pDC->LineTo(m_newRect.right, m_newRect.bottom);
		pDC->LineTo(m_newRect.right, m_newRect.top);
		pDC->LineTo(m_newRect.left, m_newRect.top);
	}
	for (it = m_tmpImg.labelList.begin(); it != m_tmpImg.labelList.end(); it++)
	{
		CPen tmpPen;
		COLORREF col = (*it).color;
		tmpPen.CreatePen(PS_SOLID, 2,col);
		pDC->SelectObject(&tmpPen);
		FRect fRect = (*it).rect;
		CRect rect = GetLabelCRect(fRect);
		pDC->MoveTo(rect.left, rect.top);
		pDC->LineTo(rect.left, rect.bottom);
		pDC->LineTo(rect.right, rect.bottom);
		pDC->LineTo(rect.right, rect.top);
		pDC->LineTo(rect.left, rect.top);
	}
	ReleaseDC(pDC);
}
void CPaintDlg::OnPaint()
{
	CDialogEx::OnPaint();
	//���ƿ��
	CDC * pDC = GetDC();
	HDC hDC = NULL;
	hDC = pDC->m_hDC;
	CRect rect;
	GetClientRect(&rect);
	CBrush bs;
	bs.CreateSolidBrush(RGB(0, 0, 0));
	pDC->FillRect(rect, &bs);
	ReleaseDC(pDC);
	DrawLabel();
}

void CPaintDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CPaintDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()


// CPaintDlg ��Ϣ�������

void CPaintDlg::SetPoint(CPoint point)
{
	if (isMouseDown){
		m_pointB = point;
		m_newRect.SetRect(m_pointA, m_pointB);
		m_newRect.NormalizeRect();
	}
	else{
		m_pointA = point;
		CPoint tmp = m_pointA;
		tmp.Offset(3,3);
		m_newRect.SetRectEmpty();
	}
	Invalidate();
}
void CPaintDlg::SetPoint(long x, long y)
{
	if (isMouseDown){
		m_pointB.x = x;
		m_pointB.y = y;
		m_newRect.SetRect(m_pointA, m_pointB);
		m_newRect.NormalizeRect();
	}
	else{
		m_pointA.x = x;
		m_pointA.y = y;
		CPoint tmp = m_pointA;
		tmp.Offset(3, 3);
		m_newRect.SetRectEmpty();
	}
	Invalidate();
}

FRect CPaintDlg::GetLabelFRect(CRect rect)
{
	//�˺�������ԭ��Ƶ��С��windowSize�����㻭������Ƶ�е���Գߴ硣
	//return: ������0��1�ķ�Χ��С����
	CRect windowSize;
	FRect relativeRect;
	int validHeight;
	int validWidth;
	int validTop;
	int validBottom;
	int validLeft;
	int validRight;
	GetClientRect(&windowSize);
	int windowHeight = windowSize.Height();
	int windowWidth = windowSize.Width(); 
	int videoHeight = m_videoSize.cy;
	int videoWidth = m_videoSize.cx;
	double standardK = videoHeight*1.0 / videoWidth;
	if (standardK < windowHeight*1.0 / windowWidth)
	{
		//������տ�����(��̶����߶����)
		validHeight = (int)(windowWidth*standardK);
		validWidth = windowWidth;
		validTop = (windowHeight - validHeight) / 2;
		validBottom = validTop + validHeight;
		validLeft = 0;
		validRight = windowWidth;
		relativeRect.top = (float)((rect.top - validTop)*1.0 / validHeight);
		relativeRect.bottom = (float)((rect.bottom - validTop)*1.0 / validHeight);
		relativeRect.left = (float)(rect.left*1.0 / validWidth);
		relativeRect.right = (float)(rect.right*1.0 / validWidth);
	}
	else
	{
		//������ո�����(�߹̶���������)
		validHeight = windowHeight;
		validWidth = (int)(windowHeight / standardK);
		validTop = 0;
		validBottom = windowHeight;
		validLeft = (windowWidth - validWidth) / 2;
		validRight = validLeft + validWidth;
		relativeRect.top = (float)(rect.top*1.0 / validHeight);
		relativeRect.bottom = (float)(rect.bottom*1.0 / validHeight);
		relativeRect.left = (float)((rect.left - validLeft)*1.0 / validWidth);
		relativeRect.right = (float)((rect.right - validLeft)*1.0 / validWidth);
	}
	return relativeRect;
}

CRect CPaintDlg::GetLabelCRect(FRect frect)
{
	//���PaintDlg�ϵľ���λ��
	CRect rect;
	CRect windowSize;
	int validHeight;
	int validWidth;
	int validTop;
	int validBottom;
	int validLeft;
	int validRight;
	GetClientRect(&windowSize);
	int windowHeight = windowSize.Height();
	int windowWidth = windowSize.Width();
	int videoHeight = m_videoSize.cy;
	int videoWidth = m_videoSize.cx;
	double standardK = videoHeight*1.0 / videoWidth;
	if (standardK < windowHeight*1.0 / windowWidth)
	{
		//������տ�����(��̶����߶����)
		validHeight = (int)(windowWidth*standardK);
		validWidth = windowWidth;
		validTop = (windowHeight - validHeight) / 2;
		validBottom = validTop + validHeight;
		validLeft = 0;
		validRight = windowWidth;
	}
	else
	{
		//������ո�����(�߹̶���������)
		validHeight = windowHeight;
		validWidth = (int)(windowHeight / standardK);
		validTop = 0;
		validBottom = windowHeight;
		validLeft = (windowWidth - validWidth) / 2;
		validRight = validLeft + validWidth;
	}
	rect.left = validLeft + validWidth*frect.left;
	rect.right = validLeft + validWidth*frect.right;
	rect.top = validTop + validHeight*frect.top;
	rect.bottom = validTop + validHeight*frect.bottom;
	return rect;
}

void CPaintDlg::OnMouseMove(CPoint& screenPoint)
{
	CPoint point = screenPoint;
	ScreenToClient(&point);
	list<CLabel>::iterator it;
	for (it = m_tmpImg.labelList.begin(); it != m_tmpImg.labelList.end(); it++)
	{
		CRect rect = GetLabelCRect((*it).rect);
		if (point.x<rect.right&&point.x>rect.left && point.y<rect.bottom&&point.y>rect.top)
		{
			CRect rect;
			rect.SetRect(point.x,point.y,point.x+200,point.y+30);
			//m_toolTip.AddTool(this, it->name,&rect);
			//m_toolTip.SetToolInfo(it->name);
		}
	}
}
