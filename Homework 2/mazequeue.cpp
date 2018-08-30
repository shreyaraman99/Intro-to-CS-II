#include <iostream>
#include <queue>
#include <cassert>
using namespace std;

class Coord
{
public:
    Coord(int rr, int cc) : m_r(rr), m_c(cc) {}
    int r() const { return m_r; }
    int c() const { return m_c; }
private:
    int m_r;
    int m_c;
};

bool pathExists(string maze[], int nRows, int nCols, int sr, int sc, int er, int ec) {
    queue<Coord> coordQueue;
    Coord start(sr, sc);
    coordQueue.push(start);
    maze[sr][sc] = '&';
    
    while (!coordQueue.empty()) {
        Coord temp = coordQueue.front();
        int r = temp.r();
        int c = temp.c();
        coordQueue.pop();
        
        if ((r == er) && (c == ec))
            return true;
        if (maze[r][c + 1] == '.') {
            coordQueue.push(Coord(r, c + 1));       // EAST
            maze[r][c + 1] = '&';
        }
        if (maze[r + 1][c] == '.') {
            coordQueue.push(Coord(r + 1, c));       // SOUTH
            maze[r + 1][c] = '&';
        }
        if (maze[r][c - 1] == '.') {
            coordQueue.push(Coord(r, c - 1));       // WEST
            maze[r][c - 1] = '&';
        }
        if (maze[r - 1][c] == '.') {
            coordQueue.push(Coord(r - 1, c));       // NORTH
            maze[r - 1][c] = '&';
        }

    }
    return false;
}

int main()
{
    string maze[10] = {
        "XXXXXXXXXX",
        "X...X..X.X",
        "X..XX....X",
        "X.X.XXXX.X",
        "XXX......X",
        "X...X.XX.X",
        "X.X.X..X.X",
        "X.XXXX.X.X",
        "X..X...X.X",
        "XXXXXXXXXX"
    };
    
    if (pathExists(maze, 10,10, 4,3, 1,8))
        cout << "Solvable!" << endl;
    else
        cout << "Out of luck!" << endl;
}
