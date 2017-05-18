#pragma once

#include "../instructions/instructions.hpp"

#include <string>

namespace ida4humans {
  struct function_chunk {
    typedef function_chunk type;
    
    function_chunk(const ::ea_t& start_ea, const ::ea_t& end_ea) {
      ida_chunk_ = get_fchunk(start_ea);
      if (ida_chunk_ == nullptr)
        throw std::runtime_error("Can't get function chunk for ea " + std::to_string(start_ea));
    }
      
    function_chunk(func_t* ida_chunk) :
      ida_chunk_(ida_chunk) {
    }

    function_chunk(const type& other) :
      ida_chunk_(other.ida_chunk_) {
    }

    function_chunk(type&& other) :
      ida_chunk_(std::move(other.ida_chunk_)) {
    }

    void swap(type& other) {
      std::swap(ida_chunk_, other.ida_chunk_);
    }

    type& operator=(const type& other) {
      type tmp(*this);
      swap(tmp);
      return *this;
    }

    ::ida4humans::instructions instructions() const {
      return ::ida4humans::instructions(start_ea(), end_ea());
    }

    bool is_entry() const {
      return ::is_func_entry(ida_chunk_);
    }

    bool is_tail() const {
      return ::is_func_tail(ida_chunk_);
    }
    
    const ::ea_t start_ea() const {
      return ida_chunk_->startEA;
    }
      
    const ::ea_t end_ea() const {
      return ida_chunk_->endEA;
    }
  private:
    ::func_t* ida_chunk_;
  };
}
