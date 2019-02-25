// p5.serialcontrol stuff
let serial; // declare a global variable for serial class
let portName = '/dev/cu.usbmodem1411';

let song;
let val

function preload() {
  song = loadSound('assets/happy.mp3');
}

function setup() {
  createCanvas(200, 200);
}

function draw() {
  background(255);
}


// serial data from the Arduino stuff

function serialEvent() {
  // the Arduino should be sending ASCII over serial
  let serialStringIn = serial.readLine(); // declare a variable to store incoming serial data as a string

  // listen for a skip detected on the shoe
  if (serialStringIn == "SKIP detected!") {
    print("SKIP detected!");
    song.play();
  }
}


// keyboard settings for troubleshooting the p5 sketch w/o serial input from the Arduino

function keyTyped() {
  if (key === 'a') {
    song.play();
  } else if (key === 'b') {
    song.stop();
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