const int LEDPin = 13;
const int numRows = 4;//定义4行
const int numCols = 4;//定义4列
const int rowPins[numRows] = {4, 5, 6, 7}; //设置4*4按键对应的引脚
const int colPins[numCols] = {8, 9, 10, 11};
const int debounceTime = 20;//去抖动时间长度 20ms

int LEDState = LOW;

void setup()
{
  pinMode(LEDPin, OUTPUT);
  digitalWrite(LEDPin, LEDState);
  for (int row = 0; row < numRows; row++)
  {
    pinMode(rowPins[row], INPUT);
    digitalWrite(rowPins[row], HIGH);
  }
  for (int column = 0; column < numCols; column++)
  {
    pinMode(colPins[column], OUTPUT);
    digitalWrite(colPins[column], HIGH);
  }
}
void loop()
{
  for (int column = 0; column < numCols; column++)
  {
    digitalWrite(colPins[column], LOW);
    for (int row = 0 ; row < numRows; row++)
    {
      if (digitalRead(rowPins[row]) == LOW) //是否有按键按下
      {
        delay(debounceTime);
        while (digitalRead(rowPins[row]) == LOW); //等待按键释放
        LEDState = !LEDState;
        digitalWrite(LEDPin, LEDState);
      }
    }
    digitalWrite(colPins[column], HIGH);
  }
}
