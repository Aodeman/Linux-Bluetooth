#include <stdio.h>

#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <unistd.h>

#include "bluetooth/bluetooth.h"
#include "bluetooth/hci.h"
#include "bluetooth/hci_lib.h"
#include "bluetooth/l2cap.h"

#define ATT_CID  4


int le_conn(void)
{
    int                  sock;
    bdaddr_t             bd_dst;
    char                buf[32];
    struct hci_dev_info  di,src_id={0};
    struct sockaddr_l2   srcaddr, dstaddr;
    struct bt_security   btsec;
    socklen_t            len;

    

    if((hci_devinfo(hci_get_route(NULL), &di))<0)
    {
        perror("Getting addr failure");
	return -1;
    }     
    
    
    sock = socket(PF_BLUETOOTH, SOCK_SEQPACKET, BTPROTO_L2CAP);
    if (sock < 0) 
    {
        perror("Failed to create socket");
        return -1;
    }

    /* Set up source address */
    memset(&srcaddr, 0, sizeof(srcaddr));
    srcaddr.l2_family = AF_BLUETOOTH;
    srcaddr.l2_cid = htobs(ATT_CID);
    srcaddr.l2_bdaddr_type = 0;
    bacpy(&srcaddr.l2_bdaddr, &src_id.bdaddr);

    if (bind(sock, (struct sockaddr *)&srcaddr, sizeof(srcaddr)) < 0)
    {
        perror("Failed to bind  socket");
        close(sock);
        return -1;
    }

    /* Set the security level */
    memset(&btsec, 0, sizeof(btsec));
    btsec.level = BT_SECURITY_LOW ;
    if (setsockopt(sock, SOL_BLUETOOTH, BT_SECURITY, &btsec, sizeof(btsec)) != 0) 
    {
        fprintf(stderr, "Failed to set L2CAP security level\n");
        close(sock);
        return -1;
    
    }

    listen(sock,4);
    /* Set up destination address */
    memset(&dstaddr, 0, sizeof(dstaddr));

    fflush(stdout);
    while(1)
    {
        if((accept(sock,(struct sockaddr *)&dstaddr,&len))<0)
	{
	    close(sock);
	    return -1;
	}	

        if((read(sock,buf,sizeof(buf)))<0)
	{
	    close(sock);
	    return -1;
	}


    }
    
    return sock;
}

int main(int argc,char *argv[])
{

    int     sock;
    char    buf[16]={0};


   le_conn();
   close(sock);
   return 0;


}
