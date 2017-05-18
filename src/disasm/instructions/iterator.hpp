#pragma once

#include <ida.hpp>
#include <pro.h>
#include <search.hpp>
#include <ua.hpp>

namespace ida4humans {
  template <typename container_t>
  struct instructions_iterator : public std::iterator<std::bidirectional_iterator_tag,
                                                      typename container_t::value_type
                                                      > {
    /* typedef const ::insn_t* const value_type;
       typedef container_t container_type; 
       typedef int difference_type;
    */
    typedef instructions_iterator<container_t> type;

    instructions_iterator(container_t* container, size_t pos) :
      container_(container),
      ea_(pos) {
    }

    instructions_iterator(const type& other) :
      container_(other.container_),
      ea_(other.ea_) {
    }

    instructions_iterator(type&& other) :
      container_(std::move(other.container_)),
      ea_(std::move(other.ea_)) {
    }

    void swap(type& other) {
      std::swap(container_, other.container_);
      std::swap(ea_, other.ea_);
    }

    type& operator=(const type& other) {
      type tmp(other);
      swap(tmp);
      return *this;
    }

    bool operator==(const type& other) const {
      return (ea_ == other.ea_);
    }

    bool operator!=(const type& other) const {
      return !(*this == other);
    }

    bool operator<(const type& other) const {
      return ea_ < other.ea_;
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

    type& operator++() {
      ea_ = ::find_code(ea_, SEARCH_DOWN);
      if ((ea_ > container_->end_ea()) || (ea_ == BADADDR)) {
        ea_ = container_->end_ea();
      }
      return *this;
    }

    type operator++(int) {
      type tmp(*this);
      this->operator++();
      return tmp;
    }

    type& operator+=(const difference_type& d) {
      if (d >= 0) {
        for (difference_type i = 0; i < d; ++i) this->operator++();
      }
      return *this;
    }

    type operator+(const difference_type& d) {
      type tmp(*this);
      tmp += d;
      return tmp;
    }

    type& operator--() {
      ea_ = ::find_code(ea_, SEARCH_UP);
      if ((ea_ < container_->start_ea()) || (ea_ == BADADDR)) {
        ea_ = container_->end_ea();
      }
      return *this;
    }

    type operator--(int) {
      type tmp(*this);
      this->operator--();
      return tmp;
    }

    type& operator-=(const difference_type& d) {
      if (d >= 0) {
        for (difference_type i = 0; i < d; ++i) this->operator--();
      }
      return *this;
    }

    type operator-(const difference_type& d) {
      type tmp(*this);
      tmp -= d;
      return tmp;
    }

    value_type operator*() {
      decode_insn(ea_);
      return &::cmd;
    }

    const ea_t& ea() const {
      return ea_;
    }
  private:
    mutable ::ea_t ea_;
    container_t* container_;
  };

  template <typename iterator_t>
  struct reverse_instructions_iterator : std::iterator<
    typename std::iterator_traits<iterator_t>::iterator_category,
    typename std::iterator_traits<iterator_t>::value_type,
    typename std::iterator_traits<iterator_t>::difference_type,
    typename std::iterator_traits<iterator_t>::pointer,
    typename std::iterator_traits<iterator_t>::reference > {
    
    using type = reverse_instructions_iterator<iterator_t>;
    using iterator_type = iterator_t;

    reverse_instructions_iterator() {
    }

    reverse_instructions_iterator(const iterator_type& it) :
      it_(it) {
    }

    void swap(type& other) {
      std::swap(it_, other.it_);
    }

    type& operator=(const type& other) {
      type tmp(other);
      swap(tmp);
      return *this;
    }

    bool operator==(const type& other) const {
      return (it_ == other.it_);
    }

    bool operator!=(const type& other) const {
      return !(*this == other);
    }

    bool operator<(const type& other) const {
      return it_ < other.it_;
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

    type& operator++() {
      --it_;
      return *this;
    }

    type operator++(int) {
      type tmp(*this);
      this->operator++();
      return tmp;
    }

    type& operator+=(const difference_type& d) {
      if (d >= 0) {
        for (difference_type i = 0; i < d; ++i) this->operator++();
      }
      return *this;
    }

    type operator+(const difference_type& d) {
      type tmp(*this);
      tmp += d;
      return tmp;
    }

    type& operator--() {
      ++it_;
      return *this;
    }

    type operator--(int) {
      type tmp(*this);
      this->operator--();
      return tmp;
    }

    type& operator-=(const difference_type& d) {
      if (d >= 0) {
        for (difference_type i = 0; i < d; ++i) this->operator--();
      }
      return *this;
    }

    type operator-(const difference_type& d) {
      type tmp(*this);
      tmp -= d;
      return tmp;
    }

    value_type operator*() {
      return *(it_ - 1);
    }

    iterator_t base() const {
      return it_;
    }
    
  private:
    iterator_t it_;
    
  };
}
