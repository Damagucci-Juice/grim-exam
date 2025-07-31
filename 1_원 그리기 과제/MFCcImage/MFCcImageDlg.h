#pragma once

#include <vector>
#include <iostream>
#include <random>

using namespace std;

//--------------------------------
// Dot 클래스: 구조체/유틸
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

//--------------------------------
// CMFCcImageDlg 대화 상자
class CMFCcImageDlg : public CDialogEx
{
private:
    // === 프라이빗 멤버 변수 ===
    CImage m_image;
    HICON m_hIcon;
    int thickness;

    // 유틸/헬퍼 함수 (private)
    bool validImagePos(int x, int y);

    // === 에러 체크/기타 ===
    void CheckDotsForOperation();

public:
    // === 생성 및 상태 ===
    CMFCcImageDlg(CWnd* pParent = nullptr);
    std::vector<Dot> m_dots;

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

    // === 점 조작/동작 ===
    void RandomMoveDots();
    static UINT RandomMoveThreadProc(LPVOID pParam);

    // === 이벤트 핸들러 ===
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnMouseMove(UINT nFlags, CPoint point);
    afx_msg void OnLButtonUp(UINT nFlags, CPoint point);

    afx_msg void OnBnClickedBntReset();
    afx_msg void OnBnClickedBtnRandom();
    afx_msg void OnEnChangeThickVal();
};
