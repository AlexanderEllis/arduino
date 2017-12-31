/*
  This is the very simple server code for listening to and responding to the arduino's button press.
  It listens for data from the arduino, and if it receives an ON signal, it returns an ON signal. Likewise for OFF.
*/
var SerialPort = require('serialport');
var PATH = '/dev/cu.usbmodemFD131';

var port = new SerialPort(PATH, { baudRate: 9600 }, function(err) {
  if (err) {
    console.log('Error: ', err.message);
  }
});

port.on('open', function() {
  port.on('data', function(data) {
    console.log(data[0]);
    if (data[0]) {
      port.write([1]);
    } else {
      port.write([0]);
    }
  });
});
