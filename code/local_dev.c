#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/hci.h>
#include <bluetooth/hci_lib.h>
#include <bluetooth/l2cap.h>
#include <bluetooth/rfcomm.h>
#include <bluetooth/sdp.h>
#include <bluetooth/sdp_lib.h>


static uint16_t uuid = SERIAL_PORT_SVCLASS_ID;
int main()
{
    int                      id,rv,ctl;
    struct hci_dev_list_req  *dl;
    struct hci_dev_req       *dr;
    struct hci_dev_info    *  di;
    char   dest[20];
    
    printf("start\n");
    id=hci_get_route(NULL);
    if(id<0)
    {
        perror("route");
	return -1;
    }
    rv=hci_open_dev(id);
    if(rv<0)
    {
        perror("open");
	return -1;
    }
    di=malloc(sizeof(di));
    hci_devinfo(id,di);
    
     printf("dd\n");
     ba2str(&di->bdaddr, dest);
     printf("%s\n",dest);
    
}
