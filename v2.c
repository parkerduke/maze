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

void printMaze(int floor) {
    char filename[100]; 
    sprintf(filename, "./map/%dfloor.txt", floor);

    system("cls");

    FILE *pg = fopen(filename, "r");
    if (pg == NULL) {
        printf("Error opening file: %s\n", filename);
        return;
    }

    char line[256]; 
    while (fgets(line, sizeof(line), pg) != NULL) {
        printf("%s", line);
    }

    fclose(pg);
    printf("\n"); 
}

void saveProgress(int floor, int x, int y, int inputmode) {
    FILE *file = fopen("save.txt", "w");
    if (file == NULL) {
        printf("Save faild!\n");
        return;
    }
    fprintf(file, "%d %d %d %d\n", floor, x, y, inputmode);
    fclose(file);
}

void savedouble(int floor, int x, int y, int a, int b, int p1s, int p2s) {
    FILE *file = fopen("double.txt", "w");
    if (file == NULL) {
        printf("Save faild!\n");
        return;
    }
    fprintf(file, "%d %d %d %d %d %d %d\n", floor, x, y, a, b, p1s, p2s);
    fclose(file);
}

bool loadProgress(int *floor, int *x, int *y, int *inputmode) {
    FILE *file = fopen("save.txt", "r");
    if (file == NULL) {
        return false;
    }
    if (fscanf(file, "%d %d %d %d", floor, x, y, inputmode) != 4) {
        fclose(file);
        return false;
    }
    fclose(file);
    return true;
}

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

bool isSpace(int x, int y) {
    CHAR_INFO ci;
    COORD bufferSize = {1, 1}; 
    COORD bufferCoord = {0, 0}; 
    SMALL_RECT readRegion = {x, y, x, y}; 

    
    if (ReadConsoleOutput(GetStdHandle(STD_OUTPUT_HANDLE), &ci, bufferSize, bufferCoord, &readRegion)) {
        return ci.Char.AsciiChar == ' '; 
    } else {
        return false; 
    }
}

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

void hidecursor()
{
   HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
   CONSOLE_CURSOR_INFO info;
   info.dwSize = 100;
   info.bVisible = FALSE;
   SetConsoleCursorInfo(consoleHandle, &info);
}

void gotoxy(int x, int y) {
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

int wherex()
{
    CONSOLE_SCREEN_BUFFER_INFO csbi;    
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);    
    return csbi.dwCursorPosition.X;
}

int wherey()
{
    CONSOLE_SCREEN_BUFFER_INFO csbi;    
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);    
    return csbi.dwCursorPosition.Y;
}

int enter(int L, int H) {
    int input;
    while (1) {
        printf("\nEnter a number between %d and %d: ", L, H);
        if (scanf("%d", &input)) {
            if (input >= L && input <= H) {
                break;
            } else {
                printf("\nInvalid option!! Inputagain: ");
            }
        } else {
            printf("\nInvalid option!! Inputagain: ");
            while (getchar() != '\n');
        }
    }
    while (getchar() != '\n'); 
    return input;
}

//chang inputmode
char shuru(int mode) {
	char move;
	
	if (mode == 1) {
		move = getch();
		if (move == 'w' || move == 'a' || move == 's' || move == 'd' || move == 'q') {
			return move;
		} else {
			return '\0';
		}
	}//WASD mode
	
	if (mode == 2) {
		move = getch();
		if (move == -32 || move == 224) {
			move = getch();
			switch (move) {
                case 72: return 'w'; // up
                case 80: return 's'; // down
                case 75: return 'a'; // left
                case 77: return 'd'; // right
            }
		} else if (move == 'q') {
			return move;
		} else {
			return '\0';
		}
	}//Arrow mode

	
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
	}//double mode
	
	return '\0';
}

void startdouble(int *floor, int *x, int *y, int *a, int *b, int *p1s, int *p2s) {
    char choice;
    if (loaddouble(floor, x, y, a, b, p1s, p2s)) {
        printf("Reload previous game? (y/n): ");
        scanf(" %c", &choice);
        while (choice != 'y' && choice != 'n') {
            printf("Invalid option!! Input again: \n ");
            scanf(" %c", &choice);
        }
        if (choice == 'n') {
            *x = 1;
            *y = 1;
            *a = 0;
            *b = 1;
            *p1s = 0;
            *p2s = 0;
        }
    } else {
        *floor = 1;
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
	system("cls");
    printf("************************\n");
    printf("****  Double mode   ****\n");
    printf("****   1.bettle     ****\n");
    printf("****   2.teamwork   ****\n");
    printf("****   3.Exit       ****\n");
    printf("************************\n");
    option = enter(1, 3);
    
	//bettle mode(logic part as same as classic mode)    	
    if (option == 1) {
        char move;
    	int floor, x, y, a, b, p1s, p2s;
    	int inputmode = 3;
    	startdouble(&floor, &x, &y, &a, &b, &p1s, &p2s);
    	
    	printMaze(floor);
    	printf("This is %d floor    P1 score: %d    P2 score: %d\n",floor, p1s, p2s);
		
		gotoxy(x, y);
		printf("A");
		gotoxy(a, b);
		printf("B");
		
		while(1) {
			if (_kbhit()) {
				move = shuru(3);
				switch(move) {
					case 'w':
						if (isSpace(x, y-1) || isplayer2(x, y-1)) {
							gotoxy(x, y);
							printf(" ");
							y -= 1;
							gotoxy(x, y);
							printf("A");
							gotoxy(a, b);
							printf("B");
						}
						break;
					case 's':
						if (isSpace(x, y+1) || isplayer2(x, y+1)) {
							gotoxy(x, y);
							printf(" ");
							y += 1;
							gotoxy(x, y);
							printf("A");
							gotoxy(a, b);
							printf("B");
						}
						break;
					case 'a':
						if (isSpace(x-1, y) || isplayer2(x-1, y)) {
							gotoxy(x, y);
							printf(" ");
							x -= 1;
							gotoxy(x, y);
							printf("A");
							gotoxy(a, b);
							printf("B");
						}
						break;
					case 'd':
						if (isSpace(x+1, y) || isplayer2(x+1, y)) {
							gotoxy(x, y);
							printf(" ");
							x += 1;
							gotoxy(x, y);
							printf("A");
							gotoxy(a, b);
							printf("B");
						}
						break;
					case 'i':
						if (isSpace(a, b-1) || isplayer1(a, b-1)) {
							gotoxy(a, b);
							printf(" ");
							b -= 1;
							gotoxy(a, b);
							printf("B");
							gotoxy(x, y);
							printf("A");
						}
						break;
					case 'k':
						if (isSpace(a, b+1) || isplayer1(a, b+1)) {
							gotoxy(a, b);
							printf(" ");
							b += 1;
							gotoxy(a, b);
							printf("B");
							gotoxy(x, y);
							printf("A");
							
						}
						break;
					case 'j':
						if (isSpace(a-1, b) || isplayer1(a-1, b)) {
							gotoxy(a, b);
							printf(" ");
							a -= 1;
							gotoxy(a, b);
							printf("B");
							gotoxy(x, y);
							printf("A");
						}
						break;
					case 'l':
						if (isSpace(a+1, b) || isplayer1(a+1, b)) {
							gotoxy(a, b);
							printf(" ");
							a += 1;
							gotoxy(a, b);
							printf("B");
							gotoxy(x, y);
							printf("A");
						}
						break;
					case 'q':
						system("cls");
        				printf("************WARING*************\n");
        				printf("*****Are you sure to leave?****\n");
        				printf("****Will auto save progress****\n");
        				printf("**********( Yes / No )*********\n");
        				printf("*******************************\n");
        				scanf(" %c", &move);
        				while (move != 'y' && move != 'n') {
            				printf("Invalid option!! Input again: \n");
            				scanf(" %c", &move);
            				while (getchar() != '\n');
        				}
        				if (move == 'y') {
        					savedouble(floor, x, y, a, b, p1s ,p2s);
        					return;
						} else if (move == 'n') {
							printMaze(floor);
							printf("This is %d floor    P1 score: %d    P2 score: %d\n",floor, p1s, p2s);
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
			
			if (x == a && y == b) {
				gotoxy(x, y);
				printf("@");
			}
			
			//when player1 arrive end
			if (x > 41 && y > 18) {
				//if finish all floor
				if (floor == 5) {
					p1s += 1;
					system("cls");
					if (p1s > p2s) {
						printf("**********Player1  Win!!***********\n");
        				printf("*****You already pass all floor****\n");
        				printf("********Your are gold brain********\n");
        				printf("*********THANKS FOR PLAYING********\n");
        				printf("***********************************\n");
					} else {
						printf("**********Player2  Win!!***********\n");
        				printf("*****You already pass all floor****\n");
        				printf("********Your are gold brain********\n");
        				printf("*********THANKS FOR PLAYING********\n");
        				printf("***********************************\n");
					}
        			Sleep(2000);
        			break;
				} else { //if didn't finish all floor
					p1s += 1;
					system("cls");
					printf("***********Player1 WIN!!***********\n");
        			printf("******Let's go to next floor!******\n");
        			printf("************(go / exit)************\n");
        			printf("***********************************\n");
        			move=getch();
                    while(move != 'g' && move != 'e'){
                        move=getch();
                    }
                    if(move == 'g') {//goo to next floor and initialize player position
                    	floor += 1;
                        printMaze(floor);
                        printf("This is %d floor    P1 score: %d    P2 score: %d\n",floor, p1s, p2s);
                        x = 1;
                        y = 1;
                        a = 0;
                        b = 1;
                        gotoxy(x, y);
                        printf("A");
                        gotoxy(a, b);
                        printf("B");
                    } else if(move=='e') {
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
			
			//when player2 arrive end(as same as player1 part)
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
					} else {
						printf("**********Player2  Win!!***********\n");
        				printf("*****You already pass all floor****\n");
        				printf("********Your are gold brain********\n");
        				printf("*********THANKS FOR PLAYING********\n");
        				printf("***********************************\n");
					}
        			Sleep(2000);
        			break;
				} else {
					p2s += 1;
					system("cls");
					printf("***********Player2 WIN!!***********\n");
        			printf("******Let's go to next floor!******\n");
        			printf("************(go / exit)************\n");
        			printf("***********************************\n");
        			move=getch();
                    while(move != 'g' && move != 'e'){
                        move=getch();
                    }
                    if(move == 'g') {
                    	floor += 1;
                        printMaze(floor);
                        printf("This is %d floor    P1 score: %d    P2 score: %d\n",floor, p1s, p2s);
                        x = 1;
                        y = 1;
                        a = 0;
                        b = 1;
                        gotoxy(x, y);
                        printf("A");
                        gotoxy(a, b);
                        printf("B");
                    } else if(move=='e') {
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
    if (loadProgress(floor, x, y, inputmode)) {
        printf("Reload previous game? (y/n): ");
        scanf(" %c", &choice);
        while (choice != 'y' && choice != 'n') {
            printf("Invalid option!! Input again: \n ");
            scanf(" %c", &choice);
        }
        if (choice == 'n') {
        	system("cls");
        	printf("which floor you want to start?: ");
            *floor = enter(1, 5);
            *x = 1;
            *y = 1;
            int option = 0;
			system("cls");
    		printf("************************\n");
    		printf("*****  Set input   *****\n");
    		printf("****   1.WASD       ****\n");
    		printf("****   2.Arrow      ****\n");
    		printf("************************\n");
    		option = enter(1, 2);
        	
    		if (option == 1) {
        		*inputmode = 1;
			}
			
			if (option == 2) {
				*inputmode = 2;
			}
        }
    } else {
    	system("cls");
        printf("which floor you want to start?: ");
        *floor = enter(1, 5);
        *x = 1;
        *y = 1;
        int option = 0;
		system("cls");
    	printf("************************\n");
    	printf("*****  Set input   *****\n");
    	printf("****   1.WASD       ****\n");
    	printf("****   2.Arrow      ****\n");
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
    int floor, x, y;
    startGame(&floor, &x, &y, &inputmode);

    char move;
    printMaze(floor);
    printf("This is %d floor\n",floor);
    gotoxy(x, y);
    printf("♀");
	
		while(1) {
			if (_kbhit()) {
				move = shuru(inputmode);
				switch(move) {
					case 'w':
						if (isSpace(x, y-1)) {
							gotoxy(x, y);
							printf(" ");
							y -= 1;
							gotoxy(x, y);
							printf("♀");
						}
						break;
					case 's':
						if (isSpace(x, y+1)) {
							gotoxy(x, y);
							printf(" ");
							y += 1;
							gotoxy(x, y);
							printf("♀");
						}
						break;
					case 'a':
						if (isSpace(x-1, y)) {
							gotoxy(x, y);
							printf(" ");
							x -= 1;
							gotoxy(x, y);
							printf("♀");
						}
						break;
					case 'd':
						if (isSpace(x+1, y)) {
							gotoxy(x, y);
							printf(" ");
							x += 1;
							gotoxy(x, y);
							printf("♀");
						}
						break;
					case 'q':
						system("cls");
        				printf("************WARING*************\n");
        				printf("*****Are you sure to leave?****\n");
        				printf("****Will auto save progress****\n");
        				printf("**********( Yes / No )*********\n");
        				printf("*******************************\n");
        				scanf(" %c", &move);
        				while (move != 'y' && move != 'n') {
            				printf("Invalid option!! Input again: \n");
            				scanf(" %c", &move);
            				while (getchar() != '\n');
        				}
        				if (move == 'y') {
        					saveProgress(floor, x, y, inputmode);
        					return;
						} else if (move == 'n') {
							printMaze(floor);
							printf("This is %d floor\n",floor);
    						gotoxy(x, y);
    						printf("♀");
						}
						break;			
					default:
						break;
				}
			}
			
			if (x > 41 && y > 18) {
				if (floor == 5) {
					system("cls");
					printf("**********CONGRAGULATION***********\n");
        			printf("*****You already pass all floor****\n");
        			printf("********Your are gold brain********\n");
        			printf("*********THANKS FOR PLAYING********\n");
        			printf("***********************************\n");
        			floor = 1;
                    x = 1;
                    y = 1;
                    remove("save.txt");
        			Sleep(2000);
        			break;
				} else {
					system("cls");
					printf("*************GOOD JOB**************\n");
        			printf("*****You already pass %d floor*****\n",floor);
        			printf("******Let's go to next floor!******\n");
        			printf("************(go / exit)************\n");
        			printf("***********************************\n");
        			move=getch();
                    while(move != 'g' && move != 'e'){
                        move=getch();
                    }
                    if(move == 'g') {
                    	floor += 1;
                        printMaze(floor);
                        printf("This is %d floor\n",floor);
                        x = 1;
                        y = 1;
                        gotoxy(x, y);
                        printf("♀");
                    } else if(move=='e') {
                    	floor += 1;
                    	x = 1;
                        y = 1;
                    	saveProgress(floor, x, y, inputmode);
                        break;
                    }
				}
			}
		}
	}

//**untial to teamwork is another program
//**i just put into the same file,didn't integrate the two together
// map
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
             	if (player->life <= 0) {//whe life = 0,game over
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
    int mode = 0;
    int moshi = 0;
    hidecursor();
    
    
    while (mode == 0) {
        system("cls");
        printf("************************\n");
        printf("*******   Maze   *******\n");
        printf("****   1.classic    ****\n");
        printf("****   2.double     ****\n");
        printf("****   3.Exit       ****\n");
        printf("************************\n");
        mode = enter(1, 4);

        if (mode == 1) {
            game(inputmode);
            mode = 0;
        }
		if (mode == 2) {
			changemode();
			mode = 0;
		}
		if (mode == 3) {
            break; 
        }
    }
    return 0;
}