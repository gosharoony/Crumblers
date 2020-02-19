//* 
//* Original code by Jon Froehlich
//* @jonfroehlich
//* http://makeabilitylab.io
//* 
//* Code modified for Team 007 | Crumblers
//* Emily Campbell, Jacob Burke, Jeff Rosen
//* HCID 521 Prototyping Studio
//* WINTER 2020
//* 
//* References
// *  - https://www.arduino.cc/en/Reference.MouseKeyboard
//*  - https://www.arduino.cc/reference/en/language/functions/usb/keyboard/keyboardpress/
//*  - https://www.arduino.cc/en/Reference/KeyboardModifiers
//* 

#include <Keyboard.h> // https://www.arduino.cc/reference/en/language/functions/usb/keyboard/
#include <Mouse.h> // https://www.arduino.cc/reference/en/language/functions/usb/mouse/

//variables

const int BUTTON_SPACEBAR_PIN = 6;
const int BUTTON_KEYBOARD_A = 7;

const int JOYSTICK_UPDOWN_PIN = A0;
const int JOYSTICK_LEFTRIGHT_PIN = A1;

// The joysticks orientation with respect to the user
// We need this because sometimes we have to place a joystick
// upside down, etc. in our designs

enum JoystickYDirection {
  UP,
  RIGHT,
  DOWN,
  LEFT
};

enum JoystickYDirection joystickYDir = RIGHT;

const int MAX_ANALOG_VAL = 1023;
const int JOYSTICK_CENTER_VALUE = int(MAX_ANALOG_VAL / 2);

// Sets the overall amount of movement in either X or Y direction
// that will trigger the Arduino board *sending* a Mouse.move()
// command to the computer

// const int JOYSTICK_MOVEMENT_THRESHOLD = 12; 

// Sets the overall mouse sensitivity based on joystick movement
// a higher value will move the mouse more with joystick movement

const int MAX_MOUSE_MOVE_VAL = 30; 

// boolean isMouseActive = true;
boolean isSpaceBarPressed = false;
boolean isUpKeyPressed = false;
boolean isRightKeyPressed = false;
boolean isDownKeyPressed = false;
boolean isLeftKeyPressed = false;
boolean isBUTTON_KEYBOARD_A = false;
// int prevButtonMouseToggleVal = HIGH;


void setup() {
  // put your setup code here, to run once:

  pinMode(BUTTON_SPACEBAR_PIN, INPUT_PULLUP);
  pinMode(BUTTON_KEYBOARD_A, INPUT_PULLUP);

  // Turn on serial for debugging
  Serial.begin(9600); 
  Keyboard.begin();
  // activateMouse(true);

}

/* void activateMouse(boolean turnMouseOn){
  if(turnMouseOn){
    Serial.println("*** Activating mouse! ***"); 
    Mouse.begin();
  }else{
    Serial.println("*** Deactivating mouse! ***");
    Mouse.end();
  }

  isMouseActive = turnMouseOn;
} */

void loop() {
  // put your main code here, to run repeatedly:

  
  int numButtonsPressed = 0;

  int buttonSpaceBarVal = digitalRead(BUTTON_SPACEBAR_PIN);
  int buttonKeyboardAVal = digitalRead(BUTTON_KEYBOARD_A);

/*    if(buttonMouseToggleVal != prevButtonMouseToggleVal){
    if(buttonMouseToggleVal == LOW && isMouseActive == false){
      activateMouse(true);
    }else if(buttonMouseToggleVal == LOW && isMouseActive == true){
      activateMouse(false);
    }
  } */

  /** HANDLE JOYSTICK INPUT AS KEYBOARD **/
  int joystickUpDownVal = analogRead(JOYSTICK_UPDOWN_PIN);
  int joystickLeftRightVal = analogRead(JOYSTICK_LEFTRIGHT_PIN);

  // If hooked up on the breadboard, we often have to 
  // install the joystick in a different orientation than
  // with the Y up direction facing up. The code below
  // handles the different orientations
  
  if(joystickYDir == RIGHT){
    int tmpX = joystickLeftRightVal;
    joystickLeftRightVal = joystickUpDownVal;
    joystickUpDownVal = MAX_ANALOG_VAL - tmpX;
  }else if(joystickYDir == DOWN){
    joystickUpDownVal = MAX_ANALOG_VAL - joystickUpDownVal;
    joystickLeftRightVal = MAX_ANALOG_VAL - joystickLeftRightVal;
  }else if(joystickYDir == LEFT){
    int tmpX = joystickLeftRightVal;
    joystickLeftRightVal = MAX_ANALOG_VAL - joystickUpDownVal;
    joystickUpDownVal = tmpX;
  }

  Serial.print("joystickLeftRightVal: ");
  Serial.print(joystickLeftRightVal);
  Serial.print(" joystickUpDownVal: ");
  Serial.println(joystickUpDownVal);

  int yDistFromCenter = joystickUpDownVal - JOYSTICK_CENTER_VALUE;
  int xDistFromCenter = joystickLeftRightVal - JOYSTICK_CENTER_VALUE;
  
  Serial.print("xDistFromCenter: ");
  Serial.print(xDistFromCenter);    
  Serial.print(" yDistFromCenter: ");
  Serial.println(yDistFromCenter);

  /* int yMouse = 0, xMouse = 0;
  if(abs(yDistFromCenter) > JOYSTICK_MOVEMENT_THRESHOLD){
    yMouse = map(joystickUpDownVal, 0, MAX_ANALOG_VAL, MAX_MOUSE_MOVE_VAL, -MAX_MOUSE_MOVE_VAL);
  }
  
  if(abs(xDistFromCenter) > JOYSTICK_MOVEMENT_THRESHOLD){
    xMouse = map(joystickLeftRightVal, 0, MAX_ANALOG_VAL, -MAX_MOUSE_MOVE_VAL, MAX_MOUSE_MOVE_VAL);
  }

  if(isMouseActive){
    Mouse.move(xMouse, yMouse, 0);
  }     */   

//JOYSTICK CASES

  if(joystickUpDownVal > 550){
    isUpKeyPressed = true;
    Keyboard.press(KEY_UP_ARROW);
    Serial.print("UP: Pressed\t");
    numButtonsPressed++;
  }else if(isUpKeyPressed == true && joystickUpDownVal < 550){
    Keyboard.release(KEY_UP_ARROW);
    isUpKeyPressed = false;
  }

  if(joystickLeftRightVal > 550){
    isRightKeyPressed = true;
    Keyboard.press(KEY_RIGHT_ARROW);
    Serial.print("RIGHT: Pressed\t");
    numButtonsPressed++;
  }else if(isRightKeyPressed == true && joystickLeftRightVal < 550){
    Keyboard.release(KEY_RIGHT_ARROW);
    isRightKeyPressed = false;
  }

  if(joystickUpDownVal < 450){
    isDownKeyPressed = true;
    Keyboard.press(KEY_DOWN_ARROW);
    Serial.print("DOWN: Pressed\t");
    numButtonsPressed++;
  }else if(isDownKeyPressed == true && joystickUpDownVal > 450){
    Keyboard.release(KEY_DOWN_ARROW);
    isDownKeyPressed = false;
  }

  if(joystickLeftRightVal < 450){
    isLeftKeyPressed = true;
    Keyboard.press(KEY_LEFT_ARROW);
    Serial.print("LEFT: Pressed");
    numButtonsPressed++;
  }else if(isLeftKeyPressed == true && joystickLeftRightVal > 450){
    Keyboard.release(KEY_LEFT_ARROW);
    isLeftKeyPressed = false;
  }



  /** HANDLE BUTTON INPUT AS KEYBOARD **/

 // SPACEBAR
  

  if(buttonSpaceBarVal == LOW){
    isSpaceBarPressed = true;
    Keyboard.press(' ');
    Serial.print("SPACE BAR: Pressed\t");
    numButtonsPressed++;
  }else if(isSpaceBarPressed == true && buttonSpaceBarVal == HIGH){
    Keyboard.release(' ');
    isSpaceBarPressed = false;
  }

// KEYBOARD A

  if(buttonKeyboardAVal == LOW){
    isBUTTON_KEYBOARD_A = true;
    Keyboard.press('a');
    Serial.print("A: Pressed\t");
    numButtonsPressed++;
  }else if(isBUTTON_KEYBOARD_A == true && buttonKeyboardAVal == HIGH){
    Keyboard.release('a');
    isBUTTON_KEYBOARD_A = false;
  }


  // Delete this line later
  delay(50);

  if(numButtonsPressed > 0){
    Serial.println();
  }
// prevButtonMouseToggleVal = buttonMouseToggleVal;

}
