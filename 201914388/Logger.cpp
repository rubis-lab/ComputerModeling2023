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

void Logger::set_task_read_write_logger_201914388() {
    std::ofstream trw_log;
    trw_log.open(utils::cpsim_path + "/Log/201914388_read_write.log", std::ios::out);

    std::string contents = "[ TIME ][ READ/WRITE ][ TASK NAME ][ DATA NAME ]\n";
    trw_log.write(contents.c_str(), contents.size());
    trw_log.close();
}

void Logger::task_read_write_logger_201914388(const std::string& task_name) {
    static bool is_first = true;
    if (is_first) {
        this->set_task_read_write_logger_201914388();
        is_first = false;
    }
    std::ofstream scheduling_log;
    scheduling_log.open(utils::cpsim_path + "/Log/201914388_read_write.log", std::ios::app);
    std::string contents = "";
    std::string time = "";
    if (task_name == "READ") {
        std::shared_ptr<TaggedData> current_data = global_object::tagged_data_read.at(global_object::tagged_data_read.size()-1);
        time = std::to_string(current_data->data_time);
        if (utils::log_task == "CC") {
            std::vector<std::string> CC_req = {"ACCEL_VALUE", "TARGET_SPEED", "TRIGGER", "Recv_SPEED"};
            for (std::string req : CC_req) {
                contents += time + "\t";
                contents += task_name + "\t";
                contents += utils::log_task + "\t";
                contents += req + "\n";
            }
        } else if (utils::log_task == "LK") {
            std::vector<std::string> LK_req = {"read1", "read2"};
            for (std::string req : LK_req) {
                contents += time + "\t";
                contents += task_name + "\t";
                contents += utils::log_task + "\t";
                contents += req + "\n";
            }
        }
    } else if (task_name == "WRITE") {
        std::shared_ptr<DelayedData> current_data = global_object::delayed_data_write.at(global_object::delayed_data_write.size()-1);
        time = std::to_string(current_data->data_time);
        if (utils::log_task == "CC") {
            std::vector<std::string> CC_req = {"BRAKE", "ACCEL"};
            for (std::string req : CC_req) {
                contents += time + "\t";
                contents += task_name + "\t";
                contents += utils::log_task + "\t";
                contents += req + "\n";
            }
        } else if (utils::log_task == "LK") {
            std::vector<std::string> LK_req = {"write3", "write4"};
            for (std::string req : LK_req) {
                contents += time + "\t";
                contents += task_name + "\t";
                contents += utils::log_task + "\t";
                contents += req + "\n";
            }
        }
        global_object::delayed_data_write.clear();
    }
    scheduling_log.write(contents.c_str(), contents.size());
    scheduling_log.close();
}

void Logger::real_cyber_schedule_logger_201914388(long long time, int job_id, std::string event_type) {
    static bool is_first = true;
    if (is_first) {
        std::ofstream trw_log;
        trw_log.open(utils::cpsim_path + "/Log/201914388_schedule.log", std::ios::out);

        std::string contents = "[ TIME ][ JOB ID ][ EVENT TYPE ]\n";
        trw_log.write(contents.c_str(), contents.size());
        trw_log.close();
        is_first = false;
    }
    std::ofstream scheduling_log;
    scheduling_log.open(utils::cpsim_path + "/Log/201914388_schedule.log", std::ios::app);
    std::string contents = "";
    contents += std::to_string(time) + "\t";
    contents += std::to_string(job_id) + "\t";
    contents += event_type + "\n";
    scheduling_log.write(contents.c_str(), contents.size());
    scheduling_log.close();
}
