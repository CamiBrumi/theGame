#include "Player.hh"


/**
 * Write the name of your player and save this file
 * with the same name and .cc extension.
 */
#define PLAYER_NAME Kamii


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
   const int COST_MULTIPLIER = 5;

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
     int dirIdx;
     if (profitableIsCity)
       dirIdx = cities[selectedProfitableObject][posActual.i][posActual.j].second;
     else
       dirIdx = paths[selectedProfitableObject][posActual.i][posActual.j].second;
       //dir = Dir(paths[selectedProfitableObject][posActual.i][posActual.j].second);
     if (dirIdx != -1)
       dir = Dir(dirIdx);
     else
       dir = Dir(NONE);
     //Pos npos = posActual + dir;
     execute(Command(id, dir));
     //cout << dir_str[dir] << endl;

   }

   void dijkstraC(Pos ini, int k, VVI& cellCost, Pos fini) {
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
       if (x == fini)
         return;
       if (d == cities[k][x.i][x.j].first) {
         //cout << "ESTEM EN EL PAS if (d == path0[x.i][x.j])" << endl;
         for (int dir = 0; dir != 4; ++dir) { //per a cada direcció / veí
           //cout << "direcció: " << dir << endl;
           Pos y = x + Dir(dir); //posició del veí segons la direcció escollida
           if (pos_ok(y) and cell(y).type != WATER) {
             int c = COST_MULTIPLIER*cost (cell(y).type); //cost de la nova posició
             int d2 = d + c + cellCost[y.i][y.j];//nou cost, segons si és herba, desert, bosc, ...
             //if (cellCost[y.i][y.j] != 1)
               //cout << "Found a cell cost which was not 1 but " << cellCost[y.i][y.j] << endl;
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

   void dijkstraP(Pos ini, int k, VVI& cellCost, Pos fini) {
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
       if (x == fini)
         return;
       if (d == paths[k][x.i][x.j].first) {
         //cout << "ESTEM EN EL PAS if (d == path0[x.i][x.j])" << endl;
         for (int dir = 0; dir != 4; ++dir) { //per a cada direcció / veí
           //cout << "direcció: " << dir << endl;
           Pos y = x + Dir(dir); //posició del veí segons la direcció escollida
           if (pos_ok(y) and cell(y).type != WATER) {
             int c = COST_MULTIPLIER*cost (cell(y).type); //cost de la nova posició
             int d2 = d + c + cellCost[y.i][y.j];//nou cost, segons si és herba, desert, bosc, ...
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

   struct pdd { // pos dir dist
     Pos pos;
     int dir, dist;

     /**
      * Default constructor (CELL_TYPE_SIZE, -1, -1, -1).
      */
     pdd () {
       pos.i = -1;
       pos.j = -1;
       dir = dist = -1;
     }

     /**
      * Given constructor.
      */
     pdd (Pos posicio, int direccio, int distancia) {
       pos = posicio;
       dir = direccio;
       dist = distancia;
     }

   };


//   bool findNearProfitableObjects(Pos posActual, VI &foundCity, VI &foundPath) {
//     queue<pdd> cua;
//     set<Pos> posicionsVisitades;
//     //     cuaPos.push(posActual); NAIN!
//     posicionsVisitades.insert(posActual);
//     //cuaDir.push(Dir(NONE));
//     // posem les quatre direccions a partir del vèrtex actual a la cua. no ho podem fer al while perquè necessitem tindre en compte la direcció cap a on ens movem inicialment
//     for (int d = 0; d < 4; ++d) {
//       Dir dir = Dir(d);
//       Pos novaPos = posActual + dir;
//       int dist = 1;
//       posicionsVisitades.insert(novaPos);
//
//       if (pos_ok(novaPos) and cell(novaPos).type != WATER) {
//         //cout << "Ork initially plans to look at position (" << novaPos.i << ", " << novaPos.j << "), going in the direction of " << d << endl;
//         cua.push(pdd(novaPos, d, dist));
//         //cuaPos.push(novaPos);
//         //cuaDir.push(d);
//       }
//     }
//     int xDir = NONE;
//     int xDist = -1;
//     int numPathsAndCitiesToSee = min(5, nb_cities()+nb_paths());
//     while (not cua.empty()) {
//      //pdd posDirDist = cua.front(); cua.pop();
//      Pos x = cua.front().pos;
//      xDir = cua.front().dir;
//      xDist = cua.front().dist;
//      cua.pop();
//      //cout << "Dir poped is " << xDir << endl;
//      if (cell(x).path_id != -1 and path_owner(cell(x).path_id) != me()) {
//         --numPathsAndCitiesToSee;
//      }
//      
//      or (cell(x).city_id != -1 and city_owner(cell(x).city_id) != me())) {
//         //cout << "Ork found position (" <<  x.i << ", " << x.j <<  ") to be feasible since it contains a city " << (cell(x).type == CITY) << " or a path " << (cell(x).type == PATH) << " and the direction of growth is " << xDir << endl;
//      }
//      for (int d = 0; d < 4 and xDist < 5; ++d) {
//        Dir dir = Dir(d);
//        Pos y = x + dir;
//        //cout << "Dir poped is " << xDir << ", old pos " << x <<  "and new pos is " << y <<" which is considered to be pos_ok:" << pos_ok(y) << " and the element has not been visited "  << (posicionsVisitades.find(y) == posicionsVisitades.end()) << endl;
//        if ((pos_ok(y) and posicionsVisitades.find(y) == posicionsVisitades.end()) and (cell(y).type != WATER)) { //no hem visitat aquesta casella
//
//          posicionsVisitades.insert(y);
//          //cuaPos.push(y);
//          //cuaDir.push(xDir); // direcció cap a on ens haurem de moure des del vèrtex inicial
//          cua.push(pdd(y, xDir, xDist + 1));
//        }
//      }
//    }
//     return false;
//  }


   //primul if din while
   //daca orcul din pos x ii al meu?
  //id != -1 and ii un orc strain
  //al doilea if din while:
  //avansez daca pos ii ok si nu ii positia la care ma uit, AND (daca ii path OR city)
   // u != v
   bool findEnemyInNearbyCell(Pos posActual) {
     queue<pdd> cua;
     set<Pos> posicionsVisitades;
     //     cuaPos.push(posActual); NAIN!
     posicionsVisitades.insert(posActual);
     //cuaDir.push(Dir(NONE));
     // posem les quatre direccions a partir del vèrtex actual a la cua. no ho podem fer al while perquè necessitem tindre en compte la direcció cap a on ens movem inicialment
     for (int d = 0; d < 4; ++d) {
       Dir dir = Dir(d);
       Pos novaPos = posActual + dir;
       int dist = 1;
       posicionsVisitades.insert(novaPos);

       if (pos_ok(novaPos) and (cell(novaPos).type == CITY or cell(novaPos).type == PATH)) {
         //cout << "Ork initially plans to look at position (" << novaPos.i << ", " << novaPos.j << "), going in the direction of " << d << endl;
         cua.push(pdd(novaPos, d, dist));
         //cuaPos.push(novaPos);
         //cuaDir.push(d);
       }
     }
     int xDir = NONE;
     int xDist = -1;
     while (not cua.empty()) {
      //pdd posDirDist = cua.front(); cua.pop();
      Pos x = cua.front().pos;
      xDir = cua.front().dir;
      xDist = cua.front().dist;
      cua.pop();
      //cout << "Dir poped is " << xDir << endl;
      if (cell(x).unit_id != -1 and unit(cell(x).unit_id).player != me()) {
         //cout << "Ork found position (" <<  x.i << ", " << x.j <<  ") to be feasible since it contains a city " << (cell(x).type == CITY) << " or a path " << (cell(x).type == PATH) << " and the direction of growth is " << xDir << endl;
          if (unit(cell(x).unit_id).health < unit(cell(posActual).unit_id).health) {
            execute(Command(cell(posActual).unit_id, Dir(xDir)));
          } else {
            Pos pContraria = posActual + Dir(dir_contraria[xDir]);
            if (cell(pContraria).type == CITY or cell(pContraria).type == PATH) {
              execute(Command(cell(posActual).unit_id, Dir(dir_contraria[xDir])));
            } else {
              bool dirFound = false;
              for (int k = 0; k < DIR_SIZE and not dirFound; ++k) {
                  if (k != dir_contraria[xDir] and k != xDir) {
                    if (cell(posActual + Dir(k)).type == CITY or cell(posActual + Dir(k)).type == PATH) {
                      execute(Command(cell(posActual).unit_id, Dir(k)));
                      dirFound = true;
                    }
                  }
              }
            }

          }
          return true;
        }
       for (int d = 0; d < 4 and xDist < 5; ++d) {
         Dir dir = Dir(d);
         Pos y = x + dir;
         //cout << "Dir poped is " << xDir << ", old pos " << x <<  "and new pos is " << y <<" which is considered to be pos_ok:" << pos_ok(y) << " and the element has not been visited "  << (posicionsVisitades.find(y) == posicionsVisitades.end()) << endl;
         if ((pos_ok(y) and posicionsVisitades.find(y) == posicionsVisitades.end()) and (cell(y).type == CITY or cell(y).type == PATH)) { //no hem visitat aquesta casella

           posicionsVisitades.insert(y);
           //cuaPos.push(y);
           //cuaDir.push(xDir); // direcció cap a on ens haurem de moure des del vèrtex inicial
           cua.push(pdd(y, xDir, xDist + 1));
        }
      }
    }
     return false;
  }

   /*Adds a cost square around pos P*/
    void drawCostSquare(VVI &cellCost, Pos p) {
       const int protectionRadius = 3;
       int iUpper = max(p.i - protectionRadius, 0);
       int iLower = min(p.i + protectionRadius, rows() - 1);
       int jLeft = max(p.j - protectionRadius, 0);
       int jRight = min(p.j + protectionRadius, cols() - 1);
//       cout << "the iUpper " << iUpper << " and the iLower " << iLower << endl;
//       cout << "the jLeft " << jLeft << " and the jRight " << jRight << endl;
       for (int i = iUpper; i < iLower; ++i) {
//          cout << "outer loop" << endl;
          for (int j = jLeft; j < jRight; ++j) {
//             cout << "Iter" << endl;
             int iAwayFromCenter = abs(protectionRadius - (i - iUpper));
             int jAwayFromCenter = abs(protectionRadius - (j - jLeft));
             if (iAwayFromCenter + jAwayFromCenter <= protectionRadius) {
                cellCost[i][j] = 1000000 - (iAwayFromCenter + jAwayFromCenter);
//                cout << "Drawing big cell cost" << endl;
             }
          }
       }
    }

    const int numCitiesForDijk = 5;
    const int numPathsForDijk = 2;


    void getNearCityAndVilages(priority_queue<pair<int, int> > &nearestCities, priority_queue<pair<int, int> > &nearestPaths, Pos posActual){
      int numCities = min(numCitiesForDijk, nb_cities());
//      priority_queue<pair<int, int> > nearestCitiesIdx(numCities, -1);
      //first is distance and second is idx
      for (int cityIdx = 0; cityIdx < nb_cities(); ++cityIdx) {//if this is too costly just restrict it to
        if (city_owner(cityIdx) != me()) {
          City destinyCity = city(cityIdx);
          int minDist = INF;
          for (int cityPosIdx = 0; cityPosIdx < destinyCity.size(); ++cityPosIdx){
            Pos cityPos = destinyCity[cityPosIdx];
            int distToCell = abs(cityPos.i - posActual.i) + abs(cityPos.j - posActual.j);
            if (minDist > distToCell)
               minDist = distToCell;
          }
          nearestCities.push(pair<int, int>(minDist, cityIdx));
          if (nearestCities.size() > numCitiesForDijk)
            nearestCities.pop();
        }
      }

      int numPaths = min(numPathsForDijk, nb_paths());

      for (int pathIdx = 0; pathIdx < nb_paths(); ++pathIdx) {//if this is too costly just restrict it to
        if (path_owner(pathIdx) != me()) {
          vector<Pos> destinyPath = path(pathIdx).second;
          int minDist = INF;
          for (int pathPosIdx = 0; pathPosIdx < destinyPath.size(); ++pathPosIdx){
            Pos pathPos = destinyPath[pathPosIdx];
            int distToCell = abs(pathPos.i - posActual.i) + abs(pathPos.j - posActual.j);
            if (minDist > distToCell)
               minDist = distToCell;
          }
          nearestPaths.push(pair<int, int>(minDist, pathIdx));
          if (nearestPaths.size() > numPathsForDijk)
             nearestPaths.pop();
        }
      }
    }
    void ork_action(int orkid) {
      VVI cellCost = VVI(rows(), VI(cols(), 1)); 
      cities = VVVP(nb_cities(), VVP(rows(), VP(cols(), PII(INF, -1)))); //those will contain the  
      paths = VVVP(nb_paths(), VVP(rows(), VP(cols(), PII(INF, -1)))); //directions taking into account
      //for (int cityIdx = 0; cityIdx < nb_cities(); ++cityIdx) {//if this is too costly just restrict it to
                                                               //the closest cities and paths
        //City destinyCity = city(cityIdx);
        //initiallizing all city positions to cost one
        //for (int cityPosIdx = 0; cityPosIdx < destinyCity.size(); ++cityPosIdx) {
          //Pos p = destinyCity[cityPosIdx];
        //}
        for (int pl = 0; pl < nb_players(); ++pl) {
          if (pl != me()) {
            VI other_orks = orks(pl);
            for (int k = 0; k < int(other_orks.size()); ++k) {
              Pos p = Pos(unit(other_orks[k]).pos.i, unit(other_orks[k]).pos.j);
              if (unit(orkid).health < unit(other_orks[k]).health) {
                drawCostSquare(cellCost, p);
              } else {
                cellCost[p.i][p.j] = 0;
              }
            }

            VI my_orks = orks(me());
            for (int k = 0; k < int(my_orks.size()); ++k) {
              Pos p = Pos(unit(my_orks[k]).pos.i, unit(my_orks[k]).pos.j);
              if (my_orks[k] != orkid) {
                drawCostSquare(cellCost, p);
                cellCost[p.i][p.j] = 1000000;//try to completely avoid your allies
              }
            }
          }
        }
      //}
      //heuristic to decrease amount of looked up cities and paths
      Unit u = unit(orkid);
      Pos posActual = u.pos;
      //getNearCitiesAndVilages(vecForeignNearCities, vecForeignNearPaths, posActual);
      priority_queue<pair<int, int> > nearestCities, nearestPaths;
      getNearCityAndVilages(nearestCities, nearestPaths, posActual);
      while (nearestCities.size() != 0) {//if this is too costly just restrict it to
        int cityIdx = nearestCities.top().second;
        nearestCities.pop();
                                                               //the closest cities and paths
        if (city_owner(cityIdx) != me()) {
          Pos samplePos = Pos(-1, -1);
          int minCost = INF;
          City destinyCity = city(cityIdx);
          Pos posOfMinCost = Pos(-1, -1);
          for (int cityPosIdx = 0; cityPosIdx < destinyCity.size(); ++cityPosIdx)
            if (minCost > cellCost[destinyCity[cityPosIdx].i][destinyCity[cityPosIdx].j]) {
              minCost = cellCost[destinyCity[cityPosIdx].i][destinyCity[cityPosIdx].j];
              posOfMinCost = destinyCity[cityPosIdx];
            }
          //and here comes the dijktra for this city
          cities[cityIdx][posOfMinCost.i][posOfMinCost.j].first = 0;
          
          if (posOfMinCost.i != -1) //you can put a warning here
            dijkstraC(posOfMinCost, cityIdx, cellCost, unit(orkid).pos);
          
          //you may also want to consider paths here, but we do not do that for the moment
        }
        
      }
      while (nearestPaths.size() != 0) {//if this is too costly just restrict it to
        int pathIdx = nearestPaths.top().second;
        nearestPaths.pop();
        
        if (path_owner(pathIdx) != me()) {
          Pos samplePos = Pos(-1, -1);
          int minCost = INF;
          vector<Pos> destinyPath = path(pathIdx).second;
          Pos posOfMinCost = Pos(-1, -1);
          for (int pathPosIdx = 0; pathPosIdx < destinyPath.size(); ++pathPosIdx)
            if (minCost > cellCost[destinyPath[pathPosIdx].i][destinyPath[pathPosIdx].j]) {
              minCost = cellCost[destinyPath[pathPosIdx].i][destinyPath[pathPosIdx].j];
              posOfMinCost = destinyPath[pathPosIdx];
            }
          //and here comes the dijktra for this path
          paths[pathIdx][posOfMinCost.i][posOfMinCost.j].first = 0;
          
          if (posOfMinCost.i != -1) //you can put a warning here
            dijkstraP(posOfMinCost, pathIdx, cellCost, unit(orkid).pos);
          //you may also want to consider paths here, but we do not do that for the moment
        
        }
      }

      int selectedProfitableObject;
      bool profitableIsCity;
      getClosestProfitableObject(orkid, selectedProfitableObject, profitableIsCity);
      if (selectedProfitableObject != -1)
        move(orkid, selectedProfitableObject, profitableIsCity); //the cities and paths
   }

   ////initiallizing all city positions to cost one
   //for (int cityPosIdx = 0; cityPosIdx < destinyCity.size(); ++cityPosIdx) {
   //  Pos p = destinyCity[cityPosIdx];
   //  cities[cityIdx][p.i][p.j].first = 0;
   //}
   /**
    * Play method, invoked once per each round.
    */
    virtual void play () {
      //cout << "ROUND = " << round() << endl;
      //cout << ">>>>>>>>>CPU LAST ROUND: " << status(3) << endl;

      VI my_orks = orks(me()); // Get the id's of my orks.

      for (int k = 0; k < int(my_orks.size()); ++k) {
         ork_action(my_orks[k]);
      }
    }

//      cities = VVVP(nb_cities(), VVP(rows(), VP(cols(), PII(INF, -1))));
//      paths = VVVP(nb_paths(), VVP(rows(), VP(cols(), PII(INF, -1))));
//
//      if (round() == 0) {
//        //cout << "ENTERED THE ROUND 0 IF" << endl;
//        cities = VVVP(nb_cities(), VVP(rows(), VP(cols(), PII(INF, -1))));
//        paths = VVVP(nb_paths(), VVP(rows(), VP(cols(), PII(INF, -1))));
//      }
//
//      for (int cityIdx = 0; cityIdx < nb_cities(); ++cityIdx) {//if this is too costly just restrict it to
//                                                               //the closest cities and paths
//        City destinyCity = city(cityIdx);
//        Pos samplePos = destinyCity[0]; //supposing each city has at least one valid position
//            if (cell(i,j).city_id != -1) {
//              int k = cell(i,j).city_id;
//              Pos p;
//              p.i = posCities[k].first = i;
//              p.j = posCities[k].second = j;
//              cities[k][i][j].first = 0;
//              dijkstraC(p, k);
//            } else if (cell(i,j).path_id != -1) {
//              int k = cell(i,j).path_id;
//              Pos p;
//              p.i = posPaths[k].first = i;
//              p.j = posPaths[k].second = j;
//              paths[k][i][j].first = 0;
//              dijkstraP(p, k);
//            }
//          }
//        }
//        /*
//        for (int k = 0; k < nb_paths(); ++k) {
//          for (int i = 0; i < rows(); ++i) {
//            for (int j = 0; j < cols(); ++j) {
//              if (paths[k][i][j].second != -1) cout << ' ' << dir_str[paths[k][i][j].second];
//              else cout << " -";
//            }
//            cout << endl;
//          }
//          cout << endl << endl;
//        } */
//    VI my_orks = orks(me()); // Get the id's of my orks.
//
//    for (int k = 0; k < int(my_orks.size()); ++k) {
//
//      int selectedProfitableObject;
//      bool profitableIsCity;
//      getClosestProfitableObject(my_orks[k], selectedProfitableObject, profitableIsCity);
//
//      int ork_i = unit(my_orks[k]).pos.i;
//      int ork_j = unit(my_orks[k]).pos.j;
//      Pos p;
//      p.i = ork_i;
//      p.j = ork_j;
//
//      bool threatened = false;
//      if (cell(ork_i, ork_j).type == CITY or cell(ork_i, ork_j).type == PATH) {
//         const int radius = 5;
//         threatened = findEnemyInNearbyCell(p); // inauntru punem radius si miscam jucatorul inspre enemic
//      }
//
//      //cout << "ORK " << k << " with position " <<  " IS MOVING IN DIRECTION: ";
//      //cout << "selectedProfitableObject = " << selectedProfitableObject << endl;
//      if (selectedProfitableObject != -1 and not threatened) //it might happen that you have simply conquered all
//         move(my_orks[k], selectedProfitableObject, profitableIsCity); //the cities and paths
//    }


 };


 /**
  * Do not modify the following line.
  */
 RegisterPlayer(PLAYER_NAME);
