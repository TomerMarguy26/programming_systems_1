/*
Authors: Tomer Marguy...........id_number: 205705874
		 Gavriel Rashkovsky.....id_number: 312178999
Project name: "Son" 
*/

#include "SonIncludesAndDefines.h"
#include "SonFunctions.h"

/*
Description: This is the main function of Son, It will execute when Father will create a new process.
			 Father will pass the current expression we want to calculate into argv[1] via the command line.
Parameters: int argc- the number of strings pointed to by argv.
			char *argv[] - an array of strings representing the individual arguments provided on the command line.
Returns:	The return value will be the final result of the small expression that we got from Father, as int.
*/

int main(int argc, char *argv []) {

	char str_in[MAX_LEN_SON]; 
	strcpy(str_in, argv[1]);
	return calc_expression(str_in);

	return -1; //return error!
}