#include "Player.hh"


/**
 * Write the name of your player and save this file
 * with the same name and .cc extension.
 */
#define PLAYER_NAME Kami


struct PLAYER_NAME : public Player {

  /**
   * Factory: returns a new instance of this class.
   * Do not modify this function.
   */
  static Player* factory () {
    return new PLAYER_NAME;
  }

  /**
   * Types and attributes for your player can be defined here.
   */

   typedef vector<int> VI;
   typedef vector<VI>  VVI;

   // Stores the location of orks.
   VVI ork_at;

   // Moves ork with identifier id.
   void move(int id) {
     Dir dir = Dir(RIGHT);
     //Pos npos = unit(id).pos + dir;
     execute(Command(id, dir));

     /*
     Unit u = unit(id);
     Pos pos = u.pos;
     // Try to move to a position within the board.
     for (int d = 0; d != DIR_SIZE; ++d) {
       Dir dir = Dir(d);
       Pos npos = pos + dir;
       if (pos_ok(npos) and cell(npos).type != WATER) {
         execute(Command(id, dir));
         return;
       }
     }
     */
   }

  /**
   * Play method, invoked once per each round.
   */
  virtual void play () {
    VI my_orks = orks(me()); // Get the id's of my orks.

    // Process orks in random order.
    VI perm = random_permutation(my_orks.size());
    for (int k = 0; k < int(perm.size()); ++k)
      move(my_orks[perm[k]]);
  }

};


/**
 * Do not modify the following line.
 */
RegisterPlayer(PLAYER_NAME);
