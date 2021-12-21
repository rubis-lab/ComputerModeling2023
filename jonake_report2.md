Case #1:
For this task, the logger function was placed on line 589, in the run_function(). 
The function is executed in the if statement that checks if there are both read and write constraints, since this is the case for both LK and CC.

Case #2:
The logger function was placed at 4 different postitions in the code.
Release was placed at line 144, since that is right after where the simulated release time is set for the job.
Similarly finish and start was set immediately after they are set.
Finished but with deadline miss was placed on line 116, since this is where the deadline is checked.