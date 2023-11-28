
#define _CRT_SECURE_NO_WARNINGS	// turns off safe commands

#include <stdio.h>

#include <stdlib.h>

#include <ctype.h>				// toupper is in here

#include <string.h>

#include <stdbool.h>

#include <time.h>

#include <math.h>

#define CLS system("cls")		// Clear the screen

#define PAUSE myPause()			// Pause the screen

#define FLUSH myFlush()			// Clear the keyboard buffer

//==========================================================

void myPause();

void marquee(char [], char);

void myFlush();

void notif(char mess[]);

void pause(int num);

void format(char string[]);


//==========================================================



void marquee(char m[], char s) {

	int size = strlen(m) + 6;

	printf("\n");

	int i;

	for ( i = 0; i < size; i++) {

		printf("%c", s);

	}// end for

	printf("\n%c%c %s %c%c\n", s, s, m, s, s);

	for (i = 0; i < size; i++) {

		printf("%c", s);

	}// end for

	printf("\n\n");

} // end marquee()

//==================================================

void myFlush() {

	while (getchar() != '\n');

} // end of myFlush()

//==================================================

void myPause() {

	printf("\nPress ENTER to continue....\n");

	getchar();

} // end myPause()

//==================================================



void notif(char mess[]) {
	int size = strlen(mess) + 6;
	printf("\n");
	for (int i = 0; i < size; i++)
		printf("-");	// end for loop
	printf("\n|  %s  |\n", mess);
	for (int i = 0; i < size; i++)
		printf("-");	// end for loop
	printf("\n\n");
}  // end notif()

//==================================================

void pause(int num)
{
	int iCurrentTime = 0;
	int iElapsedTime = 0;
	iCurrentTime = time(NULL);
	do {
		iElapsedTime = time(NULL);
	} while ((iElapsedTime - iCurrentTime) < num);	// END DO-WHILE

}	// end pause()

//==================================================

void format(char string[]) {
		string[0] = toupper(string[0]);

		for (int i = 0; i < strlen(string); i++) {

			if (string[i] == ' ')
				string[i + 1] = toupper(string[i + 1]);

			else string[i + 1] = tolower(string[i + 1]);
		}	// END FOR

}	// end format()
