#include "History.h"
#include <iostream>
using namespace std;

History::History(int nRows, int nCols) {
    m_rows = nRows;
    m_cols = nCols;
    
    
    for (int i = 0; i < m_rows; i++) {
        for (int j = 0; j < m_cols; j++) {
            m_grid[i][j] = 0;
        }
    }
    
}

bool History::record(int r, int c){
    if (r < 1 || c < 1 || r > m_rows || c > m_cols)     // if out of bounds, return false
        return false;
    m_grid[r-1][c-1]++;         // otherwise, record location
    return true;
}

void History::display() const{
    clearScreen();
    char displayGrid[MAXROWS][MAXCOLS];
    for (int r = 0; r < m_rows; r++)
        for (int c = 0; c < m_cols; c++) {
            if (m_grid[r][c] == 0)
                displayGrid[r][c] = '.';        // change 0 to .
            else if (m_grid[r][c] <= 26)
                displayGrid[r][c] = 'A' + m_grid[r][c] - 1;     // change 1-26 to A-Z
            else
                displayGrid[r][c] = 'Z';        // change 26+ to Z
        }
    for (int r = 0; r < m_rows; r++) {
        for (int c = 0; c < m_cols; c++)     // display the grid
            cout << displayGrid[r][c];
        cout << endl;
    }
    cout << endl;
}

