// Copyright (c) 2018 p5ble
//
// This software is released under the MIT License.
// https://opensource.org/licenses/MIT

// The serviceUuid must match the serviceUuid of the device you would like to connect
const serviceUuid = "b6292c11-911a-4a51-b7f2-43fe53e62a77";
let skipStateCharacteristic;
let skipState = 0;
let shoeBLE;

function setup() {
  // Create a p5ble class
  shoeBLE = new p5ble();

  createCanvas(200, 200);
  textSize(20);
  textAlign(CENTER, CENTER);

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

function draw() {
  background(250);
  // Write value on the canvas
  text(skipState, 100, 100);
}