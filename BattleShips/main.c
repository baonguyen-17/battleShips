/*
* Project Team: Neil, Bao, James, David, Piers
* Date: 9/28/2023
* Purpose: Making a really cool game!
*/

#define _CRT_SECURE_NO_WARNINGS

#include "MyTemplate.h"

#define CLS system("cls")

#define ROW 10
#define COL 10


typedef struct {
	char name[255];
	char letter;
	int length;
	int hits;
} Ship;

typedef struct {
	int shotsFired;
	int highest;
	int lowest;
	bool needsNewGame;
} Stats;

void setupBoard(char gameBoard[ROW][COL], char originalBoard[ROW][COL], Ship ships[5]);

void addShip(char gameBoard[10][10], Ship ship);

bool shipImpedes(char gameBoard[10][10], bool vert, int row, int col, int length);

void displayGame(char gameBoard[10][10], char originalBoard[10][10], Ship ships[5], Stats pstats);

void playGame(char[ROW][COL], char originalGame[ROW][COL], Ship ships[], Stats*);

bool validateInput(char[]);

int getNum(char input[]);

void formatInput(char* input);

void clearInputBuffer();

bool isPositionSunk(char letter, Ship* ships);

bool gameWon(Ship* ships);

void saveGame(char gameBoard[10][10], char originalBoard[10][10], Ship ships[5], Stats stats);

void loadGame(char gameBoard[10][10], char originalBoard[10][10], Ship* ships, Stats* pstats);

void gameOver(char gameBoard[10][10], char originalGame[10][10], Ship* ships, Stats* pstats);

void resetGame(char gameBoard[10][10], char originalGame[10][10], Ship ships[5], Stats* pstats);

void cheatPrint(char board[10][10]);

int main() {
	srand(time(NULL));
	Ship ships[5] = {
			{"Zulu",   'Z', 2, 0},
			{"Yeti",   'Y', 3, 0},
			{"XMax",   'X', 3, 0},
			{"Wiener", 'W', 4, 0},
			{"Violet", 'V', 5, 0}
	};
	char gameBoard[ROW][COL];
	char originalBoard[ROW][COL];

	Stats game = { 0, -1, -1, true };

	loadGame(gameBoard, originalBoard, ships, &game);

	if (game.needsNewGame)
		resetGame(gameBoard, originalBoard, ships, &game);


	playGame(gameBoard, originalBoard, ships, &game);

	notif("Thanks for playing!");
	pause(1);

	return 0;
}

void displayGame(char gameBoard[10][10], char originalBoard[10][10], Ship ships[5], Stats stats) {
	printf("\n");

	char divider[255] = "----";
	printf("   |");

	for (int i = 0; i < COL; i++) {
		strcat(divider, "----");
		printf(" %c |", 'A' + i);
	}

	printf("\n");
	strcat(divider, "\0");

	for (int i = 0; i < ROW; i++) {
		printf("%s\n", divider);
		printf("%2i |", i + 1);
		for (int j = 0; j < COL; j++) {
			if (gameBoard[i][j] == 'H' && isPositionSunk(originalBoard[i][j], ships)) {
				printf(" %c |", originalBoard[i][j]);
			}
			else if (gameBoard[i][j] == 'M' || gameBoard[i][j] == 'H') // H = hit, M = miss
				printf(" %c |", gameBoard[i][j]);
			else
				printf("   |");
		}
		printf("\n");
	}
	for (int i = 0; i < 5; i++) {
		if (ships[i].length == ships[i].hits)
			printf("\n%s\t%c: \tSUNK!", ships[i].name, ships[i].letter);
		else
			printf("\n%s\t%c: \tafloat...", ships[i].name, ships[i].letter);
	}

	printf("\n\nShots Fired: %i\n", stats.shotsFired);
}

bool isPositionSunk(char letter, Ship* ships) {
	for (int z = 0; z < 5; z++) {
		if (ships[z].hits == ships[z].length && ships[z].letter == letter)
			return true;

	}

	return false;
}

void setupBoard(char gameBoard[ROW][COL], char originalBoard[ROW][COL], Ship ships[5]) {
	for (int i = 0; i < ROW; i++)
		for (int j = 0; j < COL; j++)
			gameBoard[i][j] = ' ';

	for (int i = 0; i < 5; i++)
		addShip(gameBoard, ships[i]);

	for (int i = 0; i < ROW; i++)
		for (int j = 0; j < COL; j++)
			originalBoard[i][j] = gameBoard[i][j];
}

void addShip(char gameBoard[10][10], Ship ship) {
	bool vert = rand() % 2;
	int row = 0;
	int col = 0;
	do {
		if (vert) {
			row = rand() % (ROW - ship.length);
			col = rand() % COL;
		}
		else {
			row = rand() % ROW;
			col = rand() % (COL - ship.length);
		}
	} while (shipImpedes(gameBoard, vert, row, col, ship.length));

	for (int i = 0; i < ship.length; i++) {
		if (vert)
			gameBoard[row + i][col] = ship.letter;
		else
			gameBoard[row][col + i] = ship.letter;
	}
}

bool shipImpedes(char gameBoard[10][10], bool vert, int row, int col, int length) {
	for (int i = 0; i < length; i++) {
		if (vert) {
			if (gameBoard[row + i][col] != ' ')
				return true;
		}
		else {
			if (gameBoard[row][col + i] != ' ')
				return true;
		}
	}

	return false;
}


void playGame(char gameBoard[ROW][COL], char originalGame[ROW][COL], Ship ships[], Stats* pstats) {
	char playerInput[255];

	while (strcmp(playerInput, "QQ") != 0 && !gameWon(ships)) {
		bool badInput = validateInput(playerInput);


		if (strcmp(playerInput, "CC") == 0) {
			cheatPrint(originalGame);
			continue;
		}

		if (strcmp(playerInput, "SS") == 0) {
			saveGame(gameBoard, originalGame, ships, *pstats);
			continue;
		}

		if (strcmp(playerInput, "LL") == 0) {
			loadGame(gameBoard, originalGame, ships, pstats);
			displayGame(gameBoard, originalGame, ships, *pstats);
			continue;
		}

		if (strcmp(playerInput, "RR") == 0) {
			CLS;
			resetGame(gameBoard, originalGame, ships, pstats);
			continue;
		}

		if (strcmp(playerInput, "QQ") == 0)
			return;

		if (badInput == true) {
			printf("\nBad coordinates!");
			system("pause > null");
			continue;
		}

		int colLetter = playerInput[0] - 'A';
		int rowNumber = getNum(playerInput);

		switch (gameBoard[rowNumber][colLetter]) {
		case ' ':
			gameBoard[rowNumber][colLetter] = 'M';
			pstats->shotsFired++;
			break;
		case 'M':
		case 'H':
			printf("This spot has been fired! Choose another one!\n");
			system("pause > null");
			break;
		default:
			for (int i = 0; i < 5; i++) {
				if (gameBoard[rowNumber][colLetter] == ships[i].letter)
					ships[i].hits++;
			}
			gameBoard[rowNumber][colLetter] = 'H';
			pstats->shotsFired++;
			break;
		}

		CLS;
		displayGame(gameBoard, originalGame, ships, *pstats);
	}

	gameOver(gameBoard, originalGame, ships, pstats);
}

void cheatPrint(char board[10][10]) {
	printf("\n");

	for (int i = 0; i < 10; ++i) {
		for (int j = 0; j < 10; ++j) {
			printf("%c-", board[i][j]);
		}
		printf("\n");
	}
}

void gameOver(char gameBoard[10][10], char originalGame[10][10], Ship* ships, Stats* pstats) {
	pstats->needsNewGame = true;

	printf("\nHere is the state of the finished game:\n");
	displayGame(gameBoard, originalGame, ships, *pstats);
	printf("\nGGs.");

	if (pstats->lowest == -1 || pstats->lowest > pstats->shotsFired) {
		pstats->lowest = pstats->shotsFired;
		printf("\nNew best record!");
	}

	if (pstats->highest == -1 || pstats->highest < pstats->shotsFired) {
		pstats->highest = pstats->shotsFired;
		printf("\nNew worst record!");
	}

	pause(2);

	printf("\nA total of %d shots were fired over the course of this game.", pstats->shotsFired);

	pause(2);

	printf("\nThe best record so far is:  %d", pstats->lowest);

	pause(2);

	printf("\nThe worst record so far is: %d", pstats->highest);

	pause(2);

	saveGame(gameBoard, originalGame, ships, *pstats);

	char input;

	clearInputBuffer();
	printf("\nWould you like to play again? (Y/n): ");
	scanf("%c", &input);

	if (toupper(input) == 'Y') {
		CLS;
		resetGame(gameBoard, originalGame, ships, pstats);
		playGame(gameBoard, originalGame, ships, pstats);
	}
}

void resetGame(char gameBoard[10][10], char originalGame[10][10], Ship ships[5], Stats* pstats) {
	pstats->needsNewGame = false;
	pstats->shotsFired = 0;
	for (int i = 0; i < 5; i++)
		ships[i].hits = 0;

	setupBoard(gameBoard, originalGame, ships);
	displayGame(gameBoard, originalGame, ships, *pstats);
	// playGame(gameBoard, originalGame, ships, pstats);
}

void loadGame(char gameBoard[10][10], char originalBoard[10][10], Ship* ships, Stats* pstats) {
	FILE* ptr = fopen("Battleship.bin", "rb");

	CLS;
	notif("Loading .");
	pause(1);
	CLS;
	notif("Loading . .");
	pause(1);
	CLS;
	notif("Loading . . .");
	pause(1);
	CLS;


	if (ptr == NULL) {
		notif("[loadGame] Error! File could not open properly");
		return;
	}

	else if (fread(originalBoard, sizeof(char[10][10]), 1, ptr) < 1) {
		notif("[loadGame] Failed to write game board.");
		return;
	}

	else if (fread(gameBoard, sizeof(char[10][10]), 1, ptr) < 1) {
		notif("[loadGame] Failed to write game board.");
		return;
	}

	else if (fread(ships, sizeof(Ship[5]), 1, ptr) < 1) {
		notif("[loadGame] Failed to write ships.");
		return;
	}

	else if (fread(pstats, sizeof(Stats), 1, ptr) < 1) {
		notif("[loadGame] Failed to write ships.");
		return;
	}

	else {
		notif("Game load successful!");
		pause(1);
		CLS;
		displayGame(gameBoard, originalBoard, ships, *pstats);
		return;
	}

	
	fclose(ptr);
	
}

void saveGame(char gameBoard[10][10], char originalBoard[10][10], Ship ships[5], Stats stats) {
	FILE* ptr = fopen("Battleship.bin", "wb");
	if (ptr == NULL) {
		printf("\n[saveGame] Error! File could not open properly");
		return;
	}

	if (fwrite(originalBoard, sizeof(char[10][10]), 1, ptr) < 1) {
		printf("\n[saveGame] Failed to write game board.");
		return;
	}

	if (fwrite(gameBoard, sizeof(char[10][10]), 1, ptr) < 1) {
		printf("\n[saveGame] Failed to write game board.");
		return;
	}

	if (fwrite(ships, sizeof(Ship[5]), 1, ptr) < 1) {
		printf("\n[saveGame] Failed to write ships.");
		return;
	}

	if (fwrite(&stats, sizeof(Stats), 1, ptr) < 1) {
		printf("\n[saveGame] Failed to write ships.");
		return;
	}

	CLS;
	notif("Saving .");
	pause(1);
	CLS;
	notif("Saving . .");
	pause(1);
	CLS;
	notif("Saving . . .");
	pause(1);
	CLS;
	notif("Game Saved");
	pause(1);
	CLS;
	displayGame(gameBoard, originalBoard, ships, stats);


	fclose(ptr);


}

bool gameWon(Ship* ships) {
	for (int i = 0; i < 5; i++)
		if (ships[i].length != ships[i].hits)
			return false;

	return true;
}

void formatInput(char* input) {
	for (int i = 0; i < strlen(input); i++)
		input[i] = toupper(input[i]);
}

bool validateInput(char input[]) {
	char getInput[] = "\n[QQ]: Quit"
		"\n[SS]: Save Game"
		"\n[LL]: Load Game"
		"\n[RR]: Reset Game"
		"\n[CC]: Cheat Print"
		"\n\nEnter coordinates to fire (Ex: A1): ";

	printf(getInput);
	scanf("%s", input);

	formatInput(input);

	if (strlen(input) > 3 || strlen(input) == 0)
		return true;

	if (isalpha(input[0]) == 0 || isdigit(input[1]) == 0)
		return true;

	if (input[0] - 'A' >= 10 || input[1] - '0' == 0)
		return true;

	if (input[1] - '0' == 1 && input[2] - '0' > 0)
		return true;

	if (input[1] - '0' > 1 && input[2] - '0' >= 0)
		return true;

	return false;
}

int getNum(char input[]) {
	int result;

	if (strlen(input) == 2)
		result = input[1] - '1';
	else
		result = 9;

	return result;
}

void clearInputBuffer() {
	int c;
	while ((c = getchar()) != '\n' && c != EOF) {}
}