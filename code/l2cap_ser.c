#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/hci.h>
#include <bluetooth/hci_lib.h>
#include <bluetooth/l2cap.h>


int main()
{
    int                 rv,sock,client;
    char                dest[19]="B8:27:EB:04:3C:E8";
    char                buf[1024];
    socklen_t           len;
    struct sockaddr_l2  addr,rem_addr;
   

    sock=socket(PF_BLUETOOTH, SOCK_SEQPACKET, BTPROTO_L2CAP);
    if(sock<0)
    {
    	    perror("Socket");
            exit(-1);        
    }	    
    
    memset((void *)&addr,0,sizeof(addr));
    memset((void *)&rem_addr,0,sizeof(rem_addr));

    addr.l2_family=PF_BLUETOOTH;
    addr.l2_psm=htobs(0x1001);
    addr.l2_bdaddr=*BDADDR_ANY;
    rv=bind(sock,(struct sockaddr *)&addr,sizeof(addr));
    if(rv<0)
    {
        perror("BIND");
	exit -1;
    }

    listen(sock,10);
    while(1)
    {
        client=accept(sock,(struct sockaddr *)&rem_addr,&len);
        if(client<0)
        {
            perror("ACCEPT");
	    exit(-1);
        }
        memset(buf,0,sizeof(buf));

        rv=write(client,dest,sizeof(dest));
        if(rv<0)
	{
	    perror("WRITE");
	    exit(-2);
	}
    }
    return 0;
}
