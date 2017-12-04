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
   void cami(Pos u, set<Pos>& posicionsVisitades) {
     set<pair<Pos, int>> dist; //pait vèrtex, distància de u
     VE pare(n);
     queue<Pos> cua;
     cua.push(u);
     pair 
     dist = 0;
     while (not cua.empty()) {
       Pos x = cua.front(); cua.pop();
       for (int d = 0; d < NONE; ++d) {
         Dir dir = Dir(d);
         Pos y = x + dir;
         if (pos_ok(y) and posicionsVisitades.find(y) == posicionsVisitades.end()) { //no hem visitat aquesta casella
           cua.push(y);
           dist[y] = dist[]
         }
       }


       for (int y : G[x])
         if (dist[y] == -1) {
           cua.push(y);
           dist[y] = dist[x] + 1;
           pare[y] = x;
           if (y == v) {
             stack<int> pila;
             while (y != u) {
               pila.push(y);
               y = pare[y];
             }
             cout << u;
             while (not pila.empty()) {
               cout << ' ' << pila.top();
               pila.pop();
             }
             cout << endl;
             return;
           }
         }
     }
     cout << "no" << endl;
   }

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

   }

   // Moves ork with identifier id.
   void move(int id) {
     set<Pos> posicionsVisitades;
     Unit u = unit(id);
     Pos posActual = u.pos;
     Dir dirCasellaProf = NONE;
     Pos posCasellaProf = trobaCasellaProfitable(posicionsVisitades, posActual, dirCasProf);
     Dir dir = Dir(dirCasellaProf);
     Pos npos = posActual + dir;
     execute(Command(id, dir));
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
