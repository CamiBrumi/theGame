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
   VVVI cities, paths;
   VVI path0; //TODO delete this after used


   //

   // Returns true if winning.
   bool winning() {
     for (int pl = 0; pl < nb_players(); ++pl)
       if (pl != me() and total_score(me()) <= total_score(pl))
         return false;
     return true;
   }
   /*
   // u != v
   Dir trobaDireccioFactible(Pos posActual) {
     set<Pos> posicionsVisitades;
     queue<Pos> cuaPos;
     cuaPos.push(posActual);
     queue<Dir> cuaDir;
     posicionsVisitades.insert(posActual);
     //cuaDir.push(Dir(NONE));
     // posem les quatre direccions a partir del vèrtex actual a la cua. no ho podem fer al while perquè necessitem tindre en compte la direcció cap a on ens movem inicialment
     for (int d = 1; d < DIR_SIZE; ++d) {
       Dir dir = Dir(d);
       Pos novaPos = posActual + dir;
       posicionsVisitades.insert(novaPos);

       if (pos_ok(novaPos)) {
         cuaPos.push(novaPos);
         cuaDir.push(dir);
       }
     }
     Dir xDir = Dir(NONE);
     while (not cuaPos.empty()) {
       Pos x = cuaPos.front(); cuaPos.pop();
       xDir = cuaDir.front(); cuaDir.pop();
       for (int d = 1; d < DIR_SIZE; ++d) {
         Dir dir = Dir(d);
         Pos y = x + dir;
         if (pos_ok(y) and posicionsVisitades.find(y) == posicionsVisitades.end()) { //no hem visitat aquesta casella
           if ((cell(y).type == CITY and city_owner(cell(y).city_id) != me()) or (cell(y).type == PATH and path_owner(cell(y).path_id) != me())) return xDir;
           posicionsVisitades.insert(y);
           cuaPos.push(y);
           cuaDir.push(xDir); // direcció cap a on ens haurem de moure des del vèrtex inicial
         }
       }
     }
     return xDir;
   }

   // Moves ork with identifier id.
   void move(int id) {
     set<Pos> posicionsVisitades;
     Unit u = unit(id);
     Pos posActual = u.pos;
     Dir dir = trobaDireccioFactible(posActual);
     Pos npos = posActual + dir;
     execute(Command(id, dir));
   }

   void dijkstra() {
     priority_queue<pair<int, Pos>> Q;
     //VE dist(n, INF);
     distCP = VVI(rows(), VI(cols()), INF);
     //afegeixo zeros a ciut i paths
     //Q.push(pair(0, ini)); //pair (cost, posicio) des de dintre del for
     //dist[ini.i][ini.j] = 0;
     while (not Q.empty()) {
       pair<int, Pos> a = Q.top(); Q.pop();
       int d = -a.first; // perque normalment surt el maxim, per aixo ens sortira el minim
       Pos x = a.second;
       if (d == dist[x.i][x.j]) {
         for (int direc = 0; direc < NONE; direc++) {
         //for (P arc : G[x]) {

           int c = arc.first;
           int y = arc.second;
           int d2 = d + c;
           if (d2 < dist[y]) {
             dist[y] = d2;
             Q.push(P(-d2, y));
           }
         }
       }
     }
     for (int x = 0; x < n; ++x) cout << x << ' ' << dist[x] << endl;
   } */

   void dijkstra(Pos ini) {
     cout << "HELLOU" << endl;
     priority_queue<P> Q;
     //VE dist(n, INF);
     Q.push(P(0, ini));
     //dist[ini] = 0;
     while (not Q.empty()) {
       cout << "ESTEM DINS DE LA CUETA" << endl;
       P a = Q.top(); Q.pop();
       int d = -a.first;
       Pos x = a.second;
       if (d == path0[x.i][x.j]) {
         cout << "ESTEM EN EL PAS if (d == path0[x.i][x.j])" << endl;
         for (int dir = 0; dir != 4; ++dir) { //per a cada direcció / veí
           cout << "direcció: " << dir << endl;
           Pos y = x + Dir(dir); //posició del veí segons la direcció escollida
           if (pos_ok(y) and cell(y).type != WATER) {
             int c = cost (cell(y).type); //cost de la nova posició
             int d2 = d + c;//nou cost, segons si és herba, desert, bosc, ...
             cout << "d2 = " << d2 << " , path0[y.i][y.j] = " << path0[y.i][y.j] << endl;
             if (d2 < path0[y.i][y.j]) {
               cout << "HA ENTRAT EN EL if (d2 < path0[x.i][x.j]) => S'HAURIA DE MODIFICAR LA MATRIU EN EL PUNT " << x.i << " " << x.j << endl;
               path0[y.i][y.j] = d2;
               Q.push(P(-d2, y));
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
      VI my_orks = orks(me()); // Get the id's of my orks.
      if (round() == 0) {

        //omplim el el vector de Paths
        int i0, j0;
        path0 = VVI(rows(), VI(cols(), INF));
        for (int i = 0; i < rows(); ++i) {
          for (int j = 0; j < cols(); ++j) {
            if (cell(i, j).path_id == 0) {
              i0 = i;
              j0 = j;
            }
            //cout << " " << path0[i][j];
          }
          //cout << endl;
        }
        path0[i0][j0] = 0;
        Pos p;
        p.i = i0;
        p.j = j0;
        dijkstra(p);
        for (int i = 0; i < rows(); ++i) {
          for (int j = 0; j < cols(); ++j) {
            if (path0[i][j] < 10) cout << " " << path0[i][j];
            else cout << " -";
          }
          cout << endl;
        }

        /*
        distCP = VVI(rows(), VI(cols()), INF);
        for(int i = 0; i < rows(); ++i) {
          for (int j = 0; j < cols(); ++j) {
            if (cell(i, j).type == CITY or cell(i, j).type == PATH) distCP[i][j] = 0;
          }
        }
        //cuaprioritats<pair(int cost , Pos pos)> cost del tipus de terra



      }
      // Process orks in random order.
      //VI perm = random_permutation(my_orks.size());
      for (int k = 0; k < int(my_orks.size()); ++k) // for (int k = 0; k < int(perm.size()); ++k)
        move(my_orks[k]); //move(my_orks[perm[k]]);
        */
    }
  }

 };


 /**
  * Do not modify the following line.
  */
 RegisterPlayer(PLAYER_NAME);
