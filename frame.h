#ifndef FRAME_H
#define FRAME_H

#include <utility>

class Frame
{
public:
    Frame();
    Frame(int top, int left, int width = 100, int height = 100);

    // Getters
    int Top() const;
    int Left() const;
    int Bottom() const;
    int Right() const;
    int Width() const;
    int Height() const;
    std::pair<int, int> Origin() const;
    std::pair<int, int> OriginAtParentCoords() const;

    // Setters
    void SetFrame(int top, int left, int bottom, int right);
    void Top(int top);
    void Left(int left);
    void Bottom(int bottom);
    void Right(int right);
    void Origin(const std::pair<int, int> origin);

    bool ContainesPoint(int x, int y);
    void Move(int xOffset, int yOffset);
    void Move(int xOffset, int yOffset, int xMax, int yMax);

private:
    int top;
    int left;
    int bottom;
    int right;
    int originX;
    int originY;
};

class FrameComparer
{
    int tolerance = 10;

public:
    bool operator()(const Frame* f1, const Frame* f2)
    {
        int topDiff = f1->Top() - f2->Top();
        bool sameLine = (topDiff > -tolerance) && (topDiff < tolerance);

        if (sameLine)
            return (f1->Right() < f2->Left());
        else
            return (f1->Top() < f2->Bottom() - tolerance);
    }
};

#endif // FRAME_H
