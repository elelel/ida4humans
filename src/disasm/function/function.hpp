#pragma once

#include <ida.hpp>
#include <pro.h>
#include <funcs.hpp>

#include <algorithm>
#include <vector>
#include <cstdio>
#include <stdexcept>

#include "chunk.hpp"
#include "iterator.hpp"

namespace ida4humans {
  struct function {
    typedef function_chunk value_type;
    
    typedef function_iterator<function_chunk, function> iterator;
    typedef function_iterator<const function_chunk, const function> const_iterator;

    friend iterator;
    friend const_iterator;

    function(func_t* ida_func) {
      ::iterate_func_chunks(ida_func, &iterate_chunks_callback, this);
      std::sort(chunks_.begin(), chunks_.end(), [] (const function_chunk& c1, const function_chunk& c2) {
          return c1.start_ea() < c2.start_ea();
        });
    }

    function(const function& other) :
      chunks_(other.chunks_) {
    }

    function(function&& other) :
      chunks_(std::move(other.chunks_)) {
    }

    void swap(function& other) {
      std::swap(chunks_, other.chunks_);
    }

    function& operator=(const function& other) {
      function tmp(other);
      swap(tmp);
      return *this;
    }

    static function by_ea(const ::ea_t& ea) {
      func_t* f = ::get_func(ea);
      if (f == nullptr) 
        throw std::runtime_error("Can't get function at ea " + std::to_string(ea));
      return function(f);
    }

    static function by_index(const size_t index) {
      func_t* f = ::getn_func(index);
      if (f == nullptr)
        throw std::runtime_error("Can't get function at index " + std::to_string(index) + " from global function list");
      return function(f);
    }

    const_iterator begin() const {
      return const_iterator(this, 0);
    }

    const_iterator end() const {
      return const_iterator(this, size());
    }
    
    const size_t size() const {
      return chunks_.size();
    }
    
  private:
    std::vector<function_chunk> chunks_;

    static void idaapi iterate_chunks_callback(::ea_t ea1, ::ea_t ea2, void *ud) {
      function* this_ = reinterpret_cast<function*>(ud);
      if ((ea1 != BADADDR) && (ea2 != BADADDR))
        this_->chunks_.push_back(function_chunk(ea1, ea2));
    }

  };
}

