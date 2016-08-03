#ifndef player_hpp
#define player_hpp

#include "linear_math.hpp"

struct player {
  vec3 location;
  vec3 heading;
  vec3 velocity;
  vec3 up;
};

#endif
