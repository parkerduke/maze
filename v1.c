#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <windows.h>
#include <conio.h>

int inputmode = 1;

void clearScreen() {
    system("cls");
}

void printMaze(int floor) {
    char filename[100]; 
    sprintf(filename, "./map/%dfloor.txt", floor);

    clearScreen();

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

void saveProgress(int floor, int x, int y) {
    FILE *file = fopen("save.txt", "w");
    if (file == NULL) {
        printf("Save faild！\n");
        return;
    }
    fprintf(file, "%d %d %d\n", floor, x, y);
    fclose(file);
    printf("Save successful！\n");
}

bool loadProgress(int *floor, int *x, int *y) {
    FILE *file = fopen("save.txt", "r");
    if (file == NULL) {
        return false;
    }
    if (fscanf(file, "%d %d %d", floor, x, y) != 3) {
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
        return ci.Char.AsciiChar == 'B';  // 使用 Unicode 編碼比較 '♂'
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
        return ci.Char.AsciiChar == 'A';  // 使用 Unicode 編碼比較 '♀'
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

char shuru(int mode) {
	char move;
	
	if (mode == 1) {
		move = getch();
		if (move == 'w' || move == 'a' || move == 's' || move == 'd' || move == 'q') {
			return move;
		} else {
			return '\0';
		}
	}
	
	if (mode == 2) {
		move = getch();
		if (move == -32 || move == 224) {
			move = getch();
			switch (move) {
                case 72: return 'w'; // 上鍵
                case 80: return 's'; // 下鍵
                case 75: return 'a'; // 左鍵
                case 77: return 'd'; // 右鍵
            }
		} else if (move == 'q') {
			return move;
		} else {
			return '\0';
		}
	}

	
	if (mode == 3) {
		move = getch();
		if (move == 'w' || move == 'a' || move == 's' || move == 'd' || move == 'q') {
			return move;
		} else if (move == -32) {
			move = getch();
			switch (move) {
                case 72: return 'i'; // 上鍵
                case 80: return 'k'; // 下鍵
                case 75: return 'j'; // 左鍵
                case 77: return 'l'; // 右鍵
            } 
		} else {
			return '\0';
		}
	}
	
	return '\0';
}

void changeinput(int *inputmode) {
	int option = 0;
	system("cls");
    printf("************************\n");
    printf("*****  Set input   *****\n");
    printf("****   1.WASD       ****\n");
    printf("****   2.Arrow      ****\n");
    printf("****   3.Exit       ****\n");
    printf("************************\n");
    option = enter(1, 3);
        	
    if (option == 1) {
        *inputmode = 1;
        printf("**** Set success ****\n");
        Sleep(1);
	}
			
	if (option == 2) {
		*inputmode = 2;
		printf("**** Set success ****\n");
		Sleep(1);
	}
			
	if (option == 3) {
		return;
	}
	return;
}

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
        	
    if (option == 1) {
        char move;
    	int mode = 0;
    	int floor = 1;
    	int time = 0;
    	int x = 0;
    	int y = 0;
    	int a = 0;
    	int b = 0;
    	int prex = 0;
    	int prey = 0;
    	int prea = 0;
    	int preb = 0;
    	int inputmode = 3;
    	int p1s = 0;
    	int p2s = 0;
	    
    	FILE *pg;
    	
    	printMaze(floor);
		
		x = 1;
		y = 1;
		a = 0;
		b = 1;
		
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
						gotoxy(prex, prey);
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
        					return;
						} else if (move == 'n') {
							return;
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
			
			if (x > 41 && y > 18) {
				if (floor == 4) {
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
        			Sleep(5);
        			break;
				} else {
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
                    if(move == 'g') {
                    	floor += 1;
                        printMaze(floor);
                        x = 1;
                        y = 1;
                        a = 0;
                        b = 1;
                        gotoxy(x, y);
                        printf("A");
                        gotoxy(a, b);
                        printf("B");
                    } else if(move=='e') {
                        break;
                    }
				}
			}
			
			if (a > 41 && b > 18) {
				if (floor == 4) {
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
        			Sleep(5);
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
                        x = 1;
                        y = 1;
                        a = 0;
                        b = 1;
                        gotoxy(x, y);
                        printf("A");
                        gotoxy(a, b);
                        printf("B");
                    } else if(move=='e') {
                        break;
                    }
				}
			}
		}
	}
			
	if (option == 2) {
		return;
	}
			
	if (option == 3) {
		return;
		}
	return;
}

void startGame(int *floor, int *x, int *y) {
    char choice;
    if (loadProgress(floor, x, y)) {
        printf("Reload previous game? (y/n): ");
        scanf(" %c", &choice);
        while (choice != 'y' && choice != 'n') {
            printf("Invalid option!! Input again: \n ");
            scanf(" %c", &choice);
        }
        if (choice == 'n') {
            *floor = 1;
            *x = 1;
            *y = 1;
        }
    } else {
        printf("Didn't found file!!'\n");
        printf("Start new game!!\n");
        *floor = 1;
        *x = 1;
        *y = 1;
    }
}

void game(int inputmode) {
    int floor, x, y;
    startGame(&floor, &x, &y);

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
        					saveProgress(floor, x, y);
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
				if (floor == 4) {
					system("cls");
					printf("**********CONGRAGULATION***********\n");
        			printf("*****You already pass all floor****\n");
        			printf("********Your are gold brain********\n");
        			printf("*********THANKS FOR PLAYING********\n");
        			printf("***********************************\n");
        			floor = 1;
                    x = 1;
                    y = 1;
                    saveProgress(floor, x, y);
        			sleep(5);
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
                    	saveProgress(floor, x, y);
                        break;
                    }
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
        printf("****   2.Set input  ****\n");
        printf("****   3.double     ****\n");
        printf("****   4.Exit       ****\n");
        printf("************************\n");
        mode = enter(1, 4);

        if (mode == 1) {
            game(inputmode);
            mode = 0;
        }
        if (mode == 2) {
			changeinput(&inputmode);
			mode = 0;
		}
		if (mode == 3) {
			changemode();
			mode = 0;
		}
		if (mode == 4) {
            break; 
        }
    }
    return 0;
}