#include "LK.h"

void sim_main()
{
  double lk = 8000.0;
  double lk_cond = 6000.0;
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
    else if ((rtU->read1 < 0.0) && (rtU->read1 < -lk_cond))
    {
      rtDW->w3 = lk;
    }
    else
    {
      if ((rtU->read1 < 0.0) && (rtU->read1 >= -lk_cond))
      {
        rtDW->w3 = rtU->read1 * -1;
      }
    }

    if ((rtU->read1 > 0.0) && (rtU->read1 > lk_cond))
    {
      rtDW->w4 = lk;
    }
    else if ((rtU->read1 > 0.0) && (rtU->read1 <= lk_cond))
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
