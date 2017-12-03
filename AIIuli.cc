#include "Player.hh"


/**
 * Write the name of your player and save this file
 * with the same name and .cc extension.
 */
#define PLAYER_NAME Iuli


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

   Pos troba_casella_profitable(set<Pos>& posicionsVisitades, Pos posActual, Dir& dirRetornada, int orcid) {
      std::cout << "Casella profitable orcid:" << orcid << endl;
      Pos retPos;
      dirRetornada = NONE;
      if ((cell(posActual).type == CITY or cell(posActual).type == PATH) and unit(cell(posActual).unit_id).player != me()) {
         cout << "Bucle found city sau path orcid:" << orcid << endl;
         posicionsVisitades.insert(posActual);
         retPos = posActual;
      } else if (not pos_ok(posActual) or cell(posActual).type == WATER or posicionsVisitades.find(posActual) != posicionsVisitades.end()) {
         cout << "Unusable position orcid:" << orcid << endl;
         posicionsVisitades.insert(posActual);
         retPos = Pos(-1, -1);
      } else {
         posicionsVisitades.insert(posActual);
         bool posicioTrobada = false;
         for (int d = 0; d != DIR_SIZE and not posicioTrobada; ++d) {
            cout << "Bucle recursivitat orcid:" << orcid << " dir:" << d << endl;
            Dir dir = Dir(d);
            Pos npos = posActual + dir;
            Dir dummyDir;
            retPos = troba_casella_profitable(posicionsVisitades, npos, dummyDir, orcid);
            dirRetornada = dir;
            if (pos_ok(retPos))
               posicioTrobada = true;
         }
      }
      return retPos;
   }

   // Moves ork with identifier id.
   void move(int id) {
     Dir dir = Dir(RIGHT);
     //Pos npos = unit(id).pos + dir;
     set<Pos> posicions;
     Unit u = unit(id);
     Pos pos = u.pos;
     Dir dirEscollida;
     troba_casella_profitable(posicions, pos, dirEscollida, id);
     execute(Command(id, dirEscollida));

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
