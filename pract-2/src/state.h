#ifndef STATE_H
#define STATE_H

#include <string>
#include <ostream>

class State {
  public:
  State() : id("") {}
  State(const std::string& id) : id(id) {}
  std::string getId() const { return id; }
  friend std::ostream& operator<<(std::ostream& os, const State& state);
  bool operator==(const State& other) const { return id == other.id; }
  private:
   bool initial_;
   bool accept_; 
   std::string id = "";
};

#endif
