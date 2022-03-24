
// opencv_mfc_camcapture_cv.cpp: 애플리케이션에 대한 클래스 동작을 정의합니다.
//

#include "pch.h"
#include "framework.h"
#include "opencv_mfc_camcapture_cv.h"
#include "opencv_mfc_camcapture_cvDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CopencvmfccamcapturecvApp

BEGIN_MESSAGE_MAP(CopencvmfccamcapturecvApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CopencvmfccamcapturecvApp 생성

CopencvmfccamcapturecvApp::CopencvmfccamcapturecvApp()
{
	// TODO: 여기에 생성 코드를 추가합니다.
	// InitInstance에 모든 중요한 초기화 작업을 배치합니다.
}


// 유일한 CopencvmfccamcapturecvApp 개체입니다.

CopencvmfccamcapturecvApp theApp;


// CopencvmfccamcapturecvApp 초기화

BOOL CopencvmfccamcapturecvApp::InitInstance()
{
	CWinApp::InitInstance();

	CopencvmfccamcapturecvDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();

	return FALSE;
}
