/* BARBULESCU Robert Cristian - 315CD */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct page {
    int id;
    char url[50];
    char *description;
} page;

typedef struct StackNode{
	struct page *elem;
	struct StackNode *next;
} StackNode;

typedef struct stack {
    StackNode *top;
} stack;

struct stack *push(struct stack *s, struct page *page) {
    StackNode *newNode = (StackNode *)malloc(sizeof(StackNode));
    newNode->elem = page;
    newNode->next = s->top;
    s->top = newNode;
    return s;
}

struct stack *pop(struct stack *s) {
    if (s->top == NULL) {
        return s;
    }
    StackNode *tmp = s->top;
    s->top = s->top->next;
    free(tmp);
    return s;
}

typedef struct tabsList {
    struct tab *tab;
    struct tabsList *next;
    struct tabsList *prev;
} tabsList;

struct tab {
    int id;
    struct page *currentPage;
    struct stack *backwardStack;
    struct stack *forwardStack;
};

struct browser {
    struct tab *current;
    struct tabsList *list;
};

struct browser *initBrowser() {
    struct browser *sentinel;
    sentinel = (struct browser *)malloc(sizeof(struct browser));
    sentinel->current = NULL;
    sentinel->list = (struct tabsList *)malloc(sizeof(struct tabsList));
    sentinel->list->next = sentinel->list;
    sentinel->list->prev = sentinel->list;
    sentinel->list->tab = NULL;
    return sentinel;
}

struct tabsList *initTabList(struct tab *tab) {
    struct tabsList *newList;
    newList = (struct tabsList *)malloc(sizeof(struct tabsList));
    newList->tab = tab;
    newList->next = newList;
    newList->prev = newList;
    return newList;
}

struct tab *initTab(struct page *page, int lastId) {
    struct tab *newTab;
    newTab = (struct tab *)malloc(sizeof(struct tab));
    newTab->id = lastId + 1;
    newTab->currentPage = page;
    newTab->backwardStack = (struct stack *)malloc(sizeof(struct stack));
    newTab->backwardStack->top = NULL;
    newTab->forwardStack = (struct stack *)malloc(sizeof(struct stack));
    newTab->forwardStack->top = NULL;
    return newTab;
}

struct browser *new_tab(struct browser *sentinel, struct page *page, int lastId) {
    struct tab *newTab = initTab(page, lastId);
    struct tabsList *newNode = initTabList(newTab);

    newNode->prev = sentinel->list->prev;
    newNode->next = sentinel->list;
    sentinel->list->prev->next = newNode;
    sentinel->list->prev = newNode;

    sentinel->current = newTab;
    return sentinel;
}


void print(struct browser *sentinel, FILE *fout) {
    struct tabsList *iter = sentinel->list->next;
    while (iter->tab != sentinel->current) {
        iter = iter->next;
    }
    fprintf(fout, "%d ", iter->tab->id);
    iter = iter->next;
    while (iter->tab != sentinel->current) {
        if (iter != sentinel->list) {
            fprintf(fout, "%d ", iter->tab->id);
        }
        iter = iter->next;
    }
    fprintf(fout, "\n%s", sentinel->current->currentPage->description);
}

struct browser *close_tab(struct browser *sentinel, FILE *fout) {
    struct tabsList *iter = sentinel->list->next;

    if (sentinel->current->id == 0) {
        fprintf(fout, "403 Forbidden\n");
        return sentinel;
    }

    while (iter != sentinel->list) {
        if (iter->tab == sentinel->current) {
                        // Free stack contents
            while (iter->tab->backwardStack->top != NULL) {
                pop(iter->tab->backwardStack);
            }
            free(iter->tab->backwardStack);
            
            while (iter->tab->forwardStack->top != NULL) {
                pop(iter->tab->forwardStack);
            }
            free(iter->tab->forwardStack);
            
            // Remove from list
            iter->prev->next = iter->next;
            iter->next->prev = iter->prev;
            sentinel->current = iter->prev->tab;
            
            // Free the tab and list node
            free(iter->tab);
            free(iter);
            break;
        }
        iter = iter->next;
    }
    return sentinel;
}

struct browser *open(struct browser *sentinel, int id, FILE *fout) {
    struct tabsList *iter = sentinel->list->next;

    while (iter != sentinel->list) {
        if (iter->tab->id == id) {
            sentinel->current = iter->tab;
            return sentinel;
        }
        iter = iter->next;
    }

    fprintf(fout, "403 Forbidden\n");
    return sentinel;
}

struct tab *next(struct browser *sentinel) {
    struct tabsList *iter = sentinel->list->next;

    while (iter != sentinel->list) {
        if (iter->tab == sentinel->current) {
            if (iter->next == sentinel->list) {
                sentinel->current = sentinel->list->next->tab;
            } else {
                sentinel->current = iter->next->tab;
            }
            return sentinel->current;
        }
        iter = iter->next;
    }
    return sentinel->current;
}

struct tab *prev(struct browser *sentinel) {
    struct tabsList *iter = sentinel->list->next;

    while (iter != sentinel->list) {
        if (iter->tab == sentinel->current) {
            if (iter->prev == sentinel->list) {
                sentinel->current = sentinel->list->prev->tab;
            } else {
                sentinel->current = iter->prev->tab;
            }
            return sentinel->current;
        }
        iter = iter->next;
    }
    return sentinel->current;
}

struct browser *new_page(struct browser *sentinel, struct page *page) {
    struct tab *currentTab = sentinel->current;
    currentTab->backwardStack = push(currentTab->backwardStack, currentTab->currentPage);
    while (currentTab->forwardStack->top != NULL) {
        pop(currentTab->forwardStack);
    }
    currentTab->currentPage = page;
    return sentinel;
}

void print_backward_stack(StackNode *s, FILE *fout) {
    if (s == NULL) return;
    fprintf(fout, "%s\n", s->elem->url);
    print_backward_stack(s->next, fout);
}

void print_forward_stack(StackNode *s, FILE *fout) {
    if (s == NULL) return;
    print_forward_stack(s->next, fout);
    fprintf(fout, "%s\n", s->elem->url);
}
void print_history(struct browser *sentinel, int id, FILE *fout) {
    struct tabsList *iter = sentinel->list->next;
    while (iter != sentinel->list) {
        if (iter->tab->id == id) {
            struct stack *backwardStack = iter->tab->backwardStack;
            struct stack *forwardStack = iter->tab->forwardStack;
            
            print_forward_stack(forwardStack->top, fout);
            
            fprintf(fout, "%s\n", iter->tab->currentPage->url);

            print_backward_stack(backwardStack->top, fout);

            break;
        }
        iter = iter->next;
    }
    if (iter == sentinel->list) {
        fprintf(fout, "403 Forbidden\n");
    }
}

struct tab *backward(struct tab *current, FILE *fout) {
    if (current->backwardStack->top == NULL) {
        fprintf(fout, "403 Forbidden\n");
        return current;
    }
    struct page *page = current->backwardStack->top->elem;
    current->forwardStack = push(current->forwardStack, current->currentPage);
    current->currentPage = page;
    current->backwardStack = pop(current->backwardStack);
    return current;
}

struct tab *forward(struct tab *current, FILE *fout) {
    if (current->forwardStack->top == NULL) {
        fprintf(fout, "403 Forbidden\n");
        return current;
    }
    struct page *page = current->forwardStack->top->elem;
    current->backwardStack = push(current->backwardStack, current->currentPage);
    current->currentPage = page;
    current->forwardStack = pop(current->forwardStack);
    return current;
}

int main() {
    FILE *fin = fopen("tema1.in", "r");
    FILE *fout = fopen("tema1.out", "w");
    int n;
    fscanf(fin, "%d", &n);
    struct page *pages = (struct page *)malloc((n + 1) * sizeof(struct page));
    pages[0].id = 0;
    strcpy(pages[0].url, "https://acs.pub.ro/");
    pages[0].description = "Computer Science\n";
    for (int i = 1; i <= n; i++) {
        fscanf(fin, "%d", &pages[i].id);
        fscanf(fin, "%s", pages[i].url);
        fgetc(fin);
        pages[i].description = (char *)malloc(50 * sizeof(char));
        fgets(pages[i].description, 51, fin);
    }

    struct browser *sentinel = initBrowser();
    sentinel = new_tab(sentinel, &pages[0], -1);
    int ops, lastId = 0;
    fscanf(fin, "%d", &ops);

    for (int i = 0; i < ops; i++) {
        char *command;
        command = (char *)malloc(50 * sizeof(char));
        fscanf(fin, "%s", command);
        if (strcmp(command, "NEW_TAB") == 0) {
            struct page *page = &pages[0];
            sentinel = new_tab(sentinel, page, lastId++);
        } else if (strcmp(command, "CLOSE") == 0) {
            sentinel = close_tab(sentinel, fout);
        } else if (strcmp(command, "OPEN") == 0) {
            int id;
            fscanf(fin, "%d", &id);
            sentinel = open(sentinel, id, fout);
        } else if (strcmp(command, "NEXT") == 0) {
            sentinel->current = next(sentinel);
        } else if (strcmp(command, "PREV") == 0) {
            sentinel->current = prev(sentinel);
        } else if (strcmp(command, "PAGE") == 0) {
            int id, index;
            fscanf(fin, "%d", &id);
            for (index = 0; index <= n; index++) {
                if (pages[index].id == id) {
                    break;
                }
            }
            if (index > n) {
                fprintf(fout, "403 Forbidden\n");
                free(command);
                continue;
            }
            struct page *page = &pages[index];
            sentinel = new_page(sentinel, page);
        } else if (strcmp(command, "PRINT_HISTORY") == 0) {
            int id;
            fscanf(fin, "%d", &id);
            print_history(sentinel, id, fout);
        } else if (strcmp(command, "BACKWARD") == 0) {
            sentinel->current = backward(sentinel->current, fout);
        } else if (strcmp(command, "FORWARD") == 0) {
            sentinel->current = forward(sentinel->current, fout);
        } else if (strcmp(command, "PRINT") == 0) {
            print(sentinel, fout);
        }
        free(command);
    }
    fclose(fin);
    fclose(fout);
    for (int i = 1; i <= n; i++) {
        free(pages[i].description);
    }
    free(pages);
    // Free all remaining tabs
    struct tabsList *iter = sentinel->list->next;
    while (iter != sentinel->list) {
        struct tabsList *next = iter->next;
        
        // Free stacks
        while (iter->tab->backwardStack->top != NULL) {
            pop(iter->tab->backwardStack);
        }
        free(iter->tab->backwardStack);
        
        while (iter->tab->forwardStack->top != NULL) {
            pop(iter->tab->forwardStack);
        }
        free(iter->tab->forwardStack);
        
        free(iter->tab);
        free(iter);
        iter = next;
    }

    // Free sentinel
    free(sentinel->list);
    free(sentinel);
    return 0;
}
