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

/*
    std::ofstream read_write_log;
    read_write_log.open(utils::cpsim_path + "/Log/_2017-17288_read_write.log");
    read_write_log << "[TASK NAME][TIME][READ/WRITE][DATA LENGTH][RAW DATA]" << std::endl;
    read_write_log.close();

    std::ofstream event_log;
    event_log.open(utils::cpsim_path + "/Log/_2017-17288_event.log");
    event_log << "[TIME][JOB ID][EVENT TYPE]" << std::endl;
    event_log.close();
*/
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

void Logger::_201717288_task_read_write_logger(std::string task_name, 
                                                std::shared_ptr<TaggedData> read_data, 
                                                std::shared_ptr<DelayedData> write_data){
    std::ofstream read_write_log;
    read_write_log.open(utils::cpsim_path + "/Log/_2017-17288_read_write.log", std::ios::app);
    read_write_log << std::left << std::setw(10) << task_name << "\t";
    int data_length = 0;

    if(read_data != nullptr){
        data_length = 6;
        std::shared_ptr<TaggedData> current_data = read_data;
        read_write_log << std::left << std::setw(6) << current_data->data_time;
        read_write_log << std::left << std::setw(12) << "READ";
        read_write_log << std::left << std::setw(13) << data_length;
        read_write_log << std::hex << "0x" << current_data->data_read1 << " ";
        read_write_log << std::hex << "0x" << current_data->data_read2 << " ";
        read_write_log << std::hex << "0x" << current_data->data_read3 << " ";
        read_write_log << std::hex << "0x" << current_data->data_read4 << " ";
        read_write_log << std::hex << "0x" << current_data->data_read5 << " ";
        read_write_log << std::hex << "0x" << current_data->data_read6 << " " << std::endl;
    }else{
        data_length = 4;
        std::shared_ptr<DelayedData> current_data = write_data;
        read_write_log << std::left << std::setw(6) << current_data->data_time;
        read_write_log << std::left << std::setw(12) << "WRITE";
        read_write_log << std::left << std::setw(13) << data_length;
        read_write_log << std::hex << "0x" << current_data->data_write1 << " ";
        read_write_log << std::hex << "0x" << current_data->data_write2 << " ";
        read_write_log << std::hex << "0x" << current_data->data_write3 << " ";
        read_write_log << std::hex << "0x" << current_data->data_write4 << " " << std::endl;
    }
    read_write_log.close();
}

void Logger::_201717288_real_cyber_event_logger(long long time, int job_id, std::string event_type){
    std::ofstream event_log;
    event_log.open(utils::cpsim_path + "/Log/_2017-17288_event.log", std::ios::app);
    event_log<< std::left << std::setw(6) << time;
    event_log<< std::left << std::setw(8) << job_id;
    event_log<< std::left << std::setw(12) << event_type << std::endl;
}

std::string Logger::_2019_13914_print_tagged_data_log(std::string task_name, std::shared_ptr<TaggedData> current_data, int size){
    std::stringstream data;
    data << std::hex << "0x" << current_data -> data_read1 << " ";
    data << std::hex << "0x" << current_data -> data_read2 << " ";
    data << std::hex << "0x" << current_data -> data_read3 << " ";
    data << std::hex << "0x" << current_data -> data_read4 << " ";
    data << std::hex << "0x" << current_data -> data_read5 << " ";
    data << std::hex << "0x" << current_data -> data_read6 << "\n";
    
    std::stringstream log;
    log << " ";
    log << std::left << std::setw(14) << task_name;
    log << std::left << std::setw(9) << std::to_string(current_data -> data_time);
    log << std::left << std::setw(15) << "READ";
    log << std::left << std::setw(16) << std::to_string(size);

    return log.str() + data.str();
}

std::string Logger::_2019_13914_print_delayed_data_log(std::string task_name, std::shared_ptr<DelayedData> delayed_data, int size){
    std::stringstream data;
    data << std::hex << "0x" << delayed_data -> data_write1 << " ";
    data << std::hex << "0x" << delayed_data -> data_write2 << " ";
    data << std::hex << "0x" << delayed_data -> data_write3 << " ";
    data << std::hex << "0x" << delayed_data -> data_write4 << "\n";
    
    std::stringstream log;
    log << " ";
    log << std::left << std::setw(14) << task_name;
    log << std::left << std::setw(9) << std::to_string(delayed_data -> data_time);
    log << std::left << std::setw(15) << "READ";
    log << std::left << std::setw(16) << std::to_string(size);

    return log.str() + data.str();
}

void Logger::_2019_13914_task_read_write_logger(std::string task_name){
    std::ofstream log;

    static bool init = false;
    if(!init){
        log.open(utils::cpsim_path + "/Log/2019-13914_read_write.log", std::ios::out);
        log << "[ TASK NAME ] [ TIME ] [ READ/WRITE ] [ DATA LENGTH ] [ RAW DATA ]\n";
        log.close();
        init = true;
    }
    log.open(utils::cpsim_path + "/Log/2019-13914_read_write.log", std::ios::app);
    log << task_name;
    log.close();
    return;
}

void Logger::_2019_13914_real_cyber_event_logger(long long time, int job_id, std::string event_type){
    std::ofstream event_log;

    static bool init = false;
    if(!init){
        event_log.open(utils::cpsim_path+"/Log/2019-13914_event.log",std::ios::out);
        event_log << "[ TIME ] [ JOB ID ] [ EVENT TYPE ]\n";
        event_log.close();
        init = true;
    }

    std::stringstream log;
    log << " ";
    log << std::left << std::setw(9) << std::to_string(time);
    log << std::left << std::setw(11) << "J" + std::to_string(job_id);
    log << std::left << std::setw(15) << event_type;

    Event event;
    event.time = time;
    event.job_id = job_id;
    event.log = log.str();
    event_buffer.push(event);
    while(event_buffer.size() > 10){
        std::ofstream event_log;
        event_log.open(utils::cpsim_path+"/Log/2019-13914_event.log",std::ios::app);
        event_log << event_buffer.top().log << "\n";
        event_buffer.pop();
        event_log.close();
    }
}