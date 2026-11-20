#include "Chat.h"

#include <ios>

#include "Net.h"
#include "raylib.h"
#include "Utils.h"
#include <cmath>

namespace
{
    bool operator==(const Color& a, const Color& b)
    {
        return a.r == b.r && a.g == b.g && a.b == b.b && a.a == b.a;
    }
}

void Chat::resetBlink(const bool newBlink)
{
    mBlink = newBlink;
    mBlinkTime = 0.5f;
}

void Chat::removeLastLine()
{
    mChat.erase(mChat.end() - 1);
}

void Chat::addLine(std::string text, Color color)
{
    mChat.emplace_back(text, color);
}

void Chat::drawLine(std::string text, float y, Color color) const
{
    DrawTextEx(mTheme.textFont, text.c_str(), { 10.0f, y}, mTheme.textSize, 1.0f, color == BLANK ? mTheme.textColor : color);
}

Chat::Chat(const theme& theme, const serverStatus& status) : 
    mStatus(status), mTheme(theme)
{
    addLine("! Welcome to KiliChat !", GREEN);
}

Chat::~Chat()
{
    if (mStatus)
    {
        Net::sendMessage("/disconnect", mServerSocket);
    }
}

void Chat::submit(const std::string& input)
{
    if (input.empty()) return;
    
    if (input[0] == '/')
    {
        const size_t firstSpace = input.find_first_of(' ');
        std::string command;
        std::string param;
        
        if (firstSpace != std::string::npos)
        {
            command = input.substr(0, firstSpace);
            param = input.substr(firstSpace+1);
        }
        else
        {
            command = input;
            param = "";
        }
		
        if (command == "/close")
        {
            mShouldQuit = true;
        }
        else if (command == "/ip")
        {
            mStatus.ip = param;
            addLine("Set server ip to: " + mStatus.ip);
        }
        else if (command == "/port")
        {
            mStatus.port = std::stoi(param);
            addLine("Set server port to: " + std::to_string(mStatus.port));
        }
        else if (command == "/connect")
        {
            addLine("connecting ...", mTheme.accentColor);
            if (Net::connectToServer(mStatus.ip.c_str(), mStatus.port, mServerSocket, mServerAddress))
            {
                if (Net::sendMessage(param, mServerSocket))
                {
                    mStatus.connected = true;
                    removeLastLine();
                    addLine("connected to: " + std::string(SDLNet_ResolveIP(&mServerAddress)), GREEN);
                }
                else
                {
                    removeLastLine();
                    addLine("connection failed", RED);
                }
            }
            else
            {
                removeLastLine();
                addLine("connection failed", RED);
            }
        }
        else if (command == "/disconnect")
        {
            if (Net::sendMessage("/disconnect", mServerSocket))
            {
                mStatus.connected = false;
                addLine("diconnected from: " + std::string(SDLNet_ResolveIP(&mServerAddress)), GREEN );
                Net::closeConnection(mServerSocket);
            }
            else
            {
                addLine("disconnection failed", RED);
            }
        }
    }
    else
    {
        if (!mStatus)
        {
            addLine(input);
        }
        else
        {
            addLine("sending ...", mTheme.accentColor);
            if (Net::sendMessage(input, mServerSocket))
            {
                removeLastLine();
                addLine("me: " + input);
            }
            else
            {
                removeLastLine();
                addLine("error sending message", RED);
            }
        }
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
    
    if (IsKeyPressed(KEY_ENTER))
    {
        submit(mTyping);
        
        mTyping.clear();
        mTypingPos = 0;
    }
    
    if (SDLNet_SocketReady(mServerSocket))
    {
        const std::string msg = Net::receiveFrom(mServerSocket);
        if (!msg.empty())
        {
            addLine(msg);
        }
    }
    
    mBlinkTime -= GetFrameTime();
    if (mBlinkTime <= 0) resetBlink(!mBlink);
}

void Chat::draw() const
{
    const float h = static_cast<float>(GetScreenHeight());
    const float w = static_cast<float>(GetScreenWidth());
    
    ClearBackground(mTheme.mainColor);
    
    float yAnchor = h*0.85f-mTheme.textSize-10;
    for (size_t i = mChat.size()-1;; i--)
    {
        float offset = (float)(mChat.size()-1-i) * mTheme.textSize + 1.0f;
        drawLine(mChat[i].lText, yAnchor-offset, mChat[i].lColor);
        if (i == 0) break;
    }
    
    DrawRectangleV({ 0, h*0.85f}, {w,    h}, mTheme.secondaryColor);  // bottom box
    DrawRectangleV({ 0, h*0.85f}, {w,    2}, mTheme.accentColor);     // bottom line
    
    DrawRectangleV({10, h*0.85f+10}, {w-20, mTheme.textSize+10}, mTheme.mainColor); // text box
    DrawTextEx(mTheme.textFont, mTyping.c_str(), { 20, h*0.85f+15 }, mTheme.textSize, 1.0f, mTheme.textColor);
    
    const float blinkPos = MeasureTextEx(mTheme.textFont, mTyping.substr(0, mTypingPos).c_str(), mTheme.textSize, 1.0f).x+2;
    if (mBlink) DrawRectangleV({ blinkPos+20, h*0.85f+15 }, { 2, mTheme.textSize }, mTheme.textColor);
    
    DrawTextEx(mTheme.titleFont,"KiliChat", { 20.0f, h*0.925f }, mTheme.titleSize, 1.0f, mTheme.textColor);
    
    const std::string server = mStatus.ip + ":" + std::to_string(mStatus.port);
    const float serverTextSize = MeasureTextEx(mTheme.textFont, server.c_str(), mTheme.textSize, 1.0f).x;
    DrawTextEx(mTheme.textFont, server.c_str(), { w-serverTextSize-20.0f, h*0.925f }, mTheme.textSize, 1.0f, mStatus ? GREEN : RED);
}