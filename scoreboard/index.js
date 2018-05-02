const express = require('express');
var app = express();
var http = require('http').Server(app);
var io = require('socket.io')(http);
var SerialPort = require("serialport");
var serialport = new SerialPort("COM3", {
    autoOpen: false
});

//Tracking state of game
var state = {
    serial: {
        connected: false,
        connecting: false
    },
    game: {
        active: false,
        score: [0, 0],
        startTime: undefined,
        endTime: undefined
    }
}
var serialRetries = 5;
var msg = '';

//Setup root and assets files
app.get('/', function(req, res){
    res.sendFile(__dirname + '/views/index.html');
});
app.use(express.static('public'));

//Serial port event handlers
serialport.on('open', function(){
    console.log('serial connected');
    state.serial.connected = true;
    state.serial.connecting = false;
    sendEvent('serial-connected');
});

serialport.on('error', function(err){
    console.log(err.message);
    state.serial.connected = false;
    serialRetries--;
    if(serialRetries > 0) {
        state.serial.connecting = true;
        setTimeout(function() {
            serialport.open();
        }, 5000);
    } else {
        console.log("serial connection failed");
        state.serial.connecting = false;
        sendEvent('serial-connected');
    }
});

serialport.on('readable', function(){
    var cont = true;
    msg += serialport.read().toString();
    try {
        var validMsg = JSON.parse(msg);
        cont = false;
        handleCmd(validMsg);
        msg = '';
    } catch(e) {
        //continue
    }   
});

//WebSocket events
io.on('connection', function(socket) {
    console.log('client connected');
    sendEvent('init');

    socket.on('message', function() {
        if(!state.serial.connected) {
            serialRetries = 5;
            console.log('serial connecting...');
            serialport.open();
            state.serial.connecting = true;
            sendEvent('serial-connecting');
        }
    });
});

http.listen(3000, function(){
    console.log('listening on *:3000');
});

function handleCmd(msg) {
    console.log(msg.event);
    if(msg.event === 'start') {
        resetGame();
    } else if(msg.event === 'goal' || msg.event === 'adjust') {
        state.game.score = msg.score;
    } else if(msg.event === 'win') {
        endGame();
    }
    sendEvent(msg.event);
}

function sendEvent(event) {
    var msg = {event: event, state: state}
    io.emit('event', msg);
}

function resetGame() {
    game.active = true;
    game.startTime = Date.now();
    game.score = [0, 0];
}

function endGame() {
    state.game.active = false;
    state.game.endTime = Date.now();
}