#pragma once

#include <Windows.h>
#include <string>
#include <sstream>

// console utils function used by renderers
class consoleUtil {
public:
	consoleUtil();

	// remove all characters from the console
	void clearConsole();

	// clear nb of characters in parameter from cursor position to console's right direction
	void clearX(const unsigned short nbCharacters);

	// set console size for nice game rendering
	void setSize();

	// shorter function for readability
	void setCursorPosition() { SetConsoleCursorPosition(this->consoleHandle, this->cursorCoordinate); }

	// render text centered at cursorCoordinate.Y. Child classes are responsible to set cursorCoordinate.Y before calling
	const std::string renderTextXCentered(const char* inText, const bool gatherInput = false, const bool useGetch = false);
	const std::string renderTextXCentered(std::ostringstream& oss, const bool gatherInput = false, const bool useGetch = false);

	CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
	HANDLE consoleHandle;
	COORD cursorCoordinate;
};