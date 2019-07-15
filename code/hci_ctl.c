#include<stdio.h>
#include<hci.h>
#include<bluetooth.h>


int main()
{
	int     ctl,dev_id;

	dev_id=hci_get_route(NULL);
	ctl=hci_open_dev(dev_id);

	


}
