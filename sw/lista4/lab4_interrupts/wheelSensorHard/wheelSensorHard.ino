Wheels robot;

void setup() {
    robot.attach(5, 6, 9, 10, 11, 3); // Twoje piny silników
    robot.attachEncoders(A0, A1);     // Piny, pod które podpięto czujniki (lewy, prawy)
}

void loop() {
  robot.goForward(30);
  delay(200);
  robot.turnRight(90);
  delay(200);
  robot.goForward(30);
  delay(200);
  robot.turnRight(90);
  delay(200);
  robot.goForward(30);
  delay(200);
  robot.turnRight(90);
  delay(200);
  robot.goForward(30);
  delay(200);
  robot.turnRight(90);
  delay(200);
}