#include "pch.h"
#include "Dot.h"

// 10~500 사이의 값으로 점의 포지션을 업데이트함
void Dot::SetRandom() {
    x = rand() % IMAGE_WIDTH;
    y = rand() % IMAGE_HEIGHT;
}

ostream& operator<<(ostream& os, const Dot& d) {
	os << "X: " << d.x << ", Y: " << d.y;
	return os;
}