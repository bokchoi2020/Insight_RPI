import bluetooth

#Create bluetooth sock for RFCOMM
server_socket = bluetooth.BluetoothSocket(bluetooth.RFCOMM)

#Bind script on host "" to port
port=1
server_socket.bind(("",port))

#Server accepts one connection at a time
server_socket.listen(1)

#server accepts client's connection request
client_socket, address = server_socket.accept()

print "Accepted Connection from", address

while 1:
	data = client_socket.recv(1024)
	print "Received %s" %data
	
	if(data == "quit"):
		break

client_socket.close()
server_socket.close()
