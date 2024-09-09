#include <iostream>
#include <windows.h>
#include <sstream>

using namespace std;

HDC dc;
POINT point;

constexpr int HEX_LENGTH_WITH_HEAD = 7;
constexpr int HEX_LENGTH = 6;

struct COLOR_RGB {
    int r, g, b;
};

string rgb2hex(COLOR_RGB color, bool with_head) {
    stringstream ss;
    if (with_head)
        ss << "#";
    ss << std::hex << (color.r << 16 | color.g << 8 | color.b);
    return ss.str();
}

double GetMouse(double mode) {
    POINT MousePoint;
    GetCursorPos(&MousePoint);
    if (mode == 0) {
        double Mouse_X = MousePoint.x;
        return Mouse_X;
    } else {
        double Mouse_Y = MousePoint.y;
        return Mouse_Y;
    }
}

void updateCursorPoint() {
    GetCursorPos(&point);
}

COLOR_RGB getColor() {
    updateCursorPoint();
    COLORREF colorRef = GetPixel(dc, point.x, point.y);
    int red = GetRValue(colorRef);
    int blue = GetBValue(colorRef);
    int green = GetGValue(colorRef);
    return COLOR_RGB{red, green, blue};
}

void copyToClipboard(char *str, int n) {
    const size_t len = n + 1;
    const HGLOBAL hMem = GlobalAlloc(GMEM_MOVEABLE, len);
    memcpy(GlobalLock(hMem), str, len);
    GlobalUnlock(hMem);
    OpenClipboard(nullptr);
    EmptyClipboard();
    SetClipboardData(CF_TEXT, hMem);
    CloseClipboard();
}

int main() {
    updateCursorPoint();
    dc = GetDC(nullptr);

    do {
        if (GetKeyState(VK_LCONTROL) & 0x80 && GetKeyState(VK_LMENU) & 0x80 && GetKeyState('D') & 0x80) {
            const COLOR_RGB color = getColor();
            SetPixel(dc, point.x, point.y, RGB(255, 255, 255));
            const string hex = rgb2hex(color, true);

            char buf[HEX_LENGTH_WITH_HEAD];
            strcpy(buf, hex.c_str());
            copyToClipboard(buf, HEX_LENGTH_WITH_HEAD);
            cout << hex << endl;

            Beep(600, 200);
        }
        Sleep(100);
    } while (!(GetKeyState(VK_ESCAPE) & 0x80));

    return 0;
}
