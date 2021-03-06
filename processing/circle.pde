class Particle {
  PVector loc;
  PVector vel;
  PVector acc;
  color fillColor;
  float radius = 10;
  float lifespan = 200;

  Particle(PVector l) {
    loc = l.get();
    acc = new PVector(random(-3, 3), random(-3, 3));
    vel = new PVector(0, 0);
    fillColor = color(random(255), random(255), random(255));
  }

  void update() {
    lifespan-=5;
    vel = new PVector(random(-2, 2), random(-2, 2));
    vel.add(acc);
    loc.add(vel);
  }

  void display() {
    noStroke();
    rectMode(RADIUS);
    for (int i=40; i>10; i--) {
      fill(fillColor, lifespan-i*5);
      ellipse(loc.x, loc.y, radius+(i-10), radius+(i-10));
    }

    fill(fillColor, lifespan);
    ellipse(loc.x, loc.y, radius, radius);
  }

  void checkEdge() {
    if (loc.y > height) {
      loc.y = 0;
      vel.y = -vel.y;
    }
    if (loc.x > width) {
      loc.x = 0;
      vel.x = -vel.x;
    }
    if (loc.y < 0) {
      loc.y = height;
      vel.y = -vel.y;
    }
    if (loc.x < 0) {
      loc.x = width;
      vel.x = -vel.x;
    }
  }

  boolean isDead() {
    if (lifespan < 0.0) {
      return true;
    } else {
      return false;
    }
  }

  void run() {
    update();
    checkEdge();
    display();
  }
}
