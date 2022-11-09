#pragma once

#include <Windows.h>
#include <string>
#include <sstream>

class consoleUtil {
public:
	consoleUtil();

	// remove all characters from the console
	void clearConsole();

	// shorter function for readability
	void SetCursorPosition() { SetConsoleCursorPosition(this->consoleHandle, this->cursorCoordinate); }

	// render text centered at cursorCoordinate.Y. Child classes are responsible to set cursorCoordinate.Y
	const std::string renderTextXCentered(const char* inText, const bool gatherInput = false);
	const std::string renderTextXCentered(std::ostringstream& oss, const bool gatherInput = false);

	CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
	HANDLE consoleHandle;
	COORD cursorCoordinate;
};