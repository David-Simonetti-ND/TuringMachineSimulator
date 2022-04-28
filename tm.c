#include "tm.h"

void display_tm(TM *t)
{
    for (int i = 0; i < t->number_of_states; i++)
    {
        printf("%s ", t->states[i]);
    }
    printf("\n");
    for (int i = 0; i < t->number_of_input_symbls; i++)
    {
        printf("%s ", t->input_symbols[i]);
    }
    printf("\n");
    for (int i = 0; i < t->number_of_tape_symbols; i++)
    {
        printf("%s ", t->tape_symbols[i]);
    }
    printf("\n");
    printf("%s\n", t->start_state);
    printf("%s\n", t->accept_state);
    printf("%s\n", t->reject_state);
    for (int i = 0; i < t->number_of_states; i++)
    {
        for (int j = 0; j < t->number_of_tape_symbols; j++)
        {
            if (t->transition_table[i][j] == NULL) continue;
            printf("%s %c %s %c %c\n", 
            t->transition_table[i][j]->start_state,
            t->transition_table[i][j]->input_symbol,
            t->transition_table[i][j]->end_state,
            t->transition_table[i][j]->output_symbol,
            t->transition_table[i][j]->direction);
        }
    }
}

void free_tm(TM *t)
{
    for (int i = 0; i < t->number_of_states; i++)
    {
        free(t->states[i]);
    }
    free(t->states);
    for (int i = 0; i < t->number_of_input_symbls; i++)
    {
        free(t->input_symbols[i]);
    }
    free(t->input_symbols);
    for (int i = 0; i < t->number_of_tape_symbols; i++)
    {
        free(t->tape_symbols[i]);
    }
    free(t->tape_symbols);
    free(t->start_state);
    free(t->accept_state);
    free(t->reject_state);
    for (int i = 0; i < t->number_of_transitions; i++)
    {
        free(t->transitions[i].start_state);
        free(t->transitions[i].end_state);
    }
    free(t->transitions);
    for (int i = 0; i < t->number_of_states; i++)
    {
        free(t->transition_table[i]);
    }
    free(t->transition_table);
}

int count_elements(char *string, char delim)
{
    int num_symbols = 1;
    while (*string != '\0')
    {
        if (*string == delim)
        {
            num_symbols++;
        }
        string++;
    }
    return num_symbols;
}

int split_string(char **dest, char *source, char delim)
{
    char *base = source;
    int num_chars = 0;
    int dest_index = 0;
    while (*source != '\0')
    {
        if (*source == delim)
        {
            dest[dest_index] = malloc(sizeof(char) *  (num_chars + 1));
            strncpy(dest[dest_index], base, num_chars);
            source++;
            base = source;
            num_chars = 0;
            dest_index++;
        }
        num_chars++;
        source++;
    }
    dest[dest_index] = malloc(sizeof(char) *  (num_chars + 1));
    strncpy(dest[dest_index], base, num_chars);
    return dest_index + 1;
}

void create_transition(transition* trans, char *source)
{
    char *temp = strtok(source, " ");
    trans->start_state = malloc(sizeof(char) * strlen(temp));
    strcpy(trans->start_state, temp);
    
    trans->input_symbol = strtok(NULL, " ")[0];

    temp = strtok(NULL, " ");
    trans->end_state = malloc(sizeof(char) * strlen(temp));
    strcpy(trans->end_state, temp);

    trans->output_symbol = strtok(NULL, " ")[0];

    trans->direction = strtok(NULL, " ")[0];
}

void read_in_tm_from_file(TM *t, char *file_name)
{
    FILE *tm_description = fopen(file_name, "r");
    if (tm_description == NULL)
    {
        fprintf(stderr, "Error: unable to open input file %s %s\n", file_name, strerror(errno));
        exit(1);
    }
    
    char buffer[4096]; 
    fgets(buffer, 4096, tm_description);
    buffer[strcspn(buffer, "\n")] = '\0';
    
    t->number_of_states = count_elements(buffer, ' ');
    t->states = malloc(sizeof(char *) * t->number_of_states);
    split_string(t->states, buffer, ' ');

    fgets(buffer, 4096, tm_description);
    buffer[strcspn(buffer, "\n")] = '\0';

    t->number_of_input_symbls = count_elements(buffer, ' ');
    t->input_symbols = malloc(sizeof(char *) * t->number_of_input_symbls);
    split_string(t->input_symbols, buffer, ' ');

    fgets(buffer, 4096, tm_description);
    buffer[strcspn(buffer, "\n")] = '\0';

    t->number_of_tape_symbols = count_elements(buffer, ' ');
    t->tape_symbols = malloc(sizeof(char *) * t->number_of_tape_symbols);
    split_string(t->tape_symbols, buffer, ' ');

    fgets(buffer, 4096, tm_description);
    buffer[strcspn(buffer, "\n")] = '\0';
    t->start_state = malloc(sizeof(char) * strlen(buffer));
    strcpy(t->start_state, buffer);

    fgets(buffer, 4096, tm_description);
    buffer[strcspn(buffer, "\n")] = '\0';
    t->accept_state = malloc(sizeof(char) * strlen(buffer));
    strcpy(t->accept_state, buffer);

    fgets(buffer, 4096, tm_description);
    buffer[strcspn(buffer, "\n")] = '\0';
    t->reject_state = malloc(sizeof(char) * strlen(buffer));
    strcpy(t->reject_state, buffer);

    int current_pos = ftell(tm_description);
    int number_of_transitions = 0;
    int current_transition = 0;
    while (1)
    {
        fgets(buffer, 4096, tm_description);
        if (feof(tm_description))
            break;
        number_of_transitions++;
    }
    fseek(tm_description, current_pos, SEEK_SET);

    t->transitions = malloc(sizeof(transition) * number_of_transitions);
    t->number_of_transitions = number_of_transitions;
    while (1)
    {
        fgets(buffer, 4096, tm_description);
        if (feof(tm_description))
            break;
        buffer[strcspn(buffer, "\n")] = '\0';
        create_transition(&t->transitions[current_transition], buffer);
        current_transition++;
    }

    fclose(tm_description);
}

int return_index_from_symbol(TM *t, char symbol)
{
    int index = 0;
    for (index = 0; index < t->number_of_tape_symbols; index++)
    {
        if (symbol == t->tape_symbols[index][0])
        {
            return index;
        }
    }
    return -1;
}

int return_index_from_state(TM *t, char *state)
{
    int index = 0;
    for (index = 0; index < t->number_of_states; index++)
    {
        if (!strcmp(state, t->states[index]))
        {
            return index;
        }
    }
    return -1;
}

void create_transition_table(TM *t)
{
    t->transition_table = malloc(sizeof(transition **) * t->number_of_states);
    for (int i = 0; i < t->number_of_states; i++)
    {
        t->transition_table[i] = malloc(sizeof(transition *) * t->number_of_tape_symbols);
        memset(t->transition_table[i], 0, t->number_of_tape_symbols * sizeof(transition *));
    }
    for (int i = 0; i < t->number_of_transitions; i++)
    {
        int state_index = return_index_from_state(t, t->transitions[i].start_state);
        int symbol_index = return_index_from_symbol(t, t->transitions[i].input_symbol);
        t->transition_table[state_index][symbol_index] = &t->transitions[i];
    }
}

int simulate_tm(TM *t, char *tape)
{
    char *current_state = t->start_state;
    int tape_index = 0;
    char current_symbol = tape[tape_index];

    while (strcmp(current_state, t->accept_state))
    {
        int state_index = return_index_from_state(t, current_state);
        int symbol_index = return_index_from_symbol(t, current_symbol);
        transition *trans = t->transition_table[state_index][symbol_index];
        if (trans == NULL)
        {
            return 1;
        }
        printf("%s\n", tape);
        tape[tape_index] = trans->output_symbol;
        if (trans->direction == 'R') tape_index++;
        else tape_index--;
        current_state = trans->end_state;
        current_symbol = tape[tape_index];
    }
    return 0;
}

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        fprintf(stderr, "Error: please only enter two arguments which are the input file and the input string\n");
        exit(1);
    }

    TM t;
    read_in_tm_from_file(&t, argv[1]);
    create_transition_table(&t);
    display_tm(&t);
    char *tape = malloc(sizeof(char) * (strlen(argv[2]) + 2));
    strcpy(tape, argv[2]);
    strcat(tape, "_");
    int ret_val = simulate_tm(&t, tape);
    if (ret_val == 0)
    {
        printf("Turing machine accepted input %s\n", argv[2]);
    }
    else
    {
        printf("Turing machine rejected input %s\n", argv[2]);
    }
    free(tape);
    free_tm(&t);
    
    return 0;
}