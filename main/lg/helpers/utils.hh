
#pragma once

inline bool is_up_bit_pos(int value, short pos = 0)
{
  short bit_pos = (value >> pos) & 1;

  if (bit_pos == 1) {
    return true;
  }

  return false;
}

/*
 *
* 0000 1110  (это a >> 2)
  & 0000 0001  (это 1)
  -----------
  0000 0000  <-- Result 0
 *
 *
 *  pos count from zero
 */