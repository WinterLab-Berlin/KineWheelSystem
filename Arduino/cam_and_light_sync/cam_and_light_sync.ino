/**
 * Arduino UNO R3 (Nano) script to start and stop recordings when the system is operated in *Basic Mode* (inference only).
 * A *recording* is defined as the process of triggering the image capture of a connected Basler camera and switching white light LEDs.
 * 
 * This script bi-directionally communicates with a client program connected on a serial port.
 * 
 * A client can send
 *   - a "start recording",
 *   - a "stop recording", and
 *   - a "camera trigger rate"
 * message.
 * 
 * This script can send
 *   - "OK" messages to confirm commands sent by a client and
 *   - "keep alive" messages, while a recording is running.
 * 
 * 
 * High-level operation of this script:
 * 
 * (1) On startup of the Arduino, this script
 *   - turns off all lights,
 *   - disables the camera trigger signal,
 *   - sets the baudrate (as defined by the constant below), and then
 *   - enters the main loop.
 *   
 * (2) After startup (i.e., in the main loop), this script
 *   - waits for a client to start a recording.
 *   
 * (3) When recording, this script
 *   - turns on all white light LEDs (UV LEDs stay off),
 *   - triggers the camera periodically to take an image, and
 *   - checks every N iterations whether the client stopped the recording, in which case
 *      - all white LEDs are turned off,
 *      - the camera stops recording, and
 *      - code execution returns to step (2).
 *
 * Client implementation notes:
 *  - The baudrate used in the client program must match that defined in this script.
 *  - The "recording start" sequence consists of three bytes:
 *    - The first byte should be set to zero. This ensures that the state of this script
 *      will be reset to the beginning of the *main loop* whenever a new client ... TODO.
 * 
 * ## TODO
 * 
 * - Idea:
 * Allow client to send fps, b/c in *inference mode*, a high frame rate and thus delay between
 * subsequent frames is not required, only a short exposure time to keep motion blur at a minimum.
 * (FPS range should be limited to match specs of cam. E.g., 30 -- 720 FPS)
 * 
 *
 * - Test script!
 *
 * Date: Nov 11, 2022
 */

const unsigned long baudrate = 115200;   // Must match baudrate in Arduino Control App on PC

// Assignment of Arduino digital I/O pins to LED modules and Basler cam I/O connector
const int uvWheelLedsPin = 12;      // Pin to control 8x 3W UV LED, located below the wheel surface
const int uvFrontTopLedsPin = 11;   // Pin to control 5x 3W UV LED, located in front-top LED housing
const int whiteFrontTopLedPin = 10; // Pin to control 10x 1W white light LED, located in front-top LED housing
const int whiteFrontBotLedPin = 9;  // Pin to control 1x 50W white light LED, located in front-bottom LED housing
//const int baslerGpioInPin = 8;      // Pin to trigger Basler cam (GPIO In on Line 3)
const int baslerGpioInPin = 13;     // Pin to trigger Basler cam (GPIO In on Line 3)  // TODO: Switch back to pin 8

/**
 * Processing delay of Arduino recording WHILE-loop in microseconds.
 * Value determined by checking frame rate in Pylon Viewer GUI and adjusting delay 
 * value accordingly.  This delay will be subtracted from the delay between 
 * subsequent frames, so that the desired frame rate is achieved and not lowered.
 */
const unsigned long recLoopDelay = 53;  // 53 us allows for a maximum (stable) trigger rate of 720 Hz (fix FPS range in client app)
const int triggerSignalLen = 10;    // Length of HW-trigger signal (in us) sent to the Basler cam
const int stopCheckFrameCount = 3;  // Num of frames between checking if user stopped recording

// Operation codes (byte values) used for communication between Arduino and client
const byte msgStopRec = 0;  // Client request code to stop recording WHILE-loop
const byte msgStartRec = 1; // Client request code to start recording
const byte msgSendFps = 2;  // Arduino signal for client to send FPS
const byte msgWaiting = 3;  // Arduino signal for client to start recording

/*
 * Remove BEGIN
 */
//const byte msgRecording set this to a byte? and then sent it periodically while recording as keep-alive msg to client
// Magic byte sequence sent by client to start a recording. Must match values in Arduino Control App on PC.
//const byte msgStartByte1 = 233;
//const byte msgStartByte2 = 222;
/*
 * Remove END
 */

// The setup function runs once when the board is powered on or reset
void setup() {
  // Initialize serial communication at `baudrate` bits per second
  Serial.begin(baudrate);
  // Set the max. number of ms to wait for serial data.
  Serial.setTimeout(500);

  // Initialize digital pins as output.
  pinMode(uvWheelLedsPin, OUTPUT);
  pinMode(uvFrontTopLedsPin, OUTPUT);
  pinMode(whiteFrontTopLedPin, OUTPUT);
  pinMode(whiteFrontBotLedPin, OUTPUT);
  pinMode(baslerGpioInPin, OUTPUT);

  // Initialize LEDs and Basler HW-trigger pins to low (i.e., lights and cam trigger off).
  digitalWrite(uvWheelLedsPin, LOW);
  digitalWrite(uvFrontTopLedsPin, LOW);
  digitalWrite(whiteFrontTopLedPin, LOW);
  digitalWrite(whiteFrontBotLedPin, LOW);
  digitalWrite(baslerGpioInPin, LOW);
}


// The loop function runs over and over again, forever
void loop() {

  // Signal that Arduino is ready to start recording.
  Serial.write(msgWaiting);
  
  // DEBUG
  //Serial.print("Waiting");

  // Wait for at least 1 byte in the serial receive buffer.
  while (Serial.available() < 1);

  byte buf[2] = { 0 };  // Clear buffer
  Serial.readBytes(buf, 1);  // Read one byte from serial rcv buffer
  
  // Check if client sent msg to start recording.
  if (buf[0] != msgStartRec) {
    return;  // Start *main loop* from top again.
  }

  // Ask client for desired frame rate. (Send byte value.)
  Serial.write(msgSendFps);

  // Read frame rate from client
  memset(buf, 0, 2);  // Clear buffer
  size_t bytesRead = Serial.readBytes(buf, 2);  // Read two bytes from serial rcv buffer, w/ timeout
  byte hi = buf[0];
  byte lo = buf[1];
  word frameRate = word(hi, lo);

  // DEBUG: Remvoe me later
  //Serial.print(frameRate);
  
  // If frame rate is 0 or read timed out, abort and jump back to top of main loop.
  if (bytesRead != 2 || frameRate == 0) {
    return;
  }

  // Turn all white light LEDs on.
  digitalWrite(whiteFrontTopLedPin, HIGH);
  digitalWrite(whiteFrontBotLedPin, HIGH);

  bool isRecording = true;
  // NOTE: An overflow of the frame counter can be ignored, as values up to 2^32 - 1
  // can be stored, allowing run times of more than 1.000 h whren recording at 1.000 Hz.
  unsigned long frameCounter = 0;

  // NOTE: For `delayMicroseconds()`, currently, the largest value that will produce an accurate
  // delay is 16383; larger values can produce an extremely short delay. Therefore, split delay
  // into milliseconds and microseconds.
  unsigned long frameDelayUs = 1000000 / frameRate; // Delay between subsequent frames in microseconds
      // Subtract the duration of the camera trigger signal and WHILE-loop
      // iteration from the inter-frame delay to ensure the target frame rate
      // is met.
  frameDelayUs =  frameDelayUs - triggerSignalLen - recLoopDelay;
  //Serial.print('\n');
  //Serial.print(frameDelayUs);  // DEBUG: Remove
  unsigned long frameDelayMs = frameDelayUs / 1000; // Delay between subsequent frames in milliseconds
  //Serial.print('\n');          // DEBUG: Remove
  //Serial.print(frameDelayMs);  // DEBUG: Remove
  frameDelayUs =  frameDelayUs - frameDelayMs * 1000; // Delay between subsequent frames in microseconds
  //Serial.print('\n');          // DEBUG: Remove
  //Serial.print(frameDelayMs);  // DEBUG: Remove
  //Serial.print('\n');          // DEBUG: Remove
  //Serial.print(frameDelayUs);  // DEBUG: Remove
  //Serial.print('\n');          // DEBUG: Remove
  Serial.setTimeout(1);  // Set short timeout to not delay camera trigger, when checking for client msgs.

  // Recording loop switches lights and triggers camera until client stops recording
  while (isRecording) {
    // Trigger camera
    digitalWrite(baslerGpioInPin, HIGH);
    // Keep the HW-trigger signal sent to the Basler cam high for at
    // least `triggerSignalLen` microseconds to be registered by the cam.
    delayMicroseconds(triggerSignalLen);
    digitalWrite(baslerGpioInPin, LOW);

    delay(frameDelayMs);
    delayMicroseconds(frameDelayUs);

    if (frameCounter % stopCheckFrameCount) {
      // Every n-th frame, check whether client requested recording stop.
      if (Serial.available() > 0) {
        // If client has sent 0-byte, recording will be stopped.
        isRecording = (bool)Serial.read();
      }
    }
    
    ++frameCounter;
  }

  // Turn all white light LEDs off.
  digitalWrite(whiteFrontTopLedPin, LOW);
  digitalWrite(whiteFrontBotLedPin, LOW);

  // DEBUG
  //Serial.print("Stopped");

  Serial.setTimeout(500);  // Set longer timeout for when waiting for client and FPS value.

  // XXX: Remove lines below: don't report successful recording stop to client,
  // instead, on the client side, assume that recording got stopped. This
  // simplifies the logic and code on Arduino and client.
  // Report to client that recording has been stopped.
  //Serial.print(msgOk);
  
  return;  // Start *main loop* from top again.
 
}
