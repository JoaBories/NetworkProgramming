#pragma once

#include <raylib.h>
#include <string>
#include <vector>

#include "SDL.h"

struct line
{
    std::string lText;
    Color lColor;
    
    line(std::string text, const Color color) : lText(std::move(text)), lColor(color) {}
};

struct theme
{
    Font textFont;
    Font titleFont;
    float textSize;
    float titleSize;
    Color mainColor;
    Color secondaryColor;
    Color textColor;
    Color accentColor;
    
    theme(Color main_, Color secondary_, Color text_, Color accent_, const Font& textFont_, const Font& titleFont_, float textSize_, float titleSize_) :
        mainColor(main_), secondaryColor(secondary_), accentColor(accent_),
        textColor(text_), textFont(textFont_), titleFont(titleFont_),
        textSize(textSize_), titleSize(titleSize_) {}
};

struct serverStatus
{
    std::string ip;
    uint16_t port;
    bool connected;
    
    explicit operator bool() const { return connected; }
};

class Chat
{
private:
    serverStatus mStatus;
    std::vector<line> mChat;
    std::string mTyping{};
    uint32_t mTypingPos{0};
    theme mTheme;
    
    bool mShouldQuit{false};
    
    float mBlinkTime{0.0f};
    bool mBlink{false};
    
    void resetBlink(bool newBlink);
    void addLine(std::string text, Color color = BLANK);
    
public:
    Chat(const theme& theme, const serverStatus& status);
    void submit(const std::string& input);
    
    void update();
    void draw() const;
    
    bool shouldQuit() const { return mShouldQuit; }
};
