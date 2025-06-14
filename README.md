# Sokoban
Sokoban is a grid-based puzzle game where a player, Bob, must move boxes onto designated storage locations inside a warehouse. The room is bordered by walls and contains an equal number of boxes and storage spots. 
Bob can move the boxes by 
- pushing the box by moving into it, if the space behind the box is empty, or
- pulling the adjacent box into the square he previously occupied, if the space behind Bob is empty.


The objective is to help Bob find the shortest sequence of moves to place all boxes on storage locations.

<img width="419" alt="Screenshot 2025-06-10 at 12 55 29 PM" src="https://github.com/user-attachments/assets/db15a781-0937-41c5-9d75-e950d22d4429" />

A screenshot from a level in one of the earlier releases of the Sokoban puzzle (Soko-Ban, 1988).

# Strategy:
This program uses Breadth-First Search (BFS) to explore all possible valid moves that Bob can make in the grid. Each state includes the positions of Bob and all boxes. At each step, we consider:
- Normal move: Bob moves into an adjacent empty or storage square.
- Push: Bob moves into a square with a box, pushing it forward (if the next square is empty or a storage location).
- Pull: Bob moves away from a box that is directly adjacent and drags it into his current position (if the move is valid).


We explore all such moves until a state is reached where all boxes are on storage locations, and return the shortest sequence of moves that leads to this solution.

# Input format:
The first line contains two integers, N and M, where
- N is the number of boxes (0 ≤ N ≤ 3),
- M is the width/height of the square grid (3 ≤ M ≤ 13).
The next N lines each contain two integers r_i c_i, giving the 0-indexed position of the i-th box.
The next M lines each contain M characters describing the room layout:
- 'X' = wall
- 'O' = storage location
- 'B' = Bob’s starting position (exactly one)
- '-' = empty tile

Constraints:
- Box positions never overlap with walls or Bob’s initial position.
- Exactly N storage locations exist.
- Bob is fully enclosed by walls and cannot reach the grid's border.

Sample input:

3 10

3 4

5 7

6 4

---XXXX---

---XO-X---

---X--X---

-XXX--XXX-

-X------X-

-XB---O-X-

-XXX--XXX-

---X--X---

---XO-X---

---XXXX---

# Output Format:
First line: an integer K, the number of moves in the solution.

Second line: K space-separated move commands.

Each move is either:
- A regular move: one of U, D, L, R (up, down, left, right), or
- A box pull move: one of UP, DP, LP, RP (pulling a box from up, down, left, right).

The output of the program represents the shortest valid sequence of moves that places all boxes on the storage locations.





