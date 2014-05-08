#include <string>
#include <fstream>

int report (int report_type, int result);
int report (int report_type, std::string result);
int report (int report_type, std::ifstream &file, long cursor);
std::string getResponseFileFromRequestFile(std::string);
