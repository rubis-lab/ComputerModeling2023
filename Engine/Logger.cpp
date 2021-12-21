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
    _2019_18675_real_cyber_event_logger(0, 0, "", true);
}
std::string Logger::_2019_18675_log_prepare_byte_hex(int value){
    using namespace std;
    stringstream streamForHex;
    unsigned int bytes[4];
    bytes[0] = (value >> 24) & 0xFF;
    bytes[1] = (value >> 16) & 0xFF;
    bytes[2] = (value >> 8) & 0xFF;
    bytes[3] = (value) & 0xFF;
    streamForHex << "0x" << setfill('0') << setw(2) << hex << bytes[0];
    streamForHex << " 0x" << setfill('0') << setw(2) << hex << bytes[1];
    streamForHex << " 0x" << setfill('0') << setw(2) << hex << bytes[2];
    streamForHex << " 0x" << setfill('0') << setw(2) << hex << bytes[3];
    return streamForHex.str();
}

std::string Logger::_2019_18675_log_prepare_Tagged_Data(std::shared_ptr<TaggedData> taggedData){
    using namespace std;
    string time = to_string(taggedData->data_time);
    string dLeng = to_string(sizeof(int) * 6);

    stringstream streamForHex;

    streamForHex << _2019_18675_log_prepare_byte_hex(taggedData->data_read1);
    streamForHex << " " << _2019_18675_log_prepare_byte_hex(taggedData->data_read2);
    streamForHex << " " << _2019_18675_log_prepare_byte_hex(taggedData->data_read3);
    streamForHex << " " << _2019_18675_log_prepare_byte_hex(taggedData->data_read4);
    streamForHex << " " << _2019_18675_log_prepare_byte_hex(taggedData->data_read5);
    streamForHex << " " << _2019_18675_log_prepare_byte_hex(taggedData->data_read6);

    string rData( streamForHex.str() );

    return time + "\tREAD\t" + dLeng + "\t" + rData;
}

std::string Logger::_2019_18675_log_prepare_Delayed_Data(std::shared_ptr<DelayedData> delayedData){
    using namespace std;
    string time = to_string(delayedData->data_time);
    string dLeng = to_string(sizeof(int) * 4);
    
    stringstream streamForHex;

    streamForHex << _2019_18675_log_prepare_byte_hex(delayedData->data_write1);
    streamForHex << " " << _2019_18675_log_prepare_byte_hex(delayedData->data_write2);
    streamForHex << " " << _2019_18675_log_prepare_byte_hex(delayedData->data_write3);
    streamForHex << " " << _2019_18675_log_prepare_byte_hex(delayedData->data_write4);

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
            //logfile.open(utils::cpsim_path + "/Log/_2019_18675_read_write.log", std::ios_base::app); // append instead of overwrite
            logfile.open(utils::cpsim_path + "/Log/_2019_18675_read_write.log", std::ios_base::app);
        }

        logfile << task_name << "\t";
        logfile << dataInfo << std::endl;
        logfile.close();
    }
    // Incompatible Task.
}

void Logger::_2019_18675_real_cyber_event_logger(long long time, int jobID, std::string evType, bool isDestructed){
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

    if(isDestructed){
        while(prQ.size() > 0){
            logDump poppedElem = static_cast<logDump>(prQ.top());
            
            logfile << poppedElem.timeStamp << "\tJ" << poppedElem.jobID << "\t" << poppedElem.evType << endl;

            prQ.pop();
        }
    }else{
        prQ.push({time, jobID, evType});
        if(prQ.size() >= 60){
            logDump poppedElem = static_cast<logDump>(prQ.top());
            
            logfile << poppedElem.timeStamp << "\tJ" << poppedElem.jobID << "\t" << poppedElem.evType << endl;

            prQ.pop();
        }
    }

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
void Logger::jonake_task_read_write_logger(std::string task_name){
    std::ofstream log(utils::cpsim_path + "/Log/jonake_read_write.log", std::ios::app);

    std::string contents = "";
    std::shared_ptr<DelayedData> delayed_data = std::make_shared<DelayedData>();
    contents +=task_name +"\t"+std::to_string(delayed_data->data_time)+"\t"+std::to_string(delayed_data->data_write1 & 0x0f)+"\n";
    log<<contents;
    log.close();
}

void Logger::jonake_real_cyber_event_logger(long time, int job_id, std::string event_type){
    std::ofstream log(utils::cpsim_path + "/Log/jonake_event.log", std::ios::app);

    std::string contents = "";
    contents +=std::to_string(time) +"\t"+std::to_string(job_id)+"\t"+event_type+"\n";
    log<<contents;
    log.close();
}

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
/*

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
*/

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
