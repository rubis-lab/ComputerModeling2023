#include <iostream>
#include <fstream>

int main() {
    std::ofstream log;
    log.open("/home/rubis/ComputerModeling2023/Log/201914388_read_write.log");
    std::string content = "fuck";
    log.write(content.c_str(), content.size());
    log.close();
    return 0;
}