
// opencv_mfc_camcapture_cvDlg.h: 헤더 파일
//

#include <opencv2/opencv.hpp>
#pragma once


// CopencvmfccamcapturecvDlg 대화 상자
class CopencvmfccamcapturecvDlg : public CDialogEx
{
// 생성입니다.
public:
	CopencvmfccamcapturecvDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_OPENCV_MFC_CAMCAPTURE_CV_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CStatic m_picture;
	CStatic m_picture_max;
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	VideoCapture* capture;
	Mat mat_frame;
	CImage cimage_mfc;
	CImage cimage_mfc_max;
	CListBox m_list;
	afx_msg void OnBnClickedButton();
	Mat src, dst, grad_x, grad_y;
	Mat abs_grad_x, abs_grad_y, grad;
	int sum;
	int min = INT_MAX;
	afx_msg void OnBnClickedButton2();
};
