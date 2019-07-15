#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/rfcomm.h>


int main(int argc,char *argv[])
{
    int                i,newsock,sock,status,local_chan=9;
    char               buf[1024];
    socklen_t          len;
    struct sockaddr_rc local_addr={0},addr={0};

    sock=socket(PF_BLUETOOTH,SOCK_STREAM,BTPROTO_RFCOMM);
    if(sock<0)
    {
        perror("SOCKET");
	return -1;
    }

    if(local_chan>0)
    {
        local_addr.rc_family=PF_BLUETOOTH;
        local_addr.rc_channel=(uint8_t)local_chan;
        bacpy(&local_addr.rc_bdaddr, BDADDR_ANY);
        status=bind(sock,(struct sockaddr *)&local_addr,sizeof(local_addr));
        if(status<0)
        {
            perror("BIND");
            close(sock);
	    return -1;
        }
    }
    else
    {
        printf("channel error\n");
	close(sock);
	return -1;
    }

    status=listen(sock,4);
    if(status<0)
    {
        perror("LISTEN");
        close(sock);
        return -1;	
    }

    while(1)
    {
        printf("Waiting client connect...\n");
        newsock=accept(sock,(struct sockaddr *)&addr,&len);
        if(newsock<0)
	{
	    perror("ACCEPT");
	    close(sock);
	    return -1;
	}
        printf("connect success\n");
        while(1)
        {
    	    memset(buf,0,sizeof(buf));
            status=read(newsock,buf,sizeof(buf));
            if(status<0)
    	    {
    	        perror("READ");
    	    }
            for(i=0;i<status;i++)
	    {
	        printf(" %x",buf[i]);
	    }
            printf("\n");
            if(buf[0]=0x80)
       	    status=write(newsock,buf,sizeof(buf));
            printf("%s\n",buf);
            
        }
    
    }
    close(sock);
    return 0;

}
