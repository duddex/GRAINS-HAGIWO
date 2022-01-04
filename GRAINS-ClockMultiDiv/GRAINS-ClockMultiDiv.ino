//Clock M_D module
#include <MsTimer2.h>

unsigned long ext_count = 400; // For timer counting
unsigned long old_ext_pulse = 0;
unsigned long old_int_pulse = 0;
unsigned long ext_period = 0; // cycle
unsigned long ext_count_result = 0; // Timer count result
unsigned long old_ext_count_result = 0; // Timer count result
byte ext_pulse = 0; // 1 if there is an external clock, 0 if not
byte int_pulse = 0; // Internal clock
byte ext_injudge = 1; // External input presence/absence selector. If present: 1. If not present: 0. Selection switches when the timer count exceeds the specified value
byte old_ext_injudge = 2;

int AD_rate = 512; // Rate Knob input
int rate = 1000; // Used during internal clock operation

int AD_MD = 512; // MD Knob input value
int out_width_ch1 = 10; // Output pulse width
int out_width_ch2 = 10; // Output pulse width
int old_AD_MD = 512; // SW Countermeasures against misreading of RATE value when switching
int MD_ch1 = 5; // Multiple, divider determination ch1
int MD_ch2 = 5; // Multiple, divider determination ch2
int M_period_ch1 = 0; // When multiple, the cycle of the output clock. If it is not multiple, set it to 0
int M_period_ch2 = 0; // When multiple, the cycle of the output clock. If it is not multiple, set it to 0
int M_count_ch1 = 1; // When multiple, it counts for each pulse and returns to 0 when the set value is reached
int M_count_ch2 = 1;// When multiple, it counts for each pulse and returns to 0 when the set value is reached.
int D_count_ch1 = 1;// Increases the counter when an external pulse is applied
int D_count_ch2 = 1;// Increases the counter when an external pulse is applied
int D_full_ch1 = 1; // Upper limit of counter
int D_full_ch2 = 1; // Upper limit of counter
byte CH1out = 0; // 0 for LOW output, 1 for HIGH output
byte CH2out = 0; // 0 for LOW output, 1 for HIGH output
byte M_done_ch1 = 0; // For checking multiple outputs. Otherwise, one clock will result in multiple pulses.
byte M_done_ch2 = 0; // For checking multiple outputs. Otherwise, one clock will result in multiple pulses.


//---mode switching-------------
byte mode_sw = 1;
byte mode = 1; // 1=CH1, 2=CH2
byte old_mode = 0; // Measures against misreading of RATE value when switching SW
int old_MD_ch1 = 0; // Bug fix for divider clock to go wrong when switching
int old_MD_ch2 = 0; // Bug fix for divider clock to go wrong when switching

void setup() {
 pinMode(7, OUTPUT); // CH1 out
 pinMode(8, OUTPUT); // CH2 out
 pinMode(13, OUTPUT); // internal_clock_out
 pinMode(11, INPUT_PULLUP); // SW
 pinMode(3, INPUT); // ext_clock_in
 Serial.begin(9600);

 MsTimer2::set(1, timer_count); // 1ms Timer count every time
 MsTimer2::start(); // When the external input becomes High, it counts to the next High
}

void loop() {
 AD_MD = analogRead(3);
 AD_rate = 1023 - analogRead(4);
 rate = AD_rate * 2 + 100;

 old_ext_pulse = ext_pulse;
 old_int_pulse = int_pulse;
 ext_pulse = digitalRead(3);

 mode_sw = digitalRead(11);
 old_mode = mode;

 old_ext_injudge = ext_injudge;

 //-----------Divider output count reset------------
 if (D_count_ch1 >= D_full_ch1) {
   D_count_ch1 = 0;
 }
 if (D_count_ch2 >= D_full_ch2) {
   D_count_ch2 = 0;
 }

 //-----------SW Read---------------
 old_MD_ch1 = MD_ch1; // Bug fix for divider clock to go wrong when switching
 old_MD_ch2 = MD_ch2; // Bug fix for divider clock to go wrong when switching

 if ( mode_sw == 1  ) {
   mode = 1;
 }
 else if (mode_sw == 0  ) {
   mode = 2;
 }

 if ( old_mode != mode ) { // Bug fix for divider clock to go wrong when switching
   old_AD_MD = AD_MD;
 }

 //---------MD knob determination-----------------------
 if ( mode == 1 && abs(old_AD_MD - AD_MD ) > 30) { // "abs" fixes a bug that the divider clock goes wrong when switching
   old_AD_MD = 1200;//abs(old_AD_MD - AD_MD ) > 30が絶対に成立するため (not translated - Google translation did not make sense)

   if ( AD_MD >= 0 && AD_MD < 20) {
     MD_ch1 = 9;//*16
   }

   else if ( AD_MD >= 20 && AD_MD < 90) {
     MD_ch1 = 8;//*8
   }

   else if ( AD_MD >= 90 && AD_MD < 240) {
     MD_ch1 = 7;//*4
   }

   else if ( AD_MD >= 240 && AD_MD < 400) {
     MD_ch1 = 6;//*2
   }

   else if ( AD_MD >= 400 && AD_MD < 550) {
     MD_ch1 = 5;//*1
   }

   else if ( AD_MD >= 550 && AD_MD < 700) {
     MD_ch1 = 4;//1/2
   }

   else if ( AD_MD >= 700 && AD_MD < 820) {
     MD_ch1 = 3;//1/3
   }

   else if ( AD_MD >= 820 && AD_MD < 960) {
     MD_ch1 = 2;//1/4
   }

   else if ( AD_MD >= 960 && AD_MD < 1024) {
     MD_ch1 = 1;//1/8
   }
 }

 else if ( mode == 2 && abs(old_AD_MD - AD_MD ) > 30) { // "abs" fixes a bug that the divider clock goes wrong when switching
   old_AD_MD = 1200;//abs(old_AD_MD - AD_MD ) > 30が絶対に成立するため (not translated - Google translation did not make sense)

   if ( AD_MD >= 0 && AD_MD < 20) {
     MD_ch2 = 9;//*16
   }

   else if ( AD_MD >= 20 && AD_MD < 90) {
     MD_ch2 = 8;//*8
   }

   else if ( AD_MD >= 90 && AD_MD < 240) {
     MD_ch2 = 7;//*4
   }

   else if ( AD_MD >= 240 && AD_MD < 400) {
     MD_ch2 = 6;//*2
   }

   else if ( AD_MD >= 400 && AD_MD < 550) {
     MD_ch2 = 5;//*1
   }

   else if ( AD_MD >= 550 && AD_MD < 700) {
     MD_ch2 = 4;//1/2
   }

   else if ( AD_MD >= 700 && AD_MD < 820) {
     MD_ch2 = 3;//1/3
   }

   else if ( AD_MD >= 820 && AD_MD < 960) {
     MD_ch2 = 2;//1/4
   }

   else if ( AD_MD >= 960 && AD_MD < 1024) {
     MD_ch2 = 1;//1/8
   }
 }


 switch (MD_ch1) {
   case 1:
     out_width_ch1 = ext_period / 2 / 8;
     M_period_ch1 = ext_period / 8;
     break;

   case 2:
     out_width_ch1 = ext_period / 2 / 4;
     M_period_ch1 = ext_period / 4;
     break;

   case 3:
     out_width_ch1 = ext_period / 2 / 3;
     M_period_ch1 = ext_period / 3;
     break;

   case 4:
     out_width_ch1 = ext_period / 2 / 2;
     M_period_ch1 = ext_period / 2;
     break;

   case 5:
     out_width_ch1 = ext_period / 2 ;
     M_period_ch1 = 0;
     D_full_ch1 = 1;
     break;

   case 6:
     out_width_ch1 = ext_period / 2;
     M_period_ch1 = 0;
     D_full_ch1 = 2;
     break;

   case 7:
     out_width_ch1 = ext_period / 2;
     M_period_ch1 = 0;
     D_full_ch1 = 4;
     break;

   case 8:
     out_width_ch1 = ext_period / 2;
     M_period_ch1 = 0;
     D_full_ch1 = 8;
     break;

   case 9:
     out_width_ch1 = ext_period / 2;
     M_period_ch1 = 0;
     D_full_ch1 = 16;
     break;
 }


 switch (MD_ch2) {
   case 1:
     out_width_ch2 = ext_period / 2 / 8;
     M_period_ch2 = ext_period / 8;
     break;

   case 2:
     out_width_ch2 = ext_period / 2 / 4;
     M_period_ch2 = ext_period / 4;
     break;

   case 3:
     out_width_ch2 = ext_period / 2 / 3;
     M_period_ch2 = ext_period / 3;
     break;

   case 4:
     out_width_ch2 = ext_period / 2 / 2;
     M_period_ch2 = ext_period / 2;
     break;

   case 5:
     out_width_ch2 = ext_period / 2 ;
     M_period_ch2 = 0;
     D_full_ch2 = 1;
     break;

   case 6:
     out_width_ch2 = ext_period / 2 ;
     M_period_ch2 = 0;
     D_full_ch2 = 2;
     break;

   case 7:
     out_width_ch2 = ext_period / 2 ;
     M_period_ch2 = 0;
     D_full_ch2 = 4;
     break;

   case 8:
     out_width_ch2 = ext_period / 2 ;
     M_period_ch2 = 0;
     D_full_ch2 = 8;
     break;

   case 9:
     out_width_ch2 = ext_period / 2 ;
     M_period_ch2 = 0;
     D_full_ch2 = 16;
     break;
 }

 if ( MD_ch1 != old_MD_ch1 || MD_ch2 != old_MD_ch2 ) { // Bug fix for divider clock to go wrong when switching
   D_count_ch1 = 0;
   D_count_ch2 = 0;
 }

 //------------External input presence/absence selection-------------------------------
 if ( ext_count > 4000 ) { // If there is no count for 4s or more, we assume that there is no external input
   ext_injudge = 0;
 }
 else if ( ext_count < 4000 && ext_pulse == 1 ) {
   ext_injudge = 1;
 }

 if ( old_ext_injudge == 1 && ext_injudge == 0 ) { // When there is external input -> no
   ext_count = 0;
 }


 //---------Clock setting------------------------
 if ( ext_injudge != 0 ) { // Use an external clock
   if (ext_pulse == 1 && old_ext_pulse == 0) {
     old_ext_count_result = ext_count_result;// For taking an average of 2 times
     ext_count_result = ext_count;
     //            ext_count = 0;
     ext_period = (old_ext_count_result + ext_count_result) / 2; // External input cycle. Average value of 2 times to reduce variation
     //      MsTimer2::start();// When the external input becomes High, it counts to the next High.
   }
 }
 else if ( ext_injudge == 0) { // Use internal clock
   ext_period = rate;
   if ( ext_count < 5 || ext_count >= ext_period) {
     int_pulse = 1;
   }
   else if (ext_count >= 5 ) {
     int_pulse = 0;
   }

 }
 //--------------INTERNAL CLOCK Output (only when there is no external input)-----------
 if ( int_pulse == 1 ) {
   digitalWrite(13, HIGH);
 }
 else if ( ext_count >= ext_period / 2  ) {
   digitalWrite(13, LOW);
 }

 //-----------------OUT1 output------------------
 if (ext_pulse == 1 && old_ext_pulse == 0) { // For external clock
   D_count_ch1 ++;
   ext_count = 0;
   M_count_ch1 = 0;
   M_done_ch1 = 0;
   if ( MD_ch1 <= 4 ) {
     digitalWrite(7, HIGH);
     CH1out = 1;
   }
 }

 if (int_pulse == 1 && old_int_pulse == 0) { // For internal clock
   D_count_ch1 ++;
   ext_count = 0;
   M_count_ch1 = 0;
   M_done_ch1 = 0;
   if ( MD_ch1 <= 4 ) {
     digitalWrite(7, HIGH);
     CH1out = 1;
   }
 }

 if ( MD_ch1 < 5 ) {
   if ( ext_count  >= M_period_ch1  * M_count_ch1  && CH1out == 0) {
     CH1out = 1;
     digitalWrite(7, HIGH);
   }
   if ( ext_count >= M_period_ch1  * M_count_ch1 + out_width_ch1  && CH1out == 1 ) {
     digitalWrite(7, LOW);
     M_count_ch1 ++;
     CH1out = 0;
   }
 }

 else if ( MD_ch1 == 5 ) {
   if (D_count_ch1 == 1 && M_done_ch1 == 0) {
     CH1out = 1;
     M_done_ch1 = 1;
     digitalWrite(7, HIGH);
   }
   if ( ext_count >=  out_width_ch1   ) {
     digitalWrite(7, LOW);
     CH1out = 0;
   }
 }

 else if ( MD_ch1 > 5 ) {
   if (D_count_ch1 == 1 && M_done_ch1 == 0) {
     CH1out = 1;
     digitalWrite(7, HIGH);
     M_done_ch1 = 1;
   }
   if ( ext_count >=  out_width_ch1   ) {
     digitalWrite(7, LOW);
     CH1out = 0;
   }
 }


 //-----------------OUT2 output------------------
 if (ext_pulse == 1 && old_ext_pulse == 0) {
   D_count_ch2 ++;
   M_count_ch2 = 0;
   M_done_ch2 = 0;
   if ( MD_ch2 <= 4 ) {
     digitalWrite(8, HIGH);
     CH2out = 1;
   }
 }

 if (int_pulse == 1 && old_int_pulse == 0) { // For internal clock
   D_count_ch2 ++;
   M_count_ch2 = 0;
   M_done_ch2 = 0;
   if ( MD_ch2 <= 4 ) {
     digitalWrite(8, HIGH);
     CH2out = 1;
   }
 }

 if ( MD_ch2 < 5 ) {
   if ( ext_count  >= M_period_ch2  * M_count_ch2  && CH2out == 0) {
     CH2out = 1;
     digitalWrite(8, HIGH);
   }
   if ( ext_count >= M_period_ch2  * M_count_ch2 + out_width_ch2  && CH2out == 1 ) {
     digitalWrite(8, LOW);
     M_count_ch2 ++;
     CH2out = 0;
   }
 }

 else if ( MD_ch2 == 5 ) {
   if (D_count_ch2 == 1 && M_done_ch2 == 0) {
     CH2out = 1;
     M_done_ch2 = 1;
     digitalWrite(8, HIGH);
   }
   if ( ext_count >=  out_width_ch2   ) {
     digitalWrite(8, LOW);
     CH2out = 0;
   }
 }

 else if ( MD_ch2 > 5 ) {
   if (D_count_ch2 == 1 && M_done_ch2 == 0) {
     CH2out = 1;
     M_done_ch2 = 1;
     digitalWrite(8, HIGH);
   }
   if ( ext_count >=  out_width_ch2   ) {
     digitalWrite(8, LOW);
     CH2out = 0;
   }
 }


 // For development:
 Serial.print(ext_count);
 Serial.print(",");
 Serial.print(CH1out * 500);
 Serial.print(",");
 Serial.print(D_count_ch1 * 80);
 Serial.println("");

}

// Timer count. Increase the count every 1ms
void timer_count() {
 ext_count ++;
}