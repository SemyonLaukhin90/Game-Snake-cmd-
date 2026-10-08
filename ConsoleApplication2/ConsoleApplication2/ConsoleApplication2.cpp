#include <iostream>
#include <windows.h>
#include <conio.h>
#include <cstdlib>
#include <ctime>

const int width = 50;
const int height = 25;

int snakePositions[2] = { 10, 10 };
int snakeOldPositions[500][2];
int snakeLength = 3;

int leftVector[2] = { -1,  0 };
int rightVector[2] = { 1,  0 };
int downVector[2] = { 0,  1 };
int upVector[2] = { 0, -1 };

int currentVector[2] = { 1, 0 };

int spawnApplePositions[2][2];
bool isSpawnedApple[2] = { false, false };

bool isDeath = false;

int gameSpeed = 150;
int currentXP = 0;
int recordXP = 0;

void setCursorPosition(int x, int y) {
	COORD coord;
	coord.X = x;
	coord.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void setCursorVisibility(bool visible) {
	HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO cursorInfo;

	GetConsoleCursorInfo(consoleHandle, &cursorInfo);

	cursorInfo.bVisible = visible;

	SetConsoleCursorInfo(consoleHandle, &cursorInfo);
}

void setTerritorion() {
	for (int i = 0; i <= width; i++) {
		setCursorPosition(i, 0);
		std::cout << "-";
		setCursorPosition(i, height);
		std::cout << "-";
	}
	for (int i = 0; i <= height; i++) {
		setCursorPosition(0, i);
		std::cout << "|";
		setCursorPosition(width, i);
		std::cout << "|";
	}
}

void teleport() {
	if (snakePositions[0] < 1) {
		snakePositions[0] = width - 1;
	}
	else if (snakePositions[0] >= width) {
		snakePositions[0] = 1;
	}
	if (snakePositions[1] < 1) {
		snakePositions[1] = height - 1;
	}
	else if (snakePositions[1] >= height) {
		snakePositions[1] = 1;
	}
}

void spawnApple(int index) {
	bool ok;
	do {
		ok = true;
		spawnApplePositions[index][0] = 1 + std::rand() % (width - 1);
		spawnApplePositions[index][1] = 1 + std::rand() % (height - 1);

		if (spawnApplePositions[index][0] == snakePositions[0] && spawnApplePositions[index][1] == snakePositions[1]) {
			ok = false;
		}

		for (int j = 0; j < snakeLength&& ok; j++) {
			if (spawnApplePositions[index][0] == snakeOldPositions[j][0] && spawnApplePositions[index][1] == snakeOldPositions[j][1]) {
				ok = false;
			}
		}

		int other = 1 - index;
		if (isSpawnedApple[other] && spawnApplePositions[index][0] == spawnApplePositions[other][0] && spawnApplePositions[index][1] == spawnApplePositions[other][1]) {
			ok = false;
		}
	} while (!ok);

	isSpawnedApple[index] = true;
}

void eatApple(int index) {
	isSpawnedApple[index] = false;
	snakeLength += 2;
	currentXP += (int)(snakeLength * (snakeLength > 50 ? 0.5 : 1.5));
	if (gameSpeed > 50) {
		gameSpeed -= 5;
	}
	else if (gameSpeed > 30) {
		gameSpeed -= 4;
	}
	else if (gameSpeed > 15) {
		gameSpeed -= 1;
	}
	else {
		gameSpeed = 15;
	}
}

void clearData() {
	snakePositions[0] = 10;
	snakePositions[1] = 10;
	snakeLength = 3;
	gameSpeed = 150;
	currentVector[0] = 1;
	currentVector[1] = 0;
	isSpawnedApple[0] = false;
	isSpawnedApple[1] = false;
	spawnApplePositions[0][0] = 0;
	spawnApplePositions[0][1] = 0;
	spawnApplePositions[1][0] = 0;
	spawnApplePositions[1][1] = 0;
	currentXP = 0;

	for (int i = 0; i < 500; i++) {
		snakeOldPositions[i][0] = 0;
		snakeOldPositions[i][1] = 0;
	}
}

int main()
{
	std::srand(static_cast<unsigned int>(std::time(nullptr)));

	setCursorVisibility(false);

	setTerritorion();

	while (true) {


		if (_kbhit()) {
			unsigned char key = _getch();

			switch (key) {
			case 'e':
			case 211:
				if (isDeath) {
					isDeath = false;
					clearData();
					system("cls");
					setTerritorion();
				}
				break;

			case 'w':
			case 230:
				if (currentVector[1] != 1) {
					currentVector[0] = upVector[0];
					currentVector[1] = upVector[1];
				}
				break;

			case 's':
			case 235:
				if (currentVector[1] != -1) {
					currentVector[0] = downVector[0];
					currentVector[1] = downVector[1];
				}
				break;

			case 'a':
			case 228:
				if (currentVector[0] != 1) {
					currentVector[0] = leftVector[0];
					currentVector[1] = leftVector[1];
				}
				break;

			case 'd':
			case 162:
				if (currentVector[0] != -1) {
					currentVector[0] = rightVector[0];
					currentVector[1] = rightVector[1];
				}
				break;
			}
		}

		if (!isDeath) {
			for (int j = snakeLength; j > 0; j--) {
				snakeOldPositions[j][0] = snakeOldPositions[j - 1][0];
				snakeOldPositions[j][1] = snakeOldPositions[j - 1][1];
			}

			snakeOldPositions[0][0] = snakePositions[0];
			snakeOldPositions[0][1] = snakePositions[1];

			snakePositions[0] += currentVector[0];
			snakePositions[1] += currentVector[1];

			teleport();

			for (int j = 1; j < snakeLength; j++) {
				if (snakePositions[0] == snakeOldPositions[j][0] && snakePositions[1] == snakeOldPositions[j][1]) {
					isDeath = true;
				}
			}

			for (int i = 0; i < 2; i++) {
				if (!isSpawnedApple[i]) {
					spawnApple(i);
				}
			}

			for (int i = 0; i < 2; i++) {
				if (isSpawnedApple[i] && spawnApplePositions[i][0] == snakePositions[0] && spawnApplePositions[i][1] == snakePositions[1]) {
					eatApple(i);
				}
			}

			if (currentXP > recordXP) {
				recordXP = currentXP;
			}

			setCursorPosition(spawnApplePositions[0][0], spawnApplePositions[0][1]);
			std::cout << "x" << std::flush;

			setCursorPosition(spawnApplePositions[1][0], spawnApplePositions[1][1]);
			std::cout << "x" << std::flush;

			setCursorPosition(snakePositions[0], snakePositions[1]);
			std::cout << "o" << std::flush;

			setCursorPosition(0, height + 1);
			std::cout << "Record XP: " << recordXP << " | Current XP: " << currentXP << "     " << std::flush;

			if (!isDeath) {
				int lastX = snakeOldPositions[snakeLength - 1][0];
				int lastY = snakeOldPositions[snakeLength - 1][1];
				if (lastX > 0 && lastX < width && lastY > 0 && lastY < height) {
					setCursorPosition(lastX, lastY);
					std::cout << " " << std::flush;
				}
			}
		}
		else {
			setCursorPosition(35, height + 1);
			std::cout << "Game over, press 'e' to restart!" << std::endl;
		}

		Sleep(gameSpeed);
	}
	return 0;
}