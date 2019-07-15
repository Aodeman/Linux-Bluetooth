#include <stdio.h>
#include <bluetooth/sdp.h>
#include <bluetooth/sdp_lib.h>
#include <stdlib.h>
#include <bluetooth/hci.h>
#include <bluetooth/hci_lib.h>

int main(int argc,char *argv[])
{
    int        rv,sock;
    bdaddr_t   dest_addr;
    uuid_t     uuid;
    uint32_t   range = 0x0000ffff;
   
    sdp_session_t *  session;
    sdp_list_t  *    search,* attrid,* rsp; 
    struct hci_dev_info di;
    

    if(argc<2)
    {
        printf("usage: %s <bdaddr>\n",argv[0]);    
        return -1;
    }

    str2ba(argv[1],&dest_addr);

    rv=hci_devinfo(0,&di);
    if(rv<0)
    {
        perror("GET LOCAL BLUETOOTH ADDRESS FAILED");
        return -1;
    }




    printf("start\n");
    session=sdp_connect(&di.bdaddr,&dest_addr,SDP_RETRY_IF_BUSY);
    if(!session)    
    {
        perror("CONNECY");
	return -1;
    }

    sdp_uuid16_create(&uuid,PUBLIC_BROWSE_GROUP);
    search = sdp_list_append(0,&uuid);
    attrid = sdp_list_append(0,&range);
    
    printf("next\n");
    rv=sdp_service_search_attr_req(session,search,SDP_ATTR_REQ_RANGE,attrid,&rsp);
    if(rv<0)
    {
        perror("SEARCH");
	return -1;
    }
    printf("end\n");
    sdp_list_free(attrid, 0);
    sdp_list_free(search, 0);
    if(rv==0)
    {
        printf("Nothing!\n");
	return 0;
    }

    for(;rsp;rsp=rsp->next)
    {
        int    channel;
        sdp_record_t *rec = (sdp_record_t *) rsp->data;
        sdp_list_t *access = NULL;

        sdp_record_print(rec);
	sdp_get_access_protos(rec, &access);

	if(access)
	{
	    channel= sdp_get_proto_port(access, RFCOMM_UUID);
	    printf("Channel : %d\n",channel);
	
	}


    }

    free(rsp);
    sdp_close(session);
    return 0;

}
