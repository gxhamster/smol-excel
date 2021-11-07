#include <stdbool.h>

typedef union {
	int i;
	float f;
} Cell_eval;

enum CellTypes {
	INT,
	FLOAT,
	STRING,
	EXPR
};


typedef struct {
	char *contents;
	enum CellTypes cell_type;
	// A1, B2
	char *grid_pos;
	Cell_eval cell_eval;
} Cell;

#define GRID_MAX 10
typedef struct {
	Cell cells[GRID_MAX][GRID_MAX];
	size_t num_rows;
	size_t num_cols;
} CellGrid;

/* 
 * When A1 + B2 is eval it could either be a float or an integer
 * This union will hold the eval value
 */


void CellGrid_print(CellGrid *cg);
#define STREAM_BUFFER_MAX 1024
CellGrid *CellGrid_read_from_csv(const char *file_path, char delim);

int CellGrid_eval_cell_int(Cell *c);

float CellGrid_eval_cell_float(Cell *c);

Cell_eval CellGrid_eval_cell_expr(Cell *c, CellGrid *cg);

bool CellGrid_cell_is_expr(Cell *c);

bool CellGrid_cell_is_float(Cell *c);

bool CellGrid_cell_is_int(Cell *c);

Cell *CellGrid_search_by_grid_pos(const char *pos, CellGrid *cg);

/* Utility functions */

// get field from string given a delim
char *getfield(char *line, int num, char delim);
// get number of columns in a line given a delim
int get_num_of_fields(char *line, char delim);

void remove_spaces (char* str_trimmed, const char* str_untrimmed);






