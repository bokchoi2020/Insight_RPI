from flask import Flask, render_template, request, json, jsonify
from json import *
from random import *
from threading import Thread

import sys
sys.path.insert(0, '../Firmware/')
import comm
import distance

app = Flask('insight_screen')

data = {"username": "Bokchoi", "speed": 0, "speed_unit": "km/h"}

speed = 0

@app.route('/')
def handleGetMain():
	thread_bluetooth = Thread(target = comm.start_comm, args=(handleMsg,))
	thread_bluetooth.start()
	thread_distance = Thread(target = distance.scan_dist)
	thread_distance.start()
	data["speed"] = 0
	return render_template('front.html', data=data)

@app.route('/speed')
def handleGetSpeed():
	resp = {}
	resp["speed"] = speed
	response = app.response_class(
		response = json.dumps(resp),
		status = 200,
		mimetype="application/json"
	)
	return response

def handleMsg(obj):
    global speed
    msgType = obj["messageType"]
    
    if(msgType == "updateDir"):
        print("Distance Left:", obj["distanceUtil"])
        print("Instruction: ", obj["instruction"])
        if("maneuver" in obj):
            print("Direction:", obj["maneuver"])
        pass
    elif(msgType == "updateSpeed"):
        print("Speed: ", obj["speed"])
        speed = obj["speed"]
        pass
    elif(msgType == "updateSetting"):
        print("Property to update :", obj["propName"])
        print("Value :", obj["value"])
        pass
    pass