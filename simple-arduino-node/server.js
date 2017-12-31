var SerialPort = require('serialport');

var port = new SerialPort('/dev/cu.usbmodemFD131', { baudRate: 9600 }, function(err) {
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
