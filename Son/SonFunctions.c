#include "SonIncludesAndDefines.h"
#include "SonFunctions.h"

/*
Description: This function will calculate the result of the current small
			 expression that was passed from Father.
Parameters: char *str - the small expression we want to calculate, includes brackets.
Returns:	The return value is the result of the expression from Father, as int.
*/

int calc_expression(char *str) {

	int operator_index = 0,
		left_number = 0,
		right_number = 0,
		i = 1;

	char first_num[MAX_LEN_SON],
		second_num[MAX_LEN_SON],
		first_num_with_left_bracket[MAX_LEN_SON],
		expression_operator = "";

	//isolating of the left number (with the bracket).
	operator_index = find_operator_index(str);
	memset(first_num_with_left_bracket, '\0', sizeof(first_num));
	strncpy(first_num_with_left_bracket, str, operator_index);

	//loop for insert the '\0' to the end of the string.
	while (first_num_with_left_bracket[i] != '\0') {
		first_num[i - 1] = first_num_with_left_bracket[i];
		i++;
	}
	first_num[i] = '\0';
	left_number = atoi(first_num); //convert left number from string to int
	right_number = find_right_number(str, operator_index); //assigns to right_number the integer of right number.
	expression_operator = str[operator_index];

	//checks which operator should execute
	if (expression_operator == '+')
		return (left_number + right_number);
	if (expression_operator == '*')
		return (left_number * right_number);
	return -1; //return error!
}

/*
Description: This function finds the index where + or * is located in the input string.
Parameters: char *str - the small expression where the operator is, includes brackets.
Returns:	The return value is the index of the operator in the expression, as int.
*/
int find_operator_index(char *str) {
	int i = 0;
	for (i = 0; i < strlen(str); i++) {
		if (str[i] == '*' || str[i] == '+')
			return i;
	}
	return -1;//return error!
}

/*
Description: This function finds the second number (after the operator) and changes it from string to an integer
Parameters: char *str - the small expression, includes brackets.
				int operator_index
Returns:	The return value is the second number (right to the operator) in the expression, as int.
*/

int find_right_number(char *str, int operator_index) {
	int index = 0;
	char second_number[MAX_LEN_SON];
	while (operator_index < strlen(str)) {
		second_number[index] = str[operator_index + 1];
		index++;
		operator_index++;
	}
	return atoi(second_number);
}

int find_index_of_right_bracket(char *str) {
	int index_right = 0;
	while ((str[index_right] != ')') && (str[index_right] != '\0')) //loop until first ')' is found
		index_right++;

	if (str[index_right] == ')')
		return index_right;
}

int find_index_of_left_bracket(char *str, int index_left) {
	while (str[index_left] != '(') //loop back until first '(' is found from ')' index
		index_left--;
	return index_left;
}