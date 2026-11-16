#include "ChatWindow.h"

#include "raylib.h"

ChatWindow::ChatWindow(const int width, const int height, const Theme theme, const ServerStatus& status) : 
    mStatus(status), mTheme(theme)
{
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(width, height, "Kili chat");
    SetTargetFPS(10);
}

void ChatWindow::update()
{
    mBlinkTime -= GetFrameTime();
    if (mBlinkTime <= 0)
    {
        mBlink = !mBlink;
        mBlinkTime = 0.5f;
    }
}

void ChatWindow::draw() const
{
    const int h = GetScreenHeight();
    const int w = GetScreenWidth();
    
    ClearBackground(mTheme.mainColor);
    
    DrawRectangle(0, h*0.85f,  w, h, mTheme.secondaryColor); // bottom box
    DrawRectangle(0, h*0.85f, w, 2,      mTheme.accentColor); // bottom line
    DrawRectangle(10, h*0.85f+12, w-20, 24+h*0.01f, mTheme.mainColor); // text box
    
    DrawText(mTyping.c_str(), 20, h*0.855f+12, 24, mTheme.textColor);
    if (mBlink) DrawRectangle(MeasureText(mTyping.c_str(), 24)+2+20, h*0.855f+12, 2, 24, RAYWHITE);
    
    DrawText("KiliChat", 20, h*0.925f, 50, mTheme.textColor);
    DrawText(mStatus ? "connected" : "not connected", w*0.5f, h*0.92, 24, mStatus ? GREEN : RED);
    const std::string server = mStatus.ip + ":" + std::to_string(mStatus.port);
    DrawText(server.c_str(), w*0.5f, h*0.96, 24, mTheme.textColor);
}
