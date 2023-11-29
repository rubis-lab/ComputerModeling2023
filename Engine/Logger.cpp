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
    //std::ofstream rw_log;
    scheduling_log.open(utils::cpsim_path + "/Log/scheduling.log", std::ios::out); 
    //rw_log.open(utils::cpsim_path + "/Log/VictorMihaila_read_write.log", std::ios::out);    
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
    //std::string s=std::to_string(global_object::running_job->get_actual_start_time())+"\n";
    scheduling_log.write(contents.c_str(), contents.size());
    //rw_log.write(s.c_str(), s.size());
    scheduling_log.close();
    //rw_log.close();
    //VictorMihaila_task_read_write_logger(utils::log_task);
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

void VictorMihaila_task_read_write_logger(std::string task_name){
    /*while (std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - utils::simulator_start_time).count()  < utils::simulation_termination_time)
    {
        std::ofstream rw_log;
        rw_log.open(utils::cpsim_path + "/Log/VictorMihaila_read_write.log", std::ios::app);    
        utils::mtx_data_log.lock();
        if(global_object::running_job->get_task_name() == task_name){
            if(std::chrono::steady_clock::now()>global_object::running_job->get_actual_start_time() && std::chrono::steady_clock::now() > global_object::running_job->get_actual_finish_time()){
            std::string contents="";
            contents=std::to_string(global_object::running_job->get_actual_start_time())+ "      " + "READ      " + task_name + "TARGET_SPEED";
            }
        }
        
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
            rw_log.write(current_data->get_data().c_str(), current_data->get_data().size());
        }
        rw_log.close();
        utils::mtx_data_log.unlock();    
    }*/
    std::ofstream rw_log;
    rw_log.open(utils::cpsim_path + "/Log/VictorMihaila_read_write.log", std::ios::out);
    std::string contents="";
    contents+="[ TIME ] [ READ/WRITE ] [ TASK NAME ] [ DATA NAME ]\n";
    rw_log.write(contents.c_str(), contents.size());
    if(global_object::running_job->get_is_read()){
        contents="";
        contents += (std::to_string(global_object::running_job->get_actual_start_time()) + " READ " + task_name + " TARGET_SPEED \n");
        rw_log.write(contents.c_str(), contents.size());
    }
    if(global_object::running_job->get_is_write()){
        contents="";
        contents += (std::to_string(global_object::running_job->get_actual_finish_time()) + " WRITE " + task_name + " ACCEL_VALUE \n");
        rw_log.write(contents.c_str(), contents.size());
    }    
}
