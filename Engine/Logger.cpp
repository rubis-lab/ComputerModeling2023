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
    std::ofstream rw_logfile;
    rw_logfile.open(utils::cpsim_path + "/Log/_2021-82380_read_write.log", std::ios::out);
    rw_logfile << "[TASK NAME][TIME][READ/WRITE][DATA LENGTH][RAW DATA]" << std::endl;
    rw_logfile.close();

    std::ofstream event_logfile;
    event_logfile.open(utils::cpsim_path + "/Log/_2021_82380_event.log", std::ios::out);
    event_logfile << "[ TIME ] [ JOB ID ][ EVENT TYPE ]" << std::endl;
    event_logfile.close();
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

void Logger::SID_2021_82380_task_read_write_logger(std::string task_name, std::shared_ptr<TaggedData> taggedData, std::shared_ptr<DelayedData> delayedData) {
    std::ofstream rw_logfile;
    rw_logfile.open(utils::cpsim_path + "/Log/_2021-82380_read_write.log", std::ios::app);
    rw_logfile << std::setw(11) << std::left << task_name;
    
    // Read
    if(taggedData != nullptr) {
        rw_logfile << std::setw(6)  << std::left << taggedData->data_time;
        rw_logfile << std::setw(12) << std::left << "READ";
        rw_logfile << std::setw(13) << std::left << 6;
        rw_logfile << "0x" << std::hex << taggedData->data_read1 << " ";
        rw_logfile << "0x" << std::hex << taggedData->data_read2 << " ";
        rw_logfile << "0x" << std::hex << taggedData->data_read3 << " ";
        rw_logfile << "0x" << std::hex << taggedData->data_read4 << " ";
        rw_logfile << "0x" << std::hex << taggedData->data_read5 << " ";
        rw_logfile << "0x" << std::hex << taggedData->data_read6 << std::endl;
    } else { // Write
        rw_logfile << std::setw(6)  << std::left << delayedData->data_time;
        rw_logfile << std::setw(12) << std::left << "WRITE";
        rw_logfile << std::setw(13) << std::left << 4;
        rw_logfile << "0x" << std::hex << delayedData->data_write1 << " ";
        rw_logfile << "0x" << std::hex << delayedData->data_write2 << " ";
        rw_logfile << "0x" << std::hex << delayedData->data_write3 << " ";
        rw_logfile << "0x" << std::hex << delayedData->data_write4 << std::endl;
    }

    rw_logfile.close();
}

void Logger::SID_2021_82380_real_cyber_event_logger(long long time, int job_id, std::string event_type) {
    std::ofstream event_logfile;
    event_logfile.open(utils::cpsim_path + "/Log/_2021_82380_event.log", std::ios::app);
    event_logfile << std::setw(9) << std::left << time;
    event_logfile << std::setw(10) << std::left << job_id;
    event_logfile << std::setw(14) << std::left << event_type << std::endl;
}