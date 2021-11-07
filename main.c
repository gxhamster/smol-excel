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

	/*
	1,2,5.23
	3,C1+B1,2.0
	10,2,A1+B1
 	*/
	// Cell c = cg->cells[1][1];
	// printf("num: %f\n", CellGrid_eval_cell_expr(&c, cg).f);

	return 0;
}
