#include <stdio.h>
#include <string.h>
//REQUEST
#define CONNECT       0X80
#define DISCONNECT    0X81
#define PUT           (0X82 || 0X02)
#define GET           (0X83 || 0X03)

//RESPONSE
#define LEN           0XCB




void fun(unsigned char * put,char *name)
{
    unsigned short int lenth=0x1234;
    int len;
    int num=0;
    char   buf[64];
    memset(buf,0,sizeof(buf));
    len=strlen(name);
    while(len)
    {   
        if(num/2)
        {   
            buf[num]=name[num/2];
            
            len--;
	    num++;
        }   
        else{

            buf[num]=0x00;
            num++;
        }   

    }   
    put[0]=0x02;
    put[2]=(unsigned char)lenth;
    put[1]=(unsigned char)(lenth>>8);
    put[3]=0xcd;
    put[7]=0x01;
    memcpy(&put[8],name,strlen(name));					    

}

int main()
{
    int    i;
    unsigned char put[126];
    char           *name="qwertgds";
    memset(put,0,sizeof(put));
    fun(put,name);
    for(i=0;i<sizeof(put);i++)
    {
        printf("%x ",put[i]);
    
    }
    printf("\n");
    return 0;
}


