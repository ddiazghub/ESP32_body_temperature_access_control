// Include setup header file
#include "functions.h"

// Setup.
void setup() 
{
    Serial.begin(9600);

    //Initialize I2C objects
    mlx.begin(); 
    display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  //I2C addr 0x3C.
    
    servo.attach(SERVO_PIN_SIG); 
    
    // Setup display
    initDisplay();
}

// Main
void loop() {
    // If trigger not received display room temperature continously.
    if (!Serial.available()) {
      temp_amb = mlx.readAmbientTempC();

      display.clearDisplay();
      
      printToDisplay(10, 0, 1, "Temperatura ambiente:");
      printTemperature(10, 40, 3, temp_amb);


      // If room temperature is higher than the threshold display err message.
      if (temp_amb > threshold) {
        ledR.on();
        buzzer.on();

        printToDisplay(0, 53, 1, "Error: Temperatura ambiente muy alta");
      }
      return;
    }

    byte serialInput = Serial.read();
    if (!serialInput == trigger) {
      return;
    };

    // Ask person to stand at 10 cm or less from sensor.
    display.clearDisplay();
    printToDisplay(10, 20, 1, " Posicionese a máximo ");
    printToDisplay(10, 30, 1, maxDistance + "cm del sensor.");


    // Wait
    printToDisplay(10,50, 1, "Esperando...");
    while(ultrasonic.distance() > maxDistance) {

    };

    countdown(3, 45, 50, 1);
    if (ultrasonic.distance() > maxDistance) {
      display.clearDisplay();
      printToDisplay(10, 20, 1, " No se tomó la temperatura correctamente ");
      delay(2000);
      return;
    };

    
    // Take temperature and send to RFID system.
    temp_body = mlx.readObjectTempC();
    Serial.print(temp_body);

    display.clearDisplay();
    printToDisplay(10, 0, 1, " Temperatura corporal: ");
    printTemperature(10, 40, 3, temp_body);


    // If temperature is over threshold light red LED, trigger buzzer, and print alert.
    if (temp_body > threshold) {
      ledR.on();
      buzzer.on();

      printToDisplay(0, 53, 1, "Su temperatura está muy alta");
      delay(2000);
      return;
    }

    ledG.on();
    printToDisplay(25, 53, 1, "Bienvenido");
    openDoor();
}