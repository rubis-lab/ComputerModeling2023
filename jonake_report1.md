In order to get a faster speed compared to the baseline, the first test was intuitively to increase the speed in CC.cpp.
This, however, resulted in the car steering in a very unstable way and sidetracking off the road. 
To fix this, the execution time and period should be reduced to check and adjust the car’s position with a higher frequency.

Baseline:
The baseline model uses a period time of 500 ms, utilization 10% for  LK and CC, and utilization of 20% for the remaining four jobs. This gives a period of 500 ms with total utilization of 100% (2*10 + 4*20).
The baseline model's track time was recorded to 2:24.

To improve this, it is desirable to shorten the period and execution times. 
This is especially true for steering (LK), in order to maintain a smooth and stable course through the track.
Another function to keep at a high update frequency is CC to make sure that the speed is as constant as possible.

Improvements:
The first parameter to change was the period and deadline times. Given the restraints in the project, it is obvious that at maximum one job can have execution times <= 100ms (since it gives utilization of 50%). 
For somewhat simpler calculations, the LK and CC were decided to have the same execution and period times at half of the remaining tasks. Since a total period time of 200 ms would lead to the LK and CC having a 100 ms period (not possible as described above), 300 ms was chosen instead. This gives LK and CC a period time of 150 ms. Using the lowest allowed utilization for all jobs resulted in all jobs having 30 ms execution times. Total utilization is 80% (2*20 + 4*10). This allowed for the speed in CC.cpp to be increased to 70 kph (75% increase). After running the simulation, the best lap time was recorded to 1:28, which is almost a minute improvement.
