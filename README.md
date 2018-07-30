[![Dali Clock Demonstration](https://github.com/marciot/arduino-dali-clock/raw/master/artwork/youtube.jpg)](https://www.youtube.com/watch?v=sAwiu89En18)

arduino-dali-clock
==================

This is an Arduino recreation of the "Dali" digital clock where the digits "melt" into one another.

The Dali clock was first written in 1979 by Steve Capps for the Xerox Alto and later ported to the Macintosh.
Later, Jamie Zawinski re-implemented an X Windows version called [xdaliclock].

I began with the ".xbm" font files from Jamie's work, but my implementation does not share any other code with that version. I wrote an encoding program (DaliClockEncoder.ino) to convert the ".xbm" into a packed RLE representation which is used by the clock code (DaliClock.ino). 

I wrote the digit blending and time keeping routines from scratch to take advantage of the enhanced capabilities of the [FT810CB] high resolution touch screen.

This program is intended for use with a [FT810CB-HY50HD] 800x480 graphical touchscreen and a DS1302 based Real Time Clock module from eBay.

For more information on assembling this clock, see my [Instructable].

[logo]: https://github.com/marciot/arduino-dali-clock/raw/master/artwork/youtube.jpg "A dome screenshot"
[xdaliclock]: https://www.jwz.org/xdaliclock
[FT810CB-HY50HD]: https://www.hotmcu.com/5-graphical-lcd-touchscreen-800x480-spi-ft810-p-286.html
[Instructable]: https://www.instructables.com/id/80s-Style-Melting-Digital-Dali-Clock