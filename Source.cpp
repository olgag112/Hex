#include <iostream>
#include <stdio.h>
#include <string>

using namespace std;

struct cells           
{
    int row = 0;
    int column = 0;
    bool visited = false;
    cells* next = nullptr;
};

void addNode(cells*& head, int row, int column)
{
    cells* newNode = new cells;
    newNode->row = row;
    newNode->column = column;
    newNode->next = nullptr; 

    if (head == nullptr)
    {
        head = newNode;
    }
    else
    {
        cells* current = head;
        while (current->next != nullptr)
        {
            current = current->next;
        }
        current->next = newNode;
    }
}

void deleteList(cells*& head) 
{
    cells* current = head;
    cells* nextNode = nullptr;

    while (current != nullptr) 
    {
        nextNode = current->next; 
        delete current; 
        current = nextNode; 
    }

    head = nullptr; 
}

bool isBoardCorrect(int red, int blue)
{
    int difference = red - blue;
    if (difference != 0 && difference != 1)
    {
        cout << "NO" << endl;
        return false;
    }
    else return true;
}

bool checkTopCorner(char** board, bool**& visited, cells*& currCell, cells*& pawnsBorders, int x, int y, int width, int size, bool& redWon, bool& blueWon, bool& firstCall, bool&newPath);
bool checkBottomCorner(char** board, bool**& visited, cells*& currCell, cells*& pawnsBorders, int x, int y, int width, int size,  bool& redWon, bool& blueWon, bool& firstCall, bool& newPath);

void changeCurrentToNext(cells*& cell, cells*& pawnsBorders, int x, int y, bool**& visited, char** board,int width, int size, bool& redWon, bool& blueWon, bool& firstCall, bool& newPath) //rekurencja, hyba zmienic na bool
{
    cells* nextPawn = nullptr, *traverse = pawnsBorders;
    bool pawnFound = true;
    static int startX = 0, startY = 0, pawnInPath = 1;
    static int** oppositeSideCoords = new int* [size];

    if (newPath)
    {
        startX = cell->row, startY = cell->column;
        int j = startY;
       
        for(int i = 0; i < size; i++)
        {
            oppositeSideCoords[i] = new int[2];
        }

        if (startX <= size && y <= width / 2)                   // left top board side
        {
            j = width - 3;
            for (int i = 0; i < size; i++)
            {
                oppositeSideCoords[i][0] = size+i;
                oppositeSideCoords[i][1] = j;
                j -= 3;
            }
        }
        else if (startX <= size && y >= width / 2)              // right top board side
        {
            j = 2;
            for (int i = 0; i < size; i++)
            {
                oppositeSideCoords[i][0] = size + i;
                oppositeSideCoords[i][1] = j;
                j+=3;
            }
        }
        else if (startX >= size && y <= width / 2)              //left bottom
        {
            j = width / 2;
            for (int i = 0; i < size; i++)
            {
                oppositeSideCoords[i][0] = i + 1;
                oppositeSideCoords[i][1] = j;
                j += 3;
            }
        }
        else //right bottom
        {
            j = width / 2;
            for (int i = 0; i < size; i++)
            {
                oppositeSideCoords[i][0] = i + 1;
                oppositeSideCoords[i][1] = j;
                j -= 3;
            }
        }

        firstCall = false;
    }

    if (newPath)
    {
        pawnInPath = 1;
        pawnFound = true;
        newPath = false;
    }

    visited[x][y] = true;
    addNode(cell, x, y);
    cell = cell->next;

    if (pawnFound) pawnInPath++;
    
    while (pawnFound)
    {
        pawnFound = checkTopCorner(board, visited, cell, pawnsBorders, x, y, width, size, redWon, blueWon, firstCall, newPath);
        if (!pawnFound)
        {
            pawnFound = checkBottomCorner(board, visited, cell, pawnsBorders, x, y, width,size, redWon, blueWon, firstCall, newPath);
        }

        if (pawnFound)
        {
            pawnInPath++;
            changeCurrentToNext(cell, pawnsBorders, x, y, visited, board,width, size, redWon, blueWon, firstCall, newPath);
        }
    }

    for (int i = 0; i < size; i++)
    {
        if (oppositeSideCoords[i][0] == cell->row && oppositeSideCoords[i][1] == cell->column)
        {
            if (board[cell->row][cell->column] == 'r') redWon = true;
            else if (board[cell->row][cell->column] == 'b') blueWon = true;
        }
    }

   /* if (pawnInPath >= 4)
    {
        while (traverse != nullptr)
        {
            if (traverse->row == cell->row && traverse->column == cell->column)
            {
                if (cell->row != startX || cell->column != startY)
                {
                    if (board[cell->row][cell->column] == 'r') redWon = true;
                    else if (board[cell->row][cell->column] == 'b') blueWon = true;
                }
            }
            if (redWon || blueWon)
            {
                pawnInPath = 0;
                break;
            }
            traverse = traverse->next;
        }
    }*/
}
//  [x * size + x - 1, y * 2 - 3 * x]
bool checkTopCorner(char** board, bool**& visited, cells*& currCell, cells*& pawnsBorders, int x, int y, int width, int size, bool& redWon, bool& blueWon, bool&firstCall, bool&newPath)
{   
    if (currCell->row + 1 <= 2*size && currCell->column - 3>= 0 && board[currCell->row + 1][currCell->column - 3] == board[x][y] && !visited[currCell->row + 1][currCell->column - 3])
    {
        visited[currCell->row + 1][currCell->column - 3] = true;
        changeCurrentToNext(currCell, pawnsBorders, currCell->row + 1, currCell->column - 3, visited, board, width,size, redWon, blueWon, firstCall, newPath);
        return true;
    }
    else if (currCell->row + 2 <= 2*size && board[currCell->row + 2][currCell->column] == board[x][y] && !visited[currCell->row + 2][currCell->column])
    {
        visited[currCell->row + 2][currCell->column] = true;
        changeCurrentToNext(currCell, pawnsBorders, currCell->row + 2, currCell->column, visited, board, width, size, redWon, blueWon, firstCall, newPath);
        return true;
    }
    else if (currCell->row + 1 <= 2*size && currCell->column + 3 <= width-1 && board[currCell->row + 1][currCell->column + 3] == board[x][y] && !visited[currCell->row + 1][currCell->column + 3])
    {
        visited[currCell->row + 1][currCell->column + 3] = true;
        changeCurrentToNext(currCell, pawnsBorders, currCell->row + 1, currCell->column + 3, visited, board, width, size, redWon, blueWon, firstCall, newPath);
        return true;
    }
    else if (size == 1)//(x == pawnsBorders->row && y == pawnsBorders->column)
    {
        if (board[x][y] == 'r')
        {
            redWon = true;
        }
    }
    else return false;
}

bool checkBottomCorner(char** board, bool**& visited, cells*& currCell, cells*& pawnsBorders, int x, int y, int width, int size, bool& redWon, bool& blueWon, bool&firstCall, bool& newPath)
{
    if (currCell->row - 1>=0 && currCell->column - 3>=0 && !visited[currCell->row - 1][currCell->column - 3] && board[currCell->row - 1][currCell->column - 3] == board[x][y])
    {
        visited[currCell->row - 1][currCell->column - 3] = true;
        changeCurrentToNext(currCell, pawnsBorders, currCell->row - 1, currCell->column - 3, visited, board, width, size, redWon, blueWon, firstCall, newPath);
        return true;
    }
    else if (currCell->row - 2>=0 && !visited[currCell->row - 2][currCell->column] && board[currCell->row - 2][currCell->column] == board[x][y])
    {
        visited[currCell->row - 2][currCell->column] = true;
        changeCurrentToNext(currCell, pawnsBorders, currCell->row - 2, currCell->column, visited, board, width, size, redWon, blueWon, firstCall, newPath);
        return true;
    }
    else if (currCell->row - 1>=0 && currCell->column + 3 <= width-1 && !visited[currCell->row - 1][currCell->column + 3] && board[currCell->row - 1][currCell->column + 3] == board[x][y])
    {
        visited[currCell->row - 1][currCell->column + 3] = true;
        changeCurrentToNext(currCell, pawnsBorders, currCell->row - 1, currCell->column + 3, visited, board, width, size, redWon, blueWon, firstCall, newPath);
        return true;
    }
    else if (size == 1)
    {
        if (board[x][y] == 'r')
        {
            redWon = true;
        }
    }
    else return false;
}

void findPlayerPath(char** board, cells*& pawnsBorders, bool& red, bool& blue, int size, int width)
{
    cells* currPawns = pawnsBorders;        
    cells* currCell = new cells;
    int y = currPawns->column, x = currPawns->row;  
    bool firstFunctionCall = true, nextPawnFound = false;
    static bool newPath = false;

    bool** visited = new bool* [2*size+1];
    for (int a = 0; a < 2 * size + 1; a++)
    {
        visited[a] = new bool[width];
        for (int b = 0; b < width; b++)
        {
            visited[a][b] = 0;
        }
    }

    while (currPawns != nullptr && !red && !blue) 
    {
        newPath = true;
        currCell->row = x, currCell->column = y;

        if (!visited[x][y])
        {
            if (x <= size && y <= width / 2)    //left top side
            {
                if (y == width / 2)                 //top corner
                {
                    visited[x][y] = true;
                    checkTopCorner(board, visited, currCell, pawnsBorders, x, y, width, size, red, blue, firstFunctionCall, newPath);
                }
                else if (y == 2)                    //left corner
                {
                    if (!visited[x - 1][y + 3] && board[x - 1][y + 3] == board[x][y])
                    {
                        visited[x][y] = true;
                        changeCurrentToNext(currCell, pawnsBorders, x - 1, y + 3, visited, board, width, size, red, blue, firstFunctionCall, newPath);
                    }
                    else if (!visited[x + 1][y + 3] && board[x + 1][y + 3] == board[x][y])
                    {
                        visited[x][y] = true;
                        changeCurrentToNext(currCell, pawnsBorders, x + 1, y + 3, visited, board, width, size, red, blue, firstFunctionCall, newPath);
                    }
                }
                else                                //between corners
                {
                    visited[x][y] = true;
                    nextPawnFound = checkTopCorner(board, visited, currCell, pawnsBorders, x, y, width, size, red, blue, firstFunctionCall, newPath);
                    if (!nextPawnFound)
                    {
                        if (board[x - 1][y + 3] == board[x][y])     //right top cell 
                        {
                            changeCurrentToNext(currCell, pawnsBorders, x - 1, y + 3, visited, board, width, size, red, blue, firstFunctionCall, newPath);
                        }
                    }
                }
            }
            else if (x <= size && y > width / 2)
            {
                if (y == width - 3)                 //right corner
                {
                    if (!visited[x - 1][y - 3] && board[x - 1][y - 3] == board[x][y])
                    {
                        visited[x][y] = true;
                        changeCurrentToNext(currCell, pawnsBorders, x - 1, y - 3, visited, board, width, size, red, blue, firstFunctionCall, newPath);
                    }
                    else if (!visited[x + 1][y - 3] && board[x + 1][y - 3] == board[x][y])
                    {
                        visited[x][y] = true;
                        changeCurrentToNext(currCell, pawnsBorders, x + 1, y - 3, visited, board, width, size, red, blue, firstFunctionCall, newPath);
                    }
                }
                else                                //cells between corners
                {
                    visited[x][y] = true;
                    nextPawnFound = checkTopCorner(board, visited, currCell, pawnsBorders, x, y, width, size, red, blue, firstFunctionCall, newPath);
                    if (!nextPawnFound)
                    {
                        if (!visited[x - 1][y - 3] && board[x - 1][y - 3] == board[x][y])     //left top cell 
                        {
                            changeCurrentToNext(currCell, pawnsBorders, x - 1, y - 3, visited, board, width, size, red, blue, firstFunctionCall, newPath);
                        }
                    }
                }
            }
            else if (x > size && y <= width/2)        //bottom left quarter of the board
            {
                visited[x][y] = true;
                if (y == width / 2)
                {
                    nextPawnFound = checkBottomCorner(board, visited, currCell, pawnsBorders, x, y, width, size, red, blue, firstFunctionCall, newPath);
                }
                
                if (!nextPawnFound)
                {
                    if (!visited[x + 1][y + 3] && board[x + 1][y + 3] == board[x][y])
                    {
                        changeCurrentToNext(currCell, pawnsBorders, x + 1, y + 3, visited, board, width, size, red, blue, firstFunctionCall, newPath);
                    }
                }
            }
            else
            {
                visited[x][y] = true;

                nextPawnFound = checkBottomCorner(board, visited, currCell, pawnsBorders, x, y, width, size, red, blue, firstFunctionCall, newPath);
                if (!nextPawnFound)
                {
                    if (!visited[x + 1][y + 3] && board[x + 1][y + 3] == board[x][y])
                    {
                        changeCurrentToNext(currCell, pawnsBorders, x + 1, y + 3, visited, board, width, size, red, blue, firstFunctionCall, newPath);
                    }
                }
            }
        }
       
        currPawns = currPawns->next;
        while (currPawns != nullptr && visited[currPawns->row][currPawns->column])
        {
            currPawns = currPawns->next;
        }
        
        if (currPawns != nullptr)
        {
            x = currPawns->row;
            y = currPawns->column;
        }
    }
    delete currCell;
}

void executeCommand(string command, int size, int pawns, int red, int blue, int width, char** board, cells*& pawnsBorders)
{
    bool is_board_correct = isBoardCorrect(red, blue), redWon = false, blueWon = false;

    if (command[0] == 'B')              //BOARD_SIZE
    {
        cout << size << endl;
    }
    else if (command[0] == 'P')         //PAWNS NUMBER
    {
        cout << pawns << endl;
    }
    else if (is_board_correct)          //IS BOARD CORRECT
    {
        if (command[3] == 'B')          //IS BOARD CORRECT
        {
            cout << "YES" << endl;
        }
        else if (command[3] == 'G')     //IS GAME OVER
        {
            if(pawnsBorders !=nullptr) findPlayerPath(board, pawnsBorders, redWon, blueWon, size, width);
             
            if (redWon)
            {
                cout << "YES RED" << endl;
            }
            else if (blueWon)
            {
                cout << "YES BLUE" << endl;
            }
            else
            {
                cout << "NO" << endl;
            }
        }
    }
    is_board_correct = true;

}

void countPawns(char** board, int rows, int cols, int& r, int& b, int &num)     
{
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            if (board[i][j] == 'b')
            {
                b++;
                num++;
            }
            else if (board[i][j] == 'r')
            {
                r++;
                num++;
            }
        }
    }
}

void pawnsBesideBorders(char** board, cells*& allPawnsHead, cells*& pawnsBesideBorders, int width, int size)
{
    cells* curr = allPawnsHead;
    int x = curr->column, y = curr->row, xLeftBorder = width / 2, xRightBorder = width / 2;


    for (int i = 1; i < 2 * size; i++)  //dla kazdego rzedu z cellami
    {
        while (curr != nullptr && y == i)
        {
            if (i <= size)
            {
                if (board[y][x] != ' ' && x == width/2 && y == 1)     
                {
                    addNode(pawnsBesideBorders, y, x);
                }
                else if (board[y][x] != ' ' && y == size && (x == 2 || x == width - 3)) 
                {
                    addNode(pawnsBesideBorders, y, x);
                }
                else if (x == xLeftBorder && board[y][x] == 'r')
                {
                    addNode(pawnsBesideBorders, y, x);
                }
                else if (x == xRightBorder && board[y][x] == 'b')
                {
                    addNode(pawnsBesideBorders, y, x);
                }

            }
            else
            {
                if (board[y][x] != ' ' && x == width / 2 && y == 2*size - 1)
                {
                    addNode(pawnsBesideBorders, y, x);
                }
                else if (x == xLeftBorder && board[y][x] == 'b')
                {
                    addNode(pawnsBesideBorders, y, x);
                }
                else if (x == xRightBorder && board[y][x] == 'r')
                {
                    addNode(pawnsBesideBorders, y, x);
                }

            }
            curr = curr->next;
            if (curr != nullptr)
            {
                x = curr->column;
                y = curr->row;
            }
        }

        if (i < size)
        {
            xLeftBorder -= 3;
            xRightBorder += 3;
        }
        else
        {
            xLeftBorder += 3;
            xRightBorder -= 3;
        }
    }
}


int main()          // Red starts the game
{
    int maxRowLength = 1, maxRowNum = 1, x = 0,  RedNum = 0, BlueNum = 0, pawnsNum = 0, size = 0;
    cells* cellHead = nullptr, *pawnsBoardersHead = nullptr;
    string line, command;


    while (getline(cin, line))
    {
        maxRowLength = (line.length() - 1) * 2 - 1;
        size = (maxRowLength + 1) / 6;
        maxRowNum = 2 * size + 1;

        char** board = new char* [maxRowNum];      
        for (int a = 0; a < maxRowNum; a++)
        {
            board[a] = new char[maxRowLength];
        }

        x = 0;
        for (int i = 0; i < line.length(); i++)
        {
            board[x][i] = line[i];
        }
        for (int i = line.length(); i < maxRowLength; i++)
        {
            board[x][i] = ' ';
        }
        x++;

        for(x; x<maxRowNum; x++)
        {
            getline(cin, line);
            for (int y = 0; y < line.length(); y++)
            {
                board[x][y] = line[y];
                if (board[x][y] == 'r' || board[x][y] == 'b')
                {
                    addNode(cellHead, x, y);
                }
            }
            for (int i = line.length(); i < maxRowLength; i++)
            {
                board[x][i] = ' ';
            }
        }

     

        if (cellHead != nullptr)
        {
            pawnsBesideBorders(board, cellHead, pawnsBoardersHead, maxRowLength, size); 
        }

        countPawns(board, maxRowNum, maxRowLength, RedNum, BlueNum, pawnsNum);

        getline(cin, command);

        executeCommand(command, size, pawnsNum, RedNum, BlueNum, maxRowLength, board, pawnsBoardersHead);

        command.clear();

        for (int z = 0; z < maxRowNum; z++) 
        {
            delete[] board[z];  
        }
        delete[] board;
        

        deleteList(cellHead);
        deleteList(pawnsBoardersHead);
        
        maxRowLength = 0;
        maxRowNum = 0;
        size = 0;
        pawnsNum = 0;
        RedNum = 0;
        BlueNum = 0;
        x = 0;

        getline(cin, line);
    }

    return 0;
}

