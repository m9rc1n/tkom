#include <string>
#include <cstdio>
#include <fstream>
#include <iostream>

#include "log.hpp"
#include "const.hpp"

void clearSession(void);
int finish (std::string log_file);
int start (std::string file_name);

typedef struct session_report {
	bool success;
	std::string abs_path;
	std::string method;
	std::string host;
	std::string content_length;
	std::string status_code;
	std::string request_file_name;
	std::string data_file_name;
} session_report;

session_report session;

int report (int report_type, std::ifstream &file, long cursor) {
    if (session.data_file_name.length() == 0) {
        return 0;
    }
    std::ofstream output_file(session.data_file_name.c_str());
    if(output_file.is_open()) {
        file.seekg(0, file.end);
        long size = file.tellg()-cursor;
        file.seekg(cursor);
        char* memblock = new char[size];
        file.read (memblock, size);
        output_file.write (memblock, size);
        delete[] memblock;
    }
    return 0;
}

int report (int report_type, int result) {
    switch (report_type) {
		case CREATE_FILE:
			break;
		case RESPONSE_RESULT:
			break;
		case REQUEST_RESULT:
			if (result == SUCCESS) {
				session.success = true;
			}
			break;
		case SCAN_RESULT:
			break;
	}
    return 0;
}

int report (int report_type, std::string result) {
	switch (report_type) {
		case START:
			start(result);
			break;
		case FINISH:
			finish(result);
			break;
		case METHOD:
			session.method = result;
			break;
		case HOST:
			session.host = result;
			break;
		case STATUS_CODE:
			session.status_code = result;
			break;
		case CONTENT_LENGTH:
			session.content_length = result;
			break;
		case ABS_PATH:
			session.abs_path = result;
			break;
	}
    return 0;
}

void clearSession() {
    session.success = false;
	session.request_file_name.clear();
	session.abs_path.clear();
	session.host.clear();
	session.content_length.clear();
	session.status_code.clear();
	session.method.clear();
	session.data_file_name.clear();
}

int finish (std::string log_file) {
	if (session.success) {
		std::ofstream file(log_file.c_str(), std::ios::app);
		if (file.is_open()) {
			file << session.request_file_name << " ; " \
				 << session.method << " ; " \
				 << session.host << " ; " \
                 << session.abs_path << " ; " \
				 << session.content_length << "\n";
		}
		file.close();
	}
	clearSession();
    return 0;
}

int start (std::string file_name) {
	session.request_file_name = file_name;
    session.data_file_name = "output/" + getResponseFileFromRequestFile(file_name);
    return 0;
}

std::string getResponseFileFromRequestFile(std::string filename) {
	// indeks znaku rozdzielajacego
	std::size_t index_of_separator = filename.find('-');
	// jesli nie znaleziono znaku rozdzielajacego
	if (index_of_separator == NOT_FOUND) {
		return NULL;
	}
    std::string source = filename.substr(0, index_of_separator);
	std::string destination = filename.substr(index_of_separator+1);

	return destination + "-" + source;
}
