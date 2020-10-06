#ifndef CONSTANTS_H
#define CONSTANTS_H

namespace Constants
{
    enum MouseButtons
    {
        LEFT_MOUSE_BUTTON,
        RIGHT_MOUSE_BUTTON,
        MIDDLE_MOUSE_BUTTON
    };

    const int FRAME_INIT_WIDTH = 100;
    const int FRAME_INIT_HEIGHT = 100;
    const int FRAME_TOLERANCE_MIN = 0;
    const int FRAME_TOLERANCE_MAX = 50;
    const int FRAME_TOLERANCE_INIT = 1;
}

#endif // CONSTANTS_H
