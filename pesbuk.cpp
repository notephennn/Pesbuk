#include <stdio.h> 
#include <string.h> 
#include <stdlib.h> 
#include <ctype.h> 
#include <time.h> 
#define MAX 100 

typedef struct Friend {
    char username[MAX]; 
    struct Friend *next; 
} Friend; 

typedef struct User {
    char username[MAX]; 
    char full_name[MAX]; 
    char birth_date[MAX]; 
    Friend *friend; 
    struct User *next; 
} User; 

User *hashTable[MAX]; 

int midSquareHash(const char *username){
    int sum = 0; 
    for ( int i = 0; username[i] != '\0'; i++ ){
        sum += username[i]; 
    }

    int squared = sum * sum; 
    int mid = ( squared * 100 ) % 100; 
    return mid % MAX; 
}

int isvalidUsername(const char *username){
    int len = strlen(username); 
    if ( len < 5 || len > 20 ) return 0; 
    for ( int i = 0; i < len; i++){
        if ( !isalnum(username[i]) && username[i] != '_') return 0; 
    }

    return 1; 
}

int isValidFullName(const char *fullName){
    int len = strlen(full_name);
    if (len < 1 || len > 50) return 0;
    for (int i = 0; i < len; i++) {
        if (!isalpha(full_name[i]) && full_name[i] != ' ') return 0;
    }
    return 1;
}

int isValidDate(const char *birth_date) {
    int day, month, year;
    if (sscanf(birth_date, "%d/%d/%d", &day, &month, &year) != 3) return 0;
    if (day < 1 || day > 30 || month < 1 || month > 12 || year <= 0) return 0;
    return 1;
}

User *user(const char *username){
    int index = midSquareHash(username); 
    User *temp = hashTable[index]; 
    while ( temp ) {
        if ( strcmp(temp->username, username) == 0 ) return temp; 
        temp = temp->next; 
    }

    return NULL; 
}

void menu_register(){
    char username[MAX], full_name[MAX], birth_date[MAX], confirm;
    do {
        printf("Enter username: ");
        scanf("%s", username);
        if (!isValidUsername(username)) {
            printf("Invalid username! Must be 5-20 characters and only contain letters, numbers, or underscores.\n");
            continue;
        }
        if (findUser(username)) {
            printf("Username already taken!\n");
            continue;
        }
        break;
    } while (1);


    do {
        printf("Enter full name: ");
        fgets(full_name, MAX, stdin);
        full_name[strcspn(full_name, "\n")] = 0;
        if (!isValidFullName(full_name)) {
            printf("Invalid full name! Must be 1-50 characters and only contain letters and spaces.\n");
            continue;
        }
        break;
    } while (1);
    
    do {
        printf("Enter birth date (dd/mm/yyyy): ");
        scanf("%s", birth_date);
        if (!isValidDate(birth_date)) {
            printf("Invalid birth date! Must be in format dd/mm/yyyy with valid values.\n");
            continue;
        }
        break;
    } while (1);
    
    do {
        printf("Confirm registration (Y/N): ");
        scanf(" %c", &confirm);
        confirm = toupper(confirm);
        if (confirm == 'N') return;
    } while (confirm != 'Y');


    int index = midSquareHash(username);
    User *newUser = (User *)malloc(sizeof(User));
    strcpy(newUser->username, username);
    strcpy(newUser->full_name, full_name);
    strcpy(newUser->birth_date, birth_date);
    newUser->friend = NULL;
    newUser->next = NULL;


    if ( !hashTable[index]){
        hashTable[index] = newUser; 
    } else {
        User *temp = hashTable[index]; 
        while ( temp->next != NULL ) temp = temp->next; 
        temp->next = newUser; 
    }

    printf("Account registered successfully!\n");
}

int countFriends(Friend *friendlist){
    int count = 0; 
    while ( friendList ){
        count++; 
        friendlist = friendlist->next; 
    }

    return count; 
}

int isAlreadyFriend(Friend *friendList, const char *username) {
    while (friendList) {
        if (strcmp(friendList->username, username) == 0) return 1;
        friendList = friendList->next;
    }
    return 0;
}

void addFriendToList(Friend **friendList, const char *friendUsername) {
    Friend *newFriend = (Friend *)malloc(sizeof(Friend));
    strcpy(newFriend->username, friendUsername);
    newFriend->next = NULL;
    if (!(*friendList)) {
        *friendList = newFriend;
    } else {
        Friend *temp = *friendList;
        while (temp->next) temp = temp->next;
        temp->next = newFriend;
    }
}

void menu_add() {
    char username[MAX], friendUsername[MAX], confirm;
    printf("Enter your username: ");
    scanf("%s", username);
    User *user = findUser(username);
    if (!user) {
        printf("User not found!\n");
        return;
    }

    if (countFriends(user->friend) >= 10) {
        printf("You already have too many friends\n");
        return;
    }

    printf("Enter username to befriend: ");
    scanf("%s", friendUsername);
    User *friendUser = findUser(friendUsername);
    if (!friendUser) {
        printf("User not found!\n");
        return;
    }
    if (strcmp(username, friendUsername) == 0) {
        printf("You cannot befriend yourself!\n");
        return;
    }
    if (isAlreadyFriend(user->friend, friendUsername)) {
        printf("You are already friends with this user!\n");
        return;
    }
    if (countFriends(friendUser->friend) >= 10) {
        printf("The user already has too many friends\n");
        return;
    }

    do {
        printf("Confirm adding friend (Y/N): ");
        scanf(" %c", &confirm);
        confirm = toupper(confirm);
        if (confirm == 'N') return;
    } while (confirm != 'Y');

    addFriendToList(&user->friend, friendUsername);
    addFriendToList(&friendUser->friend, username);

    printf("Friend added successfully!\n");
}

void menu_show(){

}

void menu_delete(){

}

void clearHashTable(){
    for ( int i = 0; i < MAX; i++ ){
        while ( hashTable[i]){
            User *temp = hashTable[i]; 
            hashTable[i] = temp->next; 
            free(temp); 
        }
    }

    printf("Data has been clear\n"); 
}

int main(){
    while(1){
        int option; 
        do{
            puts("-----PESBUK-----"); 
            puts("1. Register Account"); 
            puts("2. Add Friends"); 
            puts("3. Show Friends birthday"); 
            puts("4. Delete Account"); 
            puts("5. Exit"); 
            printf("Input your choice: "); 
            scanf("%d", &option); 
            getchar(); 
        } while ( option < 1 || option > 5); 

        if ( option == 1 ) menu_register(); 
        else if ( option == 2 ) menu_add(); 
        else if ( option == 3 ) menu_view(); 
        else if ( option == 4) menu_delete(); 
        else if ( option == 5 ){
            clearHashTable(); 
            return 0; 
        }
    }
}