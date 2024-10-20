#define Left_IR A0
#define Right_IR A1
#define Left_Motor 3
#define Right_Motor 5
#define Trig 6
#define Echo 7
#define Pressure_Switch 8
#define Delivery 9

#define MAX_MOVES 100
char moves[MAX_MOVES];
int moveCount = 0;

void setup() {
  pinMode(Left_IR, INPUT);
  pinMode(Right_IR, INPUT);
  pinMode(Left_Motor, OUTPUT);
  pinMode(Right_Motor, OUTPUT);
  pinMode(Trig, OUTPUT);
  pinMode(Echo, INPUT);
  pinMode(Pressure_Switch, INPUT);
  pinMode(Delivery, OUTPUT);
  
  Serial.begin(9600);
}

void loop() {
  int lvalue = digitalRead(Left_IR);
  int rvalue = digitalRead(Right_IR);

  if (lvalue == LOW && rvalue == LOW) {
    Forward();
  } 
  else if (lvalue == LOW && rvalue == HIGH) {
    Right();
  } 
  else if (lvalue == HIGH && rvalue == LOW) {
    Left();
  } 
  else {
    Stop();
  }

  if (digitalRead(Pressure_Switch) == HIGH) {
    Stop();
    while(digitalRead(Pressure_Switch)==HIGH){
    DeliverySystem();
    if (isBoxEmpty()) {
    Return();
  }
    }
  }
}

void Forward() {
  digitalWrite(Left_Motor, HIGH);
  digitalWrite(Right_Motor, HIGH);
  moves[moveCount++] = 'F';
}

void Right() {
  digitalWrite(Left_Motor, HIGH);
  digitalWrite(Right_Motor, LOW);
  moves[moveCount++] = 'R';
}

void Left() {
  digitalWrite(Left_Motor, LOW);
  digitalWrite(Right_Motor, HIGH);
  moves[moveCount++] = 'L';
}

void Stop() {
  digitalWrite(Left_Motor, LOW);
  digitalWrite(Right_Motor, LOW);
}

// Delivery System
void DeliverySystem() {
  digitalWrite(Delivery, HIGH);
  delay(5000); 
  digitalWrite(Delivery, LOW);
}

bool isBoxEmpty() {
  long distance = getUltrasonicDistance();

  Serial.print("Distance: ");
  Serial.println(distance);

  if (distance < 10) {
    return true;
  }
  return false;
}

void Return() {
    Right();
    delay(3000);
    Stop(); 
    //To rotate the robot

    for (int i = moveCount - 1; i >= 0; i--) {
        if (moves[i] == 'F') {
            Forward();
            delay(3500);
            Stop();
            delay(100);
        } else if (moves[i] == 'R') {
            Left();
        } else if (moves[i] == 'L') {
            Right();
        }
    }
    Stop();
}


long getUltrasonicDistance() {
  digitalWrite(Trig, LOW);
  delayMicroseconds(2);
  digitalWrite(Trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(Trig, LOW);
  
  long duration = pulseIn(Echo, HIGH);
  return (duration * 0.034) / 2;
}