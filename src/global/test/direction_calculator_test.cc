//
// Summary: buzz source code.
//
// Author: Tony.
// Email: tonyjobmails@gmail.com.
// Last modify: 2014-01-28 09:45:49.
// File name: direction_calculator_test.cc
//
// Description:
// Define class DirectionCalculatorTest.
//

#include "global/direction_calculator.h"

const size_t kGroupNum = 10;

static const int positions[kGroupNum][5] = {
  {  0,  1,  2,  3, entity::DirectionType::SOUTHEAST, },
  {  0,  1,  3,  1, entity::DirectionType::EAST, },
  {  1,  0,  0,  3, entity::DirectionType::SOUTH, },
  {  3,  2,  2,  0, entity::DirectionType::NORTHWEST, },
  {  1,  4,  2,  3, entity::DirectionType::NORTHEAST, },
  { -1,  2, -2, -4, entity::DirectionType::NORTH, },
  { -3,  4,  4, -3, entity::DirectionType::NORTHEAST, },
  { -3,  4, -3,  2, entity::DirectionType::NORTH, },
  {  4, -2,  1,  5, entity::DirectionType::SOUTHWEST, },
  { -4,  2, -1, -5, entity::DirectionType::NORTHEAST, },
};

int main() {
  if(global::DirectionCalculator::GetInstance()->Initialize() == false) {
    return 0;
  }

  size_t failure_count = 0;

  for(size_t pos = 0; pos < kGroupNum; ++pos) {
    entity::DirectionType::type direction =
      global::DirectionCalculator::GetInstance()->GetGridDirection(positions[pos][0],
          positions[pos][1], positions[pos][2], positions[pos][3]);
    /*
    if(direction >= entity::DirectionType::MAX) {
      printf("error, (%d,%d)->(%d,%d)\n", positions[pos][0], positions[pos][1],
          positions[pos][2], positions[pos][3]);
    } else {
      printf("grid direction: (%d,%d)->(%d,%d) dir=%d\n", positions[pos][0], positions[pos][1],
          positions[pos][2], positions[pos][3], direction);
    }
    */
    if(direction != positions[pos][4]) {
      printf("Test failed, (%d,%d)->(%d,%d), direction=%d, should be(%d)\n", positions[pos][0], positions[pos][1],
          positions[pos][2], positions[pos][3], direction, positions[pos][4]);
      ++failure_count;
    }
  }

  if(failure_count == 0) {
    printf("Pass all!!\n");
  } else {
    printf("%zd failed\n", failure_count);
  }

  global::DirectionCalculator::GetInstance()->Finalize();

  return 0;
}

