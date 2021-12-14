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

std::string Logger::_2019_18675_log_prepare_Tagged_Data(std::shared_ptr<TaggedData> taggedData){
    using namespace std;
    string time = to_string(taggedData->data_time);
    string dLeng = to_string(sizeof(int) * 6);

    stringstream streamForHex;
    streamForHex << "0x" << std::setfill ('0') << std::setw(sizeof(int)*2) << std::hex << taggedData->data_read1;
    streamForHex << " 0x" << std::setfill ('0') << std::setw(sizeof(int)*2) << std::hex << taggedData->data_read2;
    streamForHex << " 0x" << std::setfill ('0') << std::setw(sizeof(int)*2) << std::hex << taggedData->data_read3;
    streamForHex << " 0x" << std::setfill ('0') << std::setw(sizeof(int)*2) << std::hex << taggedData->data_read4;
    streamForHex << " 0x" << std::setfill ('0') << std::setw(sizeof(int)*2) << std::hex << taggedData->data_read5;
    streamForHex << " 0x" << std::setfill ('0') << std::setw(sizeof(int)*2) << std::hex << taggedData->data_read6;
    string rData( streamForHex.str() );

    return time + "\tREAD\t" + dLeng + "\t" + rData;
}

std::string Logger::_2019_18675_log_prepare_Delayed_Data(std::shared_ptr<DelayedData> delayedData){
    using namespace std;
    string time = to_string(delayedData->data_time);
    string dLeng = to_string(sizeof(int) * 4);
    
    stringstream streamForHex;
    streamForHex << "0x" << std::setfill ('0') << std::setw(sizeof(int)*2) << std::hex << delayedData->data_write1;
    streamForHex << " 0x" << std::setfill ('0') << std::setw(sizeof(int)*2) << std::hex << delayedData->data_write2;
    streamForHex << " 0x" << std::setfill ('0') << std::setw(sizeof(int)*2) << std::hex << delayedData->data_write3;
    streamForHex << " 0x" << std::setfill ('0') << std::setw(sizeof(int)*2) << std::hex << delayedData->data_write4;
    string rData( streamForHex.str() );

    return time + "\tWRITE\t" + dLeng + "\t" + rData;
}

void Logger::_2019_18675_task_read_write_logger(std::string task_name, std::string dataInfo){
    using namespace std;

    if(utils::log_task == "ALL" || utils::log_task == task_name){
        std::ifstream f(utils::cpsim_path + "/Log/_2019_18675_read_write.log");
        bool isExist = f.good();
        f.close();
        std::ofstream logfile;
        
        if(!isExist){
            logfile.open(utils::cpsim_path + "/Log/_2019_18675_read_write.log");
            logfile << "[TASK NAME][TIME][READ/WRITE][DATA LENGTH][RAW DATA]" << endl;
        }else{
            logfile.open(utils::cpsim_path + "/Log/_2019_18675_read_write.log", std::ios_base::app); // append instead of overwrite
        }

        logfile << task_name << "\t";
        logfile << dataInfo << std::endl;
        logfile.close();
    }
    // Incompatible Task.
}

void Logger::_2019_18675_real_cyber_event_logger(long long time, int jobID, std::string evType){
    using namespace std;
    std::ifstream f(utils::cpsim_path + "/Log/_2019_18675_event.log");
    bool isExist = f.good();
    f.close();
    
    std::ofstream logfile;
    if(!isExist){
        logfile.open(utils::cpsim_path + "/Log/_2019_18675_event.log");
        logfile << "[TIME][JOB ID][EVENT TYPE]" << endl;
    }else{
        logfile.open(utils::cpsim_path + "/Log/_2019_18675_event.log", std::ios_base::app); // append instead of overwrite
    }

    logfile << time << "\tJ" << jobID << "\t" << evType << endl;
    logfile.close();
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
