#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct node {
    char page[100];
    struct node* next;
};

struct bstNode {
    char page[100];
    struct bstNode *left, *right;
};

void push(struct node** top, char page[]) {
    struct node* newNode = (struct node*)malloc(sizeof(struct node));
    strcpy(newNode->page, page);
    newNode->next = *top;
    *top = newNode;
}

char* pop(struct node** top) {
    if (*top == NULL) return NULL;
    struct node* temp = *top;
    char* page = strdup(temp->page);
    *top = temp->next;
    free(temp);
    return page;
}

void clearStack(struct node** top) {
    while (*top) {
        struct node* temp = *top;
        *top = (*top)->next;
        free(temp);
    }
}

void insertFront(struct node** head, char page[]) {
    struct node* newNode = (struct node*)malloc(sizeof(struct node));
    strcpy(newNode->page, page);
    newNode->next = *head;
    *head = newNode;
}

void displayList(struct node* head, const char* title, const char* prefix) {
    if (!head) {
        printf("%s is empty.\n", title);
        return;
    }
    printf("%s:\n", title);
    while (head) {
        printf("%s %s\n", prefix, head->page);
        head = head->next;
    }
}

int isBookmarked(struct node* bookmarks, char page[]) {
    while (bookmarks) {
        if (strcmp(bookmarks->page, page) == 0)
            return 1;
        bookmarks = bookmarks->next;
    }
    return 0;
}

struct bstNode* insertBST(struct bstNode* root, char page[]) {
    if (!root) {
        struct bstNode* newNode = (struct bstNode*)malloc(sizeof(struct bstNode));
        strcpy(newNode->page, page);
        newNode->left = newNode->right = NULL;
        return newNode;
    }
    if (strcmp(page, root->page) < 0)
        root->left = insertBST(root->left, page);
    else if (strcmp(page, root->page) > 0)
        root->right = insertBST(root->right, page);
    return root;
}

void inorderBST(struct bstNode* root) {
    if (root) {
        inorderBST(root->left);
        printf("%s\n", root->page);
        inorderBST(root->right);
    }
}


int searchBST(struct bstNode* root, char page[]) {
    if (!root) return 0;
    if (strcmp(root->page, page) == 0) return 1;
    if (strcmp(page, root->page) < 0)
        return searchBST(root->left, page);
    return searchBST(root->right, page);
}

int main() {
    struct node *backStack = NULL, *forwardStack = NULL, *history = NULL, *bookmarks = NULL;
    struct bstNode *historyBST = NULL;
    char currentPage[100] = "Home";
    char newPage[100];
    int choice;

    do {
        printf("\n--- Browser Menu ---\n");
        printf("1. Visit New Page\n");
        printf("2. Go Back\n");
        printf("3. Go Forward\n");
        printf("4. Show History\n");
        printf("5. Clear History\n");
        printf("6. Exit\n");
        printf("7. Bookmark Current Page\n");
        printf("8. View Bookmarks\n");
        printf("9. Search in History\n");
        printf("10. View Sorted History\n");
        printf("Current Page: %s\n", currentPage);
        printf("Enter choice: ");
        scanf("%d", &choice);
        getchar(); 

        switch (choice) {
            case 1:
                printf("Enter new page URL: ");
                fgets(newPage, sizeof(newPage), stdin);
                newPage[strcspn(newPage, "\n")] = 0;

                push(&backStack, currentPage);
                strcpy(currentPage, newPage);
                insertFront(&history, newPage);
                historyBST = insertBST(historyBST, newPage);
                clearStack(&forwardStack);
                break;

            case 2: {
                char* page = pop(&backStack);
                if (page) {
                    push(&forwardStack, currentPage);
                    strcpy(currentPage, page);
                    free(page);
                } else {
                    printf("No pages to go back to.\n");
                }
                break;
            }

            case 3: {
                char* page = pop(&forwardStack);
                if (page) {
                    push(&backStack, currentPage);
                    strcpy(currentPage, page);
                    free(page);
                } else {
                    printf("No pages to go forward to.\n");
                }
                break;
            }

            case 4:
                displayList(history, "Browsing History (Latest First)", "\u2192");
                break;

            case 5:
                clearStack(&history);
                printf("Browsing history cleared.\n");
                break;

            case 6:
                printf("Exiting browser...\n");
                break;

            case 7:
                if (!isBookmarked(bookmarks, currentPage)) {
                    insertFront(&bookmarks, currentPage);
                    printf("Bookmarked: %s\n", currentPage);
                } else {
                    printf("Already bookmarked: %s\n", currentPage);
                }
                break;

            case 8:
                displayList(bookmarks, "Bookmarked Pages", "\u2605");
                break;

            case 9:
                printf("Enter page to search in history: ");
                fgets(newPage, sizeof(newPage), stdin);
                newPage[strcspn(newPage, "\n")] = 0;
                if (searchBST(historyBST, newPage))
                    printf("Page '%s' found in history.\n", newPage);
                else
                    printf("Page '%s' not found in history.\n", newPage);
                break;

            case 10:
                printf("\nSorted Browsing History:\n");
                inorderBST(historyBST);
                break;

            default:
                printf("Invalid choice. Try again.\n");
        }
    } while (choice != 6);

    clearStack(&backStack);
    clearStack(&forwardStack);
    clearStack(&history);
    clearStack(&bookmarks);
    return 0;
}