/*
  Spring Shoes
  Idit Barak and Noah Pivnick
  2189.00 Intro to Wearables (Jingwen Zhu)

  This sketch borrows code from the following examples:

   - BLE start notification example on Jingwen Zhu's ITP Intro to Wearables github repo
     https://github.com/ZhuJingwen/intro-to-wearables-2019/tree/1b629b750c70ed50bbc4415cd270ffeea3e7c945/Week%207%20Examples/Week7_Example_BLE_startNotification

*/

let skipSong;
let playSong = false;

/*
   Bluetooth LE stuff
*/

// The serviceUuid must match the serviceUuid of the device you would like to connect
const serviceUuid = "b6292c11-911a-4a51-b7f2-43fe53e62a77";
let skipStateCharacteristic;
let skipState = 0;
let shoeBLE;

/*
   p5.serialcontrol stuff
*/

let serial; // declare a global variable for serial class
let portName = '/dev/cu.usbmodem1411';

function preload() {
  skipSong = loadSound('assets/skipSong.mp3');
}

function setup() {
  // Create a p5ble class
  shoeBLE = new p5ble();

  createCanvas(200, 200);
  textSize(20);
  textAlign(CENTER, CENTER);

  /*
     Bluetooth LE stuff
  */

  // Create a 'Connect and Start Notifications' button
  const connectButton = createButton('Connect and Start Notifications')
  connectButton.mousePressed(connectAndStartNotify);

  // Create a 'Stop Notifications' button
  const stopButton = createButton('Stop Notifications')
  stopButton.mousePressed(stopNotifications);
}

function connectAndStartNotify() {
  // Connect to a device by passing the service UUID
  shoeBLE.connect(serviceUuid, gotCharacteristics);
}

// A function that will be called once got characteristics
function gotCharacteristics(error, characteristics) {
  if (error) console.log('error: ', error);
  console.log('characteristics: ', characteristics);
  skipStateCharacteristic = characteristics[0];
  console.log(skipStateCharacteristic);
  // Start notifications on the first characteristic by passing the characteristic
  // And a callback function to handle notifications
  shoeBLE.startNotifications(skipStateCharacteristic, handleNotifications);
  // You can also pass in the dataType
  // Options: 'unit8', 'uint16', 'uint32', 'int8', 'int16', 'int32', 'float32', 'float64', 'string'
  // shoeBLE.startNotifications(skipStateCharacteristic, handleNotifications, 'string');
}

// A function that will be called once got characteristics
function handleNotifications(data) {
  console.log('data: ', data);
  skipState = data;
}

// A function to stop notifications
function stopNotifications() {
  shoeBLE.stopNotifications(skipStateCharacteristic);
}

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


function draw() {
  background(250);
  // Write value on the canvas
  text(skipState, 100, 100);


  if (skipState == 1) {
    playSong = true;

  } else if (skipState == 0) {
    playSong = false;
  }

  if (playSong === true && !skipSong.isPlaying()) {
    skipSong.play();
    print("skipSong started");

  } else if (playSong === false && skipSong.isPlaying()) {
    skipSong.stop();
    print("skipSong stopped");
  }
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