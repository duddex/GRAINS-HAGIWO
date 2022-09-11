# GRAINS-MozziFMVCO

Original code and much more information about this module: <https://note.com/solder_state/n/n88317851a4c7>  
(Hint: use Google Chrome's "Translate to English" option from the context menu of the page)

Set the M/G switch to M (for "Mozzi")

The original code uses 6 input parameters
* knob1: carrier frequency
* knob2: modulation frequency
* knob3: modulation amplitude
* freq CV: modulation frequency CV
* level CV: modulation amplitude CV
* V/oct

GRAINS only has 5 inputs. So I had to set knob_freq to a fixed value (line 41 in the code). Feel free to experiment with other values.
