#pragma once

#include <iostream>
#define IMAGE_WIDTH 640
#define IMAGE_HEIGHT 480
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
