# Multiplayer Top-Down Shooter

### Background

The goal of the project was to create a multiplayer networking application that allows players, using different devices, to connect together. The chosen application was a fast-paced competitive top-down shooter game.

### Networking

The networking model is a client-server model. A match-making server is always open and waiting for either a game server or a client to connect to it. Once a game server and at least two clients are connected to it, informations are exchanged from the clients to the servers and vice-versa and thereafter all disconnect from the match-making server. 

When the game begins, the clients and server communicate using both UDP and TCP. UDP is used for information that needs continuous updates (eg. player movement) and TCP is used for actions that are triggered less often (eg. player shooting). Prediction and interpolation is also implemented to smooth possible network faults and make the game expirience more enjoyable.

<img src="https://media.giphy.com/media/Zsc2GFCDQYaIqUtnim/giphy-downsized.gif" width="400">

### The Game

As soon as the players connect, they spawn in different positions around the map. There are two possible attacks:

- The main fast-paced gun (deals 1 damage)

<img src="https://media.giphy.com/media/tIgyJT5OvNeMcSbfS7/giphy.gif" width="400">

- The homing bullet which can be controlled using the mouse (deals 5 damage)

<img src="https://media.giphy.com/media/2LMPom92pbulXNONdg/giphy.gif" width="400">

The goal of the game is to be the last player standing.

### Controls

- Move: WASD

- Shoot normal bullet: Left Mouse Button

- Shoot homing bullet: Right Mouse Button


### The Application

A zipped file containing the executable can be downloaded from the Release page or [here](https://github.com/StylianosZachariou/Multiplayer-Top-Down-Shooter/releases/download/1.0/executable.zip).
