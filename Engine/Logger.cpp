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
    std::ofstream read_write_log;
    read_write_log.open(utils::cpsim_path + "/Log/_2023-81570_read_write.log");
    //[TASK NAME][TIME][READ/WRITE][DATA LENGTH][RAW DATA]
    //case #1
    read_write_log << "[TIME][READ/WRITE][TASK NAME][DATA NAME]" << std::endl;
    read_write_log.close();
    //case #2
    std::ofstream event_log;
    event_log.open(utils::cpsim_path + "/Log/_2023-81570_schedule.log");
    event_log << "[TIME][JOB ID][EVENT TYPE]" << std::endl;
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

void Logger::_2023_81570_task_read_write_logger(std::string task_name,std::shared_ptr<TaggedData> read_data,std::shared_ptr<DelayedData> write_data)
{
    std::ofstream read_write_log;
    read_write_log.open(utils::cpsim_path + "/Log/_2023-81570_read_write.log", std::ios::app);
    

    if(read_data != nullptr){
        std::shared_ptr<TaggedData> current_data = read_data;
        read_write_log << std::left << std::setw(6) << current_data->data_time;
        read_write_log << std::left << std::setw(12) << "READ";
        read_write_log << std::left << std::setw(10) << task_name << "\t";
        if(current_data->data_read1)
            read_write_log << std::left << std::setw(13) << "TACCEL_VALUE" << "\t";
        if(current_data->data_read2)
            read_write_log << std::left << std::setw(13) << "TARGET_SPEED" << "\t";
        if(current_data->data_read3)
            read_write_log << std::left << std::setw(13) << "SPEED" << "\t";
        if(current_data->data_read4)
            read_write_log << std::left << std::setw(13) << "TRIGGER" << "\t";
        read_write_log<< std::endl;
    }
    else{
        std::shared_ptr<DelayedData> current_data = write_data;
        read_write_log << std::left << std::setw(6) << current_data->data_time;
        read_write_log << std::left << std::setw(12) << "WRITE";
        read_write_log << std::left << std::setw(10) << task_name << "\t";
        if(current_data->data_write1)
            read_write_log << std::left << std::setw(13) << "Send_BRAKE" << "\t";
        if(current_data->data_write2)
            read_write_log << std::left << std::setw(13) << "Send_ACCEL" << "\t";
        read_write_log<< std::endl;
    }
    read_write_log.close();
}

void Logger::_2023_81570_real_cyber_event_logger(long long time, int job_id, std::string event_type)
{
    std::ofstream event_log;
    event_log.open(utils::cpsim_path + "/Log/_2023-81570_schedule.log", std::ios::app);
    event_log<< std::left << std::setw(6) << time;
    event_log<< "J";
    event_log<< std::left << std::setw(8) << job_id;
    event_log<< std::left << std::setw(12) << event_type << std::endl;
}