#include "LK.h"


void sim_main()
{
  double STEERING = 1500.0;

  if (rtU->read2 <= STEERING / 2)
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
    else if ((rtU->read1 < 0.0) && (rtU->read1 < -STEERING))
    {
      rtDW->w3 = 5000.0;
    }
    else
    {
      if ((rtU->read1 < 0.0) && (rtU->read1 >= -STEERING))
      {
        //rtDW->w3 = rtU->read1 * -1;
        rtDW->w4 = 0.0;
        if(rtDW->w3 > rtU->read1) { // smooth steer
          rtDW->w3 -= 100.0;
        }
        else {
          rtDW->w3 += 100.0;
        }
      }
    }

    if ((rtU->read1 > 0.0) && (rtU->read1 > STEERING))
    {
      rtDW->w4 = 5000.0;
    }
    else if ((rtU->read1 > 0.0) && (rtU->read1 <= STEERING))
    {
      //rtDW->w4 = rtU->read1;
      rtDW->w3 = 0.0;
      if(rtDW->w4 < rtU->read1) { // smooth steer
        rtDW->w4 += 100.0;
      }
      else {
        rtDW->w4 -= 100.0;
      }
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