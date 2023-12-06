#include <Arduino.h>
#include <FastLED.h>

#define M5_BUTTON_PIN 39
#define S1_PIN 0
#define LED_PIN 22
#define CH1_PIN 26
#define CH2_PIN 18
#define CH3_PIN 19
#define CH4_PIN 25

#define LED_LENGTH 50

CRGB ch1[LED_LENGTH];
CRGB ch2[LED_LENGTH];
CRGB ch3[LED_LENGTH];
CRGB ch4[LED_LENGTH];

void set_low_test_pins()
{
    digitalWrite(LED_PIN, LOW);
    digitalWrite(CH1_PIN, LOW);
    digitalWrite(CH2_PIN, LOW);
    digitalWrite(CH3_PIN, LOW);
    digitalWrite(CH4_PIN, LOW);
}

void update_led(bool update)
{
    static int led_state = LOW;

    if (update)
    {
        led_state = !led_state;
        digitalWrite(LED_PIN, led_state);
    }
}

void update_brightness(bool update)
{
    static int brightness[] = {0x20, 0x40, 0x60, 0x80, 0xA0, 0xC0, 0xE0, 0xFF};
    static int brightness_index = 0;

    if (update)
    {
        brightness_index = (brightness_index + 1) % 8;

        FastLED.setBrightness(brightness[brightness_index]);
        fill_solid(ch1, LED_LENGTH, CRGB::White);
        fill_solid(ch2, LED_LENGTH, CRGB::White);
        fill_solid(ch3, LED_LENGTH, CRGB::White);
        fill_solid(ch4, LED_LENGTH, CRGB::White);
        FastLED.show();
    }
}

bool eval_s1_press()
{
    static int debounced_button_state;
    static int last_button_state = LOW;

    static long last_debounce_time = 0;
    static long debounce_delay = 50;

    bool pressed = false;

    int current_button_state = digitalRead(S1_PIN);
    if (current_button_state != last_button_state)
    {
        last_debounce_time = millis();
    }

    last_button_state = current_button_state;
    if ((millis() - last_debounce_time) > debounce_delay)
    {
        if (debounced_button_state != current_button_state)
        {
            debounced_button_state = current_button_state;
            if (debounced_button_state == HIGH)
            {
                pressed = true;
            }
        }
    }

    return pressed;
}

void setup()
{
    Serial.begin(115200);

    pinMode(M5_BUTTON_PIN, INPUT_PULLUP);
    pinMode(S1_PIN, INPUT_PULLUP);

    pinMode(LED_PIN, OUTPUT);
    pinMode(CH1_PIN, OUTPUT);
    pinMode(CH2_PIN, OUTPUT);
    pinMode(CH3_PIN, OUTPUT);
    pinMode(CH4_PIN, OUTPUT);

    set_low_test_pins();

    FastLED.addLeds<WS2811, CH1_PIN, BGR>(ch1, LED_LENGTH);
    FastLED.addLeds<WS2811, CH2_PIN, BGR>(ch2, LED_LENGTH);
    FastLED.addLeds<WS2811, CH3_PIN, BGR>(ch3, LED_LENGTH);
    FastLED.addLeds<WS2811, CH4_PIN, BGR>(ch4, LED_LENGTH);

    update_led(true);
    update_brightness(true);
}

void loop()
{
    int pressed = eval_s1_press();

    update_led(pressed);
    update_brightness(pressed);
}
