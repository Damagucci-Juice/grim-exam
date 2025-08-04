#pragma once

#include <vector>
#include <iostream>
#include <random>
#include "Dot.h"
using namespace std;

#include "Process.h"
#include <chrono>
#include <thread>
using namespace chrono;

// MARK: - 전역 Util 함수
// void threadProcess(CWnd* pParent)
// bool CStringToNonNegativeDouble(const CString& str, double& outValue)

//--------------------------------
// CMFCcImageDlg 대화 상자
class CMFCcImageDlg : public CDialogEx
{
private:
    // === 프라이빗 멤버 변수 ===
    CImage m_image;
    HICON m_hIcon;
    CString raw_thickness;

    // 유틸/헬퍼 함수 (private)
    bool validImagePos(int x, int y);

    // === 에러 체크/기타 ===
    void CheckDotsForOperation();

public:
    // === 생성 및 상태 ===
    CMFCcImageDlg(CWnd* pParent = nullptr);
    std::vector<Dot> m_dots;
    double thickness;

#ifdef AFX_DESIGN_TIME
    enum { IDD = IDD_MFCCIMAGE_DIALOG };
#endif

protected:
    // === MFC 내부 오버라이드/메시지 ===
    virtual void DoDataExchange(CDataExchange* pDX);
    virtual BOOL OnInitDialog();
    afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
    afx_msg void OnPaint();
    afx_msg HCURSOR OnQueryDragIcon();
    DECLARE_MESSAGE_MAP()

public:
    // === UI/이미지 처리 ===
    void DrawCanvas();
    void UpdateDisplay();
    void UpdateImageWithDots();
    void UpdateDotLabel(int idx);

    // === 도트 및 그리기 ===
    void DrawDot(unsigned char* fm, int i, int j, int nRadius, int nGray);
    bool IsInDot(int i, int j, int nCenterX, int nCenterY, int nRadius);
    bool GetCircumcenter(const Dot& p1, const Dot& p2, const Dot& p3, double& cx, double& cy);
    void DrawCircle(const std::vector<Dot>& dots, double thickness);

    // === 이벤트 핸들러 ===
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnMouseMove(UINT nFlags, CPoint point);
    afx_msg void OnLButtonUp(UINT nFlags, CPoint point);

    afx_msg void OnBnClickedBntReset();
    afx_msg void OnBnClickedBtnRandom();
    afx_msg void OnEnChangeThickVal();
    afx_msg void OnOK();
};
