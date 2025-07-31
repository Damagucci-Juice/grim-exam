# CircleSimulator
<img width="1233" height="510" alt="image" src="https://github.com/user-attachments/assets/3ae12b16-b6d9-437d-acea-ac708e46771f" />

## 앱 소개

윈도우프로그램으로 점을 화면에 3개를 찍으면 그 점을 바탕으로 원을 그려주는 어플리케이션

## 앱 기능

1. 화면에 점을 찍는다. 
2. 점의 최대 개수는 3개까지 찍는다
3. 개별점은 드래그가 가능하다
4. 생기는 원을 관찰한다
5. 원 두께를 설정한다
6. 초기화하고 싶으면 초기화 버튼을 누른다.
7. ‘랜덤 이동’ 버튼을 눌러 5초 동안 10번 원이 랜덤 위치에 나타나는 시뮬레이션을 나타낸다.

## 고민과 해결

### 1. 원을 그릴 때 점이 3개가 직선을 이루는 경우 컴퓨팅 리소스를 너무 많이 잡아먹는 문제

```cpp
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

```

- `CMCFcImageDlg::GetCircumcenter` 함수에서 점 3개를 받아서 원을 그릴 수 있는지 평가하는 함수
- 여기서 이 `double d = 2 * (p1.x * (p2.y - p3.y) + p2.x * (p3.y - p1.y) + p3.x * (p1.y - p2.y));` 공식의 결과값이 0에 가깝다면 거의 일직선이 되어 그리려는 원이 아주 커지는 일이 발생
- → **d의 값이 유효한 값 이하로 작아졌을 때** 더 이상의 계산을 하지 않고 false를 반환
- → r 반지름 값을 이용해서 **반지름이 화면의 크기 이상으로 클 경우**도 더 이상 함수 진행을 하지 않도록 false 반환

### 2. 선 굵기 속성을 엔터 버튼을 눌러서 입력할 때 앱이 종료되는 에러

엔터가 입력되었을 때 부모의 `OnOk` 함수를 호출하지 않으므로써 아무런 동작도 하지 않도록 수정

```cpp
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
```

## 데모
### 점을 찍는다
<img width="818" height="638" alt="image" src="https://github.com/user-attachments/assets/ac4e6ceb-e4a3-45e7-ad59-2d7f44dd8eef" />


### 점 3개를 찍는다
<img width="818" height="638" alt="image" src="https://github.com/user-attachments/assets/85fafca3-c706-43c5-b442-cc206f918dbb" />


### 점 하나를 드래그 한다
<img width="818" height="638" alt="image" src="https://github.com/user-attachments/assets/e7930241-43b8-417c-ab3b-f8161fd2b646" />


### 드래그 중에 원을 실시간으로 그린다
https://github.com/user-attachments/assets/ecfd96ef-8ccb-4614-923a-a04b9d67fc5d


### 선 굵기를 입력한다
<img width="818" height="638" alt="image" src="https://github.com/user-attachments/assets/b75745cc-a376-466c-96d5-e4446e270663" />


### 초기화한다
<img width="818" height="638" alt="image" src="https://github.com/user-attachments/assets/e1f6c040-0896-4b93-a35c-c55daa482ea9" />


### 랜덤이동한다
https://github.com/user-attachments/assets/9c88f65d-bce1-4fa7-83d5-53c0ce08ca81


