# GRAINS-ClockMultiDiv

Original code and much more information about this module: <https://note.com/solder_state/n/n8907f2f6e8f5>
(Hint: use Google Chrome's "Translate to English" option from the context menu of the page)

## Input / Knobs

### Default

* IN1/P1: Gate length
* IN2/P2: Mult / Div
* P3: Internal clock rate
* IN3: external clock input
* AUDIO In: Mode switch (LOW: channel 1, HIGH: channel 2)

Output:

* D: internal clock output
* OUT: Channel 1/Channel 2 (depends on M/G switch)

### Customization

GRAINS has 5 input pins. IN1/P1 and IN2/P2 can be controlled manually using the potentiometer or can be controlled with control voltage. P3 can be controlled only manually and IN3 and AUDIO IN can only receive control voltage.

Depending on your needs you can select if you want to control the parameters manually or with control voltage by assigning the constants SWITCH, EXT_CLOCK_IN, CLOCK_RATE, MULT_DIV and GATE_LENGTH to the input pins you want to use.

```C++
...
#define GRAINS_IN1_P1 (A2)
#define GRAINS_IN2_P2 (A1)
#define GRAINS_P3 (A0)
#define GRAINS_IN3 (A3)
#define GRAINS_AUDIO_IN (A4)
...
static const byte SWITCH GRAINS_AUDIO_IN;
static const byte EXT_CLOCK_IN GRAINS_IN3;
static const byte GATE_LENGTH GRAINS_IN1_P1;
static const byte CLOCK_RATE GRAINS_P3;
static const byte MULT_DIV GRAINS_IN2_P2;
...
```
