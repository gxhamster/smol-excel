#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "CellGrid.h"

int main(void)
{
	CellGrid *cg = CellGrid_read_from_csv("input.csv", ',');
	CellGrid_print(cg);
		
	// Cell *c = CellGrid_search_by_grid_pos("C3", cg);
	// CellGrid_eval_cell_expr(c, cg);	
	
	printf("num: %d\n", cg->cells[2][2].cell_eval.i);

	return 0;
}
