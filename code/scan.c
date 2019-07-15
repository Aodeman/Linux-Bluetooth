#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/hci.h>
#include <bluetooth/hci_lib.h>



int main()
{
    int            i,num=1;
    int            max_rsp,num_rsp;
    int            sock,len,flags,dev_id;    
    inquiry_info * ii=NULL;
    
    char           addr[19];
    char           name[248];

    dev_id=hci_get_route(NULL);  //得到第一个可用的(蓝牙驱动)Dongle Devices id
    if(dev_id<0)
    {
        perror("get dongle");
	return -1;
    }
    sock=hci_open_dev(dev_id);   //open hci socket
    if(sock<0)
    {
        perror("Socket");
	return -1;
    }
    
    //Search bluetooth devices

    len=8;           //time to inquiry 1=1.25s
    max_rsp=255;     //max number of searches 
    flags = IREQ_CACHE_FLUSH;   //使用IREQ_CACHE_FLUSH，则会真正重新inquiry。否则可能会传回上次的结果
    ii=malloc(max_rsp*sizeof(inquiry_info));
    while(num)
    {
        num_rsp=hci_inquiry(dev_id,len,max_rsp,NULL,&ii,flags);//搜索bluetooth，返回搜到的蓝牙个数
        if(num_rsp<0)
        {
            perror("hci_inquiry");
        }

    
        for(i=0;i<num_rsp;i++)
        {
            ba2str(&(ii+i)->bdaddr,addr);   
            memset(name,0,sizeof(name));
    	    if(hci_read_remote_name(sock, &(ii+i)->bdaddr, sizeof(name),name,0)<0)
    	    {
	        strcpy(name ,"[unknow]");
	    }
            printf(" %s %s\n",addr,name);
    
        }
	num=0;

    }
    





    free(ii);
    close(sock);
    return 0;

}
