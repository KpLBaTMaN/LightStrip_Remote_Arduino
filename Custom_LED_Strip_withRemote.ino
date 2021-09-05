#include <IRremote.h>
#include <FastLED.h>
#include <pixeltypes.h>

const int RECV_PIN = 8;

//LED VARIABLES
#define LED_PIN     5
#define NUM_LEDS    300
#define BRIGHTNESS  64
#define LED_TYPE    WS2811
#define COLOR_ORDER GRB
CRGB leds[NUM_LEDS];

#define UPDATES_PER_SECOND 100

uint8_t brightness = 255;

//Custom Controls
bool toggle_blending = true;
int menu_option = 0;
int selection_option = 0;
int counter_temp = 0;


//Variable status
CRGBPalette16 currentPalette;
TBlendType    currentBlending;

extern CRGBPalette16 myRedWhiteBluePalette;
extern const TProgmemPalette16 myRedWhiteBluePalette_p PROGMEM;



void setup() {
  delay( 3000 ); // power-up safety delay
  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.setBrightness(  BRIGHTNESS );

  SetupMyTest();
  currentBlending = LINEARBLEND;

  //Serial.begin(9600);
  IrReceiver.begin(RECV_PIN, ENABLE_LED_FEEDBACK);
}

void loop() {

  while (!IrReceiver.isIdle());  // if not idle, wait till complete - DO NOT REMOVE
  if (IrReceiver.decode()) {
    //Serial.println(IrReceiver.decodedIRData.decodedRawData, HEX); //used to find decoded data from IR remotes
    remote_control_options();
    IrReceiver.resume();
  }


  static uint8_t startIndex = 0;
  startIndex = startIndex + 1; /* motion speed */


  FillLEDsFromPaletteColors( startIndex);

  FastLED.show();
  FastLED.delay(1000 / UPDATES_PER_SECOND);

}

//Programming all the buttons for the program
void remote_control_options() {
  switch (IrReceiver.decodedIRData.decodedRawData)
  {
    //numbers are different modes in this code.
    //After selecting mode, we can toggle through colours by using LEFT and RIGHT buttons.
    case 0xE916FF00:
      //Custom Colours
      //Serial.print("1");
      menu_option = 1;
      selection_option = 0;
      select_mode();
      break;
    case 0xE619FF00:
      SetupTotallyRandomPalette();
      //Serial.print("2");
      break;
    case 0xF20DFF00:
      //Serial.print("3");
      currentPalette = LavaColors_p;
      break;
    case 0xF30CFF00:
      //Serial.print("4");
      currentPalette = CloudColors_p;
      break;
    case 0xE718FF00:
      //Serial.print("5");
      currentPalette = OceanColors_p;
      break;
    case 0xA15EFF00:
      //Serial.print("6");
      currentPalette = ForestColors_p;
      break;
    case 0xF708FF00:
      //Serial.print("7");
      currentPalette = RainbowColors_p;
      break;
    case 0xE31CFF00:
      //Serial.print("8");
      currentPalette = RainbowStripeColors_p;
      break;
    case 0xA55AFF00:
      //Serial.print("9");
      currentPalette = PartyColors_p;
      break;
    case 0xAD52FF00:
      //Serial.print("0");
      //turn on mode
      menu_option = 0;
      selection_option = 0;
      select_mode();
      break;
    case 0xBD42FF00:
      //Serial.print("*");
      switch_blending();
      break;
    case 0xB54AFF00:
      //Serial.print("#");
      break;
    case 0xBB44FF00:
      //Serial.print("Left");
      //flicks through modes left
      selection_option -= 1;
      select_mode();

      break;
    case 0xBC43FF00:
      //Serial.print("Right");
      //flicks through modes right
      selection_option += 1;
      select_mode();
      break;
    case 0xB946FF00:
      //Serial.print("Up");
      up_button();
      break;
    case 0xEA15FF00:
      down_button();
      //Serial.print("Down");
      break;
    case 0xBF40FF00:
      //Serial.print("OK");
      break;
  }
}
void switch_blending() {
  if (toggle_blending == true) {
    currentBlending = LINEARBLEND;
    toggle_blending = false;
  } else {
    currentBlending = NOBLEND;
    toggle_blending = true;
  }
}

void select_mode() {
  switch (menu_option)
  {
    case 0:
      if (selection_option > 7) {
        selection_option = 0;
      }
      if (selection_option < 0) {
        selection_option = 7;
      }
      switch (selection_option) {
        case 0:   currentPalette = LavaColors_p;          break;  // orange, red, black and yellow),
        case 1:   currentPalette = CloudColors_p;         break;  // blue and white
        case 2:   currentPalette = OceanColors_p;         break;  // blue, cyan and white
        case 3:   currentPalette = ForestColors_p;        break;  // greens and blues
        case 4:   currentPalette = RainbowColors_p;       break;  // standard rainbow animation
        case 5:   currentPalette = RainbowStripeColors_p; break;  // single colour, black space, next colour and so forth
        case 6:   currentPalette = PartyColors_p;         break;  // red, yellow, orange, purple and blue
        case 7:   currentPalette = HeatColors_p;          break;  // red, orange, yellow and white
      }
      break;


    case 1:
    
    //6 main colours
      switch (selection_option) {
         
        case 0:   fill_solid( currentPalette, 16, CRGB::White);          break;  
        case 1:   fill_solid( currentPalette, 16, CRGB::Red);        break; 
        case 2:   fill_solid( currentPalette, 16, CRGB::Green );        break; 
        case 3:   fill_solid( currentPalette, 16, CRGB::Blue );        break;
        case 4:   fill_solid( currentPalette, 16, CRGB::Cyan );       break;  
        case 5:   fill_solid( currentPalette, 16, CRGB::Magenta ); break;  
        case 6:   fill_solid( currentPalette, 16, CRGB::Yellow );         break;  
      }

      break;
    case 2:
      if (selection_option > 7) {
        selection_option = 0;
      }
      if (selection_option < 0) {
        selection_option = 7;
      }
      switch (selection_option) {
        case 0:   SetupBlackAndWhiteStripedPalette();          break;  
        case 1:   currentPalette = myRedWhiteBluePalette_p;         break; 
        case 2:   SetupPurpleAndGreenPalette();       break;  
        case 3:   SetupMyTest();        break;  
        case 4:   currentPalette = RainbowColors_p;       break;  
        case 5:   currentPalette = RainbowStripeColors_p; break;  
        case 6:   currentPalette = PartyColors_p;         break;  
        case 7:   currentPalette = HeatColors_p;          break;  
      }
      break;


  }
}
void up_button()
{
  brightness += 17;  //255/15 = 17
}

void down_button()
{
  brightness -= 17;  //255/15 = 17
}

void FillLEDsFromPaletteColors( uint8_t colorIndex)
{

  for ( int i = 0; i < NUM_LEDS; i++) {
    leds[i] = ColorFromPalette( currentPalette, colorIndex, brightness, currentBlending);
    colorIndex += 3;
  }
}


void ChangePalette()
{

  uint8_t secondHand = (millis() / 1000) % 210;
  static uint8_t lastSecond = 99;

  Serial.println(lastSecond);


  if ( lastSecond != secondHand) {

    lastSecond = secondHand;


    if ( secondHand ==  0)  {
      SetupMyTest();
      currentBlending = NOBLEND;
    }
    if ( secondHand ==  10)  {
      SetupMyTest();
      currentBlending = LINEARBLEND;
    }

    if ( secondHand == 50)  {
      SetupTotallyRandomPalette();
      currentBlending = LINEARBLEND;
    }






  }
}

// This function fills the palette with totally random colors.
void SetupTotallyRandomPalette()
{
  for ( int i = 0; i < 16; i++) {
    currentPalette[i] = CHSV( random8(), 255, random8());
  }
}

// This function sets up a palette of black and white stripes,
// using code.  Since the palette is effectively an array of
// sixteen CRGB colors, the various fill_* functions can be used
// to set them up.
void SetupBlackAndWhiteStripedPalette()
{
  // 'black out' all 16 palette entries...
  fill_solid( currentPalette, 16, CRGB::Black);
  // and set every fourth one to white.
  currentPalette[0] = CRGB::White;
  currentPalette[4] = CRGB::White;
  currentPalette[8] = CRGB::White;
  currentPalette[12] = CRGB::White;

}

// This function sets up a palette of purple and green stripes.
void SetupPurpleAndGreenPalette()
{
  CRGB purple = CHSV( HUE_PURPLE, 255, 255);
  CRGB green  = CHSV( HUE_GREEN, 255, 255);
  CRGB black  = CRGB::Black;

  currentPalette = CRGBPalette16(
                     green,  green,  black,  black,
                     purple, purple, black,  black,
                     green,  green,  black,  black,
                     purple, purple, black,  black );
}

void SetupMyTest()
{
  //http://fastled.io/docs/3.1/struct_c_r_g_b.html
  CRGB Hotpink = CRGB::HotPink;
  CRGB Goldenrod  = CRGB::Goldenrod;
  CRGB black  = CRGB::Black;

  currentPalette = CRGBPalette16(
                     Goldenrod,  Goldenrod,  Goldenrod,  black,
                     Hotpink, Hotpink, Hotpink,  black,
                     Goldenrod,  Goldenrod,  Goldenrod,  black,
                     Hotpink, Hotpink, Hotpink,  black );
}

const TProgmemPalette16 myRedWhiteBluePalette_p PROGMEM =
{
  CRGB::Red,
  CRGB::Gray, // 'white' is too bright compared to red and blue
  CRGB::Blue,
  CRGB::Black,

  CRGB::Red,
  CRGB::Gray,
  CRGB::Blue,
  CRGB::Black,

  CRGB::Red,
  CRGB::Red,
  CRGB::Gray,
  CRGB::Gray,
  CRGB::Blue,
  CRGB::Blue,
  CRGB::Black,
  CRGB::Black
};

//1 - E916FF00
//2 - E619FF00
//3 - F20DFF00
//4 - F30CFF00
//5 - E718FF00
//6 - A15EFF00
//7 - F708FF00
//8 - E31CFF00
//9 - A55AFF00
//0 - AD52FF00
//* - BD42FF00
//# - B54AFF00
//"left button" - BB44FF00
//"right button" - BC43FF00
//"up button" - B946FF00
//"down button" - EA15FF00
//OK - BF40FF00
