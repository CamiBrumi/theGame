#include "Player.hh"


/**
 * Write the name of your player and save this file
 * with the same name and .cc extension.
 */
#define PLAYER_NAME Kami


/**
 * Factory: returns a new instance of this class.
 * Do not modify this function.
 */
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

   //

   // Returns true if winning.
   bool winning() {
     for (int pl = 0; pl < nb_players(); ++pl)
       if (pl != me() and total_score(me()) <= total_score(pl))
         return false;
     return true;
   }

   // u != v
   Dir trobaDireccioFactible(Pos posActual) {
     set<Pos> posicionsVisitades;
     queue<Pos> cuaPos;
//     cuaPos.push(posActual);
     queue<int> cuaDir;
     posicionsVisitades.insert(posActual);
     //cuaDir.push(Dir(NONE));
     // posem les quatre direccions a partir del vèrtex actual a la cua. no ho podem fer al while perquè necessitem tindre en compte la direcció cap a on ens movem inicialment
     for (int d = 0; d < 4; ++d) {
       Dir dir = Dir(d);
       Pos novaPos = posActual + dir;
       posicionsVisitades.insert(novaPos);

       if (pos_ok(novaPos)) {
         //cout << "Ork initially plans to look at position (" << novaPos.i << ", " << novaPos.j << "), going in the direction of " << d << endl;
         cuaPos.push(novaPos);
         cuaDir.push(d);
       }
     }
     int xDir = NONE;
     while (not cuaPos.empty()) {
       Pos x = cuaPos.front(); cuaPos.pop();
       xDir = cuaDir.front(); cuaDir.pop();
       //cout << "Dir poped is " << xDir << endl;
       if ((cell(x).type == CITY and city_owner(cell(x).city_id) != me()) or (cell(x).type == PATH and path_owner(cell(x).path_id) != me())) {
         //cout << "Ork found position (" <<  x.i << ", " << x.j <<  ") to be feasible since it contains a city " << (cell(x).type == CITY) << " or a path " << (cell(x).type == PATH) << " and the direction of growth is " << xDir << endl;
          return Dir(xDir);
       }
       for (int d = 0; d < 4; ++d) {
         Dir dir = Dir(d);
         Pos y = x + dir;
         //cout << "Dir poped is " << xDir << ", old pos " << x <<  "and new pos is " << y <<" which is considered to be pos_ok:" << pos_ok(y) << " and the element has not been visited "  << (posicionsVisitades.find(y) == posicionsVisitades.end()) << endl;
         if (pos_ok(y) and posicionsVisitades.find(y) == posicionsVisitades.end()) { //no hem visitat aquesta casella
           //cout << "Ork contemplates position (" << y.i << ", " <<  y.j << ") " << " and plans it in the direction " << xDir << std::endl;
           posicionsVisitades.insert(y);
           cuaPos.push(y);
           cuaDir.push(xDir); // direcció cap a on ens haurem de moure des del vèrtex inicial
         }
       }
     }
     return Dir(xDir);
   }

   /*
   Pos trobaCasellaProfitable(set<Pos>& posicionsVisitades, Pos posActual, Dir& dirRetornada) {
     Pos retPos;
     Cell tipusCasellaActual = Cell(posActual).type;
     if ((tipusCasellaActual == CITY or tipusCelaActual == PATH) and unit(Cell(posActual).unit_id).player != me()) retPos = posActual;
     else if (tipusCasellaActual == WATER or !pos_ok(posActual) or posicionsVisitades.find(posActual) != posicionsVisitades.end()) {
       retPos = Pos(-1, -1); // si es una posicio visitada, no tenim que anar cap a ella (ens bloquejariem en un bucle infinit). en canvi, si l'iterador és == .end del set => no hem visitat aquesta posicio
       posicionsVisitades.insert(posActual);
     }
     else {
       posicionsVisitades.insert(posActual);
       bool pos_trobada = false;
       for (int d = 0; d != DIR_SIZE and !pos_trobada; ++d) {
         Dir dir = Dir(d);
         Pos npos = posActual + dir;
         posicionsVisitades.insert(npos);
         Dir dummyDir;
         retPos = trobaCasellaProfitable(posicionsVisitades, npos, dummyDir);
         dirRetornada = dir;
         if (pos_ok(retPos)) pos_trobada = true;
       }
     }
     return retPos;

   } */

   // Moves ork with identifier id.
   void move(int id) {
     set<Pos> posicionsVisitades;
     Unit u = unit(id);
     Pos posActual = u.pos;
     //cout << "Debugging ork " << id << " which stands in position " << posActual << endl;
     Dir dir = trobaDireccioFactible(posActual);
     //Pos npos = posActual + dir;
     execute(Command(id, dir));
   }

   /**
    * Play method, invoked once per each round.
    */
    virtual void play () {
      VI my_orks = orks(me()); // Get the id's of my orks.

      // Process orks in random order.
      //VI perm = random_permutation(my_orks.size());
      for (int k = 0; k < int(my_orks.size()); ++k) // for (int k = 0; k < int(perm.size()); ++k)
        move(my_orks[k]); //move(my_orks[perm[k]]);
    }

 };


 /**
  * Do not modify the following line.
  */
 RegisterPlayer(PLAYER_NAME);
