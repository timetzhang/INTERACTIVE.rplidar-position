import processing.serial.*;
import java.util.Iterator;

ArrayList<Particle> plist = new ArrayList<Particle>();
Serial myPort;
int x, y;

void setup() {
  fullScreen(P3D);
  background(20);

  //init serial port
  String portName = Serial.list()[1];
  println(portName);
  myPort = new Serial(this, portName, 115200);
}


void draw() {
  background(20);

  if (myPort.available() > 0) {
    String[] s = myPort.readString().split(",");
    String sx, sy;
    try {     
      sx = s[0].trim();
      sy = s[1].trim();
      x = int(sx);
      y = int(sy);
      
      println("code: " + x + "," + y);
    }
    catch(Exception e) {
    }
  }

  plist.add(new Particle(new PVector(map(x, -1000, 1000, 0, width), map(y, 0, 1000, 0, height)))); //, 
  Iterator<Particle> it = plist.iterator();

  while (it.hasNext()) {
    Particle p = it.next();
    p.run();
    if (p.isDead()) {
      it.remove();
    }
  }
}
