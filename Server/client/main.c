#include <stdio.h>
#include <winsock2.h>
#include <string.h>
#include <conio.h>
#include <stdlib.h>
#include "cJSON.h"
char username[100] ;
int initialize(int);
char *send_data(char *);
void print_new();
int search () ;
struct sockaddr_in server;
int m_index = 0;
int singup () ;
int login () ;
int tweetprofile() ;
int sendtweet () ;
int refresh () ;
int Personal () ;
int setbio() ;
int changepassword ();
int logout();
int like();
int comment();
int deletetweet() ;
char token [200] ;
char Cm[200][400] ;
int main () {
    int menu_chose , singuppos , twitter_menu , timeline , l ;
    int can_initialize = initialize(12345);
    pos3 :
    printf ("Register menu : \n1.Sing up\n2.Log in") ;
    printf ("\n") ;
    scanf ("%d" , &menu_chose ) ;
    pos1 :
    switch (menu_chose) {
        case 1  :
            singuppos = singup();
            if (singuppos == 1) {
                printf("account created!\n");
                goto pos2;
            } else if (singuppos == 0) {
                printf("this username is already taken!\ntry again!\n");
                goto pos1;
            }
        pos2 :
        case 2 :
            l = login();
            if (l == -1) {
                printf("Invalid Password\n");
                goto pos3;
            }
            if (l == 0) {
                printf("Username Dose not existed\n");
                goto pos3;
            }}
    pos4 :
    printf ("Twitter Menu :\n") ;
    printf ("1.TimeLine\n2.Search\n3.Tweet Profile\n4.Personal area\n5.Log out\n6.deletetweet");
    scanf ("%d" , &twitter_menu) ;
    switch (twitter_menu) {
        case 1 : printf ("1.Send tweet\n2.refresh\n") ;
            scanf ("%d" , &timeline) ;
            if (timeline == 1) {
                printf ("Enter your tweet \n") ;
                sendtweet () ;
                goto pos4 ;
            }
            if (timeline == 2) {
                refresh () ;
                printf ("1.like or comment a tweet\n") ;
                printf ("2.Back\n") ;
                int aaa ;
                scanf("%d" , &aaa) ;
                if (aaa == 1) {
                    pos6 :
                    printf ("1.like\n2.Comment\n3.back\n") ;
                    int bbb ;
                    scanf ("%d" , &bbb) ;
                    if (bbb == 1) {
                        like () ;
                        goto pos6 ;
                    }
                    if (bbb == 2) {
                        comment() ;
                        goto pos6 ;
                    }
                    if (bbb == 3) {
                        goto pos4 ;
                    }
                }
                if (aaa == 2) {
                    goto pos4 ;
                }
            }
            break ;
        case 2 : search() ;
            printf ("\n1.Back") ;
            int back ;
            scanf ("%d" , &back) ;
            if (back == 1) {
                goto pos4 ;
            }
            break ;
        case 3 : tweetprofile() ;
            printf ("\n1.Back") ;
            int back2 ;
            scanf ("%d" , &back2) ;
            if (back2 == 1) {
                goto pos4 ;
            }
            break ;
        case 4 : Personal () ;
            printf ("\n3.Back\n") ;
            int ppp ;
            scanf ("%d" , &ppp) ;
            if (ppp == 1) {
                setbio() ;
                printf ("\n1.Back\n") ;
                int back3 ;
                scanf ("%d" , &back3) ;
                if (back3 == 1) {
                    goto pos4 ;
                }
            }
            if (ppp == 2) {
                changepassword () ;
                printf ("\n1.Back\n") ;
                int back4 ;
                scanf ("%d" , &back4) ;
                if (back4 == 1) {
                    goto pos4 ;
                }
            }
            if (ppp == 3) {
                goto pos4 ;
            }
            break ;
        case 5 :  logout () ;
            printf ("\n") ;
            goto pos3 ;
            break ;
        case 6 : deletetweet() ;
        goto pos4 ;
        break ;
    }}

int comment() {
    char Send[200] ;
    char id[10] ;
    char Payam[200] ;
    char receve[100] ;
    printf ("Enter id:\n") ;
    scanf("%s" , id) ;
    printf ("Enter your Message:\n") ;
    getchar() ;
    scanf ("%[^\n]s%*c" , Payam) ;
    strcpy(Send,"comment ") ;
    strcat(Send,token) ;
    strcat(Send,",") ;
    strcat(Send,id) ;
    strcat(Send,",") ;
    strcat(Send,Payam) ;
    strcat(Send,"\n") ;
    strcpy(receve,send_data(Send)) ;
    if (receve[0] == 'C') {
        printf("Comment Added to tweet") ;
    }
    else {
        printf("Tweet is not existed");
    }
}
int like() {
    char Send[200] ;
    char receve[200] ;
    char id[10] ;
    printf ("enter tweet id :\n") ;
    scanf ("%s" , id) ;
    strcpy(Send,"like ") ;
    strcat(Send,token) ;
    strcat(Send,",") ;
    strcat(Send,id) ;
    strcat(Send,"\n") ;
    strcpy(receve,send_data(Send)) ;
    if (receve[0] == 'i') {
        printf("There is no tweet with this id!") ;
    }
    else {
        printf("You like tweet successfully") ;
    }
}
int logout() {
    char Send[200] ;
    strcpy(Send,"logout ");
    strcat(Send,token) ;
    strcat(Send,"\n") ;
    send_data(Send) ;
    printf ("logout!") ;
}
int singup () {
    char  password [100] , result [200] ;
    char *a ;
    printf ("Enter Username :\n") ;
    scanf ("%s" ,username) ;
    printf ("Enter Password\n") ;
    scanf ("%s" , password) ;
    char Send[100000] ;
    strcpy(Send, "signup ") ;
    strcat(Send,username) ;
    strcat(Send,",") ;
    strcat(Send,password) ;
    strcat(Send,"\n") ;
    strcpy( result , send_data(Send)) ;
    if (result[9] == 'S') {
        return 1 ;
    }
    else if (result [9] == 'E') {
        return 0 ;
    }
}
int login () {
    char  password [100] , result[200] ;
    printf ("Enter Username :\n") ;
    scanf ("%s" ,username) ;
    printf ("Enter Password\n") ;
    scanf ("%s" , password) ;
    char Send[100000] ;
    strcpy(Send, "login ") ;
    strcat(Send,username) ;
    strcat(Send,",") ;
    strcat(Send,password) ;
    strcat(Send,"\n") ;
    strcpy(result , send_data(Send)) ;
    if (result[9]=='T') {
        int j = 0 ;
        for (int i = 27 ; i <= 58 ; i++) {
            token[j] = result[i] ;
            j++ ;
        }
        return 1 ;
    }
    else if (result [27] == 'I') {
        return -1 ;
    }
    else {
        return 0 ;
    }
}
int sendtweet() {
    char Payam[1000] , Send[1000] ;
    getchar() ;
    scanf("%[^\n]s%*c" ,Payam) ;
    strcpy(Send,"send tweet " );
    strcat(Send,token) ;
    strcat(Send ,",") ;
    strcat(Send,Payam) ;
    strcat(Send,"\n") ;
    send_data(Send) ;
}
int refresh() {
    return 0 ;
    char Send[100], result[10000] , final[1000];
    strcpy(Send, "refresh ");
    strcat(Send, token);
    strcat(Send, "\n");
    send_data(Send);
    /*
    cJSON *root = cJSON_Parse(result) ;
    cJSON *message = cJSON_GetObjectItem(root,"message");
    for (int i = 0 ; i < cJSON_GetArraySize(message) ; i++){
        cJSON *subitem = cJSON_GetArrayItem(message ,i) ;
        cJSON *id = cJSON_GetObjectItem(subitem,"id") ;
        cJSON *author = cJSON_GetObjectItem(subitem,"author") ;
        cJSON *content = cJSON_GetObjectItem(subitem,"content") ;
        cJSON *likes = cJSON_GetObjectItem(subitem,"likes") ;
        printf ("author:%s       id:%d\n" , author->valuestring ,id->valueint) ;
        printf ("%s\n" , content->valuestring) ;
        printf ("Likes:%d\n\n" , likes->valueint) ;
    }
     */
}
int search () {
    char name[100], Send[200], Server[400000] , receve3[10] ;
    printf("Enter Username: \n");
    scanf("%s", name);
    strcpy(Send, "search ");
    strcat(Send, token);
    strcat(Send, ",");
    strcat(Send, name);
    strcat(Send, "\n");
/*
    unsigned long long int i = 9;
    if (Server[i] == 'E') {
        printf ("Username Dose Not Existed") ;
        return 0 ;
    }
    i = 41;
    printf ("Username :") ;
    while ((int) Server[i] != 34) {
        printf("%c", Server[i]);
        i++;
    }
    printf("\n");
    i = i + 8;
    if ((int) Server[i+1] ==  34) {
        printf("Bio:Non");
    } else {
        printf ("Bio:") ;
        do {
            printf( "%c", Server[i]);
            i++;
        } while ((int) Server[i] != 34);
    }
    printf("\n");
    i = i + 22;
    printf("Followers: ");
    while ((int) Server[i] != 44) {
        printf("%c", Server[i]);
        i++;
    }
    i = i + 22;
    printf("  ");
    printf("Followings: ");
    while ((int) Server[i] != 44) {
        printf("%c", Server[i]);
        i++;
    }
    i = i + 17;
    printf("\n");
    while ((int) Server[i] != 34) {
        printf("%c", Server[i]);
        i++;
    }
    printf("\n");
    i = i + 15;
    if ((int) Server[i] == 93) {
        printf("no tweets");
    }
    while ((int) Server[i] != 93 ) {
        int c = 0 ;
        i = i + 6;
        printf("id: ");
        while ((int) Server[i] != 44) {
            if ((int) Server[i] >= 48 && (int) Server[i] <= 57) {
                printf("%c", Server[i]); }
            i++;
        }
        printf("             ");
        i = i + 11;
        while ((int) Server[i] != 34) {
            printf("%c", Server[i]);
            i++;
        }
        i = i + 13;
        printf("\n");
        while ((int) Server[i] != 34) {
            printf("%c", Server[i]);
            i++;
        }
        i = i + 13;
        while ((int) Server[i] != 125) {
            i++;
            if ((int) Server[i] == 58) {
                c++ ;
            }
        }
        printf ("\nComments: %d" , c) ;
        i = i + 10;
        printf("\n");
        printf("Likes: ");
        while ((int) Server[i] != 125) {
            printf("%c", Server[i]);
            i++;
        }
        printf ("\n") ;
        if ((int) Server[i+1] == 93)
            break ;
    }*/
    char fu[10] ;
    printf ("enter f for follow and u for unfollow!") ;
    scanf("%s" ,fu) ;
    if (fu[0] == 'u') {
        strcpy(Send,"unfollow ") ;
        strcat(Send,token) ;
        strcat(Send,",") ;
        strcat(Send,name) ;
        strcat(Send,"\n") ;
        strcpy(receve3,send_data(Send)) ;
        if (receve3[0] == 'n') {
            printf("Username does not existed!");
        }
        else if (receve3[0]=='a') {
            printf("You are not following him to unfollow!");
        }
        else if (receve3[0] == 'w') {
            printf("you unfollow him successfully") ;
        }
    }
    if (fu[0] == 'f') {
        strcpy(Send,"follow ") ;
        strcat(Send,token) ;
        strcat(Send,",") ;
        strcat(Send,name) ;
        strcat(Send,"\n") ;
        send_data(Send) ;
        strcpy(receve3,send_data(Send)) ;
        if (receve3[0] == 'n') {
            printf("Username does not existed!");
        }
        else if (receve3[0]=='a') {
            printf("you follow him successfully") ;
        }
        else if (receve3[0] == 'p') {
            printf("you follow him successfully") ;
        }
    }
}
int  tweetprofile() {
    char Send[1000] , Server[2000] ;
    int c = 0 ;
    strcpy(Send,"profile ") ;
    strcat(Send,token) ;
    strcat(Send,"\n") ;
    strcat(Server,send_data(Send)) ;
    long long int  j = 44;
    printf ("Username :%s" , username) ;
    while ((int) Server[j] != 34) {
        j++;
    }
    printf("\n");
    j = j + 9 ;
    if ((int) Server[j] == 34) {
        printf("Bio:Non");
    } else {
        printf ("Bio:") ;
        do {
            printf("%c", Server[j]);
            j++;
        } while ((int) Server[j] != 34);
    }
    printf("\n");
    j = j + 22;
    printf("Followers: ");
    while ((int) Server[j] != 44) {
        printf("%c", Server[j]);
        j++;
    }
    j = j + 22;
    printf("  ");
    printf("Followings: ");
    while ((int) Server[j] != 44) {
        printf("%c", Server[j]);
        j++;
    }
    j = j + 17;
    printf("\n");
    while ((int) Server[j] != 34) {
        printf("%c", Server[j]);
        j++;
    }
    printf("\n");
    j = j + 15;
    if ((int) Server[j] == 93) {
        printf("no tweets");
    }
    while ((int) Server[j] != 93 ) {
        c = -1 ;
        j = j + 6;
        printf("id: ");
        while ((int) Server[j] != 44) {
            if ((int) Server[j] >= 48 && (int) Server[j] <= 57) {
                printf("%c", Server[j]); }
            j++;
        }
        printf("             ");
        j = j + 11;
        while ((int) Server[j] != 34) {
            printf("%c", Server[j]);
            j++;
        }
        j = j + 13;
        printf("\n");
        while ((int) Server[j] != 34) {
            printf("%c", Server[j]);
            j++;
        }
        j = j + 13;
        while ((int) Server[j] != 125) {
            j++ ;
            if ((int) Server[j-2] == 58) {
                c++ ;
            }
        }
        printf ("\nComments: %d" , c) ;
        j = j + 10;
        printf("\n");
        printf("Likes: ");
        while ((int) Server[j] != 125) {
            printf("%c", Server[j]);
            j++;
        }
        printf ("\n") ;
        if ((int) Server[j+1] == 93)
            break ;
    }
}
int Personal () {
    printf ("1.set bio\n2.change password") ;
}
int setbio() {
    char Send[200] , bio[200] , Server[200];
    printf ("enter bio:") ;
    getchar();
    scanf("%[^\n]s%*c" ,bio) ;
    strcpy(Send,"set bio ") ;
    strcat(Send,token) ;
    strcat(Send,",") ;
    strcat(Send,bio) ;
    strcat(Send,"\n") ;
    strcpy(Server,send_data(Send)) ;
    if (Server[9] == 'E') {
        printf ("You Can set bio only once!\n") ;
    }
    else {
        printf ("Your Bio Set!\n") ;
    }
}
int  changepassword () {
    char Send[200] , Server[200] ;
    char old[200] , new[200] ;
    printf ("enter your current password:\n") ;
    scanf("%s" , old) ;
    printf ("enter your new password:\n") ;
    scanf ("%s" , new) ;
    strcpy(Send,"change password ") ;
    strcat(Send,token) ;
    strcat(Send,",") ;
    strcat(Send,old) ;
    strcat(Send,",") ;
    strcat(Send,new) ;
    strcat(Send,"\n") ;
    strcpy(Server,send_data(Send)) ;
    if (Server[9] == 'S') {
        printf ("Your Password changed!\n") ;
    }
    if (Server[9] == 'E') {
        printf ("Password is not correct!\n") ;
    }
}
int deletetweet() {
    char receve[200] , Server[200] , id[20];
    printf("enter tweet id: \n") ;
    scanf ("%s" , id) ;
    strcpy(Server,"delete ") ;
    strcat(Server,token) ;
    strcat(Server,",") ;
    strcat(Server,id) ;
    strcpy(receve,send_data(Server)) ;
    if (receve[1] == 'a') {
        printf("Tweet not found!\n") ;
    }
    else if (receve[1] == 'b') {
        printf("tweet is not yours\n") ;
    }
    else if (receve[1] == 'c') {
        printf("tweet successfully deleted!\n") ;
    }
}
int initialize(int port){
    WSADATA wsadata;
    int wsaStartUp = WSAStartup(MAKEWORD( 2, 2), &wsadata);
    if(wsaStartUp != 0){
        printf("Error");
        return -1;
    }
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_port = htons(port);
    return 0;
}
char *send_data(char *data){
    char *buffer = malloc(1000);
    memset(buffer, 0, 1000);

    int client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == INVALID_SOCKET){
        return buffer;
    }

    int can_connect = connect(client_socket, (struct sockaddr *) &server, sizeof(server));
    if (can_connect != 0){
        return buffer;
    }
    send(client_socket, data, strlen(data), 0);
    recv(client_socket, buffer, 999, 0);
    closesocket(client_socket);
    return buffer;
}

