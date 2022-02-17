#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include <time.h>
#include "cJSON.h"
char name[20] ;
char buffer[1000] ;
int sam = 0 ;
char token[40] ;
int client_socket ;
char response[100] ;
int initialize(int port);
void start(int server_socket);
void signup();
void login();
int main () {
    int port = 12345;
    int server_socket = initialize(port) ;
    if(server_socket != INVALID_SOCKET) {
        start(server_socket) ;
    }
}
int initialize(int port){
    WSADATA wsadata;
    int wsa_res = WSAStartup(MAKEWORD(2, 2),&wsadata);
    if (wsa_res != 0){
        printf("Failed to run wsadata\n");
        return INVALID_SOCKET;
    }
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == INVALID_SOCKET){
        printf("Could not create socket\n");
        return INVALID_SOCKET;
    }
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(port);
    int bind_res = bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr));
    if (bind_res == SOCKET_ERROR) {
        printf("Bind failed\n");
        return INVALID_SOCKET;
    }
    listen (server_socket, SOMAXCONN);
    return server_socket;
}
void start (int server_socket ) {
    int len = sizeof(struct sockaddr_in);
    struct sockaddr_in client_addr;
    char *messages[1000];
    int size = 0;
    while ((client_socket = accept(server_socket, (struct sockaddr *) &client_addr, &len)) != INVALID_SOCKET) {
        memset(buffer, 0, 1000);
        recv(client_socket, buffer, 999, 0);
        if (buffer[0] == 'l' && buffer[1] == 'o') {
            FILE *pointer_file;
            int i = 6, index, j = 0;
            char password[20], filename[25];
            memset(name, 0, 20);
            memset(password, 0, 20);
            memset(filename, 0, 25);
            while (buffer[i] != ',') {
                name[j] = buffer[i];
                i++;
                j++;
            }
            i++;
            j = 0;
            while (buffer[i] != '\n') {
                password[j] = buffer[i];
                i++;
                j++;
            }
            strcpy(filename, name);
            strcat(filename, ".json");
            pointer_file = fopen(filename, "r");
            if (pointer_file != NULL) {
                char *Json = malloc(200);
                cJSON *cjson_test1 = NULL;
                cJSON *cjson_password = NULL;
                char *information_of_file = malloc(100000);
                char new_information_of_file[100000];
                memset(information_of_file, 0, 99999);
                while (fgets(information_of_file, sizeof(information_of_file), pointer_file) != NULL) {
                    strcat(new_information_of_file, information_of_file);
                }
                free(information_of_file);
                cjson_test1 = cJSON_Parse(new_information_of_file);
                cjson_password = cJSON_GetObjectItem(cjson_test1, "password");
                if (strcmp(password, cjson_password->valuestring) == 0) {
                    for (int i = 0; i <= 31; i++) {
                        token[i] = '1';
                    }
                    cJSON *cjson_test2 = NULL;
                    cjson_test2 = cJSON_CreateObject();
                    cJSON_AddStringToObject(cjson_test2, "type", "Token");
                    cJSON_AddStringToObject(cjson_test2, "message", token);
                    char *str = NULL;
                    printf("\nUser Login Successfully!\n");
                    str = cJSON_PrintUnformatted(cjson_test2);
                    send(client_socket, str, strlen(str), 0);
                    closesocket(client_socket);
                } else {
                    printf("\nPassword is not correct!\n");
                    response[27] = 'I' ;
                    response[9] = 'F' ;
                    send(client_socket, response, strlen(response), 0);
                    closesocket(client_socket);
                }
            } else {
                printf("\nusername existed!\n");
                response[27] = 'Y';
                response[9] = 'F';
                send(client_socket, response, strlen(response), 0);
                closesocket(client_socket);
            }
        } else if (buffer[0] == 's' && buffer[1] == 'i') {
            int i = 7,  index;
            char name1[20], password[20], filename[25] , idchar[10];
            memset(idchar,0,10) ;
            memset(name1, 0, 20);
            memset(password, 0, 20);
            memset(filename, 0, 25);
            int j = 0;
            FILE *pointer_file;
            while (buffer[i] != ',') {
                name1[j] = buffer[i];
                i++;
                j++;
            }
            i++;
            j = 0;
            while (buffer[i] != '\n') {
                password[j] = buffer[i];
                j++;
                i++;
            }
            strcpy(filename, name1);
            strcat(filename, ".json");
            pointer_file = fopen(filename, "r");
            if (pointer_file == NULL) {
                cJSON *cjson_test = NULL;
                cJSON *cjson_username = NULL;
                cJSON *cjson_password = NULL;
                cJSON *cjson_followers = NULL;
                cJSON *cjson_followings = NULL;
                cJSON *cjson_personalTweets = NULL;
                char *str = NULL;
                cjson_test = cJSON_CreateObject();
                cJSON_AddStringToObject(cjson_test, "username", name1);
                cJSON_AddStringToObject(cjson_test, "password", password);
                cJSON_AddStringToObject(cjson_test, "bio" , "");
                cJSON_AddArrayToObject(cjson_test, "followers");
                cJSON_AddArrayToObject(cjson_test, "followings");
                cJSON_AddArrayToObject(cjson_test, "personalTweets");
                str = cJSON_PrintUnformatted(cjson_test);
                pointer_file = fopen(filename, "w");
                fputs(str, pointer_file);
                fclose(pointer_file);
                response[9] = 'S';
                printf("account created!");
                send(client_socket, str, strlen(str), 0);
                closesocket(client_socket);
            } else {
                printf("user name exited!");
                response[9] = 'E';
                send(client_socket, response, strlen(response), 0);
                closesocket(client_socket);
            }



        } else if (buffer[0] == 's' && buffer[1] == 'e' && buffer[2] == 'n') {
            FILE *pointer_file;
            char entertoken[100], message[100], filename[100], idchar[10];
            memset(idchar,0,10) ;
            memset(entertoken, 0, 100);
            memset(message,0,100) ;
            memset(filename,0,100) ;
            int j = 0, id;
            for (int i = 11; i <= 42; i++) {
                entertoken[j] = buffer[i];
                j++;
            }
            int p = 44;
            j = 0;
            while (buffer[p] != '\n') {
                message[j] = buffer[p];
                p++;
                j++;
            }
            if (strcmp(entertoken, token) == 0) {
                for (int z = 0; z <= 100; z++) {
                    sprintf(filename, "%d", z);
                    strcat(filename, "tweet");
                    strcat(filename, ".json");
                    pointer_file = fopen(filename, "r");
                    if (pointer_file == NULL) {
                        id = z;
                        break;
                    }
                }
                sprintf(idchar, "%d", id);
                printf ("%s" , idchar) ;
                fclose(pointer_file);
                pointer_file = fopen(filename, "w");
                cJSON *cjson_test2 = NULL;
                char *str = NULL;
                cjson_test2 = cJSON_CreateObject();
                cJSON_AddNumberToObject(cjson_test2, "id", id);
                cJSON_AddStringToObject(cjson_test2, "author", name);
                cJSON_AddStringToObject(cjson_test2, "content", message);
                cJSON_AddObjectToObject(cjson_test2, "comments");
                cJSON_AddNumberToObject(cjson_test2, "likes", 0);
                str = cJSON_PrintUnformatted(cjson_test2);
                printf("Tweet %s Send.", message) ;
                str = cJSON_PrintUnformatted(cjson_test2);
                fputs(str, pointer_file);
                fclose(pointer_file);
                memset(filename,0,sizeof(filename)) ;
                strcpy(filename,name) ;
                strcat(filename,".json") ;
                pointer_file = fopen(filename,"r") ;
                char *information_of_file = malloc(100000);
                char new_information_of_file[100000];
                memset(new_information_of_file,0,100000) ;
                memset(information_of_file, 0, 100000);
                while (fgets(information_of_file, sizeof(information_of_file), pointer_file) != NULL) {
                    strcat(new_information_of_file, information_of_file);
                }
                printf( "%s\n" , new_information_of_file) ;
                free(information_of_file);
                fclose(pointer_file) ;
                int cjson_followers_size = 0, cjson_following_size = 0, personaltweet = 0;
                cJSON *personaltweetsitem = NULL;
                cJSON *followersitem = NULL;
                cJSON *followingsitem = NULL;
                cJSON *follow = NULL;
                cJSON *cjson_username = NULL;
                cJSON *cjson_password = NULL;
                cJSON *cjson_bio = NULL;
                cJSON *cjson_followers = NULL;
                cJSON *cjson_followings = NULL;
                cJSON *cjson_personaltweetss = NULL;
                cJSON *personalTweets = NULL;
                follow = cJSON_Parse(new_information_of_file);
                cjson_username = cJSON_GetObjectItem(follow, "username");
                cjson_password = cJSON_GetObjectItem(follow, "password");
                cjson_bio = cJSON_GetObjectItem(follow, "bio");
                cjson_followers = cJSON_GetObjectItem(follow, "followers");
                cjson_followers_size = cJSON_GetArraySize(cjson_followers);
                char followerss[10][20];
                for (int i = 0; i < cjson_followers_size; i++) {
                    followersitem = cJSON_GetArrayItem(cjson_followers, i);
                    strcpy(followerss[i], followersitem->valuestring);
                }
                cjson_followings = cJSON_GetObjectItem(follow, "followings");
                char followingss[10][20];
                cjson_following_size = cJSON_GetArraySize(cjson_followings);
                for (int i = 0; i < cjson_following_size; i++) {
                    followingsitem = cJSON_GetArrayItem(cjson_followings, i);
                    strcpy(followingss[i], followingsitem->valuestring);
                }
                char personaltweetss[10][100];
                personaltweetsitem = cJSON_GetObjectItem(follow, "personalTweets");
                personaltweet = cJSON_GetArraySize(cjson_followings);
                for (int i = 0; i < personaltweet; i++) {
                    personaltweetsitem = cJSON_GetArrayItem(personaltweetsitem, i);
                    strcpy(personaltweetss[i], personaltweetsitem->valuestring);
                }
                cJSON *cjson_followers13 = NULL;
                cJSON *cjson_followings13 = NULL;
                cJSON *cjson_personal13 = NULL;
                cJSON *follow2 = cJSON_CreateObject();
                cJSON_AddStringToObject(follow2, "username", cjson_username->valuestring);
                cJSON_AddStringToObject(follow2, "password", cjson_password->valuestring);
                cJSON_AddStringToObject(follow2, "bio", cjson_bio->valuestring) ;
                cjson_followers13 = cJSON_CreateArray() ;
                for (int i = 0 ; i < cjson_followers_size; i++) {
                    cJSON_AddItemToArray(cjson_followers13, cJSON_CreateString(followerss[i]));
                }
                cJSON_AddItemToObject(follow2, "followers", cjson_followers13);
                cjson_followings13 = cJSON_CreateArray();
                for (int i = 0; i < cjson_following_size; i++) {
                    cJSON_AddItemToArray(cjson_followings13, cJSON_CreateString(followingss[i]));
                }
                cJSON_AddItemToObject(follow2, "followings", cjson_followings13);
                cjson_personal13 = cJSON_CreateObject() ;
                for (int i = 0 ; i < personaltweet; i++) {
                    cJSON_AddItemToArray(cjson_personal13, cJSON_CreateString(personaltweetss[i]));
                }
                cJSON_AddItemToArray(cjson_personal13,cJSON_CreateString(idchar)) ;
                cJSON_AddItemToObject(follow2, "personalTweets", cjson_personal13);
                pointer_file = fopen(filename , "w");
                char *str1 = cJSON_PrintUnformatted(follow2);
                fputs(str1,pointer_file) ;
                fclose(pointer_file) ;
                closesocket(client_socket);
            } else {
                closesocket(client_socket);
                printf("Invalid token Try Again!");
            }
        } else if (buffer[0] == 'l' && buffer[1] == 'i') {
            char entertoken[200], id[20], filename[100], response1[20];
            memset(response1,0,20) ;
            memset(entertoken,0,200) ;
            memset(filename,0,100) ;
            memset(id,0,20) ;
            cJSON *cjson_test3 = NULL;
            cJSON *cjson_like = NULL;
            int j = 0, i = 5, counter = 0 ;
            FILE *file_open;
            for (int i = 5; i <= 36; i++) {
                entertoken[j] = buffer[i];
                j++;
            }
            i = 38;
            j = 0;
            while (buffer[i] != '\n') {
                id[j] = buffer[i];
                i++;
                j++;
            }
            if (strcmp(entertoken, token) == 0) {
                strcpy(filename, id);
                strcat(filename, "tweet");
                strcat(filename, ".json");
                file_open = fopen(filename, "r");
                if (file_open == NULL) {
                    response1[0] = 'i' ;
                    printf("There is no tweet with this id!");
                    send(client_socket, response1 , strlen(response1) , 0) ;
                    closesocket(client_socket);
                } else {
                    char *information_of_file = calloc(1000 , sizeof(char));
                    char new_information_of_file[1000];
                    memset(information_of_file, 0, 1000);
                    memset(new_information_of_file,0,1000) ;
                    while (fgets(information_of_file, sizeof(information_of_file), file_open) != NULL) {
                        strcat(new_information_of_file, information_of_file);
                    }
                    free(information_of_file);
                    fclose(file_open);
                    cJSON *root = cJSON_Parse(new_information_of_file);
                    cJSON *id11 = cJSON_GetObjectItem(root, "id");
                    cJSON *author = cJSON_GetObjectItem(root, "author");
                    cJSON *content = cJSON_GetObjectItem(root, "content");
                    cJSON *Comment1 = cJSON_GetObjectItem(root, "comments");
                    cJSON *like = cJSON_GetObjectItem(root, "likes");
                    char * x = malloc(100000);
                    memset(x, 0, 100000);
                    x = cJSON_Print(Comment1) ;
                    int counter1 = 0;
                    for (int i = 0; i < strlen(x); i++) {
                        if (*(x + i) == ':') {
                            counter1++; }
                    }
                    int j = 0;
                    char **xx = malloc(100000);
                    int jj = 0;
                    char *tok = strtok(x, "\"");
                    while (tok != NULL) {
                        if (j % 2 == 1) {
                            *(xx + jj) = malloc(1000);
                            memset(*(xx + jj), 0, 1000);
                            sprintf(*(xx + jj), tok);
                            jj++;
                        }
                        tok = strtok(NULL, "\"");
                        j++;
                    }
                    cJSON *root1 = cJSON_CreateObject();
                    cJSON *id14 = cJSON_CreateNumber(id11->valueint);
                    cJSON_AddItemToObject(root1, "id", id14);
                    cJSON *author14 = cJSON_CreateString(author->valuestring);
                    cJSON_AddItemToObject(root1, "author", author14);
                    cJSON *content14 = cJSON_CreateString(content->valuestring);
                    cJSON_AddItemToObject(root1, "content", content14);
                    cJSON *Comments = cJSON_CreateObject();
                    cJSON_AddItemToObject(root1, "comments", Comments);
                    for (int i = 0; i < 2 * counter1; i++) {
                        cJSON_AddStringToObject(Comments, *(xx+i), *(xx+i+1));
                        i++;
                    }
                    cJSON *like13 = cJSON_CreateNumber(like->valueint+1);
                    cJSON_AddItemToObject(root1, "likes", like13);
                    free(x);
                    for (int i = 0; i < 2 * counter1; i++) {
                        free(*(xx + i));
                    }
                    free(xx);
                    char *Send = cJSON_PrintUnformatted(root1);
                    file_open = fopen(filename, "w");
                    fprintf(file_open, "%s", Send);
                    fclose(file_open);
                    response1[0] = 'W' ;
                    send(client_socket,response1,sizeof(response1) ,0) ;
                    closesocket(client_socket);
                }
            } else {
                printf("Invalid Socket!");
                closesocket(client_socket);
            }
        } else if (buffer[0] == 'c' && buffer[1] != 'h') {
            FILE *file_open;
            char newtoken[100], id[30], message[200], filename[200] , receve1[10];
            memset(filename, 0, 200);
            memset(newtoken, 0, 100);
            memset(id, 0, 30);
            memset(message, 0, 200);
            int i = 0, j = 0;
            i = 7;
            for (int i = 8; i <= 39; i++) {
                newtoken[j] = buffer[i];
                j++;
            }
            i = 41;
            j = 0;
            while (buffer[i] != ',') {
                id[j] = buffer[i];
                j++;
                i++;
            }
            i++;
            j = 0;
            while (buffer[i] != '\n') {
                message[j] = buffer[i];
                i++;
                j++;
            }
            strcpy(filename, id);
            strcat(filename, "tweet");
            strcat(filename, ".json");
            if (strcmp(newtoken, token) == 0) {
                file_open = fopen(filename, "r");
                if (file_open == NULL) {
                    receve1[0] = 'N' ;
                    send(client_socket ,receve1 , strlen(receve1) , 0) ;
                    printf("Tweet is not existed");
                    closesocket(client_socket);
                } else {
                    char *information_of_file = malloc(100000);
                    char new_information_of_file[100000];
                    memset(information_of_file, 0, 99999);
                    memset(new_information_of_file,0,100000) ;
                    while (fgets(information_of_file, sizeof(information_of_file), file_open) != NULL) {
                        strcat(new_information_of_file, information_of_file);
                    }
                    free(information_of_file);
                    fclose(file_open);
                    cJSON *root1 = cJSON_Parse(new_information_of_file);
                    cJSON *id11 = cJSON_GetObjectItem(root1, "id");
                    cJSON *author11 = cJSON_GetObjectItem(root1, "author");
                    cJSON *content11 = cJSON_GetObjectItem(root1, "content");
                    cJSON *Comment11 = cJSON_GetObjectItem(root1, "comments");
                    cJSON *like11 = cJSON_GetObjectItem(root1, "likes");
                    char *x = malloc(100000);
                    memset(x, 0, 100000);
                    x = cJSON_Print(Comment11);
                    int counter1 = 0;
                    for (int i = 0; i < strlen(x); i++) {
                        if (*(x + i) == ':') counter1++;
                    }
                    j = 0;
                    char **xx = malloc(100000);
                    int jj = 0;
                    char *tok = strtok(x, "\"");
                    while (tok != NULL) {
                        if (j % 2 == 1) {
                            *(xx + jj) = malloc(1000);
                            sprintf(*(xx + jj), tok);
                            jj++;
                        }
                        tok = strtok(NULL, "\"");
                        j++;
                    }
                    cJSON *root = cJSON_CreateObject();
                    cJSON *id13 = cJSON_CreateNumber(id11->valueint);
                    cJSON_AddItemToObject(root, "id", id13);
                    cJSON *author13 = cJSON_CreateString(author11->valuestring);
                    cJSON_AddItemToObject(root, "author", author13);
                    cJSON *Content13 = cJSON_CreateString(content11->valuestring);
                    cJSON *Comments = cJSON_CreateObject();
                    cJSON_AddItemToObject(root, "comments", Comments);
                    for (int i = 0; i < 2 * counter1; i++) {
                        cJSON_AddStringToObject(Comments, *(xx + i), *(xx + i + 1));
                        i = i + 1;
                    }
                    cJSON_AddStringToObject(Comments, name, message);
                    cJSON *like13 = cJSON_CreateNumber(like11->valueint);
                    cJSON_AddItemToObject(root, "likes", like13);
                    free(x);
                    for (int i = 0; i < 2 * counter1; i++) {
                        free(*(xx + i));
                    }
                    free(xx);
                    char *Send_data = cJSON_PrintUnformatted(root);
                    file_open = fopen(filename, "w");
                    fprintf(file_open, "%s", Send_data);
                    fclose(file_open);
                    printf("Comment Added to tweet\n");
                    receve1[0] = 'C' ;
                    send(client_socket ,receve1 , strlen(receve1) , 0) ;
                    closesocket(client_socket);
                }
            } else {
                printf("Invalid Token");
                closesocket(client_socket) ;
            }
        } else if (buffer[0] == 'f') {
            char newtoken[40], user[50], filename[100] , Send[10];
            FILE *Pointer;
            memset(filename, 0, 100);
            memset(newtoken, 0, 40);
            memset(user, 0, 50);
            int i = 7, j = 0;
            for (int i = 7; i <= 38; i++) {
                newtoken[j] = buffer[i];
                j++;
            }
            i = 40;
            j = 0;
            while (buffer[i] != '\n') {
                user[j] = buffer[i];
                j++;
                i++;
            }
            strcpy(filename, user);
            strcat(filename, ".json");
            if (strcmp(newtoken, token) == 0) {
                Pointer = fopen(filename, "r");
                if (Pointer == NULL) {
                    Send[0] = 'n' ;
                    fclose(Pointer);
                    printf("Username does not existed!");
                    send(client_socket,Send,strlen(Send) ,0) ;
                    closesocket(client_socket);
                } else {
                    char *information_of_file = malloc(100000);
                    char new_information_of_file[100000];
                    memset(new_information_of_file,0,100000) ;
                    memset(information_of_file, 0, 99999);
                    while (fgets(information_of_file, sizeof(information_of_file), Pointer) != NULL) {
                        strcat(new_information_of_file, information_of_file);
                    }
                    free(information_of_file);
                    fclose(Pointer);
                    int cjson_followers_size = 0, cjson_following_size = 0, personaltweet = 0;
                    cJSON *personaltweetsitem = NULL;
                    cJSON *followersitem = NULL;
                    cJSON *followingsitem = NULL;
                    cJSON *follow = NULL;
                    cJSON *cjson_username = NULL;
                    cJSON *cjson_password = NULL;
                    cJSON *cjson_bio = NULL;
                    cJSON *cjson_followers = NULL;
                    cJSON *cjson_followings = NULL;
                    cJSON *cjson_personaltweetss = NULL;
                    cJSON *personalTweets = NULL;
                    follow = cJSON_Parse(new_information_of_file);
                    cjson_username = cJSON_GetObjectItem(follow, "username");
                    cjson_password = cJSON_GetObjectItem(follow, "password");
                    cjson_bio = cJSON_GetObjectItem(follow, "bio");
                    cjson_followers = cJSON_GetObjectItem(follow, "followers");
                    cjson_followers_size = cJSON_GetArraySize(cjson_followers);
                    char followerss[10][20];
                    for (int i = 0; i < cjson_followers_size; i++) {
                        followersitem = cJSON_GetArrayItem(cjson_followers, i);
                        strcpy(followerss[i], followersitem->valuestring);
                    }
                    cjson_followings = cJSON_GetObjectItem(follow, "followings");
                    char followingss[10][20];
                    cjson_following_size = cJSON_GetArraySize(cjson_followings);
                    for (int i = 0; i < cjson_following_size; i++) {
                        followingsitem = cJSON_GetArrayItem(cjson_followings, i);
                        strcpy(followingss[i], followingsitem->valuestring);
                    }
                    char personaltweetss[10][100];
                    personaltweetsitem = cJSON_GetObjectItem(follow, "personalTweets");
                    personaltweet = cJSON_GetArraySize(cjson_followings);
                    for (int i = 0; i < personaltweet; i++) {
                        personaltweetsitem = cJSON_GetArrayItem(personaltweetsitem, i);
                        strcpy(personaltweetss[i], personaltweetsitem->valuestring);
                    }
                    int z = 0;
                    for (int i = 0; i <= cjson_followers_size; i++) {
                        if (strcmp(followerss[i], name) == 0) {
                            printf("already followed!");
                            for(int i = 0 ; i < 20 ; i++) {
                                Send[i] = 'a' ;
                            }
                            send(client_socket,Send,strlen(Send) ,0) ;
                            closesocket(client_socket);
                            z = 1 ;
                        }
                    }
                    if (z == 1) {

                    } else {
                        cJSON *cjson_followers13 = NULL;
                        cJSON *cjson_followings13 = NULL;
                        cJSON *cjson_personal13 = NULL;
                        cJSON *follow2 = cJSON_CreateObject();
                        cJSON_AddStringToObject(follow2, "username", cjson_username->valuestring);
                        cJSON_AddStringToObject(follow2, "password", cjson_password->valuestring);
                        cJSON_AddStringToObject(follow2, "bio", cjson_bio->valuestring);
                        cjson_followers13 = cJSON_CreateArray();
                        for (int i = 0; i < cjson_followers_size; i++) {
                            cJSON_AddItemToArray(cjson_followers13, cJSON_CreateString(followerss[i]));
                        }
                        cJSON_AddItemToArray(cjson_followers13, cJSON_CreateString(name));
                        cJSON_AddItemToObject(follow2, "followers", cjson_followers13);
                        cjson_followings13 = cJSON_CreateArray();
                        for (int i = 0; i < cjson_following_size; i++) {
                            cJSON_AddItemToArray(cjson_followings13, cJSON_CreateString(followingss[i]));
                        }
                        cJSON_AddItemToObject(follow2, "followings", cjson_followings13);
                        cjson_personal13 = cJSON_CreateObject();
                        for (int i = 0; i < personaltweet; i++) {
                            cJSON_AddItemToArray(cjson_personal13, cJSON_CreateString(personaltweetss[i]));
                        }
                        cJSON_AddItemToObject(follow2, "personalTweets", cjson_personal13);
                        Pointer = fopen(filename, "w");
                        char *str = cJSON_PrintUnformatted(follow2);
                        fputs(str, Pointer);
                        fclose(Pointer);
                        char follow3[100];
                        memset(follow3, 0, 100);
                        strcpy(follow3, name);
                        strcat(follow3, ".json");
                        Pointer = fopen(follow3, "r");
                        char *information_of_file1 = malloc(100000);
                        char new_information_of_file1[100000];
                        memset(new_information_of_file1, 0, 100000);
                        memset(information_of_file1, 0, 99999);
                        while (fgets(information_of_file1, sizeof(information_of_file1), Pointer) != NULL) {
                            strcat(new_information_of_file1, information_of_file1);
                        }
                        free(information_of_file1);
                        fclose(Pointer);
                        int cjson_followers_size1 = 0, cjson_following_size1 = 0, personaltweet1 = 0;
                        cJSON *personaltweetsitem1 = NULL;
                        cJSON *followersitem1 = NULL;
                        cJSON *followingsitem1 = NULL;
                        cJSON *follow1 = NULL;
                        cJSON *cjson_username1 = NULL;
                        cJSON *cjson_password1 = NULL;
                        cJSON *cjson_bio1 = NULL;
                        cJSON *cjson_followers1 = NULL;
                        cJSON *cjson_followings1 = NULL;
                        cJSON *cjson_personaltweetss1 = NULL;
                        cJSON *personalTweets1 = NULL;
                        follow1 = cJSON_Parse(new_information_of_file1);
                        cjson_username1 = cJSON_GetObjectItem(follow1, "username");
                        cjson_password1 = cJSON_GetObjectItem(follow1, "password");
                        cjson_bio1 = cJSON_GetObjectItem(follow1, "bio");
                        cjson_followers1 = cJSON_GetObjectItem(follow1, "followers");
                        cjson_followers_size1 = cJSON_GetArraySize(cjson_followers1);
                        char followerss1[10][20];
                        for (int i = 0; i < cjson_followers_size1; i++) {
                            followersitem1 = cJSON_GetArrayItem(cjson_followers1, i);
                            strcpy(followerss1[i], followersitem1->valuestring);
                        }
                        cjson_followings1 = cJSON_GetObjectItem(follow1, "followings");
                        char followingss1[10][20];
                        cjson_following_size1 = cJSON_GetArraySize(cjson_followings1);
                        for (int i = 0; i < cjson_following_size1; i++) {
                            followingsitem1 = cJSON_GetArrayItem(cjson_followings1, i);
                            strcpy(followingss1[i], followingsitem1->valuestring);
                        }
                        char personaltweetss1[10][100];
                        personaltweetsitem1 = cJSON_GetObjectItem(follow1, "personalTweets");
                        personaltweet1 = cJSON_GetArraySize(cjson_followings1);
                        for (int i = 0; i < personaltweet1; i++) {
                            personaltweetsitem1 = cJSON_GetArrayItem(personaltweetsitem1, i);
                            strcpy(personaltweetss1[i], personaltweetsitem1->valuestring);
                        }
                        cJSON *cjson_followers14 = NULL;
                        cJSON *cjson_followings14 = NULL;
                        cJSON *cjson_personal14 = NULL;
                        cJSON *follow4 = cJSON_CreateObject();
                        cJSON_AddStringToObject(follow4, "username", cjson_username1->valuestring);
                        cJSON_AddStringToObject(follow4, "password", cjson_password1->valuestring);
                        cJSON_AddStringToObject(follow4, "bio", cjson_bio1->valuestring);
                        cjson_followers14 = cJSON_CreateArray();
                        for (int i = 0; i < cjson_followers_size1; i++) {
                            cJSON_AddItemToArray(cjson_followers14, cJSON_CreateString(followerss1[i]));
                        }
                        cJSON_AddItemToObject(follow4, "followers", cjson_followers1);
                        cjson_followings14 = cJSON_CreateArray();
                        for (int i = 0; i < cjson_following_size1; i++) {
                            cJSON_AddItemToArray(cjson_followings14, cJSON_CreateString(followingss1[i]));
                        }
                        cJSON_AddItemToArray(cjson_followings14, cJSON_CreateString(user));
                        cJSON_AddItemToObject(follow4, "followings", cjson_followings14);
                        cjson_personal14 = cJSON_CreateObject();
                        for (int i = 0; i < personaltweet; i++) {
                            cJSON_AddItemToArray(cjson_personal14, cJSON_CreateString(personaltweetss1[i]));
                        }
                        cJSON_AddItemToObject(follow4, "personalTweets", cjson_personal14);
                        Pointer = fopen(follow3, "w");
                        char *str1 = cJSON_PrintUnformatted(follow4);
                        fputs(str1, Pointer);
                        for(int i = 0 ; i < 20 ; i++) {
                            Send[i] = 'p' ;
                        }
                        printf("User Followed!") ;
                        send(client_socket,Send,strlen(Send) ,0) ;
                        fclose(Pointer);
                        closesocket(client_socket);
                        memset(buffer,0,sizeof(buffer)) ;
                         sam = 1 ;
                    }
                }
            } else {
                printf("Invalid token!");
                closesocket(client_socket);
            }
        } else if (buffer[0] == 's' && buffer[1] == 'e' && buffer[2] == 'r' && buffer[3] == 'c') {
            closesocket(client_socket);
        } else if (buffer[0] == 'u' && buffer[1] == 'n') {
            char username[30], newtoken[30], filename[50] , Send[10];
            int index, index1;
            FILE *Pointer;
            memset(username, 0, 30);
            memset(newtoken, 0, 30);
            memset(filename, 0, 30);
            int i = 9, j = 0;
            for (int i = 9; i <= 40; i++) {
                newtoken[j] = buffer[i];
                j++;
            }
            i = 42;
            j = 0;
            while (buffer[i] != '\n') {
                username[j] = buffer[i];
                j++;
                i++;
            }
            strcpy(filename, username);
            strcat(filename, ".json");
            if (strcmp(newtoken, token) != 0) {
                Pointer = fopen(filename, "r");
                if (Pointer == NULL) {
                    for (int i = 0 ; i < 20 ; i++) {
                        Send[i] = 'n';
                    }
                    send(client_socket,Send,strlen(Send),0) ;
                    printf("Username does not existed!\n");
                    fclose(Pointer);
                    closesocket(client_socket);
                } else {
                    char *information_of_file = malloc(100000);
                    char new_information_of_file[100000];
                    memset(information_of_file, 0, 100000);
                    memset(new_information_of_file,0,100000) ;
                    while (fgets(information_of_file, sizeof(information_of_file), Pointer) != NULL) {
                        strcat(new_information_of_file, information_of_file);
                    }
                    free(information_of_file);
                    fclose(Pointer);
                    int cjson_followers_size = 0, cjson_following_size = 0, personaltweet = 0;
                    cJSON *personaltweetsitem = NULL;
                    cJSON *followersitem = NULL;
                    cJSON *followingsitem = NULL;
                    cJSON *follow = NULL;
                    cJSON *cjson_username = NULL;
                    cJSON *cjson_password = NULL;
                    cJSON *cjson_bio = NULL;
                    cJSON *cjson_followers = NULL;
                    cJSON *cjson_followings = NULL;
                    cJSON *cjson_personaltweetss = NULL;
                    cJSON *personalTweets = NULL;
                    follow = cJSON_Parse(new_information_of_file);
                    cjson_username = cJSON_GetObjectItem(follow, "username");
                    cjson_password = cJSON_GetObjectItem(follow, "password");
                    cjson_bio = cJSON_GetObjectItem(follow, "bio");
                    cjson_followers = cJSON_GetObjectItem(follow, "followers");
                    cjson_followers_size = cJSON_GetArraySize(cjson_followers) ;
                    char followerss[10][20];
                    for (int i = 0; i < cjson_followers_size; i++) {
                        followersitem = cJSON_GetArrayItem(cjson_followers, i);
                        strcpy(followerss[i], followersitem->valuestring);
                    }
                    cjson_followings = cJSON_GetObjectItem(follow, "followings");
                    char followingss[10][20];
                    cjson_following_size = cJSON_GetArraySize(cjson_followings) ;
                    for (int i = 0; i < cjson_following_size; i++) {
                        followingsitem = cJSON_GetArrayItem(cjson_followings, i);
                        strcpy(followingss[i], followingsitem->valuestring);
                    }
                    char personaltweetss[10][100];
                    personaltweetsitem = cJSON_GetObjectItem(follow, "personalTweets");
                    personaltweet = cJSON_GetArraySize(cjson_followings) ;
                    for (int i = 0; i < personaltweet; i++) {
                        personaltweetsitem = cJSON_GetArrayItem(personaltweetsitem, i);
                        strcpy(personaltweetss[i], personaltweetsitem->valuestring);
                    }
                    int z = 0 ;
                    for (int i = 0; i <= cjson_followers_size; i++) {
                        if (strcmp(followerss[i], name) == 0) {
                            z = 1;
                            break;
                        }
                    }
                    if (z == 0) {
                        for (int i = 0 ; i < 20 ; i++) {
                            Send[i] = 'a';
                        }
                        printf("You are not following him to unfollow!\n");
                        send(client_socket,Send,strlen(Send),0) ;
                        closesocket(client_socket) ;
                    } else {
                        cJSON *cjson_followers13 = NULL;
                        cJSON *cjson_followings13 = NULL;
                        cJSON *cjson_personal13 = NULL;
                        cJSON *follow2 = cJSON_CreateObject();
                        cJSON_AddStringToObject(follow2, "username", cjson_username->valuestring);
                        cJSON_AddStringToObject(follow2, "password", cjson_password->valuestring);
                        cJSON_AddStringToObject(follow2, "bio", cjson_bio->valuestring);
                        cjson_followers13 = cJSON_CreateArray();
                        for (int i = 0 ; i < cjson_followers_size ; i++) {
                            if (followerss[i] == name) {
                            index = i ;
                            break ;
                        }}
                        for (int i = index  ; i < cjson_followers_size ;i++) {
                            strcpy(followerss[i],followerss[i+1]);
                        }
                        cjson_followers_size-- ;
                        for (int i = 0 ; i < cjson_followers_size; i++) {
                            cJSON_AddItemToArray(cjson_followers13, cJSON_CreateString(followerss[i]));
                        }
                        cJSON_AddItemToObject(follow2, "followers", cjson_followers13);
                        cjson_followings13 = cJSON_CreateArray();
                        for (int i = 0; i < cjson_following_size; i++) {
                            cJSON_AddItemToArray(cjson_followings13, cJSON_CreateString(followingss[i]));
                        }
                        cJSON_AddItemToObject(follow2, "followings", cjson_followings13);
                        cjson_personal13 = cJSON_CreateObject();
                        for (int i = 0 ; i < personaltweet; i++) {
                            cJSON_AddItemToArray(cjson_personal13, cJSON_CreateString(personaltweetss[i]));
                        }
                        cJSON_AddItemToObject(follow2, "personalTweets", cjson_personal13);
                        Pointer = fopen(filename, "w");
                        char *str = cJSON_PrintUnformatted(follow2);
                        fputs(str, Pointer);
                        fclose(Pointer);
                        char follow3[100];
                        memset(follow3, 0, 100);
                        strcpy(follow3, name);
                        strcat(follow3, ".json");
                        Pointer = fopen(follow3, "r");
                        char *information_of_file1 = malloc(100000);
                        char new_information_of_file1[100000];
                        memset(new_information_of_file1,0,100000) ;
                        memset(information_of_file1, 0, 100000);
                        while (fgets(information_of_file1, sizeof(information_of_file1), Pointer) != NULL) {
                            strcat(new_information_of_file1, information_of_file1);
                        }
                        free(information_of_file1);
                        fclose(Pointer);
                        int cjson_followers_size1 = 0, cjson_following_size1 = 0, personaltweet1 = 0;
                        cJSON *personaltweetsitem1 = NULL;
                        cJSON *followersitem1 = NULL;
                        cJSON *followingsitem1 = NULL;
                        cJSON *follow1 = NULL;
                        cJSON *cjson_username1 = NULL;
                        cJSON *cjson_password1 = NULL;
                        cJSON *cjson_bio1 = NULL;
                        cJSON *cjson_followers1 = NULL;
                        cJSON *cjson_followings1 = NULL;
                        cJSON *cjson_personaltweetss1 = NULL;
                        cJSON *personalTweets1 = NULL;
                        follow1 = cJSON_Parse(new_information_of_file1);
                        cjson_username1 = cJSON_GetObjectItem(follow1, "username");
                        cjson_password1 = cJSON_GetObjectItem(follow1, "password");
                        cjson_bio1 = cJSON_GetObjectItem(follow1, "bio");
                        cjson_followers1 = cJSON_GetObjectItem(follow1, "followers");
                        cjson_followers_size1 = cJSON_GetArraySize(cjson_followers1);
                        char followerss1[10][20];
                        for (int i = 0; i < cjson_followers_size1; i++) {
                            followersitem1 = cJSON_GetArrayItem(cjson_followers1, i);
                            strcpy(followerss1[i], followersitem1->valuestring);
                        }
                        cjson_followings1 = cJSON_GetObjectItem(follow1, "followings");
                        char followingss1[10][20];
                        cjson_following_size1 = cJSON_GetArraySize(cjson_followings1);
                        for (int i = 0; i < cjson_following_size1; i++) {
                            followingsitem1 = cJSON_GetArrayItem(cjson_followings1, i);
                            strcpy(followingss1[i], followingsitem1->valuestring);
                        }
                        char personaltweetss1[10][100];
                        personaltweetsitem1 = cJSON_GetObjectItem(follow1, "personalTweets");
                        personaltweet1 = cJSON_GetArraySize(cjson_followings1);
                        /*
                        for (int i = 0; i < personaltweet1; i++) {
                            personaltweetsitem1 = cJSON_GetArrayItem(personaltweetsitem1, i);
                            strcpy(personaltweetss1[i], personaltweetsitem1->valuestring);
                        }
                         */
                        cJSON *cjson_followers14 = NULL;
                        cJSON *cjson_followings14 = NULL;
                        cJSON *cjson_personal14 = NULL;
                        cJSON *follow4 = cJSON_CreateObject();
                        cJSON_AddStringToObject(follow4, "username", cjson_username1->valuestring);
                        cJSON_AddStringToObject(follow4, "password", cjson_password1->valuestring);
                        cJSON_AddStringToObject(follow4, "bio", cjson_bio1->valuestring);
                        cjson_followers14 = cJSON_CreateArray();
                        for (int i = 0; i < cjson_followers_size1; i++) {
                            cJSON_AddItemToArray(cjson_followers14, cJSON_CreateString(followerss1[i]));
                        }
                        cJSON_AddItemToObject(follow4, "followers", cjson_followers1);
                        cjson_followings14 = cJSON_CreateArray();
                        for (int i = 0; i < cjson_following_size1; i++) {
                            if (followerss1[i] == username) {
                                index = i;
                                break;
                            }
                        }
                        for (i = index; i < cjson_following_size1 - 1; i++) {
                            strcpy(followingss1[i], followingss1[i + 1]);
                        }
                        cjson_following_size1--;
                        for (int i = 0; i < cjson_following_size1; i++) {
                            cJSON_AddItemToArray(cjson_followings14, cJSON_CreateString(followingss1[i]));
                        }
                        cJSON_AddItemToObject(follow4, "followings", cjson_followings14) ;
                        cjson_personal14 = cJSON_CreateObject();
                        /*
                        for (int i = 0; i < personaltweet; i++) {
                            cJSON_AddItemToArray(cjson_personal14, cJSON_CreateString(personaltweetss1[i]));
                        }
                         */
                        cJSON_AddItemToObject(follow4, "personalTweets", cjson_personal14);
                        Pointer = fopen(follow3, "w");
                        char *str1 = cJSON_PrintUnformatted(follow4);
                        for (int i = 0 ; i < 20 ; i++) {
                            Send[i] = 'w';
                        }
                        printf("unfollow successfully!\n") ;
                        send(client_socket,Send,strlen(Send),0) ;
                        fputs(str1, Pointer);
                        fclose(Pointer);
                        closesocket(client_socket);
                    }
                }
            }
        }
        else if (buffer[0] == 's' && buffer[1] == 'e' && buffer[2] == 't') {
            char bio[30] , newtoken[30] , fileopen[50], response1[20];
            FILE * Pointer ;
            memset(response1 , 0 , 20) ;
            memset(bio, 0 , 30) ;
            memset(newtoken , 0 , 30) ;
            memset(fileopen , 0 , 50) ;
            int i = 8 , j = 0 ;
            for (int i = 8 ; i <= 39 ; i++) {
                newtoken[j] = buffer[i] ;
                j++ ;
            }
            i = 41 ;
            j = 0 ;
            while (buffer[i] != '\n') {
                bio[j] = buffer[i] ;
                j++ ;
                i++ ;
            }
            strcpy(fileopen , name) ;
            strcat(fileopen ,".json") ;
            Pointer = fopen(fileopen , "r") ;
            if (strcmp(token , newtoken) != 0) {
                char *information_of_file = malloc(100000);
                char new_information_of_file[100000];
                memset(new_information_of_file,0,100000) ;
                memset(information_of_file, 0, 100000);
                while (fgets(information_of_file, sizeof(information_of_file), Pointer) != NULL) {
                    strcat(new_information_of_file, information_of_file);
                }
                free(information_of_file);
                fclose(Pointer) ;
                int cjson_followers_size = 0, cjson_following_size = 0, personaltweet = 0;
                cJSON *personaltweetsitem = NULL;
                cJSON *followersitem = NULL;
                cJSON *followingsitem = NULL;
                cJSON *follow = NULL;
                cJSON *cjson_username = NULL;
                cJSON *cjson_password = NULL;
                cJSON *cjson_bio = NULL;
                cJSON *cjson_followers = NULL;
                cJSON *cjson_followings = NULL;
                cJSON *cjson_personaltweetss = NULL;
                cJSON *personalTweets = NULL;
                follow = cJSON_Parse(new_information_of_file);
                cjson_username = cJSON_GetObjectItem(follow, "username");
                cjson_password = cJSON_GetObjectItem(follow, "password");
                cjson_bio = cJSON_GetObjectItem(follow, "bio");
                cjson_followers = cJSON_GetObjectItem(follow, "followers");
                cjson_followers_size = cJSON_GetArraySize(cjson_followers);
                char followerss[10][20];
                for (int i = 0; i < cjson_followers_size; i++) {
                    followersitem = cJSON_GetArrayItem(cjson_followers, i);
                    strcpy(followerss[i], followersitem->valuestring);
                }
                cjson_followings = cJSON_GetObjectItem(follow, "followings");
                char followingss[10][20];
                cjson_following_size = cJSON_GetArraySize(cjson_followings);
                for (int i = 0; i < cjson_following_size; i++) {
                    followingsitem = cJSON_GetArrayItem(cjson_followings, i);
                    strcpy(followingss[i], followingsitem->valuestring);
                }
                char personaltweetss[10][100];
                personaltweetsitem = cJSON_GetObjectItem(follow, "personalTweets");
                personaltweet = cJSON_GetArraySize(cjson_followings);
                for (int i = 0; i < personaltweet; i++) {
                    personaltweetsitem = cJSON_GetArrayItem(personaltweetsitem, i);
                    strcpy(personaltweetss[i], personaltweetsitem->valuestring);
                }
                cJSON *cjson_followers13 = NULL;
                cJSON *cjson_followings13 = NULL;
                cJSON *cjson_personal13 = NULL;
                cJSON *follow2 = cJSON_CreateObject();
                cJSON_AddStringToObject(follow2, "username", cjson_username->valuestring);
                cJSON_AddStringToObject(follow2, "password", cjson_password->valuestring);
                cJSON_AddStringToObject(follow2, "bio", bio );
                cjson_followers13 = cJSON_CreateArray();
                for (int i = 0 ; i < cjson_followers_size; i++) {
                    cJSON_AddItemToArray(cjson_followers13, cJSON_CreateString(followerss[i]));
                }
                cJSON_AddItemToObject(follow2, "followers", cjson_followers13);
                cjson_followings13 = cJSON_CreateArray();
                for (int i = 0; i < cjson_following_size; i++) {
                    cJSON_AddItemToArray(cjson_followings13, cJSON_CreateString(followingss[i]));
                }
                cJSON_AddItemToObject(follow2, "followings", cjson_followings13);
                cjson_personal13 = cJSON_CreateObject();
                for (int i = 0 ; i < personaltweet; i++) {
                    cJSON_AddItemToArray(cjson_personal13, cJSON_CreateString(personaltweetss[i]));
                }
                cJSON_AddItemToObject(follow2, "personalTweets", cjson_personal13);
                Pointer = fopen(fileopen , "w");
                char *str = cJSON_PrintUnformatted(follow2);
                fputs(str, Pointer);
                fclose(Pointer);
                response1[9] = 'W' ;
                printf("\nBio Set\n") ;
                send(client_socket ,response1,sizeof(response1) , 0) ;
                closesocket(client_socket) ;
            }
            else {
                response1[9] = 'E' ;
                printf ("/nInvalid socket.") ;
                send(client_socket , response1 , sizeof(response1) , 0) ;
                closesocket(client_socket) ;
            }
        }
        else if (buffer[0] == 'c' && buffer[1] == 'h') {
            FILE * Pointer ;
            char newtoken[50] , password1[20] , newpassword[20] , fileopen[30] , response3[50] ;
            memset(fileopen,0,30) ;
            memset(newtoken,0,50) ;
            memset(password1,0,20) ;
            memset(newpassword,0,20) ;
            int j = 0 , i = 17 ;
            for (int i = 17 ; i <= 47 ; i++) {
                newtoken[j] = buffer[i] ;
                j++;
            }
            i = 49 ;
            j = 0 ;
            while (buffer[i] != ',') {
                password1[j] = buffer[i] ;
                i++;
                j++;
            }
            i++;
            j = 0 ;
            while (buffer[i] != '\n') {
                newpassword[j] = buffer[i] ;
                i++;
                j++;
            }
            if (strcmp(newtoken,token) != 0) {
                strcpy(fileopen , name) ;
                strcat(fileopen , ".json") ;
                Pointer = fopen(fileopen , "r") ;
                char *information_of_file = malloc(100000);
                char new_information_of_file[100000];
                memset(new_information_of_file,0,100000) ;
                memset(information_of_file, 0, 100000);
                while (fgets(information_of_file, sizeof(information_of_file), Pointer) != NULL) {
                    strcat(new_information_of_file, information_of_file);
                }
                free(information_of_file);
                fclose(Pointer) ;
                int cjson_followers_size = 0, cjson_following_size = 0, personaltweet = 0;
                cJSON *personaltweetsitem = NULL;
                cJSON *followersitem = NULL;
                cJSON *followingsitem = NULL;
                cJSON *follow = NULL;
                cJSON *cjson_username = NULL;
                cJSON *cjson_password = NULL;
                cJSON *cjson_bio = NULL;
                cJSON *cjson_followers = NULL;
                cJSON *cjson_followings = NULL;
                cJSON *cjson_personaltweetss = NULL;
                cJSON *personalTweets = NULL;
                follow = cJSON_Parse(new_information_of_file);
                cjson_username = cJSON_GetObjectItem(follow, "username");
                cjson_password = cJSON_GetObjectItem(follow, "password");
                cjson_bio = cJSON_GetObjectItem(follow, "bio");
                cjson_followers = cJSON_GetObjectItem(follow, "followers");
                cjson_followers_size = cJSON_GetArraySize(cjson_followers);
                char followerss[10][20];
                for (int i = 0; i < cjson_followers_size; i++) {
                    followersitem = cJSON_GetArrayItem(cjson_followers, i);
                    strcpy(followerss[i], followersitem->valuestring);
                }
                cjson_followings = cJSON_GetObjectItem(follow, "followings");
                char followingss[10][20];
                cjson_following_size = cJSON_GetArraySize(cjson_followings);
                for (int i = 0; i < cjson_following_size; i++) {
                    followingsitem = cJSON_GetArrayItem(cjson_followings, i);
                    strcpy(followingss[i], followingsitem->valuestring);
                }
                char personaltweetss[10][100];
                personaltweetsitem = cJSON_GetObjectItem(follow, "personalTweets");
                personaltweet = cJSON_GetArraySize(cjson_followings);
                for (int i = 0; i < personaltweet; i++) {
                    personaltweetsitem = cJSON_GetArrayItem(personaltweetsitem, i);
                    strcpy(personaltweetss[i], personaltweetsitem->valuestring);
                }
                if (strcmp(password1,cjson_password->valuestring) == 0) {
                    cJSON *cjson_followers13 = NULL;
                    cJSON *cjson_followings13 = NULL;
                    cJSON *cjson_personal13 = NULL;
                    cJSON *follow2 = cJSON_CreateObject();
                    cJSON_AddStringToObject(follow2, "username", cjson_username->valuestring);
                    cJSON_AddStringToObject(follow2, "password", newpassword);
                    cJSON_AddStringToObject(follow2, "bio", cjson_bio->valuestring );
                    cjson_followers13 = cJSON_CreateArray();
                    for (int i = 0 ; i < cjson_followers_size; i++) {
                        cJSON_AddItemToArray(cjson_followers13, cJSON_CreateString(followerss[i]));
                    }
                    cJSON_AddItemToObject(follow2, "followers", cjson_followers13);
                    cjson_followings13 = cJSON_CreateArray();
                    for (int i = 0; i < cjson_following_size; i++) {
                        cJSON_AddItemToArray(cjson_followings13, cJSON_CreateString(followingss[i]));
                    }
                    cJSON_AddItemToObject(follow2, "followings", cjson_followings13);
                    cjson_personal13 = cJSON_CreateObject();
                    for (int i = 0 ; i < personaltweet; i++) {
                        cJSON_AddItemToArray(cjson_personal13, cJSON_CreateString(personaltweetss[i]));
                    }
                    cJSON_AddItemToObject(follow2, "personalTweets", cjson_personal13);
                    Pointer = fopen(fileopen , "w");
                    char *str = cJSON_PrintUnformatted(follow2);
                    fputs(str, Pointer);
                    fclose(Pointer);
                    memset(response3,0,50) ;
                    for(int i = 0 ; i <= 50 ; i++) {
                    response3[i] = 'S' ;
                        }
                    printf ("Password changed!") ;
                    send(client_socket,response3,sizeof(response3),0) ;
                    closesocket(client_socket) ;
                }
        else {
        printf ("Password is not correct!") ;
        memset(response3,0,50) ;
                    for(int i = 0 ; i <= 50 ; i++) {
                        response3[i] = 'S' ;
                    }
        response3[9] = 'E' ;
        send(client_socket,response3,sizeof(response3) ,0) ;
        closesocket(client_socket) ;
        }
            }
            else {
                printf ("Token is not valid!") ;
                closesocket(client_socket) ;
            }
        }
       else if (buffer[0] == 'l' && buffer[1] == 'o' && buffer[2] == 'g') {
           char newtoken[50] ;
           memset(newtoken,0,50) ;
           int i = 7 , j = 0 ;
           while (buffer[i] != '\n') {
               newtoken[j] = buffer[i] ;
               i++ ;
               j++ ;
           }
           if (strcmp(newtoken,token) == 0) {
               printf("successfully logout!") ;
           }
           else {
               printf ("token is not valid! or User is not in his account!") ;
           }
       }
       else if (buffer[0] == 'd') {
            FILE * Pointer ;
            char newtoken[40] , id[20] , fileopen[100] , response13[100] ;
            memset(response13,0,100) ;
            memset(newtoken,0,40) ;
            memset(id,0,20) ;
            int i = 6 , j = 0 ;
            while (buffer[i] != ',') {
                newtoken[j] = buffer[i] ;
                j++ ;
                i++;
            }
            i++ ;
            j = 0 ;
            while (buffer[i] != '\n') {
                id[j] = buffer[i] ;
                j++;
                i++;
            }
            printf ("%s" , id ) ;
            strcpy(fileopen,id) ;
            strcat(fileopen,"tweet") ;
            strcat(fileopen,".json") ;
            Pointer = fopen(fileopen,"r") ;
            if (Pointer == NULL) {
                printf("Tweet not found!") ;
                strcpy(response13,"aaaaaaaaa") ;
                send(client_socket,response13,strlen(response13),0) ;
                fclose(Pointer) ;
                closesocket(client_socket) ;
            }
            else {
                char *information_of_file = calloc(1000 , sizeof(char));
                char new_information_of_file[1000];
                memset(information_of_file, 0, 1000);
                memset(new_information_of_file,0,1000) ;
                while (fgets(information_of_file, sizeof(information_of_file), Pointer) != NULL) {
                    strcat(new_information_of_file, information_of_file);
                }
                free(information_of_file);
                fclose(Pointer);
                cJSON *root = cJSON_Parse(new_information_of_file);
                cJSON *author = cJSON_GetObjectItem(root, "author");
                if (strcmp(author->valuestring , name) == 0) {
                    printf("tweet successfully deleted!") ;
                    remove(fileopen) ;
                    strcpy(response13,"cccccccccc") ;
                    send(client_socket,response13,strlen(response13),0) ;
                    closesocket(client_socket) ;
                }
                else {
                  printf("tweet is not yours") ;
                    strcpy(response13,"bbbbbbbbbb") ;
                    send(client_socket,response13,strlen(response13),0) ;
                    fclose(Pointer) ;
                    closesocket(client_socket) ;
                }
            }
       }
    }
}

