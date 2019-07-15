#include <stdio.h>
#include <stdlib.h>

#include <sys/ioctl.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <unistd.h>

#include <bluetooth/bluetooth.h>
#include <bluetooth/hci.h>
#include <bluetooth/hci_lib.h>
#include <bluetooth/l2cap.h>
#include <bluetooth/rfcomm.h>
#include <bluetooth/sdp.h>
#include <bluetooth/sdp_lib.h>


int main()
{
    int                      rv,ctl,dev_id;
    struct sockaddr_hci      sock;
    struct hci_dev_info      di;
    struct hci_dev_list_req  *dl;

    dev_id=hci_get_route(NULL);
    ctl=hci_open_dev(dev_id);
   
    dl = malloc(HCI_MAX_DEV * sizeof(struct hci_dev_req) + sizeof(*dl));
    if(dl<0)
    {
        return -1;
    }
    memset(dl, 0, HCI_MAX_DEV * sizeof(struct hci_dev_req) + sizeof(*dl));//hci_for_each_dev();
    memset((void *)&di, 0, sizeof(struct hci_dev_info));
    dl->dev_num = HCI_MAX_DEV;
    di.dev_id = dev_id;
    rv=ioctl(ctl, HCIGETDEVINFO, (void *) &di);
//    rv=ioctl(ctl,HCIGETDEVLIST, (void *)dl);
    if(rv<0)
    {
        perror("HCIGETDEVINFO");
        goto error;
    }
    rv=ioctl(ctl,HCIDEVDOWN);
    if(rv<0)
    {
        perror("DOWN");
        goto error;
    }


error:
    free(dl);
    close(ctl);
    return -1;
}
