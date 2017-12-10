#include "Player.hh"


/**
 * Write the name of your player and save this file
 * with the same name and .cc extension.
 */
#define PLAYER_NAME Camelia


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
   //VVP path0; //TODO delete this after used
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

   //the closest profitable is considered only if it is not yours
   void getClosestProfitableObject(int id, int &selectedProfitableObject, bool &profitableIsCity) {
      int closestCityIdx, closestPathIdx;
      int lowestCityCost, lowestPathCost;
      closestCityIdx = closestPathIdx = -1;
      lowestCityCost = lowestPathCost = 99999999; //replace this cost with a maximum constant
      Unit u = unit(id);
      Pos posActual = u.pos;
      int i0 = posActual.i;
      int j0 = posActual.j;
      for (int cityIdx = 0; cityIdx < nb_cities(); ++cityIdx) { //TODO nb_cities()
         if (lowestCityCost > cities[cityIdx][i0][j0].first and city_owner(cityIdx) != me()) {
            lowestCityCost = cities[cityIdx][i0][j0].first;
            closestCityIdx = cityIdx;
         }
      }
      for (int pathIdx = 0; pathIdx < nb_paths(); ++pathIdx) { //TODO nb_paths()
         if (lowestPathCost > paths[pathIdx][i0][j0].first and path_owner(pathIdx) != me()) {
            lowestPathCost = paths[pathIdx][i0][j0].first;
            closestPathIdx = pathIdx;
         }
      }
      if (lowestPathCost < lowestCityCost) {
         selectedProfitableObject = closestPathIdx;
         profitableIsCity = false;
      } else {
         selectedProfitableObject = closestCityIdx;
         profitableIsCity = true;
      }
   }

   // Moves ork with identifier id.
   //if profitableIsCity is false, then it means that the paths VVVP must be used
   //selectedProfitableObject is the index of the city or path to be used
   void move(int id, int selectedProfitableObject, bool profitableIsCity) {

     Unit u = unit(id);
     Pos posActual = u.pos;
     Dir dir;
     if (profitableIsCity)
       dir = Dir(cities[selectedProfitableObject][posActual.i][posActual.j].second);
     else
       dir = Dir(paths[selectedProfitableObject][posActual.i][posActual.j].second);
     //Pos npos = posActual + dir;
     execute(Command(id, dir));
     //cout << dir_str[dir] << endl;

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
             int c = 2*cost (cell(y).type); //cost de la nova posició
             int d2 = d + c + 1;//nou cost, segons si és herba, desert, bosc, ...
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
             int c = 2*cost (cell(y).type); //cost de la nova posició
             int d2 = d + c + 1;//nou cost, segons si és herba, desert, bosc, ...
             //cout << "d2 = " << d2 << " , path0[y.i][y.j] = " << paths[k][y.i][y.j] << endl;
             //cout << "d2 = " << d2 << endl;
             //cout << "paths[k][y.i][y.j].first = " << paths[k][y.i][y.j].first << endl;
             if (d2 < paths[k][y.i][y.j].first) {
               //cout << "HA ENTRAT EN EL if (d2 < path0[x.i][x.j]) => S'HAURIA DE MODIFICAR LA MATRIU EN EL PUNT " << x.i << " " << x.j << endl;
               paths[k][y.i][y.j].first = d2;
               Q.push(P(-d2, y));
               paths[k][y.i][y.j].second = dir_contraria[dir];
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
      //cout << "ROUND = " << round() << endl;
      //cout << ">>>>>>>>>CPU LAST ROUND: " << status(3) << endl;
      if (round() == 0) {
        //cout << "ENTERED THE ROUND 0 IF" << endl;
        cities = VVVP(nb_cities(), VVP(rows(), VP(cols(), PII(INF, -1))));
        paths = VVVP(nb_paths(), VVP(rows(), VP(cols(), PII(INF, -1))));

        //cout << "paths[0][1][1].first = " << paths[0][1][1].first << endl;
        VP posCities = VP(nb_cities(), PII(-1, -1));
        VP posPaths = VP(nb_paths(), PII(-1, -1));
        //cout << "nb_cities = " << nb_cities() << endl;
        //cout << "nb_paths = " << nb_paths() << endl;

        for(int i = 0; i < rows(); ++i) {
          for (int j = 0; j < cols(); ++j) {

            if (cell(i,j).city_id != -1 and posCities[cell(i,j).city_id].first == -1) {
              int k = cell(i,j).city_id;
              Pos p;
              p.i = posCities[k].first = i;
              p.j = posCities[k].second = j;
              cities[k][i][j].first = 0;
              dijkstraC(p, k);
            } else if (cell(i,j).path_id != -1 and posPaths[cell(i,j).path_id].first == -1) {
              int k = cell(i,j).path_id;
              Pos p;
              p.i = posPaths[k].first = i;
              p.j = posPaths[k].second = j;
              paths[k][i][j].first = 0;
              dijkstraP(p, k);
            }
          }
        }
        /*
        for (int k = 0; k < nb_paths(); ++k) {
          for (int i = 0; i < rows(); ++i) {
            for (int j = 0; j < cols(); ++j) {
              if (paths[k][i][j].second != -1) cout << ' ' << dir_str[paths[k][i][j].second];
              else cout << " -";
            }
            cout << endl;
          }
          cout << endl << endl;
        } */

    }
    VI my_orks = orks(me()); // Get the id's of my orks.

    for (int k = 0; k < int(my_orks.size()); ++k) {

      int selectedProfitableObject;
      bool profitableIsCity;
      getClosestProfitableObject(my_orks[k], selectedProfitableObject, profitableIsCity);
      //cout << "ORK " << k << " with position " <<  " IS MOVING IN DIRECTION: ";
      cout << "selectedProfitableObject = " << selectedProfitableObject << endl;
      if (selectedProfitableObject != -1) //it might happen that you have simply conquered all
         move(my_orks[k], selectedProfitableObject, profitableIsCity); //the cities and paths
    }
  }

 };


 /**
  * Do not modify the following line.
  */
 RegisterPlayer(PLAYER_NAME);
