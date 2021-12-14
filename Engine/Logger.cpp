#include "Logger.h"
#include "Utils.h"

#include <fstream>
#include <string>
#include <cstdlib>
#include <stdio.h>
#include <iomanip>
#include <climits>
#include <mutex>
#include <string.h>
#include <sstream>

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
    // std::cout << utils::log_task << std::endl;
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
    /* Initialization of scheduling.log */
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

    /* Initialization of id_2021_82006_read_write.log */
    std::cout << "Initializing read write log file" << std::endl;
    std::ofstream id_2021_82006_read_write;
    id_2021_82006_read_write.open(utils::cpsim_path + "/Log/id_2021_82006_read_write.log", std::ios::out);     
    std::string contents1 = "";
    contents1 += "[TASK NAME][TIME][READ/WRITE][DATA LENGTH][RAW DATA]\n";
    id_2021_82006_read_write.write(contents1.c_str(), contents1.size());
    id_2021_82006_read_write.close();

    /* Initialization of id_2021_82006_event.log */
    std::cout << "Initializing event log file" << std::endl;
    std::ofstream id_2021_82006_event;
    id_2021_82006_event.open(utils::cpsim_path + "/Log/id_2021_82006_event.log", std::ios::out);     
    std::string contents2 = "";
    contents2 += "[TIME][JOB ID][EVENT TYPE]\n";
    id_2021_82006_event.write(contents2.c_str(), contents2.size());
    id_2021_82006_event.close();

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

void Logger::id_2021_82006_task_read_write_logger(std::string task_name, bool read) {
    std::ofstream id_2021_82006_read_write;
    id_2021_82006_read_write.open(utils::cpsim_path + "/Log/id_2021_82006_read_write.log", std::ios::app);  
    // std::shared_ptr<DelayedData> write_data = global_object::delayed_data_write.front();
    // std::shared_ptr<TaggedData> read_data = global_object::tagged_data_read.front();
    std::string contents = "";
    std::stringstream sstream;
    contents += task_name + std::string(12 - 2,' ');
    if (strcmp(task_name.c_str(), "CC")) {
        if(read) {
            std::shared_ptr<TaggedData> read_data = global_object::tagged_data_read.at(global_object::tagged_data_read.size()-1);
            contents += std::to_string(read_data->data_time) + std::string(6 - std::to_string(read_data->data_time).length(),' ');
            contents += "READ" + std::string(13 - 5,' ');
            contents += std::to_string(std::to_string(read_data->data_read1).length()) + std::string(13 - std::to_string(std::to_string(read_data->data_read1).length()).length(),' ');
            sstream<< std::hex << read_data->data_read1;
            contents += sstream.str();
        }
        else{
            // std::shared_ptr<TaggedData> read_data = global_object::tagged_data_read.front();
            contents += "WRITE" + std::string(13 - 5,' ');
        }
        //contents += std::to_string(write_data->data_time) + (6 - std::to_string(write_data->data_time).size(),' ');
        // contents += std::to_string(delayed->data_time) + (6 - std::to_string(delayed->data_time).size(),' ');
        // contents += std::to_string(std::to_string(write_data->data_write4).length()) + (13 - std::to_string(write_data->data_write4).size(),' ');
        // sstream<< std::hex << write_data->data_write4;
        // contents += sstream.str();
        contents += "\n";
        id_2021_82006_read_write.write(contents.c_str(), contents.size());
        id_2021_82006_read_write.close();
    }

    else if (strcmp(task_name.c_str(), "LK")) {
        if(read) {
            std::shared_ptr<TaggedData> read_data = global_object::tagged_data_read.at(global_object::tagged_data_read.size()-1);
            contents += std::to_string(read_data->data_time) + std::string(6 - std::to_string(read_data->data_time).length(),' ');
            contents += "READ" + std::string(13 - 5,' ');
            contents += std::to_string(std::to_string(read_data->data_read1).length()) + std::string(13 - std::to_string(std::to_string(read_data->data_read1).length()).length(),' ');
            sstream<< std::hex << read_data->data_read1;
            contents += sstream.str();
        }
        else {
            // std::shared_ptr<TaggedData> read_data = global_object::tagged_data_read.front();
            contents += "WRITE" + std::string(13 - 5,' ');
        }
        // contents += std::to_string(read_data->data_time) + std::string(5,' ');
        // contents += std::to_string(std::to_string(read_data->data_read4).length()) + (13 - std::to_string(read_data->data_read4).size(),' ');
        // sstream<< std::hex << read_data->data_read4;
        // contents += sstream.str();
        contents += "\n";
        id_2021_82006_read_write.write(contents.c_str(), contents.size());
        id_2021_82006_read_write.close();
    }
}

void Logger::id_2021_82006_real_cyber_event_logger(long long time, int job_id, std::string event_type) {
    std::ofstream id_2021_82006_event;
    id_2021_82006_event.open(utils::cpsim_path + "/Log/id_2021_82006_event.log", std::ios::app);
    std::string contents = "";
    // std::shared_ptr<ScheduleData> current_data = global_object::schedule_data.front();

    contents += std::to_string(time) + std::string(6 - std::to_string(time).size(),' ');
    contents += std::to_string(job_id) + std::string(8 - std::to_string(job_id).size(),' ');

    if(strcmp(event_type.c_str(), "RELEASED")) {
        contents += "RELEASED";
    }

    else if(strcmp(event_type.c_str(), "STARTED")) {
        contents += "STARTED";
    }

    else if(strcmp(event_type.c_str(), "FINISHED")) {
        contents += "FINISHED";
    }

    else if(strcmp(event_type.c_str(), "FINISHED")) {
        contents += "FINISHED (DEADLINE MISS)";
    }

    contents += "\n";
    id_2021_82006_event.write(contents.c_str(), contents.size());
    id_2021_82006_event.close();
    id_2021_82006_event.close();
}
