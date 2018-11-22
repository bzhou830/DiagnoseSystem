// RBDcm.cpp : ����Ӧ�ó��������Ϊ��
//
#include "stdafx.h"
#include "afxwinappex.h"
#include "afxdialogex.h"
#include "RBDcm.h"
#include "MainFrm.h"
#include "RBDcmDoc.h"
#include "RBDcmView.h"
#include "StartDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CRBDcmApp
BEGIN_MESSAGE_MAP(CRBDcmApp, CWinApp)
	ON_COMMAND(ID_APP_ABOUT, &CRBDcmApp::OnAppAbout)
	// �����ļ��ı�׼�ĵ�����
	ON_COMMAND(ID_FILE_NEW, &CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &CWinApp::OnFileOpen)
END_MESSAGE_MAP()


// CRBDcmApp ����
CRBDcmApp::CRBDcmApp()
{
	// ֧����������������
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_ALL_ASPECTS;
#ifdef _MANAGED
	// ���Ӧ�ó��������ù�����������ʱ֧��(/clr)�����ģ���:
	//     1) �����д˸������ã�������������������֧�ֲ�������������
	//     2) ��������Ŀ�У������밴������˳���� System.Windows.Forms ������á�
	System::Windows::Forms::Application::SetUnhandledExceptionMode(System::Windows::Forms::UnhandledExceptionMode::ThrowException);
#endif
	// TODO: ������Ӧ�ó��� ID �ַ����滻ΪΨһ�� ID �ַ�����������ַ�����ʽ
	//Ϊ CompanyName.ProductName.SubProduct.VersionInformation
	SetAppID(_T("RBDcm.AppV01.NoVersion"));
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}

// Ψһ��һ�� CRBDcmApp ����
CRBDcmApp theApp;

// CRBDcmApp ��ʼ��
BOOL CRBDcmApp::InitInstance()
{
	/*
	CString a,b;        //��¼����
	while (a!="robin" || b != "leah")     
	{
		if (2 == ShowLoginDlg(a, b))
			return TRUE;
	}
	*/
	/*CStartDlg dlg;
	dlg.DoModal();*/

	// ���һ�������� Windows XP �ϵ�Ӧ�ó����嵥ָ��Ҫ
	// ʹ�� ComCtl32.dll �汾 6 ����߰汾�����ÿ��ӻ���ʽ��
	//����Ҫ InitCommonControlsEx()�����򣬽��޷��������ڡ�
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	//��������Ϊ��������Ҫ��Ӧ�ó�����ʹ�õĹ����ؼ��ࡣ
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);
	CWinApp::InitInstance();
	if (!AfxOleInit())// ��ʼ�� OLE ��
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}

	AfxEnableControlContainer();
	EnableTaskbarInteraction(FALSE);

	// ʹ�� RichEdit �ؼ���Ҫ  AfxInitRichEdit2()	
	// AfxInitRichEdit2();
	// ��׼��ʼ��
	// ���δʹ����Щ���ܲ�ϣ����С
	// ���տ�ִ���ļ��Ĵ�С����Ӧ�Ƴ�����
	// ����Ҫ���ض���ʼ������
	// �������ڴ洢���õ�ע�����
	// TODO: Ӧ�ʵ��޸ĸ��ַ����������޸�Ϊ��˾����֯��
	SetRegistryKey(_T("RBDcm������������ϵͳ"));
	LoadStdProfileSettings(4);  // ���ر�׼ INI �ļ�ѡ��(���� MRU)

	// ע��Ӧ�ó�����ĵ�ģ�塣�ĵ�ģ��
	// �������ĵ�����ܴ��ں���ͼ֮�������
	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CRBDcmDoc),
		RUNTIME_CLASS(CMainFrame),       // �� SDI ��ܴ���
		RUNTIME_CLASS(CLUView));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);

	// ������׼ shell ���DDE�����ļ�������������
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	// ��������������ָ����������
	// �� /RegServer��/Register��/Unregserver �� /Unregister ����Ӧ�ó����򷵻� FALSE��
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// Ψһ��һ�������ѳ�ʼ���������ʾ����������и���
	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();
	// �������к�׺ʱ�ŵ��� DragAcceptFiles
	// �� SDI Ӧ�ó����У���Ӧ�� ProcessShellCommand ֮����
	//SkinH_Attach();
	return TRUE;
}

int CRBDcmApp::ExitInstance()
{
	//TODO: �����������ӵĸ�����Դ
	AfxOleTerm(FALSE);
	return CWinApp::ExitInstance();
}

// CRBDcmApp ��Ϣ�������
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
public:
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

// �������жԻ����Ӧ�ó�������
void CRBDcmApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}




