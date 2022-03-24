
// opencv_mfc_camcapture_cvDlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "opencv_mfc_camcapture_cv.h"
#include "opencv_mfc_camcapture_cvDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CopencvmfccamcapturecvDlg 대화 상자
CopencvmfccamcapturecvDlg::CopencvmfccamcapturecvDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_OPENCV_MFC_CAMCAPTURE_CV_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CopencvmfccamcapturecvDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PICTURE, m_picture);
	DDX_Control(pDX, IDC_LIST, m_list);
	DDX_Control(pDX, IDC_PICTURE2, m_picture_max);
}

BEGIN_MESSAGE_MAP(CopencvmfccamcapturecvDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_DESTROY()
	ON_WM_TIMER()
ON_WM_LBUTTONDOWN()
ON_BN_CLICKED(IDC_BUTTON, &CopencvmfccamcapturecvDlg::OnBnClickedButton)
ON_BN_CLICKED(IDC_BUTTON2, &CopencvmfccamcapturecvDlg::OnBnClickedButton2)
END_MESSAGE_MAP()


// CopencvmfccamcapturecvDlg 메시지 처리기

BOOL CopencvmfccamcapturecvDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.
	SetWindowText(_T("Focused image of SEM"));

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CopencvmfccamcapturecvDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}
HCURSOR CopencvmfccamcapturecvDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CopencvmfccamcapturecvDlg::OnDestroy()
{
	KillTimer(1000);
	CDialogEx::OnDestroy();
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}

void CopencvmfccamcapturecvDlg::OnTimer(UINT_PTR nIDEvent)
{
    //mat_frame가 입력 이미지입니다. 
	capture->read(src);
	//if (src.empty())KillTimer(1000);
	if (!src.empty()) {
		GaussianBlur(src, dst, Size(3, 3), 0, 0);
		//bilateralFilter(src, dst, -1, 10, 5);
		Sobel(dst, grad_x, CV_16S, 1, 0, 3);
		Sobel(dst, grad_y, CV_16S, 0, 1, 3);
		convertScaleAbs(grad_x, abs_grad_x);
		convertScaleAbs(grad_y, abs_grad_y);
		addWeighted(abs_grad_x, 0.5, abs_grad_y, 0.5, 0, grad);

		for (int i = 0; i < grad.rows; i++) {
			for (int j = 0; j < grad.cols; j++) {
				sum = sum + abs(grad.at<uchar>(i, j));
			}
		}
	}
	if (sum == 0) KillTimer(1000);
	mat_frame = src.clone();
	//화면에 보여주기 위한 처리입니다.

	int bpp = 8 * mat_frame.elemSize();
	assert((bpp == 8 || bpp == 24 || bpp == 32));

	int padding = 0;
	//32 bit image is always DWORD aligned because each pixel requires 4 bytes
	if (bpp < 32)
		padding = 4 - (mat_frame.cols % 4);

	if (padding == 4)
		padding = 0;

	int border = 0;
	//32 bit image is always DWORD aligned because each pixel requires 4 bytes
	if (bpp < 32)
	{
		border = 4 - (mat_frame.cols % 4);
	}

	Mat mat_temp;
	if (border > 0 || mat_frame.isContinuous() == false)
	{
		// Adding needed columns on the right (max 3 px)
		cv::copyMakeBorder(mat_frame, mat_temp, 0, 0, 0, border, cv::BORDER_CONSTANT, 0);
	}
	else
	{
		mat_temp = mat_frame;
	}

	RECT r;
	m_picture.GetClientRect(&r);
	cv::Size winSize(r.right, r.bottom);

	cimage_mfc.Create(winSize.width, winSize.height, 24);


	BITMAPINFO* bitInfo = (BITMAPINFO*)malloc(sizeof(BITMAPINFO) + 256 * sizeof(RGBQUAD));
	bitInfo->bmiHeader.biBitCount = bpp;
	bitInfo->bmiHeader.biWidth = mat_temp.cols;
	bitInfo->bmiHeader.biHeight = -mat_temp.rows;
	bitInfo->bmiHeader.biPlanes = 1;
	bitInfo->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bitInfo->bmiHeader.biCompression = BI_RGB;
	bitInfo->bmiHeader.biClrImportant = 0;
	bitInfo->bmiHeader.biClrUsed = 0;
	bitInfo->bmiHeader.biSizeImage = 0;
	bitInfo->bmiHeader.biXPelsPerMeter = 0;
	bitInfo->bmiHeader.biYPelsPerMeter = 0;


	//그레이스케일 인경우 팔레트가 필요
	if (bpp == 8)
	{
		RGBQUAD* palette = bitInfo->bmiColors;
		for (int i = 0; i < 256; i++)
		{
			palette[i].rgbBlue = palette[i].rgbGreen = palette[i].rgbRed = (BYTE)i;
			palette[i].rgbReserved = 0;
		}
	}

	if (mat_temp.cols == winSize.width && mat_temp.rows == winSize.height)
	{
		// source and destination have same size
		// transfer memory block
		// NOTE: the padding border will be shown here. Anyway it will be max 3px width

		SetDIBitsToDevice(cimage_mfc.GetDC(),
			//destination rectangle
			0, 0, winSize.width, winSize.height,
			0, 0, 0, mat_temp.rows,
			mat_temp.data, bitInfo, DIB_RGB_COLORS);
	}
	else
	{
		// destination rectangle
		int destx = 0, desty = 0;
		int destw = winSize.width;
		int desth = winSize.height;

		// rectangle defined on source bitmap
		// using imgWidth instead of mat_temp.cols will ignore the padding border
		int imgx = 0, imgy = 0;
		int imgWidth = mat_temp.cols - border;
		int imgHeight = mat_temp.rows;

		StretchDIBits(cimage_mfc.GetDC(),
			destx, desty, destw, desth,
			imgx, imgy, imgWidth, imgHeight,
			mat_temp.data, bitInfo, DIB_RGB_COLORS, SRCCOPY);
	}

	HDC dc = ::GetDC(m_picture.m_hWnd);
	cimage_mfc.BitBlt(dc, 0, 0);
	if (sum < min) {
		min = sum;
		HDC dc_max = ::GetDC(m_picture_max.m_hWnd);
		cimage_mfc.BitBlt(dc_max, 0, 0);
		::ReleaseDC(m_picture_max.m_hWnd, dc_max);
	}
	
	::ReleaseDC(m_picture.m_hWnd, dc);

	sum = sum / 100000;
	CString str;
	str.Format(_T("sum of pixel values : %d"), sum);
	m_list.InsertString(0, str);
	sum = 0;

	cimage_mfc.ReleaseDC();
	cimage_mfc.Destroy();
	
	CDialogEx::OnTimer(nIDEvent);
}



void CopencvmfccamcapturecvDlg::OnBnClickedButton()
{
	CFileDialog fileDlg(TRUE, NULL, NULL, OFN_READONLY, _T("All Files(*.*)|*.*||"));
	if (fileDlg.DoModal() == IDOK)
	{
		CString path = fileDlg.GetPathName();
		CT2CA pszString(path);
		string strPath(pszString);

		capture = new VideoCapture(strPath);

		if (!capture->isOpened())
		{
			MessageBox(_T("동영상을 열수 없습니다. \n"));
		}
		SetTimer(1000, 30, NULL);
	}
}


void CopencvmfccamcapturecvDlg::OnBnClickedButton2()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	KillTimer(1000);
}
