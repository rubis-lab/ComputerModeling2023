#ifndef LOGGER_H__
#define LOGGER_H__
#include <iostream>
#include <vector>
#include <queue>
#include "Job.h"
#include "TaggedData.h"
#include "DelayedData.h"

/** This file is engine code of CPSim-Re engine
 * @file Logger.h
 * @class Logger
 * @author Seonghyeon Park
 * @date 2020-04-30
 * @brief Codes for Engine-Logger 
*/

struct Event{
    long long time;
    int job_id;
    std::string log;
};

struct compare{
    bool operator()(Event a, Event b){
        if(a.time == b.time)
            return a.job_id > b.job_id;
        return a.time > b.time;
    }
};

class Logger{
private:
    std::vector<std::shared_ptr<Job>> m_execution_order_buffer;
    std::vector<double> m_current_time_buffer;
    std::priority_queue<Event, std::vector<Event>, compare> event_buffer;
    
public:
    /**
     * Constructors and Destructors
     */
    Logger();
    ~Logger();

    /**
     * Getter & Setter
     */

    std::vector<std::shared_ptr<Job>> get_execution_order_buffer();
    void set_execution_order_buffer(std::vector<std::shared_ptr<Job>>);
    void add_current_simulated_job(std::shared_ptr<Job>);
    void start_logging();
    void log_task_vector_status(std::vector<std::shared_ptr<Task>>&);
    void log_job_vector_of_each_ECU_status(std::vector<std::vector<std::vector<std::shared_ptr<Job>>>>&);
    void log_job_vector_of_simulator_status(std::vector<std::shared_ptr<Job>>&);
    void log_which_job_was_deadline_miss(std::shared_ptr<Job>);
    void print_job_execution_on_ECU(std::vector<std::shared_ptr<Job>>, std::vector<std::shared_ptr<Job>>, int);
    void print_job_execution_schedule();
    void print_offline_guider_status();
    void set_schedule_log_info(std::vector<std::shared_ptr<Task>>&);

    void id_2021_82006_task_read_write_logger(std::string task_name, bool read);
    void id_2021_82006_real_cyber_event_logger(long long time, int job_id, std::string event_type);

/*
    std::string _2019_13914_print_tagged_data_log(std::string, std::shared_ptr<TaggedData>, int);
    std::string _2019_13914_print_delayed_data_log(std::string, std::shared_ptr<DelayedData>, int);
    void _2019_13914_task_read_write_logger(std::string);
    void _2019_13914_real_cyber_event_logger(long long, int, std::string);
*/
/*
    void _201717288_task_read_write_logger(std::string task_name, std::shared_ptr<TaggedData> read_data, std::shared_ptr<DelayedData> write_data);
    void _201717288_real_cyber_event_logger(long long time, int job_id, std::string event_type);
*/

};

#endif