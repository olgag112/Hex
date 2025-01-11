This project is an implementation of Hex game, developed during studies as an assignment. Code is written in C++ language.

Rules of the game are as follows:
The game is played by two players (red and blue).
The starting player is the red player.
Players take turns placing a piece of their color on any unoccupied space.
The player who forms a continuous line connecting two edges of the board with their own color wins.


Commands:

BOARD_SIZE - a number from the range <1;11> indicating the size of the board. 

PAWNS_NUMBER - a number from the range <0;121> indicating the number of pawns of both players on the board. 

IS_BOARD_CORRECT - a YES/NO response indicating whether the state of the board is correct, in other words, whether the number of pawns of one player is appropriate relative to the number of pawns of the other player.

IS_GAME_OVER - a YES RED/YES BLUE/NO response indicating whether (and if so, which) player has ended the game, i.e., whether they have connected two edges of the board with an uninterrupted path of their color. 
