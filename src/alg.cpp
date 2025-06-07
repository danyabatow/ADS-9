// Copyright 2022 NNTU-CS
#include  <iostream>
#include  <fstream>
#include  <locale>
#include  <cstdlib>
#include  <algorithm>
#include  <stdexcept>
#include  "tree.h"

PMTree::PMTree(const std::vector<char>& elements)
    : element_count(elements.size()) {
  if (elements.empty()) {
    root = nullptr;
    return;
  }

  root = std::make_shared<Node>('\0');
  buildTree(root, elements);
}

void PMTree::buildTree(std::shared_ptr<Node> parent,
                       const std::vector<char>& elements) {
  if (elements.empty()) {
    return;
  }

  for (char elem : elements) {
    auto child = std::make_shared<Node>(elem);
    parent->children.push_back(child);

    std::vector<char> remaining;
    for (char e : elements) {
      if (e != elem) {
        remaining.push_back(e);
      }
    }

    buildTree(child, remaining);
  }
}

std::vector<std::vector<char>> PMTree::getAllPerms() const {
  std::vector<std::vector<char>> result;
  if (!root) return result;

  std::vector<char> current;
  getAllPermsHelper(root, current, result);
  return result;
}

void PMTree::getAllPermsHelper(std::shared_ptr<Node> node,
                               std::vector<char>& current,
                               std::vector<std::vector<char>>& result) const {
  if (node->value != '\0') {
    current.push_back(node->value);
  }

  if (node->children.empty()) {
    result.push_back(current);
  } else {
    for (const auto& child : node->children) {
      getAllPermsHelper(child, current, result);
    }
  }

  if (node->value != '\0') {
    current.pop_back();
  }
}

std::vector<char> PMTree::getPerm1(int num) const {
  std::vector<char> result;
  if (num < 1 || num > countPermutations() || !root) {
    return result;
  }

  int counter = 0;
  std::vector<char> current;
  getPermHelper1(root, current, counter, num, result);
  return result;
}

void PMTree::getPermHelper1(std::shared_ptr<Node> node,
                            std::vector<char>& current, int& counter,
                            int target, std::vector<char>& result) const {
  if (node->value != '\0') {
    current.push_back(node->value);
  }

  if (node->children.empty()) {
    counter++;
    if (counter == target) {
      result = current;
    }
  } else {
    for (const auto& child : node->children) {
      if (result.empty()) {
        getPermHelper1(child, current, counter, target, result);
      }
    }
  }

  if (node->value != '\0') {
    current.pop_back();
  }
}

std::vector<char> PMTree::getPerm2(int num) const {
  std::vector<char> result;
  if (num < 1 || num > countPermutations() || !root) {
    return result;
  }

  int remaining = num - 1;
  std::vector<char> current;
  getPermHelper2(root, current, remaining, result);
  return result;
}

bool PMTree::getPermHelper2(std::shared_ptr<Node> node,
                            std::vector<char>& current, int& remaining,
                            std::vector<char>& result) const {
  if (node->value != '\0') {
    current.push_back(node->value);
  }

  if (node->children.empty()) {
    if (remaining == 0) {
      result = current;
      return true;
    }
    remaining--;
    return false;
  }

  for (const auto& child : node->children) {
    if (getPermHelper2(child, current, remaining, result)) {
      return true;
    }
  }

  if (node->value != '\0') {
    current.pop_back();
  }
  return false;
}

size_t PMTree::countPermutations() const {
  size_t count = 1;
  for (size_t i = 2; i <= element_count; ++i) {
    count *= i;
  }
  return count;
}

std::vector<std::vector<char>> getAllPerms(const PMTree& tree) {
  return tree.getAllPerms();
}

std::vector<char> getPerm1(const PMTree& tree, int num) {
  return tree.getPerm1(num);
}

std::vector<char> getPerm2(const PMTree& tree, int num) {
  return tree.getPerm2(num);
}

