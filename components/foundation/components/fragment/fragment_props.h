

#pragma once

namespace foundation
{
  class Fragment;
  struct FragmentProps final : BaseProps<FragmentProps, Fragment>
  {
    static FragmentProps up() { return FragmentProps{}; }
  };
}