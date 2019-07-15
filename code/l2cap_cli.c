#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/hci.h>
#include <bluetooth/hci_lib.h>
#include <bluetooth/l2cap.h>

#define BDADDR  "EE:CA:46:65:82:2F"

int  main(int argv,char *argc[])
{
    int                 i,rv;
    int                 sock,newsock;
    char *              dest;
    char                src[18]="B8:27:EB:04:3C:E8";
    char                bufc[256],buf[256];
    struct sockaddr_l2  local_l2_addr;
    struct sockaddr_l2  remote_l2_addr;
   
    if(argv<2)
    {
        printf("usge:%s <bdaddr>\n",argc[0]);
	return -1;
    }

    sock=socket(PF_BLUETOOTH, SOCK_RAW, BTPROTO_L2CAP);
    if(sock<0)
    {
        perror("socket");
	return -1;
    }
    printf("Socket success!\n");
/*    
    memset(&local_l2_addr, 0 , sizeof(struct sockaddr_l2));
    local_l2_addr.l2_family = PF_BLUETOOTH;
    bacpy(&local_l2_addr.l2_bdaddr , BDADDR_ANY);
    rv=bind(sock,(struct sockaddr *)&local_l2_addr,sizeof(local_l2_addr));
    if(rv<0)
    {
        perror("Bind");
	return -2;
    }
    printf("Bind success!\n");
*/

    dest=argc[1];
    printf("dest %s\n",dest);
    memset(&remote_l2_addr, 0 , sizeof(struct sockaddr_l2));
    remote_l2_addr.l2_family = PF_BLUETOOTH;
    str2ba(dest, &remote_l2_addr.l2_bdaddr);
    remote_l2_addr.l2_psm=0x00017;
    rv=connect(sock, (struct sockaddr*)&remote_l2_addr, sizeof(struct sockaddr_l2));
    if(rv<0)
    {
        perror("connect");
	return -1;
    }
    
    printf("connect success\n");



    while(1)
    {
	memset(buf,0,sizeof(buf));
        rv=read(sock,buf,sizeof(buf));
	if(rv<0)
	{
	    perror("Read");
	    exit(-1);
	}
	printf("Recive %d bit :",rv);
	for(i=0;i<rv;i++)
	{
	    printf("%2x ",buf[i]);
	}
	printf("\n");
	if(buf[0]==0x0a)
	{
	    memset(bufc,0,sizeof(bufc));
	    bufc[0]=0x0b;
	    bufc[1]=buf[1];
	    bufc[2]=0x08;
	    bufc[3]=0x00;
	    bufc[4]=0x02;
            bufc[5]=0x00;
	    bufc[6]=0x00;
	    bufc[7]=0x00;
	    bufc[8]=0x00;
	    bufc[9]=0x00;
	    bufc[10]=0x00;
	    bufc[11]=0x00;
	
	    rv=write(sock,bufc,sizeof(buf));
	    if(rv<0)
	    {
	        perror("WRITE");
	    }
	    else
	    {
	        printf("qwe\n");
	    }
	}
    }

    return 0;
}
