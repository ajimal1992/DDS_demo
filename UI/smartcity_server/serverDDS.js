//cvar express = require('express');
//cvar app = express();

//cvar server = require('http').createServer(app);

var fs = require('fs');
express = require('express');
app = express();
var server = require('http').createServer(app);
var io = require('socket.io')(server);
var request = require('request');

server.listen(55555, function() {
    console.log('Server listening at port %d', 55555);
});

var trigger_is_done = true;


//host the index file
app.use(express.static(__dirname));

//spawn process
var spawn = require('child_process').spawn;
var publisher = spawn('./UI/rtps-dds-connector/build/app',['publisher']);
var subscriber = spawn('./UI/rtps-dds-connector/build/app',['subscriber']);

//
var lamp_states = {};

subscriber.stdout.on('data', function(data) {
    var rawDatas = data.toString().split('\n');
    rawDatas.forEach(function(rawData){
    	if(rawData){ //if data exists
            var topic = "";
            var ID = "";
            var value = "";
    	    if(rawData.search(topic = "Temperature")!=-1){
                ID = rawData.substring(rawData.search("ID=")+3,rawData.search(","));
                value = rawData.substring(rawData.search("value=")+6);
            }
            else if(rawData.search(topic = "Trafficlight")!=-1){
                ID = rawData.substring(rawData.search("ID=")+3,rawData.search(","));
                value = rawData.substring(rawData.search("value=")+6);
            }
            else if(rawData.search(topic = "Priority")!=-1){
                // console.log(rawData);
                ID = rawData.substring(rawData.search("ID=")+3);
                // console.log("ID = " + ID);
                // console.log("index: " + rawData.search("ID=")+3);

            }
            else if(rawData.search(topic = "LamppostsBC")!=-1){
                ID = rawData.substring(rawData.search("ID=")+3,rawData.search(","));
                value = rawData.substring(rawData.search("value=")+6);
                lamp_states[ID] = value;
            }
            else if(rawData.search(topic = "Humidity")!=-1){
                ID = rawData.substring(rawData.search("ID=")+3,rawData.search(","));
                value = rawData.substring(rawData.search("value=")+6);
            }
            else if(rawData.search(topic = "Light")!=-1){
                ID = rawData.substring(rawData.search("ID=")+3,rawData.search(","));
                value = rawData.substring(rawData.search("value=")+6);
            }
            else if(rawData.search(topic = "Speeding")!=-1){
                ID = rawData.substring(rawData.search("ID=")+3,rawData.search(","));
                value = rawData.substring(rawData.search("value=")+6);
            }
            else if(rawData.search(topic = "GeoLocation")!=-1){
                ID = rawData.substring(rawData.search("ID=")+3,rawData.search(","));
                value = rawData.substring(rawData.search("value=")+6);
            }
            else if(rawData.search(topic = "Carpark")!=-1){
                //console.log(rawData);
                ID = rawData.substring(rawData.search("ID=")+3,rawData.search(","));
                value = rawData.substring(rawData.search("value=")+6);
            }
            else{
                topic = "Undefined Topic"
                ID = rawData;
            }
            var json = {
                "ID": ID,
                "value": value
            };
            if(topic == "Speeding" && value=="1"){
                notifySpeeding();
            }
            io.emit(topic, json);       
    	}
    });
});

io.on('connection', function(socket) {
    socket.on("LamppostsCtrl", function(data) {
        console.log("recieved: ID->" + data.ID + "-value->" + data.value);
        toggleFakeLampPost(data.ID,data.value);
    });
});


function sendCommand(clusterID, value){
    publisher.stdin.write(clusterID + '\n');
    publisher.stdin.write(value + '\n');
}


/*
sensorsport.on('data', function(data) {
    //sensors object
    var sensors = {
        temperature: null,
        humididty: null,
        intensity: null,
        geolocation: null,
        speeding: null,
        carpark: null
    }
    //parse data from serialport
    sensors.temperature = data.substring(0, data.indexOf('t'));
    sensors.humidity = data.substring(data.indexOf('t') + 1, data.indexOf('h'));
    sensors.intensity = data.substring(data.indexOf('h') + 1, data.indexOf('i'));
    sensors.speeding = data.substring(data.indexOf('i') + 1, data.indexOf('s'));
    sensors.geolocation = data.substring(data.indexOf('s') + 1, data.indexOf('g'));
    sensors.carpark = data.substring(data.indexOf('g') + 1, data.indexOf('c'));

    if(sensors.speeding == "1")
    {
        console.log("triggered");
        notifySpeeding();
    }
    // console.log(sensors);

    //send the data to the client
    io.emit("sensors", sensors);
});
*/

/*
trafficport.on('data', function(data) {
    //sensors object
    var trafficlight = {
        trafficlight1: null,
        trafficlight2: null,
    }

    var ambulance;

    //parse data from serialport
    trafficlight.trafficlight1 = data.substring(0, data.indexOf('a'));
    trafficlight.trafficlight2 = data.substring(data.indexOf('a') + 1, data.indexOf('b'));
    ambulance = data.substring(data.indexOf('b') + 1, data.indexOf('t'));

    //send the data to the client

    //this block is to filter the data from sending continuously
    //only send to client if ambulance data is parsed. Otherwise send traffic light data
    if(ambulance ==  1){
        io.emit("ambulance", ambulance);
        // console.log(ambulance);
    }

    else{
        io.emit("trafficlight", trafficlight);
        console.log(trafficlight);
    }
});
*/

/*
app.get('/lamppost', function(request, response) {
    //smartcity.local:8000/lamppost?state=true
    //this opens an API for external app to control real lamp post
    var state = request.query.state;
    //convert string to real booleans
    if (state == 'true')
        state = true;
    else if (state == 'false')
        state = false;
    response.end("Received Lamp Post State: " + state);
    toggleRealLampPost(state);
    toggleFakeLampPost(2,state);
});

app.get('/states', function(request, response) {
	console.log("Testing.");
	var JSONres = {
		states: lamp_states
	};
	
	//console.log("testing with log");
	console.log(JSONres);
    response.json(JSONres); 
});

app.get('/toggleState', function(request, response) {
	//http://localhost:55555/toggleState?state=true&index=0
	var state = request.query.state;
	var index = request.query.index;
	var JSONres = {
		index: index,
		state: state
	};
	
	if (state == 'true')
        state = true;
    else if (state == 'false')
        state = false;
    //toggleRealLampPost(state);
    toggleFakeLampPost(index,state);
	console.log("Received state: " + state + ", index: " + index);	
	response.json(JSONres); //Send an OK response
});
*/
function toggleRealLampPost(state) {
    if (state == true) {
        var url = "http://mylinkit.local:8001/?value=high"
    } else if (state == false) {
        var url = "http://mylinkit.local:8001/?value=low"
    }
    try{
        request(url, function(error, response, body) {
            // console.log('error:', error); // Print the error if one occurred
            // console.log('statusCode:', response && response.statusCode); // Print the response status code if a response was received
            // console.log('body:', body); // Print the HTML for the Google homepage.
        });
    }
    catch(err){
        console.log(err);
    }
}

function toggleFakeLampPost(ID,value){
    lamp_states[ID] = value;
    sendCommand(ID,value);
    if(ID == "c3"){
        if(value == "1")
            toggleRealLampPost(true);
        else
            toggleRealLampPost(false);
    }
}

/*
function toggleFakeAndRealLampposts(state){
    var url = "";
    if (state == true) {
        sendCommand("c3","1");
        url = "http://mylinkit.local:8001/?value=high";
    } 
    else if (state == false) {
        sendCommand("c3","0");
        url = "http://mylinkit.local:8001/?value=low";
    }
    try{
        request(url, function(error, response, body) {
            // console.log('error:', error); // Print the error if one occurred
            // console.log('statusCode:', response && response.statusCode); // Print the response status code if a response was received
            // console.log('body:', body); // Print the HTML for the Google homepage.
        });
                //stopre lamp post states
        lamp_states[3] = state;

        //update the client of the lamppost states
        // io.emit("lamp_states", lamp_states);
    }
    catch(err){
        console.log(err);
    }
}
*/

function notifySpeeding(){
    var counter = 0;
    var state = lamp_states["c3"];
    var speed_trigger = setInterval(function (){
        toggleFakeLampPost("c3",state);
        //DELETE THIS BLOCK IF LAMPPOST IS DOWN!!!!
        //PANIC MODE STARTS HERE
        // try{
        //     request(url, function(error, response, body) {
        //         // console.log('error:', error); // Print the error if one occurred
        //         // console.log('statusCode:', response && response.statusCode); // Print the response status code if a response was received
        //         // console.log('body:', body); // Print the HTML for the Google homepage.
        //     });
        // }
        // catch(err){
        //     console.log(err);
        // }
        //PANIC MODE ENDS HERE
        
        if(counter == 10){
            // return;
            clearInterval(speed_trigger);
            toggleFakeLampPost("c3",state);
        }
        if(state == "1")
            state = "0";
        else state = "1";
        counter = counter + 1;
    },200);
}



