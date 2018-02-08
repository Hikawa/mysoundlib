#include "Group.h"

#include <stdexcept>
#include <algorithm>

Group::Group(const std::list<SoundProcessor*>& children) {
  std::list<SoundProcessor*> acc;
  for (SoundProcessor* child: children) {
    auto lastSource = acc.end();
    for (auto it = acc.begin(); it != acc.end(); ++it) {
      for (int input = 0; input < child->inputPortCount(); ++input) {
        if (&child->inputPort(input)->source()->processor() == *it) {
          lastSource = it;
          break;
        }
      }
    }
    auto firstDestination = acc.rend();
    auto it = acc.rbegin();
    for (; (it != acc.rend()) && (it.base() != lastSource); it++) {
      for (int input = 0; input < (*it)->inputPortCount(); ++input) {
        if (&(*it)->inputPort(input)->source()->processor() == child) {
          firstDestination = it;
        }
      }
    }
    for (; it != acc.rend(); ++it) {
      for (int input = 0; input < (*it)->inputPortCount(); ++input) {
        if (&(*it)->inputPort(input)->source()->processor() == child) {
          throw std::runtime_error("Dependency cycle in group");
        }
      }
    }
  }
  for (SoundProcessor* child: acc)
    children_.push_back(std::unique_ptr<SoundProcessor>(child));
}

void Group::process() {
  for (const auto& child: children_)
    child->process();
}

