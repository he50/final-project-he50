# Development

---

 - **4/14/20** Wrote final project proposal
   - ~~Figure out which libraries to use~~
   - Box2d and sqlite

 - **4/21/20** Added libaries to CMakeLists
   - Box2d and SQLite
   - Added Leaderboard.cc and Player.cc from snake for score tracking.
   - Added example of simple Box2D implementation from cited source.
   - On mouse click, thin rectangles representing missiles fire with 
   negative gravity.

 - **4/24/20** Refactor Missiles to shoot circles
   - Missiles shoot circles and b2World has no gravity.
   - Add key commands
   - Add player to cinder as well as texture
   - TODO: ~~Add aliens and shield~~
   - TODO: ~~Refactor player class~~
   
 - **4/25/20** Refactor code from space_invaders.cc to my_library
    - Add invader.h and invader.cc to create b2bodies
    - Add player.h and player.cc to create b2bodies
    - Add shield.h and shield.cc to create b2bodies
    - TODO: ~~add shield class~~
    - Put all Add____() functions into draw()
    
 - **4/26/20** Figure out collisions in Box2D
    - try to iterate through contact list
    - TODO: ~~set the setuserdata field for checking~~
    - use erase and remove to get rid of bodies from vector lists
    - Added all voice and animation to game
    
 - **4/27/20** Score Display
    - Implement from Snake, PrintText.
    - Display score and store in variable
    - Added all voice and animation to game
    - Updated README.md
    - TODO: figure out shield and missile interaction
    - TODO: invader missiles and game over screen
    - TODO: figure out lag and reorganization
    
    

