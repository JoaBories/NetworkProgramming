#include "Chat.h"

#include <ios>

#include "Net.h"
#include "raylib.h"
#include "Utils.h"

void Chat::resetBlink(const bool newBlink)
{
    mBlink = newBlink;
    mBlinkTime = 0.5f;
}

void Chat::addLine(std::string text, Color color)
{
    mChat.emplace_back(text, color);
}

Chat::Chat(const theme& theme, const serverStatus& status) : 
    mStatus(status), mTheme(theme)
{
}

void Chat::submit(const std::string& input)
{
    const auto brokeInput = Utils::breakString(input, ' ');
		
    if (brokeInput[0] == "close")
    {
        mShouldQuit = true;
    }
    else if (brokeInput[0] == "ip")
    {
        mStatus.ip = brokeInput[1];
        std::cout << "Set server ip to: " << mStatus.ip << '\n';
    }
    else if (brokeInput[0] == "port")
    {
        mStatus.port = std::stoi(brokeInput[1]);
        std::cout << "Set server port to: " << mStatus.port << '\n';
    }
    else if (brokeInput[0] == "server")
    {
        std::cout << "current server: " << mStatus.ip << ":" << mStatus.port << '\n';
    }
    else
    {
        std::string message = input.substr(input.find_first_of(' '));
        Net::sendMessageToServer(mStatus.ip.c_str(), mStatus.port, message);
    }
}

void Chat::update()
{
    const char inputChar = static_cast<char>(GetCharPressed());
    if(inputChar != 0)
    {
        mTyping.insert(mTyping.begin() + mTypingPos, inputChar);
        mTypingPos++;
        resetBlink(true);
    }

    const bool backSpacePressed = IsKeyPressed(KEY_BACKSPACE) || IsKeyPressedRepeat(KEY_BACKSPACE);
    if(backSpacePressed && mTypingPos != 0)
    {
        mTyping.erase(mTyping.begin() + mTypingPos-1);
        mTypingPos--;
        resetBlink(true);
    }
    
    const bool leftPressed = IsKeyPressed(KEY_LEFT) || IsKeyPressedRepeat(KEY_LEFT);
    if (leftPressed && mTypingPos > 0)
    {
        mTypingPos--;
        resetBlink(true);
    }
    
    const bool rightPressed = IsKeyPressed(KEY_RIGHT) || IsKeyPressedRepeat(KEY_RIGHT);
    if (rightPressed && mTypingPos < mTyping.length())
    {
        mTypingPos++;
        resetBlink(true);
    }
    
    mBlinkTime -= GetFrameTime();
    if (mBlinkTime <= 0) resetBlink(!mBlink);
}

void Chat::draw() const
{
    const float h = static_cast<float>(GetScreenHeight());
    const float w = static_cast<float>(GetScreenWidth());
    
    ClearBackground(mTheme.mainColor);
    
    DrawRectangleV({ 0, h*0.85f}, {w,    h}, mTheme.secondaryColor);  // bottom box
    DrawRectangleV({ 0, h*0.85f}, {w,    2}, mTheme.accentColor);     // bottom line
    
    DrawRectangleV({10, h*0.85f+10}, {w-20, mTheme.textSize+10}, mTheme.mainColor); // text box
    DrawTextEx(mTheme.textFont, mTyping.c_str(), { 20, h*0.85f+15 }, mTheme.textSize, 1.0f, mTheme.textColor);
    
    const float blinkPos = MeasureTextEx(mTheme.textFont, mTyping.substr(0, mTypingPos).c_str(), mTheme.textSize, 1.0f).x+2;
    if (mBlink) DrawRectangleV({ blinkPos+20, h*0.85f+15 }, { 2, mTheme.textSize }, mTheme.textColor);
    
    DrawTextEx(mTheme.titleFont,"KiliChat", { 20.0f, h*0.925f }, mTheme.titleSize, 1.0f, mTheme.textColor);
    DrawTextEx(mTheme.textFont, mStatus ? "connected" : "not connected", { w*0.5f, h*0.92f }, mTheme.textSize, 1.0f, mStatus ? GREEN : RED);
    const std::string server = mStatus.ip + ":" + std::to_string(mStatus.port);
    DrawTextEx(mTheme.textFont, server.c_str(), { w*0.5f, h*0.96f }, mTheme.textSize, 1.0f, mTheme.textColor);
}
