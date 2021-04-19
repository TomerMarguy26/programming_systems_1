#include "FatherIncludesAndDefines.h"
#include "FatherFunctions.h"

/*
Description: This is the function that will be called from the main, each time when there's still a right bracket in
			 the expression.
Parameters: char *str - the current big expression. will be updated every process.
Returns:	No returns, the function is void.
*/

int Calculate_expression(char *str) {
	int remain_exp_index = 0, temp_left_last_index = 0, son_exit_code = 0;
	char residue_big_expression_left[MAX_LEN] = "", small_expression[MAX_LEN] = ""; //small_expression will be sent to Son proc.
	char calc_val[MAX_LEN_SON]; //calc_val will contain the value of the calculated middle (small) expression
	if (write_expression_to_file(str) == -1) { //write the current expression to file (before calculating)
		return -1;
	}

	update_left_expression(str, residue_big_expression_left);
	find_first_expression(str, small_expression);
	son_exit_code = CreateProcessSimpleMain(small_expression);//Begin a new process to the son with argument "small_expression",
													//the return value from the son will be the calculated number in int.
	itoa(son_exit_code, calc_val, 10); //put into calc_val the calculated value of small exp
	strcat(residue_big_expression_left, calc_val); //combine left exp with small exp calc
	remain_exp_index = find_index_of_right_bracket(str) + 1; //what left from the father exp - right to calculation

	while (residue_big_expression_left[temp_left_last_index] != '\0') //find the end of temp_left_last_index
		temp_left_last_index++;
	while (str[remain_exp_index] != '\0') {
		residue_big_expression_left[temp_left_last_index] = str[remain_exp_index];
		remain_exp_index++;
		temp_left_last_index++;
	}

	strcpy(str, residue_big_expression_left); //updates str

	// reset all values for next itration
	memset(residue_big_expression_left, 0, strlen(residue_big_expression_left));
	remain_exp_index = 0;
	temp_left_last_index = 0;
	return 0;
}

/*
Description: This function will fetch the first small expression that we want to calculate from the big expression, including brackets
Parameters: char *str - the big expression of this round
			char *small_expression - the small expression will extract from the big one (str) into *small_expression.
Returns:	No returns, the function is void.
*/

void find_first_expression(char *str, char *small_expression_to_memory) {
	int index_of_right_bracket = 0, index_of_left_bracket = 0, i = 0, small_expression_length = 0;
	char small_expression[MAX_LEN] = "";
	index_of_right_bracket = find_index_of_right_bracket(str); //find first ')'
	index_of_left_bracket = find_index_of_left_bracket(str, index_of_right_bracket); // find first '(' reverse from ')'
	small_expression_length = index_of_right_bracket - index_of_left_bracket;
	memset(small_expression_to_memory, '\0', sizeof(small_expression_to_memory)); 
	strncpy(small_expression_to_memory, str, index_of_right_bracket + 1);

	//Cut just the numbers, opreator and '(', ')' from the sub string
	for (i = 0; i < small_expression_length + 1; i++) {
		small_expression_to_memory[i] = small_expression_to_memory[index_of_left_bracket];
		index_of_left_bracket++;
	}
	small_expression_to_memory[i + 1] = '\0';
}

void update_left_expression(char *str, char *new_str) {
	strncpy(new_str, str, find_index_of_left_bracket(str, find_index_of_right_bracket(str)));// copies to the new string the expression left, left to the first calc
}

int write_expression_to_file(char *str) {
	FILE  *fp = fopen("Computation.txt", "a");
	if (NULL == fp)
	{
		printf("Failed to open file. \n");
		return -1; //in case of failure the function will return -1
	}
	else {
		fprintf(fp, "%s%c", str, '\n');
		fclose(fp);
		return 0;
	}
}

int find_index_of_right_bracket(char *str) {
	int index_right = 0;
	while ((str[index_right] != ')') && (str[index_right] != '\0')) //loop until first ')' is found
		index_right++;

	if (str[index_right] == ')')
		return index_right;
	else //write to file the last result, a number without brackets, last write!
	{
		if (write_expression_to_file(str) == -1) {
			return -1;
		}
		else
			return 0;
	}
}

int find_index_of_left_bracket(char *str, int index_left) {
	while (str[index_left] != '(') //loop back until first '(' is found from ')' index
		index_left--;
	return index_left;
}


/*
Description: This functions were taken from the recitation files. These functions will
			 calculate the result of the current small expression that was passed from Father.
Parameters: char *small_expression - the small expression we want to calculate, includes brackets.
Returns:	The return value is the result of the expression from Father, as int.
*/

int CreateProcessSimpleMain(char *small_expression)
{
	PROCESS_INFORMATION procinfo;
	DWORD				waitcode;
	DWORD				exitcode;
	BOOL				retVal;
	CHAR				command[MAX_LEN_SON + 8] = ""; /* <ISP> TCHAR is a win32  */
	sprintf(command, "Son.exe %s", small_expression);	/* generic char which may be either a simple (ANSI) char or a unicode char, */
	command[strlen(command)] = '\0';									/* depending on behind-the-scenes operating system definitions. Type LPTSTR */
													/* is a string of TCHARs. Type LPCTSTR is a const string of TCHARs. */

													/*  Start the child process. */
	retVal = CreateProcessSimple(command, &procinfo);

	if (retVal == 0)
	{
		printf("Process Creation Failed!\n");
		return;
	}

	waitcode = WaitForSingleObject(
		procinfo.hProcess,
		TIMEOUT_IN_MILLISECONDS); /* Waiting 2.5 secs for the process to end */

	printf("WaitForSingleObject output: ");
	switch (waitcode)
	{
	case WAIT_TIMEOUT:
		printf("WAIT_TIMEOUT\n"); break;
	case WAIT_OBJECT_0:
		printf("WAIT_OBJECT_0\n"); break;
	default:
		printf("0x%x\n", waitcode);
	}

	if (waitcode == WAIT_TIMEOUT) /* Process is still alive */
	{
		printf("Process was not terminated before timeout!\n"
			"Terminating brutally!\n");
		TerminateProcess(
			procinfo.hProcess,
			BRUTAL_TERMINATION_CODE); /* Terminating process with an exit code of 55h */
		Sleep(10); /* Waiting a few milliseconds for the process to terminate */
	}

	GetExitCodeProcess(procinfo.hProcess, &exitcode);

	printf("The exit code for the process is 0x%x\n", exitcode);

	CloseHandle(procinfo.hProcess); /* Closing the handle to the process */
	CloseHandle(procinfo.hThread); /* Closing the handle to the main thread of the process */
	return (int)exitcode;
}

/*oOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoOoO*/

BOOL CreateProcessSimple(LPTSTR CommandLine, PROCESS_INFORMATION *ProcessInfoPtr)
{
	STARTUPINFO	startinfo = { sizeof(STARTUPINFO), NULL, 0 }; /* <ISP> here we */
															  /* initialize a "Neutral" STARTUPINFO variable. Supplying this to */
															  /* CreateProcess() means we have no special interest in this parameter. */
															  /* This is equivalent to what we are doing by supplying NULL to most other */
															  /* parameters of CreateProcess(). */

	return CreateProcess(NULL, /*  No module name (use command line). */
		CommandLine,			/*  Command line. */
		NULL,					/*  Process handle not inheritable. */
		NULL,					/*  Thread handle not inheritable. */
		FALSE,					/*  Set handle inheritance to FALSE. */
		NORMAL_PRIORITY_CLASS,	/*  creation/priority flags. */
		NULL,					/*  Use parent's environment block. */
		NULL,					/*  Use parent's starting directory. */
		&startinfo,				/*  Pointer to STARTUPINFO structure. */
		ProcessInfoPtr			/*  Pointer to PROCESS_INFORMATION structure. */
	);
}