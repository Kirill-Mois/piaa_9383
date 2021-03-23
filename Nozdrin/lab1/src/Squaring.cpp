#include "Squaring.h"

Squaring::Squaring(int size) {
  this->m_size = size;
  m_current.resize(m_size);
  for (int i = 0; i < m_size; i++) {
    m_current[i].resize(m_size);
    for (int j = 0; j < m_size; j++)
      m_current[i][j] = 0;
  }
}
int Squaring::getMaxSquareSize(int i, int j) {
  int maxSize;
  for (maxSize = 1; maxSize < std::min(m_size-i, m_size-j); maxSize++) {
    for (int k = 0; k < maxSize; k++)
      if (m_current[i + k][j + maxSize] != 0) {
        return maxSize;
      }
    for (int k = 0; k <= maxSize; k++)
      if (m_current[i + maxSize][j + k] != 0) {
        return maxSize;
      }
  }
  return (maxSize < m_size) ? maxSize : 0;
}
bool Squaring::placeSquare(int i, int j, int size) {
  if ((i < 0) || (j < 0) || (i >= m_size) || (j >= m_size))
    return false;
  if (m_current[i][j] != 0)
    return false;
  if (size == 0) {
    size = getMaxSquareSize(i, j);
    std::cerr << "placeSquare: size = " << size << "\n";
  }
  m_counter++;
  for (int k_i = 0; k_i < size; k_i++)
    for (int k_j = 0; k_j < size; k_j++)
      m_current[i+k_i][j+k_j] = m_counter;
  return true;
}
std::vector<std::vector<int>> Squaring::getCurrentSquaring(){
  return m_current;
};
void Squaring::setCurrentSquaring(std::vector<std::vector<int>> squaring){
  m_current = std::move(squaring);
};
int Squaring::getCounter() const {
  return m_counter;
};
void Squaring::eval() {
  m_minimalSquaring = 2*m_size;
  this->baseCase1();
  while (true) {
    if (this->complete())
      if (m_counter < m_minimalSquaring) {
        m_squares_best = m_squares;
        m_best = m_current;
        m_minimalSquaring = m_counter;
      }
    if (!backtrack())
      break;
  }
}
void Squaring::run() {
//  if (m_size % 2 == 0) {
//    int a = m_size/2;
//    std::cout<< "4\n1 1 "             << a << '\n';
//    std::cout<< a+1 <<' '<< 1   <<' ' << a << '\n';
//    std::cout<< 1   <<' '<< a+1 <<' ' << a << '\n';
//    std::cout<< a+1 <<' '<< a+1 <<' ' << a << std::endl;
//    return;
//  }
//  if (m_size % 3 == 0) {
//    multiplier = m_size/3;
//    m_size = 3;
//  }
//  if (m_size % 5 == 0) {
//    multiplier = m_size/5;
//    m_size = 5;
//  }
  eval();
  printResult();
}
void Squaring::printResult() {
  std::cout << m_minimalSquaring << '\n';
  printSquares(m_squares_best);
}
void Squaring::printSquares(const std::stack<Square>& squares) const {
  auto copy = squares;
  while (!copy.empty()) {
    std::cout << copy.top().m_x*multiplier+1 << ' ' << copy.top().m_y*multiplier+1 << ' '
              << copy.top().m_size*multiplier << std::endl;
    copy.pop();
  }
}
void Squaring::baseCase1() {
  m_counter = 1;
  for (int i = 0; i < m_size-1; i++)
    for (int j = 0; j < m_size-1; j++)
      m_current[i][j] = m_counter;
  m_squares.push(Square(0, 0, m_size-1));

  for (int i = 0; i < m_size-1; i++) {
    m_counter++;
    m_current[i][m_size-1] = m_counter;
    m_squares.push(Square(i, m_size - 1, 1));
  }
  for (int i = 0; i < m_size; i++) {
    m_counter++;
    m_current[m_size-1][i] = m_counter;
    m_squares.push(Square(m_size - 1, i, 1));
  }
}
void Squaring::baseCase2() {
  for (int i = 0; i < m_size; i++)
    for (int j = 0; j < m_size; j++) {
      if ((i < m_size/2+1) && (j < m_size/2+1))
        m_current[i][j] = 1;
      else if (j < m_size/2)
        m_current[i][j] = 2;
      else if (i < m_size/2)
        m_current[i][j] = 3;
    }
  m_counter = 3;
  m_squares.push(Square(0, 0, m_size / 2 + 1));
  m_squares.push(Square(m_size / 2 + 1, 0, m_size / 2));
  m_squares.push(Square(0, m_size / 2 + 1, m_size / 2));
}
std::ostream& operator<<(std::ostream& os, Squaring& squaring) {
  for (int i = 0; i < squaring.m_size; i++) {
    for (int j = 0; j < squaring.m_size; j++)
      os << squaring.m_current[i][j] << ' ';
    os << std::endl;
  }
//  std::cout << squaring.m_minimalSquaring << '\n';
//  for (int i = 0; i < squaring.m_minimalSquaring; i++) {
//    Square tmp = squaring.m_squares_best.top();
//    squaring.pop();
//    os  << tmp.m_x * squaring.multiplier + 1 << ' '
//        << tmp.m_y * squaring.multiplier + 1 << ' '
//        << tmp.m_size * squaring.multiplier << std::endl;
//  }
  return os;
}
bool Squaring::complete() {
  std::cout << *this;
  std::cerr << "complete...\n";
  for (int i = 0; i < m_size; i++)
    for (int j = 0; j < m_size; j++) {
      if (m_current[i][j] != 0)
        continue;
      this->placeSquare(i, j);
//      if (this->placeSquare(i, j))
//        j += m_squares.top().m_size;
      if (m_counter > m_minimalSquaring) {
        std::cerr << "bad solution, (>min)\n";
        return false;
      }
    }
  std::cout << *this;
  std::cerr << "complete -> true\n";
  return true;
}
void Squaring::pop() {
  Square tmp = m_squares.top();
  for (int i = tmp.m_y; i < tmp.m_y + tmp.m_size; i++)
    for (int j = tmp.m_x; j < tmp.m_x + tmp.m_size; j++)
      m_current[i][j] = 0;
  m_squares.pop();
  m_counter--;
}
bool Squaring::backtrack() {
  std::cerr << "backtrack...\n";
  Square tmp = m_squares.top();
  while ((tmp.m_size == 1) && (!m_squares.empty())) {
    this->pop();
    tmp = m_squares.top();
  }
  if (m_squares.empty())
    return false;
  int x = tmp.m_x;
  int y = tmp.m_y;
  int size = tmp.m_size;
  this->pop();
  placeSquare(x, y, size-1);
  std::cerr << "backtrack->true\n";
  return true;
}
