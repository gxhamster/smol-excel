#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

#include "CellGrid.h"
#include "Stack.h"

void CellGrid_print(CellGrid *cg)
{
	size_t i;
	size_t j;
	for (i = 0; i < cg->num_rows; i++) {
		for (j = 0; j < cg->num_cols; j++) {
			if (cg->cells[i][j].cell_type == EXPR_INT) {
				printf("%s: %d ", cg->cells[i][j].grid_pos,
						cg->cells[i][j].cell_eval.i);
			} else if (cg->cells[i][j].cell_type == EXPR_FLOAT) {
				printf("%s: %.2f ", cg->cells[i][j].grid_pos,
						cg->cells[i][j].cell_eval.f);
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


// Call this function before parsing expr
CellGrid *CellGrid_parse_numbers(CellGrid *cg)
{

	size_t i;
	size_t j;
	Cell *cur_cell;
	for (i = 0; i < cg->num_rows; i++) {
		for (j = 0; j < cg->num_cols; j++) {
			cur_cell = &cg->cells[i][j];
			// Set evaluated value of each cell
			// Also set the type
			if (CellGrid_cell_is_int(cur_cell)) {
				cur_cell->cell_type = INT;
				cur_cell->cell_eval.i = CellGrid_eval_cell_int(cur_cell);
			} else if (CellGrid_cell_is_float(cur_cell)) {
				cur_cell->cell_type = FLOAT;
				cur_cell->cell_eval.f = CellGrid_eval_cell_float(cur_cell);
			}
		}
	}

	return cg;
}

// parses the expr in the grid
// call after calling CellGrid_parse_numbers()
CellGrid *CellGrid_parse_expr(CellGrid *cg)
{
	size_t i;
	size_t j;
	Cell *cur_cell;

	for (i = 0; i < cg->num_rows; i++) {
		for (j = 0; j < cg->num_cols; j++) {
			cur_cell = &cg->cells[i][j];
			if (CellGrid_cell_is_expr(cur_cell)) {
                cur_cell->cell_type = EXPR;
			}
		}
	}

	return cg;
}

// function definition of the revstr()  
void reverse_str(char *str1)  
{  
    // declare variable  
    int i, len, temp;  
    len = strlen(str1); // use strlen() to get the length of str string  
      
    // use for loop to iterate the string   
    for (i = 0; i < len/2; i++)  
    {  
        // temp variable use to temporary hold the string  
        temp = str1[i];  
        str1[i] = str1[len - i - 1];  
        str1[len - i - 1] = temp;  
    }  
}  

// Function to print Excel column name for a given column number
char *CellGrid_get_grid_pos(int col, int row)
{
    char str[MAX]; // To store result (Excel column name)
    int i = 0; // To store current index in str which is result
 
    while (col > 0) {
        // Find remainder
        int rem = col % 26;
 
        // If remainder is 0, then a 'Z' must be there in output
        if (rem == 0) {
            str[i++] = 'Z';
            col = (col / 26) - 1;
        }
        else // If remainder is non-zero
        {
            str[i++] = (rem - 1) + 'A';
            col = col / 26;
        }
    }
    str[i] = '\0';
 
    // Reverse the string and print result
    reverse_str(str);
    
    // Add row number
    char row_str[20];
    sprintf(row_str, "%d", row);
    strcat(str, row_str);
    
    char *result = strdup(str);
    return result;
}

// only parses the string content
CellGrid *CellGrid_read_from_csv(const char *file_path, char delim)
{
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
		if (cols > GRID_MAX) {
			fprintf(stderr, "ERROR: Number of columns is greater than maximum allowed");
			exit(-1);
		}
		cg->num_cols = cols;
		size_t i;
		Cell *cur_cell;
		for (i = 0; i < cols; i++) {
			cur_cell = &cg->cells[rows][i];
			cur_cell->contents = getfield(temp_line, i, delim);

			// Set cell position on grid 
			cur_cell->grid_pos = CellGrid_get_grid_pos(i+1, rows+1);
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

void remove_spaces(char* str_trimmed, const char* str_untrimmed)
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
	size_t i;
	size_t j;
	for (i = 0; i < cg->num_rows; i++) {
		for (j = 0; j < cg->num_cols; j++) {
			if (strcmp(cg->cells[i][j].grid_pos, pos) == 0)
				return &cg->cells[i][j];
		}
	}

	return NULL;
}


// Alert: Function only handles addition expr
Cell_eval CellGrid_eval_cell_expr1(Cell *c, CellGrid *cg)
{

	// A1+B2-C2
	// 1: A1 B2 C2
	// 2: + -
	char *found;
	char *src_str = c->contents;
	char *temp = strdup(src_str);



	Stack *operands = create_stack();
	Stack *operators = create_stack();

	while ( (found = strsep(&temp, "+-*/")) != NULL) {
		stack_push(operands, (void *)found);
		// printf("found: %s\n", found);
	}


	int i;
	int j;
	for (i = 0; i < strlen(src_str); i++) {
		if (is_operator(src_str[i])) {
			stack_push(operators, (void *)&src_str[i]);
		}
	}

	float res;
	size_t len = operands->size;
	int num_of_floats;
	int num_of_ints;

	for (i = 0, j = 0; i < len; i++) {
		Cell *c = CellGrid_search_by_grid_pos((char *)stack_pop(operands), cg);
		// Also set the cell type
		switch (c->cell_type) {
			case INT:
				res += c->cell_eval.i;
				num_of_ints++;
				break;
			case FLOAT:
				res += c->cell_eval.f;
				num_of_floats++;
				break;
			case EXPR_FLOAT:
				res += c->cell_eval.f;
				num_of_floats++;
				break;
			case EXPR_INT:
				res += c->cell_eval.i;
				num_of_ints++;
				break;
		}
	}

	printf("result: %f\n" , res);
	free(temp);

	Cell_eval ce;
	if (num_of_floats == 0)
		ce.i = res;
	else
		ce.f = res;

	return ce;
}


/*
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

	// A1+B1+C3

	// Get corresponding cells for the grid pos
	Cell *c1 = CellGrid_search_by_grid_pos(operand1, cg);
	Cell *c2 = CellGrid_search_by_grid_pos(operand2, cg);
	Cell_eval ce;

	enum CellTypes type1;
	enum CellTypes type2;
	type1 = c1->cell_type;
	type2 = c2->cell_type;

	// TODO: Handle different operators
	// TODO: Handle multiple operands and operators

	// This is bad
	if (type1 == EXPR || type2 == EXPR) {
		fprintf(stderr, "Cannot Handle expressions pointing to expressions yet\n");
		exit(-1);
	} else if (type1 == INT && type2 == INT) {
		c->cell_type = EXPR_INT;
		ce.i = c1->cell_eval.i + c2->cell_eval.i;
		return ce;
	} else if (type1 == FLOAT && type2 == INT) {
		c->cell_type = EXPR_FLOAT;
		ce.f = c1->cell_eval.f + c2->cell_eval.i;
		return ce;
	} else if (type1 == INT && type2 == FLOAT) {
		c->cell_type = EXPR_FLOAT;
		ce.f = c1->cell_eval.i + c2->cell_eval.f;
		return ce;
	} else {
		c->cell_type = EXPR_FLOAT;
		ce.f = c1->cell_eval.f + c2->cell_eval.f;
		return ce;
	}
}

CellGrid *CellGrid_eval_cells(CellGrid *cg)
{

	size_t i;
	size_t j;
	Cell *cur_cell;

	for (i = 0; i < cg->num_rows; i++) {
		for (j = 0; j < cg->num_cols; j++) {
			cur_cell = &cg->cells[i][j];
			if (cur_cell->cell_type == EXPR) {
				cur_cell->cell_eval = CellGrid_eval_cell_expr(cur_cell, cg);
			}
		}
	}
	return cg;
}

bool is_operator(char c)
{
	switch(c) {
		case '+':
		case '-':
		case '*':
		case '/':
			return true;
		default:
			return false;
	}
}

float CellGrid_eval_cell_float(Cell *c)
{
	return atof(c->contents);
}

int CellGrid_eval_cell_int(Cell *c)
{
	return atoi(c->contents);
}
