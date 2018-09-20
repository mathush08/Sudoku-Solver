/////////////////////////////////////////////////////////////////////////////
// INTEGRITY INSTRUCTIONS

// Explicitly state the level of collaboration on this question
// Examples:
//   * I discussed ideas with classmate(s) [include name(s)]
//   * I worked together with classmate(s) in the lab [include name(s)]
//   * Classmate [include name] helped me debug my code
//   * I consulted website [include url]
//   * None
// A "None" indicates you completed this question entirely by yourself
// (or with assistance from course staff)
/////////////////////////////////////////////////////////////////////////////
// INTEGRITY STATEMENT:
// I received help from the following sources:

// None

// Name: Mathush Muhunthan
// login ID: m2muhunt
/////////////////////////////////////////////////////////////////////////////

#include "cs136.h"
#include "sudoku.h"
#include "array.h"

const int DIM = 9;

static const int EMPTY = 0;
static const char BLANK = '-';
static const int MIN = 1;
static const int MAX = 9;

static const int SUCCESS = 0;
static const int ERROR = -1;
static const int ERASE_EMPTY_CELL = -2;
static const int ERASE_FILLED_CELL = -3;
static const int ERROR_NEXT_CELL = -4;

struct sudoku {
  int puzzle[DIM * DIM];
  int sol[DIM * DIM];
};

// You need to implement the following four functions.

int fill_cell(struct sudoku *s, int row, int col, int num) {
  assert(s);
  assert(0 <= row && row <= 8);
  assert(0 <= col && col <= 8);
  assert(1 <= num && num <= 9);
  assert(s->sol[row*DIM + col] == EMPTY);

  // Implement this function
  // Row Check
  for (int i = 0; i < DIM; ++i) {
    if (s->puzzle[row*DIM + i] == num) {
      return ERROR;
    }
  }
  // Column Check
  for (int j = 0; j < DIM; ++j) {
    if (s->puzzle[col + j*DIM] == num) {
      return ERROR;
    }
  }
  // Box Check
  int x_box_corner = row - (row % 3);
  const int y_box_corner = col - (col % 3);
  for (int k = 0; k < DIM/3; ++k) {
    if (s->puzzle[(y_box_corner + k) + x_box_corner*DIM] == num) {
      return ERROR;
    }
  }
  ++x_box_corner;
  for (int k = 0; k < DIM/3; ++k) {
    if (s->puzzle[(y_box_corner + k) + x_box_corner*DIM] == num) {
      return ERROR;
    }
  }
  ++x_box_corner;
  for (int k = 0; k < DIM/3; ++k) {
    if (s->puzzle[(y_box_corner + k) + x_box_corner*DIM] == num) {
      return ERROR;
    }
  }
  s->sol[row*DIM + col] = num;
  return SUCCESS;
}

// Helper Function
// is_valid(s, row, col, num) checks to see if the given number
// is valid in the given location excluding the given spot
bool is_valid(const struct sudoku *s, int row, int col, int num) {
  if (num == EMPTY) {
    return false;
  }
  for (int i = 0; i < DIM; ++i) {
    if (i == col) {
      continue;
    }
    if (s->sol[row*DIM + i] == num) {
      return false;
    }
  }
  // Column Check
  for (int j = 0; j < DIM; ++j) {
    if (j == row) {
      continue;
    }
    if (s->sol[col + j*DIM] == num) {
      return false;
    }
  }
  // Box Check
  int x_box_corner = row - (row % 3);
  const int y_box_corner = col - (col % 3);
  for (int k = 0; k < DIM/3; ++k) {
    if ((x_box_corner == row) && (y_box_corner + k == col)) {
      continue;
    }
    if (s->puzzle[(y_box_corner + k) + x_box_corner*DIM] == num) {
      return false;
    }
  }
  ++x_box_corner;
  for (int k = 0; k < DIM/3; ++k) {
    if ((x_box_corner == row) && (y_box_corner + k == col)) {
      continue;
    }
    if (s->sol[(y_box_corner + k) + x_box_corner*DIM] == num) {
      return false;
    }
  }
  ++x_box_corner;
  for (int k = 0; k < DIM/3; ++k) {
    if ((x_box_corner == row) && (y_box_corner + k == col)) {
      continue;
    }
    if (s->sol[(y_box_corner + k) + x_box_corner*DIM] == num) {
      return false;
    }
  }
  return true;
}

void choices_cell(const struct sudoku *s, int row, int col, 
                  int choices[], int *num_choices) {
  assert(s);
  assert(choices);
  assert(0 <= row && row <= 8);
  assert(0 <= col && col <= 8);

  // Implement this function.
  int count = 0;
  for (int i = MIN; i <= MAX; ++i) {
    if (is_valid(s, row, col, i)) {
      choices[count] = i;
      count++;
    }
  }
  *num_choices = count;
}

bool solved_puzzle(const struct sudoku *s) {
  assert(s);

  // Implement this function
  for (int i = 0; i < DIM * DIM; ++i) {
    int row = i / DIM;
    int col = i % DIM;
    int num = s->sol[i];
    if (!is_valid(s, row, col, num)) {
      return false;
    }
  }
  return true;
}

bool solve(struct sudoku *s) {
  assert(s);

  // Implement this function
  if (solved_puzzle(s)) {
    return true;
  }

  // finding unfilled position
  int pos = 0;
  while (pos < DIM * DIM && s->sol[pos] != EMPTY) {
    ++pos;
  }
  if (pos == DIM * DIM) 
    return false;

  // cannot solve current pos
  int row = pos / DIM;
  int col = pos % DIM;
  int choices[9] = {0};
  int num_choices = 0;
  choices_cell(s, row, col, choices, &num_choices);
  if (num_choices == 0) {
    return false;
  }

  // trying all values at pos
  for (int i = 0; i < num_choices; ++i) {
    s->sol[pos] = choices[i];
    if (solve(s)) {
      return true;
    }
  }

  // no values work for pos
  erase_cell(s, row, col);
  return false;
}

// There is no need to modify the rest of the functions.

struct sudoku *read_sudoku(void) {
  struct sudoku *s = malloc(sizeof(struct sudoku));
  char c = 0;
  for (int row = 0; row < DIM; ++row) {
    for (int col = 0; col < DIM; ++col) {
      scanf(" %c", &c);
      if (c == BLANK) {
        s->puzzle[row * DIM + col] = 0;
      } else {
        s->puzzle[row * DIM + col] = c - '0';
      }
    }
  }

  // copy puzzle to solution
  reset_sol(s);

  return s;
}

void sudoku_destroy(struct sudoku *s) {
  assert(s);
  free(s);
}

void print_sol(const struct sudoku *s) {
  assert(s);

  printf("\n");
  for (int row = 0; row < DIM; ++row) {
    for (int col = 0; col < DIM; ++col) {
      int num = s->sol[row * DIM + col];
      if (num == EMPTY) {
        printf("%c", BLANK);
      } else {
        printf("%d", num);
      }  
    }
    printf("\n");
  }
  printf("\n");
}

void reset_sol(struct sudoku *s) {
  assert(s);

  for (int row = 0; row < DIM; ++row) {
    for (int col = 0; col < DIM; ++col) {
      s->sol[row * DIM + col] = s->puzzle[row * DIM + col];
    }
  }  
}

// cell_empty(board, row, col) returns true
// if cell (row,col) is empty on board.
// requires: board is a valid sudoku puzzle.
static bool cell_empty(const int board[], int row, int col) {
  assert(board);
  assert(0 <= row && row <= DIM - 1);
  assert(0 <= col && col <= DIM - 1);

  return board[row * DIM + col] == EMPTY;
}

int erase_cell(struct sudoku *s, int row, int col) {
  assert(s);
  assert(0 <= row && row <= DIM - 1);
  assert(0 <= col && col <= DIM - 1);

  if (cell_empty(s->sol, row, col)) {
    return ERASE_EMPTY_CELL;
  }
  if (!cell_empty(s->puzzle, row, col)) {
    return ERASE_FILLED_CELL;
  }
  s->sol[row * DIM + col] = EMPTY;
  return SUCCESS;
}


int next_cell(const struct sudoku *s, int *row, int *col) {
  assert(s);
  assert(row);
  assert(col);

  int choices[DIM];
  int num_choices = 0;
  for (int i = 0; i < DIM; ++i) {
    for (int j = 0; j < DIM; ++j) {
      if (!cell_empty(s->sol, i, j)) continue;
      choices_cell(s, i, j, choices, &num_choices);
      if (num_choices == 1) {
        *row = i;
        *col = j;
        return SUCCESS;
      }
    }
  }
  return ERROR_NEXT_CELL;
}
