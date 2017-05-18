#pragma once

#include <area.hpp>

#include <stdexcept>

namespace ida4humans {
  struct area;

  struct area : public area_t {
  public:
    area(const ::ea_t _start_ea, const ::ea_t _end_ea) {
      if (_start_ea > _end_ea) {
        throw std::range_error(std::string("Invalid area range: " + std::to_string(_start_ea) + " > " + std::to_string(_end_ea)));
      }
      if (_start_ea < std::numeric_limits<::ea_t>::min()) {
        throw std::underflow_error(std::string("Invalid area range: " + std::to_string(_start_ea) + " is lower than numeric limit"));
      }
      if (_end_ea > std::numeric_limits<::ea_t>::max()) {
        throw std::underflow_error(std::string("Invalid area range: " + std::to_string(_start_ea) + " is higher than numeric limit"));
      }

      startEA = _start_ea;
      endEA = _end_ea; 
    }

    area(const area_t& a) :
      area(a.startEA, a.endEA) {
    }

    area(const area& other){
      startEA = other.startEA;
      endEA = other.endEA;
    }

    area(area&& other){
      startEA = std::move(other.startEA);
      endEA = std::move(other.endEA);
    }

    void swap(area& other) {
      std::swap(other.startEA, startEA);
      std::swap(other.endEA, endEA);
    }

    ::ea_t& start_ea() {
      return startEA;
    }

    ::ea_t& end_ea() {
      return endEA;
    }
  };
}
