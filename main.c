#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "CellGrid.h"

int main(void)
{
	CellGrid *cg = CellGrid_read_from_csv("input.csv", ',');
	// CellGrid_print(cg);
	
	CellGrid_eval_cell_expr(&cg->cells[2][2]);
	
	return 0;
}
