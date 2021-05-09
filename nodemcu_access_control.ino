// Include function definition header file which already includes the program variable definitions.
#include "functions.h"

// Main program, includes setup and main loop functions.

// Setup
void setup() 
{
    Serial.begin(9600);

    //Initialize I2C objects
    mlx.begin();
    initDisplay();
    
    servo.attach(SERVO_PIN_SIG);
    servo.write(SERVO_REST);
    servo.detach();
}

// Main
void loop() {
    ledR.off();
    ledG.off();
    display.clearDisplay();

    // If trigger not received display room temperature continously.
    if (!Serial.available()) {
      temp_amb = mlx.readAmbientTempC();
      
      printToDisplay(0, 0, 1, "Temperatura ambiente:");
      printTemperature(17, 25, 2, temp_amb);


      // If room temperature is higher than the threshold display error message.
      if (temp_amb > THRESHOLD) {
        ledR.on();

        printToDisplay(0, 48, 1, "  Error: Temperatura    ambiente muy alta");

        while (mlx.readAmbientTempC() > THRESHOLD) {
          buzzer.error();
        }
      }
      delay(300);
      return;
    }

    // Read from serial buffer, return if different from trigger char.
    byte serialInput = Serial.read();
    if (serialInput != TRIGGER) {
      return;
    };

    while (true) {
      // Ask person to stand at a distance not higher than 10 cm from sensor.
      display.clearDisplay();
      printToDisplay(0, 5, 1, " Posicionese a maximo ");
      display.setCursor(19, 15);
      display.print(10);
      display.print("cm del sensor");

      // Wait.
      printToDisplay(30, 50, 1, "Esperando...");
      while(ultrasonic.ping_cm() > MAX_DISTANCE) {
        delay(300);
      };

      // Count 3 seconds and take temperature, if person moved ouut of range notify and return.
      countdown(3, 62, 40, 1);
      if (ultrasonic.ping_cm() > MAX_DISTANCE) {
        display.clearDisplay();
        printToDisplay(25, 17, 1, "No se tomo la");
        printToDisplay(25, 27, 1, " Temperatura");
        printToDisplay(25, 37, 1, "correctamente");
        buzzer.error();
        continue;
      }
      break;
    }

    
    // Take temperature, show on display and send to RFID system.
    temp_body = mlx.readObjectTempC();
    Serial.print(temp_body);

    display.clearDisplay();
    printToDisplay(0, 0, 1, "Temperatura corporal:");
    printTemperature(17, 25, 2, temp_body);


    // If temperature is over threshold, light red LED, trigger buzzer, and print alert.
    if (temp_body > THRESHOLD) {
      ledR.on();

      printToDisplay(0, 48, 1, " Su temperatura esta       muy alta");
      buzzer.error();
      return;
    }

    // If temperature is below threshold, grant access.
    ledG.on();
    printToDisplay(32, 53, 1, "Bienvenido");
    buzzer.success();
    openDoor();
}