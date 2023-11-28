## report 1

### Important changes

* increase speed to 80km/h.
* the lap time is around 1min 16sec in lots of tests(first lap).
* only 1 Ecu has been used in total.
* all the SWC parameters are set to 200,200,40,40.

***

### How did I do it

*  Firstly configure all parts according to the baseline given, and then increase the speed in the CC.cpp file. As a result, as long as the speed is increased a little, the car will lose control and hit the wall. Therefore, I think that the direction adjustment should be more frequent and meticulous than the speed adjustment, and in order to perform the task more frequently, it is more beneficial to use additional ECUs. Even if there are additional penalties.
*  But in the mid presentation, TA told us that the max of speed is 80, so I change the design idea to give partial stability and using just one ECU to avoid the penalty. Because the max speed is 80, which is not very fast to need pretty much stablility.

* I think it is the highest priority to minimize the period of LK, so the period of LK is set at 300 and the execution time is set at 30 . However, due to the frequent start, the body repeatedly vibrates and falls into an unstable state, and then hits the wall. So I set the LK and CC to 200, 30. And the steering was updated to 3000 in the LK.cpp file. This time it was found that the body vibration was significantly reduced and more stable.

* After that I started to increase the speed and found that read2 = READ2/100 ensured the fastest speed and the car would not hit the wall. 

***

## Report 2

### Case 1

***

* To implement logging for case1, the logging function is first declared in log.h and implemented in log.cpp. 
* The logging function is a function that takes in the task name, Read Data, and Write Data and outputs them in the appropriate format. 
* Then call the above function at the appropriate location in jop.cpp. Its location is inside Job:runfunction(). Job:runfunction processes read and write. 
* Therefore, if get-is-read () and get_is_write() are true, Read Data and Write Data are handed over to the logging function for output, respectively.

***

### case 2

***

* Same as case1, in order to implement schedule logging, the logging function is implemented first in the log.cpp. The logging function is a function that receives the time, job id, and event type and outputs them in the appropriate format.
* Then call the above function at the appropriate location of executor.cpp. Its location is inside Executor:runsimulation. 
* First, I found the job release section. In this position, if job->get_actual_start_time() < 0, or job->get_actual_finish_time() < job->get_actual_deadline(), Output 'FINISHED' event log, otherwise output 'RELEASED'. 
* Then I found the job start section. Its location is the part where run_job = job is set. Output the "STARTED" event log at this location.

