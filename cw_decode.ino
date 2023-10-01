#include <LiquidCrystal.h>

LiquidCrystal lcd(12, 11, 5, 4, 3, 2); // Initialize the LCD screen

const int soundPin = A0; // Analog pin for sound input
const int threshold = 300; // Threshold for detecting sound
const int dotDuration = 100; // Duration of a dot in milliseconds
const int dashDuration = dotDuration * 3; // Duration of a dash (3 times the dot duration)
const int filterFrequency = 700; // Frequency to filter and accept (in Hz)
const int filterBandwidth = 20; // Bandwidth of the filter (in Hz)

String morseCode = ""; // Variable to store Morse code sequence

void setup() {
  lcd.begin(16, 2); // Set the LCD screen to 16x2 characters
  lcd.print("Morse Decoder"); // Print the initial message on the LCD screen
  lcd.setCursor(0, 1);
  lcd.print("Listening...");
}

void loop() {
  int soundLevel = analogRead(soundPin); // Read the sound level from the analog pin
  
  if (soundLevel > threshold) { // If sound level is above the threshold
    delay(50); // Wait for 50 milliseconds to debounce
    soundLevel = analogRead(soundPin); // Read the sound level again
    
    if (soundLevel > threshold) { // If sound level is still above the threshold
      if (isFrequencyInRange(filterFrequency - filterBandwidth, filterFrequency + filterBandwidth)) {
        decodeMorse(); // Decode the Morse code
      }
    }
  }
}

bool isFrequencyInRange(int lowerBound, int upperBound) {
  int frequency = getSoundFrequency();
  
  if (frequency >= lowerBound && frequency <= upperBound) {
    return true;
  }
  
  return false;
}

int getSoundFrequency() {
  int soundLevel = analogRead(soundPin); // Read the sound level from the analog pin
  float voltage = soundLevel * (5.0 / 1023.0); // Convert the sound level to voltage
  float frequency = voltage * 1000.0; // Convert the voltage to frequency
  
  return frequency;
}

void decodeMorse() {
  int dotCount = 0; // Variable to count the number of dots
  int dashCount = 0; // Variable to count the number of dashes
  bool isDot = false; // Flag to indicate if a dot is being received
  
  while (true) {
    int soundLevel = analogRead(soundPin); // Read the sound level from the analog pin
    
    if (soundLevel > threshold) { // If sound level is above the threshold
      delay(50); // Wait for 50 milliseconds to debounce
      soundLevel = analogRead(soundPin); // Read the sound level again
      
      if (soundLevel > threshold) { // If sound level is still above the threshold
        if (!isDot) {
          dotCount++;
          isDot = true;
        }
        else {
          dashCount++;
          isDot = false;
        }
      }
    }
    
    if (dotCount > 0 && millis() - dotDuration * dotCount > dashDuration) {
      // If there's a gap longer than a dash, interpret the Morse code
      interpretMorse(dotCount, dashCount);
      dotCount = 0;
      dashCount = 0;
    }
  }
}

void interpretMorse(int dotCount, int dashCount) {
  char morseChar;
  
  if (dotCount == 1 && dashCount == 0) {
    morseChar = '.';
  }
  else if (dotCount == 3 && dashCount == 0) {
    morseChar = '-';
  }
  else if (dotCount == 0 && dashCount == 0) {
    morseChar = ' ';
  }
  
  morseCode += morseChar; // Add the Morse character to the Morse code sequence
  
  if (morseChar == ' ') { // If a space is received, decode the Morse code sequence
    String decodedText = decodeMorseCode(morseCode);
    displayText(decodedText);
    morseCode = ""; // Reset the Morse code sequence
  }
}

String decodeMorseCode(String morseCode) {
  // Add more Morse code characters if needed
  if (morseCode == ".-") return "A";
  else if (morseCode == "-...") return "B";
  else if (morseCode == "-.-.") return "C";
  else if (morseCode == "-..") return "D";
  else if (morseCode == ".") return "E";
  else if (morseCode == "..-.") return "F";
  else if (morseCode == "--.") return "G";
  else if (morseCode == "....") return "H";
  else if (morseCode == "..") return "I";
  else if (morseCode == ".---") return "J";
  else if (morseCode == "-.-") return "K";
  else if (morseCode == ".-..") return "L";
  else if (morseCode == "--") return "M";
  else if (morseCode == "-.") return "N";
  else if (morseCode == "---") return "O";
  else if (morseCode == ".--.") return "P";
  else if (morseCode == "--.-") return "Q";
  else if (morseCode == ".-.") return "R";
  else if (morseCode == "...") return "S";
  else if (morseCode == "-") return "T";
  else if (morseCode == "..-") return "U";
  else if (morseCode == "...-") return "V";
  else if (morseCode == ".--") return "W";
  else if (morseCode == "-..-") return "X";
  else if (morseCode == "-.--") return "Y";
  else if (morseCode == "--..") return "Z";
  else return ""; // Return an empty string if Morse code is not recognized
}

void displayText(String text) {
  lcd.clear(); // Clear the LCD screen
  lcd.print("Decoded Text:");
  lcd.setCursor(0, 1);
  lcd.print(text);
}
