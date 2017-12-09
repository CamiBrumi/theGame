#include "Player.hh"


/**
 * Write the name of your player and save this file
 * with the same name and .cc extension.
 */
#define PLAYER_NAME Kami2


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
   typedef vector<VVI> VVVI; //vector de matrius
   typedef pair<int, Pos> P;

   // Stores the location of orks.
   VVI ork_at;
   const int INF = 1e8;

   typedef pair<int, int> PII;
   typedef vector<PII> VP;
   typedef vector<VP>  VVP;
   VVP path0; //TODO delete this after used
   typedef vector<VVP> VVVP; //vector de matrius de pairs.
   VVVP cities, paths;

   const int dir_contraria [4] = { 2, 3, 0, 1 };
   const char dir_str [4] = { 'B', 'R', 'T', 'L' };

   // Returns true if winning.
   bool winning() {
     for (int pl = 0; pl < nb_players(); ++pl)
       if (pl != me() and total_score(me()) <= total_score(pl))
         return false;
     return true;
   }

   // Moves ork with identifier id.
   void move(int id) {
     Unit u = unit(id);
     Pos posActual = u.pos;
     Dir dir = Dir(path0[posActual.i][posActual.j].second);
     //Pos npos = posActual + dir;
     execute(Command(id, dir));
     cout << dir_str[dir] << endl;
   }

   void dijkstraC(Pos ini, int k) {
     //cout << "HELLOU" << endl;
     priority_queue<P> Q;
     //VE dist(n, INF);
     Q.push(P(0, ini));
     //dist[ini] = 0;
     while (not Q.empty()) {
       //cout << "ESTEM DINS DE LA CUETA" << endl;
       P a = Q.top(); Q.pop();
       int d = -a.first;
       Pos x = a.second;
       if (d == cities[k][x.i][x.j].first) {
         //cout << "ESTEM EN EL PAS if (d == path0[x.i][x.j])" << endl;
         for (int dir = 0; dir != 4; ++dir) { //per a cada direcció / veí
           //cout << "direcció: " << dir << endl;
           Pos y = x + Dir(dir); //posició del veí segons la direcció escollida
           if (pos_ok(y) and cell(y).type != WATER) {
             int c = cost (cell(y).type); //cost de la nova posició
             int d2 = d + c;//nou cost, segons si és herba, desert, bosc, ...
             //cout << "d2 = " << d2 << " , path0[y.i][y.j] = " << path0[y.i][y.j] << endl;
             if (d2 < cities[k][y.i][y.j].first) {
               //cout << "HA ENTRAT EN EL if (d2 < path0[x.i][x.j]) => S'HAURIA DE MODIFICAR LA MATRIU EN EL PUNT " << x.i << " " << x.j << endl;
               cities[k][y.i][y.j].first = d2;
               Q.push(P(-d2, y));
               cities[k][y.i][y.j].second = dir_contraria[dir];
             }
           }
         }
       }
     }
     //for (int x = 0; x < n; ++x) cout << x << ' ' << dist[x] << endl;
   }

   void dijkstraP(Pos ini, int k) {
     //cout << "HELLOU" << endl;
     priority_queue<P> Q;
     //VE dist(n, INF);
     Q.push(P(0, ini));
     //dist[ini] = 0;
     while (not Q.empty()) {
       //cout << "ESTEM DINS DE LA CUETA" << endl;
       P a = Q.top(); Q.pop();
       int d = -a.first;
       Pos x = a.second;
       if (d == paths[k][x.i][x.j].first) {
         //cout << "ESTEM EN EL PAS if (d == path0[x.i][x.j])" << endl;
         for (int dir = 0; dir != 4; ++dir) { //per a cada direcció / veí
           //cout << "direcció: " << dir << endl;
           Pos y = x + Dir(dir); //posició del veí segons la direcció escollida
           if (pos_ok(y) and cell(y).type != WATER) {
             int c = cost (cell(y).type); //cost de la nova posició
             int d2 = d + c;//nou cost, segons si és herba, desert, bosc, ...
             //cout << "d2 = " << d2 << " , path0[y.i][y.j] = " << path0[y.i][y.j] << endl;
             if (d2 < paths[k][x.i][x.j].first) {
               //cout << "HA ENTRAT EN EL if (d2 < path0[x.i][x.j]) => S'HAURIA DE MODIFICAR LA MATRIU EN EL PUNT " << x.i << " " << x.j << endl;
               paths[k][x.i][x.j].first = d2;
               Q.push(P(-d2, y));
               paths[k][x.i][x.j].second = dir_contraria[dir];
             }
           }
         }
       }
     }
     //for (int x = 0; x < n; ++x) cout << x << ' ' << dist[x] << endl;
   }
   /**
    * Play method, invoked once per each round.
    */
    virtual void play () {
      if (round() == 0) {
        cities = VVVP(nb_cities(), VVP(rows(), VP(cols(), PII(INF, -1))));
        paths = VVVP(nb_paths(), VVP(rows(), VP(cols(), PII(INF, -1))));

        for (int k = 0; k < nb_cities(); ++k) {
          bool found = false;
          int i0, j0;
          //path0 = VVP(rows(), VP(cols(), PII(INF, -1))); // a cada posició de la matriu hi ha un pair de cost i direcció
          for (int i = 0; i < rows() and not found; ++i) {
            for (int j = 0; j < cols() and not found; ++j) {
              if (cell(i, j).city_id == k) {
                i0 = i;
                j0 = j;
                found = true;
              }

            }

          }
          cities[k][i0][j0].first = 0;
          Pos p;
          p.i = i0;
          p.j = j0;
          dijkstraC(p, k); //TODO param wether is a city or a path

        }
        for (int k = 0; k < nb_paths(); ++k) {
          bool found = false;
          int i0, j0;
          //path0 = VVP(rows(), VP(cols(), PII(INF, -1))); // a cada posició de la matriu hi ha un pair de cost i direcció
          for (int i = 0; i < rows() and not found; ++i) {
            for (int j = 0; j < cols() and not found; ++j) {
              if (cell(i, j).path_id == k) {
                i0 = i;
                j0 = j;
                found = true;
              }

            }

          }
          paths[k][i0][j0].first = 0;
          Pos p;
          p.i = i0;
          p.j = j0;
          dijkstraP(p, k); //TODO param wether is a city or a path
        }

        for (int i = 0; i < rows(); ++i) {
          for (int j = 0; j < cols(); ++j) {
            if (path0[i][j].second != -1) cout << " " << dir_str[path0[i][j].second];
            else cout << " -";
          }
          cout << endl;
        }
    }
    VI my_orks = orks(me()); // Get the id's of my orks.

    for (int k = 0; k < int(my_orks.size()); ++k) {
      cout << "ORK " << k << " IS MOVING IN DIRECTION: ";
      move(my_orks[k]);
    }
  }

 };


 /**
  * Do not modify the following line.
  */
 RegisterPlayer(PLAYER_NAME);
