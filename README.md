### Bonus

In the objective of making the game more dynamic, the following game rules has been added:

- every 3 turn just after asking players if they want to use their character's ability, they will both be submit to a *dodge challenge* (one after the other)
- the challenge is a mental calculation of a random difficulty (easy, medium, hard)
- the fastest player to input the right answer win the challenge 
- the challenge winner character will dodge a percent of next incoming damage based on the challenge difficulty
- the mental calculations difficulty is set to be more about reaction time than players mental calculation's skill. Consequently it is accessible to all players

### Project conf

solution:

- IDE: visual studio 2019
- language standard: C++ 17
- build with windows console API, won't work on linux

external lib used:

- [jsoncpp](https://github.com/open-source-parsers/jsoncpp)

git branches:

- master: standard game 
- bonus: game with bonus

### Folder structure

- assets: contain characters ascii arts
- data: contain characters and abilities data json files
- lib: external lib installed manually
- src: all c++ source files (convention: *.h* in *public* folders, *.cpp* in *private* folders)

