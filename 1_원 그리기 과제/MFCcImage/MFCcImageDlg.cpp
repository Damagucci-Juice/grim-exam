
// MFCcImageDlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "MFCcImage.h"
#include "MFCcImageDlg.h"
#include "afxdialogex.h"
#include <stdexcept> 
#include <afx.h>


#ifdef _DEBUG
#define new DEBUG_NEW
#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")
#endif


// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()

bool CStringToNonNegativeDouble(const CString& str, double& outValue)
{
	// 문자열을 double로 변환 시도
	// 
	// _stscanf_s 함수는 안전하게 포맷 변환 가능
	if (_stscanf_s(str, _T("%lf"), &outValue) == 1) {
		// 변환 성공 후 0 이상인지 체크
		if (outValue >= 0.0) {
			return true;
		}
	}

	// 변환 실패 또는 음수면 false 반환
	return false;
}


// Random Move를 10회 실행하는 스레드 함수
void threadProcess(CWnd* pParent)
{
	CMFCcImageDlg* pWnd = reinterpret_cast<CMFCcImageDlg*>(pParent);
	if (!pWnd) return;
	// 랜덤 이동 버튼 비활성화
	pWnd->GetDlgItem(IDC_BTN_RANDOM)->EnableWindow(FALSE);

	int i = 0;
	while (i < 10) {
		if (pWnd->m_dots.size() < 3) break;

		// 점 3개 랜덤 이동
		for (size_t k = 0; k < pWnd->m_dots.size(); ++k) {
			pWnd->m_dots[k].SetRandom();
			pWnd->UpdateDotLabel(k);
		}
		pWnd->UpdateImageWithDots();

		// 정원 그리기
		try {
			pWnd->DrawCircle(pWnd->m_dots, pWnd->thickness);

			// 화면 업데이트
			pWnd->UpdateDisplay();

			Sleep(500);	// 0.5s
			i++;
		}
		catch (exception& e) {
			// 만약 에러가 발생하면 그것은 카운트에 차지하지 않고, 총 10번을 채우도록 설정
			cout << e.what() << endl;
			continue;
		}
	}

	// 랜덤 이동 버튼 활성화
	pWnd->GetDlgItem(IDC_BTN_RANDOM)->EnableWindow(TRUE);
}

// CMFCcImageDlg 대화 상자



CMFCcImageDlg::CMFCcImageDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MFCCIMAGE_DIALOG, pParent)
	, thickness(1), raw_thickness("1")
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMFCcImageDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_THICK_VAL, raw_thickness);
}

BEGIN_MESSAGE_MAP(CMFCcImageDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_BN_CLICKED(IDC_BNT_RESET, &CMFCcImageDlg::OnBnClickedBntReset)
	ON_EN_CHANGE(IDC_THICK_VAL, &CMFCcImageDlg::OnEnChangeThickVal)
	ON_BN_CLICKED(IDC_BTN_RANDOM, &CMFCcImageDlg::OnBnClickedBtnRandom)
END_MESSAGE_MAP()


// CMFCcImageDlg 메시지 처리기
BOOL CMFCcImageDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
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

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.
	DrawCanvas();
	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CMFCcImageDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 애플리케이션의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CMFCcImageDlg::OnPaint()
{

	// m_image 화면에 그림
	CPaintDC dc(this);

	if (m_image) {
		m_image.Draw(dc, 0, 0);
	}	

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

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CMFCcImageDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CMFCcImageDlg::DrawCanvas()
{
	// 로드 전 CImage 정리
	if (m_image != NULL) {
		m_image.Destroy();
	}
	int nWidth = 640;
	int nHeight = 480;
	int nBpp = 8;
	
	// 이미지 생성
	m_image.Create(nWidth, -nHeight, nBpp);

	// 이미지 배경색
	if (nBpp == 8) {
		static RGBQUAD rgb[256];
		for (int i = 0; i < 256; i++)
			rgb[i].rgbRed = rgb[i].rgbGreen = rgb[i].rgbBlue = i;
		m_image.SetColorTable(0, 256, rgb);
	}

	// 메모리에 해당 값 색칠
	unsigned char* fm = (unsigned char*)m_image.GetBits();
	memset(fm, 0xff, nWidth * nHeight);
	UpdateDisplay();
}

void CMFCcImageDlg::UpdateDisplay() {
	CClientDC dc(this);
	m_image.Draw(dc, 0, 0);
}

// 포지션 유효성 검사:: 이미지가 화면 밖에 나가더라도 유효하도록 하게 설정
bool CMFCcImageDlg::validImagePos(int x, int y) {
	bool bRet = false;
	int nWidth = m_image.GetWidth();
	int nHeight = m_image.GetHeight();
	CRect rect(0, 0, nWidth, nHeight);
	return rect.PtInRect(CPoint(x, y));
}

void CMFCcImageDlg::DrawDot(unsigned char* fm, int x, int y, int nRadius, int nGray) {
	int nCenterX = x + nRadius;
	int nCenterY = y + nRadius;
	int nPitch = m_image.GetPitch();

	for (int j = y; j < y + nRadius * 2; j++) {
		for (int i = x; i < x + nRadius * 2; i++) {
		
			if (IsInDot(i, j, nCenterX, nCenterY, nRadius) && validImagePos(i,j)) {
				fm[j * nPitch + i] = nGray;
			}
		}
	}
}

bool CMFCcImageDlg::IsInDot(int i, int j, int nCenterX, int nCenterY, int nRadius) {
	bool bRet = false;

	double dX = i - nCenterX;
	double dY = j - nCenterY;
	double dDist = dX * dX + dY * dY;
	if (dDist < nRadius * nRadius) {
		bRet = true;
	}

	return bRet;
}

// 좌클릭 위치에 원을 그리기
void CMFCcImageDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	const int hitRange = 10;  // 점 "근처" 클릭 허용 범위(픽셀)

	bool bSelectedAny = false;

	// 1) 기존 점들 중 클릭 근처에 있으면 selected = true (드래그 시작)
	for (auto& dot : m_dots)
	{
		if (abs(dot.x - point.x) <= hitRange && abs(dot.y - point.y) <= hitRange)
		{
			dot.selected = true;
			bSelectedAny = true;
			break;  // 복수 점 드래그 막으려면 한번만 선택
		}
	}

	// 2) 드래그할 점이 없다면, 점 개수가 3개 미만일 때 새 점 추가
	if (!bSelectedAny && m_dots.size() < 3)
	{
		m_dots.emplace_back(point.x, point.y);
		UpdateImageWithDots();
		UpdateDotLabel(m_dots.size() - 1);
	}

	if (m_dots.size() == 3) {
		try {
			DrawCircle(m_dots, thickness);
			UpdateDisplay();
		}
		catch (exception& e) {
			// 너무 큰원이 만들어질 경우 에러 반환
			cout << e.what() << endl;
			return;
		}
	}

	CDialogEx::OnLButtonDown(nFlags, point);
}


void CMFCcImageDlg::UpdateImageWithDots()
{
	if (m_image)
		m_image.Destroy();

	int nWidth = 640;
	int nHeight = 480;
	int nBpp = 8;
	m_image.Create(nWidth, -nHeight, nBpp);

	// 팔레트 세팅 (기본 흰색 배경과 검정색 0번 색상 확실히 설정)
	static RGBQUAD rgb[256];
	for (int i = 0; i < 256; i++)
		rgb[i].rgbRed = rgb[i].rgbGreen = rgb[i].rgbBlue = i;

	rgb[0].rgbRed = rgb[0].rgbGreen = rgb[0].rgbBlue = 0;      // 0번 검정
	rgb[255].rgbRed = rgb[255].rgbGreen = rgb[255].rgbBlue = 255; // 255번 흰색
	m_image.SetColorTable(0, 256, rgb);

	int nPitch = m_image.GetPitch();
	unsigned char* fm = (unsigned char*)m_image.GetBits();

	// 배경 흰색 초기화
	memset(fm, 255, nWidth * nHeight);

	// 저장된 점들 그리기
	const int radius = 10;
	for (const auto& dot : m_dots)
	{
		int x = dot.x - radius;
		int y = dot.y - radius;
		DrawDot(fm, x, y, radius, 0);
	}

	UpdateDisplay();
}

void CMFCcImageDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	if (nFlags & MK_LBUTTON) 
	{
		for (int i = 0; i < m_dots.size(); i++)
		{
			Dot& selectedDot = m_dots[i];
			if (selectedDot.selected)
			{
				selectedDot.x = point.x;
				selectedDot.y = point.y;
				
				UpdateImageWithDots();
				UpdateDotLabel(i);

				/// 점 3개일 때 path 원 그리기
				if (m_dots.size() == 3) {
					try {
						DrawCircle(m_dots, thickness);
						UpdateDisplay();
					}
					catch (exception& e) {
						// 너무 큰원이 만들어질 경우 에러 반환
						cout << e.what() << endl;
						break;
					}
				}
				break;  
			}
		}
	}

	CDialogEx::OnMouseMove(nFlags, point);
}

void CMFCcImageDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	for (auto& dot : m_dots)
		dot.selected = false;

	CDialogEx::OnLButtonUp(nFlags, point);
}

// 원을 이룰 수 있는지 판단, 
// true: 원 그리기 가능
// false: 원 그리기 불가
bool CMFCcImageDlg::GetCircumcenter(const Dot& p1, const Dot& p2, const Dot& p3, double& cx, double& cy)
{
	const double EPS = 5;         
	const double MAX_RADIUS = 1000; // 도화지 크기 대비 적당히 설정
	// 분모가 너무 작아져서 무한대에 가까워 지는 경우 필터링
	double d = 2 * (p1.x * (p2.y - p3.y) + p2.x * (p3.y - p1.y) + p3.x * (p1.y - p2.y));
	if (fabs(d) < EPS) return false;

	double dx = (
		(p1.x * p1.x + p1.y * p1.y) * (p2.y - p3.y)
		+ (p2.x * p2.x + p2.y * p2.y) * (p3.y - p1.y)
		+ (p3.x * p3.x + p3.y * p3.y) * (p1.y - p2.y)
		) / d;

	double dy = (
		(p1.x * p1.x + p1.y * p1.y) * (p3.x - p2.x)
		+ (p2.x * p2.x + p2.y * p2.y) * (p1.x - p3.x)
		+ (p3.x * p3.x + p3.y * p3.y) * (p2.x - p1.x)
		) / d;

	// 반지름이 너무 큰 경우 필터링
	double r = sqrt((p1.x - dx) * (p1.x - dx) + (p1.y - dy) * (p1.y - dy));
	if (r > MAX_RADIUS) return false;  
	cx = dx;
	cy = dy;
	return true;
}

// 원 그리기
void CMFCcImageDlg::DrawCircle(const std::vector<Dot>& dots, double thickness)
{
	if (dots.size() != 3) return;

	unsigned char* fm = (unsigned char*)m_image.GetBits();

	double cx, cy;
	if (!GetCircumcenter(dots[0], dots[1], dots[2], cx, cy)) {
		throw std::runtime_error("3개의 점이 거의 일직선상에 있어 정원을 만들 수 없습니다.");
	}

	double r = sqrt((dots[0].x - cx) * (dots[0].x - cx) + (dots[0].y - cy) * (dots[0].y - cy));

	int nPitch = m_image.GetPitch();

	for (int j = int(cy - r - thickness); j <= int(cy + r + thickness); ++j)
	{
		for (int i = int(cx - r - thickness); i <= int(cx + r + thickness); ++i)
		{
			double dx = i - cx;
			double dy = j - cy;
			double d = sqrt(dx * dx + dy * dy);
			if (fabs(d - r) <= thickness && validImagePos(i, j))
				fm[j * nPitch + i] = 0;
		}
	}
}

void CMFCcImageDlg::OnBnClickedBntReset()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	// 점 초기화 
	m_dots.clear();
	const UINT ids[3] = { IDC_DOT1_VAL, IDC_DOT2_VAL, IDC_DOT3_VAL };
	for (auto& id : ids) {
		CString str;
		str.AppendFormat(_T("X: - Y: -"));
		GetDlgItem(id)->SetWindowText(str);
	}

	// 선 굵기 초기화
	UpdateData(true);
	thickness = 1;
	raw_thickness = "1";
	UpdateData(false);

	// 캔버스 초기화
	DrawCanvas();
}


void CMFCcImageDlg::OnEnChangeThickVal()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// ENM_CHANGE가 있으면 마스크에 ORed를 플래그합니다.

	UpdateData(TRUE);
	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	
	if (CStringToNonNegativeDouble(raw_thickness, thickness)) {
		try {
			UpdateImageWithDots();
			DrawCircle(m_dots, thickness);
			UpdateDisplay();
		}
		catch (exception& e) {
			cout << e.what() << endl;
		}

	} else {
		return;
	}

	UpdateData(FALSE);
}

// 점의 정보를 업데이트
void CMFCcImageDlg::UpdateDotLabel(int idx) {
	const UINT ids[3] = { IDC_DOT1_VAL, IDC_DOT2_VAL, IDC_DOT3_VAL };
	CString str;
	str.AppendFormat(_T("X: %d Y: %d"), m_dots[idx].x, m_dots[idx].y);
	GetDlgItem(ids[idx])->SetWindowText(str);
}

// 랜덤 이동 버튼 액션
void CMFCcImageDlg::OnBnClickedBtnRandom()
{
	try {
		CheckDotsForOperation(); // 에러 반환 가능성
		thread _thread(threadProcess, this);
		_thread.detach();
	}
	catch (const exception& e) {
		CString msg(e.what());
		AfxMessageBox(msg);
		return;
	}
	
	
}

// 점 3개 미만으로 들고 있는지 검토해서 에러 반환
void CMFCcImageDlg::CheckDotsForOperation() {
	if (m_dots.size() < 3) {
		throw runtime_error("점의 개수가 모자랍니다.");
	}
}

// 굵기 속성 변경 후 엔터시 프로그램 종료를 방지
void CMFCcImageDlg::OnOK()
{
	// 엔터로 다이얼로그 종료 또는 앱 죽음을 방지 
	// CDialogEx::OnOK(); 호출 안 함!
	/// 점 3개일 때 path 원 그리기
	if (m_dots.size() != 3) { return; }

	if (CStringToNonNegativeDouble(raw_thickness, thickness)) {
		// thickness이 0 이상인 유효한 double값
		try {
			UpdateImageWithDots();
			DrawCircle(m_dots, thickness);
			UpdateDisplay();
		}
		catch (exception& e) {
			cout << e.what() << endl;
		}
	}
	else {
		return;
	}
}