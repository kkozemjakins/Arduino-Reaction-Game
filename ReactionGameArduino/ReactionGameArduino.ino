// You must install the 'lcd.h' library provided by Keyestudio for this display.
#include <lcd.h>
lcd Lcd;

// --- PIN DEFINITIONS ---
// Traffic Light LEDs
const int RED_LED_PIN = 12;
const int YELLOW_LED_PIN = 11;
const int GREEN_LED_PIN = 10;

// Player 1
const int PLAYER1_BUTTON_PIN = 2;
const int PLAYER1_LED_PIN = 5;

// Player 2
const int PLAYER2_BUTTON_PIN = 3;
const int PLAYER2_LED_PIN = 6;

// Game Control
const int BUZZER_PIN = 9;
const int START_BUTTON_PIN = 4;

// --- GAME STATE VARIABLES ---
enum GameState {
  WAITING,
  COUNTDOWN,
  GO,
  FINISHED
};

GameState currentGameState = WAITING;

// Scorekeeping
int player1Score = 0;
int player2Score = 0;

// Reaction times for last round
float reactionTimeP1 = 0.0;
float reactionTimeP2 = 0.0;

float goSignalTime = 0.0;

// Flags to check if players have pressed
bool p1Pressed = false;
bool p2Pressed = false;




// Timing variables for non-blocking delays
unsigned long startTime = 0;
const long COUNTDOWN_DELAY = 1000; // 1 second for each light
const int WINNER_LED_ON_TIME = 2000; // 2 seconds


// Frequencies for notes (simple scale)
#define NOTE_C4  262
#define NOTE_D4  294
#define NOTE_E4  330
#define NOTE_F4  349
#define NOTE_G4  392
#define NOTE_A4  440
#define NOTE_B4  494
#define NOTE_C5  523

// Melody for Player 1 (short rising tune)
int melodyP1[] = { NOTE_C4, NOTE_E4, NOTE_G4, NOTE_C5 };
int durationP1[] = { 200, 200, 200, 400 }; // ms per note

// Melody for Player 2 (short falling tune)
int melodyP2[] = { NOTE_C5, NOTE_G4, NOTE_E4, NOTE_C4 };
int durationP2[] = { 200, 200, 200, 400 }; // ms per note


// --- SETUP FUNCTION ---
void setup() {
  Serial.begin(9600);

  // Initialize LEDs and Buzzer pins as OUTPUTs
  pinMode(RED_LED_PIN, OUTPUT);
  pinMode(YELLOW_LED_PIN, OUTPUT);
  pinMode(GREEN_LED_PIN, OUTPUT);
  pinMode(PLAYER1_LED_PIN, OUTPUT);
  pinMode(PLAYER2_LED_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);

  // Initialize button pins as INPUT_PULLUP
  // This means the pin is HIGH by default and goes LOW when the button is pressed
  pinMode(PLAYER1_BUTTON_PIN, INPUT_PULLUP);
  pinMode(PLAYER2_BUTTON_PIN, INPUT_PULLUP);
  pinMode(START_BUTTON_PIN, INPUT_PULLUP);

  // Initialize the LCD display
  Lcd.Init();
  Lcd.Clear();
  Lcd.Cursor(1, 1);
  Lcd.Display("Reaction Game!");
  delay(2000);
  displayScores();
}

// --- MAIN LOOP ---
void loop() {
  // Check if the start button is pressed in the WAITING or FINISHED state
  if (currentGameState == WAITING || currentGameState == FINISHED) {
    if (digitalRead(START_BUTTON_PIN) == LOW) {
      // Small debounce delay
      delay(50);
      if (digitalRead(START_BUTTON_PIN) == LOW) {
        resetGame();
        currentGameState = COUNTDOWN;
        startTime = millis();
      }
    }
  }

  // State machine logic
  switch (currentGameState) {
    case COUNTDOWN:
      handleCountdown();
      break;

    case GO:
      handleGo();
      break;

    case FINISHED:
      // The winner's LED stays on for a brief period
      break;
      
    default:
      // WAITING state is handled by the start button check above
      break;
  }
}

// --- HELPER FUNCTIONS ---

void resetGame() {
  // Reset LEDs and buzzer
  digitalWrite(RED_LED_PIN, LOW);
  digitalWrite(YELLOW_LED_PIN, LOW);
  digitalWrite(GREEN_LED_PIN, LOW);
  digitalWrite(PLAYER1_LED_PIN, LOW);
  digitalWrite(PLAYER2_LED_PIN, LOW);
  noTone(BUZZER_PIN);

  // Reset reaction times & flags
  reactionTimeP1 = 0;
  reactionTimeP2 = 0;
  p1Pressed = false;
  p2Pressed = false;

  displayScores();
}


void handleCountdown() {
  unsigned long currentTime = millis();
  
  // 3 - Red Light
  if (currentTime - startTime < COUNTDOWN_DELAY) {
    digitalWrite(RED_LED_PIN, HIGH);
  }
  // 2 - Yellow Light
  else if (currentTime - startTime < COUNTDOWN_DELAY * 2) {
    digitalWrite(RED_LED_PIN, LOW);
    digitalWrite(YELLOW_LED_PIN, HIGH);
  }
  // 1 - Green Light
  else if (currentTime - startTime < COUNTDOWN_DELAY * 3) {
    digitalWrite(YELLOW_LED_PIN, LOW);
    digitalWrite(GREEN_LED_PIN, HIGH);
    tone(BUZZER_PIN, 1000); // Play a tone


    goSignalTime = millis();  // ✅ mark the "GO" moment
    
    // Transition to the GO state
    currentGameState = GO;
  }
}

void handleGo() {
  static int firstPlayer = 0; // 0 = none, 1 = P1, 2 = P2

  // Player 1
  if (!p1Pressed && digitalRead(PLAYER1_BUTTON_PIN) == LOW) {
    reactionTimeP1 = (millis() - goSignalTime) / 1000.0; // convert to seconds
    p1Pressed = true;
    if (firstPlayer == 0) {
      firstPlayer = 1; // P1 is the winner
    }
  }

  // Player 2
  if (!p2Pressed && digitalRead(PLAYER2_BUTTON_PIN) == LOW) {
    reactionTimeP2 = (millis() - goSignalTime) / 1000.0; // convert to seconds
    p2Pressed = true;
    if (firstPlayer == 0) {
      firstPlayer = 2; // P2 is the winner
    }
  }

  // If both players have pressed, declare the winner and finish the round
  if (p1Pressed && p2Pressed) {
    declareWinner(firstPlayer);
    
    // Reset for next round
    p1Pressed = false;
    p2Pressed = false;
    firstPlayer = 0;
  }
}





void playMelody(int melody[], int duration[], int length) {
  for (int i = 0; i < length; i++) {
    tone(BUZZER_PIN, melody[i], duration[i]);
    delay(duration[i] * 1.3);  // small gap between notes
  }
  noTone(BUZZER_PIN);
}



void declareWinner(int winner) {
  char* Pwinner;

  noTone(BUZZER_PIN);
  digitalWrite(GREEN_LED_PIN, LOW);

  if (winner == 1) {
    player1Score++;
    Pwinner = "P1";
    digitalWrite(PLAYER1_LED_PIN, HIGH);
    playMelody(melodyP1, durationP1, 4);  // 🎵 Player 1 melody
  } else {
    player2Score++;
    Pwinner = "P2";
    digitalWrite(PLAYER2_LED_PIN, HIGH);
    playMelody(melodyP2, durationP2, 4);  // 🎵 Player 2 melody
  }

  Lcd.Clear();
  Lcd.Cursor(0, 4);
  Lcd.Display("Player ");
  Lcd.Cursor(1, 4);
  Lcd.Display(Pwinner);
  Lcd.Cursor(2, 4);
  Lcd.Display("Wins!");

  delay(WINNER_LED_ON_TIME);
  digitalWrite(PLAYER1_LED_PIN, LOW);
  digitalWrite(PLAYER2_LED_PIN, LOW);

  displayScores();
  currentGameState = FINISHED;  // Now we end the round here
}





void displayScores() {
  char buffer[20];

  Lcd.Clear();

  // Convert reaction times to integer and milliseconds
  int secP1 = (int)reactionTimeP1;
  int msP1 = (int)((reactionTimeP1 - secP1) * 1000);

  int secP2 = (int)reactionTimeP2;
  int msP2 = (int)((reactionTimeP2 - secP2) * 1000);

  // Player 1
  Lcd.Cursor(0, 1);
  sprintf(buffer, "P1:%d RT:%d.%03d", player1Score, secP1, msP1);
  Lcd.Display(buffer);

  // Player 2
  Lcd.Cursor(1, 1);
  sprintf(buffer, "P2:%d RT:%d.%03d", player2Score, secP2, msP2);
  Lcd.Display(buffer);
}


