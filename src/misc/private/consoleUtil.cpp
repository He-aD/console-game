#include "consoleUtil.h"
#include <iostream>
#include "constants.h"

consoleUtil::consoleUtil() {
	this->consoleHandle = nullptr;
	this->cursorCoordinate.X = 0;
	this->cursorCoordinate.Y = 0;

    // hydrate console ptrs
	this->consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	GetConsoleScreenBufferInfo(this->consoleHandle, &this->consoleInfo);
}

const std::string consoleUtil::renderTextXCentered(const char* inText, const bool gatherInput) {
	std::string input;

    // calculate cursor x offset from inText to be centered
	const unsigned short xOffset = (this->consoleInfo.dwSize.X - (short)strlen(inText)) / 2;
	this->cursorCoordinate.X = xOffset;

    // set cursor position and render
	this->setCursorPosition();
	std::cout << inText;

    // gather input if asked
	if (gatherInput) {
		std::cin >> input;
	}

    // return eventual input
	return input;
}

const std::string consoleUtil::renderTextXCentered(std::ostringstream& oss, const bool gatherInput) {
	std::string input{ this->renderTextXCentered(oss.str().c_str(), gatherInput) };

	oss.str("");
	oss.clear();

	return input;
}

void consoleUtil::clearConsole() {
    SMALL_RECT scrollRect{};
    CHAR_INFO fill{};

    // Scroll the rectangle of the entire buffer.
    scrollRect.Left = 0;
    scrollRect.Top = 0;
    scrollRect.Right = this->consoleInfo.dwSize.X;
    scrollRect.Bottom = this->consoleInfo.dwSize.Y;

    // Scroll it upwards off the top of the buffer with a magnitude of the entire height.
    this->cursorCoordinate.X = 0;
    this->cursorCoordinate.Y = (SHORT)(0 - this->consoleInfo.dwSize.Y);

    // Fill with empty spaces with the buffer's default text attribute.
    fill.Char.UnicodeChar = TEXT(' ');
    fill.Attributes = this->consoleInfo.wAttributes;

    // Do the scroll
    ScrollConsoleScreenBuffer(this->consoleHandle, &scrollRect, NULL, this->cursorCoordinate, &fill);

    // Move the cursor to the top left corner too.
    this->consoleInfo.dwCursorPosition.X = 0;
    this->consoleInfo.dwCursorPosition.Y = 0;

    SetConsoleCursorPosition(this->consoleHandle, this->consoleInfo.dwCursorPosition);
}

void consoleUtil::clearX(const unsigned short nbCharacters) {
    this->setCursorPosition();

    for (unsigned short i = 0; i < nbCharacters; i++) {
        std::cout << " ";
    }
}

void consoleUtil::setSize() {
    HWND hwnd = GetConsoleWindow();

    MoveWindow(hwnd, 
        constants::consolePosition.x, 
        constants::consolePosition.y,
        constants::consolePosition.width,
        constants::consolePosition.height,
        TRUE);
}