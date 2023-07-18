#include "rgbled.hpp"

void RGBLED::loadSetting(Setting s)
{
    RGBLED::R = s.ledRed;
    RGBLED::G = s.ledGreen;
    RGBLED::B = s.ledBlue;
    pinMode(RGBLED::R, OUTPUT);
    pinMode(RGBLED::G, OUTPUT);
    pinMode(RGBLED::B, OUTPUT);
    RGBLED::off();
}
void RGBLED::off()
{
    digitalWrite(RGBLED::R, LOW);
    digitalWrite(RGBLED::G, LOW);
    digitalWrite(RGBLED::B, LOW);
    RGBLED::color = C_OFF;
    RGBLED::state = false;
}
void RGBLED::red()
{
    digitalWrite(RGBLED::R, HIGH);
    digitalWrite(RGBLED::G, LOW);
    digitalWrite(RGBLED::B, LOW);
    RGBLED::color = C_RED;
    RGBLED::state = true;
}
void RGBLED::green()
{
    digitalWrite(RGBLED::R, LOW);
    digitalWrite(RGBLED::G, HIGH);
    digitalWrite(RGBLED::B, LOW);
    RGBLED::color = C_GREEN;
    RGBLED::state = true;
}
void RGBLED::blue()
{
    digitalWrite(RGBLED::R, LOW);
    digitalWrite(RGBLED::G, LOW);
    digitalWrite(RGBLED::B, HIGH);
    RGBLED::color = C_BLUE;
    RGBLED::state = true;
}
void RGBLED::purple()
{
    digitalWrite(RGBLED::R, HIGH);
    digitalWrite(RGBLED::G, LOW);
    digitalWrite(RGBLED::B, HIGH);
    RGBLED::color = C_PURPLE;
    RGBLED::state = true;
}
void RGBLED::yellow()
{
    digitalWrite(RGBLED::R, HIGH);
    digitalWrite(RGBLED::G, HIGH);
    digitalWrite(RGBLED::B, LOW);
    RGBLED::color = C_PURPLE;
    RGBLED::state = true;
}
void RGBLED::cyan()
{
    digitalWrite(RGBLED::R, LOW);
    digitalWrite(RGBLED::G, HIGH);
    digitalWrite(RGBLED::B, HIGH);
    RGBLED::color = C_CYAN;
    RGBLED::state = true;
}
void RGBLED::on(u_int8_t color)
{
    switch (color)
    {
    case C_RED:
        RGBLED::red();
        break;
    case C_GREEN:
        RGBLED::green();
        break;
    case C_BLUE:
        RGBLED::blue();
        break;
    case C_YELLOW:
        RGBLED::yellow();
        break;
    case C_CYAN:
        RGBLED::cyan();
        break;
    case C_PURPLE:
        RGBLED::purple();
        break;
    default:
        RGBLED::off();
        break;
    }
}
void RGBLED::test()
{
    RGBLED::red();
    delay(800);
    RGBLED::green();
    delay(800);
    RGBLED::blue();
    delay(800);
    RGBLED::yellow();
    delay(800);
    RGBLED::cyan();
    delay(800);
    RGBLED::purple();
    delay(800);
    RGBLED::off();
}
void RGBLED::blink(uint8_t color)
{
    if (RGBLED::state) {
        RGBLED::off();
    } else {
        RGBLED::on(color == C_OFF ? RGBLED::color : color);
    }
}