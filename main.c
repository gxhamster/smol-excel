#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "CellGrid.h"
#include "Stack.h"

int main(void)
{
	CellGrid *cg = CellGrid_read_from_csv("input.csv", ',');
    cg = CellGrid_parse_numbers(cg);
    cg = CellGrid_parse_expr(cg);
    cg = CellGrid_eval_cells(cg);
    CellGrid_print(cg);

    /*
       1,2,5.23
       3,C1+B1,2.0
       10,2,A1+B1
       */

    // C1+B2
    // CellGrid_eval_cell_expr1(&cg->cells[1][1], cg);

    return 0;
}
