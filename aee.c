#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STATES 100
#define MAX_ALPHABET 5

typedef struct {
    int transitions[MAX_ALPHABET][MAX_STATES];
    int is_final[MAX_STATES];
} NFA;

typedef struct {
    int transitions[MAX_ALPHABET];
    int is_final;
} DFA;

void add_transition(NFA *nfa, int state, char symbol, int next_state) {
    int symbol_index = symbol - 'a';
    nfa->transitions[symbol_index][state] = next_state;
}

void epsilon_closure(NFA *nfa, int state, int *closure) {
    closure[state] = 1;

    for (int i = 0; i < MAX_ALPHABET; i++) {
        int next_state = nfa->transitions[i][state];
        if (next_state != -1 && !closure[next_state]) {
            epsilon_closure(nfa, next_state, closure);
        }
    }
}

void print_dfa(DFA *dfa, int num_states) {
    printf("Tabela de Transições do DFA:\n");
    printf("Estado\t|");
    for (char symbol = 'a'; symbol < 'a' + MAX_ALPHABET; symbol++) {
        printf("\t%c", symbol);
    }
    printf("\t| Final\n");

    for (int i = 0; i < num_states; i++) {
        printf("%d\t|", i);
        for (int j = 0; j < MAX_ALPHABET; j++) {
            printf("\t%d", dfa[i].transitions[j]);
        }
        printf("\t| %d\n", dfa[i].is_final);
    }
}

void state_elimination(NFA *nfa, int num_states, char *alphabet) {
    int dfa_size = 0;
    DFA dfa[MAX_STATES];

    // Inicializa os estados do DFA
    for (int i = 0; i < MAX_STATES; i++) {
        for (int j = 0; j < MAX_ALPHABET; j++) {
            dfa[i].transitions[j] = -1;
        }
        dfa[i].is_final = 0;
    }

    // Realiza a eliminação de estados
    for (int i = 0; i < num_states; i++) {
        int closure[MAX_STATES] = {0};
        epsilon_closure(nfa, i, closure);

        for (int j = 0; j < MAX_ALPHABET; j++) {
            if (alphabet[j] == '1') {
                continue;  // Ignora transições de épsilon
            }

            int next_state = -1;
            for (int k = 0; k < num_states; k++) {
                if (closure[k]) {
                    int transition_state = nfa->transitions[j][k];
                    if (transition_state != -1) {
                        if (next_state == -1) {
                            next_state = transition_state;
                        } else {
                            next_state = num_states + dfa_size;
                            break;
                        }
                    }
                }
            }

            dfa[i].transitions[j] = next_state;
        }

        dfa[i].is_final = nfa->is_final[i];
        dfa_size++;
    }

    // Imprime o DFA resultante
    print_dfa(dfa, dfa_size);
}

int main() {
    NFA nfa;
    int num_states, num_transitions;

    printf("Digite o número de estados: ");
    scanf("%d", &num_states);

    printf("Digite o número de transições: ");
    scanf("%d", &num_transitions);

    // Inicializa as transições do NFA
    for (int i = 0; i < MAX_STATES; i++) {
        for (int j = 0; j < MAX_ALPHABET; j++) {
            nfa.transitions[j][i] = -1;
        }
        nfa.is_final[i] = 0;
    }

    // Insere as transições do NFA
    printf("Insira as transições no formato: <from_state> <symbol> <to_state>\n");
    for (int i = 0; i < num_transitions; i++) {
        int from_state, to_state;
        char symbol;

        printf("Transição %d: ", i + 1);
        scanf("%d %c %d", &from_state, &symbol, &to_state);

        add_transition(&nfa, from_state, symbol, to_state);
    }

    // Insere os estados finais do NFA
    printf("Digite o número de estados finais: ");
    int num_final_states;
    scanf("%d", &num_final_states);

    printf("Digite os estados finais: ");
    for (int i = 0; i < num_final_states; i++) {
        int final_state;
        scanf("%d", &final_state);
        nfa.is_final[final_state] = 1;
    }

    // Insere o alfabeto
    char alphabet[MAX_ALPHABET];
    printf("Digite o alfabeto (por exemplo, 'abc', use '1' para épsilon): ");
    scanf("%s", alphabet);

    // Executa a eliminação de estados para converter NFA para DFA
    state_elimination(&nfa, num_states, alphabet);

    return 0;
}
