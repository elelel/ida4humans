#pragma once

#include <ida.hpp>
#include <pro.h>
#include <funcs.hpp>

namespace ida4humans {
  template <typename T, typename container_t>
  struct function_iterator  : public std::iterator<std::forward_iterator_tag,
                                                   typename container_t::value_type> {
    typedef T value_type;
    typedef container_t container_type;
    typedef function_iterator<T, container_t> type;
    typedef int difference_type;

    function_iterator(container_t* container, const size_t pos) :
      container_(container),
      pos_(pos) {
    }

    function_iterator(const type& other) :
      container_(other.container_),
      pos_(other.pos_) {
    }

    function_iterator(type&& other) :
      container_(std::move(other.container_)),
      pos_(std::move(other.pos_)) {
    }

    void swap(type& other) {
      std::swap(container_, other.container_);
      std::swap(pos_, other.pos_);
    }

    type& operator=(const type& other) {
      type tmp(other);
      swap(*this, tmp);
      return *this;
    }

    type& operator++() {
      ++pos_;
      return *this;
    }

    type operator++(int) {
      type tmp(*this);
      ++*this;
      return tmp;
    }

    type& operator+=(const difference_type d) {
      pos_ += d;
      return *this;
    }

    type operator+=(const difference_type d) const {
      type tmp(*this);
      tmp.pos_ += d;
      return tmp;
    }

    type operator+(const difference_type d) {
      type tmp(*this);
      tmp += d;
      return tmp;
    }

    bool operator==(const type& other) const {
      return (pos_ == other.pos_);
    }

    bool operator!=(const type& other) const {
      return !(*this == other);
    }

    bool operator<(const type& other) const {
      return pos_ < other.pos_;
    }

    bool operator<=(const type& other) const {
      return (*this < other) || (*this == other);
    }

    bool operator>(const type* other) const {
      return !(*this <= other);
    }

    bool operator>=(const type*other) const {
      return !(*this < other);
    }
    
    value_type& operator*() {
      return container_->chunks_[pos_];
    }

    const value_type& operator*() const {
      return container_->chunks_[pos_];
    }
  protected:
    container_type* container_;
    size_t pos_;
  };
  
}
