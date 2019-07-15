#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/rfcomm.h>



#define LOCAL_CID 9
unsigned char OBEX_CONNECT_REQUEST[]={0x80,0x00,0x07,0x10,0x00,0x60,0x00};

unsigned char PUT[]={
         0x02,0x00,0x59,0xcb,0x00,0x00,0x00,0x01,
	 0xc0,0x00,0x00,0x00,0x01,0x01,0x00,0x29,
	 0x00,0x32,0x00,0x30,0x00,0x31,0x00,0x38,
	 0x00,0x2d,0x00,0x30,0x00,0x33,0x00,0x2d,
	 0x00,0x30,0x00,0x34,0x00,0x2e,0x00,0x6c,
	 0x00,0x6f,0x00,0x67,0x00,0x2e,0x00,0x74,
	 0x00,0x78,0x00,0x74,0x00,0x00,0x42,0x00,
	 0x0e,0x74,0x65,0x78,0x74,0x2f,0x70,0x6c,
	 0x61,0x69,0x6e,0x00,0xc3,0x00,0x00,0x00,
	 0x0d,0x48,0x00,0x10,0x68,0x65,0x6c,0x6c,
	 0x6f,0x77,0x20,0x62,0x6f,0x79,0x21,0x0d,
	 0x0a
};




int main(int argc,char *argv[])
{
    int                a,remote_cid;
    int                newsock,sock,status;
    unsigned char      buf[1024];
    unsigned char      put[]={0x82,0x00,0x0b,0xcb,0x00,0x00,0x00,0x01,0x49,0x00,0x03};
    char               name[32];
    socklen_t          len;
    struct sockaddr_rc local_addr={0},re_addr={0};

    if(argc<3)
    {
        printf("%s usage: <bdaddr> <channel>\n",argv[0]);
	return -1;
    }
    printf("%d\n",sizeof(PUT));
    sock=socket(PF_BLUETOOTH,SOCK_STREAM,BTPROTO_RFCOMM);
    if(sock<0)
    {
        perror("SOCKET");
	return -1;
    }

    local_addr.rc_family=PF_BLUETOOTH;
    local_addr.rc_channel=(uint8_t)LOCAL_CID;
    bacpy(&local_addr.rc_bdaddr, BDADDR_ANY);
    status=bind(sock,(struct sockaddr *)&local_addr,sizeof(local_addr));
    if(status<0)
    {
        perror("BIND");
	close(sock);
	return -1;
    }
    
    remote_cid=atoi(argv[2]);

    re_addr.rc_family=PF_BLUETOOTH;
    re_addr.rc_channel=(uint8_t)remote_cid;
    str2ba(argv[1],&re_addr.rc_bdaddr);

    status=connect(sock,(struct sockaddr *)&re_addr,sizeof(re_addr));
    if(status<0)
    {
        perror("connect"); 
	return -1;
    }
    printf("connect success\n");
    
    while(1)
    {
    	    printf("OBEX connect :");
	    for(a=0;a<sizeof(OBEX_CONNECT_REQUEST);a++)
	    {
		int i=1;
	        printf("%x ",OBEX_CONNECT_REQUEST[a]);
		if(a==i*10)
		{
		    printf("\n");
		    i++;
		}
	    }
	    printf("\n");
            memset(buf,0,sizeof(buf));
       	    
	    status=write(sock,(char *)&OBEX_CONNECT_REQUEST,sizeof(OBEX_CONNECT_REQUEST));
	    if(status<0)
	    {
	        perror("write");
		return -1;
	    }
	    printf("Send OBEX connect request.\n");
            status=read(sock,buf,sizeof(buf));
            if(status<0)
    	    {
    	        perror("READ");
		return -1;
    	    }
	    printf("RECIVE %d BYTE: \n",status);
	    for( a=0;a<status;a++)
	    {
	        printf("%x ",buf[a]);
	    }
	    printf("\n");

            if(buf[0]!=0xa0)
	    {
	        printf("connect reject!\n");
	        return 0;
	    }
	    printf("connect success!\n");
            
	    
	    memset(buf,0,sizeof(buf));                //remote information 
            status=read(sock,buf,sizeof(buf));
	    for( a=0;a<status;a++)
	    {
	        printf("%x ",buf[a]);
	    }
            

	    memset(buf,0,sizeof(buf));              //
	    status=read(sock,buf,sizeof(buf));
	    if(status<0)
	    {
		perror("end send");
		return -1;
	    }
	    if(buf[0]=0xa0)
	    {
	        printf("put success\n");
	    }
	    for( a=0;a<status;a++)
	    {
		    printf("%x ",buf[a]);
            }
	    printf("\n");
        
            memset(buf,0,sizeof(buf));

	    char disc[]={0x81,0x00,0x08,0xcb,0x00,0x00,0x00,0x01};

            status=write(sock,disc,sizeof(disc));
	    if(status<0)
	    {
	    
	         perror("disconnect");
		 return -1;
	    }

	    status=read(sock,buf,sizeof(buf));
	    if(status<0)
	    {
	    
	         perror("read disconnect");
		 return -1;
	    }
            


	    break;
    }
    close(sock);
    return 0;

}
