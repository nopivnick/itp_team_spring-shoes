
var song;

var val

function preload(){
  song = loadSound('assets/happy.mp3');

  
}


function setup() {
  createCanvas(200,200);
 
}
function draw() {
  background(255);
}

function keyTyped() {
  if (key === 'a') {
      
     song.play();
 
    }
  else if (key === 'b') {

    song.stop();
  }
}


