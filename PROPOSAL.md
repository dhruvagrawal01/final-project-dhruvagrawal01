# Space Impact

Author: Dhruv Agrawal

---

## Project Idea:

 For this project, I plan on replicating [Space Impact](https://www.youtube.com/watch?v=Oc5EoSdWZQo&feature=youtu.be),
 a game in older models of Nokia phones. It is similar to [Space Invaders](https://en.wikipedia.org/wiki/Space_Invaders)
 but occurs in horizontal space, thus taking gravity into account. I want to work on this project because growing up,
 I played **Space Invaders** a lot and this seems like a nice twist to it.
 
 ![GamePlay](https://cdn.toucharcade.com/wp-content/uploads/2013/04/mzl.ygudvigj.640x960-75-525x350.jpg)
 
 
## Functionality:
 
### Basic Gameplay:
 
 * A status bar at the top with the number of lives left and the current score.
 * A spaceship representing the user.
 * Enemy bots that kill the user on coming in contact.
 * A control key to shoot ammo
 * Arrow keys to help the user dodge the enemies.
 * A leaderboard when the game ends.
  
### Stretch Features:
 
 * A mini boss that takes more number of bullets to kill and can shoot back.
 * Conversion of enemy bots into collectible coins when killed.
  
 
## Rough Timeline

### Week 1:

 * Come up with the basic code organization.
 * Create classes for the user's spaceship and enemy bots.

### Week 2:
 
 * Set up the game engine with the status bar.
 * Write code to make the user's spaceship shoot ammo.
  
### Week 3:

 * Add enemy bots to the game using a similar logic to the spaceship.
 * Keep track of the user's score and add a leaderboard.


## Libraries:

 * [Cinder](https://libcinder.org): For creating the application, 2D graphics, and most functionality.
 * [Box2D](https://github.com/cinder/Cinder/tree/master/blocks/Box2D): For the game physics.
 * [SQLite](http://www.sqlite.org): For database management.
