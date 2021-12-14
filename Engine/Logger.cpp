#include "Logger.h"
#include "Utils.h"

#include <fstream>
#include <string>
#include <cstdlib>
#include <stdio.h>
#include <iomanip>
#include <climits>
#include <mutex>
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
    
    //construct the first line of 2021-81892_read_write.log
    std::ofstream MyFile;
    MyFile.open(utils::cpsim_path + "/Log/_2021-81892_read_write.log", std::ios::out);
    std::string first_line = "";
    first_line += "[TASK NAME][TIME][READ/WRITE][DATA LENGTH][RAW DATA]";
    first_line += "\n";
    MyFile.write(first_line.c_str(), first_line.size());
    MyFile.close();

    //construct the first line of 2021-81892_event.log
    std::ofstream MyFile2;
    MyFile2.open(utils::cpsim_path + "/Log/_2021-81892_event.log", std::ios::out);
    std::string first_line2 = "";
    first_line2 += "[TIME][JOB ID][EVENT TYPE]";
    first_line2 += "\n";
    MyFile2.write(first_line2.c_str(), first_line2.size());
    MyFile2.close();

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

void Logger::id_202181892_task_read_write_logger(std::string task_name, int write1, int write2, int write3, int write4){
    std::ofstream MyFile;
    MyFile.open(utils::cpsim_path + "/Log/_2021-81892_read_write.log", std::ios::app);
    
    if(!global_object::tagged_data_read.empty())
    {
        std::shared_ptr<TaggedData> read_data = global_object::tagged_data_read.at(global_object::tagged_data_read.size()-1);
        if((utils::log_task).compare(task_name)==0){
            std::string contents = "";
            contents += task_name + std::string(11-task_name.size(),' ');
            if(task_name.compare("LK")==0 || task_name.compare("CC")==0 || task_name.compare("SENSING")==0){
                //read_data1
                contents += std::to_string(std::__size_to_integer(utils::current_time)) + std::string(6-(std::to_string(std::__size_to_integer(utils::current_time))).length(),' ');
                contents += "READ" + std::string(12-4,' ');
                contents += std::to_string(std::to_string((read_data->data_read1)).length()) + std::string(13-(std::to_string((std::to_string((read_data->data_read1)).length()))).length(),' ');
                std::stringstream sstream;
                sstream << std::hex << read_data->data_read1;
                std::string hex_string = sstream.str();
                std::string hex_format = "0x" + hex_string;
                contents += hex_format + '\n';
                //read_data2
                contents += task_name + std::string(11-task_name.size(),' ');
                contents += std::to_string(std::__size_to_integer(utils::current_time)) + std::string(6-(std::to_string(std::__size_to_integer(utils::current_time))).length(),' ');
                contents += "READ" + std::string(12-4,' ');
                contents += std::to_string(std::to_string((read_data->data_read2)).length()) + std::string(13-(std::to_string((std::to_string((read_data->data_read2)).length()))).length(),' ');
                std::stringstream sstream2;
                sstream2 << std::hex << read_data->data_read2;
                std::string hex_string2 = sstream2.str();
                std::string hex_format2 = "0x" + hex_string2;
                contents += hex_format2 + '\n';
                
            }
            if(task_name.compare("CC")==0 || task_name.compare("SENSING")==0){
                //read_data3
                contents += task_name + std::string(11-task_name.size(),' ');
                contents += std::to_string(std::__size_to_integer(utils::current_time)) + std::string(6-(std::to_string(std::__size_to_integer(utils::current_time))).length(),' ');
                contents += "READ" + std::string(12-4,' ');
                contents += std::to_string(std::to_string((read_data->data_read3)).length()) + std::string(13-(std::to_string((std::to_string((read_data->data_read3)).length()))).length(),' ');
                std::stringstream sstream;
                sstream << std::hex << read_data->data_read3;
                std::string hex_string = sstream.str();
                std::string hex_format = "0x" + hex_string;
                contents += hex_format + '\n';
                //read_data4
                contents += task_name + std::string(11-task_name.size(),' ');
                contents += std::to_string(std::__size_to_integer(utils::current_time)) + std::string(6-(std::to_string(std::__size_to_integer(utils::current_time))).length(),' ');
                contents += "READ" + std::string(12-4,' ');
                contents += std::to_string(std::to_string((read_data->data_read4)).length()) + std::string(13-(std::to_string((std::to_string((read_data->data_read4)).length()))).length(),' ');
                std::stringstream sstream2;
                sstream2 << std::hex << read_data->data_read4;
                std::string hex_string2 = sstream2.str();
                std::string hex_format2 = "0x" + hex_string2;
                contents += hex_format2 + '\n';
            }
            MyFile.write(contents.c_str(), contents.size());
            //MyFile.close();
        }
    }
    
    if(!((write1==0) && (write2==0) && (write3==0) && (write4==0))){//!global_object::delayed_data_write.empty()){
        //std::shared_ptr<DelayedData> write_data = global_object::delayed_data_write.front();
        //std::shared_ptr<DelayedData> write_data = global_object::delayed_data_write.at(global_object::delayed_data_write.size()-1);
        if((utils::log_task).compare(task_name)==0){
            std::string contents = "";
            contents += task_name + std::string(11-task_name.size(),' ');
            if(task_name.compare("LK")==0){
                //write_data3
                contents += std::to_string(std::__size_to_integer(utils::current_time)) + std::string(6-(std::to_string(std::__size_to_integer(utils::current_time))).length(),' ');
                contents += "WRITE" + std::string(12-5,' ');
                contents += std::to_string(std::to_string((write3)).length()) + std::string(13-(std::to_string((std::to_string((write3)).length()))).length(),' ');
                std::stringstream sstream;
                sstream << std::hex << write3;
                std::string hex_string = sstream.str();
                std::string hex_format = "0x" + hex_string;
                contents += hex_format + '\n';
                //write_data4
                contents += task_name + std::string(11-task_name.size(),' ');
                contents += std::to_string(std::__size_to_integer(utils::current_time)) + std::string(6-(std::to_string(std::__size_to_integer(utils::current_time))).length(),' ');
                contents += "WRITE" + std::string(12-5,' ');
                contents += std::to_string(std::to_string((write4)).length()) + std::string(13-(std::to_string((std::to_string((write4)).length()))).length(),' ');
                std::stringstream sstream2;
                sstream2 << std::hex << write4;
                std::string hex_string2 = sstream2.str();
                std::string hex_format2 = "0x" + hex_string2;
                contents += hex_format2 + '\n';
            }
            if(task_name.compare("CC")==0){
                //write_data1
                contents += std::to_string(std::__size_to_integer(utils::current_time)) + std::string(6-(std::to_string(std::__size_to_integer(utils::current_time))).length(),' ');
                contents += "WRITE" + std::string(12-5,' ');
                contents += std::to_string(std::to_string((write1)).length()) + std::string(13-(std::to_string((std::to_string((write1)).length()))).length(),' ');
                std::stringstream sstream;
                sstream << std::hex << write1;
                std::string hex_string = sstream.str();
                std::string hex_format = "0x" + hex_string;
                contents += hex_format + '\n';
                //write_data2
                contents += task_name + std::string(11-task_name.size(),' ');
                contents += std::to_string(std::__size_to_integer(utils::current_time)) + std::string(6-(std::to_string(std::__size_to_integer(utils::current_time))).length(),' ');
                contents += "WRITE" + std::string(12-5,' ');
                contents += std::to_string(std::to_string((write2)).length()) + std::string(13-(std::to_string((std::to_string((write2)).length()))).length(),' ');
                std::stringstream sstream2;
                sstream2 << std::hex << write2;
                std::string hex_string2 = sstream2.str();
                std::string hex_format2 = "0x" + hex_string2;
                contents += hex_format2 + '\n';
            }
            MyFile.write(contents.c_str(), contents.size());
        }
    }
    MyFile.close();
}

void id_202181892_real_cyber_event_logger(long long time, int job_id, std::string event_type){
    
    std::ofstream MyFile;
    MyFile.open(utils::cpsim_path + "/Log/_2021-81892_event.log", std::ios::app);
    std::string contents = "";
    contents += std::to_string(std::__size_to_integer(time)) + std::string(6-(std::to_string(std::__size_to_integer(time))).length(),' ');
    contents += 'J' + std::to_string(job_id) + std::string(8-1-std::to_string(job_id).length(),' ');
    contents += event_type;
    MyFile.write(contents.c_str(), contents.size());
    MyFile.close();
}