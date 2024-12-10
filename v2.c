#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <windows.h>
#include <conio.h>

#define WIDTH 43
#define HEIGHT 21
#define INITIAL_LIFE 3

int inputmode = 1; //Input mode initialized to WASD

// Function to display the maze on the screen for a given floor
void printMaze(int floor) {
    char filename[100]; // Define a character array to store the filename

    // Create the file path string dynamically based on the current floor number
    sprintf(filename, "./map/%dfloor.txt", floor);

    // Clear the screen to prepare for the new maze display
    system("cls");

    // Open the corresponding map file for the given floor
    FILE *pg = fopen(filename, "r");
    
    // Check if the file was opened successfully
    if (pg == NULL) {
        return; // Exit the function
    }

    char line[256]; // Buffer to hold each line of the file (max length = 255 characters + null terminator)
    
    // Read each line of the file until the end is reached
    while (fgets(line, sizeof(line), pg) != NULL) {
        // Print the line to the screen
        printf("%s", line);
    }

    // Close the file after reading its contents
    fclose(pg);

    // Print an empty line for better readability after displaying the maze
    printf("\n");
}


// Function to save the game progress to a file
void saveProgress(int floor, int x, int y, int inputmode) {
	char move;
	printf("save progress? (y/n)\n");
	printf("Input: ");
	scanf(" %c", &move);
    while (move != 'y' && move != 'n') {
        printf("Invalid option!! Input again: \n");
        scanf(" %c", &move);
        while (getchar() != '\n');
    }
    if (move == 'y') {
        // Open the file "save.txt" for writing
    	FILE *file = fopen("save.txt", "w");
    
    	// If the file can't be opened, notify the user and stop execution
    	if (file == NULL) {
        	printf("Save failed!\n");
        	return;
    	}
    
    	// Write the current game progress (floor, x, y, and inputmode) to the file
    	fprintf(file, "%d %d %d %d\n", floor, x, y, inputmode);
    
    	// Close the file
    	fclose(file);
	} else if (move == 'n') {
		remove("save.txt");
		return;
	}
}

// as sme as saveProgress fun.
void savedouble(int floor, int x, int y, int a, int b, int p1s, int p2s) {
	char move;
	printf("save progress? (y/n)\n");
	printf("Input: ");
	scanf(" %c", &move);
    while (move != 'y' && move != 'n') {
        printf("Invalid option!! Input again: \n");
        scanf(" %c", &move);
        while (getchar() != '\n');
    }
    if (move == 'y') {
        FILE *file = fopen("double.txt", "w");
    	if (file == NULL) {
        	printf("Save faild!\n");
        	return;
    	}
    	fprintf(file, "%d %d %d %d %d %d %d\n", floor, x, y, a, b, p1s, p2s);
    	fclose(file);
	} else if (move == 'n') {
		remove("double.txt");
		return;
	}
}

// Function to load the saved game progress from a file
bool loadProgress(int *floor, int *x, int *y, int *inputmode) {
    // Open the file "save.txt" for reading
    FILE *file = fopen("save.txt", "r");
    
    // If the file can't be opened, return false to indicate failure
    if (file == NULL) {
        return false;
    }
    
    // Read saved progress values from the file
    if (fscanf(file, "%d %d %d %d", floor, x, y, inputmode) != 4) {
        // If reading fails, close the file and return false
        fclose(file);
        return false;
    }
    
    // Close the file after reading
    fclose(file);
    
    // Return true to indicate success
    return true;
}

// as same as loadProgress fun.
bool loaddouble(int *floor, int *x, int *y, int *a, int *b, int *p1s, int *p2s) {
    FILE *file = fopen("double.txt", "r");
    if (file == NULL) {
        return false;
    }
    if (fscanf(file, "%d %d %d %d %d %d %d\n", floor, x, y, a, b, p1s, p2s) != 7) {
        fclose(file);
        return false;
    }
    fclose(file);
    return true;
}

// Function to check if a specific coordinate in the console contains a space character
bool isSpace(int x, int y) {
    CHAR_INFO ci; // Structure to hold character information read from the console
    COORD bufferSize = {1, 1}; // Size of the buffer, here it is just 1x1 to read a single cell
    COORD bufferCoord = {0, 0}; // Coordinate offset within the buffer, starting at (0,0)
    
    // Define the region of the console to read from; it only reads a single cell at coordinate (x, y)
    SMALL_RECT readRegion = {x, y, x, y};

    // Use ReadConsoleOutput to check the content of the specified console cell
    if (ReadConsoleOutput(GetStdHandle(STD_OUTPUT_HANDLE), &ci, bufferSize, bufferCoord, &readRegion)) {
        // If the read is successful, check if the character at the location is a space (' ')
        return ci.Char.AsciiChar == ' ';
    } else {
        // If the read fails, return false
        return false; 
    }
}

// as same as isSpace fun.
bool isplayer2(int x, int y) {
    CHAR_INFO ci;
    COORD bufferSize = {1, 1}; 
    COORD bufferCoord = {0, 0}; 
    SMALL_RECT readRegion = {x, y, x, y}; 

    if (ReadConsoleOutput(GetStdHandle(STD_OUTPUT_HANDLE), &ci, bufferSize, bufferCoord, &readRegion)) {
        return ci.Char.AsciiChar == 'B';
    } else {
        return false; 
    }
}

// as same as isSpace fun.
bool isplayer1(int x, int y) {
    CHAR_INFO ci;
    COORD bufferSize = {1, 1}; 
    COORD bufferCoord = {0, 0}; 
    SMALL_RECT readRegion = {x, y, x, y}; 

    if (ReadConsoleOutput(GetStdHandle(STD_OUTPUT_HANDLE), &ci, bufferSize, bufferCoord, &readRegion)) {
        return ci.Char.AsciiChar == 'A';
    } else {
        return false; 
    }
}

// Function to hide the console cursor
void hidecursor()
{
    // Get the handle to the standard output stream
    HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    
    // Create a structure to hold cursor information
    CONSOLE_CURSOR_INFO info;
    
    // Set the cursor size and visibility
    info.dwSize = 100;       // Set cursor size
    info.bVisible = FALSE;   // Set visibility to FALSE to hide the cursor
    
    // Apply the cursor settings to the console
    SetConsoleCursorInfo(consoleHandle, &info);
}


// Function to move the console cursor to a specified position
void gotoxy(int x, int y) {
    // Define a coordinate structure
    COORD coord;
    
    // Set the horizontal and vertical coordinates
    coord.X = x; // Set horizontal position
    coord.Y = y; // Set vertical position
    
    // Move the cursor to the specified position
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

// Function to prompt the user to enter a valid number between range
int enter(int L, int H) {
    int input; // Variable to store user input

    // Infinite loop to ensure valid input is received
    while (1) {
        // Prompt user to enter a number between L and H
        printf("\nEnter a number between %d and %d: ", L, H);
        
        // Attempt to read user input
        if (scanf("%d", &input)) {
            // Check if the input is within the valid range
            if (input >= L && input <= H) {
                // Input is valid, exit the loop
                break;
            } else {
                // Input is outside the range, notify the user input again
                printf("\nInvalid option!! Input again: ");
            }
        } else {
            // Input is not a number, notify the user inpuut again
            printf("\nInvalid option!! Input again: ");
            
            // Clear invalid input from the input buffer
            while (getchar() != '\n');
        }
    }

    // Clear any remaining characters in the input buffer
    while (getchar() != '\n'); 
    
    // Return the valid input to the caller
    return input;
}

//different inputmode
char shuru(int mode) {
	char move;
	
	//WASD mode
	if (mode == 1) {
		move = getch(); // get input from user
		if (move == 'w' || move == 'a' || move == 's' || move == 'd' || move == 'q') {
			return move; //return result
		} else { // if input invaild
			return '\0'; //return nothing
		}
	}
	
	//Arrow mode
	if (mode == 2) {
		move = getch();
		if (move == -32 || move == 224) { //isn't special key?
			move = getch();
			switch (move) {
                case 72: return 'w'; // up
                case 80: return 's'; // down
                case 75: return 'a'; // left
                case 77: return 'd'; // right
            }
		} else if (move == 'q') { //leave game
			return move;
		} else {
			return '\0';
		}
	}

	// double mode
	if (mode == 3) {
		move = getch();
		if (move == 'w' || move == 'a' || move == 's' || move == 'd' || move == 'q') {
			return move;
		} else if (move == -32) {
			move = getch();
			switch (move) {
                case 72: return 'i'; 
                case 80: return 'k'; 
                case 75: return 'j'; 
                case 77: return 'l'; 
            } 
		} else {
			return '\0';
		}
	}
	return '\0';
}

// Function to initialize or reload the state of a double-player game
void startdouble(int *floor, int *x, int *y, int *a, int *b, int *p1s, int *p2s) {
    char choice; // Variable to store the user's decision
    // Attempt to load a saved game state
    if (loaddouble(floor, x, y, a, b, p1s, p2s)) {
    	// If a saved game exists, prompt the user to reload it
        printf("Reload previous game? (y/n): ");
        scanf(" %c", &choice); // Read user's choice (yes or no)
         // Validate the user's input
        while (choice != 'y' && choice != 'n') {
            printf("Invalid option!! Input again: \n ");
            scanf(" %c", &choice);
        }
        // If the user chooses not to reload, reset the game state
        if (choice == 'n') {
            *x = 1; //reset p1 x position
            *y = 1; //reset p1 y position
            *a = 0; //reset p2 x position
            *b = 1; //reset p2 y position
            *p1s = 0; //reset p1 score
            *p2s = 0; //reset p2 score
        }
    } else {
        *floor = 1; //reset maze floor
        *x = 1;
        *y = 1;
        *a = 0;
        *b = 1;
        *p1s = 0;
        *p2s = 0;
    }
}

//choose double mode
void changemode() {
	int option = 0;
	system("cls"); //clean screen
    printf("************************\n");
    printf("****  Double mode   ****\n");
    printf("****   1.bettle     ****\n"); //who get to final point faster
    printf("****   2.teamwork   ****\n"); //teamwork together to finish the challange
    printf("****   3.Exit       ****\n"); //back to main menu
    printf("************************\n");
    option = enter(1, 3);
    
	//bettle mode(logic part as same as classic mode)
    if (option == 1) {
        doublegame();
        return;
	}
	
	//teamwork mode(ice fire man game)		
	if (option == 2) {
		teamwork();//go to main program of the game
		return;
	}
	
	//exit		
	if (option == 3) {
		return;
		}
	return;
}
 
void startGame(int *floor, int *x, int *y, int *inputmode) {
    char choice;
    // Attempt to load progress from a previous game
    if (loadProgress(floor, x, y, inputmode)) {
        // Ask player ireload the previous game or not
        printf("Reload previous game? (y/n): ");
        scanf(" %c", &choice);

        // Ensure input is valid
        while (choice != 'y' && choice != 'n') {
            printf("Invalid option!! Input again: \n ");
            scanf(" %c", &choice);
        }

        // chooses not to reload the previous game
        if (choice == 'n') {
            system("cls"); // Clear the screen
            printf("Which floor do you want to start?: ");
            *floor = enter(1, 5); // Let the player choose which floor to start
            *x = 1; // Reset Player1's x-coordinate
            *y = 1; // Reset Player1's y-coordinate

            // select input mode
            int option = 0;
            system("cls"); // Clear the screen
            printf("************************\n");
            printf("*****  Set input   *****\n");
            printf("****   1. WASD      ****\n");
            printf("****   2. Arrow     ****\n");
            printf("************************\n");
            option = enter(1, 2); //choose 1 or 2

            if (option == 1) {
                *inputmode = 1; // Set input mode to WASD
            }
            if (option == 2) {
                *inputmode = 2; // Set input mode to Arrow keys
            }
        }
    } else { // If no saved progress is found
        system("cls"); 
        printf("Which floor do you want to start?: ");
        *floor = enter(1, 5); 
        *x = 1; 
        *y = 1; 

        // Input mode selection
        int option = 0;
        system("cls"); // Clear the screen
        printf("************************\n");
        printf("*****  Set input   *****\n");
        printf("****   1. WASD      ****\n");
        printf("****   2. Arrow     ****\n");
        printf("************************\n");
        option = enter(1, 2);

        if (option == 1) {
            *inputmode = 1;
        }
        if (option == 2) {
            *inputmode = 2;
        }
    }
}

void game(int inputmode) {
    int floor, x, y; // current floor and player position
    startGame(&floor, &x, &y, &inputmode); // Initialize the game (load or start new)

    char move; // player's input
    printMaze(floor); // print maze
    printf("This is %d floor\n", floor); // player of the current floor
    gotoxy(x, y); // go to player position
    printf("♀"); // Display player character

    while (1) { 
        if (_kbhit()) { // if key has been pressed
            move = shuru(inputmode); // get user input
            switch (move) { // Handle movement and actions
                case 'w': // up
                    if (isSpace(x, y - 1)) { // Check if walkable
                        gotoxy(x, y);
                        printf(" "); // Clear current position
                        y -= 1; // Move on
                        gotoxy(x, y);
                        printf("♀"); // Update position
                    }
                    break;
                case 's': // down
                    if (isSpace(x, y + 1)) { 
                        gotoxy(x, y);
                        printf(" ");
                        y += 1; // Move down
                        gotoxy(x, y);
                        printf("♀");
                    }
                    break;
                case 'a': // left
                    if (isSpace(x - 1, y)) { 
                        gotoxy(x, y);
                        printf(" ");
                        x -= 1; // Move left
                        gotoxy(x, y);
                        printf("♀");
                    }
                    break;
                case 'd': // right
                    if (isSpace(x + 1, y)) { 
                        gotoxy(x, y);
                        printf(" ");
                        x += 1; // Move right
                        gotoxy(x, y);
                        printf("♀");
                    }
                    break;
                case 'q': // Quit game
                    system("cls"); // Clear screen
                    printf("************WARNING*************\n");
                    printf("*****Are you sure to leave?****\n");
                    printf("**********( Yes / No )*********\n");
                    printf("*******************************\n");
                    printf("Input: ");
                    scanf(" %c", &move);
                    // Ensure valid input
                    while (move != 'y' && move != 'n') {
                        printf("Invalid option!! Input again: \n");
                        scanf(" %c", &move);
                        while (getchar() != '\n'); // Clear input buffer
                    }
                    if (move == 'y') { // go to save file fun.
                        saveProgress(floor, x, y, inputmode);
                        return; // Exit loop
                    } else if (move == 'n') { // Continue the game
                        printMaze(floor);
                        printf("This is %d floor\n", floor);
                        gotoxy(x, y);
                        printf("♀");
                    }
                    break;
                default:
                    break;
            }
        }

        // Check if the player has reached the goal
        if (x > 41 && y > 18) {
            if (floor == 5) { // If it's the last floor
                system("cls");
                printf("**********CONGRATULATIONS***********\n");
                printf("*****You have completed all floors!****\n");
                printf("********Your are a gold brain*********\n");
                printf("*********THANKS FOR PLAYING***********\n");
                printf("**************************************\n");
                floor = 1; // Reset game progress
                x = 1;
                y = 1;
                remove("save.txt"); // Remove saved progress
                Sleep(2000); // show info. 2s
                break; 
            } else { // isn't last floor
                system("cls");
                printf("*************GOOD JOB**************\n");
                printf("*****You have completed floor %d*****\n", floor);
                printf("******Let's go to the next floor!****\n");
                printf("************(go / exit)************\n");
                printf("***********************************\n");
                move = getch(); // Wait for user input
                while (move != 'g' && move != 'e') { // Ensure valid input
                    move = getch();
                }
                if (move == 'g') { // Move to the next floor
                    floor += 1;
                    printMaze(floor);
                    printf("This is %d floor\n", floor);
                    x = 1;
                    y = 1; // Reset player position
                    gotoxy(x, y);
                    printf("♀");
                } else if (move == 'e') { // Exit and save progress
                    floor += 1;
                    x = 1;
                    y = 1;
                    saveProgress(floor, x, y, inputmode);
                    break; // Exit the game loop
                }
            }
        }
    }
}

void doublegame() {
    char move;
    int floor, x, y, a, b, p1s, p2s;                 // (x,y) p1 position (a,b) p2 position (p1s, p2s) player score
    int inputmode = 3;                               // change inputmode to double mode
    startdouble(&floor, &x, &y, &a, &b, &p1s, &p2s); // starting menu

    printMaze(floor);                                                              // printmaze
    printf("This is %d floor    P1 score: %d    P2 score: %d\n", floor, p1s, p2s); // print game info.
    gotoxy(x, y);                                                                  // go to player1 position
    printf("A");                                                                   // print p1 character
    gotoxy(a, b);                                                                  // go to player2 position
    printf("B");                                                                   // print p2 character

    while (1){
        if (_kbhit()) { // if have input
            move = shuru(3); // enble inputmode
            switch (move)
            {
            case 'w': // p1 up
                if (isSpace(x, y - 1) || isplayer2(x, y - 1)) { // isn't can move
                    gotoxy(x, y);
                    printf(" ");
                    y -= 1;
                    gotoxy(x, y);
                    printf("A");
                    gotoxy(a, b);
                    printf("B");
                }
                break;
            case 's': // p1 down
                if (isSpace(x, y + 1) || isplayer2(x, y + 1)) {
                    gotoxy(x, y);
                    printf(" ");
                    y += 1;
                    gotoxy(x, y);
                    printf("A");
                    gotoxy(a, b);
                    printf("B");
                }
                break;
            case 'a': // p1 left
                if (isSpace(x - 1, y) || isplayer2(x - 1, y)) {
                    gotoxy(x, y);
                    printf(" ");
                    x -= 1;
                    gotoxy(x, y);
                    printf("A");
                    gotoxy(a, b);
                    printf("B");
                }
                break;
            case 'd': // p1 right
                if (isSpace(x + 1, y) || isplayer2(x + 1, y)) {
                    gotoxy(x, y);
                    printf(" ");
                    x += 1;
                    gotoxy(x, y);
                    printf("A");
                    gotoxy(a, b);
                    printf("B");
                }
                break;
            case 'i': // p2 up
                if (isSpace(a, b - 1) || isplayer1(a, b - 1)) {
                    gotoxy(a, b);
                    printf(" ");
                    b -= 1;
                    gotoxy(a, b);
                    printf("B");
                    gotoxy(x, y);
                    printf("A");
                }
                break;
            case 'k': // p2 down
                if (isSpace(a, b + 1) || isplayer1(a, b + 1)) {
                    gotoxy(a, b);
                    printf(" ");
                    b += 1;
                    gotoxy(a, b);
                    printf("B");
                    gotoxy(x, y);
                    printf("A");
                }
                break;
            case 'j': // p2 left
                if (isSpace(a - 1, b) || isplayer1(a - 1, b)) {
                    gotoxy(a, b);
                    printf(" ");
                    a -= 1;
                    gotoxy(a, b);
                    printf("B");
                    gotoxy(x, y);
                    printf("A");
                }
                break;
            case 'l': // p2 right
                if (isSpace(a + 1, b) || isplayer1(a + 1, b)) {
                    gotoxy(a, b);
                    printf(" ");
                    a += 1;
                    gotoxy(a, b);
                    printf("B");
                    gotoxy(x, y);
                    printf("A");
                }
                break;
            case 'q':          // leave game
                system("cls"); // clean screen
                printf("************WARING*************\n");
                printf("*****Are you sure to leave?****\n");
                printf("**********( Yes / No )*********\n");
                printf("*******************************\n");
                printf("Input: ");
                scanf(" %c", &move);
                // Validate the user's input
                while (move != 'y' && move != 'n') {
                    printf("Invalid option!! Input again: \n");
                    scanf(" %c", &move);
                    while (getchar() != '\n')
                        ;
                }
                if (move == 'y') { // go to save file fun.
                    savedouble(floor, x, y, a, b, p1s, p2s);
                    return;
                }
                else if (move == 'n') { // back to game
                    printMaze(floor);
                    printf("This is %d floor    P1 score: %d    P2 score: %d\n", floor, p1s, p2s);
                    gotoxy(x, y);
                    printf("A");
                    gotoxy(a, b);
                    printf("B");
                }
                break;
            default:
                break;
            }
        }

        if (x == a && y == b) { // if p1 and p2 overlapping
            gotoxy(x, y);
            printf("@"); // show @
        }

        // when player1 arrive at the finish point
        if (x > 41 && y > 18) {
            // if finish all floor
            if (floor == 5) {
                p1s += 1;      // p1 score +1
                system("cls"); // clean screen
                if (p1s > p2s) { // if p1 win
                    printf("**********Player1  Win!!***********\n");
                    printf("*****You already pass all floor****\n");
                    printf("********Your are gold brain********\n");
                    printf("*********THANKS FOR PLAYING********\n");
                    printf("***********************************\n");
                }
                else { // if p2 win
                    printf("**********Player2  Win!!***********\n");
                    printf("*****You already pass all floor****\n");
                    printf("********Your are gold brain********\n");
                    printf("*********THANKS FOR PLAYING********\n");
                    printf("***********************************\n");
                }
                Sleep(2000); // show info. 2(s)
                break;
            }
            else {  // if didn't finish all floor
                p1s += 1; // p1 score +1
                system("cls");
                printf("***********Player1 WIN!!***********\n");
                printf("******Let's go to next floor!******\n");
                printf("************(go / exit)************\n");
                printf("***********************************\n");
                move = getch(); // get user input
                                // Ensure input is either 'g' (go) or 'e' (exit)
                while (move != 'g' && move != 'e') {
                    move = getch();
                }
                if (move == 'g') {   // go to next floor and initialize player position
                    floor += 1; // Increment floor count
                    printMaze(floor); // Display the maze for the new floor
                    printf("This is %d floor    P1 score: %d    P2 score: %d\n", floor, p1s, p2s);
                    x = 1;
                    y = 1;
                    a = 0;
                    b = 1;
                    gotoxy(x, y);
                    printf("A");
                    gotoxy(a, b);
                    printf("B");
                }
                else if (move == 'e') { // If 'e' (exit) is chosen
                    floor += 1;
                    x = 1;
                    y = 1;
                    a = 0;
                    b = 1;
                    savedouble(floor, x, y, a, b, p1s, p2s); // go to save file fun.
                    break;
                }
            }
        }

        // when player2 arrive end(as same as player1 part)
        if (a > 41 && b > 18) {
            if (floor == 5) {
                p2s += 1;
                system("cls");
                if (p1s > p2s) {
                    printf("**********Player1  Win!!***********\n");
                    printf("*****You already pass all floor****\n");
                    printf("********Your are gold brain********\n");
                    printf("*********THANKS FOR PLAYING********\n");
                    printf("***********************************\n");
                }
                else {
                    printf("**********Player2  Win!!***********\n");
                    printf("*****You already pass all floor****\n");
                    printf("********Your are gold brain********\n");
                    printf("*********THANKS FOR PLAYING********\n");
                    printf("***********************************\n");
                }
                Sleep(2000);
                break;
            }
            else {
                p2s += 1;
                system("cls");
                printf("***********Player2 WIN!!***********\n");
                printf("******Let's go to next floor!******\n");
                printf("************(go / exit)************\n");
                printf("***********************************\n");
                move = getch();
                while (move != 'g' && move != 'e') {
                    move = getch();
                }
                if (move == 'g') {
                    floor += 1;
                    printMaze(floor);
                    printf("This is %d floor    P1 score: %d    P2 score: %d\n", floor, p1s, p2s);
                    x = 1;
                    y = 1;
                    a = 0;
                    b = 1;
                    gotoxy(x, y);
                    printf("A");
                    gotoxy(a, b);
                    printf("B");
                }
                else if (move == 'e') {
                    floor += 1;
                    x = 1;
                    y = 1;
                    a = 0;
                    b = 1;
                    savedouble(floor, x, y, a, b, p1s, p2s);
                    break;
                }
            }
        }
    }
}

//**until to teamwork function is another program
//**we just put into the same file,didn't integrate the two together
// teamwork map
char map[HEIGHT][WIDTH + 1] = {
    "###########################################",
    "#      #       #R                         #",
    "# ###### ##### ########################## #",
    "# # P  # #   #      #                T    #",
    "# # ## # # # ###### ####################  #",
    "# # ##G# # # #                           ##",
    "# #        # ####### ######################",
    "# ####### #                              T#",
    "#         #############################  ##",
    "##############     ~~~~~~~~~~            ##",
    "#                 #########################",
    "######## ########################      Q  #",
    "#      #                  ########### ### #",
    "# ###### ################ ###########T### #",
    "#      # #              #                 #",
    "# ####################### T################",
    "#            D             #       #      #",
    "### ########################## ###### ### #",
    "#                                     T #E#",
    "###########################################"
};

// player struct
typedef struct {
    int x, y;       // current position
    char symbol;    // player symbol
    int life;       // life points
} Player;

int doorOpen = 0;      
int bridgeVisible = 0; 

// setcursorposition
void setCursorPosition(int x, int y) {
    COORD coord = {x, y};
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

// check player isn't on special tile
int checkOnTile(Player player, char tile) {
    return map[player.y][player.x] == tile;
}

// draw screen
void drawMap(Player ice, Player fire) {
    setCursorPosition(0, 0); // corsor position back to (0, 0)
    printf("ICE man (I): LIFE %d | FIRE man (F): LIFE %d\n", ice.life, fire.life);
    //show game info.
	
	//array scan
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            if (i == ice.y && j == ice.x)
                putchar(ice.symbol);
            else if (i == fire.y && j == fire.x)
                putchar(fire.symbol);
            else if (bridgeVisible && map[i][j] == '~')//if have open bridge signal
                putchar('B');//open bridge
            else if (doorOpen && map[i][j] == 'D')//if have open door signal
                putchar(' ');//open door
            else
                putchar(map[i][j]);
        }
        putchar('\n');
    }
}

// cheack checkpoint status
void updateMechanisms(Player ice, Player fire) {
    // check if door should be opened
    if (checkOnTile(ice, 'P') && checkOnTile(fire, 'Q')) {
        doorOpen = 1; //give open door signal
    }

    // check if bridge should be opened
    if (checkOnTile(ice, 'R') && checkOnTile(fire, 'G')) {
        bridgeVisible = 1; //give open bridge signal
    }
}

// player move
void movePlayer(Player *player, int dx, int dy) {
    int newX = player->x + dx;
    int newY = player->y + dy;

    if (newX >= 0 && newX < WIDTH && newY >= 0 && newY < HEIGHT) {
        char tile = map[newY][newX];
        
        // if the bridge is already open, treat ~ as B
        if (bridgeVisible && tile == '~') {
            tile = 'B';
        }
        
        // if the door is already open, treat ~ as B
        if (doorOpen && tile == 'D') {
            tile = ' ';
        }

        // Check if passage is possible
        if (tile != '#' && tile != 'D' && tile != '~') {
            player->x = newX;
            player->y = newY;
            
            //If a trap is triggered
        	if (tile == 'T') {
        		printf("%c Haha!! You lose your live\n", player->symbol);
        		player->x = 1;
             	player->y = (player->symbol == 'I') ? 1 : 2;//Return to the starting point
             	player->life--;//life point -1
             	if (player->life <= 0) {//when life = 0,game over
                 	printf("%c You died!! Game over!!\n", player->symbol);
                 	Sleep(2000);
                 	exit(0);
             	}
		 	}
        }
    }
}

// main program
void teamwork() {
    Player ice = {1, 1, 'I', INITIAL_LIFE};  // iceman initial location
    Player fire = {1, 2, 'F', INITIAL_LIFE}; // fireman initial location

    system("cls");
    hidecursor();
    char move;

    while (1) {
        drawMap(ice, fire);

        // update checkpoint status
        updateMechanisms(ice, fire);

        // check isn't goto exit
        if (checkOnTile(ice, 'E') && checkOnTile(fire, 'E')) {
            printf("Congralation!! Challange successful!! \n");
            Sleep(2000);
            break;
        }

        if (_kbhit()) { // check isn't input
            char ch = _getch();
            switch (ch) {
                // Iceman move (WASD)
                case 'w': movePlayer(&ice, 0, -1); break;
                case 's': movePlayer(&ice, 0, 1); break;
                case 'a': movePlayer(&ice, -1, 0); break;
                case 'd': movePlayer(&ice, 1, 0); break;
                // Fireman move (ARROW)
                case 72: movePlayer(&fire, 0, -1); break; // up
                case 80: movePlayer(&fire, 0, 1); break;  // down
                case 75: movePlayer(&fire, -1, 0); break; // left
                case 77: movePlayer(&fire, 1, 0); break;  // right
                // back to main menu
                case 'q':
                	system("cls");
        			printf("************WARING*************\n");
        			printf("*****Are you sure to leave?****\n");
        			printf("****Won't auto save progress****\n");
        			printf("**********( Yes / No )*********\n");
        			printf("*******************************\n");
        			scanf(" %c", &move);
        			while (move != 'y' && move != 'n') {
            			printf("Invalid option!! Input again: \n");
            			scanf(" %c", &move);
            			while (getchar() != '\n');
        			}
        			if (move == 'y') {
        				return;
					} else if (move == 'n') {
						drawMap(ice, fire);
					}
				break;	
            }
        }

    }
}

int main() {
    int mode = 0;   // current menu selection
    hidecursor();   // hide the cursor

    // Main menu
    while (mode == 0) {
        system("cls"); // Clear screen
        printf("************************\n");
        printf("*******   Maze   *******\n");
        printf("****   1. Classic   ****\n"); //Play the classic mode
        printf("****   2. Double    ****\n"); //Play the double-player mode
        printf("****   3. Exit      ****\n"); //Exit the game
        printf("************************\n");
        mode = enter(1, 3); // select an option (1-3)

        if (mode == 1) { // If selected classic mode
            game(inputmode); // Start the single-player classic mode game
            mode = 0;        // Reset `mode` to 0 to return to the main menu after the game ends
        }
        if (mode == 2) { // If selected double mode
            changemode(); // Function to start the double-player mode
            mode = 0;
        }
        if (mode == 3) { // If selected exit
            break; // Exit the loop
        }
    }
    return 0; // Exit the program
}
