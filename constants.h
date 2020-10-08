#ifndef CONSTANTS_H
#define CONSTANTS_H

struct Constants
{
    enum MouseButtons
    {
        LEFT_MOUSE_BUTTON,
        RIGHT_MOUSE_BUTTON,
        MIDDLE_MOUSE_BUTTON
    };

    static const int FRAME_INIT_WIDTH;
    static const int FRAME_INIT_HEIGHT;
    static const int FRAME_TOLERANCE_MIN;
    static const int FRAME_TOLERANCE_MAX;
    static const int FRAME_TOLERANCE_INIT;
    static const int FRAME_COLOR_TOLERANCE;
    static const char* COLOR_PICKER_CURSOR_PATH;
};

#endif // CONSTANTS_H
