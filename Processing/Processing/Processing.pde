import processing.serial.*;

Serial serial;

ArrayList<PVector> points = new ArrayList<PVector>();
PVector position = new PVector(1,0);
float rotation = 0;
float WALL_HEIGHT = 100;
float SPEED = 2.0;

void setup(){
  size(640, 640);
  smooth();
  serial = new Serial(this, "COM6", 9600);
}

void draw(){
  update();
  
  // Scene shift
  background(127);
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
  serial_read();
}

void add_point(float distance){
  points.add(new PVector(distance * cos(-rotation) - position.x, distance * sin(-rotation) - position.y));
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

void move(boolean forward){
  if(forward){
    position = new PVector(position.x - cos(rotation) * SPEED, position.y + sin(rotation) * SPEED);
  }else{
    position = new PVector(position.x + cos(rotation) * SPEED, position.y - sin(rotation) * SPEED);
  }
}

void keyPressed(){
  serial.write(key);
}
