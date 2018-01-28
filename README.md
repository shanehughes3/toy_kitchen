# toy_kitchen

An AVR-controlled system for controlling lights in a toy kitchen, with timeout.


## Synopsis

My wife made a toy kitchen for my son out of cardboard. I wanted to make it
a bit more "interactive" without going overboard, so I decided on a simple
LED system to give him some buttons and knobs to play with. Since it would be
easier to make it battery-powered, I wanted to be as conservative with
current as possible.

Like many children's toys, I figured a simple timeout would work best - it
should shut off after 10 minutes without input. I also wanted to power down the
AVR at that point, since even the 4mA I measured at idle would drain a 9v in
a matter of days. Thus the controller starts and remains in power-down mode
until awoken with a pin-change interrupt, and returns there after timeout.

Currently there are two buttons powering two banks of LEDs - one for
under-cabinet lighting and one for an oven light. The next addition (once I
get some pots) is to add knobs that will power adjustable "burner" lights.


## Setup

Schematic coming soon - but it's pretty simple.

This is built around an attiny2313.


## Build/Load

```
make           // assumes t2313
make install   // assumes usbasp programmer - may require root
make clean
```


## License

Beerware