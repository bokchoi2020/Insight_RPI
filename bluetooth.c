#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/sdp.h>
#include <bluetooth/sdp_lib.h>
#include <bluetooth/rfcomm.h>

//thanks to https://github.com/RyanGlScott/BluetoothTest/blob/master/C%20BlueZ%20Server/bluez_server.c
//thanks to Albert Huang, MIT https://people.csail.mit.edu/albert/bluez-intro/
/* To compile this, use the following Bash command:
* gcc -I/usr/include/glib-2.0/ -I/usr/lib/glib-2.0/include -o server-with-haskell server-with-haskell.c -lbluetooth
/* Allows this service to be discovered when running sdptool. For example, you can find this service
* after starting it by running
*
* $ sdptool browse local
*
* (Adapted from http://www.btessentials.com/examples/bluez/sdp-register.c)
* */

//bADDR temp variables for c++ 
//(in C you can just use temporary addresses BADDR_ANY and BADDR_LOCAL in bluetooth.h)
bdaddr_t bADDR_ANY = {0, 0, 0, 0, 0, 0};
bdaddr_t bADDR_LOCAL = {0, 0, 0, 0xff, 0xff, 0xff};

sdp_session_t *register_service(uint8_t rfcomm_channel) {

	/* A 128-bit number used to identify this service. The words are ordered from most to least
	* significant, but within each word, the octets are ordered from least to most significant.
	* For example, the UUID represneted by this array is "ac9635ee-84a7-11e8-adc0-fa7ae01bbebc". (The
	* hyphenation is a convention specified by the Service Discovery Protocol of the Bluetooth Core
	* Specification, but is not particularly important for this program.)
	
	* it is represented as 0xEE3596AC, 0xE811A784, 0x7AFAC0AD, 0xBCBE1BE0 due to 
	* little endian to big endian conversion or vice versa.
	
	* This UUID is the Bluetooth Base UUID and is commonly used for simple Bluetooth applications.
	* Regardless of the UUID used, it must match the one that the Armatus Android app is searching
	* for.
	*/
	uint32_t svc_uuid_int[] = { 0xEE3596AC, 0xE811A784, 0x7AFAC0AD, 0xBCBE1BE0 };
	const char *service_name = "Insight Bluetooth server";
	const char *svc_dsc = "A HERMIT server that interfaces with the Insight Android app";
	const char *service_prov = "Insight";

	uuid_t root_uuid, l2cap_uuid, rfcomm_uuid, svc_uuid,
	       svc_class_uuid;
	sdp_list_t *l2cap_list = 0,
	            *rfcomm_list = 0,
	             *root_list = 0,
	              *proto_list = 0,
	               *access_proto_list = 0,
	                *svc_class_list = 0,
	                 *profile_list = 0;
	sdp_data_t *channel = 0;
	sdp_profile_desc_t profile;
	sdp_record_t record = { 0 };
	sdp_session_t *session = 0;

	// set the general service ID
	sdp_uuid128_create(&svc_uuid, &svc_uuid_int);
	sdp_set_service_id(&record, svc_uuid);

	char str[256] = "";
	sdp_uuid2strn(&svc_uuid, str, 256);
	printf("Registering UUID %s\n", str);

	// set the service class
	sdp_uuid16_create(&svc_class_uuid, SERIAL_PORT_SVCLASS_ID);
	svc_class_list = sdp_list_append(0, &svc_class_uuid);
	sdp_set_service_classes(&record, svc_class_list);

	// set the Bluetooth profile information
	sdp_uuid16_create(&profile.uuid, SERIAL_PORT_PROFILE_ID);
	profile.version = 0x0100;
	profile_list = sdp_list_append(0, &profile);
	sdp_set_profile_descs(&record, profile_list);

	// make the service record publicly browsable
	sdp_uuid16_create(&root_uuid, PUBLIC_BROWSE_GROUP);
	root_list = sdp_list_append(0, &root_uuid);
	sdp_set_browse_groups(&record, root_list);

	// set l2cap information
	sdp_uuid16_create(&l2cap_uuid, L2CAP_UUID);
	l2cap_list = sdp_list_append(0, &l2cap_uuid);
	proto_list = sdp_list_append(0, l2cap_list);

	// register the RFCOMM channel for RFCOMM sockets
	sdp_uuid16_create(&rfcomm_uuid, RFCOMM_UUID);
	channel = sdp_data_alloc(SDP_UINT8, &rfcomm_channel);
	rfcomm_list = sdp_list_append(0, &rfcomm_uuid);
	sdp_list_append(rfcomm_list, channel);
	sdp_list_append(proto_list, rfcomm_list);

	access_proto_list = sdp_list_append(0, proto_list);
	sdp_set_access_protos(&record, access_proto_list);

	// set the name, provider, and description
	sdp_set_info_attr(&record, service_name, service_prov, svc_dsc);

	// connect to the local SDP server, register the service record,
	// and disconnect
	session = sdp_connect(&bADDR_ANY, &bADDR_LOCAL, SDP_RETRY_IF_BUSY);
	sdp_record_register(session, &record, 0);

	// cleanup
	sdp_data_free(channel);
	sdp_list_free(l2cap_list, 0);
	sdp_list_free(rfcomm_list, 0);
	sdp_list_free(root_list, 0);
	sdp_list_free(access_proto_list, 0);
	sdp_list_free(svc_class_list, 0);
	sdp_list_free(profile_list, 0);

	return session;
}
struct sockaddr_rc loc_addr = { 0 }, rem_addr = { 0 };
int sock, client;

int init_server() {
	int port = 1, result;
	//struct sockaddr_rc loc_addr = { 0 }, rem_addr = { 0 };
	char buffer[1024] = { 0 };
	socklen_t opt = sizeof(rem_addr);

	// local bluetooth adapter
	loc_addr.rc_family = AF_BLUETOOTH;
	loc_addr.rc_bdaddr = bADDR_ANY;//*BDADDR_ANY;
	loc_addr.rc_channel = (uint8_t) port;

	// register service
	sdp_session_t *session = register_service(port);

	// allocate socket
	sock = socket(AF_BLUETOOTH, SOCK_STREAM, BTPROTO_RFCOMM);
	printf("socket() returned %d\n", sock);

	// bind socket to port 1 of the first available
	result = bind(sock, (struct sockaddr *)&loc_addr, sizeof(loc_addr));
	printf("bind() on channel %d returned %d\n", port, result);

	// put socket into listening mode
	result = listen(sock, 1);
	printf("listen() returned %d\n", result);

	//sdpRegisterL2cap(port);

	// accept one connection
	printf("calling accept()\n");
	client = accept(sock, (struct sockaddr *)&rem_addr, &opt);
	printf("accept() returned %d\n", client);

	ba2str(&rem_addr.rc_bdaddr, buffer);
	fprintf(stderr, "accepted connection from %s\n", buffer);
	memset(buffer, 0, sizeof(buffer));

	return client;
}

void reconnnect()
{
	close(client);
	socklen_t opt = sizeof(rem_addr);
	char buffer[1024] = { 0 };

	printf("calling accept()\n");
	client = accept(sock, (struct sockaddr *)&rem_addr, &opt);
	printf("accept() returned %d\n", client);

	ba2str(&rem_addr.rc_bdaddr, buffer);
	fprintf(stderr, "accepted connection from %s\n", buffer);
	memset(buffer, 0, sizeof(buffer));
	
}

char input[1024] = { 0 };
char *read_server(int client) {
	// read data from the client
	//char input[1024] = { 0 };
	int bytes_read;
	bytes_read = read(client, input, sizeof(input));
	if (bytes_read > 0) {
		printf("received [%s]\n", input);
		return input;
	} else {
		printf("Disconnected. read returned %d\n", bytes_read);
		if(bytes_read == 0)
			printf("Disconnected properly.\n");
		else
			printf("Sudden error caused disconnect.\n");
		reconnnect();
		return "";
	}
}

void write_server(int client, char *message) {
	// send data to the client
	char messageArr[1024] = { 0 };
	int bytes_sent;
	sprintf(messageArr, message);
	bytes_sent = write(client, messageArr, sizeof(messageArr));
	if (bytes_sent > 0) {
		printf("sent [%s]\n", messageArr);
	}
}

int main()
{
	int client = init_server();	

	char message[1024];
	while(1)
	{
		char *temp = read_server(client);
		memcpy(&message, temp, sizeof(message));

		printf("%s", temp);

	}
	return 0;
}