#include "frame.h"

Frame::Frame() :
    top(0), left(0), bottom(0), right(0),
    originX(0), originY(0)
{

}

Frame::Frame(int top, int left, int width, int height)
    : top(top), left(left),
      bottom(top + height), right(left + width),
      originX(width / 2), originY(height / 2)
{

}

int Frame::Top() const
{
    return top;
}

int Frame::Left() const
{
    return left;
}

int Frame::Bottom() const
{
    return bottom;
}

int Frame::Right() const
{
    return right;
}

int Frame::Width() const
{
    return right - left;
}
int Frame::Height() const
{
    return bottom - top;
}

std::pair<int, int> Frame::Origin() const
{
    return { originX, originY };
}

std::pair<int, int> Frame::OriginAtParentCoords() const
{
    return { left + originX, top + originY };
}

void Frame::SetFrame(int top, int left, int bottom, int right)
{
    this->top = top;
    this->bottom = bottom;
    this->left = left;
    this->right = right;
    originX = Width() / 2;
    originY = Height() / 2;
}

void Frame::Top(int top)
{
    this->top = top;
    originY = Height() / 2;
}

void Frame::Left(int left)
{
    this->left = left;
    originX = Width() / 2;
}

void Frame::Bottom(int bottom)
{
    this->bottom = bottom;
    originY = Height() / 2;
}

void Frame::Right(int right)
{
    this->right = right;
    originX = Width() / 2;
}

void Frame::Origin(const std::pair<int, int> origin)
{
    if (origin.first < 0 || origin.first >= Width() ||
            origin.second < 0 || origin.second >= Height())
        return;

    originX = origin.first;
    originY = origin.second;
}

bool Frame::ContainesPoint(int x, int y)
{
    return (x >= left && x <= right && y >= top && y <= bottom);
}

void Frame::Move(int xOffset, int yOffset)
{
    top += yOffset;
    bottom += yOffset;
    left += xOffset;
    right += xOffset;
}

void Frame::Move(int xOffset, int yOffset, int xMax, int yMax)
{
    if (top + yOffset >= 0 && top + yOffset < yMax - Height())
    {
        top += yOffset;
        bottom += yOffset;
    }
    if (left + xOffset >= 0 && left + xOffset < xMax - Width())
    {
        left += xOffset;
        right += xOffset;
    }
}
