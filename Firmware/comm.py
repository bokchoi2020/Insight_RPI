import bluetooth
import json

INSIGHT_UUID = "ac9635ee-84a7-11e8-adc0-fa7ae01bbebc"
buffer = []

def est_comm():
    #Initialize RPI bluetooth server socket.
    server_socket = bluetooth.BluetoothSocket( bluetooth.RFCOMM )
    server_socket.bind(("", bluetooth.PORT_ANY))
    server_socket.listen(1)
    port = server_socket.getsockname()[1]

    bluetooth.advertise_service( server_socket, "SampleServer",
                        service_id = INSIGHT_UUID,
                        service_classes = [ INSIGHT_UUID, bluetooth.SERIAL_PORT_CLASS ],
                        profiles = [ bluetooth.SERIAL_PORT_PROFILE ])

    print ("Waiting for connection on RFCOMM channel %d" % port)

    client_socket,address = server_socket.accept()
    print ("Accepted connection from ",address)
    
    return server_socket, client_socket

def handleMsg(obj):
    msgType = obj["messageType"]
    
    if(msgType == "updateDir"):
        print("Distance Left:", obj["distanceUtil"])
        print("Instruction: ", obj["instruction"])
        if("maneuver" in obj):
            print("Direction:", obj["maneuver"])
        pass
    elif(msgType == "updateSpeed"):
        print("Speed: ", obj["speed"])
        pass
    elif(msgType == "updateSetting"):
        print("Property to update :", obj["propName"])
        print("Value :", obj["value"])
        pass
    pass
    

#Establish connection.
server_socket, client_socket = est_comm()

while 1:
    #try to parse the incoming data.
    try:
        raw_data = client_socket.recv(1024)
         #print(raw_data)
        data = str(raw_data.decode('utf-8'))
        print("length of data", len(data))
        print ("Received: %s" % data)
        buffer.extend(list(data))
        index = -1
        try:
            index = buffer.index('\x00')
        except ValueError:
            print("null not found")
            pass
        print(index)
        if(index > -1):
            message = ''.join(buffer[:index])
            print("Full Message:", message)
            del buffer[:index+1]
            handleMsg(json.loads(message))
            client_socket.send("ack\0");
            
        #process the parsed data.
        #print ("JSON parsed: %s" % obj["data"])
        if (data == "q"):
            print ("Quit")
            client_socket.close()
            server_socket.close()
            break
    except bluetooth.btcommon.BluetoothError:
        print ("Android device disconnected.")
        client_socket.close()
        server_socket.close()
        #Wait for reconnection.
        server_socket, client_socket = est_comm()
    except AttributeError:
        print ("Issue parsing json.")
        client_socket.send("nack\0");
    
    
