
// ImageLabelDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ImageLabel.h"
#include "ImageLabelDlg.h"
#include "afxdialogex.h"
#include "EventController.h"
#include "ImageLabelFile.h"
#include "LabelXMLFile.h"
#include "CvvImage.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()

CString CImageLabelDlg::GetPoseRadioString(int value)
{
	CString pose;
	switch (value)
	{
	case 0:
		pose = _T("Unspecified");
		break;
	case 1:
		pose = _T("SideFaceLeft");
		break;
	case 2:
		pose = _T("SideFaceRight");
		break;
	case 3:
		pose = _T("Frontal");
		break;
	case 4:
		pose = _T("Rear");
	default:
		break;
	}
	return pose;
}
CString CImageLabelDlg::GetShotRadioString(int value)
{
	CString shot;
	switch (value)
	{
	case 0:
		shot = _T("Unspecified");
		break;
	case 1:
		shot = _T("CloseShot");
		break;
	case 2:
		shot = _T("Longshot");
		break;
	default:
		break;
	}
	return shot;
}
TCITEMW CImageLabelDlg::GetTabSelectItem(CTabCtrl& tabCtrl, TCHAR *buffer)
{
	TCITEMW tabItem;
	tabItem.cchTextMax = 256;
	tabItem.pszText = buffer;
	tabItem.mask = TCIF_TEXT;
	tabCtrl.GetItem(tabCtrl.GetCurSel(), &tabItem);
	return tabItem;
}

// CImageLabelDlg �Ի���

CImageLabelDlg::CImageLabelDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CImageLabelDlg::IDD, pParent)
	, m_radio_pose(0)
	, m_bTruncated(FALSE)
	, m_bDifficult(FALSE)
	, m_radio_shot(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CImageLabelDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB1, m_tab_label);
	DDX_Control(pDX, IDC_TREE1, m_tre_file);
	DDX_Control(pDX, IDC_TREE2, m_tre_label);
	DDX_Control(pDX, IDC_LST_SHOW, m_lst_label);
	DDX_Control(pDX, IDC_STT_PIC, m_picture);
	DDX_Control(pDX, IDC_STT_RECT, m_rectangle);
	DDX_Control(pDX, IDC_VOLUME_COLOR, m_volColor);

	DDX_Control(pDX, IDC_BT_ADD, m_bt_add);
	DDX_Radio(pDX, IDC_RD_POSE_UNSPECIFY, m_radio_pose);
	DDX_Radio(pDX, IDC_RD_SISTANCE_UNSPECIFIED, m_radio_shot);
	DDX_Check(pDX, IDC_CHK_TRUNCATED, m_bTruncated);
	DDX_Check(pDX, IDC_CHK_DIFFICULT, m_bDifficult);
}

BEGIN_MESSAGE_MAP(CImageLabelDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_COMMAND(ID_32771, &CImageLabelDlg::OnPressImportImgDir)
	ON_COMMAND(ID_32775, &CImageLabelDlg::OnPressImportLabel)
	ON_COMMAND(ID_32778, &CImageLabelDlg::OnPressDelLabel)
	ON_COMMAND(ID_32779, &CImageLabelDlg::OnPressExportVOCXML)
	ON_COMMAND(ID_ACC_ADDLABEL,&CImageLabelDlg::OnBnClickedBtAdd)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, &CImageLabelDlg::OnTcnSelchangeLabelTab)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE1, &CImageLabelDlg::OnTvnSelchangedTreFile)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE2, &CImageLabelDlg::OnTvnSelchangedTreLabel)
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	//color button	
	ON_MESSAGE(CPN_SELENDOK, OnSelEndOK)
	ON_MESSAGE(CPN_SELENDCANCEL, OnSelEndCancel)
	ON_MESSAGE(CPN_SELCHANGE, OnSelChange)
	ON_MESSAGE(CPN_CLOSEUP, OnCloseUp)
	ON_MESSAGE(CPN_DROPDOWN, OnDropDown)
	ON_BN_CLICKED(IDC_BT_ADD, &CImageLabelDlg::OnBnClickedBtAdd)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_LST_SHOW, OnCustomdrawListShow)
	ON_NOTIFY(NM_RCLICK, IDC_LST_SHOW, &CImageLabelDlg::OnNMRClickLstShow)
	ON_NOTIFY(NM_RCLICK, IDC_TREE1, &CImageLabelDlg::OnNMRClickTreeFile)
	ON_COMMAND(ID__32781, &CImageLabelDlg::OnPressDelFile)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_TREE1, &CImageLabelDlg::OnNMCustomdrawTreeFile)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_TREE2, &CImageLabelDlg::OnNMCustomdrawTreeLabel)
	ON_COMMAND(ID_32782, &CImageLabelDlg::OnPressExportNIMIObjectDetectionXML)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LST_SHOW, &CImageLabelDlg::OnLvnItemchangedLstShow)
	ON_COMMAND(ID__32783, &CImageLabelDlg::OnPressModifyLabel)
	ON_COMMAND(ID_32786, &CImageLabelDlg::OnSaveLabelInfoXML)
END_MESSAGE_MAP()




// CImageLabelDlg ��Ϣ�������

BOOL CImageLabelDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO:  �ڴ���Ӷ���ĳ�ʼ������
	CMenu menu;
	menu.LoadMenu(IDR_MENU1);
	SetMenu(&menu);
	m_rect_pic.SetRect(CPoint(0, 0), CPoint(960, 580));

	//color button
	m_volColor.Color = RGB(255, 255, 0);
	m_volColor.DefaultColor = RGB(255, 255, 0);
	m_volColor.TrackSelection = TRUE;
	m_volColor.CustomText = _T("������ɫ...");
	m_volColor.DefaultText = _T("�Զ�");

	m_lst_label.DeleteAllItems();
	while (m_lst_label.DeleteColumn(0));
	m_lst_label.InsertColumn(0, _T("Index"), LVCFMT_CENTER, 100);
	m_lst_label.InsertColumn(1, _T("Domain"), LVCFMT_CENTER, 100);
	m_lst_label.InsertColumn(2, _T("Type"), LVCFMT_CENTER, 100);
	m_lst_label.InsertColumn(3, _T("Label"), LVCFMT_CENTER, 200);
	m_lst_label.InsertColumn(4, _T("Truncated"), LVCFMT_CENTER, 100);
	m_lst_label.InsertColumn(5, _T("Difficult"), LVCFMT_CENTER, 100);
	m_lst_label.InsertColumn(6, _T("Pose"), LVCFMT_CENTER, 100);
	m_lst_label.InsertColumn(7, _T("Shot"), LVCFMT_CENTER, 100);
	m_lst_label.InsertColumn(8, _T("Color"), LVCFMT_CENTER, 100);
	m_lst_label.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_HEADERDRAGDROP);//������ק

	m_hAcc = LoadAccelerators(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_ACCELERATOR1));
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CImageLabelDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CImageLabelDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;
		
		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CImageLabelDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

BOOL CImageLabelDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO:  �ڴ����ר�ô����/����û���

	//��ʱû�����ã��϶�ʱ���ˢ��
	if (WM_KEYFIRST <= pMsg->message&&pMsg->message <= WM_KEYLAST)
	{
		HACCEL hAccel = m_hAcc;
		if (hAccel && ::TranslateAccelerator(m_hWnd, hAccel, pMsg))
		{
			return TRUE;
		}
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}


void CImageLabelDlg::OnPressImportImgDir()
{
	// TODO:  �ڴ���������������
	CString imageDir = CEventController::GetInstance()->OnImportImageDirectory();
	if (!PathFileExistsW(imageDir))
	{
		AfxMessageBox(_T("·������"));
	}
	else
	{
		//prepare
		m_tre_file.DeleteAllItems();
		CImageLabelFileIOController* fileController = CImageLabelFileIOController::GetInstance();
		fileController->SetFolderPath(imageDir);
		fileController->SetStorePath(imageDir + _T("\\data.ilf.xml"));

		//�����ļ���
		HTREEITEM hRoot = m_tre_file.InsertItem(imageDir);
		InitImageFileTreeView(hRoot);
		m_tre_file.Expand(hRoot, TVE_EXPAND);
		if (fileController->ReadFileFromXML() == FALSE)
		{
			AfxMessageBox(_T("ԴĿ¼û�б�ע��¼�ļ�Ӵ�����ڴ��㿪ʼ��"));
		}
	}
}
void CImageLabelDlg::OnPressImportLabel()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	if (CEventController::GetInstance()->OnImportLabelFile(m_labelXMLFileNode) == -1)
	{
		AfxMessageBox(_T("����XML��ǩ�ļ�ʧ�ܣ�"));
		return;
	}
	if (m_labelXMLFileNode.hasChildren == 0)
		return;
	//ˢ�½���

	//��ǩҳˢ�¡���ǩ���νṹˢ��
	TCITEM item;
	item.mask = TCIF_TEXT;
	m_tab_label.DeleteAllItems();
	int index = 0;
	for (list<LabelXMLFileNode>::iterator it = m_labelXMLFileNode.childrenList.begin(); it != m_labelXMLFileNode.childrenList.end(); it++)
	{
		//��ǩtab�ؼ�����
		item.pszText = it->value.LockBuffer();
		m_tab_label.InsertItem(index++, &item);
	}
	//���õ�һ��ҳ�汻����
	m_tab_label.SetCurSel(0);
	//�������ú���ˢ��ҳ��
	OnTcnSelchangeLabelTab(NULL, NULL);
}
void CImageLabelDlg::OnPressExportVOCXML()
{
	CEventController::GetInstance()->OnExportVOCXML();
}
void CImageLabelDlg::OnPressExportNIMIObjectDetectionXML()
{
	CEventController::GetInstance()->OnExportNIMIDetectionXML();
}


void CImageLabelDlg::InitImageFileTreeView(HTREEITEM hparent)
{
	CString strText = GetImageFileTreePath(hparent, TRUE);  //�����б�����Ŀ����  
	if (strText.Right(1) != "\\")                   //���ұ�1��ʼ��ȡ��������nCount���ַ�  
		strText += _T("\\");
	strText += _T("*.*");
	//����ǰĿ¼���ļ�ö�ٲ�InsertItem��״��ʾ  
	CFileFind file;                                       //���屾���ļ�����  
	BOOL bContinue = file.FindFile(strText);              //���Ұ����ַ������ļ�  
	while (bContinue)
	{
		bContinue = file.FindNextFile();                  //������һ���ļ�  
		if (!file.IsDirectory())          //�ҵ��ļ�Ϊ�����Ҳ�Ϊ��"."  
		{
			CString fileName = file.GetFileName();
			CString postFix = fileName.Right(3);
			if (postFix == "jpg" || postFix == "JPG" || postFix == "PNG" || postFix == "BMP" || postFix == "bmp" || postFix == "png")
			{
				HTREEITEM hItem = m_tre_file.InsertItem(fileName, hparent); //����̷�·������״�ļ���  
				fileName = GetImageFileTreePath(hItem, FALSE);
				CImageLabelFileIOController::GetInstance()->AddImageFile(fileName);
			}

		}
		else if (file.IsDirectory() && !file.IsDots())
		{
			CString fileName = file.GetFileName();
			HTREEITEM hItem = m_tre_file.InsertItem(fileName, hparent);
			InitImageFileTreeView(hItem);
		}

	}
	//OnTreeView(hparent, m_tre_filePath.GetItemText(hparent));								//�ڶ�������ݹ����
}
CString CImageLabelDlg::GetImageFileTreePath(HTREEITEM htreeItem,BOOL rootNeeded)
{
	CString path = m_tre_file.GetItemText(htreeItem);
	HTREEITEM parent = m_tre_file.GetParentItem(htreeItem);
	if (rootNeeded)
	{
		while (parent != NULL)
		{
			path = m_tre_file.GetItemText(parent) + _T("\\") + path;
			parent = m_tre_file.GetParentItem(parent);
		}
	}
	else
	{
		while (m_tre_file.GetParentItem(parent)!=NULL)
		{
			path = m_tre_file.GetItemText(parent) + _T("\\") + path;
			parent = m_tre_file.GetParentItem(parent);
		}
	}
	
	return path;
}
void CImageLabelDlg::OnRefreshLabelTreeViewCtrl(HTREEITEM hTreeItem, LabelXMLFileNode node)
{
	HTREEITEM hNewItem;
	if (hTreeItem == NULL)
		hNewItem = m_tre_label.InsertItem(node.value);
	else{
		hNewItem = m_tre_label.InsertItem(node.value, hTreeItem);
		m_tre_label.Expand(hTreeItem, TVE_EXPAND);
	}
	if (node.hasChildren)
	{
		for (list<LabelXMLFileNode>::iterator it = node.childrenList.begin(); it != node.childrenList.end(); it++)
		{
			OnRefreshLabelTreeViewCtrl(hNewItem, *it);
		}
	}
}

void CImageLabelDlg::OnTcnSelchangeLabelTab(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	int index = m_tab_label.GetCurSel();
	//Ѱ�ұ�ǩҳ������
	int i = 0;
	list<LabelXMLFileNode>::iterator it=m_labelXMLFileNode.childrenList.begin();
	while (i < index)
	{
		i++;
		it++;
	}
	//��������
	m_tre_label.DeleteAllItems();
	list<LabelXMLFileNode>::iterator itt = it->childrenList.begin();
	while (itt != it->childrenList.end())
	{
		OnRefreshLabelTreeViewCtrl(NULL, *itt);
		itt++;
	}
}
void CImageLabelDlg::OnTvnSelchangedTreFile(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;
	CString imgPath = GetImageFileTreePath(m_tre_file.GetSelectedItem(),TRUE);
	m_imageMat.release();
	m_rect_draw.SetRectEmpty();
	OnRefreshListShowCtrl();
	m_bIsPicShow = ShowImage(m_picture.GetDC(), imgPath, m_rect_pic.Width(), m_rect_pic.Height());
	//AfxMessageBox(imgPath);
}
void CImageLabelDlg::OnTvnSelchangedTreLabel(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;
}
void CImageLabelDlg::OnRefreshListShowCtrl()
{
	m_lst_label.DeleteAllItems();
	HTREEITEM pItem = m_tre_file.GetSelectedItem();
	CString fileName = GetImageFileTreePath(pItem, FALSE);

	CImageLabelFileIOController* _instance = CImageLabelFileIOController::GetInstance();
	list<CImageFile>::iterator it = _instance->GetIteratorOfFile(fileName);
	if (it == _instance->imageFileList.end())
		return;
	list<CLabel>::iterator itt;
	CString color;
	CString index;
	int i = 0;
	for (itt = (*it).labelList.begin(); itt != (*it).labelList.end(); itt++)
	{
		index.Format(_T("%d"), i);
		m_lst_label.InsertItem(i, index);
		m_lst_label.SetItemText(i, 1, itt->domain);
		m_lst_label.SetItemText(i, 2, itt->type);
		m_lst_label.SetItemText(i, 3, itt->name);
		m_lst_label.SetItemText(i, 4, itt->bTruncated ? _T("�ڵ�") : _T("���ڵ�"));
		m_lst_label.SetItemText(i, 5, itt->bDifficult ? _T("��") : _T("һ��"));
		CString pose = itt->pose;
		if (pose == _T("Frontal"))
		{
			pose = _T("����");
		}
		if (pose == _T("Rear"))
		{
			pose = _T("����");
		}
		m_lst_label.SetItemText(i, 6, pose);
		CString shot = it->shot;
		if (shot == _T("CloseShot"))
		{
			shot = _T("����");
		}
		if (shot == _T("LongShot"))
		{
			shot = _T("Զ��");
		}
		m_lst_label.SetItemText(i, 7, shot);
		color.Format(_T("%ul"), itt->color);
		m_lst_label.SetItemText(i, 8,color);
		i++;
	}
	m_lst_label.EnsureVisible(m_lst_label.GetItemCount() - 1, FALSE);
	m_lst_label.RedrawItems(0, m_lst_label.GetItemCount());

}
/************************************************************************/
/*                     ������ʾͼ���Լ�������ע��                       */
/************************************************************************/
void CImageLabelDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ
	//390,0  ��OnMove�е�ֵһ��
	//���û��ͼ�ڱ���ʾ�����ܿ�ʼ��ͼ
	if (m_bIsPicShow)
	{
		m_rect_draw.SetRectEmpty();
		CRect picRect;
		m_picture.GetWindowRect(&picRect);
		ScreenToClient(&picRect);
		int x = point.x - picRect.left < m_rect_cur_showImg.left ? m_rect_cur_showImg.left : point.x - picRect.left;
		int y = point.y - picRect.top < m_rect_cur_showImg.top ? m_rect_cur_showImg.top : point.y - picRect.top;
		if (x > m_rect_cur_showImg.right)
			x = m_rect_cur_showImg.right;
		if (y > m_rect_cur_showImg.bottom)
			y = m_rect_cur_showImg.bottom;
		m_point_bg.SetPoint(x,y);
		
		m_bIsMouseDown = TRUE;
	}
}
void CImageLabelDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	if (m_bIsPicShow&&m_bIsMouseDown)
	{
		CRect picRect;
		m_picture.GetWindowRect(&picRect);
		ScreenToClient(&picRect);
		int x = point.x - picRect.left < m_rect_cur_showImg.left ? m_rect_cur_showImg.left : point.x - picRect.left;
		int y = point.y - picRect.top < m_rect_cur_showImg.top ? m_rect_cur_showImg.top : point.y - picRect.top;
		if (x > m_rect_cur_showImg.right)
			x = m_rect_cur_showImg.right;
		if (y > m_rect_cur_showImg.bottom)
			y = m_rect_cur_showImg.bottom;
		m_point_ed.SetPoint(x, y);
		m_rect_draw.SetRect(m_point_bg, m_point_ed);
		m_bIsPicShow = ShowImage(m_picture.GetDC(), NULL, m_rect_pic.Width(), m_rect_pic.Height());
	}
}
void CImageLabelDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	if (m_bIsPicShow&&m_bIsMouseDown)
	{
		CRect picRect;
		m_picture.GetWindowRect(&picRect);
		ScreenToClient(&picRect);
		int x = point.x - picRect.left < m_rect_cur_showImg.left ? m_rect_cur_showImg.left : point.x - picRect.left;
		int y = point.y - picRect.top < m_rect_cur_showImg.top ? m_rect_cur_showImg.top : point.y - picRect.top;
		if (x > m_rect_cur_showImg.right)
			x = m_rect_cur_showImg.right;
		if (y > m_rect_cur_showImg.bottom)
			y = m_rect_cur_showImg.bottom;
		m_point_ed.SetPoint(x, y);
		//�洢��Ϣ
		m_rect_draw.SetRect(m_point_bg, m_point_ed);
		m_bIsPicShow = ShowImage(m_picture.GetDC(), NULL, m_rect_pic.Width(), m_rect_pic.Height());
		m_bIsMouseDown = FALSE;
	}

}
void CImageLabelDlg::OnCustomdrawListShow(NMHDR* pNMHDR, LRESULT* pResult)
{
	//This code based on Michael Dunn's excellent article on
	//list control custom draw at http://www.codeproject.com/listctrl/lvcustomdraw.asp

	NMLVCUSTOMDRAW* pLVCD = reinterpret_cast<NMLVCUSTOMDRAW*>(pNMHDR);

	// Take the default processing unless we set this to something else below.
	*pResult = CDRF_DODEFAULT;

	// First thing - check the draw stage. If it's the control's prepaint
	// stage, then tell Windows we want messages for every item.
	if (CDDS_PREPAINT == pLVCD->nmcd.dwDrawStage)
	{
		*pResult = CDRF_NOTIFYITEMDRAW;
	}
	else if (CDDS_ITEMPREPAINT == pLVCD->nmcd.dwDrawStage)
	{
		// This is the notification message for an item.  We'll request
		// notifications before each subitem's prepaint stage.

		*pResult = CDRF_NOTIFYSUBITEMDRAW;
	}
	else if ((CDDS_ITEMPREPAINT | CDDS_SUBITEM) == pLVCD->nmcd.dwDrawStage)
	{

		COLORREF clrNewTextColor = RGB(0, 0, 0);
		COLORREF clrNewBkColor = RGB(255, 255, 255);	//leave the bkgrnd color white
		if (pLVCD->iSubItem == 0)
			return;
		int    nItem = static_cast<int>(pLVCD->nmcd.dwItemSpec);
		CString strTemp = m_lst_label.GetItemText(nItem, pLVCD->iSubItem);
		USES_CONVERSION;
		unsigned long col = atol(T2A(strTemp));
		if (col != 0)
		{
			clrNewBkColor = col;
			int red = GetRValue(col);
			int blue = GetBValue(col);
			int green = GetGValue(col);
			clrNewTextColor = RGB(255 - red, 255 - green, 255 - blue);
		}
		pLVCD->clrText = clrNewTextColor;
		pLVCD->clrTextBk = clrNewBkColor;


		// Tell Windows to paint the control itself.
		*pResult = CDRF_DODEFAULT;

	}
}
void DrawRect(CDC* pDC, CRect drawRect,COLORREF color,int penSize=2)
{
	if (pDC == NULL)
		return;
	CPen pen;
	pen.CreatePen(PS_SOLID, penSize, color);
	pDC->SelectObject(&pen);
	pDC->MoveTo(drawRect.left, drawRect.top);
	pDC->LineTo(drawRect.left, drawRect.bottom);
	pDC->LineTo(drawRect.right, drawRect.bottom);
	pDC->LineTo(drawRect.right, drawRect.top);
	pDC->LineTo(drawRect.left, drawRect.top);
}
BOOL CImageLabelDlg::ShowImage(CDC* pDC, CString strPath, int windowWidth, int windowHeight)
{
	int iWndWidth = windowWidth;
	int iWndHeight = windowHeight;
	if (m_imageMat.empty())
	{

		if (PathIsDirectoryW(strPath) || !PathFileExistsW(strPath))
		{
			pDC->FillSolidRect(0, 0, iWndWidth, iWndHeight, RGB(0, 0, 0));
			return FALSE;
		}
		USES_CONVERSION;
		m_imageMat = cv::imread(T2A(strPath));
		if (m_imageMat.empty())
		{
			return FALSE;
		}
	}

	

	// ��ȡͼ���͸�,ע������Ŀ�͸߲���ͼ��ķֱ���  
	long hmWidth;
	long hmHeight;
	hmWidth = m_imageMat.cols;
	hmHeight = m_imageMat.rows;
	double winRatio = iWndWidth*1.0 / iWndHeight;
	double picRatio = hmWidth*1.0 / hmHeight;
	int iShowWidth = iWndWidth;
	int iShowHeight = iWndHeight;
	int iShowX = 0;
	int iShowY = 0;
	if (picRatio >= winRatio)
	{
		iShowHeight = iShowWidth*1.0 / picRatio;
		iShowY = iWndHeight / 2 - iShowHeight / 2;
	}
	else
	{
		iShowWidth = iShowHeight*picRatio;
		iShowX = iWndWidth / 2 - iShowWidth / 2;
	}
	CDC MemDC;
	CBitmap MemBitmap;
	MemDC.CreateCompatibleDC(NULL);
	MemBitmap.CreateCompatibleBitmap(pDC, iWndWidth, iWndHeight);
	CBitmap *pOldBit = MemDC.SelectObject(&MemBitmap);
	MemDC.FillSolidRect(0, 0, iWndWidth, iWndHeight, RGB(0, 0, 0));
	CRect rect(iShowX, iShowY, iShowX + iShowWidth, iShowY + iShowHeight);
	m_rect_cur_showImg = rect;
	CvvImage cimg;
	IplImage iplImage = m_imageMat;
	cimg.CopyOf(&iplImage);
	cimg.DrawToHDC(MemDC.GetSafeHdc(), &rect);
	cimg.Destroy();
	//defined upside
	if (!m_rect_draw.IsRectEmpty()){
		m_drawColor = m_volColor.GetColor();
		DrawRect(&MemDC, m_rect_draw, m_drawColor);
	}
	
	
	//Get Labeled ImageData
	CString fileName = GetImageFileTreePath(m_tre_file.GetSelectedItem(), FALSE);
	CImageLabelFileIOController* pFileInstance = CImageLabelFileIOController::GetInstance();
	list<CImageFile>::iterator it = pFileInstance->GetIteratorOfFile(fileName);
	if (it != pFileInstance->imageFileList.end())
	{
		POSITION lstSelPos = m_lst_label.GetFirstSelectedItemPosition();
		int SelIndex =m_lst_label.GetNextSelectedItem(lstSelPos);
		int index = 0;
		for (list<CLabel>::iterator itt = it->labelList.begin(); itt != it->labelList.end(); itt++)
		{
			CRect drawRect = GetCRect(itt->frect, m_rect_pic, CRect(0, 0, hmWidth, hmHeight));
			if (index==SelIndex)
				DrawRect(&MemDC, drawRect, itt->color,4);
			else
				DrawRect(&MemDC, drawRect, itt->color);
			index++;
		}
	}


	pDC->BitBlt(0, 0, iWndWidth, iWndHeight, &MemDC, 0, 0, SRCCOPY);
	MemDC.SelectObject(pOldBit);
	MemBitmap.DeleteObject();
	MemDC.DeleteDC();
	ReleaseDC(pDC);

	

	//�ǵ��������ͷ���Դ����Ȼ�ᵼ���ڴ�й¶  
	


	return TRUE;
}

/************************************************************************/
/*                     Color Button�¼�                                 */
/************************************************************************/

LONG CImageLabelDlg::OnSelEndOK(UINT /*lParam*/, LONG /*wParam*/)
{
	TRACE0("Selection ended OK/n");
	return TRUE;
}
LONG CImageLabelDlg::OnSelEndCancel(UINT /*lParam*/, LONG /*wParam*/)
{
	TRACE0("Selection cancelled/n");
	return TRUE;
}
LONG CImageLabelDlg::OnSelChange(UINT /*lParam*/, LONG /*wParam*/)
{
	TRACE0("Selection changed/n");
	return TRUE;
}
LONG CImageLabelDlg::OnCloseUp(UINT /*lParam*/, LONG /*wParam*/)
{
	TRACE0("Colour picker close up/n");
	return TRUE;
}
LONG CImageLabelDlg::OnDropDown(UINT /*lParam*/, LONG /*wParam*/)
{
	TRACE0("Colour picker drop down/n");
	return TRUE;
}

void CImageLabelDlg::OnBnClickedBtAdd()
{
	UpdateData(TRUE);
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	CString fileName = GetImageFileTreePath(m_tre_file.GetSelectedItem(),FALSE);
	
	TCHAR buffer[500];
	TCITEMW item = GetTabSelectItem(m_tab_label, buffer);
	CString domain = item.pszText;

	HTREEITEM i_sel_label = m_tre_label.GetSelectedItem();
	CString labelName = m_tre_label.GetItemText(i_sel_label);

	HTREEITEM i_node = m_tre_label.GetParentItem(i_sel_label);
	CString type = m_tre_label.GetItemText(i_node);
	while (m_tre_label.GetParentItem(m_tre_label.GetParentItem(i_node))!=NULL)
	//�п���type�ж༶��
	{
		i_node = m_tre_label.GetParentItem(i_node);
		type = m_tre_label.GetItemText(i_node) + _T("\\") + type;
	}

	
	BOOL res = CEventController::GetInstance()->OnAddLabel(fileName, domain, type, labelName,
		m_rect_draw, m_rect_pic, CRect(0, 0, m_imageMat.cols, m_imageMat.rows), m_imageMat.channels(),GetShotRadioString(m_radio_shot),
										m_drawColor,m_bTruncated,m_bDifficult,GetPoseRadioString(m_radio_pose));
	//�ָ�����
	m_bDifficult = FALSE;
	m_bTruncated = FALSE;
	m_radio_pose = 0;
	UpdateData(FALSE);
	OnRefreshListShowCtrl();
}
void CImageLabelDlg::OnPressDelLabel()
{
	CString fileName = GetImageFileTreePath(m_tre_file.GetSelectedItem(), FALSE);
	POSITION ps;
	int nIndex;
	ps = m_lst_label.GetFirstSelectedItemPosition();
	while (ps)
	{
		nIndex = m_lst_label.GetNextSelectedItem(ps);
		//ɾ����ע
		CEventController::GetInstance()->OnDelLabel(fileName, nIndex);
	}
	m_bIsPicShow = ShowImage(m_picture.GetDC(), fileName, m_rect_pic.Width(), m_rect_pic.Height());
	OnRefreshListShowCtrl();
}
void CImageLabelDlg::OnPressModifyLabel()
{
	// TODO:  �ڴ���������������
	UpdateData(TRUE);
	CString fileName = GetImageFileTreePath(m_tre_file.GetSelectedItem(), FALSE);
	POSITION ps;
	int nIndex;
	ps = m_lst_label.GetFirstSelectedItemPosition();
	nIndex = m_lst_label.GetNextSelectedItem(ps);
	TCHAR buffer[500];
	TCITEMW item = GetTabSelectItem(m_tab_label, buffer);
	CString domain = item.pszText;

	HTREEITEM i_sel_label = m_tre_label.GetSelectedItem();
	CString labelName = m_tre_label.GetItemText(i_sel_label);

	HTREEITEM i_node = m_tre_label.GetParentItem(i_sel_label);
	CString type = m_tre_label.GetItemText(i_node);
	while (m_tre_label.GetParentItem(m_tre_label.GetParentItem(i_node)) != NULL)
		//�п���type�ж༶��
	{
		i_node = m_tre_label.GetParentItem(i_node);
		type = m_tre_label.GetItemText(i_node) + _T("\\") + type;
	}
	CEventController::GetInstance()->OnModifyLabel(fileName, nIndex, domain, type, labelName, GetShotRadioString(m_radio_shot), m_bTruncated, m_bDifficult, GetPoseRadioString(m_radio_pose));
	OnRefreshListShowCtrl();
}
void CImageLabelDlg::OnPressDelFile()
{
	// TODO:  �ڴ���������������
	CString fileName = GetImageFileTreePath(m_tre_file.GetSelectedItem(), FALSE);
	CImageLabelFileIOController::GetInstance()->DelImageFile(fileName);
	CImageLabelFileIOController::GetInstance()->SaveFileToXML();
	fileName = GetImageFileTreePath(m_tre_file.GetSelectedItem(), TRUE);
	DeleteFile(fileName);
	m_tre_file.DeleteItem(m_tre_file.GetSelectedItem());
}
/************************************************************************/
/*                                                                      */
/************************************************************************/
void CImageLabelDlg::OnNMRClickLstShow(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMListView = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	if (pNMListView->iItem != -1)
	{
		DWORD dwPos = GetMessagePos();
		CPoint point(LOWORD(dwPos), HIWORD(dwPos));
		CMenu menu;
		VERIFY(menu.LoadMenu(IDR_MENU_LIST_SHOW));
		CMenu* popup = menu.GetSubMenu(0);
		ASSERT(popup != NULL);
		popup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, this);
	}
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;
}
void CImageLabelDlg::OnNMRClickTreeFile(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	LPNMITEMACTIVATE pNMListView = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	if (pNMListView->iItem != -1)
	{
		DWORD dwPos = GetMessagePos();
		CPoint point(LOWORD(dwPos), HIWORD(dwPos));
		CMenu menu;
		VERIFY(menu.LoadMenu(IDR_MENU_TREE_FILE));
		CMenu* popup = menu.GetSubMenu(0);
		ASSERT(popup != NULL);
		popup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, this);
	}
	*pResult = 0;
}

/************************************************************************/
/*                              ���νṹһֱ����                        */
/************************************************************************/
void CImageLabelDlg::OnNMCustomdrawTreeFile(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;

	LPNMTVCUSTOMDRAW pDraw = (LPNMTVCUSTOMDRAW)pNMHDR;
	DWORD dwDrawStage = pDraw->nmcd.dwDrawStage;
	UINT uItemState = pDraw->nmcd.uItemState;
	//
	*pResult = CDRF_NOTIFYITEMDRAW;
	//|CDRF_NOTIFYPOSTPAINT|CDRF_NOTIFYSUBITEMDRAW|CDRF_NOTIFYPOSTERASE;
	CDC* pdc = CDC::FromHandle(pDraw->nmcd.hdc);
	CRect rc;
	HTREEITEM hItem = (HTREEITEM)pDraw->nmcd.dwItemSpec;
	m_tre_file.GetItemRect(hItem, &rc, TRUE);//FALSE);text only
	CString txt = m_tre_file.GetItemText(hItem);
	if ((dwDrawStage & CDDS_ITEM) && (uItemState & CDIS_SELECTED))
	{// 
		pdc->FillSolidRect(&rc, RGB(49, 106, 197));//clr);
		//
		pdc->SetTextColor(RGB(255, 255, 255));//white
		pdc->SetBkColor(RGB(49, 106, 197));//clr);
		//

		CFont* pfnt = pdc->GetCurrentFont();
		//
		pdc->TextOut(rc.left + 2, rc.top + 2, txt);
		pdc->SelectObject(pfnt);

		*pResult |= CDRF_SKIPDEFAULT;
		// afxDump << "1\n";
	}
	else // without these ,1st blue !
	{
		pdc->FillSolidRect(&rc, GetSysColor(COLOR_WINDOW));
		pdc->SetTextColor(GetSysColor(COLOR_WINDOWTEXT));
		pdc->TextOut(rc.left + 2, rc.top + 2, txt);
		// afxDump << "2\n";
	}
}
void CImageLabelDlg::OnNMCustomdrawTreeLabel(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;

	LPNMTVCUSTOMDRAW pDraw = (LPNMTVCUSTOMDRAW)pNMHDR;
	DWORD dwDrawStage = pDraw->nmcd.dwDrawStage;
	UINT uItemState = pDraw->nmcd.uItemState;
	//
	*pResult = CDRF_NOTIFYITEMDRAW;
	//|CDRF_NOTIFYPOSTPAINT|CDRF_NOTIFYSUBITEMDRAW|CDRF_NOTIFYPOSTERASE;
	CDC* pdc = CDC::FromHandle(pDraw->nmcd.hdc);
	CRect rc;
	HTREEITEM hItem = (HTREEITEM)pDraw->nmcd.dwItemSpec;
	m_tre_label.GetItemRect(hItem, &rc, TRUE);//FALSE);text only
	CString txt = m_tre_label.GetItemText(hItem);
	if ((dwDrawStage & CDDS_ITEM) && (uItemState & CDIS_SELECTED))
	{// 
		pdc->FillSolidRect(&rc, RGB(49, 106, 197));//clr);
		//
		pdc->SetTextColor(RGB(255, 255, 255));//white
		pdc->SetBkColor(RGB(49, 106, 197));//clr);
		//

		CFont* pfnt = pdc->GetCurrentFont();
		//
		pdc->TextOut(rc.left + 2, rc.top + 2, txt);
		pdc->SelectObject(pfnt);

		*pResult |= CDRF_SKIPDEFAULT;
		// afxDump << "1\n";
	}
	else // without these ,1st blue !
	{
		pdc->FillSolidRect(&rc, GetSysColor(COLOR_WINDOW));
		pdc->SetTextColor(GetSysColor(COLOR_WINDOWTEXT));
		pdc->TextOut(rc.left + 2, rc.top + 2, txt);
		// afxDump << "2\n";
	}
}
void CImageLabelDlg::OnLvnItemchangedLstShow(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;
	CString fileName = GetImageFileTreePath(m_tre_file.GetSelectedItem(), TRUE);
	m_bIsPicShow = ShowImage(m_picture.GetDC(), fileName, m_rect_pic.Width(), m_rect_pic.Height());
}





void CImageLabelDlg::OnSaveLabelInfoXML()
{
	// TODO:  �ڴ���������������
	CImageLabelFileIOController* _instance = CImageLabelFileIOController::GetInstance();
	_instance->SaveFileToXML();
}
