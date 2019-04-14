/*
   p5.serialcontrol stuff
*/

let serial; // declare a global variable for serial class
let portName = '/dev/cu.usbmodem1411';

let skipSong;
let val

function preload() {
  skipSong = loadSound('assets/skipSong.mp3');
}

function setup() {
  createCanvas(200, 200);

  /*
     p5.serialcontrol stuff
  */

  serial = new p5.SerialPort("10.17.231.12"); // instantiate a new serial object
  serial.on('list', printList); // list the available serial ports
  serial.on('connected', serverConnected); // callback for connecting to the server
  serial.on('open', portOpen); // callback for the port opening
  serial.on('data', serialEvent); // callback for when new data arrives
  serial.on('error', serialError); // callback for errors
  serial.on('close', portClose); // callback for the port closing
  serial.open(portName);
}

function draw() {
  background(255);
}

// serial data from the Arduino stuff

function serialEvent() {
  // the Arduino should be sending ASCII over serial
  let serialStringIn = serial.readLine(); // declare a variable to store incoming serial data as a string

  // listen for a threshold met under BALL of foot message from the Arduino
  if (serialStringIn == " *** BALL sensor ABOVE threshold *** ") {
    print("Ball");
    // listen for a threshold met under HEEL of foot message from the Arduino
  } else if (serialStringIn == " *** HEEL sensor ABOVE threshold *** ") {
    print("Heel");
    // listen for a skip detected on the shoe
  } else if (serialStringIn == "SKIP!") {
    print("SKIP!");
    skipSong.play();
  }
}

// keyboard settings for troubleshooting the p5 sketch w/o serial input from the Arduino

function keyTyped() {
  if (key === 'a') {
    skipSong.play();
    print("skipSong started");
  } else if (key === 'b') {
    skipSong.stop();
    print("skipSong stopped");
  }
}


// functions used with p5.serialcontrol

// get a list of serial port names and put them in an array called portList
function printList(portList) {
  // loop through the array of serial port names
  for (var i = 0; i < portList.length; i++) {
    // and print the list to console
    print(i + " " + portList[i]);
  }
}

function serverConnected() {
  print('connected to server.');
}

function portOpen() {
  print('the serial port opened.')
}

function serialError(err) {
  print('Something went wrong with the serial port. ' + err);
}

function portClose() {
  print('The serial port closed.');
}