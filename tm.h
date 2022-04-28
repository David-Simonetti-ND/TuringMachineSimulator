#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>

typedef struct transition
{
    char *start_state;
    char input_symbol;
    char *end_state;
    char output_symbol;
    char direction;
} transition;

typedef struct turing_machine
{
    char **states;
    int number_of_states;
    char **input_symbols;
    int number_of_input_symbls;
    char **tape_symbols;
    int number_of_tape_symbols;
    char *start_state;
    char *accept_state;
    char *reject_state;
    transition *transitions;
    int number_of_transitions;
    transition ***transition_table;
} TM;

void read_in_tm_from_file(TM *t, char *file_name);
int split_string(char **dest, char *source, char delim);