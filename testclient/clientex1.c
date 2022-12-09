#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h> 

#define RCVBUFSIZE 1024   /* Size of receive buffer */
#define SNDBUFSIZE 1024   /* Size of receive buffer */

int key[95];

int getfromf(char* x, FILE* f, int debug)
{
    int i = 0, a;

    char infc;
    char *str = NULL;
    str = malloc(50*sizeof(char));
    strcpy(str,"");
    infc = fgetc(f);

    //make sure you're not reading after the file
    if(infc == EOF)
        return 1;
    else    //while it's not 0x20(space), 0xFFFFFFFF(bad data), 0x09(tab), or 0x0A(<cr>)
        while((infc != 0x20) && (infc != 0xFFFFFFFF) && (infc != 0x09) && (infc != 0x0A))
        {
            str[i] = infc;
            if(debug) printf("str[%d] = %X, %c, ", i, str[i], str[i]);  //DEBUG
            ++i;
            infc = fgetc(f);    //grab the data
        }
    
    str[i] = '\0';

    if(str == NULL) //let them know if it's null too.
    {
        x = NULL;
        return 1;
    }
    
    i = 0;
    if(debug) printf("\ncopying instring to a malloced string \n");
    while(str[i] != '\0')   //copy the malloced string into our string
    {
        x[i] = str[i];
        if(debug) printf("x[%d] = %X, %c, ", i, x[i], x[i]);  //DEBUG
        if(debug) printf("str[%d] = %X, %c, ", i, str[i], str[i]);  //DEBUG
        ++i;
    }
    x[i] = '\0';
    
    realloc(str, 0);    //reallocate the ram
    return 0;
}

void initperm()
{
    int i = 0;
    char fnamein[30] = "./key.txt", tempstr[4];
    FILE *inf;
    
    if((inf = fopen(fnamein, "r")) == NULL)
    {
        perror("could not open file\n");
        exit(2);
    }

    for(i = 0; i<95;i++)
    {
        getfromf(tempstr,inf,0);
        key[i] = atoi(tempstr)+32;
        memset(&tempstr, '0', strlen(tempstr));
        //printf("loop %d\n",i);
    }
    fclose(inf);
    printf("our key is");
}

void encode(char * in)
{
    int i;
    for(i=0;i<strlen(in);i++)
    {
        if(in[i] == 0x0A)
            in[i] = 0x0A;
        else
            in[i]=key[in[i]-32];
    }
}

void decode(char* in)
{
    int i,j = 0 ;
    for(i=0;i<strlen(in);i++)
    {
        //printf("i = %d",i);
        if(in[i] == 0x0A)
            in[i] = 0x0A;
        else
        {
            while(key[j] != in[i])
                ++j;
            in[i] = j+32;
            j = 0;
        }
    }      

}

int main(int argc, char *argv[])
{
    int servSock = 0, n = 0;
    char recvBuff[RCVBUFSIZE];
    char sendBuff[SNDBUFSIZE];
    char command[20], username[8];
    struct sockaddr_in serv_addr; 
    int quit=0, mistake=0, bytesRcvd;
    char servIP[16];
    int port;   
    initperm();

    printf("Please input the port to connect:");
    scanf("%d", &port);


    strcpy(servIP, "158.83.254.100");             /* First arg: DRACO1 IP address (dotted quad) */

    if((servSock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("\n Error : Could not create socket \n");
        return 1;
    } 

    memset(&serv_addr, '0', sizeof(serv_addr)); 
    serv_addr.sin_family      = AF_INET;             /* Internet address family */
    serv_addr.sin_addr.s_addr = inet_addr(servIP);   /* Server IP address */
    serv_addr.sin_port        = htons(port); /* Server port */


    if( connect(servSock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
       printf("\n Error : Connect Failed \n");
       return 1;
    } 

    if ((bytesRcvd = recv(servSock, recvBuff, RCVBUFSIZE + 1, 0)) <= 0)
            printf("Error: recv() failed or connection closed prematurely\n");      
//    printf("pre-decode recvBuff = %s of %d\n", recvBuff, bytesRcvd);
    decode(recvBuff);
//    printf("post-decode recvBuff = %s\n", recvBuff);
    do
    {
        printf("Server_echo:%s \n",recvBuff);
         printf("Please type to send to server (type -1 to quit):");
        scanf("%s", sendBuff);
        if ( (strcmp(sendBuff, "-1") == 0) || (strcmp(sendBuff, "quit") == 0))
        {
            bytesRcvd = 0;
            break;
        }                 
        else
    	{
	        encode(sendBuff);
	        if (send(servSock, sendBuff, sizeof(sendBuff), 0) < 0)
	            printf("Error: send() failed\n");
	         sleep(1);
            memset(&recvBuff, '0', sizeof(recvBuff)); 
           if ((bytesRcvd = recv(servSock, recvBuff, RCVBUFSIZE + 1, 0)) <= 0)
                  printf("Error: recv() failed or connection closed prematurely\n");              
//		   printf("pre-decode recvBuff = %s of %d\n", recvBuff, bytesRcvd);
		   recvBuff[bytesRcvd]='\0';
           decode(recvBuff);
         }
    } while (bytesRcvd>0);
    
    close(servSock); //close the socket
    return 0;
}

