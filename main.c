#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "CellGrid.h"

int main(void)
{
	CellGrid *cg = CellGrid_read_from_csv("input.csv", ',');
	cg = CellGrid_parse_numbers(cg);
	cg = CellGrid_parse_expr(cg);

	CellGrid_print(cg);

	// Cell *c = CellGrid_search_by_grid_pos("C3", cg);
	// CellGrid_eval_cell_expr(c, cg);

	Cell c = cg->cells[1][1];
	printf("num: %f\n", CellGrid_eval_cell_expr(&c, cg).f);

	return 0;
}
