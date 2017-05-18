#pragma once


#include <ida.hpp>
#include <pro.h>
#include <search.hpp>
#include <ua.hpp>
#include <intel.hpp>

#include <iterator>
#include <limits>
#include <stdexcept>

#include "iterator.hpp"

namespace ida4humans {
  struct instructions {
    typedef const ::insn_t* value_type;
    using iterator = instructions_iterator<instructions>;
    using const_iterator = instructions_iterator<const instructions>;
    using reverse_iterator = reverse_instructions_iterator<iterator>;
    using const_reverse_iterator = reverse_instructions_iterator<const_iterator>;

    friend iterator;
    friend const_iterator;
      
    instructions(const ::ea_t& start_ea, const ::ea_t& end_ea) :
      start_ea_(start_ea), end_ea_(end_ea) {
      ::ea_t ea = ::find_code(end_ea, SEARCH_UP);
      if (ea != BADADDR) {
        decode_insn(ea);
        end_ea_ = ea + ::cmd.size;
      }
    }

    iterator begin() {
      return iterator(this, start_ea_);
    }

    const_iterator cbegin() const {
      return const_iterator(this, start_ea_);
    }

    iterator end() {
      return iterator(this, end_ea_);
    }

    const_iterator cend() {
      return const_iterator(this, end_ea_);
    }

    reverse_iterator rbegin() {
      return reverse_iterator(end());
    }

    reverse_iterator rend() {
      return reverse_iterator(begin());
    }
      
    ea_t& start_ea() {
      return start_ea_;
    }

    const ea_t& start_ea() const {
      return start_ea_;
    }

    ea_t& end_ea() {
      return end_ea_;
    }

    const ea_t& end_ea() const {
      return end_ea_;
    }
      
  private:
    ::ea_t start_ea_;
    ::ea_t end_ea_;
  };
}
