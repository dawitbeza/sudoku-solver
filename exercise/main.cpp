#include <iostream>
#include <vector>
#include <string>
#include <set>
#include <stack>

//this passes meta infromation or extra infomration for  our printBoard function(ex: where is invalid placement)
struct ReturnValidity {
	ReturnValidity(bool valid, int invalidRow, int invalidCol, int rowTested, int  colTested, int value, const std::string& message)
		: valid{ valid }, invalidRow(invalidRow), invalidCol(invalidCol), rowTested(rowTested), colTested(colTested), value(value), invalidMessage(message) {
	}
	ReturnValidity() = default;
	bool valid;
	int invalidRow;
	int invalidCol;
	int rowTested;
	int colTested;
	int value;
	std::string invalidMessage;
};

ReturnValidity isValidPlacement(std::vector<std::vector<int>>& board, int row, int col, int value) {
	std::string invalidMessage;//for storing the invalid message
	if (value < 0 || value > 9) {
		invalidMessage += ("you are allowed only to enter values from 0 to 9\n\n");
		return { false, 0, 0, row, col, value, invalidMessage};
	}

	//checking row wise validity
	for (int i = 0; i < 9; i++)
		if (board[row][i] == value && i != col && value != 0) {
			invalidMessage += ("cannot place " + std::to_string(value) + " at " + "(" + std::to_string(row + 1) + ", " + std::to_string(col + 1) +
				") because there is " + std::to_string(value) + " in same row " + std::to_string(row + 1) + " at column " + std::to_string(i + 1) + "\n\n");
			return { false, row, i, row, col, value, invalidMessage};
		}
	//checking column wise validity
	for (int i = 0; i < 9; i++)
		if (board[i][col] == value && i != row && value != 0) {
			invalidMessage += ("cannot place " + std::to_string(value) + " at " + "(" + std::to_string(row + 1) + ", " + std::to_string(col + 1) + 
				") because there is " + std::to_string(value) + " in same column " + std::to_string(col + 1) + " at row " + std::to_string(i + 1) + "\n\n");

			return { false, i, col, row, col, value, invalidMessage};
		}

	//checking validity in their respective 3X3 grid

	//this will get the column and the row of the begining cell of its 3X3 grid
	int gridRow3X3 = row - row % 3;
	int gridCol3X3 = col - col % 3;
	for (int i = gridRow3X3; i < gridRow3X3 + 3; i++) {
		for (int j = gridCol3X3; j < gridCol3X3 + 3; j++) {
			if (board[i][j] == value && !(row == i && col == j) && value != 0) {
				invalidMessage += ( "cannot place " + std::to_string(value) + " at " + "(" + std::to_string(row + 1) + ", " + std::to_string(col + 1) +
					") because there is " + std::to_string(value) + " in same 3X3 gird at row " + std::to_string(i + 1) + " and at column " + std::to_string(j + 1) + "\n\n");
				return {false, i, j, row, col, value, invalidMessage};
			}
		}

	}
	return { true, 0, 0, 0, 0, 0, "no message"};
}


bool checkSolved(std::vector<std::vector<int>>& board) {
	for (int row = 0; row < board.size(); row++)
		for (int col = 0; col < board[0].size(); col++)
			if (board[row][col] == 0)
				return false;
	return true;
}
void printBoard(std::vector<std::vector<int>>& board, const ReturnValidity& validity) {
	std::cout << "****THE BOARD****" << std::endl << std::endl;
	std::cout << "     ";
	//printing the column label
	for (int i = 0; i < 9; i++) {
		if ((i + 1) % 3 == 0)
			std::cout << i + 1 << "    ";
		else std::cout << i + 1 << "  ";
	}
	std::cout << std::endl << std::endl;
	for (int row = 0; row < board.size(); row++) {
		std::cout << row + 1 << "    ";//print row label
		for (int col = 0; col < board[0].size(); col++) {
			// this will add asterics in the cell that make the placement invalid
				if (((row == validity.invalidRow && col == validity.invalidCol && board[row][col] != 0) || 
				  (row == validity.rowTested && col == validity.colTested)) && !validity.valid) {
					if ((col + 1) % 3 == 0 && col != 0)
						std::cout << board[row][col] << "*  ";
					else std::cout << board[row][col] << "* ";
				}
			else {
				if ((col + 1) % 3 == 0 && col != 0)
					std::cout << board[row][col] << "    ";
				else std::cout << board[row][col] << "  ";
			}
		}
		if ((row + 1) % 3 == 0)
			std::cout << std::endl;//add extra line in the row for making 3X3 grid
		std::cout << std::endl;
	}
	std::cout << std::endl << std::endl;
	//we only print error message when there is invalid placement
	if (!validity.valid)
		std::cout << validity.invalidMessage << std::endl;
	else std::cout << "SUCCESFULLY PLACED!!!!!!!!" << std::endl;
}



std::pair<int, int> findFirstEmptyCell(const std::vector<std::vector<int>>& board) {
	int row;
	int col;
	for (row = 0; row < board.size(); row++) {
		for (col = 0; col < board.size(); col++) {
			if (board[row][col] == 0)
				return { row, col };
		}
	}
	return { -1, -1 };
}
bool solveSoduku(std::vector<std::vector<int>>& board, int& attempt) {
	std::pair<int, int> pair = findFirstEmptyCell(board);
	int row = pair.first;
	int col = pair.second;
	if (row == -1 && col == -1)
		return true;
	for (int i = 1; i < 10; i++) {
		attempt++;
		if (!isValidPlacement(board, row, col, i).valid) continue;
		board[row][col] = i;
		if (solveSoduku(board, attempt))
			return true;
		board[row][col] = 0;
	}
	return false;
}


int main() {
	std::vector<std::vector<int>> board = {
	{8, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 3, 6, 0, 0, 0, 0, 0},
	{0, 7, 0, 0, 9, 0, 2, 0, 0},
	{0, 5, 0, 0, 0, 7, 0, 0, 0},
	{0, 0, 0, 0, 4, 5, 7, 0, 0},
	{0, 0, 0, 1, 0, 0, 0, 3, 0},
	{0, 0, 1, 0, 0, 0, 0, 6, 8},
	{0, 0, 8, 5, 0, 0, 0, 1, 0},
	{0, 9, 0, 0, 0, 0, 4, 0, 0}
	};


	int attempt = 0;
	solveSoduku(board, attempt);
	printBoard(board, isValidPlacement(board, 0, 0, 4));
	std::cout << attempt;
}