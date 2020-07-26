MAR1D
=====

## Under Construction
The game is undergoing a lot of changes. I'd like to get it into shape so I can be proud to put it out there. Until I do, don't expect the README to match the game or anything like that.

Four things need to get done:

- Easy install & cross-compatibility.
  - As much as I might want everyone to just switch to linux, it's not going to happen. And a Nix package is certainly not sufficient for most people. I'd like the game to have a download-and-go package for Mac, Linux, and, yes, Windows.
- Menu
  - Not everyone can use command line options. I need a menu screen for settings, remapping controls, etc.
- Playtesting
  - I don't need this to play *exactly* like the original, but for a while I was missing running, which is pretty essential. When I'm done with the first two I'm going to need some people to test it and make sure I'm not missing anything glaring.
- A website
  - It's gotta be presentable.

## Description
The original Super Mario Bros as you've never seen it. Step into Mario's shoes in this first person clone of the classic Mario game. True to the original, however, the game still takes place in a two dimensional world. You must view the world as mario does, as a one dimensional line.

## Inspiration / Ideas
The book "Flatland" by Edwin Abbott Abbott did a really neat thing in trying to make intuitive the notion of *living* in a two dimensional world. But when we exist in a world, we *act* in that world, and the natural way to bring an unfamiliar space to our intuitions is with an active roll. To this end, I've created MAR1D, a video game that is played in two dimensional space, so we can not only understand, but *feel* what two dimensions is.

I have plans to make similar games that explore other geometries in the same way; though these games will probably not be done for a while.

Other notable similar exploits include:

* [Flatland](https://en.wikipedia.org/wiki/Flatland)
* [magictile](http://roice3.org/magictile/)
* [A Slower Speed of Light](http://gamelab.mit.edu/games/a-slower-speed-of-light/)
* [Antichamber](http://www.antichamber-game.com/)
* [Hyperrogue](http://www.roguetemple.com/z/hyper/)
* [4D Toys](http://www.4dtoys.com/) (Though I've never personally played it)
* [Hyperbolica](https://store.steampowered.com/app/1256230/Hyperbolica/) (Someone seems to have actually made my dream game.)
* Send me any I'm missing! I love playing with these things :)

## Playing the Game
I'll be trying to put out binaries in the near future, but for now you'll have to compile it yourself, and then run the `./MAR1D` executable in the main directory, or `./install/bin/MAR1D` (or wherever you've installed it) with optional command line arguments as described below.

The `./install/bin/MAR1D` executable takes a few command line arguments (note that all numerical arguments must be integers):
* `-m` mutes the audio
* `-f` unmutes the sound effects if the audio is muted, so it does nothing on its own, but `-mf` will enable only sound effects.
* `-w##` sets the display area to a width of `##` pixels.
* `-s##` sets the mouse sensitivity.
* `-r` reverses the mouse y axis. Not sure why this is necessary, but lots of games seem to include it.

## Compiling
### Dependencies
* Cmake
* SDL2
* SDL2_mixer
* GLU
* libconfig

### Linux
It should be enough to just run ./compile.sh from the main source directory if you have all the dependencies.
If you would like to install it under a certain prefix, you can modify ./compile.sh to change `-DCMAKE_INSTALL_PREFIX=../install` to point to wherever you would like to install it

***NOTE:*** If you use nix(os), there's a nix package for MAR1D under 'mar1d'. If you don't use nix(os), you should!

### Mac OS X

## License
Licensed under AGPL v3.0, check COPYING for more details. If you have something you want to do with it that doesn't work under that license, let me know. Basically, if you're trying to make and share awesome things with it, I want to help you do that.

## Extras

Below is a screencap of me playing through the first stage, as well as an image depicting the same video, with time on the x-axis.

![mar1d-gif](./mar1d.gif)

![mar1d-png](./mar1d.png)

## Credits
* Edwin Abbott Abbott
  * Writing Flatland and inspiring this game
* [Maurice Guégan](http://stabyourself.net/about/)
  * Making [Mari0](http://stabyourself.net/mari0/), and inspiring me to hack on Mario
  * The audio files in MAR1D are from Mari0
  * Helping me out with the game logic for spawning enemies
  * Being an awesome guy
* Owen Lynch
  * Hosting the game on his github
  * Helping out with some of the coding (Especially setting up cmake. I hate compiling C code)
  * Being a great friend
* Friends and Family
  * Testing
  * Moral Support
  * Putting up with me getting sucked into coding for long stretches
* Yavni
  * Photoshoping the menuscreen image
  * Coming up with the name "MAR1D"
* Nintendo
  * Making Super Mario Bros
  * Inspiring game makers
* My parents
  * Moral support
  * Raising me
  * Inspiring me
  * Shaping me into an inquisitive person
