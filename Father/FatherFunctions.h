#pragma once
void find_first_expression(char *str, char *temp);
void update_left_expression(char *str, char *new_str);
int write_expression_to_file(char *str);
int Calculate_expression(char *str);
int find_index_of_right_bracket(char *str);
int find_index_of_left_bracket(char *str, int index_left);
int CreateProcessSimpleMain(char *str);
BOOL CreateProcessSimple(LPTSTR CommandLine, PROCESS_INFORMATION *ProcessInfoPtr);