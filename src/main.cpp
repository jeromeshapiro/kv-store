#include <iostream>
#include <string>
#include "hashMap.h"

int main() {
  kvmap::HashMap<unsigned, unsigned>* map = new kvmap::HashMap<unsigned, unsigned>(1);

  for (int i = 1; i < 1001; i++) {
    map->set(i, i);
  }

  std::cout << "1 equals " << map->get(1) <<std:: endl;
  std::cout << "2 equals " << map->get(2) <<std:: endl;
  std::cout << "3 equals " << map->get(3) <<std:: endl;
  std::cout << "4 equals " << map->get(4) <<std:: endl;
  std::cout << "5 equals " << map->get(5) << std::endl;
  std::cout << "there are " << map->count() << " items in the map" << std::endl;
  std::cout << "deleting key 5" << std::endl;
  map->remove(5);
  std::cout << "there are " << map->count() << " items in the map" << std::endl;
  std::cout << "1 equals " << map->get(1) <<std:: endl;
  std::cout << "2 equals " << map->get(2) <<std:: endl;
  std::cout << "3 equals " << map->get(3) <<std:: endl;
  std::cout << "4 equals " << map->get(4) << std::endl;
  std::cout << "5 equals " << map->get(5) << std::endl;
  std::cout << "clearing the map" << std::endl;
  map->clear();
  std::cout << "there are " << map->count() << " items in the map" << std::endl;
  std::cout << "1 equals " << map->get(1) <<std:: endl;
  std::cout << "2 equals " << map->get(2) <<std:: endl;
  std::cout << "3 equals " << map->get(3) <<std:: endl;
  std::cout << "4 equals " << map->get(4) << std::endl;
  std::cout << "5 equals " << map->get(5) << std::endl;
 
  delete map;
  return 0;
}
