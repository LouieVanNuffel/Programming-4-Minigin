# Pengo
Minigin based engine + pengo game project

Repo link: [GitHub](https://github.com/LouieVanNuffel/Programming-4-Minigin.git)

Student: Louie Van Nuffel - 2DAE10

## Engine Patterns:
Game loop:
- Game loop with start, update, lateupdate, cleanup(destroy)


GameObject:
- All entities are gameobjects with components to add functionality


Components:
- Components were used to add functionality to entities, UI


Subject-Observer Pattern:
- Used pattern where I didn't need constant polling to happen
- Events get broadcasted when a character dies, when a character changes direction, to update the ui of lives and score


Command-Action Bindings:
- Commands bind to an action
- Actions get bound to input or get called by other objects
- Commands are bound to action for both playable characters and the snobee enemies
- For playable characters the actions get bound to keyboard and controller input
- For the enemies the actions get called by logic components


State:
- Animations changed with states
- A gameobject can have an animator component
- You can add animation states to this animator component
- The animator component holds these states, updates them and changes states when called for
- The animation states themselves hold their animation sequence, and updates the textures of the texturecomponent on the object
- The states change when they receive a certain event


Service Locator:
- Service locator is used for the sound system


Sound:
- The sound system works with a queue
- Playing sounds happens on a seperate thread


Singleton:
- CollisionSystem is a singleton that detects collisions for all the registered colliders
- GameState (specific to pengo game) is a singleton that holds the score and the amount of players
- InputManager is a singleton that can be called to check for input and bind input to commands directly
- Time is a singleton that holds the deltatime and fixedtime
- 


Added functionality to gameobject/component:
- Added OnDestroy(), gets called in cleanup after lateupdate when deleting objects marked to be destroyed
- Added OnCollisionEnter(), gets called when gameobject has a collider component attached to it and the collision system detects it collided with something. It passes the other collider component to the function.

