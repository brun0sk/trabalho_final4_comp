#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_STATES 256
#define MAX_TRANSITIONS 256
typedef struct Transition {
    int state;
    char symbol;
} Transition;

typedef struct State {
    int id;
    int is_final;
    Transition transitions[MAX_TRANSITIONS];
    int num_transitions;
} State;

typedef struct NFA {
    State states[MAX_STATES];
    int num_states;
    int start_state;
} NFA;

void add_state (NFA *nfa , int is_final) {
    State new_state;
    new_state.id = nfa -> num_states;
    new_state.is_final = is_final;
    new_state.num_transitions = 0;
    nfa->states[nfa -> num_states++] = new_state ;
}
void add_transition (NFA *nfa , int from_state , int to_state , char symbol) {
    State *state = &nfa->states[from_state];
    Transition new_transition;
    new_transition.state = to_state;
    new_transition.symbol = symbol;
    state -> transitions[state ->num_transitions ++] = new_transition;
}
NFA thompsons_construction ( const char *regex) {
    NFA nfa;
    nfa.num_states = 0 ;
    add_state (&nfa , 0) ; // Add start state
    nfa.start_state = 0;
    int i, from_state, to_state;
    int point;
    char state;
    for (int i = 0; regex[i] != '\0'; i++) {
        from_state = nfa.num_states - 1;

        if (regex[i] == 'a' || regex[i] == 'b') { // For simplicity, only 'a' and 'b' are considered
            add_state(&nfa, 0);
            to_state = nfa.num_states - 1;
            add_transition(&nfa, from_state, to_state, regex[i]);
        } else if (regex[i] == '|') {
            // Union operation
            // Implementation of union operation goes here
            add_state(&nfa, 0);
            to_state = nfa.num_states - 1;
            state = nfa.states[nfa.num_states-3].transitions[0].symbol;
            printf("%c \n", state);
            nfa.states[nfa.num_states-3].transitions[0].symbol = '\0';
            from_state = nfa.num_states - 2;
            add_transition(&nfa, from_state, to_state, state);

            point = 0; // Usado para verificar | em sequencia
            add_state(&nfa, 0);
            for (int j = i+1; regex[j] != '\0'; j++){
                if (regex[j] == 'a' || regex[j] == 'b' && point<1){
                    add_transition(&nfa, from_state, to_state, regex[j]);
                    point += 1;
                }
                else if (point == 1 && regex[j] == '|')
                {
                    /* code */
                    point = 0;
                }
                else{
                    break;
                }
                i++;    // so adiciona quando estado adicionado
            }
            add_transition(&nfa, from_state+1, to_state+1, '\0');

        } else if (regex[i] == '*') {
            // Kleene star operation
            // Implementation of Kleene star operation goes here
            
        }
        // Other operations like concatenation can be implemented similarly
    }

    nfa.states[nfa.num_states - 1].is_final = 1; // Last state is the final state
    return nfa;
}

int main() {
    const char *regex = "(a|b)a";
    NFA nfa = thompsons_construction(regex);
    printf("NFA for regex '%s':\n", regex);

    for (int i = 0; i < nfa.num_states; i++) {
        State state = nfa.states[i];
        printf(" State %d(%s): \n", state.id, state.is_final ? "final" : "non-final");

        for (int j = 0; j < state.num_transitions; j++) {
            Transition t = state.transitions[j];
            printf("  --%c--> State %d\n", t.symbol, t.state);
        }
    }
    return 0 ;
}

