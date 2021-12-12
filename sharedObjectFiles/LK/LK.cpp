#include "LK.h"

void sim_main()
{
  #define STEERING_VALUE_VKJY1 7000.0 //IF.
  #define STEERING_VALUE_VKJY2 5000.0 //Write value.
  if (rtU->read2 <= 5000.0)
  {
    rtDW->w3 = 0.0;
    rtDW->w4 = 0.0;
  }
  else
  {
    if (rtU->read1 > 0.0)
    {
      rtDW->w3 = 0.0;
    }
    else if ((rtU->read1 < 0.0) && (rtU->read1 < -STEERING_VALUE_VKJY1))
    {
      rtDW->w3 = STEERING_VALUE_VKJY2;
    }
    else
    {
      if ((rtU->read1 < 0.0) && (rtU->read1 >= -STEERING_VALUE_VKJY1))
      {
        rtDW->w3 = rtU->read1 * -1;
      }
    }

    if ((rtU->read1 > 0.0) && (rtU->read1 > STEERING_VALUE_VKJY1))
    {
      rtDW->w4 = STEERING_VALUE_VKJY2;
    }
    else if ((rtU->read1 > 0.0) && (rtU->read1 <= STEERING_VALUE_VKJY1))
    {
      rtDW->w4 = rtU->read1;
    }
    else
    {
      if (rtU->read1 < 0.0)
      {
        rtDW->w4 = 0.0;
      }
    }
  }
  rtY->write3 = rtDW->w3;
  rtY->write4 = rtDW->w4;
}
