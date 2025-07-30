
// MFCcImageDlg.h: 헤더 파일
//

#pragma once

#include <vector>
#include <iostream>
#include <random>

using namespace std;

class Dot
{
public: 
	int x;
	int y;
	bool selected;  // 점이 드래그 중인지 여부

	Dot(int _x, int _y) : x(_x), y(_y), selected(false) {}
	
	void SetRandom();

	 friend ostream& operator<<(ostream& os, const Dot& d);
};

// CMFCcImageDlg 대화 상자
class CMFCcImageDlg : public CDialogEx
{
private:
	CImage m_image;
	bool validImagePos(int x, int y);
// 생성입니다.
public:
	CMFCcImageDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.
	std::vector<Dot> m_dots;

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MFCCIMAGE_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	void DrawCanvas();
	void UpdateDisplay();
	void drawDot(unsigned char* fm, int i, int j, int nRadius, int nGray);
	bool isInCircle(int i, int j, int nCenterX, int nCenterY, int nRadius);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	void UpdateImageWithDots();
	void OnMouseMove(UINT nFlags, CPoint point);
	void OnLButtonUp(UINT nFlags, CPoint point);
	bool GetCircumcenter(const Dot& p1, const Dot& p2, const Dot& p3, double& cx, double& cy);
	void drawCircle(const std::vector<Dot>& dots, double thickness);
	afx_msg void OnBnClickedBntReset();
	int thickness;
	afx_msg void OnEnChangeThickVal();
	void UpdateDotLabel(int idx);
	afx_msg void OnBnClickedBtnRandom();
	void RandomMoveDots();
	static UINT RandomMoveThreadProc(LPVOID pParam);
};