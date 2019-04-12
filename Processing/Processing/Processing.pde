import processing.serial.*;

Serial serial;

ArrayList<PVector> points = new ArrayList<PVector>();
PVector position = new PVector(0,0);
float rotation = 0;
float last_distance = 0;
float WALL_HEIGHT = 100;
float SPEED = 2.0;

// Processing Code

void setup(){
  size(640, 640);
  smooth();
  //serial = new Serial(this, "COM6", 9600);
}

void draw(){
  update();
  
  // Background
  background(127);
  
  // Text
  fill(255);
  text("Position : (" + position.x + ", " + position.y + ")", 10, 20);
  text("Direction : " + rotation, 10, 40);
  text("Last Distance : " + last_distance, 10, 60);
  
  // Shift
  translate(width/2, height/2);
  
  //Player
  noStroke();
  fill(255,0,0);
  ellipse(0, 0, 30, 30);
  
  // Points
  rotate(rotation - PI/2);
  translate(position.x, position.y);
  stroke(0);
  strokeWeight(5);
  
  for(PVector point : points){
    point(point.x, point.y);
  }
}

void update(){
  //serial_read();
}

void keyPressed(){
  serial.write(key);
}

// User-defined Functions

void add_point(float distance){
  last_distance = distance;
  points.add(new PVector(distance * cos(-rotation) - position.x, distance * sin(-rotation) - position.y));
}

void move(boolean forward){
  if(forward){
    position = new PVector(position.x - cos(rotation) * SPEED, position.y + sin(rotation) * SPEED);
  }else{
    position = new PVector(position.x + cos(rotation) * SPEED, position.y - sin(rotation) * SPEED);
  }
}

void serial_read(){
  while(serial.available() > 0){
    int function = serial.read();
    
    switch(function){
      case 'w':
        move(true);
        break;
      case 's':
        move(false);
        break;
      case 'p':
        int value = serial.read();
        add_point(value);
        break;
      case 'a':
        rotation+=PI/8;
        break;
      case 'd':
        rotation-=PI/8;
        break;
    }
  }
}
