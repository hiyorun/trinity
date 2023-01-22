#include "Pinout.h"
#include <BleKeyboard.h>
BleKeyboard trinity("Trinity", "Hiyo", 100);

short Rows[RowCount] = {Row1, Row2 , Row3, Row4, Row5};
short Columns[ColCount] = {Col1, Col2, Col3, Col4, Col5, Col6, Col7};
short KeyState[LayerCount][RowCount][ColCount] = {0};

int Layout[LayerCount][RowCount][ColCount] =
{
  {
    {KEY_ESC        , '1'         , '2'         , '3', '4', '5', '6', '7', '8'          , '9', '0'           , '-'            , '=', KEY_BACKSPACE  , KEY_INSERT    , KEY_HOME      , KEY_PAGE_UP     },
    {KEY_TAB        , 'q'         , 'w'         , 'e', 'r', 't', 'y', 'u', 'i'          , 'o', 'p'           , '['            , ']', '\\'           , KEY_DELETE    , KEY_END       , KEY_PAGE_DOWN   },
    {KEY_CAPS_LOCK  , 'a'         , 's'         , 'd', 'f', 'g', 'h', 'j', 'k'          , 'l', ';'           , '\''           , ' ', KEY_RETURN     , ' '           , ' '           , ' '            },
    {KEY_LEFT_SHIFT , 'z'         , 'x'         , 'c', 'v', 'b', 'n', 'm', ','          , '.', '/'           , KEY_RIGHT_SHIFT, ' ', ' '            , ' '           , KEY_UP_ARROW  , ' '            },
    {KEY_LEFT_CTRL  , KEY_LEFT_GUI, KEY_LEFT_ALT, ' ', ' ', ' ', ' ', ' ', KEY_RIGHT_ALT, FN , KEY_RIGHT_CTRL, ' '            , ' ', ' '            , KEY_LEFT_ARROW, KEY_DOWN_ARROW, KEY_RIGHT_ARROW},
  },
};

int currentRow = 0;

void setup() {
  Serial.begin(9600);
  Serial.println("Booting up");
  Serial.println("Setting down all rows");
  for (int i = 0; i < RowCount; i++)
  {
    pinMode(Rows[i], OUTPUT);
    digitalWrite(Rows[i], LOW);
  }

  Serial.println("Pulling down all columns");
  for (int i = 0; i < ColCount; i++)
  {
    pinMode(Columns[i], INPUT_PULLDOWN);
  }
  pinMode(13, INPUT_PULLUP);
  Serial.println("Starting ARONA..");
  trinity.begin();
  Serial.println("Short D13 with ground to initiate reboot");
}

void loop() {
  if (digitalRead(13) == LOW) {
    Serial.println("Rebooting");
    ESP.restart();
  }
  if (trinity.isConnected()) {
    digitalWrite(Rows[currentRow], HIGH);
    int currentCol = 0;
    while (currentCol <= (ColCount - 1)) {
      if (digitalRead( Columns[currentCol]) == HIGH && KeyState[0][currentRow][currentCol] == 0) {
        trinity.press(Layout[0][currentRow][currentCol]);
        KeyState[0][currentRow][currentCol] = 1;
        Serial.println(Layout[0][currentRow][currentCol]);
      }
      else if (digitalRead( Columns[currentCol]) == LOW && KeyState[0][currentRow][currentCol] == 1) {
        trinity.release(Layout[0][currentRow][currentCol]);
        KeyState[0][currentRow][currentCol] = 0;
        Serial.println(Layout[0][currentRow][currentCol]);
      }
      currentCol++;
    }
    digitalWrite(Rows[currentRow], LOW);
    currentRow++;
    if (currentRow >= RowCount) {
      currentRow = 0;
    }
  }
  delay(1);
}
