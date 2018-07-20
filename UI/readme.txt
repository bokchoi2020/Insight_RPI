You'll need to install Flask 1.0: http://flask.pocoo.org/docs/1.0/installation/

To start the server, you'll need to first start a python virtual environment.
(Instructions are also in the link above.)
With the venv started, run
"$env:FLASK_APP="server.py"; flask run" to start the server.
Hit up http://127.0.0.1:5000/ in a browser to see the client.

All server code is in server.py.
All client code is in templates/front.html.
