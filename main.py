from flask import Flask, request, render_template
from flask_pymongo import PyMongo

app = Flask(__name__)
app.config["MONGO_URI"] = "mongodb://localhost:27017/finalproject"
mongo = PyMongo(app)
db = mongo.db
app = Flask(__name__)

@app.route("/api")
def receive_data():
    ## use args to get 
    humidity = request.args['humidity']
    temperature = request.args["temp"]
    light = request.args["light"]
    sound = request.args["sound"]
    db.data.insert_one({"humidity": humidity, "temperature": temperature, "light":light, "sound":sound})
    print("{}    {}    {}   {}".format(humidity, temperature, light, sound))
    return "success"



@app.route("/page/<id>")
def render_charts(id):
    cursor = db.data.find({})
    humidity = []
    count = 0 
    for doc in cursor:
        count += 1
        humidity.append({"y":float(doc[id]), "x":count})
    return render_template("chart.html", data=humidity[1:])