String t[100];

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(9600);
  t[65] = ".-";
  t[66] = "-...";
  t[67] = "-.-.";
  t[68] = "-..";
  t[69] = ".";
  t[70] = "..-.";
  t[71] = "--.";
  t[72] = "....";
  t[73] = "..";
  t[74] = ".---";
  t[75] = "-.-";
  t[76] = ".-..";
  t[77] = "--";
  t[78] = "-.";
  t[79] = "---";
  t[80] = ".--.";
  t[81] = "--.-";
  t[82] = ".-.";
  t[83] = "...";
  t[84] = "-";
  t[85] = "..-";
  t[86] = "...-";
  t[87] = ".--";
  t[88] = "-..-";
  t[89] = "-.--";
  t[90] = "--..";
}

void loop() {

  while(Serial.available()) {
    char data = Serial.read();
    if(data == ' ') {
      delay(500);
    }
    for(int i = 0; i < t[data].length(); i++) {
      digitalWrite(LED_BUILTIN, HIGH);
      if(t[data][i] == '.') {
        delay(300);
      } else if(t[data][i] == '-') {
        delay(900);
      } else {
        Serial.println("unknown char");
      }
      digitalWrite(LED_BUILTIN, LOW);
      delay(300);
    }
  }

}
