/*
 * @Author: Dizzrt
 * @LastEditTime: 2021-12-29 21:27:52
 */

#ifndef __BIG_MAP_H__
#define __BIG_MAP_H__

template <typename T> struct __map_node {
    T val;

    bool isRed = true;
    __map_node *left = nullptr;
    __map_node *right = nullptr;
};

template <typename T> struct __map_iterator {
    __map_node<T> *node;
    __map_iterator(__map_node<T> *_node) : node(_node) {}
};

template <typename T> class map {
  private:
    /* data */
  public:
    map(/* args */);
    //~map();
};

template <typename T> map<T>::map(/* args */) {}

// map::~map() {}

#endif