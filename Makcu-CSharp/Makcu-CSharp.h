#pragma once

using namespace System;

namespace MakcuProxy {
    public ref class MakcuLib {

    public:
        static bool Init(int comPort, int baudRate);

        static void Close();

        static bool MouseMove(int x, int y);

        static bool MouseMoveMock(int x, int y, int time);

        static bool MouseMoveto(int x, int y);

        static bool MouseLeft(int type);

        static bool MouseRight(int type);

        static bool GetMouseKeyState(int key);

        static bool UpdateMouseState();
    };
}