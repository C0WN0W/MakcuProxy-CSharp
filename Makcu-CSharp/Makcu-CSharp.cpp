#ifndef NOMINMAX
#define NOMINMAX
#endif

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include "Makcu.hpp" 
#include "Makcu-CSharp.h"

#pragma comment(lib, "User32.lib")

#pragma managed

namespace MakcuProxy {

    bool MakcuLib::Init(int comPort, int baudRate) {
        auto* makcu = &Makcu::Get();
        if (!makcu->myserial.open(comPort, baudRate)) {
            return false;
        }
        return true;
    }

    void MakcuLib::Close() {
        Makcu::Get().myserial.close();
    }

    bool MakcuLib::MouseMove(int x, int y) {
        return Makcu::Get().MakcuMouseMove(x, y);
    }

    bool MakcuLib::MouseMoveMock(int x, int y, int time) {
        return Makcu::Get().MakcuMouseMoveMock(x, y, time);
    }

    bool MakcuLib::MouseMoveto(int x, int y) {
        return Makcu::Get().MakcuMouseMoveto(x, y);
    }

    bool MakcuLib::MouseLeft(int type) {
        return Makcu::Get().MakcuMouseLeft(type);
    }

    bool MakcuLib::MouseRight(int type) {
        return Makcu::Get().MakcuMouseRight(type);
    }

    bool MakcuLib::GetMouseKeyState(int key) {
        auto* makcu = &Makcu::Get();
        switch (key) {
        case 0: return makcu->MakcuLeft();
        case 1: return makcu->MakcuRight();
        case 2: return makcu->MakcuMiddle();
        case 3: return makcu->MakcuSide2();
        case 4: return makcu->MakcuSide1();
        default: return false;
        }
    }

    bool MakcuLib::UpdateMouseState() {
        auto* makcu = &Makcu::Get();
        bool success = true;
        for (int i = 0; i < 5; ++i) {
            success &= makcu->MakcuMouseState(i);
        }
        return success;
    }
}