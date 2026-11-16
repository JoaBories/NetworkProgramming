#pragma once

#include <raylib.h>
#include <string>
#include <vector>

#include "SDL.h"
#include "SDL_video.h"

struct Line
{
    std::string Text;
    Color Color;
};

struct Theme
{
    Color mainColor;
    Color secondaryColor;
    Color textColor;
    Color accentColor;
    
    Theme(Color main_, Color secondary_, Color text_, Color accent_) :
        mainColor(main_), secondaryColor(secondary_), textColor(text_), accentColor(accent_) {}
};

struct ServerStatus
{
    std::string ip;
    uint16_t port;
    bool connected;
    
    explicit operator bool() const { return connected; }
};

class ChatWindow
{
private:
    ServerStatus mStatus;
    std::vector<Line> mChat;
    std::string mTyping{};
    Theme mTheme;
    
    float mBlinkTime;
    bool mBlink;
    
public:
    ChatWindow(int width, int height, Theme theme, const ServerStatus& status);
    
    void update();
    
    void updateStatus(const ServerStatus& status) { mStatus = status; }
    void updateTyping(const std::string& text) { mTyping = text; }

    void draw() const;
};
