#ifndef SPIDX_H
#define SPIDX_H

#include <iostream>
#include <map>
#include <vector>
#include "./bbxtool.h"

using namespace BBXTOOL;

struct ResponsibleMetaServer {
  ResponsibleMetaServer(int metaServerID, BBX *bbx)
      : m_metaServerID(metaServerID), m_bbx(bbx){};
  int m_metaServerID;
  BBX *m_bbx;
  ~ResponsibleMetaServer(){};
};

struct Spidx {

  Spidx(){};

  // init the metaServerBBOXList according to the partitionNum and the bbox of
  // the global domain
  void initSpidx(size_t ndim, size_t metaServerNum, BBX* globalBBX);

  // get the corresponding metaserver according to the input bbox
  std::vector<ResponsibleMetaServer> getMetaServerID(BBX *BBXQuery);

  // the data structure that maps the server id into the bounding box
  std::map<int, BBX *> metaServerIDToBBX;

  //the data structure that maps the meta server id into the address of the meta server
  std::map<int, std::string> metaServerIDToAddr;
  
  // print the value of the data structure
  void printDTMInfo() {
    auto it = metaServerIDToBBX.begin();
    while (it != metaServerIDToBBX.end()) {
      // Accessing KEY from element pointed by it.
      int id = it->first;
      std::cout << "id: " << id << std::endl;
      it->second->printBBXinfo();
      it++;
    }
  }

};

#endif