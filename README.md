# Connect 6 AI Project

Developed in Nov. 2013 for APS105 - Computer Fundamentals programming course. 400 students participated in the tournament. After a huge round-robbin, my code placed top 8!

Task was to develop a program in C that plays Connect 6 on a 19x19 board. Connect 6 is a board game that mixes Connect Four with the Chinese game GO. More about the game can be found here: https://en.wikipedia.org/wiki/Connect6.

For this assignment, the game was simplified such that each player only makes one move per turn and there is no handicap. Solutions for this must operate under the following constraints (Facing disqualification if broken):
 - Any moves made by the program must be legal (i.e. can't play a move outside the board or on an existing piece)
 - Moves must be made within 1 second
 - Ties are broken by the program that took less overall time in the game
 
 
# Strategy

I employed a recursive strategy to play the game. The program follows the following task flow:

1) Check for immediate wins (One move wins)
2) Check for immediate losses (One move wins for the opponent)
3) Check for forced wins (Moves that lead into a sequences where a win is inevitable)
4) Check for forced losses (Same as 3, from the perspective of the opponent)
5) Play a generic offensive move (Fallback option if time is running short)

