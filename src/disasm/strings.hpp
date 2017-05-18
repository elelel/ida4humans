#pragma once

#include <ida.hpp>
#include <bytes.hpp>
#include <segment.hpp>
#include <strlist.hpp>

#include <algorithm>
#include <cstdint>
#include <iterator>
#include <vector>

#include "../logging.hpp"

namespace ida4humans {
  class string;
  class string_iterator;
  
  class string {
  public:
    typedef std::vector<uint8_t> data_type;

    static string by_index(const int index) {
      ::string_info_t si;
      get_strlist_item(index, &si);
      return string(si);
    }

    string(::string_info_t si) {
      si_= si;
    }
    
    string(::string_info_t *si) {
      si_= *si;
    }
    
    string(const string &other) :
      si_(other.si_) {
    }

    string(string &&other) :
      si_(std::move(other.si_)) {
    }

    bool operator==(const string &other) const {
      return ea() == other.ea();
    }

    void swap(string &other) {
      ::string_info_t si_tmp(other.si_);
      other.si_ = si_;
      si_ = si_tmp;
    }
      
    string& operator=(const string &other) {
      if (this != &other) {
        string tmp(other);
        swap(tmp);
      }
      return *this;
    }
    
    data_type get_bytes() const {
      auto len = length();
      if (len <= MAXSTR) {
        data_type rslt(len);
        get_many_bytes(ea(), &rslt[0], length());
        return rslt;
      }
      return data_type();
    }

    const ::ea_t ea() const {
      return si_.ea;
    }

    const int length() const {
      return si_.length;
    }

    const int type() const {
      return si_.type;
    }

  private:
    string() {
    }

    ::string_info_t si_;
  };

  class string_iterator : public std::iterator<std::random_access_iterator_tag, const string> {
  public:
    string_iterator() : pos_(0) {}
      
    string_iterator(const int i) : pos_(i) {}

    string_iterator(const string_iterator& other) : pos_(other.pos_) {}

    void swap(string_iterator &other) {
      std::swap(pos_, other.pos_);
    }
      
    string_iterator& operator=(const string_iterator &other) {
      if (this != &other) {
        string_iterator tmp(other);
        swap(tmp);
      }
      return *this;
    }

    string_iterator& operator++() {
      ++pos_;
      return *this;
    }

    string_iterator operator++(int) {
      string_iterator tmp(*this);
      ++*this;
      return tmp;
    }

    string_iterator& operator--() {
      --pos_;
      return *this;
    }

    string_iterator operator--(int) {
      string_iterator tmp(*this);
      --*this;
      return tmp;
    }

    string_iterator& operator+=(const string_iterator &other) {
      pos_ += other.pos_;
      return *this;
    }

    string_iterator& operator+=(const int offset) {
      pos_ += offset;
      return *this;
    }

    string_iterator operator+(const string_iterator &other) {
      string_iterator tmp(*this);
      tmp += other;
      return tmp;
    }

    string_iterator operator+(const int offset) {
      string_iterator tmp(*this);
      tmp += offset;
      return tmp;
    }

    string_iterator& operator-=(const string_iterator &other) {
      pos_ -= other.pos_;
      return *this;
    }

    string_iterator& operator-=(const int offset) {
      pos_ -= offset;
      return *this;
    }

    string_iterator operator-(const string_iterator &other) {
      string_iterator tmp(*this);
      tmp -= other;
      return tmp;
    }

    string_iterator operator-(const int offset) {
      string_iterator tmp(*this);
      tmp -= offset;
      return tmp;
    }

    const string operator*() const {
      return string::by_index(pos_);
    }

    const string operator[](int idx) {
      return string::by_index(pos_ + idx);
    }


    bool operator==(const string_iterator& other) const {
      //     LOG(std::string("iterator operator == ")+ std::to_string(pos_) + " vs. " + std::to_string(other.pos_));
      return pos_ == other.pos_;
    }

    bool operator!=(const string_iterator& other) const {
      return !(*this == other);
    }

    bool operator<=(const string_iterator& other) const {
      return pos_ <= other.pos_;
    }

    bool operator<(const string_iterator& other) const {
      return pos_ < other.pos_;
    }

    bool operator>=(const string_iterator& other) const {
      return pos_ >= other.pos_;
    }

    bool operator>(const string_iterator& other) const {
      return pos_ > other.pos_;
    }

  private:
    int pos_;
  };

  class strings {
  public:
    static void refresh_for_all_segments() {
      const int total_seg_no = get_segm_qty();
      for (int seg_no = 0; seg_no < total_seg_no; ++seg_no) {
        segment_t *seg = getnseg(0);
        refresh_strlist(seg->startEA, seg->endEA);
      }
    }

    inline static string_iterator begin() {
      return string_iterator(0);
    }

    inline static string_iterator end() {
      auto qty = get_strlist_qty();
      return string_iterator(get_strlist_qty());
    }
  private:
    strings() {};    
  };


}
