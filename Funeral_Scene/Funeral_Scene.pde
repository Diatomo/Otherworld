


import gohai.glvideo.*;
import processing.serial.*;
import ddf.minim.*;


Minim minim;
AudioPlayer minor, major;

//commmunication
//Serial myPort;

//assets
GLMovie myMovie;

int volume = 0;
int volMinor = 0;
int volMajor = 254;

float[] numbers;

void setup(){
  size(2000,1800, P2D);//dimension of canvas
  //fullScreen();
  frameRate(120);

  numbers = new float[255];
  float acc = -30;
  int counter = 0;
  while (counter < 255){
    acc = acc + (60.0/255.0);
    numbers[counter] = acc;
    //println(numbers[counter]);
    counter++;
  }

  //printArray(Serial.list());
  myPort = new Serial(this, Serial.list()[0]);

  //assets
  myMovie = new GLMovie(this, "ambient.mp4");
  minim = new Minim(this);

  minor = minim.loadFile("minor.mp3");
  major = minim.loadFile("major.mp3");

  //loops
  minor.loop();
  major.loop();
  //volume
  minor.setGain(numbers[volMinor]);
  major.setGain(numbers[volMajor]);
  myMovie.loop();
}


/*
 *  fxn :: draw
 */
void draw(){
  if (myMovie.available()){
    myMovie.read();
  }
  image(myMovie,0,0,width,height);
  update();
  changeVolume();
}

void changeVolume(){
  minor.setGain(numbers[volMinor]);
  major.setGain(numbers[volMajor]);
}

void update(){
   // while (myPort.available() > 0){
     // volume = myPort.read();
    //}
    //println(volume);
    if (myMovie.available()){
      myMovie.read();
    }
    image(myMovie,0,0,width,height);
    if (volume >= 0 && volume < 254){
      volMinor = (254 - volume);
      if (volume == 0){
        volMajor = volume;
      }
      else{
        volMajor = volume - 1;
      }
  }
}

