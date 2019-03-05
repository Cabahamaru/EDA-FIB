#include "Player.hh"


/**
 * Write the name of your player and save this file
 * with the same name and .cc extension.
 */
#define PLAYER_NAME PIPO_MUERE


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
  typedef vector<double> Vd;
  typedef vector<int> V;
  typedef vector<Vd> M;
  typedef vector<bool> Vb;
  typedef vector<Vb> Mb;
  
  V my_orks; //id dels orks que controlo
  V all_orks; //id de tots els orks de la partida
  V all_cities; //id de les ciutats de la partida
  V all_paths; //id dels camins de la partida
  M island; //mapa de la illa amb els costs per moviment
  Mb visited;
  M dist;
  vector< vector<Dir> > Mdir;
  bool first = true;
  
  void prepare_all() {
    island = M(rows(), Vd(cols(), -1));
    for (int i = 0; i < rows(); ++i) {
      for (int j = 0; j < cols(); ++j) {
        Cell c = cell(i, j);
        if (c.type != WATER) {
          if (c.type == GRASS) island[i][j] = cost_grass();
          else if (c.type == FOREST) island[i][j] = cost_forest();
          else if (c.type == SAND) island[i][j] = cost_sand();
          else if (c.type == CITY) island[i][j] = 0.1;
          else if (c.type == PATH) island[i][j] = 0.1;
        }
        if (c.unit_id != -1) 
          all_orks.push_back(c.unit_id);
        if (c.city_id != -1) {
          if (all_cities.size() == 0) all_cities.push_back(c.city_id);
          else {
            bool f = true;
            for (int k = 0; k < int(all_cities.size()); ++k) {
              if (all_cities[k] == c.city_id) f = false;
            }
            if (f) all_cities.push_back(c.city_id);
          }
        }
        if (c.path_id != -1) {
          if (all_paths.size() == 0) all_paths.push_back(c.path_id);
          else {
            bool f = true;
            for (int k = 0; k < int(all_paths.size()); ++k) {
              if (all_paths[k] == c.path_id) f = false;
            }
            if (f) all_paths.push_back(c.path_id);
          }
        }
        
      }
    }
  }
  
  struct where_to_go {
    Pos pos; //posició a la q em trobo
    Dir dir; //direcció on m'haig de moure
  };
  
  //si id es de una ciutat, i es meva, retorna true
  //si id es -1 o la ciutat no es meva, retorna false
  bool my_city(const int id) {
    if (id == -1) return false;
    if (city_owner(id) == me()) return true;
    else return false;
  }
  
  bool my_path(const int id) {
    if (id == -1) return false;
    if (path_owner(id) == me()) return true;
    else return false;
  }
  
  typedef pair<double,Pos> P;
  
    struct CompareDist
{
    bool operator() (const P& a, const P& b)
    {
        return a.first > b.first;
    }
};
  
  Dir dijkstra(Pos pos) {
    dist = M(rows(), Vd(cols(),14700));
    visited = Mb(rows(), Vb(cols(),false));
    Mdir = vector< vector<Dir> >(rows(), vector<Dir> (cols(),NONE));
    dist[pos.i][pos.j] = 0;
    priority_queue< P, vector<P>, CompareDist > pq;
    pq.push(make_pair(dist[pos.i][pos.j],pos));
    visited[pos.i][pos.j] = true;
    while (( ((cell(pos.i,pos.j).type != CITY) or (my_city(cell(pos.i,pos.j).city_id))) and ((cell(pos.i,pos.j).type != PATH) or (my_path(cell(pos.i,pos.j).path_id))) ) and (not pq.empty())) {
      //NORTH
      if (pos_ok(pos+TOP) and (cell((pos+TOP).i, (pos+TOP).j).type != WATER) and (not visited[(pos+TOP).i][(pos+TOP).j]) and (dist[pos.i-1][pos.j] > dist[pos.i][pos.j] + island[pos.i-1][pos.j])) {
          if (first) {
            Mdir[pos.i-1][pos.j] = TOP;
            dist[pos.i-1][pos.j] = dist[pos.i][pos.j] + island[pos.i-1][pos.j];
          }
          else {
            Mdir[pos.i-1][pos.j] = Mdir[pos.i][pos.j];
            dist[pos.i-1][pos.j] = dist[pos.i][pos.j] + island[pos.i-1][pos.j];
          }
          visited[pos.i-1][pos.j] = true;
          pq.push(make_pair(dist[pos.i-1][pos.j],pos+TOP));
      }
      //EAST
      if (pos_ok(pos+RIGHT) and (cell((pos+RIGHT).i, (pos+RIGHT).j).type != WATER) and (not visited[(pos+RIGHT).i][(pos+RIGHT).j]) and (dist[pos.i][pos.j+1] > dist[pos.i][pos.j] + island[pos.i][pos.j+1])) {
          if (first) {
            Mdir[pos.i][pos.j+1] = RIGHT;
            dist[pos.i][pos.j+1] = dist[pos.i][pos.j] + island[pos.i][pos.j+1];
          }
          else {
            Mdir[pos.i][pos.j+1] = Mdir[pos.i][pos.j];
            dist[pos.i][pos.j+1] = dist[pos.i][pos.j] + island[pos.i][pos.j+1];
          }
          visited[(pos+RIGHT).i][(pos+RIGHT).j] = true;
          pq.push(make_pair(dist[pos.i][pos.j+1],pos+RIGHT));
      }
      //SOUTH
      if (pos_ok(pos+BOTTOM) and (cell((pos+BOTTOM).i, (pos+BOTTOM).j).type != WATER) and (not visited[(pos+BOTTOM).i][(pos+BOTTOM).j]) and (dist[pos.i+1][pos.j] > dist[pos.i][pos.j] + island[pos.i+1][pos.j]) ) {
          if (first) {
            Mdir[pos.i+1][pos.j] = BOTTOM;
            dist[pos.i+1][pos.j] = dist[pos.i][pos.j] + island[pos.i+1][pos.j];
          }
          else {
            Mdir[pos.i+1][pos.j] = Mdir[pos.i][pos.j];
            dist[pos.i+1][pos.j] = dist[pos.i][pos.j] + island[pos.i+1][pos.j];
          }
          visited[(pos+BOTTOM).i][(pos+BOTTOM).j] = true;
          pq.push(make_pair(dist[pos.i+1][pos.j],pos+BOTTOM));
      }
      //WEST
      if (pos_ok(pos+LEFT) and (cell((pos+LEFT).i, (pos+LEFT).j).type != WATER) and (not visited[(pos+LEFT).i][(pos+LEFT).j]) and (dist[pos.i][pos.j-1] > dist[pos.i][pos.j] + island[pos.i][pos.j-1])) {
          if (first) {
            Mdir[pos.i][pos.j-1] = LEFT;
            dist[pos.i][pos.j-1] = dist[pos.i][pos.j] + island[pos.i][pos.j-1];
          }
          else {
           Mdir[pos.i][pos.j-1] = Mdir[pos.i][pos.j];
            dist[pos.i][pos.j-1] = dist[pos.i][pos.j] + island[pos.i][pos.j-1];
          }
          visited[(pos+LEFT).i][(pos+LEFT).j] = true;
          pq.push(make_pair(dist[pos.i][pos.j-1],pos+LEFT));
      }
      first = false;
      pq.pop();
      pos = (pq.top()).second;
      dist[pos.i][pos.j] = (pq.top()).first;
    }
    return Mdir[pos.i][pos.j];
  }
  
  bool is_ally(const int id) {
    for (int i = 0; i < int(my_orks.size()); ++i) {
      if (id == my_orks[i]) return true;
    }
    return false;
  }
  
  bool nearby_enemies(const Unit u) {
    Pos pos = u.pos;
    Pos north = pos+TOP;
    Pos east = pos+RIGHT;
    Pos south = pos+BOTTOM;
    Pos west = pos+LEFT;
    if ((cell(north.i,north.j).unit_id != -1) and (not is_ally(cell(north.i,north.j).unit_id))) return true;
    if ((cell(east.i,east.j).unit_id != -1) and (not is_ally(cell(east.i,east.j).unit_id))) return true;
    if ((cell(south.i,south.j).unit_id != -1) and (not is_ally(cell(south.i,south.j).unit_id))) return true;
    if ((cell(west.i,west.j).unit_id != -1) and (not is_ally(cell(west.i,west.j).unit_id))) return true;
    return false;
  }
  
  Dir where_is_enemy(const Unit u) {
    Pos pos = u.pos;
    Pos north = pos+TOP;
    Pos east = pos+RIGHT;
    Pos south = pos+BOTTOM;
    Pos west = pos+LEFT;
    if ((cell(north.i,north.j).unit_id != -1) and (not is_ally(cell(north.i,north.j).unit_id))) return TOP;
    if ((cell(east.i,east.j).unit_id != -1) and (not is_ally(cell(east.i,east.j).unit_id))) return RIGHT;
    if ((cell(south.i,south.j).unit_id != -1) and (not is_ally(cell(south.i,south.j).unit_id))) return BOTTOM;
    if ((cell(west.i,west.j).unit_id != -1) and (not is_ally(cell(west.i,west.j).unit_id))) return LEFT;
    return NONE;
  }
  
  bool more_health_than_enemy(const Unit u, const Dir dir) {
    Pos my_pos = u.pos;
    Pos enemy_pos = my_pos+dir;
    Unit enemy = unit(cell(enemy_pos.i,enemy_pos.j).unit_id);
    if (enemy.health <= (u.health - cost(cell(enemy_pos.i,enemy_pos.j).type))) return true;
    else return false;
  }
  
  
  
  bool pos_taken(Pos pos, Dir dir) {
    Pos npos = pos+dir;
    if (cell(npos.i,npos.j).unit_id == -1) return false;
    if (not is_ally(cell(npos.i,npos.j).unit_id)) return false;
    return true;
  }
  
  void attack(const Unit u, const Dir dir) {
    execute(Command(u.id,dir));
  }
 
  bool esquiva(const Unit u, const Dir dir) {
    int id = u.id;
    Pos pos = u.pos;
     if (dir == TOP) {
      if (pos_ok(pos+BOTTOM) and (cell((pos+BOTTOM).i, (pos+BOTTOM).j).type != WATER) and not pos_taken(pos,BOTTOM) and cell((pos+BOTTOM).i, (pos+BOTTOM).j).type < SAND) execute(Command(id,BOTTOM));
      else if (pos_ok(pos+LEFT) and (cell((pos+LEFT).i, (pos+LEFT).j).type != WATER) and not pos_taken(pos,LEFT) and cell((pos+LEFT).i, (pos+LEFT).j).type < SAND) execute(Command(id,LEFT));
      else if (pos_ok(pos+RIGHT) and (cell((pos+RIGHT).i, (pos+RIGHT).j).type != WATER) and not pos_taken(pos,RIGHT) and cell((pos+RIGHT).i, (pos+RIGHT).j).type < SAND) execute(Command(id,RIGHT));
      
      else return false;
    }
    if (dir == LEFT) {
      if (pos_ok(pos+RIGHT) and (cell((pos+RIGHT).i, (pos+RIGHT).j).type != WATER) and not pos_taken(pos,RIGHT) and cell((pos+RIGHT).i, (pos+RIGHT).j).type < SAND) execute(Command(id,RIGHT));
      else if (pos_ok(pos+TOP) and (cell((pos+TOP).i, (pos+TOP).j).type != WATER) and not pos_taken(pos,TOP) and cell((pos+TOP).i, (pos+TOP).j).type < SAND) execute(Command(id,TOP));
      else if (pos_ok(pos+BOTTOM) and (cell((pos+BOTTOM).i, (pos+BOTTOM).j).type != WATER) and not pos_taken(pos,BOTTOM) and cell((pos+BOTTOM).i, (pos+BOTTOM).j).type < SAND) execute(Command(id,BOTTOM));
      
      return false;
    }
    if (dir == RIGHT) {
      if (pos_ok(pos+LEFT) and (cell((pos+LEFT).i, (pos+LEFT).j).type != WATER) and not pos_taken(pos,LEFT) and cell((pos+LEFT).i, (pos+LEFT).j).type < SAND) execute(Command(id,LEFT));
      else if (pos_ok(pos+TOP) and (cell((pos+TOP).i, (pos+TOP).j).type != WATER) and not pos_taken(pos,TOP) and cell((pos+TOP).i, (pos+TOP).j).type < SAND) execute(Command(id,TOP));
      else if (pos_ok(pos+BOTTOM) and (cell((pos+BOTTOM).i, (pos+BOTTOM).j).type != WATER) and not pos_taken(pos,BOTTOM) and cell((pos+BOTTOM).i, (pos+BOTTOM).j).type < SAND) execute(Command(id,BOTTOM));
      
      else return false;
    }
    if (dir == BOTTOM) {
      if (pos_ok(pos+TOP) and (cell((pos+TOP).i, (pos+TOP).j).type != WATER) and not pos_taken(pos,TOP) and cell((pos+TOP).i, (pos+TOP).j).type < SAND) execute(Command(id,TOP));
      else if (pos_ok(pos+LEFT) and (cell((pos+LEFT).i, (pos+LEFT).j).type != WATER) and not pos_taken(pos,LEFT) and cell((pos+LEFT).i, (pos+LEFT).j).type < SAND) execute(Command(id,LEFT));
      else if (pos_ok(pos+RIGHT) and (cell((pos+RIGHT).i, (pos+RIGHT).j).type != WATER) and not pos_taken(pos,RIGHT) and cell((pos+RIGHT).i, (pos+RIGHT).j).type < SAND) execute(Command(id,RIGHT));
      else return false;
    }
    return true;
  }
  
  bool ork_is_mine(int id) {
    if (id == -1) return true;
    for (int i = 0; i < int(my_orks.size()); ++i) {
      if (id == my_orks[i]) return true;
    }
    return false;
  }
  
    Dir conquer(Pos pos) {
    queue<where_to_go> q;
    visited = Mb(rows(), Vb(cols(), false));
    where_to_go w;
    w.pos = pos;
    w.dir = NONE;
    q.push(w);
    visited[pos.i][pos.j] = true;
    while  ( ork_is_mine(cell(pos.i,pos.j).unit_id) and (not q.empty()) ) {
      if ( pos_ok(pos+TOP) and (cell((pos+TOP).i,(pos+TOP).j).type == CITY or cell((pos+TOP).i,(pos+TOP).j).type == PATH) and not visited[(pos+TOP).i][(pos+TOP).j] and (cell((pos+TOP).i, (pos+TOP).j).type != WATER)) {
          if (first) {
            w.dir = TOP;
            w.pos = pos+TOP;
          }
          else {
            w.dir = (q.front()).dir;
            w.pos = pos+TOP;
          }
          visited[(pos+TOP).i][(pos+TOP).j] = true;
          q.push(w);
        }
      if (  pos_ok(pos+RIGHT) and (cell((pos+RIGHT).i,(pos+RIGHT).j).type == CITY or cell((pos+RIGHT).i,(pos+RIGHT).j).type == PATH) and (not visited[(pos+RIGHT).i][(pos+RIGHT).j]) and (cell((pos+RIGHT).i, (pos+RIGHT).j).type != WATER)) {
          if (first) {
            w.dir = RIGHT;
            w.pos = pos+RIGHT;
          }
          else {
            w.dir = (q.front()).dir;
            w.pos = pos+RIGHT;
          }
          visited[(pos+RIGHT).i][(pos+RIGHT).j] = true;
          q.push(w);
        }
      if ( pos_ok(pos+BOTTOM) and (cell((pos+BOTTOM).i,(pos+BOTTOM).j).type == CITY or cell((pos+BOTTOM).i,(pos+BOTTOM).j).type == PATH) and (not visited[(pos+BOTTOM).i][(pos+BOTTOM).j]) and (cell((pos+BOTTOM).i, (pos+BOTTOM).j).type != WATER)) {
          if (first) {
            w.dir = BOTTOM;
            w.pos = pos+BOTTOM;
          }
          else {
            w.dir = (q.front()).dir;
            w.pos = pos+BOTTOM;
          }
          visited[(pos+BOTTOM).i][(pos+BOTTOM).j] = true;
          q.push(w);
        }
      if ( pos_ok(pos+LEFT) and (cell((pos+LEFT).i,(pos+LEFT).j).type == CITY or cell((pos+LEFT).i,(pos+LEFT).j).type == PATH) and (not visited[(pos+LEFT).i][(pos+LEFT).j]) and (cell((pos+LEFT).i, (pos+LEFT).j).type != WATER)) {
          if (first) {
            w.dir = LEFT;
            w.pos = pos+LEFT;
          }
          else {
            w.dir = (q.front()).dir;
            w.pos = pos+LEFT;
          }
          visited[(pos+LEFT).i][(pos+LEFT).j] = true;
          q.push(w);
        }
      first = false;
      q.pop();
      w = q.front();
      pos = w.pos;
    }
    return w.dir;
  }
  
  /**
   * Play method, invoked once per each round.
   */
  virtual void play () {
    if (round() == 0) {
      prepare_all();
    }
    my_orks = orks(me());
    V perm = random_permutation(my_orks.size());
    for (int i = 0; i < int(perm.size()); ++i) {
      Unit u = unit(my_orks[perm[i]]);
      Pos pos = u.pos;
      if (nearby_enemies(u) and not pos_taken(pos,where_is_enemy(u))) {
        if (more_health_than_enemy(u,where_is_enemy(u))) {
          attack(u,where_is_enemy(u));
        }
       else {
          //if (esquiva(u,where_is_enemy(u))) return;
          Dir dir = dijkstra(pos);
          if (dir == where_is_enemy(u)) esquiva(u,where_is_enemy(u));
          else execute(Command(u.id,dir));
          first = true;
       }
      }
      else {
        if ( (cell(pos.i,pos.j).type == CITY and city_owner(cell(pos.i,pos.j).city_id) != me()) or ((cell(pos.i,pos.j).type == PATH and path_owner(cell(pos.i,pos.j).path_id) != me())) ) {
          Dir dir = conquer(pos);
          execute(Command(u.id,dir));
          first = true;
        }
        else {
          Dir dir = dijkstra(pos);
          execute(Command(u.id,dir));
          first = true;
        }
      }
    }
  }
};

/**
 * Do not modify the following line.
 */
RegisterPlayer(PLAYER_NAME);