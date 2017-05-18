#pragma once

#include <pro.h>

#include "function/function.hpp"

#include "area.hpp"
#include "../logging.hpp"

namespace ida4humans {
  namespace program {

    template <typename T>
    struct functions_iterator : public std::iterator<std::forward_iterator_tag, T> {
      typedef T value_type;
      typedef functions_iterator<T> type;
      typedef int difference_type;

      functions_iterator(const size_t pos) :
        pos_(pos) {
      }

      functions_iterator(const type& other) :
        pos_(other.pos_) {
      }

      functions_iterator(type&& other) :
        pos_(std::move(other.pos_)) {
      }

      void swap(type& other) {
        std::swap(pos_, other.pos_);
      }

      type& operator=(const type& other) {
        type tmp(other);
        swap(*this);
        return tmp;
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

      value_type operator*() {
        return function::by_index(pos_);
      }

    private:
      size_t pos_;
      
    };

    struct functions {
      typedef functions_iterator<const function> const_iterator;
      typedef functions_iterator<function> iterator;

      static functions& instance() {
        static functions fs;
        return fs;
      }
      
      static iterator begin() {
        return iterator(0);
      }

      static const_iterator cbegin() {
        return const_iterator(0);
      }

      static iterator end() {
        return iterator(::get_func_qty());
      }

      static const_iterator cend() {
        return const_iterator(::get_func_qty());
      }
    };
  }
}
