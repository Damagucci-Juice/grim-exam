#include "pch.h"
#include "Dot.h"

// 10~500 ������ ������ ���� �������� ������Ʈ��
void Dot::SetRandom() {
    x = rand() % IMAGE_WIDTH;
    y = rand() % IMAGE_HEIGHT;
}

ostream& operator<<(ostream& os, const Dot& d) {
	os << "X: " << d.x << ", Y: " << d.y;
	return os;
}