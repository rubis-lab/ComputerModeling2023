#include "Logger.h"
#include "Utils.h"

#include <fstream>
#include <string>
#include <cstdlib>
#include <stdio.h>
#include <iomanip>
#include <climits>
#include <mutex>

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
    std::ofstream scheduling_log;
    scheduling_log.open(utils::cpsim_path + "/Log/2018_11150_read_write.log", std::ios::out|std::ios::trunc);     
    
    if(!scheduling_log){
        std::cerr << "Failed to open log file.\n";
        exit(1);
    }

    scheduling_log << "[ TASK NAME ] [ TIME ] [ READ/WRITE ] [ DATA LENGTH ] [ RAW DATA ]\n";
    scheduling_log.close();


    std::ofstream event_log;
    event_log.open(utils::cpsim_path + "/Log/2018_11150_event.log", std::ios::out|std::ios::trunc);     
    
    if(!event_log){
        std::cerr << "Failed to open log file.\n";
        exit(1);
    }

    event_log << "[TIME][JOB ID][EVENT TYPE]\n";
    event_log.close();
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

std::string Logger::_2018_11150_task1_tagged(std::string task_name, std::shared_ptr<TaggedData> data){
    std::stringstream content;
    content << std::left << std::setw(14) << task_name;
    content << std::left << std::setw(9) << std::to_string(data->data_time);
    content << std::left << std::setw(15) << "READ";
    content << std::left << std::setw(16) << std::to_string(6*sizeof(int));
    content << std::left << std::hex << "0x" << data->data_read1 << " "
                         << std::hex << "0x" << data->data_read2 << " "
                         << std::hex << "0x" << data->data_read3 << " "
                         << std::hex << "0x" << data->data_read4 << " "
                         << std::hex << "0x" << data->data_read5 << " "
                         << std::hex << "0x" << data->data_read6 << "\n";
    return content.str();
}
std::string Logger::_2018_11150_task1_delayed(std::string task_name, std::shared_ptr<DelayedData> data){
    std::stringstream content;
    content << std::left << std::setw(14) << task_name;
    content << std::left << std::setw(9) << std::to_string(data->data_time);
    content << std::left << std::setw(15) << "READ";
    content << std::left << std::setw(16) << std::to_string(4*sizeof(int));
    content << std::left << std::hex << "0x" << data->data_write1 << " "
                         << std::hex << "0x" << data->data_write2 << " "
                         << std::hex << "0x" << data->data_write3 << " "
                         << std::hex << "0x" << data->data_write4 << "\n";
    return content.str();
}

void Logger::_2018_11150_task_read_write_logger(std::string contents)
{
    std::ofstream scheduling_log;
    scheduling_log.open(utils::cpsim_path + "/Log/2018_11150_read_write.log", std::ios::app);     
    
    if(!scheduling_log){
        std::cerr << "Failed to open log file.\n";
        exit(1);
    }

    scheduling_log << contents;
    scheduling_log.close();
}

void Logger::_2018_11150_real_cyber_event_logger(long long time, int job_id, std::string event_type){
    std::ofstream event_log;
    event_log.open(utils::cpsim_path + "/Log/2018_11150_event.log", std::ios::app);     
    
    if(!event_log){
        std::cerr << "Failed to open log file.\n";
        exit(1);
    }

    event_log << std::left << std::setw(6) << std::to_string(time);
    event_log << std::left << std::setw(8) << "J" + std::to_string(job_id);
    event_log << std::left << event_type << "\n";
    event_log.close();
}
