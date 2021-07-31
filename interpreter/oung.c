#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define TRUE 1
#define FALSE 0
#define GOOD 1
#define BAD 0
#define LINE_LIMIT 80
#define STMT_LIMIT 256

enum token_t {
    ZERO,
    INPUT,
    EQUAL,
    BYTE,
    ADD,
    SUB,
    MUL,
    DIV,
    VAR,
    PRINT,
    OR,
    AND,
    SEP,
    SLEEP,
    END,
    ONE,
    ERROR
};

enum token_t convert_oung_to_inst(const char oung[4]);
int eval(enum token_t cmd[STMT_LIMIT], unsigned int limit);

int allocate_var(char* identifier, unsigned int id_len, char value);
int deallocate_var(char* identifier);

int is_id_valid(const char* identifier);
struct variable_t* get_variable(const char* identifier);

int oung_input(enum token_t cmd[STMT_LIMIT], unsigned int limit);
int oung_init_var(enum token_t cmd[STMT_LIMIT], unsigned int limit);
int oung_add(enum token_t cmd[STMT_LIMIT], unsigned int limit);
int oung_subtract(enum token_t cmd[STMT_LIMIT], unsigned int limit);
int oung_multiply(enum token_t cmd[STMT_LIMIT], unsigned int limit);
int oung_divide(enum token_t cmd[STMT_LIMIT], unsigned int limit);
int oung_print(enum token_t cmd[STMT_LIMIT], unsigned int limit);
int oung_or(enum token_t cmd[STMT_LIMIT], unsigned int limit);
int oung_and(enum token_t cmd[STMT_LIMIT], unsigned int limit);
int oung_sleep(enum token_t cmd[STMT_LIMIT], unsigned int limit);

void traverse_forward();
void traverse_backward();

struct variable_t {
    char* identifier;
    char value;
    struct variable_t* prev;
    struct variable_t* next;
};

struct variable_t* var_list_head = NULL;
struct variable_t* var_list_tail = NULL;

int main(int argv, char** argc) {

    if (argv != 2) {
        printf("Proper command usage : oung [source_name]");
        return 1;
    }

    const char* file_name = argc[1];
    FILE* source = fopen(file_name, "r");

    // scan each line
    char line[LINE_LIMIT];
    enum token_t cmd[STMT_LIMIT];
    unsigned int cmd_pos = 0;

    while (fgets(line, LINE_LIMIT, source) != NULL) {
        // remove new lines
        line[strcspn(line, "\n")] = 0;

        char delim[ ] = " ";
        char *ptr = strtok(line, delim);
        while (ptr != NULL)
        {
            enum token_t inst = convert_oung_to_inst(ptr);

            if (inst == END) {
                eval(cmd, cmd_pos);
                cmd_pos = 0;
            }

            else {
                cmd[cmd_pos++] = inst;
            }

            ptr = strtok(NULL, delim);
        }
    }

    return 0;
}

enum token_t convert_oung_to_inst(const char oung[4]) {
    const char* oungs[16] = {
        "oung", "ounG", "ouNg", "ouNG",
        "oUng", "oUnG", "oUNg", "oUNG",
        "Oung", "OunG", "OuNg", "OuNG",
        "OUng", "OUnG", "OUNg", "OUNG"
    };

    for (int i = 0; i < 16; i++) {
        if (!strcmp(oung, oungs[i])) {
            return (enum token_t)i;
        }
    }

    return ERROR;
}

// evaluate the statement
int eval(enum token_t cmd[STMT_LIMIT], unsigned int limit) {
    int status_code = GOOD;

    // good cmd
    if (limit > 0) {
        switch (cmd[0]) {
            case INPUT:
                status_code = oung_input(&cmd[1], limit-1);
                break;
            case BYTE:
                status_code = oung_init_var(&cmd[1], limit-1);
                break;
            case ADD:
                status_code = oung_add(&cmd[1], limit-1);
                break;
            case SUB:
                status_code = oung_subtract(&cmd[1], limit-1);
                break;
            case MUL:
                status_code = oung_multiply(&cmd[1], limit-1);
                break;
            case DIV:
                status_code = oung_divide(&cmd[1], limit-1);
                break;
            case PRINT:
                status_code = oung_print(&cmd[1], limit-1);
                break;
            case OR:
                status_code = oung_or(&cmd[1], limit-1);
                break;
            case AND:
                status_code = oung_and(&cmd[1], limit-1);
                break;
            default:
                status_code = BAD;
        }
    }

    // bad cmd size
    else {
        status_code = BAD;
    }

    return status_code;
}

int oung_print(enum token_t cmd[STMT_LIMIT], unsigned int limit) {
    int status_code = GOOD;

    if (cmd[0] == VAR) {
        int id_len = 0;
        for (int i = 1; cmd[i] != VAR && i < limit; i += 8) {
            id_len++;
        }

        char id[id_len];

        for (int i = 0; i < id_len; i++) {
            for (int j = 0; j < 8; j++) {
                enum token_t cur_token = cmd[i*8+j+1];

                id[i] <<= 1;
            
                if (cur_token == ONE) {
                    id[i] |= 1;
                }

                else if (cur_token != ZERO) {
                    printf("Given identifier is not a completed byte!");
                    return BAD;
                }
            }
        }

        printf("%s\n", id);
        // traverse_forward();

        struct variable_t* var = get_variable(id);
        if (var == NULL) {
            printf("Could not find the variable! "
                "Maybe it was deleted by OVOT policy.");
            return BAD;
        }

        printf("%c", var->value);
        status_code = deallocate_var(var->identifier);
    }

    else if (cmd[0] == ONE || cmd[0] == ZERO) {
        char printed_char = 0;

        for (int i = 0; i < limit; i++) {
            printed_char <<= 1;
            
            if (cmd[i] == ONE) {
                printed_char |= 1;
            }

            else if (cmd[i] != ZERO) {
                status_code = BAD;
                break;
            }
        }

        printf("%c", printed_char);
    }

    else {
        status_code = BAD;
    }

    return status_code;
}

int allocate_var(char* identifier, unsigned int id_len, char value) {
    if (!is_id_valid(identifier)) {
        return BAD;
    }

    struct variable_t* new_var = (struct variable_t*)malloc(sizeof(struct variable_t));
    if (new_var == NULL) {
        return BAD;
    }

    new_var->identifier = (char*)malloc(sizeof(char)*id_len);
    if (new_var->identifier == NULL) {
        return BAD;
    }

    strncpy(new_var->identifier, identifier, id_len);

    new_var->value = value;

    new_var->prev = var_list_tail;
    new_var->next = NULL;

    // initialize the list
    if (var_list_head == NULL) {
        var_list_head = new_var;
    }

    // append the list
    else {
        var_list_tail->next = new_var;
    }

    // move the tail pointer
    var_list_tail = new_var;

    return GOOD;
}

int is_id_valid(const char* identifier) {
    int status_code = GOOD;

    struct variable_t* cur = var_list_head;
    while (cur != NULL) {
        if (strcmp(cur->identifier, identifier) == 0) {
            status_code = BAD;
            break;
        }

        cur = cur->next;
    }

    return status_code;
}

struct variable_t* get_variable(const char* identifier) {
    struct variable_t* cur = var_list_head;
    while (cur != NULL) {
        if (strcmp(cur->identifier, identifier) == 0) {
            return cur;
        }

        cur = cur->next;
    }

    return NULL;
}

void traverse_forward() {
    struct variable_t* cur = var_list_head;
    while (cur != NULL) {
        printf("%s\n", cur->identifier);
        cur = cur->next;
    }
}

void traverse_backward() {
    struct variable_t* cur = var_list_tail;
    while (cur != NULL) {
        printf("%s\n", cur->identifier);
        cur = cur->prev;
    }
}

int deallocate_var(char* identifier) {
    struct variable_t* cur = var_list_head;
    while (cur != NULL) {
        if (strcmp(cur->identifier, identifier) == 0) {
            // case [X] <-> [] -> 
            if (cur == var_list_head) {
                var_list_head = cur->next;

                if (cur->next != NULL) {
                    cur->next->prev = NULL;
                }
            }

            // case -> [] <-> [X] ->
            else if (cur == var_list_tail) {
                var_list_tail = cur->prev;

                if (cur->prev != NULL) {
                    cur->prev->next = NULL;
                }
            }

            // case -> [] <-> [X] <-> [] ->
            else {
                cur->prev->next = cur->next;
                cur->next->prev = cur->prev;
            }

            free(cur->identifier);
            free(cur);
            break;
        }

        cur = cur->next;
    }

    return BAD;
}

int oung_init_var(enum token_t cmd[STMT_LIMIT], unsigned int limit) {
    int status_code = GOOD;

    if (cmd[0] != VAR) {
        status_code = BAD;
    }

    else {
        // find the bound
        int id_len = 0;
        for (int i = 1; cmd[i] != VAR && i < limit; i += 8) {
            id_len++;
        }

        char id[id_len];

        for (int i = 0; i < id_len; i++) {
            for (int j = 0; j < 8; j++) {
                enum token_t cur_token = cmd[i*8+j+1];

                id[i] <<= 1;
            
                if (cur_token == ONE) {
                    id[i] |= 1;
                }

                else if (cur_token != ZERO) {
                    return BAD;
                }
            }
        }

        char value = 0;
        for (int i = id_len*8+2; i < limit; i++) {
            value <<= 1;
            
            if (cmd[i] == ONE) {
                value |= 1;
            }

            else if (cmd[i] != ZERO) {
                status_code = BAD;
                break;
            }
        }

        status_code = allocate_var(id, id_len, value);
    }

    return status_code;
}

int oung_input(enum token_t cmd[STMT_LIMIT], unsigned int limit) {
    int status_code = GOOD;

    char input;
    scanf(" %c", &input);

    if (cmd[0] != VAR) {
        status_code = BAD;
    }

    else {
        int id_len = 0;
        for (int i = 1; cmd[i] != VAR && i < limit; i += 8) {
            id_len++;
        }

        char id[id_len];

        for (int i = 0; i < id_len; i++) {
            for (int j = 0; j < 8; j++) {
                enum token_t cur_token = cmd[i*8+j+1];

                id[i] <<= 1;
            
                if (cur_token == ONE) {
                    id[i] |= 1;
                }

                else if (cur_token != ZERO) {
                    return BAD;
                }
            }
        }

        status_code = allocate_var(id, id_len, input);
    }

    return status_code;
}

int oung_add(enum token_t cmd[STMT_LIMIT], unsigned int limit) {
    int status_code = GOOD;

    if (cmd[0] != VAR) {
        status_code = BAD;
    }

    else {
        int id_len = 0;
        for (int i = 1; cmd[i] != VAR && i < limit; i += 8) {
            id_len++;
        }

        char l_operand[id_len];

        for (int i = 0; i < id_len; i++) {
            for (int j = 0; j < 8; j++) {
                enum token_t cur_token = cmd[i*8+j+1];

                l_operand[i] <<= 1;
            
                if (cur_token == ONE) {
                    l_operand[i] |= 1;
                }

                else if (cur_token != ZERO) {
                    return BAD;
                }
            }
        }

        if (cmd[id_len*8+2] != SEP) {
            return BAD;
        }

        else if (cmd[id_len*8+3] == VAR) {
            if (cmd[limit-1] != VAR) {
                return BAD;
            }

            int r_id_len = 0;
            for (int i = id_len*8+4; cmd[i] != VAR && i < limit; i += 8) {
                r_id_len++;
            }

            char r_operand[r_id_len];

            for (int i = 0; i < r_id_len; i++) {
                for (int j = 0; j < 8; j++) {
                    enum token_t cur_token = cmd[(id_len*8+4)+i*8+j];

                    r_operand[i] <<= 1;
                
                    if (cur_token == ONE) {
                        r_operand[i] |= 1;
                    }

                    else if (cur_token != ZERO) {
                        return BAD;
                    }
                }
            }

            struct variable_t* l_value = get_variable(l_operand);
            struct variable_t* r_value = get_variable(r_operand);

            l_value->value += r_value->value;

            status_code = deallocate_var(r_operand);
        }

        else {
            char r_operand = 0;

            for (int i = id_len*8+3; i < limit; i++) {
                r_operand <<= 1;
                
                if (cmd[i] == ONE) {
                    r_operand |= 1;
                }

                else if (cmd[i] != ZERO) {
                    status_code = BAD;
                    break;
                }
            }

            struct variable_t* l_value = get_variable(l_operand);

            l_value->value += r_operand;
        }
    }

    return status_code;
}

int oung_subtract(enum token_t cmd[STMT_LIMIT], unsigned int limit) {
    int status_code = GOOD;

    if (cmd[0] != VAR) {
        status_code = BAD;
    }

    else {
        int id_len = 0;
        for (int i = 1; cmd[i] != VAR && i < limit; i += 8) {
            id_len++;
        }

        char l_operand[id_len];

        for (int i = 0; i < id_len; i++) {
            for (int j = 0; j < 8; j++) {
                enum token_t cur_token = cmd[i*8+j+1];

                l_operand[i] <<= 1;
            
                if (cur_token == ONE) {
                    l_operand[i] |= 1;
                }

                else if (cur_token != ZERO) {
                    return BAD;
                }
            }
        }

        if (cmd[id_len*8+2] != SEP) {
            return BAD;
        }

        else if (cmd[id_len*8+3] == VAR) {
            if (cmd[limit-1] != VAR) {
                return BAD;
            }

            int r_id_len = 0;
            for (int i = id_len*8+4; cmd[i] != VAR && i < limit; i += 8) {
                r_id_len++;
            }

            char r_operand[r_id_len];

            for (int i = 0; i < r_id_len; i++) {
                for (int j = 0; j < 8; j++) {
                    enum token_t cur_token = cmd[(id_len*8+4)+i*8+j];

                    r_operand[i] <<= 1;
                
                    if (cur_token == ONE) {
                        r_operand[i] |= 1;
                    }

                    else if (cur_token != ZERO) {
                        return BAD;
                    }
                }
            }

            struct variable_t* l_value = get_variable(l_operand);
            struct variable_t* r_value = get_variable(r_operand);

            l_value->value -= r_value->value;

            status_code = deallocate_var(r_operand);
        }

        else {
            char r_operand = 0;

            for (int i = id_len*8+3; i < limit; i++) {
                r_operand <<= 1;
                
                if (cmd[i] == ONE) {
                    r_operand |= 1;
                }

                else if (cmd[i] != ZERO) {
                    status_code = BAD;
                    break;
                }
            }

            struct variable_t* l_value = get_variable(l_operand);

            l_value->value -= r_operand;
        }
    }

    return status_code;
}

int oung_multiply(enum token_t cmd[STMT_LIMIT], unsigned int limit) {
    int status_code = GOOD;

    if (cmd[0] != VAR) {
        status_code = BAD;
    }

    else {
        int id_len = 0;
        for (int i = 1; cmd[i] != VAR && i < limit; i += 8) {
            id_len++;
        }

        char l_operand[id_len];

        for (int i = 0; i < id_len; i++) {
            for (int j = 0; j < 8; j++) {
                enum token_t cur_token = cmd[i*8+j+1];

                l_operand[i] <<= 1;
            
                if (cur_token == ONE) {
                    l_operand[i] |= 1;
                }

                else if (cur_token != ZERO) {
                    return BAD;
                }
            }
        }

        if (cmd[id_len*8+2] != SEP) {
            return BAD;
        }

        else if (cmd[id_len*8+3] == VAR) {
            if (cmd[limit-1] != VAR) {
                return BAD;
            }

            int r_id_len = 0;
            for (int i = id_len*8+4; cmd[i] != VAR && i < limit; i += 8) {
                r_id_len++;
            }

            char r_operand[r_id_len];

            for (int i = 0; i < r_id_len; i++) {
                for (int j = 0; j < 8; j++) {
                    enum token_t cur_token = cmd[(id_len*8+4)+i*8+j];

                    r_operand[i] <<= 1;
                
                    if (cur_token == ONE) {
                        r_operand[i] |= 1;
                    }

                    else if (cur_token != ZERO) {
                        return BAD;
                    }
                }
            }

            struct variable_t* l_value = get_variable(l_operand);
            struct variable_t* r_value = get_variable(r_operand);

            l_value->value *= r_value->value;

            status_code = deallocate_var(r_operand);
        }

        else {
            char r_operand = 0;

            for (int i = id_len*8+3; i < limit; i++) {
                r_operand <<= 1;
                
                if (cmd[i] == ONE) {
                    r_operand |= 1;
                }

                else if (cmd[i] != ZERO) {
                    status_code = BAD;
                    break;
                }
            }

            struct variable_t* l_value = get_variable(l_operand);

            l_value->value *= r_operand;
        }
    }

    return status_code;
}

int oung_divide(enum token_t cmd[STMT_LIMIT], unsigned int limit) {
    int status_code = GOOD;

    if (cmd[0] != VAR) {
        status_code = BAD;
    }

    else {
        int id_len = 0;
        for (int i = 1; cmd[i] != VAR && i < limit; i += 8) {
            id_len++;
        }

        char l_operand[id_len];

        for (int i = 0; i < id_len; i++) {
            for (int j = 0; j < 8; j++) {
                enum token_t cur_token = cmd[i*8+j+1];

                l_operand[i] <<= 1;
            
                if (cur_token == ONE) {
                    l_operand[i] |= 1;
                }

                else if (cur_token != ZERO) {
                    return BAD;
                }
            }
        }

        if (cmd[id_len*8+2] != SEP) {
            return BAD;
        }

        else if (cmd[id_len*8+3] == VAR) {
            if (cmd[limit-1] != VAR) {
                return BAD;
            }

            int r_id_len = 0;
            for (int i = id_len*8+4; cmd[i] != VAR && i < limit; i += 8) {
                r_id_len++;
            }

            char r_operand[r_id_len];

            for (int i = 0; i < r_id_len; i++) {
                for (int j = 0; j < 8; j++) {
                    enum token_t cur_token = cmd[(id_len*8+4)+i*8+j];

                    r_operand[i] <<= 1;
                
                    if (cur_token == ONE) {
                        r_operand[i] |= 1;
                    }

                    else if (cur_token != ZERO) {
                        return BAD;
                    }
                }
            }

            struct variable_t* l_value = get_variable(l_operand);
            struct variable_t* r_value = get_variable(r_operand);

            l_value->value /= r_value->value;

            status_code = deallocate_var(r_operand);
        }

        else {
            char r_operand = 0;

            for (int i = id_len*8+3; i < limit; i++) {
                r_operand <<= 1;
                
                if (cmd[i] == ONE) {
                    r_operand |= 1;
                }

                else if (cmd[i] != ZERO) {
                    status_code = BAD;
                    break;
                }
            }

            struct variable_t* l_value = get_variable(l_operand);

            l_value->value /= r_operand;
        }
    }

    return status_code;
}

int oung_or(enum token_t cmd[STMT_LIMIT], unsigned int limit) {
    int status_code = GOOD;

    if (cmd[0] != VAR) {
        status_code = BAD;
    }

    else {
        int id_len = 0;
        for (int i = 1; cmd[i] != VAR && i < limit; i += 8) {
            id_len++;
        }

        char l_operand[id_len];

        for (int i = 0; i < id_len; i++) {
            for (int j = 0; j < 8; j++) {
                enum token_t cur_token = cmd[i*8+j+1];

                l_operand[i] <<= 1;
            
                if (cur_token == ONE) {
                    l_operand[i] |= 1;
                }

                else if (cur_token != ZERO) {
                    return BAD;
                }
            }
        }

        if (cmd[id_len*8+2] != SEP) {
            return BAD;
        }

        else if (cmd[id_len*8+3] == VAR) {
            if (cmd[limit-1] != VAR) {
                return BAD;
            }

            int r_id_len = 0;
            for (int i = id_len*8+4; cmd[i] != VAR && i < limit; i += 8) {
                r_id_len++;
            }

            char r_operand[r_id_len];

            for (int i = 0; i < r_id_len; i++) {
                for (int j = 0; j < 8; j++) {
                    enum token_t cur_token = cmd[(id_len*8+4)+i*8+j];

                    r_operand[i] <<= 1;
                
                    if (cur_token == ONE) {
                        r_operand[i] |= 1;
                    }

                    else if (cur_token != ZERO) {
                        return BAD;
                    }
                }
            }

            struct variable_t* l_value = get_variable(l_operand);
            struct variable_t* r_value = get_variable(r_operand);

            l_value->value |= r_value->value;

            status_code = deallocate_var(r_operand);
        }

        else {
            char r_operand = 0;

            for (int i = id_len*8+3; i < limit; i++) {
                r_operand <<= 1;
                
                if (cmd[i] == ONE) {
                    r_operand |= 1;
                }

                else if (cmd[i] != ZERO) {
                    status_code = BAD;
                    break;
                }
            }

            struct variable_t* l_value = get_variable(l_operand);

            l_value->value |= r_operand;
        }
    }

    return status_code;
}

int oung_and(enum token_t cmd[STMT_LIMIT], unsigned int limit) {
    int status_code = GOOD;

    if (cmd[0] != VAR) {
        status_code = BAD;
    }

    else {
        int id_len = 0;
        for (int i = 1; cmd[i] != VAR && i < limit; i += 8) {
            id_len++;
        }

        char l_operand[id_len];

        for (int i = 0; i < id_len; i++) {
            for (int j = 0; j < 8; j++) {
                enum token_t cur_token = cmd[i*8+j+1];

                l_operand[i] <<= 1;
            
                if (cur_token == ONE) {
                    l_operand[i] |= 1;
                }

                else if (cur_token != ZERO) {
                    return BAD;
                }
            }
        }

        if (cmd[id_len*8+2] != SEP) {
            return BAD;
        }

        else if (cmd[id_len*8+3] == VAR) {
            if (cmd[limit-1] != VAR) {
                return BAD;
            }

            int r_id_len = 0;
            for (int i = id_len*8+4; cmd[i] != VAR && i < limit; i += 8) {
                r_id_len++;
            }

            char r_operand[r_id_len];

            for (int i = 0; i < r_id_len; i++) {
                for (int j = 0; j < 8; j++) {
                    enum token_t cur_token = cmd[(id_len*8+4)+i*8+j];

                    r_operand[i] <<= 1;
                
                    if (cur_token == ONE) {
                        r_operand[i] |= 1;
                    }

                    else if (cur_token != ZERO) {
                        return BAD;
                    }
                }
            }

            struct variable_t* l_value = get_variable(l_operand);
            struct variable_t* r_value = get_variable(r_operand);

            l_value->value &= r_value->value;

            status_code = deallocate_var(r_operand);
        }

        else {
            char r_operand = 0;

            for (int i = id_len*8+3; i < limit; i++) {
                r_operand <<= 1;
                
                if (cmd[i] == ONE) {
                    r_operand |= 1;
                }

                else if (cmd[i] != ZERO) {
                    status_code = BAD;
                    break;
                }
            }

            struct variable_t* l_value = get_variable(l_operand);

            l_value->value &= r_operand;
        }
    }

    return status_code;
}
