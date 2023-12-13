#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STATES 100
#define MAX_ALPHABET 128

typedef struct {
    int transitions[MAX_ALPHABET][MAX_STATES];
    int is_final[MAX_STATES];
    int num_states;
} NFA;

int add_state(NFA *nfa) {
    int state = nfa->num_states++;
    nfa->is_final[state] = 0;
    return state;
}

void add_transition(NFA *nfa, int from_state, int to_state, char symbol) {
    int symbol_index = (int)symbol;
    nfa->transitions[symbol_index][from_state] = to_state;
}

void print_nfa(NFA *nfa) {
    printf("NFA Transition Table:\n");
    printf("State\t| a\t| b\t| Final\n");

    for (int i = 0; i < nfa->num_states; i++) {
        printf("%d\t|", i);
        for (int j = 'a'; j <= 'b'; j++) {
            printf("\t%d", nfa->transitions[j][i]);
        }
        printf("\t| %d\n", nfa->is_final[i]);
    }
}

NFA thompsons_construction(const char *regex) {
    NFA nfa;
    nfa.num_states = 0;

    // Inicializa o estado inicial
    int start_state = add_state(&nfa);
    int current_state = start_state;

    for (int i = 0; regex[i] != '\0'; i++) {
        if (regex[i] == 'a' || regex[i] == 'b') {
            int next_state = add_state(&nfa);
            add_transition(&nfa, current_state, next_state, regex[i]);
            current_state = next_state;
        } else if (regex[i] == '.') {
            // Concatenação
            int next_state = add_state(&nfa);
            add_transition(&nfa, current_state, next_state, '\0');
            current_state = next_state;
        } else if (regex[i] == '|') {
            // União
            int branch_state = add_state(&nfa);
            add_transition(&nfa, branch_state, current_state, '\0');
            int next_state = add_state(&nfa);
            add_transition(&nfa, current_state, next_state, '\0');
            current_state = next_state;
        } else if (regex[i] == '*') {
            // Fechamento de Kleene
            int loop_state = add_state(&nfa);
            add_transition(&nfa, loop_state, current_state, '\0');
            add_transition(&nfa, current_state, loop_state, '\0');
            current_state = loop_state;
        }
    }

    // Marca o estado final
    nfa.is_final[current_state] = 1;

    return nfa;
}

int main() {
    char regex[100];
    printf("Digite a expressão regular: ");
    scanf("%s", regex);

    NFA nfa = thompsons_construction(regex);
    print_nfa(&nfa);

    return 0;
}
