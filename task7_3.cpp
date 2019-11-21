/*
 * Дано число N и N строк.
 * Каждая строка содержит команду добавления или удаления натуральных чисел,
 * а также запрос на получение k-ой порядковой статистики.
 * Команда добавления числа A задается положительным числом A,
 * команда удаления числа A задается отрицательным числом “-A”.
 * Запрос на получение k-ой порядковой статистики задается числом k.
 * Требуемая скорость выполнения запроса - O(log n) в среднем. Для решения задачи реализуйте декартово дерево.
 */

#include <iostream>

struct TreapNode {
  explicit TreapNode(int _value, int _priority): value(_value), priority(_priority), child_counter(1) {};
  int value;
  int priority;
  int child_counter;
  TreapNode* right = nullptr;
  TreapNode* left = nullptr;
};

class Treap {
 public:
  void Add(int value);
  void Delete(int value);

 private:
  TreapNode* root;
  std::pair<TreapNode*, TreapNode*> Split(TreapNode* node, int x);
  TreapNode* Merge(TreapNode* left, TreapNode* right);

  void UpdateChildCounter();
};

std::pair<TreapNode*, TreapNode*> Treap::Split(TreapNode *node, int x) {
  if (!node) {
    return std::make_pair(nullptr, nullptr);
  }
  if (node->value < x) {
    const auto right_pair = Split(node->right, x);
    node->right = right_pair.first;
    return std::make_pair(node, right_pair.second);
  } else {
    const auto left_pair = Split(node->left, x);
    node->left = left_pair.second;
    return std::make_pair(left_pair.first, node);
  }
}

TreapNode* Treap::Merge(TreapNode *left, TreapNode *right) {
  if (!left || !right) {
    return left == nullptr ? right : left;
  }
  if (left->priority > right->priority) {
    left->right = Merge(left->right, right);
    return left;
  } else {
    right->left = Merge(left, right->left);
    return right;
  }
}

void Treap::Add(int value) {
  auto childs = Split(root, value);
  TreapNode* temp_root = Merge(childs.first, new TreapNode(value, value));
  root = Merge(temp_root, childs.second);
}

void Treap::Delete(int value) {
  auto childs = Split(root, value);
  auto left_childs = Split(childs.first, value-1);
  root = Merge(left_childs.first, childs.second);
}

int main() {
  int n;
  std::cin >> n;
  Treap treap;
  for (int i = 0; i < n; i++) {
    int value, k;
    std::cin >> value;
    if (value < 0) {
      treap.Delete(abs(value));
    } else {
      treap.Add(value);
    }
  }
  return 0;
}