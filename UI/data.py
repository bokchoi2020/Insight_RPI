from flask import Flask, render_template
from json import *
from random import *

app = Flask('insight_screen')

data = {"username": "Bokchoi", "speed": 0, "speed_unit": "km/h"}

@app.route('/')
def hello():
	data["speed"] = random()*60
	return render_template('front.html', data=data)
