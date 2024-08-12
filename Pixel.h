#include <Adafruit_NeoPixel.h>

#define pin_led 26 // VESPA S1 pinout
#define nmb_px 8 // numeros de pixels do led

Adafruit_NeoPixel pixels(nmb_px, pin_led, NEO_GRB + NEO_KHZ800);


void px_begin() {

  pixels.begin(); // iniciando pixels 
  pixels.setBrightness(50); // luminosidade dos pixels
}


// FUNÇAO ACENDENDO TODOS OS LEDS //
void px_fill(uint8_t r, uint8_t g, uint8_t b) {

  for(int i = 0; i < nmb_px; i++) pixels.setPixelColor(i, pixels.Color(r, g, b));
    pixels.show();
}


/*void rainbow(int wait) {
  // Hue of first pixel runs 5 complete loops through the color wheel.
  // Color wheel has a range of 65536 but it's OK if we roll over, so
  // just count from 0 to 5*65536. Adding 256 to firstPixelHue each time
  // means we'll make 5*65536/256 = 1280 passes through this loop:
  for(long firstPixelHue = 0; firstPixelHue < 5*65536; firstPixelHue += 256) {
    // strip.rainbow() can take a single argument (first pixel hue) or
    // optionally a few extras: number of rainbow repetitions (default 1),
    // saturation and value (brightness) (both 0-255, similar to the
    // ColorHSV() function, default 255), and a true/false flag for whether
    // to apply gamma correction to provide 'truer' colors (default true).
    pixels.rainbow(firstPixelHue);
    // Above line is equivalent to:
    // strip.rainbow(firstPixelHue, 1, 255, 255, true);
    pixels.show(); // Update strip with new contents
    delay(wait);  // Pause for a moment
  }
}*/