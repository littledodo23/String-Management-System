//danah abu rayya 1210195 sec5
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
// Define the maximum length of an input string
#define MAX_STRING_LENGTH 31
typedef struct node {
    char data[MAX_STRING_LENGTH];
    struct node* next;
    struct node* prev;
}node;
//pointers for indicate location
struct node* head = NULL;
node* createNew( char data[MAX_STRING_LENGTH]) {
    node* new = (node*)malloc(sizeof(node));
    // Check if there is left space in memory
    if (new == NULL) {
        printf("Out of memory\n");
        exit(EXIT_FAILURE);  // Exit the program in case of memory allocation failure
    }
    strncpy(new->data, data, MAX_STRING_LENGTH - 1);
    new->data[MAX_STRING_LENGTH - 1] = '\0';
    new->next = NULL;
    new->prev = NULL;
    return new;
}
void insert(node** head, char data[MAX_STRING_LENGTH]) {
    node* new_node = createNew(data);
    if (*head == NULL) {
        *head = new_node;
    } else {
        node* temp = *head;
        while (temp->next != NULL) {
            temp = temp->next;
        }
        temp->next = new_node;
        new_node->prev = temp;
    }
}
int isEmpty(node * head){
    return head == NULL;
}
void delete(node** head, const char* string) {
    node* current_node = *head;

    // Check if the list is empty
    if (isEmpty(*head)) {
        printf("List is empty. Cannot delete the target.\n");
        return;
    }

    // If the node to delete is the head node
    if (strcmp(current_node->data, string) == 0) {
        *head = current_node->next;

        if (*head != NULL) {
            (*head)->prev = NULL;
        }

        free(current_node);
        return;
    }
    // Search for the node to delete
    while (current_node != NULL && strcmp(current_node->data, string) != 0) {
        current_node = current_node->next;
    }

    // If the target node is not found
    if (current_node == NULL) {
        printf("Node not found. Cannot delete the target.\n");
        return;
    }

    // Delete the node
    if (current_node->next != NULL) {
        current_node->next->prev = current_node->prev;
    }

    if (current_node->prev != NULL) {
        current_node->prev->next = current_node->next;
    }

    free(current_node);
}

// to dispaly the list :
void display(node* head) {
    node* current_node = head;
    while (current_node != NULL) {
        printf("%s \n", current_node->data);
        current_node = current_node->next;
    }


}
void makeEmpty(node** list) {
    node* current = *list;
    while (current != NULL) {
        node* temp = current;
        current = current->next;
        free(temp);
    }
    *list = NULL;
}

int insertFromFile(char* file) {
    // Make the linked list empty before loading new data
    makeEmpty(&head);
    FILE* ptr;
    ptr = fopen(file, "r");
    char str[MAX_STRING_LENGTH];

    if (ptr != NULL) {
        while (fgets(str, MAX_STRING_LENGTH, ptr) != NULL) {
            // Remove the newline character if present
            size_t len = strlen(str);
            if (len > 0 && str[len - 1] == '\n') {
                str[len - 1] = '\0';
            }
            if (!isdigit(str[0])){//check if the first char isn't a num
                str[31]='/0';//make every word is 30 char and less and delete the rest 
                int c;
                while ((c = fgetc(ptr)) != EOF && c != '\n');
                insert(&head, str);

            }


        }
        fclose(ptr);
        return 1;  // Successfully read and inserted words
    } else {
        printf("Error!!! The file can't be opened.\n");
        return 0;  // Failed to open the file
    }
}
int readFile() {

    FILE *ptr;


    ptr = fopen("input.txt", "r");
    char str[MAX_STRING_LENGTH];

    if (ptr != NULL) {
         while (fscanf(ptr, "%30s", str) == 1) {
            // Keep only the first 31 characters of each line
            str[31] = '\0';

            // Print the modified line
            printf("%s\n", str);
             int c;
             while ((c = fgetc(ptr)) != EOF && c != '\n');
        }

        fclose(ptr); // Close the file after reading
    } else {
        printf("Error opening the file");
    }
}


void writeOnFile(char *file ){FILE *ptr;
    char s[MAX_STRING_LENGTH];

    printf("Enter text to append to the file: ");
    scanf("%s",&s);

    ptr = fopen(file, "a");
    fprintf(ptr, "%s\n", s);
    fclose(ptr);
}
void radixSort() {
    int maxLength = 0;
    // Find the maximum length of a string in the list
    node* current = head;
    while (current != NULL) {
        int length = strlen(current->data);
        if (length > maxLength) {
            maxLength = length;
        }
        current = current->next;
    }

    // Create buckets for each character (65 for space, 10 for numbers, 26 for uppercase letters, and 26 for lowercase letters)
    node* buckets[65];
    for (int i = 0; i < 65; i++) {
        buckets[i] = NULL;
    }

    // Sort strings based on each character from right to left
    for (int digit = maxLength - 1; digit >= 0; digit--) {
        // Distribute strings to appropriate buckets
        current = head;
        while (current != NULL) {
            char currentChar = (digit >= strlen(current->data)) ? '\0' : current->data[digit];
            int index;

            if (currentChar == ' ') {
                // Space
                index = 0;
            } else if (currentChar >= '0' && currentChar <= '9') {
                // Numbers
                index = currentChar - '0' + 1;
            } else if (currentChar >= 'A' && currentChar <= 'Z') {
                // Uppercase letters
                index = currentChar - 'A' + 11;
            } else if (currentChar >= 'a' && currentChar <= 'z') {
                // Lowercase letters
                index = currentChar - 'a' + 37;
            } else {
                // Other characters
                index =64; // Place them at the end of the buckets
            }

            // Save the next node before changing current->next
            node* nextNode = current->next;

            // Link current node into the appropriate bucket
            current->next = buckets[index];
            current->prev = NULL;
            if (buckets[index] != NULL) {
                buckets[index]->prev = current;
            }
            buckets[index] = current;

            current = nextNode;
        }

        // Repopulate the linked list from buckets
        head = NULL;
        for (int i = 64; i >= 0; i--) {
            // Traverse the bucket list and insert into the main list
            while (buckets[i] != NULL) {
                node* temp = buckets[i];
                buckets[i] = buckets[i]->next;

                // Insert at the beginning of the main list
                temp->next = head;
                temp->prev = NULL;
                if (head != NULL) {
                    head->prev = temp;
                }
                head = temp;
            }

            // Clear the linked list associated with the current bucket
            makeEmpty(&buckets[i]);
        }
    }
}

void writeFile(const char* filename) {
    FILE* ptr = fopen(filename, "w");
    if (ptr != NULL) {
        struct node* current = head;
        while (current != NULL) {
            // Remove newline characters before writing to the file
            size_t len = strlen(current->data);
            for (size_t i = 0; i < len; i++) {
                if (current->data[i] != '\n') {
                    fprintf(ptr, "%c", current->data[i]);
                }
            }
            fprintf(ptr, " "); // Add space between strings
            current = current->next;
        }

        fclose(ptr);
        printf("Strings written to output file successfully\n");
    } else {
        printf("Error opening file\n");
    }
}


void displayMainMenu(){
    printf("\nchose from the menu \n");
    printf("1-Load the string\n");
    printf("2-print the strings befor sorting \n");
    printf("3-sort the string\n");
    printf("4-print the sorted string \n");
    printf("5-Add a new word to the list of sorted strings (and sort it)\n");
    printf("6-delete a word from sorted string\n");
    printf("7-save the output file \n");
    printf("8- exit \n");

}
int main() {
    int key = 1;
    head = NULL;
    const char* input = "input.txt";
    const char* output = "output.txt";

    printf("welcome\n");

    while (key != 0) {
        displayMainMenu();
        scanf("%d", &key);
        switch (key) {
            case 1 :
                if(  insertFromFile(input)==1){
                    insertFromFile(input);
                    printf("the data are loaded ");
                }


                break;


            case 2:
                if(isEmpty(head)==1){
                    printf("the list is empty !! load the data first ");
                } else{
                    printf("Data in the linked list before sorting are:\n");
                    readFile();

                }
                //readFile(stringFile);

                break;
            case 3:
                if(isEmpty(head)==1){
                    printf("the list is empty!! fill it with data first");
                } else {
                    radixSort();
                }

                break;
            case 6:
                if(isEmpty(head)==1){
                    printf("the list is empty!! fill it with data first");
                } else{


                    printf("enter the word you want to delete\n");
                    char *s[MAX_STRING_LENGTH];
                    scanf("%s",&s);
                    delete(&head,s);
                    printf("the word has been deleted succesfully");

                }
                break;
            case 4:
                if(isEmpty(head)==1){
                    printf("the list is empty !! load the data first ");
                } else{
                    printf("Data in the linked list after sorting are:\n");
                    display(head);
                }

                break;
            case 5:
                writeOnFile(input);
                insertFromFile(input);
                radixSort();


                break;
            case 7:
                if(isEmpty(head)==1){
                    printf("the list is empty!! fill it with data first");
                } else {
                    writeFile(output);
                }
                break;
            case 8:

                exit(0);
            default:
                printf("no such operation!\tplease try again\n");
                displayMainMenu();

        }}

    return 0;
}