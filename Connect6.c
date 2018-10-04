// Raymond Wang - University of Toronto
// APS105
// Lab 7, part 2. Create best AI you can
// Nov. 2013

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

// Initialize Variables

char** board;
char AI, Human;
int* move;
int* move2;
int* move3;
int n, midpoint;
bool depthwarning, offense;
clock_t start, check;
float T;
typedef struct {
	int x;
	int y;
	} spot;

// Helper Functions

int max(int a, int b)
	{
	if(a < b) return b;
	return a;
	}

int min(int a, int b)
	{
	if(b > a) return a;
	return b;
	}

int manh_d (const void* p1, const void* p2)
	{
	int x1 = ((spot*)p1)->x;
	int y1 = ((spot*)p1)->y;
	int x2 = ((spot*)p2)->x;
	int y2 = ((spot*)p2)->y;
	int score1 = abs(x1 - midpoint) + abs(y1 - midpoint);
	int score2 = abs(x2 - midpoint) + abs(y2 - midpoint);
	
	if (score1 < score2)
		return -1;
	else if (score1 > score2)
		return 1;
	else 
		return 0;
	}

void printBoard()
	{
	//Prints the board.
	int row;	
	for (row = 0; row < n; row++) printf("%s\n", board[row]);
	return;}

bool spotCheck(int row, int col)
	{
	//Checks the entered move. If the spot is taken, returns a false bool
	bool check;
	if (board[row][col] != 'U') check = false;
	else check = true;
	return check;}

void Addmove(char colour, int row, int col)
	{
	//Adds an entered move into the board
	board[row][col] = colour;
	return;}

void Clearmove(int row, int col)
	{
	//Clears a move
	board[row][col] = 'U';
	return;}

int horizCount(int row, int col, char c)
	{
	//Count the sequence of horizontals
	int Hcount = 0;
	int i;

	//Scan right
	for (i=col; i < n; i++) {
		if (board[row][i] == c) Hcount++;
		else break;}

	//Scan left
	for (i=col; i >= 0; i--) {
		if (board[row][i] == c) Hcount++;
		else break;}
	Hcount--;
	return Hcount;}

int Scanleft(int row, int col, char c)
	{
	int Hcount = 0;
	int i;

	//Scan left
	for (i=col-1; i >= 0; i--) {
		if (board[row][i] == c) Hcount++;
		else break;}
	return Hcount;}

int Scanright(int row, int col, char c)
	{
	int Hcount = 0;
	int i;

	//Scan right
	for (i=col+1; i < n; i++) {
		if (board[row][i] == c)	Hcount++;
		else break;}
	return Hcount;}

int vertCount(int row, int col, char c)
	{
	//Count the sequence of verticals
	int Vcount = 0;
	int i;

	//Scan up
	for (i = row; i >= 0; i--) {
		if (board[i][col] == c) Vcount++;
		else break;}

	//Scan down
	for (i = row; i < n; i++) {
		if (board[i][col] == c) Vcount++;
		else break;}
	Vcount--;
	return Vcount;}

int Scanup(int row, int col, char c)
	{
	int Vcount = 0;
	int i;

		//Scan up
	for (i = row-1; i >= 0; i--) {
		if (board[i][col] == c) Vcount++;
		else break;}
	return Vcount;}

int Scandown(int row, int col, char c)
	{
	//Scan down
	int Vcount = 0;
	int i;
	for (i = row+1; i < n; i++) {
		if (board[i][col] == c) Vcount++;
		else break;}
	return Vcount;}

int diag1Count(int row, int col, char c)
	{
	//Count the sequence of diagonals (Top left to bottom right)
	int D1count = 0;
	int i, j;

	//Scan up left
	for (i = row, j = col; (i >= 0)&&(j >= 0); i--, j--) {
		if (board[i][j] == c) D1count++;
		else break;}

	//Scan down right
	for (i = row, j = col; (i < n)&&(j < n); i++, j++) {
		if (board[i][j] == c) D1count++;
		else break;}
	D1count--;
	return D1count;}

int diag2Count(int row, int col, char c)
	{
	//Count the sequence of right diagonals (Top right to bottom left)
	int D2count = 0;
	int i, j;

	//Scan up right
	for (i = row, j = col; (i >= 0)&&(j < n); i--, j++) {
		if (board[i][j] == c) D2count++;
		else break;}

	//Scan down left
	for (i = row, j = col; (i < n)&&(j >= 0); i++, j--) {
		if (board[i][j] == c) D2count++;
		else break;}
	D2count--;
	return D2count;}

bool winTest( char c)
	{
	//Keep counters on each direction, define win condition
	int i, j, Count1, Count2, Count3, Count4;
	bool win = false;

	//Run through every element and check win condition
	for (i = 0; i < n && !win ; i++) {
		for (j = 0; j < n && !win; j++) {
			//[0] is horiz, [1] is vert, [2] is Tleft to Bright diag, [3] is other diag.
			if (board[i][j] == c) {
				Count1 = horizCount(i, j, c);
				if (Count1 > 5) win = true;	

				Count2 = vertCount(i, j, c);
				if (Count2 > 5) win = true;

				Count3 = diag1Count(i, j, c);
				if (Count3 > 5) win = true;

				Count4 = diag2Count(i, j, c);
				if (Count4 > 5) win = true;
				}
			}
		}
	//Return win condition (true or false)
	return win;
	}

int findLongest(int row, int col)
	{
	//Finds longest string of a colour a spot
	char c = board[row][col];
	int count[4] = {0, 0, 0, 0};

	//Also define a variable to hold the largest string
	int largest;
	count[0] += horizCount(row, col, c);
	count[1] += vertCount(row, col, c);

	if (count[0] >= count[1]) largest = count[0];
	else largest = count[1];
	count[2] += diag1Count(row, col, c);

	if (count[2] > largest) largest = count[2];
	count[3] += diag2Count(row, col, c);

	if (count[3] > largest) largest = count[3];
	return largest;}


	// ---------------- Game Strat Functions ------------


void AIFirstmove()
	// Hard code most optimal and central first move
	{
	if (AI == 'B'){
		move[0] = n/2;
		move[1] = n/2;}
	else {
		int i, j;
		for (i = 0; i < n; i++){
			for (j = 0; j < n; j++) {
				if (board[i][j] == 'B'){
					int L = Scanleft(i, j, 'U');
					int R = Scanright(i, j, 'U');
					int U = Scanup(i, j, 'U');
					int D = Scandown(i, j, 'U');
					if (L > R){
						if (U > D){
							move[0] = i-1;
							move[1] = j-1;}
						else if (D > U){
							move[0] = i+1;
							move[1] = j-1;}
						else{
							move[0] = i;
							move[1] = j-1;}}
					else if (R > L){
						if (U > D){
							move[0] = i-1;
							move[1] = j+1;}
						else if (D > U){
							move[0] = i+1;
							move[1] = j+1;}
						else{
							move[0] = i;
							move[1] = j+1;}}
					else{
						if (U >= D){
							move[0] = i-1;
							move[1] = j;}
						else{
							move[0] = i+1;
							move[1] = j;}}
					}
				}
			}
		}
	return;}

bool Instantwin(char c)
	// Look for wins in one move
	{
	int row, col, threat;
	for (row = 0; row < n; row++){
		for (col = 0; col < n; col++){
			if (board[row][col] == 'U'){
				Addmove(c, row, col);
				int length = findLongest(row, col);
				if (length >= 6) {
					Clearmove(row, col);
					move[0] = row;
					move[1] = col;
					return true;}
				Clearmove(row, col);
				}
			}
		}
	return false;}

int Score(int row, int col, char c)
	// Score the square provided
	{
	int Hscore = 0, Vscore = 0, D1score = 0, D2score = 0, Hbound = 0, Vbound = 0, D1bound = 0, D2bound = 0;
	char d;
	if (c == AI) d = Human;
	else d = AI;
	int i, j, largest;
	bool space = false;
	//Horizontal, Scan left
	for (i = col; i >= 0; i--)
		{
		if ((space == true)&&(board[row][i] == 'U')) break;
		if (board[row][i] == 'U') space = true;
		else if (board[row][i] == c) Hscore++;
		if ((i == 0)||(board[row][i] == d)){
			Hbound++;
			Hscore--;
			break;}
		}
	//Scan right
	for (i = col; i < n; i++)
		{
		if ((space == true)&&(board[row][i] == 'U')) break;
		if (board[row][i] == 'U') space = true;
		else if (board[row][i] == c) Hscore++;
		if ((i == n-1)||(board[row][i] == d)){
			Hbound++;
			Hscore--;
			break;}
		}
	Hscore--;

	if (Hbound == 2) Hscore = 0;
	largest = Hscore;

	//Vertical
	space = false;

	//Scan up
	for (i = row; i >= 0; i--)
		{
		if ((space == true)&&(board[i][col] == 'U')) break;
		if (board[i][col] == 'U') space = true;
		else if (board[i][col] == c) Vscore++;
		if ((i == 0)||(board[i][col] == d)) {
			Vbound++;
			Vscore--;
			break;}
		}
	//Scan down
	for (i = row; i < n; i++)
		{
		if ((space == true)&&(board[i][col] == 'U'))break;
		if (board[i][col] == 'U') space = true;
		else if (board[i][col] == c) Vscore++;
		if ((i == n-1)||(board[i][col] == d))  {
			Vbound++;
			Vscore--;
			break;}
		}
	Vscore--;

	if (Vbound == 2) Vscore = 0;
	if (Vscore > largest) largest = Vscore;
	//Diag1

	space = false;
	//Scan TL
	for (i = row, j = col; (i >= 0)&&(j >= 0); i--, j--)
		{
		if ((space == true)&&(board[i][j] == 'U')) break;
		if (board[i][j] == 'U') space = true;
		else if (board[i][j] == c) D1score++;
		if ((i == 0)||(j == 0)||(board[i][j] == d)) {
			D1bound++;
			D1score--;
			break;}
		}
	//Scan BR
	for (i = row, j = col; (i < n)&&(j < n); i++, j++)
		{
		if ((space == true)&&(board[i][j] == 'U')) break;
		if (board[i][j] == 'U') space = true;
		else if (board[i][j] == c) D1score++;
		if ((i == n-1)||(j == n-1)||(board[i][j] == d))  {
			D1bound++;
			D1score--;
			break;}
		}
	D1score--;

	if (D1bound == 2) D1score = 0;
	if (D1score > largest) largest = D1score;
	//Diag2

	space = false;
	//Scan TR
	for (i = row, j = col; (i >= 0)&&(j < n); i--, j++)
		{
		if ((space == true)&&(board[i][j] == 'U')) break;
		if (board[i][j] == 'U') space = true;
		else if (board[i][j] == c) D2score++;
		if ((i == 0)||(j == n-1)||(board[i][j] == d)) {
			D2bound++;
			D2score--;
			break;}
		}
	//Scan BL
	for (i = row, j = col; (i < n) && (col >= 0); i++, j--)
		{
		if ((space == true)&&(board[i][j] == 'U')) break;
		if (board[i][j] == 'U') space = true;
		else if (board[i][j] == c) D2score++;
		if ((i == n-1)||(j == 0)||(board[i][j] == d)) {
			D2bound++;
			D2score--;
			break;}
		}
	D2score--;
	if (D2bound == 2) D2score = 0;

	if (D2score > largest) largest = D2score;
	return largest;
	}

int ForcedAtk(bool who, int layer);

int ForcedDef(bool who, int x, int y, int basescore, int layer)
	{
	check = clock();
	/*T = (((float)check - (float)start)/CLOCKS_PER_SEC);
	if ((T >= 0.5)&&(offense == false))
		return 1;
	else if ((T >= 0.3)&&(offense == true))
		return 1;*/
	//Returns 0 for will be beaten, 1 for defendable
	//Who is true or false indicating defender. True = AI, false = Human
	int row, col, threat, c, d;
	if (who == true){
		c = AI;
		d = Human;}
	else {
		c = Human;
		d = AI;}
	for (row = max(0, x-5); row < min(n, x+6); row++){
		for (col = max(0, y-5); col < min(n, y+6); col++) {
			if (board[row][col] == 'U') {
				Addmove(c, row, col);
				int score = Score(x, y, d);
				if (score < basescore) {
					threat = ForcedAtk(!who, layer+1);
					if (threat == 1){
						Clearmove(row, col);
						if(layer == 0)
						{
							move[0] = row;
							move[1] = col;
						}
						return 1;}
					}
				Clearmove(row, col);
				}
			}
		}

	return 0;
	}


int ForcedAtk(bool who, int layer)
	{
	check = clock();
	/*T = (((float)check - (float)start)/CLOCKS_PER_SEC);
	if ((T >= 0.5)&&(offense == false))
		return 1;
	else if ((T >= 0.3)&&(offense == true))
		return 1;
	if (layer == 6){
		return 1;}*/
	//Returns 0 for winning move, 1 for not a forced win
	//Who is true or false indicating attacker. True = AI, false = human
	int row, col, threat, c;
	if (who == true)
		c = AI;
	else
		c = Human;
	for (row = 0; row < n; row++){
		for (col = 0; col < n; col++){
			if (board[row][col] == 'U'){
				Addmove(c, row, col);
				int length = findLongest(row, col);
				if (length >=6) {
					Clearmove(row, col);
					return 0;}
				int score = Score(row, col, c);
				if (score >= 4) {
					threat = ForcedDef(!who, row, col, score, layer);
					if (threat == 0) {
						Clearmove(row, col);
						return 0;}
					}
				Clearmove(row, col);
				}
			}
		}
	return 1;
	}

void Offense(spot *order)
	/* General offensive strat if a forcing winning move can't be found in time.
	  Looks to play a move that will create sequences of 3. The idea is if enough 
	  sequence 3s are created, eventually in a future move a forced win might be
	  available
	  */
	{
	int S2count = 0, S1count = 0, i, score, row, col;
	for (i = 0; i < n*n; i++) {
		row = order[i].x;
		col = order[i].y;
		if (board[row][col] == 'U') {
			Addmove(AI, row, col);
			score = Score(row, col, AI);
			if (score == 3) {
				move[0] = row;
				move[1] = col;
				Clearmove(row, col);
				return;}
			if ((score == 2)&&(S2count == 0)) {
				move2[0] = row;
				move2[1] = col;
				S2count++;}
			if ((score <= 1)&&(S1count == 0)) {
				move3[0] = row;
				move3[1] = col;
				S1count++;}
			Clearmove(row, col);
			}
		}
	if (S2count != 0 ) {
		move[0] = move2[0];
		move[1] = move2[1];
		return;}
	else {
		move[0] = move3[0];
		move[1] = move3[1];
		return;}
	return;
	}


// -----------------------------Main Code----------------------------------------

int main(void)
	{
	move = (int*)malloc(2*sizeof(int));
	move[0] = 0;
	move[1] = 0;
	
	move2 = (int*)malloc(2*sizeof(int));
	move2[0] = 0;
	move2[1] = 0;
	
	move3 = (int*)malloc(2*sizeof(int));
	move3[0] = 0;
	move3[1] = 0;
	

	int row, col, Hrow, Hcol, dummy;
	bool win = false, Bwin = false, Wwin = false;
	printf("Enter board dimensions (n): ");
	scanf("%d", &n);
	
	//Setting up the structs and reorganized searching pattern
	midpoint = n/2;
	spot order[n*n];
	int i, j, k = 0;
	for (i = 0; i < n; i++) {
		for (j = 0; j < n; j++) {
			order[k].x = i;
			order[k].y = j;
			k++;
			}
		}
	
	qsort(order, n*n, sizeof(spot), manh_d);
	
	for (i = 0; i < n*n; i++) 
		printf("%d, %d \n", order[i].x, order[i].y);
	

	//Allocating memory to the 2D array
	board = (char**)malloc(n*sizeof(char*)+1);

	for (row = 0; row < n; row++)
		board[row] = (char*)malloc(n*sizeof(char)+1);

	//Setting up the entire board. Each spot is untaken, or 'U'
	for (row = 0; row < n; row++) {
		for (col = 0; col < n; col ++)
			board[row][col] = 'U';}

	printBoard();

	printf("Computer playing B or W? (B moves first): ");
	scanf(" %c", &AI);

	if (AI == 'B') Human = 'W';
	else Human = 'B';

	int moveslimit = n*n;
	int movescounter = 0;

	bool kill = false, death = false;
	int Defend, Force;

	if (AI == 'B') {
		while (movescounter < moveslimit) {
			//Computer's move
			//start = clock();
			if (movescounter == 0){
				AIFirstmove();
				Addmove('B', move[0], move[1]);}
				
			else {
				while (true) {
					//Test for instant win and loss
					depthwarning = false;
					kill = Instantwin(AI);
					if (kill == true){
						Addmove('B', move[0], move[1]);
						break;}
					death = Instantwin(Human);
					if (death == true){
						Addmove('B', move[0], move[1]);
						break;}
					Defend = ForcedAtk(false, 0);
					if (Defend == 0){
						ForcedDef(true, Hrow, Hcol, Score(Hrow, Hcol, 'W'), 0);
						Addmove('B', move[0], move[1]);
						break;}
					Force = ForcedAtk(true, 0);
					if (Force == 0){
						Addmove('B', move[0], move[1]);
						break;}
					else {
						Offense(order);
						Addmove('B', move[0], move[1]);
						break;}
					}
				}
			movescounter++;
			printf("Computer lays a stone at ROW %d COL %d.\n", move[0], move[1]);
			printBoard();

			Bwin = winTest('B');
			if ((Bwin)||(movescounter == moveslimit)) break;

			//Human's move
			printf("Lay down a stone (ROW COL): \n");
			scanf("	%d %d", &Hrow, &Hcol);

			//Check to see if spot is taken
			bool check = spotCheck(Hrow, Hcol);
			while (!check) {
				printf("That square is occupied.\n");
				
				printf("Lay down a stone (ROW COL): \n");
				scanf(" %d %d", &Hrow, &Hcol);
				
				check = spotCheck(Hrow, Hcol);}

			Addmove('W', Hrow, Hcol);
			movescounter++;
			printBoard();

			Wwin = winTest('W');
			if (Wwin) break;
			}
		}
		

	else {
		while (movescounter < moveslimit) {
			//Human's move
			printf("Lay down a stone (ROW COL): \n");
			scanf("	%d %d", &Hrow, &Hcol);

			//Check to see if spot is taken
			bool check = spotCheck(Hrow, Hcol);
			while (!check) {
				printf("That square is occupied.\n");
				
				printf("Lay down a stone (ROW COL): \n");
				scanf(" %d %d", &Hrow, &Hcol);
				
				check = spotCheck(Hrow, Hcol);}

			Addmove('B', Hrow, Hcol);
			movescounter++;
			printBoard();

			Bwin = winTest('B');
			if ((Bwin)||(movescounter == moveslimit)) break;

			//Computer's move
			//start = clock();
			if (movescounter < 2) {
				AIFirstmove();
				Addmove('W', move[0], move[1]);}
			else {
				while (true) {
					//Test for instant win and loss
					depthwarning = false;
					kill = Instantwin(AI);
					if (kill == true){
						Addmove('W', move[0], move[1]);
						break;}
					death = Instantwin(Human);
					if (death == true){
						Addmove('W', move[0], move[1]);
						break;}
					Defend = ForcedAtk(false, 0);
					if (Defend == 0){
						ForcedDef(true, Hrow, Hcol, Score(Hrow, Hcol, 'B'), 0);
						Addmove('W', move[0], move[1]);
						break;}
					Force = ForcedAtk(true, 0);
					if (Force == 0){	
						Addmove('W', move[0], move[1]);
						break;}
					else {
						Offense(order);
						Addmove('W', move[0], move[1]);
						break;}
					}
				}
			movescounter++;
			printf("Computer lays a stone at ROW %d COL %d.\n", move[0], move[1]);
			printBoard();

			Wwin = winTest('W');
			if (Wwin) break;
			}
		}

	if (Bwin) printf("Black player wins.");
	else if (Wwin) printf("White player wins.");
	else printf("Draw!");

int a;
for (a = 0; a < n; a++)
		free(board[a]);

free(move);
free(move2);
free(move3);
free(order);

return 0;
}

