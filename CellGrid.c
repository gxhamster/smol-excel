#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include "CellGrid.h"

void CellGrid_print(CellGrid *cg)
{
	size_t i;
	size_t j;
	for (i = 0; i < cg->num_rows; i++) {
		for (j = 0; j < cg->num_cols; j++) {
			if (cg->cells[i][j].cell_type == EXPR) {

				printf("%s: %d ", cg->cells[i][j].grid_pos, 
						cg->cells[i][j].cell_eval.i);
			} else {
			printf("%s: %s ", cg->cells[i][j].grid_pos, 
					cg->cells[i][j].contents);
			}
		}	
		printf("\n");
	}
}


// field starts from 0 not 1
char *getfield(char *line, int num, char delim)
{
	char *token;
	char *copy = (char *)malloc(strlen(line) + 1);
	strcpy(copy, line);

	char temp_delim1[] = {delim, '\0'};
	char temp_delim2[] = {delim, '\n', '\0'};

	int count = 0;
	token = strtok(copy, temp_delim1);
	if (count == num)
		return token;

	// /usr/bin:/bin:/usr/sbin:/sbin

	while (( token = strtok(NULL, temp_delim2) )) {
		count++;
		if (count == num)
			return token;
	}

	return NULL;
}

int get_num_of_fields(char *line, char delim)
{
	char *temp;
	int count = 0;
	count++;
	temp = line;
	while(*temp != '\0') {
		if (*temp == delim)
			count++;
		temp++;
	}
	return count;
}

CellGrid *CellGrid_read_from_csv(const char *file_path, char delim)
{
	char *COL_LETTERS = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	FILE* stream = fopen(file_path, "r");
	char line[STREAM_BUFFER_MAX];
	CellGrid *cg = malloc(sizeof(CellGrid));
	if (cg == NULL)
		return NULL;

	size_t rows;
	size_t cols;
	rows = 0;
	// fgets store the \n
	while(fgets(line, STREAM_BUFFER_MAX, stream) != NULL) {
		char *temp_line = strdup(line);
		cols = get_num_of_fields(temp_line, delim);
		cg->num_cols = cols;
		size_t i;
		Cell *cur_cell; 
		for (i = 0; i < cols; i++) {

			cur_cell = &cg->cells[rows][i];
			cur_cell->contents = getfield(temp_line, i, delim);

			// TODO: Set cell position on grid (This needs to be changed)
			char pos[3];
			pos[0] = COL_LETTERS[i];
			pos[1] = (rows+1) + '0';
			pos[2] = '\0';

			cur_cell->grid_pos = strdup(pos);
			// printf("Grid pos: %s\n", cur_cell->grid_pos);

			// Set evaluated value of each cell
			// Also set the type
			if (CellGrid_cell_is_int(cur_cell)) {
				cur_cell->cell_type = INT;
				cur_cell->cell_eval.i = CellGrid_eval_cell_int(cur_cell);
			} else if (CellGrid_cell_is_float(cur_cell)) {
				cur_cell->cell_type = FLOAT;
				cur_cell->cell_eval.f = CellGrid_eval_cell_float(cur_cell);
			} else if (CellGrid_cell_is_expr(cur_cell)) {
				cur_cell->cell_type = EXPR;
				cur_cell->cell_eval = CellGrid_eval_cell_expr(cur_cell, cg);
			}
		}
		rows++;
		cg->num_rows = rows;
		free(temp_line);
	}
	// printf("rows: %d  cols: %d\n", rows, cols);	

	return cg;	
}

bool CellGrid_cell_is_float(Cell *c)
{
	double dbl;
	int r;
	r = sscanf(c->contents, "%lf", &dbl);
	if(r == 1) {
		return true;
	}
	return false;
}

bool CellGrid_cell_is_int(Cell *c)
{
	int i, r, n;
	r = sscanf(c->contents, "%d%n", &i, &n);
	if(r == 1 && (size_t)n == strlen(c->contents)) {
		return true;
	}
	return false;
}

// expr -> A1+B1
bool CellGrid_cell_is_expr(Cell *c)
{
	char *temp;
	if (c->contents == NULL)
		return false;
	for (temp = c->contents; *temp != '\0'; temp++) {
		if (*temp == '+' || *temp == '-')
			return true;
	}
	return false;
}

void remove_spaces (char* str_trimmed, const char* str_untrimmed)
{
	while (*str_untrimmed != '\0')
	{
		if(!isspace(*str_untrimmed))
		{
			*str_trimmed = *str_untrimmed;
			str_trimmed++;
		}
		str_untrimmed++;
	}

	*str_trimmed = '\0';

}

Cell *CellGrid_search_by_grid_pos(const char *pos, CellGrid *cg)
{
	int i;
	int j;
	for (i = 0; i < cg->num_rows; i++) {
		for (j = 0; j < cg->num_cols; j++) {
			if (strcmp(cg->cells[i][j].grid_pos, pos) == 0)
				return &cg->cells[i][j];
		}

	}

	return NULL;
}


/* 
 * TODO: Evaluate expression
 * Simplitciy can only handle two operands
 */
Cell_eval CellGrid_eval_cell_expr(Cell *c, CellGrid *cg)
{
	char *trimmed = malloc(strlen(c->contents) + 1);
	remove_spaces(trimmed, c->contents);

	// Get the grid positions from the expr
	// This is not good (fix: use a better method)
	char operand1[3], operand2[3];
	char *begin = trimmed;
	char *operator = strchr(trimmed, '+');
	char *temp = operand1;
	while(begin != operator) {
		*temp = *begin;
		begin++;
		temp++;
	}
	*temp = '\0';


	temp = operand2;
	char *operand2_start = operator+1;
	strcpy(operand2, operand2_start);

	// printf("1: %s 2: %s\n", operand1, operand2);

	// Get corresponding cells for the grid pos
	Cell *c1 = CellGrid_search_by_grid_pos(operand1, cg);
	Cell *c2 = CellGrid_search_by_grid_pos(operand2, cg);
	Cell_eval ce;

	if (c1->cell_type == INT || c2->cell_type == INT) {
		ce.i = c1->cell_eval.i + c2->cell_eval.i;
		return ce;
	} else {
		ce.f = c1->cell_eval.f + c2->cell_eval.f;
		return ce;
	}

	ce.i = 0;
	return ce;


	// printf("expr: %s\n", trimmed);
}

float CellGrid_eval_cell_float(Cell *c)
{
	return atof(c->contents);
}

int CellGrid_eval_cell_int(Cell *c)
{
	return atoi(c->contents);		
}











