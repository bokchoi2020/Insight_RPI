import bluetooth
 
server_socket=bluetooth.BluetoothSocket( bluetooth.RFCOMM )
uuid = ac9635ee-84a7-11e8-adc0-fa7ae01bbebc 
#port = 1
#server_socket.bind(("",port))
server_sock.bind(("", bluetooth.PORT_ANY))
server_socket.listen(1)
port = server_sock.getsockname()[1]

bluetooth.advertise_service( server_socket, "SampleServer",
                        service_id = uuid,
                        service_classes = [ uuid, bluetooth.SERIAL_PORT_CLASS ],
                        profiles = [ bluetooth.SERIAL_PORT_PROFILE ] 
                         )

print "Waiting for connection on RFCOMM channel %d" % port

client_socket,address = server_socket.accept()
print "Accepted connection from ",address
while 1:
 data = client_socket.recv(1024)
 print "Received: %s" % data
 if (data == "q"):
  print ("Quit")
  break 
client_socket.close()
server_socket.close()
