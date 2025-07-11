// Smart Plant Incubator 
// Author: Sara El Bari
// Monitors soil moisture, water level, temperature & humidity
// Controls pump, valve, buzzer, and status LEDs

// --- Pin Definitions ---
#define RED_LED 3
#define GREEN_LED 4
#define BUZZER 2
#define TEMP_HUMIDITY_PIN 5  // For DHT11 if used
#define WATER_PUMP_RELAY 7
#define VALVE_RELAY 6
#define SOIL_SENSOR A2
#define WATER_SENSOR A0

// --- Thresholds ---
const int SOIL_DRY_THRESHOLD = 450;
const int SOIL_WET_THRESHOLD = 300;
const int WATER_AVAILABLE_THRESHOLD = 400;
const int WATER_FULL_THRESHOLD = 500;

// --- Global Variables ---
int soilMoisture = 0;
int waterLevel = 0;

void setup() {
  pinMode(RED_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(BUZZER, OUTPUT);
  pinMode(WATER_PUMP_RELAY, OUTPUT);
  pinMode(VALVE_RELAY, OUTPUT);

  digitalWrite(GREEN_LED, HIGH);  // Assume system OKAY at start
  digitalWrite(RED_LED, LOW);
  digitalWrite(BUZZER, LOW);
  digitalWrite(WATER_PUMP_RELAY, LOW);
  digitalWrite(VALVE_RELAY, LOW);

  Serial.begin(9600);
}

void loop() {
  // Read sensor data
  soilMoisture = analogRead(SOIL_SENSOR);
  waterLevel = analogRead(WATER_SENSOR);

  Serial.print("Soil Moisture: ");
  Serial.print(soilMoisture);
  Serial.print(" | Water Level: ");
  Serial.println(waterLevel);

  // --- Soil Moisture Logic ---
  if (soilMoisture > SOIL_DRY_THRESHOLD) {
    // Soil is dry
    if (waterLevel > WATER_AVAILABLE_THRESHOLD) {
      // Water available — activate pump
      digitalWrite(WATER_PUMP_RELAY, HIGH);
      delay(5000);  // Run pump for 5 seconds
      digitalWrite(WATER_PUMP_RELAY, LOW);

      // Indicate normal operation
      digitalWrite(GREEN_LED, HIGH);
      digitalWrite(RED_LED, LOW);
      digitalWrite(BUZZER, LOW);
    } else {
      // Reservoir empty — alert user
      digitalWrite(BUZZER, HIGH);
      digitalWrite(RED_LED, HIGH);
      digitalWrite(GREEN_LED, LOW);
    }
  } else if (soilMoisture < SOIL_WET_THRESHOLD) {
    // Soil is moist enough — do nothing
    digitalWrite(WATER_PUMP_RELAY, LOW);
    digitalWrite(GREEN_LED, HIGH);
    digitalWrite(RED_LED, LOW);
    digitalWrite(BUZZER, LOW);
  }

  // --- Drain Excess Water ---
  if (waterLevel < WATER_FULL_THRESHOLD) {
    // Collection tray is full — activate valve
    digitalWrite(VALVE_RELAY, HIGH);
    delay(3000);  // Open valve for 3 seconds
    digitalWrite(VALVE_RELAY, LOW);
  }

  delay(1000);  // Wait 1 second before next loop
}
 