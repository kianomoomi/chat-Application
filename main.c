#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#define MAX 80
#define PORT 12345
#define SA struct sockaddr
#include "cJSON.c"
#include <windows.h>
#include <conio.h>
char buffer[10000]= {0};
char token[200];
char  type[1000];
char  content[10000];
char _send[1000];
char _message[1000];
cJSON * c_content;

void SetColor(int ForgC)
{
     WORD wColor;
     HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
     CONSOLE_SCREEN_BUFFER_INFO csbi;
     if(GetConsoleScreenBufferInfo(hStdOut, &csbi))
     {
          wColor = (csbi.wAttributes & 0xF0) + (ForgC & 0x0F);
          SetConsoleTextAttribute(hStdOut, wColor);
     }
     return;
}

void make_socket()
{
    int client_socket;
    struct sockaddr_in servaddr;

    WORD wVersionRequested;
    WSADATA wsaData;
    int err;
    wVersionRequested = MAKEWORD(2, 2);

    err = WSAStartup(wVersionRequested, &wsaData);
    if (err != 0)
    {
        printf("WSAStartup failed with error: %d\n", err);
        return ;
    }
    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == -1)
    {
        SetColor(4);
        printf("Socket creation failed...\n");
        SetColor(0);
        exit(0);
    }
    else
    {
        SetColor(2);
        printf("Socket successfully created..\n");
        SetColor(0);
    }


    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    servaddr.sin_port = htons(PORT);
    if (connect(client_socket, (SA*)&servaddr, sizeof(servaddr)) != 0)
    {
        SetColor(4);
        printf("Connection to the server failed...\n");
        SetColor(0);
        exit(0);
    }
    else
    {
        SetColor(2);
        printf("Successfully connected to the server..\n");
        SetColor(0);
    }


    send(client_socket, buffer, sizeof(buffer), 0);
    memset(buffer, 0, sizeof(buffer));
    recv(client_socket, buffer, sizeof(buffer), 0);
    cJSON * set = cJSON_Parse(buffer);
    cJSON * c_type = cJSON_GetObjectItem(set, "type");
    c_content = cJSON_GetObjectItem(set, "content");
    strcpy(type, c_type->valuestring);
    if(!cJSON_IsArray(c_content))
        strcpy(content, c_content->valuestring);
    //closesocket(client_socket);
}
void registeration()
{
    char username[100], password[100];
    printf("Please enter your username:\n");
    SetColor(2);
    scanf("%s", username);
    SetColor(0);
    printf("Please enter your password:\n");
    SetColor(2);
    scanf("%s", password);
    SetColor(0);
    memset(buffer, 0, sizeof(buffer));
    strcat(buffer, "register ");
    strcat(buffer, username);
    strcat(buffer, ", ");
    strcat(buffer, password);
    strcat(buffer,"\n");
    make_socket();
    if(*type == 'E')
    {
        SetColor(4);
        printf("An error has occured\n");
        SetColor(0);
    }

    else
    {
        SetColor(2);
        printf("user successfully registered\n");
        SetColor(0);
         main();
    }

}

void create_channel()
{
    printf("please enter your channel name :");
    char channel_name[100];
    SetColor(1);
    scanf("%s", channel_name);
    SetColor(0);
    memset(buffer,0,sizeof(buffer));
    strcat(buffer, "create channel ");
    strcat(buffer, channel_name);
    strcat(buffer, ", ");
    strcat(buffer, token);
    strcat(buffer, "\n");
    memset(type, 0, sizeof(type));
    memset(content, 0, sizeof(content));
    make_socket();
    if(*type == 'E')
    {
        SetColor(4);
        printf("An error has occured\n");
        SetColor(0);
        main_menu();
    }

    else
    {
        SetColor(2);
        printf("Channel successfully created\n");
        SetColor(0);
        chat_menu();
    }

}
void send_message()
{
    char message[1000];
    SetColor(0);
    printf("Please enter your message :");
    SetColor(1);
    scanf("%s", message);
    SetColor(0);
    memset(buffer, 0, sizeof(buffer));
    strcat(buffer, "send ");
    strcat(buffer, message);
    strcat(buffer, ", ");
    strcat(buffer, token);
    strcat(buffer, "\n");
    memset(type, 0, sizeof(type));
    memset(content, 0, sizeof(content));
    make_socket();
    if(*type == 'E')
    {
        SetColor(4);
         printf("An error has occured\n");
         SetColor(0);
    }
        chat_menu();
}
void refresh()
{
    memset(buffer, 0, sizeof(buffer));
    strcat(buffer, "refresh ");
    strcat(buffer, token);
    strcat(buffer, "\n");
    make_socket();
    if(*type == 'L')
    {
        for(int i = 0; i < cJSON_GetArraySize(c_content); i++)
        {
            cJSON *item = cJSON_GetArrayItem(c_content, i);
            cJSON *sender = cJSON_GetObjectItem(item, "sender");
            memset(_send, 0, sizeof(_send));
            strcpy(_send, sender->valuestring);
            memset(_message, 0, sizeof(_message));
            cJSON *message =cJSON_GetObjectItem(item, "content");
            strcpy(_message, message->valuestring);
            if(!strcmp(_send,"server"))
            {
                SetColor(1);
                printf("%s\n", _message);
                SetColor(0);
            }

            else
            {
               SetColor(0);
               printf("%s : ", _send);
               SetColor(1);
               printf("%s\n", _message);
               SetColor(0);
            }

        }
    }
    else
    {
        system("cls");
        SetColor(4);
        printf("An error has occured\n");
        SetColor(0);
    }
    chat_menu();
}
void channel_members()
{
    memset(buffer, 0, sizeof(buffer));
    strcat(buffer, "channel members ");
    strcat(buffer, token);
    strcat(buffer, "\n");
    make_socket();
    if(*type == 'L')
    {
        for(int i = 0; i < cJSON_GetArraySize(c_content); i++)
        {
            cJSON *item = cJSON_GetArrayItem(c_content, i);
            memset(_send, 0, sizeof(_send));
            strcpy(_send, item->valuestring);
            printf("%s\n", _send);
        }
    }
    else
    {
        system("cls");
        SetColor(4);
        printf("An error has occured\n");
        SetColor(0);
    }
    chat_menu();
}
void leave_channel()
{
    memset(buffer, 0, sizeof(buffer));
    strcat(buffer, "leave ");
    strcat(buffer, token);
    strcat(buffer, "\n");
    make_socket();
    if(*type != 'E')
    main_menu();
    else
    {
        SetColor(4);
        printf("An error has occured\n");
        SetColor(0);
    }
}
void chat_menu()
{
    SetColor(0);
    printf(" 1 : Send Message\n");
    SetColor(9);
    printf(" 2 : Refresh\n");
    SetColor(5);
    printf(" 3 : Channel members\n");
    SetColor(4);
    printf(" 4 : Leave channel\n");
    SetColor(0);
    int adad;
    scanf("%d", &adad);
    switch(adad)
    {
    case 1 :
        send_message();
    case 2 :
        refresh();
    case 3 :
        channel_members();
    case 4 :
        leave_channel();
    }
}
void join_channel()
{
    SetColor(0);
    printf("please enter your channel name :");
    char channel_name[100];
    SetColor(1);
    scanf("%s", channel_name);
    SetColor(0);
    memset(buffer, 0, sizeof(buffer));
    strcat(buffer, "join channel ");
    strcat(buffer, channel_name);
    strcat(buffer, ", ");
    strcat(buffer, token);
    strcat(buffer, "\n");
    memset(type, 0, sizeof(type));
    memset(content, 0, sizeof(content));
    make_socket();
    if(*type == 'E')
    {
        SetColor(4);
        printf("An error has occured\n");
        SetColor(0);
        main_menu();
    }

    else
    {
        SetColor(0);
        printf("you successfully joined the channel\n");
        SetColor(1);
        printf("%s\n", channel_name);
        chat_menu();
    }
}
void logout()
{
    memset(buffer, 0, sizeof(buffer));
    strcat(buffer, "logout ");
    strcat(buffer, token);
    strcat(buffer, "\n");
    memset(type, 0, sizeof(type));
    memset(content, 0, sizeof(content));
    make_socket();
    if(*type == 'E')
    {
        SetColor(4);
        printf("An error has occured\n");
        SetColor(0);
    }

    else
        main();

}
void main_menu()
{
    SetColor(0);
    printf(" 1 : Create Channel\n");
    SetColor(9);
    printf(" 2 : Join channel\n");
    SetColor(4);
    printf(" 3 :Logout\n");
    SetColor(0);
    int adad;
    scanf("%d", &adad);
    switch(adad)
    {
    case 1 :
        create_channel();
        break;
    case 2 :
        join_channel();
        break;
    case 3 :
        logout();
        break;
    }
}
void login()
{
    char username[100], password[100];
    printf("Please enter your username:");
    SetColor(2);
    scanf("%s", username);
    SetColor(0);
    printf("Please enter your password:");
    SetColor(2);
    scanf("%s", password);
    SetColor(0);
    memset(buffer, 0, sizeof(buffer));
    strcat(buffer, "login ");
    strcat(buffer, username);
    strcat(buffer, ", ");
    strcat(buffer, password);
    strcat(buffer, "\n");
    memset(type, 0, sizeof(type));
    memset(content, 0, sizeof(content));
    make_socket();
    if (*type == 'A')
    {
        strcpy(token, content);
    }
    if(*type == 'E')
    {
        SetColor(4);
        printf("An error has occured\n");
        SetColor(0);
        main();
    }

    else
        main_menu();
}


int main()
{
   /* int a = 0;
    int ch = 0;
    ch = getch();
    while(ch == 0xE0)
    {
        int ch2;
        ch2 = getch();
        if(ch2 == 80 && a < 6)
        {
            a++;
            printf("down");
        }
        if(ch2 == 72 && a > 1)
        {
            a--;
            printf("up");
        }
        ch = getch();
        if(ch != 0xE0 && ch != 13 && ch != 10)
            if( ch >= 49 && ch <= 54)
        {
            printf("%c", ch);
            a = ch - 48;
        }
        else
            a = 0;
    }*/

    system("COLOR F0");
    system("cls");
    printf(" Account Menu:\n");
    SetColor(2);
    printf(" 1 : Register\n");
    SetColor(9);
    printf(" 2 : Login\n");
    SetColor(4);
    printf(" 3 : close\n");
    int adad;
    SetColor(0);
    scanf("%d", &adad);
    switch(adad)
    {
    case 1 :
        registeration();
        break;
    case 2 :
        login();
        break;
    case 3:
        exit(0);
    }


}
