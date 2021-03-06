# Color Match - A simple LibSX based GUI game in Ubuntu #

Match all the card pairs with same color and win the game!

## Features ##

There are three activities in this game:

1. Play game.
2. View leaderboard.
3. Set the number of cards by row and column number.

### Game Playing ###

* The clock at the top right will start ticking since you made the first click.
* The time is counted in seconds.
* Each time you click a unpaired card, the card will flip and show its color.
  If you click two cards with same color in a row, this pair will stay revealed.
  Otherwise, when you make further click, this pair will hide their color again.
* When you finished matching all the card pairs, a result window will show up,
  in which state the time you used and whether you've made a record. If a new
  record is made, the window will prompt your name and update the leaderboard.
* You can click the "Back" button and go back to the start window whenever you want.

### Leaderboard ###

* Leaderboard show the top 10 records for each game size.
* Each record contains the player's name and game time.
* One player can hold several records in the leaderboards.
* Player can initiate the leaderboard by click "Reset" button.

### Setting ###

You can specify the number of row(X) and column(Y).
If both number you gave is odd, the column number is increase by one,
which makes it possible to color the cards pair by pair.

## Build ##

This game use the APIs provided by [LibSX](https://www.ee.usyd.edu.au/tutorials/ee_database/programming/libsx/libsx.html),
a very light-weight library based on X and the Athena Widget.

LibSX can be installed via `sudo apt-get install libsx0`.
However, there's no need to install if you only want to try this game,
the link file and header is already included.

There may still be some dependancies on X libraries that I have not come
across during coding. So I assume it already satisfied XD.

You can simply run the game:

```
./ColorMatch
```

## Develop ##

There's already a simple make script, you can simply build the project:

```
make all
```

You can also clear the intermediate files after the project is built:

```
make clean
```

Source code of each activity described in Features can be found under `src` directory.

The header files are maintained under `include` directory
