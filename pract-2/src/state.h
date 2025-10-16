#ifndef STATE_H
#define STATE_H

#include <string>
#include <ostream>

class State {
  public:
  State() : initial_(false), accept_(false), id("") {}
  State(const std::string& id) : initial_(false), accept_(false), id(id) {}
  std::string getId() const { return id; }
  void setAccept(bool accept) { accept_ = accept; }
  bool isAccept() const { return accept_; }
  friend std::ostream& operator<<(std::ostream& os, const State& state);
  bool operator==(const State& other) const { return id == other.id; }
  void operator=(const State& other) { id = other.id; initial_ = other.initial_; accept_ = other.accept_; }
  private:
   bool initial_;
   bool accept_; 
   std::string id = "";
};

#endif
