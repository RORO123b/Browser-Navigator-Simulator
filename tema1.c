/* BARBULESCU Robert Cristian - 315CD */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct page {
    int id;
    char url[50];
    char *description;
};

typedef struct StackNode{
	int elem;
	struct StackNode *next;
} StackNode;

typedef struct stack {
StackNode *top;
};

typedef struct tabsList {
    struct tab *tab;
    struct tabsList *next;
    struct tabsList *prev;
};

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
    sentinel->list->next = sentinel->list;
    sentinel->list->prev = sentinel->list;
    return sentinel;
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
    sentinel->current = newTab;
    struct browser *iter = sentinel;
    while (iter->list->next != sentinel->list) {
        iter->list = iter->list->next;
    }
    iter->list->next = newTab;
    iter->list->next->next = sentinel->list;
    sentinel->list->prev = iter->list->next->next;
    iter->list->next->prev = iter->list;
    sentinel->current = newTab;
    return sentinel;
    
}

void print(struct browser *sentinel) {
    FILE *fout = fopen("tema1.out", "w");
    fprintf(fout, "%d ", sentinel->list->prev->tab->id);
    struct browser *iter = sentinel->list->next;
    while (iter->list->next != sentinel->list) {
        fprintf(fout, "%d ",iter->list->tab->id);
        iter->list = iter->list->next;
    }
    fprintf(fout, "\n");
    fprintf(fout, "%s", sentinel->current->currentPage->description);
    fclose(fout);
}

int main() {
    FILE *fin = fopen("tema1.in", "r");
    int n;
    fscanf(fin, "%d", &n);
    struct page *pages = (struct page *)malloc((n + 1) * sizeof(struct page));
    pages[0].id = 0;
    strcpy(pages[0].url, "https://acs.pub.ro/");
    pages[0].description = "Computer Science";
    for (int i = 1; i <= n; i++) {
        fscanf(fin, "%d", &pages[i].id);
        fscanf(fin, "%s", pages[i].url);
        pages[i].description = (char *)malloc(50 * sizeof(char));
        fscanf(fin, "%s", pages[i].description);
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
            struct page *page = &pages[lastId];
            sentinel = new_tab(sentinel, page, lastId++);
        } else if (strcmp(command, "close") == 0) {
            // Close tab logic
        } else if (strcmp(command, "back") == 0) {
            // Back logic
        } else if (strcmp(command, "forward") == 0) {
            // Forward logic
        } else if (strcmp(command, "print") == 0) {
            print(sentinel);
        }
        free(command);
    }
    fclose(fin);
    return 0;
}
