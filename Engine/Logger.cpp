#include "Logger.h"
#include "Utils.h"

#include <fstream>
#include <string>
#include <cstdlib>
#include <stdio.h>
#include <iomanip>
#include <climits>
#include <mutex>
#include <tuple>

/**
 *  This file is the cpp file for the Logger class.
 *  @file Logger.cpp
 *  @brief cpp file for Engine-Logger
 *  @author Seonghyeon Park
 *  @date 2020-03-31
 */


/**
 * @fn Logger::Logger()
 * @brief the function of basic constructor of Logger
 * @author Seonghyeon Park
 * @date 2020-04-01
 * @details 
 *  - None
 * @param none
 * @return none
 * @bug none
 * @warning none
 * @todo none
 */
Logger::Logger()
{
    
}

/**
 * @fn Logger::~Logger()
 * @brief the function of basic destructor of Logger
 * @author Seonghyeon Park
 * @date 2020-04-01
 * @details 
 *  - None
 * @param none
 * @return none
 * @bug none
 * @warning none
 * @todo none
 */
Logger::~Logger()
{

}

/**
 * @fn void start_logging()
 * @brief this function starts the logging of simulation events
 * @author Seonghyeon Park
 * @date 2020-04-01
 * @details 
 *  - None
 * @param none
 * @return none
 * @bug none
 * @warning none
 * @todo none
 */

void Logger::set_schedule_log_info(std::vector<std::shared_ptr<Task>>& task_vector)
{
    std::ofstream scheduling_log;
    scheduling_log.open(utils::cpsim_path + "/Log/scheduling.log", std::ios::out);     
    std::string contents = "";
    for(int idx = 0; idx < task_vector.size(); idx++)
    {
        contents += "ECU" + std::to_string(task_vector.at(idx)->get_ECU()->get_ECU_id())+ ": " + task_vector.at(idx)->get_task_name();
        if(idx == task_vector.size() - 1)
            contents += "\n";
        else
        {
            contents += ", ";
        }
    }
    scheduling_log.write(contents.c_str(), contents.size());
    scheduling_log.close();
}

void Logger::start_logging()
{
    std::ofstream scheduling_log;
    while (std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - utils::simulator_start_time).count()  < utils::simulation_termination_time)
    {
        scheduling_log.open(utils::cpsim_path + "/Log/scheduling.log", std::ios::app);    
        utils::mtx_data_log.lock();
        if(global_object::schedule_data.size() > 10)
        {
            int min_idx = 0;
            std::shared_ptr<ScheduleData> current_data = global_object::schedule_data.front();
            for (int idx = 0; idx < global_object::schedule_data.size(); idx ++)
            {
                if(current_data->get_time() > global_object::schedule_data.at(idx)->get_time())
                {
                    current_data = global_object::schedule_data.at(idx);
                    min_idx = idx;
                }
            }
            
            global_object::schedule_data.erase(global_object::schedule_data.begin() + min_idx);
            scheduling_log.write(current_data->get_data().c_str(), current_data->get_data().size());
        }
        scheduling_log.close();
        utils::mtx_data_log.unlock();    
    }    
}

void Logger::_201616286_task_read_write_logger(std::string task_name, std::string type, int data_time, int nbytes, char *data)
{
    std::ofstream read_write_log;
    read_write_log.open(utils::cpsim_path + "/Log/_201616286_read_write.log", std::ios::app);    
    read_write_log << std::left;
    read_write_log << std::setw(10) << task_name;
    read_write_log << std::setw(10) << data_time;
    read_write_log << std::setw(10) << type;
    read_write_log << std::setw(10) << nbytes;
    
    for (int i = 0 ; i < nbytes ; i += 1)
        read_write_log << "0x" << std::setw(2) << std::setfill('0') << std::hex << (0x000000ff & (int) data[i]) << " ";
    read_write_log << std::endl;
    read_write_log.close();
}

void Logger::_201616286_real_cyber_event_logger(long long time, int task_id, int job_id, std::string event_type)
{
    long long t;
    std::string j, e;
    std::ofstream event_log;

    this -> event_queue.push(std::make_tuple(-time, "J" + std::to_string(task_id) + std::to_string(job_id), event_type));
    event_log.open(utils::cpsim_path + "/Log/_201616286_event.log", std::ios::app);    

    while ((!this->event_queue.empty()) && ((-std::get<0>(this -> event_queue.top())) <= utils::current_time)) {
        
        std::tie(t, j, e) = event_queue.top();
        event_log << std::left;
        event_log << std::setw(10) << -t;
        event_log << std::setw(10) << j;
        event_log << std::setw(10) << e;
        event_log << std::endl;
        event_queue.pop();
    }
    event_log.close();
}

