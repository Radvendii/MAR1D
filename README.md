MAR1D
=====

## Under Construction
The game is undergoing a lot of changes. I'd like to get it into shape so I can be proud to put it out there. Until I do, don't expect the README to match the game or anything like that.

Two things need to get done:

- Mac .app bundle
- Trailer


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

### Options

Options can be configured either through an in-game menu, which will save to a config file. That config file can also be edited directly. They can also be set through command line options (except on windows, where the in-game / config file will have to suffice)

The `MAR1D` executable takes a few command line arguments (note that all numerical arguments must be integers):
* `-m##` sets the music volume (0-128)
* `-f##` sets the sound effects volume (0-128).
* `-w##` sets the display area to a width of `##` pixels.
* `-s##` sets the mouse sensitivity.
* `-y` inverts the mouse y axis. Not sure why this is necessary, but lots of games seem to include it.
* `-d` debug mode

In addition, there are two environment variables which affect where MAR1D puts / looks for files.

* `MAR1D_RECORDING_DIR` where to put .png recordings of your playthrough. By default this is the current directory on Linux and Windows, and the current user's desktop on Mac OS X.
* `MAR1D_RESOURCE_DIR` where to find the resource files. By default this is wherever they were installed.

## Compiling

MAR1D uses the meson build system, with a nix wrapper around it.

You should be able to just compile directly with meson, but I don't actually know how to do that so you'll have to figure it out yourself. If you do, please send a pull request updating this section with instructions.

To compile on a system using the [nix package manager](https://nixos.org/), it's as simple as `nix-build`.

To cross-compile for windows, `nix-build windows.nix` (then you can either run through wine or transfer it to a windows computer).

To compile a universal linux binary that can be transferred to other computers, you have two options. You can use `nix-build bundle.nix`, which produces a binary symlinked to `./result`. The disadvantage of bundle is it's extremely slow to compile and boot up.

Or you can use [flatpak](https://www.flatpak.org/), which has the disadvantage of needing flatpak installed on the target platform, but is much faster. (TODO: expand this section)

To compile a .app bundle for Mac OS X, you can run `nix-build darwin-app.nix`.

### Dependencies
* meson (with ninja and pkgconfig)
* SDL2
* SDL2_mixer
* GLU
* libconfig

## License
Licensed under AGPL v3.0, check COPYING for more details. If you have something you want to do with it that doesn't work under that license, let me know. Basically, if you're trying to make and share awesome things with it, I want to help you do that.

## Extras

Below is a screencap of me playing through the first stage, as well as an image depicting the same video, with time on the x-axis.

![mar1d-gif](./mar1d.gif)

![mar1d-png](./mar1d.png)

## Credits
* [Taeer Bar-Yam](https://taeer.bar-yam.me)
  * Hey, that's me!
* [Owen Lynch](owenlynch.org)
  * Helping out with the coding when I get stuck
  * Telling me when I'm being an idiot
* [Sophie Galowitz](http://sophiegalowitz.com/)
  * Aesthetic
* [Yavni Bar-Yam](https://yavni.org)
  * Photoshoping the menuscreen image
  * Coming up with the name "MAR1D"
* [Maurice Guégan](http://stabyourself.net/about/)
  * Making [Mari0](http://stabyourself.net/mari0/), and inspiring me to hack on Mario
  * The audio files in MAR1D are from Mari0
  * Helping me out with the game logic for spawning enemies
* Friends and Family
  * Testing
  * Moral Support
  * Putting up with me getting sucked into coding for long stretches
* Edwin Abbott Abbott
  * Writing Flatland and inspiring this game
* Nintendo
  * Making Super Mario Bros, and inspiring game makers
