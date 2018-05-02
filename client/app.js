var SerialPort = require("serialport");
var serialport = new SerialPort("COM3");
var exec = require('child_process').exec;

var playerCmd = "\"C:\\Program Files (x86)\\VideoLAN\\VLC\\vlc\" --intf=\"dummy\" ";

serialport.on('open', function(){
  console.log('Serial Port Opend');
  var msg = '';
  serialport.on('readable', function(){
      var cont = true;
      msg += serialport.read().toString();
      // console.log(msg);
      try {
        JSON.parse(msg);
        cont = false;
        handleCmd(JSON.parse(msg));
        msg = '';
      } catch(e) {
        //continue
        // console.log("invalid");
      }   
  });
});

function handleCmd(cmd) {
  console.log(cmd.event);
  if(cmd.event == "goal") {
    playSound("ritter_horn.wav");
  } else if(cmd.event == "start") {
    playSound("pac.wav");
  } else if(cmd.event == "win") {
    playSound("roar.wav");
  }
}

function playSound(name) {
  var proc = exec(playerCmd + name + " vlc://quit");
}