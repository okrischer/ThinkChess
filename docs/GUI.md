# Enhancing the GUI

In this chapter we'll add a dashboard to our app, including

-   a *timer* for keeping track of the player's thinking time
-   an indicator for *material evaluation*
-   the display of *captured pieces* for each player
-   a *move indicator* for the last move
-   a display for the complete *moves history*, and
-   an *info field* for displaying messages from the app.

We will also store the current game to a text-file for later use, and add
a splash-screen, allowing to display the result of a game and to switch
the game mode.

Furthermore, we will give a player the option to take back a move, to
offer a draw, and to resign.

Finally, we'll revisit some *special moves*.

With all that in place, our app will look like so:

![](img/display.jpg)

## Refactoring

First of all, I created a new translation unit at `app/display.cpp` and
its header file at `include/display.hpp`, which will contain all the
display-oriented logic.

Next, I created another new unit at `app/position.cpp` and its header
file at\
`include/position.hpp` which contains all the move-oriented logic.\
I moved all the functions from the old file `app/moves.cpp` to this new
file and deleted the old file.

This new file contains a new type `Position`, which holds all the
position-relevant variables, so I could delete those from the `main()`
function.

``` {.cpp .numberLines}
class Position {
public:
  ~Position() {}
  Position(short gs) : gamestate{gs}, player{true},
               board{8, vector<Piece*>(8)},
               mvCount{0}, castled{0}, checkmate{-1, -1},
               checked{false}, eval{0.f}
  {}

  // returns a material evaluation for both players
  void evaluate();

  // take back last move
  bool takeBackMove();

  // make a move
  bool makeMove(pair<int, int> from, pair<int, int> to);

  // gamestate: 0 = game over, 1 = 2-player, 2 = analyze 
  short gamestate;

  // player on turn, starting with white
  bool player;
  
  // matrix of pieces representing the board
  vector<vector<Piece*>> board;

  // vector of moves, used as a stack
  vector<string> moves;

  // list of captured pieces, used as a stack
  vector<Piece*> captured;

  // total count of moves
  int mvCount;
  
  // castled: 0 = no, 1 = white, 2 = black, 3 = both
  short castled;
  
  // coordinates of the piece, which gave checkmate
  std::pair<int, int> checkmate;
  
  // last move gave check
  bool checked;

  // basic evaluation of positions
  float eval;

  // infotext
  string info;
};
```

Observe, that I've made all the member types public, in order to avoid
creating all the associated *getter* and *setter* functions.
This would not be a good design decision, if we were to create a library,
which was intended to be used by different clients, as those clients
could harm the game logic.
But, in our case, the only client is the `main()` function of our app,
and I suppose that we know what we are doing.

With the new type `Position` in place, we can initialize all relevant
variables with a single call from inside the main function:
`Position position(0)`{.cpp} and call the `makeMove` function with just
two parameters (which are the field coordinates from the board) like so:
`position.makeMove(touched, to)`{.cpp}.

Finally, I extended the viewport of the app and made it not resizable:

``` {.cpp .numberLines}
auto window = sf::RenderWindow{ {870, 640u},
                                "ThinkChess++",
                                sf::Style::Close,
                                settings };

window.clear(sf::Color(100, 100, 100));
```

The last line just sets the background of the display area to gray
(inside the game loop).

## Timer

For displaying the *timer*, we first have to include the `chrono`
library and define two time counters, as well as two time points:

``` {.cpp .numberLines}
#include <chrono>
using namespace chrono;

unsigned wTime = 0;
unsigned bTime = 0;
steady_clock::time_point last;
last = steady_clock::now();
steady_clock::time_point now;
```

For calculating the current thinking time for both player, we have this
code inside the game loop, just before drawing anything:

``` {.cpp .numberLines}
if (position.gamestate == 1) {
  now = steady_clock::now();
  if (now - last > 1s) {
    last = steady_clock::now();
    if (position.player) wTime++;
    else bTime++;
  }
}
```

The code checks, wether we are in playing mode (gamestate is set to 1),
and captures the current time point (2).\
If the difference between `now` and `last` is greater than one second (3),
increase the time counter for the player whose turn it is (5-6).
Remember, we have set the `sf::FramerateLimit` to 10, so this code will be
called 10 times per second, and we cannot simply increase the time counters for each pass.

In order to actually draw the timer, we have this code inside the game
loop (directly after the last code snippet):

``` {.cpp .numberLines}
string timer = "";
if (position.player) {
  window.draw(wActive);
  timer = getTime(wTime);
  wTimer.setString(timer);
} else {
  window.draw(bActive);
  timer = getTime(bTime);
  bTimer.setString(timer);
}
if (position.checkmate.first != -1) {
  if (position.player) {
    bActive.setFillColor(sf::Color(200, 0, 0));
    window.draw(bActive);
  } else {
    wActive.setFillColor(sf::Color(200, 0, 0));
    window.draw(wActive);
  }
}
window.draw(wTimer);
window.draw(bTimer);
```

This code also draws the indicator for the active player (the white or
black circle in front of the timer) in lines 3 and 7. If the app has
detected a checkmate (11), the active player's indicator is set to a red
color (13, 16).

For that to work, we need to have the respective `sf::Sprite`s to be
defined (before entering the game loop):

``` {.cpp .numberLines}
// set timer
sf::Text wTimer;
wTimer.setFont(noto);
wTimer.setCharacterSize(24);
wTimer.setStyle(sf::Text::Bold);
wTimer.setFillColor(sf::Color::White);
wTimer.setString("00:00:00");
wTimer.setPosition(690.f, 10.f);

sf::Text bTimer = wTimer;
bTimer.setFillColor(sf::Color::Black);
bTimer.setPosition(690.f, 40.f);

// marker for aktive player
sf::CircleShape wActive(10.f);
wActive.setPosition(650.f, 15.f);
wActive.setFillColor(sf::Color::White);

sf::CircleShape bActive(10.f);
bActive.setPosition(650.f, 45.f);
bActive.setFillColor(sf::Color::Black);

```

In order to display text with SFML, we have to set a font to the
`sf::Text` sprite (3), and we also must have a font already in place
(just before the last snippet):

``` {.cpp .numberLines}
sf::Font noto;
if (!noto.loadFromFile("../img/NotoMono-Regular.ttf")) {
  cout << "failed to load the font\n";
  return 1;
}
```

I have chosen the *NotoMono-Regular* font, because it is a fixed-space
font and quite compact: it only contains 897 glyphs (the images for
actually drawing each character), in contrast to more than 5,000 glyphs
for fancier fonts.

The last thing missing is the function `getTime()` to convert a time
counter (an unsigned integer value) into a readable time format.
This function is located inside the new `app/display.cpp` implementation
file:

``` {.cpp .numberLines}
string getTime(unsigned t) {
  string result = "";
  unsigned h = 0;
  unsigned m = t / 60;
  unsigned s = t % 60;
  if (m >= 60) {
    h = m / 60;
    m = m % 60;
  }
  if (h < 10) {
    result.append(1, '0');
    result.append(to_string(h));
  } else {
    result.append(to_string(h));
  }
  result.append(1, ':');
  if (m < 10) {
    result.append(1, '0');
    result.append(to_string(m));
  } else {
    result.append(to_string(m));
  }
  result.append(1, ':');
  if (s < 10) {
    result.append(1, '0');
    result.append(to_string(s));
  } else {
    result.append(to_string(s));
  }
  return result;
}
```

The logic is straight forward:\
just divide the total amount of seconds by 60 to get the minutes (4),
and take the remainder of that calculation to get the remaining seconds (5).\
If the resulting minutes are greater than 60 (6), repeat those steps to get
the hours and remaining minutes (7-8).\
The rest of the code (10-29) is just for formatting: if hours,
minutes, and seconds are less than 10, insert the digit `0` at the
appropriate position.

## Material evaluation

*Material evaluation* is the first step in evaluating the players
positions on the board.

Evaluating positions is a complex task, for which one must have a deeper
understanding of several factors, including the pawn structure, control
of the center, space and initiative.
We'll explore the basics of position evaluation in the next chapter,
which will also be the first chapter mainly about *playing* chess.

For now, we are only interested in the first part of this process, which
is straight forward: add up the values of the pieces of each player and
compare them.

For that we need a new function, which needs access to the board; so
it's a natural choice to define it as a member of the type `Position`:

``` {.cpp .numberLines}
void evaluate() {
  pair<int, int> matEval = evaluateBoard(board);
  eval = float(matEval.first) / 100
       - float(matEval.second) / 100;
}
```

The function calls a helper function, which is defined in the
corresponding implementation file:

``` {.cpp .numberLines}
pair<int, int> evaluateBoard(const vector<vector<Piece*>>& bd) {
  int white = 0;
  int black = 0;
  
  for (int row = 0; row < 8; row++) {
    for (int col = 0; col < 8; col++) {
      auto pc = bd[row][col];
      if (pc) {
        pc->isWhite() ? white += pc->getValue()
                      : black += pc->getValue();
      }
    }
  }
  pair<int, int> eval = make_pair(white, black);
  return eval;
}
```

Observe my design choice: I defined all functions, which need a reference
to the board, but will not modify it, as helper functions outside the class
`Position`, and I made the reference to the board `const`{.cpp}.
So the compiler will ensure that indeed no updates to the
board will happen inside those functions.

I also decided to slightly modify the value scheme for pieces of
section [pieces](#subsec:pieces): the queen now has a value of 9, and the
rooks each a value of 5.
To be able to do more accurate calculations later, the values are defined
as *centipawns* by multiplying them with 100.
With that, we'll be able to make all future calculations on the base of integer values,
which should be faster than calculating with `float` representations of the value.

The current implementations of the pieces look like so:

``` {.cpp .numberLines}
class Queen : public Piece {
public:
  ~Queen() {}
  Queen(bool w, int r, int c) :
    type{'Q'}, white{w}, value{900}, row{r}, col{c} {}

  char getType() override { return type; }
  int getValue() override { return value; }
  bool isWhite() override { return white; }
  int getRow() override { return row; }
  int getCol() override { return col; }
  void makeMove(int r, int c) override { row = r; col = c; }
  bool isValid(const vector<vector<Piece*>>& bd, int r, int c) override;

private:
  char type;
  bool white;
  int value;
  int row;
  int col;
};
```

Now we can use the evaluation function inside the game loop to actually
draw the *evaluation meter*:

``` {.cpp .numberLines}
if (moved) {
  if (position.mvCount % 2 == 0) {
    position.evaluate();
    eval = position.eval;
    if (eval > 10.0) {
      eval = 10.f;
      mb[2].color = sf::Color(200, 0, 0, 200);
      mb[4].color = sf::Color(200, 0, 0, 200);
      mb[5].color = sf::Color(200, 0, 0, 200);
    }
    if (eval < -10.0) {
      eval = -10.f;
      mb[0].color = sf::Color(200, 0, 0, 200);
      mb[1].color = sf::Color(200, 0, 0, 200);
      mb[3].color = sf::Color(200, 0, 0, 200);
    }
    mi[2].position = sf::Vector2f(750.f + eval*6.f, 80.f);
    mi[3].position = sf::Vector2f(750.f + eval*6.f, 100.f);
    if (eval > 0) {
      mi[2].color = sf::Color::White;
      mi[3].color = sf::Color::White;
    } else {
      mi[2].color = sf::Color::Black;
      mi[3].color = sf::Color::Black;
    }
  }
}
```

When a move is complete, i.e. both players have moved (2), evaluate the
material (3).\
Then, set the local variable `eval` to the calculated value (4);
we are doing so, because we don't want to tinker with the
member variables of `Position` from outside the class (remember my
caveat for this design decision).\
For display purposes, we restrict that value to the range $[-10, 10]$,
and set the background color of the evaluation meter to red, if the
value is outside that range (5-16).\
Finally, we set the position and the color of the evaluation indicator
(17-24). We'll see soon, how this works.

You may have noticed that this code will only be executed, if a move was
made (1).
Remember, this code is placed inside the game loop, and we don't want to
evaluate the board for every update of the display.
So I modified the `Position::makeMove()`{.cpp} function to report success,
and call it like so: `moved = position.makeMove(touched, to)`{.cpp}:

``` {.cpp .numberLines}
bool makeMove(pair<int, int> td, pair<int, int> to) {
  auto pcf = board[td.first][td.second];
  auto pct = board[to.first][to.second];

  if (!pcf) {
    info = "no piece selected";
    return false;
  }
  if (pcf->isWhite() != player) {
    info = "it's not your turn";
    return false;
  }
  // --- snip ---
}
```

The background of the evaluation meter (`mb`) is defined as an SFML
primitive (a set of two unconnected triangles), and the evaluation
indicator (`mi`) as a set of two unconnected vertical lines:

``` {.cpp .numberLines}
sf::VertexArray mb(sf::Triangles, 6);
mb[0].position = sf::Vector2f(690.f, 80.f);
mb[1].position = sf::Vector2f(690.f, 100.f);
mb[2].position = sf::Vector2f(810.f, 100.f);
mb[3].position = sf::Vector2f(690.f, 80.f);
mb[4].position = sf::Vector2f(810.f, 80.f);
mb[5].position = sf::Vector2f(810.f, 100.f);
mb[0].color = sf::Color::White;
mb[1].color = sf::Color::White;
mb[2].color = sf::Color::Black;
mb[3].color = sf::Color::White;
mb[4].color = sf::Color::Black;
mb[5].color = sf::Color::Black;

sf::VertexArray mi(sf::Lines, 4);
mi[0].position = sf::Vector2f(750.f, 80.f);
mi[1].position = sf::Vector2f(750.f, 100.f);
mi[2].position = sf::Vector2f(750.f, 80.f);
mi[3].position = sf::Vector2f(750.f, 100.f);
mi[0].color = sf::Color::Green;
mi[1].color = sf::Color::Green;
mi[2].color = sf::Color::Black;
mi[3].color = sf::Color::Black;
```

I've set the color of the triangle points to black and white, such that
the meter's background color is interpolated to fill the background with
a gradient from white to black.
The green line of the indicator shows the neutral position
(the evaluation value 0) and it's static.
The black line ($mi[2]$ and $mi[3]$) is the actual indicator, which we have
updated when processing a move.

With this setup, the indicator will move to the right when the
evaluation value is greater than zero (i.e. white has an advantage), and
move to the left when black has an advantage.
This may seem counter-intuitive at first, but once you get used to it,
you'll catch the evaluation at a glance.

## Captured pieces

For displaying the captured pieces of each player, we only need two
background areas:

``` {.cpp .numberLines}
sf::RectangleShape bcw(sf::Vector2f(210.f, 20.f));
bcw.setFillColor(sf::Color(30, 30, 30, 200));
bcw.setPosition(650.f, 115.f);
sf::RectangleShape bcb = bcw;
bcb.setFillColor(sf::Color(220, 220, 220));
bcb.move(0.f, 30.f);
```

and a way to draw miniature versions of the pieces figures onto those:

``` {.cpp .numberLines}
window.draw(bcw);
window.draw(bcb);
int wc = -1;
int bc = -1;
for (auto piece : position.captured) {
  sf::Sprite cp;
  switch (piece->getType()) {
  case 'K':
    piece->isWhite() ? cp = wk : cp = bk;
    break;
  case 'Q':
    piece->isWhite() ? cp = wq : cp = bq;
    break;
  case 'R':
    piece->isWhite() ? cp = wr : cp = br;
    break;
  case 'B':
    piece->isWhite() ? cp = wb : cp = bb;
    break;
  case 'N':
    piece->isWhite() ? cp = wn : cp = bn;
    break;
  case 'P':
    piece->isWhite() ? cp = wp : cp = bp;
    break;
  }
  cp.scale(0.3f, 0.3f);
  if (piece->isWhite()) {
    wc++;
    cp.setPosition(650.f + wc*15.f, 115.f);
  } else {
    bc++;
    cp.setPosition(650.f + bc*15.f, 145.f);
  }
  window.draw(cp);
}
```

Getting the captured pieces is easy, as we had already stored them in a
separate data structure `captured` within the `position` object, while
making a move:

``` {.cpp .numberLines}
// valid move?
if (pcf->isValid(board, to.first, to.second)) {
  // can capture?
  if (pct && pct->isWhite() != pcf->isWhite()) {
    cap = true;
    captured.push_back(pct);
  } else if (pct) { // same color
    info = "illegal move";
    return false;
  }
  // --- snip ---
}
```

I've rejected the idea of storing the captures pieces in a linked list;
instead, I'm using a vector of pieces: `vector<Piece*>`.
We can easily use a vector as a *stack* (a *LIFO* data structure) by using its
member functions `push_back()` and `pop_back()` in a very performant way.
Additionally, vectors provide an iterator, which we can use in the
*range-for* statement `for (auto piece : position.captured)`, without
actually *popping* the elements.

## Move indicator

The *move indicator* is implemented with a similar pattern:

``` {.cpp .numberLines}
// current move background
sf::RectangleShape mvb(sf::Vector2f(120.f, 20.f));
mvb.setFillColor(sf::Color(200, 200, 0, 200));
mvb.setPosition(690.f, 190.f);

// current move indicator
sf::Text mvi;
mvi.setFont(noto);
mvi.setCharacterSize(16);
mvi.setFillColor(sf::Color::Black);
mvi.setPosition(720.f, 190.f);
```

As with the evaluation meter, we only update the move indicator when a
move was made:

``` {.cpp .numberLines}
if (moved) {
  // --- snip ---
  position.mvCount > 0 ? mvi.setString(position.moves.back())
                       : mvi.setString("");
  position.checked ? mvb.setFillColor(sf::Color(200, 100, 0, 200))
                   : mvb.setFillColor(sf::Color(200, 200, 0, 200));
  position.checked = false;
  position.info.clear();
  itt.setString("");
  // --- snip ---
}
```

Additionally, if the `position.checked` flag was set when making
the move, we set the background color of the move indicator to orange.

## Moves history

The next feature of our GUI is the display of the complete moves history
for a game, but we also want to persistently store the history in a text
file.
This is done with the following code:

``` {.cpp .numberLines}
if (moved) {
  // --- snip ---
  if (position.mvCount % 2 == 0) {
    game << position.moves.back() << "\n";
    history += position.moves.back();
    history += "\n";
  } else {
    game << (position.mvCount / 2) + 1 << ". "
         << position.moves.back() << " ";
    history += to_string((position.mvCount / 2) + 1);
    history += ". ";
    history += position.moves.back();
    history += " ";
  }
  // trim history string
  if (position.mvCount > 52 && position.mvCount % 2 == 1) { 
    auto pos = history.find("\n") + 1;
    history = history.substr(pos, history.size());
  }
  hist.setString(history);
  // --- snip ---
}
```

If it was a black move (3), write the move in algebraic notation to the
file-stream `game` and add a newline (4).
Also add it to the `history` string (5-6).

If it was a white move (7), write it to the file-stream as well (9), but
only after prepending it with the current move number (8). Also add the
move to the history string (10-13).

If there are more than 52 half-moves (i.e. 26 complete moves) in the
history (16), we trim the history string from its beginning (17-18), as
there's only space for 26 moves in the history display area.
But, we'll never trim the history of the file stream, so there's no
action needed in this case.

In order for this to work, we need to have the file-stream and the
history field in place:

``` {.cpp .numberLines}
#include <fstream>

string lastGame = "../games/last.txt";
std::fstream game;

// moves history
sf::RectangleShape hisb(sf::Vector2f(180.f, 380.f));
hisb.setFillColor(sf::Color::White);
hisb.setPosition(660.f, 220.f);
sf::Text hist;
hist.setFont(noto);
hist.setCharacterSize(12);
hist.setFillColor(sf::Color::Black);
hist.setPosition(670.f, 230.f);
```

We're using an `fstream` to be able to read from and write to that stream.
But that's not enough: we also need to make sure that the game
file actually exists, and if not, to create it:

``` {.cpp .numberLines}
game.open(lastGame, std::ios::trunc);
if (!game.is_open()) {
  game.clear();
  game.open(lastGame, std::ios::out); // create file
  game.close();
  game.open(lastGame);
}
```

If the file already exists, open it in truncation mode, so that all
contents will be cleared (1).
If not (2), re-open the file in output mode to create the file (4),
close and open it again in write mode (5-6).

We must not forget do synchronize the file stream with the actual file
and to close the stream whenever a game is finished:

``` {.cpp .numberLines}
if (game.is_open()) {
  game.flush();
  game.close();
}
```

This is currently done in two cases: when the main window is closed
(inside the event loop), and when a checkmate is detected (at the very
end of the game loop).

## Keyboard input

### Splash Screen

First, we're going to add a splash screen to our GUI to be able to start
and end a game properly:

``` {.cpp .numberLines}
sf::RectangleShape bsplash(sf::Vector2f(280.f, 90.f));
bsplash.setFillColor(sf::Color(30, 30, 30, 200));
bsplash.setPosition(155.f, 235.f);
sf::Text welcome;
welcome.setFont(noto);
welcome.setCharacterSize(16);
welcome.setFillColor(sf::Color(220, 0, 0));
welcome.setStyle(sf::Text::Bold);
welcome.setPosition(170.f, 240.f);
welcome.setString("Welcome to ThinkChess++");
sf::Text spt;
spt.setFont(noto);
spt.setCharacterSize(16);
spt.setFillColor(sf::Color(0, 220, 0));
spt.setPosition(210.f, 270.f);
spt.setString("Start new game <S>\nLoad last game <L>");
```

Remember, we initialize the main position object like so:
`Position position(0)`{.cpp}.\
Having the gamestate initially set to 0, allows us to distinguish
between the game modes and to draw the display, when starting a new
game, like so:

``` {.cpp .numberLines}
// inside game loop
if (position.gamestate == 0) {
  window.draw(bsplash);
  window.draw(spt);
  if (position.checkmate.first != -1) {
    string restart;
    if (position.player) restart = "      White ";
    else restart = "      Black ";
    restart.append("wins!");
    welcome.setString(restart);
  }
  window.draw(welcome);
}
```

This also allows us to distinguish between modes inside the event loop,
and the player is able to start a new game in splash mode:

``` {.cpp .numberLines}
// inside event loop
if (position.gamestate == 0) {
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
    position = Position(1);
    resetBoard(position);
    game.open(lastGame, std::ios::trunc);
    if (!game.is_open()) {
      game.clear();
      game.open(lastGame, std::ios::out); // create file
      game.close();
      game.open(lastGame);
    }
    bActive.setFillColor(sf::Color::Black);
    wActive.setFillColor(sf::Color::White);
    mi[2].position = sf::Vector2f(750.f - position.eval*3.f, 80.f);
    mi[3].position = sf::Vector2f(750.f - position.eval*3.f, 100.f);
    mvb.setFillColor(sf::Color(200, 200, 0, 200));
    mvi.setString("");
    wTime = 0;
    bTime = 0;
    last = chrono::steady_clock::now();
    bTimer.setString("00:00:00");
    history.clear();
    hist.setString(history);
  }
}
```

Here, we read input from the keyboard (3) and if the key 'S' is pressed,
the position object is re-created with gamestate 1 (4), and the board is
reset (5).\
We also re-load the game file (6-12, which we have seen in the last
section), and reset the active players indicators (13-14).\
The evaluation meter is also reset (15-17) and the move indicator
cleared (17-18).\
Then, reset the thinking time for both players (19-20), start the timer
(21) and reset the timers display (22). Finally, clear the moves history
(23-24).

All this is necessary because of this: we use the splash screen not only
when starting the app, but also whenever a checkmate during a game is
detected and the game is ended:

``` {.cpp .numberLines}
// last statement in game loop
if (position.checkmate.first != -1) {
  position.gamestate = 0;
  if (game.is_open()) {
    if (position.player) {
      game << "\n";
      game << "1-0\n";
    }
    else game << "0-1\n";
    game.flush();
    game.close();
  }
}
```

### Draw and resign

In section [stalemate](#subsec:stale) we learned that our app will not
discover a *stalemate*.
To make up for this, each player gets the opportunity to
offer a draw, after a move has made.

``` {.cpp .numberLines}
// inside event loop
if (position.gamestate == 1) {
  // draw offer
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) && !draw) {
    draw = true;
    itt.setString("Accept a draw? (Y/N)");
  }
  // respond to draw offer
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Y) && draw) {
    position.gamestate = 0;
    draw = false;
    if (!position.player) game << "\n";
    game << "1/2-1/2\n";
    game.flush();
    game.close();
    welcome.setString("     It's a draw!");
  }
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::N) && draw) {
    itt.setString("Draw offer declined.");
    draw = false;
  }
  // --- snip ---
}
```

If the D-key is pressed during the game (4), a corresponding message is
displayed (6).\
Now the other player has to decide if she's accepting the draw.
If so, she'll respond with 'Y' (9), and the game is ended (10) and the result
is recorded (12-16).\
Otherwise, if the response is 'N' (18), the draw offer is declined (19)
and the player, who made the offer, is still on turn.

Observe that the app will respond to the keys 'Y' and 'N' only if a draw
was actually offered, in order to prevent unintentional draw (9, 18).

Similar rules are in place, when a player wants to resign:

``` {.cpp .numberLines}
if (sf::Keyboard::isKeyPressed(sf::Keyboard::G) && !giveUp) {
  giveUp = true;
  itt.setString("Sure to give up? (Y/N)");
}
if (sf::Keyboard::isKeyPressed(sf::Keyboard::Y) && giveUp) {
  position.gamestate = 0;
  giveUp = false;
  if (position.player) {
    game << "0-1\n";
    welcome.setString("    White gives up!\n");
  } else {
    game << "\n";
    game << "1-0\n";
    welcome.setString("    Black gives up!\n");
  }
  game.flush();
  game.close();
}
if (sf::Keyboard::isKeyPressed(sf::Keyboard::N) && giveUp) {
  giveUp = false;
  itt.setString("Okay, move on.");
}
```

But this time, the player who made the request is expected to confirm,
again in order to prevent unintentional resign.

For both cases, draw and resign, the app will behave like this: if no
answer is given to the request, the player whose turn it is, can proceed
with a regular move, and the request is declined.

### Take back a move

I wanted to add another feature for smoothing the game experience:
while playing a regular game, each player should be able to take back the
last move unless the other player has moved.
This does not comply with the original rules, but since you can't argue with
an app, I decided to add it anyway.
The code for triggering a take-back is located inside the event loop,
just as the triggers for *draw* and *resign* from the last section:

``` {.cpp .numberLines}
// inside event loop && gamestate == 1
if (sf::Keyboard::isKeyPressed(sf::Keyboard::T) && !takeback) {
  takeback = true;
  string lastMove = position.moves.back();
  int sz = lastMove.size();
  moved = position.takeBackMove();
  if (moved) {
    if (position.mvCount > 0) {
      string newLast = position.moves.back();
      mvi.setString(newLast);
      newLast.back() == '+'
          ? mvb.setFillColor(sf::Color(200, 100, 0, 200))
          : mvb.setFillColor(sf::Color(200, 200, 0, 200));
    } else {
      mvi.setString("");
    }
    if (position.mvCount % 2 == 0) { // last move was white
      if (position.mvCount >= 199) sz += 6;
      else if (position.mvCount >= 19) sz += 5;
      else sz += 4;
    } else sz += 1; // last move was black
    game.seekp(-sz, std::ios_base::end);
    history = history.substr(0, history.size() - sz);
    hist.setString(history);
    moved = false;
  }
}
```

When a player hits the T-key (2), get the last move and determine the
length of that string (4-5).\
Then call the new function `takeBackMove()` and capture its success (6);
this is the main task for this feature.

If the take-back was successful (7), we have to update the game history,
as the move has already been recorded (8-25).\
If the last move was not the very first move (8), set the move indicator
to that last move (9-10) and set the background of the move indicator to
orange if the last move gave check (11-13).\
If the last move was white (17), calculate the offset for the new
writing position (18-20), otherwise (black move) set the offset to the
length of the `lastMove`-string and add 1 for a single whitespace (21).
This is necessary, because we have added the move number in front of
every white move, which is increasing with every move.\
Finally, set the writing position for the file-stream to the calculated
offset (22) and right-trim the history string (23).

With that in place, let's have a look on the new `takeBackMove`
function:

``` {.cpp .numberLines}
bool takeBackMove() {
  string move;
  if (moves.size() > 0) {
    move = moves.back();
    moves.pop_back();
    mvCount--;
  } else return false;
  vector<int> coords = parseMove(move);
  auto pc = board[coords[4]][coords[3]];
  // promotion
  if (move.back() == 'Q' || move[move.size()-2] == 'Q') {
    bool white = coords[4] == 0;
    pc = new Pawn(white, coords[1], coords[0]);
  }
  board[coords[1]][coords[0]] = pc;
  pc->makeMove(coords[1], coords[0]);
  if (coords[2] == 0) { // normal move
    board[coords[4]][coords[3]] = nullptr;
  } else { // captured
    auto cp = captured.back();
    board[coords[4]][coords[3]] = cp;
    captured.pop_back();
  }
  player = !player;
  return true;
}
```

If the last move was the first move, do nothing and return failure (7),
otherwise get the move's algebraic notation string (4) and delete it
from the moves vector (5-6).\
Then get the coordinates of the last move by parsing its notation string
(8), and get the piece of the target coordinates from the last move (9).\
If the last move was a *promotion* (queening), replace that piece with a
new *Pawn* (11-14).\
Then set the piece to the starting coordinates from the last move
(15-16).

If the move was a 'normal' move, delete the piece from the target
position (17-18); otherwise (if it was a capture) place the last
captured piece on the target coordinates and delete it from the vector
of captured pieces (19-23).

The `parseMove` function from line 8 and its helper functions
`fileToCol` and `rankToRow` are implemented like so:

``` {.cpp .numberLines}
vector<int> parseMove(string move) {
  vector<int> coords(5, 0);
  if (isupper(move[0])) move.erase(0, 1);
  for (int i = 0; i < 5; i++) {
    if (i == 0 || i == 3) { // file
      coords[i] = fileToCol(move[i]);
    } else if (i == 1 || i == 4) { // rank
      coords[i] = rankToRow(move[i]);
    } else { // captured?
      move[i] == 'x' ? coords[i] = 1 : coords[i] = 0;
    }
  }
  return coords;
}

// convert files to colums
int fileToCol(char file) {
  int col = file - 97;
  return col;
}

// convert ranks to rows
int rankToRow(char rank) {
  int row = 56 - rank;
  return row;
}
```

The `parseMove` function converts the algebraic notation of the last
move to a vector of board coordinates.
If a piece was captured with the last move, we set the according flag
on position 2 of that vector.
The helper functions just convert the characters of the move string to
the according integer values.

This logic works fine for regular moves and promotion, but unfortunately
it does not work for *castling*, since we have no coordinates for this
move in the notation string (either `0-0` for kingside castling or
`0-0-0` for queenside castling).\
So we have to solve this the hard way by undoing the castling manually.
The following code snippet is placed inside the `takeBackMove` function,
just after the first test:

``` {.cpp .numberLines}
bool takeBackMove() {
  // --- snip ---
  if (move.back() == '0' || move[move.size()-2] == '0') {
    if (move.back() == '+') move.pop_back();
    char form;
    move == "0-0" ? form = 'K' : form = 'Q';
    bool white = (mvCount % 2 == 0);
    if (white && form == 'K') {
      board[7][4] = board[7][6];
      board[7][6] = nullptr;
      auto king = board[7][4];
      king->makeMove(7, 4);
      board[7][7] = board[7][5];
      board[7][5] = nullptr;
      auto rook = board[7][7];
      rook->makeMove(7, 7);
      castled == 1 ? castled = 0 : castled = 2;
    }
    if (white && form == 'Q') {
      board[7][4] = board[7][2];
      board[7][2] = nullptr;
      auto king = board[7][4];
      king->makeMove(7, 4);
      board[7][0] = board[7][3];
      board[7][3] = nullptr;
      auto rook = board[7][0];
      rook->makeMove(7, 0);
      castled == 1 ? castled = 0 : castled = 2;
    }
    if (!white && form == 'K') {
      board[0][4] = board[0][6];
      board[0][6] = nullptr;
      auto king = board[0][4];
      king->makeMove(0, 4);
      board[0][7] = board[0][5];
      board[0][5] = nullptr;
      auto rook = board[0][7];
      rook->makeMove(0, 7);
      castled == 2 ? castled = 0 : castled = 1;
    }
    if (!white && form == 'Q') {
      board[0][4] = board[0][2];
      board[0][2] = nullptr;
      auto king = board[0][4];
      king->makeMove(0, 4);
      board[0][0] = board[0][3];
      board[0][3] = nullptr;
      auto rook = board[0][0];
      rook->makeMove(0, 0);
      castled == 2 ? castled = 0 : castled = 1;
    }
    player = !player;
    return true;
  }
  // --- snip ---
}
```

## Special moves revisited

We have two open task from
section [special moves](#sec:specmov):

-   implement *en passant* for pawns
-   verify that neither the king nor the rooks have moved for
    *castling*,

which we'll take care of in this section.

### En passant

In order to show *en passant* as a valid move, I added this code to the
`Pawn::isValid()`{.cpp} member function:

``` {.cpp .numberLines}
bool Pawn::isValid(const vector<vector<Piece*>>& bd, int r, int c) {
  bool valid = false;
  auto pc = bd[r][c];
  if (white) {
    // en passant
    if (row == 3 && r == 2 && abs(col-c) == 1) {
      if (!pc) {
        auto pawn = bd[row][c];
        if (pawn && pawn->getType()=='P' && pawn->isWhite() != white) {
          valid = true;
        }
      }
    }
  } else { // black
    // en passant
    if (row == 4 && r == 5 && abs(col-c) == 1) {
      if (!pc) {
        auto pawn = bd[row][c];
        if (pawn && pawn->getType()=='P' && pawn->isWhite() != white) {
          valid = true;
        }
      }
    }
  }
  return valid;
}
```

If a white pawn is placed on the 5th rank, respectively a black pawn on
the 4th rank, and it wants to make a capture move (6, 16), check wether
it's a regular capture (7, 17).\
If there's no piece at the target coordinates, check wether there's a
pawn of the other color placed right next to it (9, 19).
If so, mark the move as valid (10, 20).

Observe, that *en passant* will be shown as valid, even if the opponents
last move wasn't an initial pawn move.
There's no way of checking this inside the `isValid` function, since we
don't have access to the main position object.
I could have changed that, but this would also mean to change all the other
`isValid` functions as well, and I didn't want to do that.

So, we're going to add the test for the last pawn move to the
`Position::makeMove`{.cpp} function:

``` {.cpp .numberLines}
bool makeMove(pair<int, int> from, pair<int, int> to) {
  auto pcf = board[from.first][from.second];
  auto pct = board[to.first][to.second];
  bool cap = false;
  bool passant = false;

  // valid move?
  if (pcf->isValid(board, to.first, to.second)) {
    // can capture?
    if (pct && pct->isWhite() != pcf->isWhite()) {
      cap = true;
      captured.push_back(pct);
    } else if (pct) { // same color
      info = "illegal move";
      return false;
    }
    // en passant
    if (pcf && pcf->getType() == 'P') {
      if (pcf->isWhite() && pcf->getRow() == 3) { // white
        if (!pct) {
          auto pawn = board[pcf->getRow()][to.second];
          if (pawn && pawn->getType() == 'P' &&
              pawn->isWhite() != player)
          {
            string lastMove = moves.back();
            pair<int,int> epTo=make_pair(pawn->getRow(),pawn->getCol());
            auto epPc = new Pawn(!player, 1, to.second);
            string ep = convertFromBoard(false, epPc, epTo);
            delete epPc;
            if (ep != lastMove) {
              info = "illegal move";
              return false;
            } else {
              passant = true;
              cap = true;
              captured.push_back(pawn);
              board[pcf->getRow()][to.second] = nullptr;
            }
          }
        }
      } else if (!pcf->isWhite() && pcf->getRow() == 4) { // black
        if (!pct) {
          auto pawn = board[pcf->getRow()][to.second];
          if (pawn && pawn->getType() == 'P' &&
              pawn->isWhite() != player)
          {
            string lastMove = moves.back();
            pair<int,int> epTo=make_pair(pawn->getRow(),pawn->getCol());
            auto epPc = new Pawn(!player, 6, to.second);
            string ep = convertFromBoard(false, epPc, epTo);
            delete epPc;
            if (ep != lastMove) {
              info = "illegal move";
              return false;
            } else {
              passant = true;
              cap = true;
              captured.push_back(pawn);
              board[pcf->getRow()][to.second] = nullptr;
            }
          }
        }
      }
    } // end en passant
    string move = convertFromBoard(cap, pcf, to);
    if (passant) move.append("ep");
    // --- snip ---
  }
}
```

Observe, that we were able to put the new logic inside the test for
valid moves (8), since the updated `Pawn::isValid` function will
now report the move as generally valid.

If the piece to move is a pawn (18), and if it's placed on a rank
allowing an *en passant* (19), test wether there is a piece placed at
the target field (20).\
If not, check wether there's a pawn of the opposite color placed right
next to the pawn (21-23).
This is redundant to the logic of the `isValid` function, but that doesn't hurt,
as it makes sure that we're dealing only with an *en passant move*.

Now we can actually check for the last move (26-37), and if it was the
initial move for the other pawn, mark the move as valid (34-37).
Otherwise abort the move (30-32).

Repeat that procedure for black (41-59), and if it is an *en passant*
move, add `ep` to the moves notation in order to indicate that special
move (66).

The only thing left to do is to take care about taking back an *en
passant* move:

``` {.cpp .numberLines}
bool takeBackMove() {
  string move;
  if (moves.size() > 0) {
    move = moves.back();
    moves.pop_back();
    mvCount--;
  } else return false;
  // en passant
  if (move.back() == 'p' && move[move.size()-2] == 'e') {
    move.pop_back();
    move.pop_back();
    vector<int> coords = parseMove(move);
    auto pc = board[coords[4]][coords[3]];
    board[coords[4]][coords[3]] = nullptr;
    board[coords[1]][coords[0]] = pc;
    pc->makeMove(coords[1], coords[0]);
    auto cp = captured.back();
    captured.pop_back();
    bool white = (mvCount % 2 == 0);
    if (white) {
      board[coords[4]+1][coords[3]] = cp;
    } else {
      board[coords[4]-1][coords[3]] = cp;
    }
    player = !player;
    return true;
  }
  // --- snip ---
}
```

If the last move was an *en passant* (9), remove the last two characters
from it, in order to get the coordinates for a regular pawn move (12).
Then, get the piece of the target position of the last move (13), delete
it from that position (14), and set it to its original position before
the move (15-16).

Then we have to deal with the captured pawn (17-24):\
first, retrieve it from the captured pieces (17-18).
Then check wether the last move was white (19).\
If so, move the captured piece one rank down and place it back there (21),
otherwise move it one rank up and place it back there (23).
Finally, complete the move and report success (25-26).

### Castling

In order to make testing for *King* and *Rook* moves possible, I moved
the helper function `castling` from its compilation unit
`app/position.cpp` to its header file at `include/position.hpp` and made
it a member function of the type `Position`.
This also fits my design choice of defining all functions with a non
constant reference to the board as member functions.

With that, I was able to add the following code to the `castling` function:

``` {.cpp .numberLines}
char castling(Piece* king, pair<int, int> to) {
  if (check(board, king->isWhite())) return 'N'; // king is in check
  string history;
  for (auto mv : moves) {
    history += mv;
  }
  if (king->isWhite()) { // white
    auto pos = history.find("Ke1");
    if (pos != std::string::npos) return 'N'; // king has moved
    if (king->getRow() == 7 && king->getCol() == 4) {
      if (to.first == 7 && to.second == 6) { // kingside
        auto rook = board[7][7];
        if (!rook || rook->getType() != 'R'
                  || rook->isWhite() != king->isWhite())
        {
          return 'N'; // no rook
        } else {
          auto pos = history.find("Rh1");
          if (pos != std::string::npos) return 'N'; // rook has moved
        }
        // --- snip ---
      }
    }
  }
}
```

I've created a history string (3), and filled it with all moves so far
(4-6).
Then, for the white king, the history is searched for the substring `Ke1` (8).
If that substring is found in the history, the king has already moved,
and castling is denied (9).

We use the same logic to test for moves of the kingside rook (13-20),
and only need to apply it for kingside castling and the black king as
well, and we are done here.
