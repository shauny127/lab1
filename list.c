// ============================================================================
// list.c -- YOUR WORK for Lab 1.
//
// Implement every function below so it matches its contract in list.h. Each has
// a TODO describing what to do; the header has the ownership rule and Big-O.
//
// The placeholder bodies let the project compile and stay leak-free BEFORE you
// start, so the whole workflow (make, make valgrind, make format) works from
// minute one. As you implement each function, real behaviour appears.
//
// The `(void)x;` lines just tell the compiler "I know x is unused for now" so
// -Werror does not stop you. Delete each one once you use that variable.
// ============================================================================

#include "list.h"

#include <stdlib.h> // you will need malloc and free

TokenList list_create(void) {
    // GIVEN: an empty list is all-null with length 0. (This one is done for you
    // as an example of a designated initializer -- study it, then build the
    // rest below.)
    return (TokenList){.head = nullptr, .tail = nullptr, .length = 0};
}

void list_destroy(TokenList *list) {
    TokenNode *cur = list->head;
    while (cur != nullptr) {
        TokenNode *next = cur->next;
        token_free(&cur->token);
        free(cur);
        cur = next;
    }
    *list = list_create();
}

bool list_push_front(TokenList *list, Token token) {
    TokenNode *new_node = malloc(sizeof *new_node);
    if (new_node == nullptr) {
        token_free(&token);
        return false;
    }
    new_node->token = token;
    new_node->next  = list->head;
    list->head      = new_node;
    if (list->tail == nullptr) {
        list->tail = new_node;
    }
    list->length++;
    return true;
}

bool list_push_back(TokenList *list, Token token) {
    // TODO: allocate a node holding `token` and append it at the BACK. If the
    // list is empty, it becomes both head and tail; otherwise link tail->next to
    // it and move tail. Then length++. Same malloc-failure rule as above.
    TokenNode *new_node = malloc(sizeof *new_node);
    if (new_node == nullptr) {
        token_free(&token);
        return false;
    }
    new_node->token = token;
    new_node->next  = nullptr;
    if (list->tail == nullptr) {
        list->head = new_node;
        list->tail = new_node;
    } else {
        list->tail->next = new_node;
        list->tail= new_node;
    }
    list->length++;
    return true;
}

bool list_insert_after(TokenList *list, TokenNode *node, Token token) {
    if (node == nullptr) {
        token_free(&token);
        return false;
    }
    TokenNode *new_node = malloc(sizeof *new_node);

    if (new_node == nullptr) {
        token_free(&token);
        return false;
    }
    new_node->token = token;
    new_node->next  = node->next;
    node->next      = new_node;
    if (list->tail == node) {
        list->tail = new_node;
    }
    list->length++;
    return true;
}

TokenNode *list_find_first(const TokenList *list, TokenKind kind) {
    for (TokenNode *cur = list->head; cur != nullptr; cur = cur->next) {
        if (cur->token.kind == kind) {
            return cur;
        }
    }
    return nullptr;
}

bool list_remove_first(TokenList *list, TokenKind kind) {
    TokenNode *prev = nullptr;
    TokenNode *cur  = list->head;
    while (cur != nullptr && cur->token.kind != kind) {
        prev = cur;
        cur  = cur->next;
    }
    if (cur == nullptr) {
        return false;
    }
    if (prev == nullptr) {
        list->head = cur->next;
    } else {
        prev->next = cur->next;
    }
    if (list->tail == cur) {
        list->tail = prev;
    }
    token_free(&cur->token);
    free(cur);
    list->length--;
    return true;
}

size_t list_length(const TokenList *list) {
    // TODO: return the length you maintain in the header.
    return list->length;
}

void list_print(const TokenList *list, FILE *out) {
    for (TokenNode *cur = list->head; cur != nullptr; cur = cur->next) {
        fprintf(out, "%-9s '%s' (%d:%d)\n", token_kind_name(cur->token.kind), cur->token.lexeme,
                cur->token.line, cur->token.col);
    }
}

void list_to_dot(const TokenList *list, FILE *out) {
    fprintf(out, "digraph tokens {\n");
    fprintf(out, "  rankdir=LR;\n");
    fprintf(out, "  node [shape=record];\n");

    int i = 0;
    for (TokenNode *cur = list->head; cur != nullptr; cur = cur->next) {
        fprintf(out, "  n%d [label=\"%s\"];\n", i, token_kind_name(cur->token.kind));
        i++;
    }
    for (int j = 0; j < i - 1; j++) {
        fprintf(out, "  n%d -> n%d;\n", j, j + 1);
    }
    if (i > 0) {
        fprintf(out, "  n%d -> nnull;\n", i - 1);
    }
    fprintf(out, "  nnull [label=\"nullptr\"];\n");
    fprintf(out, "}\n");
}

bool list_check_invariant(const TokenList *list) {
    size_t counted  = 0;
    TokenNode *cur  = list->head;
    TokenNode *last = nullptr;
    while (cur != nullptr) {
        last = cur;
        counted++;
        cur = cur->next;
    }
    if (counted != list->length) {
        return false;
    }
    if (last != list->tail) {
        return false;
    }
    if (list->tail == nullptr && list->head != nullptr) {
        return false;
    }
    if (list->tail != nullptr && list->head == nullptr) {
        return false;
    }
    return true;
}
