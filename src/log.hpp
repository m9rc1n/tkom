#include <string>
#include <fstream>

int report (int report_type, long result);
int report (int report_type, std::string result);
int report (int report_type, std::ifstream &file);
std::string getResponseFileFromRequestFile(std::string);
