#define STROBE (0)
#define DATA (1)
#define CLOCK (2)
#define OE (3)

void setup() {
  pinMode(STROBE, OUTPUT);
  pinMode(DATA, OUTPUT);
  pinMode(CLOCK, OUTPUT);
  pinMode(OE, OUTPUT);
  digitalWrite(OE, HIGH);
  setRegisters(0);

  Serial.begin(9600);
  while (!Serial) {}
}

void sendBit(uint8_t val) {
  digitalWrite(CLOCK, LOW);
  digitalWrite(DATA, val);
  digitalWrite(CLOCK, HIGH);
}

void setRegisters(int r1) {
  digitalWrite(STROBE, LOW);
  for (int i = 0; i < 16; i++) {
    sendBit(r1 & 0x8000 ? HIGH : LOW);
    r1 = r1 << 1;
  }
  digitalWrite(STROBE, HIGH);
}

void setMotor(int r1) {
  if (r1 == 0) {
    setRegisters(0);
  } else {
    r1--;
    setRegisters(1 << r1);
  }
}

void testMotors() {
  static int i = 0;
  setMotor(i + 1);
  i = (i + 1) % 16;
  delay(1000);
}

void loop() {

  if (Serial.available()) {
    int c = Serial.read();
    if (c >= '0' && c <= '9') {
      setMotor(c - '0' + 1);
      delay(1000);
      setMotor(0);
    } else if (c >= 'a' && c <= 'f') {
      setMotor(c - 'a' + 11);
      delay(1000);
      setMotor(0);
    } else if (c == 't') {
      testMotors();
    }
  }

}

