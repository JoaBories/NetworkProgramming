#include <SDL.h>
#include <SDL_net.h>
#include <iostream>
#include <string>

#include "Chat.h"
#include "Utils.h"
#include "Net.h"

//======================================
//  _____  _     _____ _____ _   _ _____ 
// /  __ \| |   |_   _|  ___| \ | |_   _|
// | /  \/| |     | | | |__ |  \| | | |  
// | |    | |     | | |  __|| . ` | | |  
// | \__/\| |_____| |_| |___| |\  | | |  
//  \____/\_____/\___/\____/\_| \_/ \_/  
//======================================

int main(int argc, char* argv[]) {
	if (SDLNet_Init() == -1) {
		std::cerr << "SDLNet_Init error: " << SDLNet_GetError() << '\n';
		return 1;
	}

	SetConfigFlags(FLAG_WINDOW_RESIZABLE);
	InitWindow(600, 800, "Kili chat");
	SetTargetFPS(60);
	
	Font textFont = LoadFontEx("resources/CascadiaCode.ttf", 24, nullptr, 0);
	Font titleFont = LoadFontEx("resources/CascadiaCode.ttf", 50, nullptr, 0);
	
	const auto darkTheme = theme(
		{25, 25, 25, 255},
		{50, 50, 50, 255},
		{ 245, 245, 245, 255 },
		{ 244, 89, 41, 255 },
		textFont, titleFont,
		24.0f, 50.0f
	);
	
	serverStatus status { "localhost", 4242, false };
	
	Chat* chat = new Chat(darkTheme, status);

	std::cout << "Kili Chat client init !\n";
	
	while (!WindowShouldClose())
	{
		chat->update();
		
		if (chat->shouldQuit()) break;
		
		BeginDrawing();
		chat->draw();
		EndDrawing();
	}
	
	UnloadFont(textFont);
	UnloadFont(titleFont);
	delete chat;
	
	SDLNet_Quit();
	CloseWindow();
	
	return 0;
}
