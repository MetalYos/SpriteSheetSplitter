#ifndef SETTINGS_H
#define SETTINGS_H

struct Settings
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
    static const int FRAME_DETECTION_ALGO_STEP;
    static bool FRAME_DETECTION_ALGO_USE_ADAPTIVE_STEP;
};

#endif // SETTINGS_H
