# LD33 - Maze Monster

This game was written in less than 48 hours for the Ludum Dare 33 compo with the theme "You are the Monster".

You play a monster that survives on energy orbs. Without these orbs you will stop emitting light and die. The goal of the game is to collect all the energy orbs without dying.

You also have to watch out for the humans. They think you are a dangerous monster and they will come after you and try to kill you when they see you.

The maze is generated randomly every time you play. In the menu you will be able to change the default settings and choose the size of the maze, your speed, the speed of the humans, ...
But even without changing any settings each time the maze will be completely different.

To move use the arrow keys or WASD or QZSD.


### Download executable

There are executables available for [Windows](https://github.com/texus/LD33-Game/releases/download/LD33-entry/LD33-MazeMonster-Windows.zip), [Mac OS X](https://github.com/texus/LD33-Game/releases/download/LD33-entry/LD33-MazeMonster-OSX.zip) and [64-bit Linux](https://github.com/texus/LD33-Game/releases/download/LD33-entry/LD33-MazeMonster-Linux64.tar.gz).


### Build yourself

You will need git and cmake in order to build this project.

The following instructions are for unix, but the steps are similar on windows (just a bit more difficult).

The code below will download everything, build and install the SFML and TGUI dependencies and create the game executable in the root folder. It assumes that the SFML dependencies are already installed. If not, the first cmake command will fail and tell you which library could not be found.

```
git clone https://github.com/texus/LD33-Game
cd LD33-Game
git submodule update --init --recursive
mkdir build
mkdir build/SFML
mkdir build/TGUI
cd build/SFML
cmake ../../SFML
make -j4
sudo make install
cd ../TGUI
cmake ../../TGUI
make -j4
sudo make install
cd ..
cmake ..
make -j4
make install
cd ..
./Game
```
