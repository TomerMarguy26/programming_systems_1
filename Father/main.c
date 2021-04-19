/*
Authors: Tomer Marguy...........id_number: 205705874
	Gavriel Rashkovsky.....id_number: 312178999
Project name: "Father"
*/

#include "FatherIncludesAndDefines.h"
#include "FatherFunctions.h"

/*
Description: This is the main function of Father, This is the first code to run.
			 argv[1] will be the inital expression that we want to calculate.
			 The file with the calculation proccess will  be printed and appear in Father folder.
Parameters: int argc- the number of strings pointed to by argv.
			char *argv[] - an array of strings representing the individual arguments provided on the command line.
Returns:	exit program with 0 if everything went ok.
*/

int main(int argc, char *argv[]) {
	char str_in[MAX_LEN]; //the inital big expression
	strcpy(str_in, argv[1]); //put the big expression into str_in
	
	while (find_index_of_right_bracket(str_in) > 0) { //when there is no ')' in str_in, find_index_of_right_bracket will return 0.
		if (Calculate_expression(str_in) == -1) { //if there were no problems with file writing 
			break;
		}
	}

	return 0;
}