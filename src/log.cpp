#include <string>
#include <vector>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <iostream>

#include "log.hpp"
#include "const.hpp"

void clearSession(void);
int finish (std::string log_file);
int finish (std::ifstream &file);
int start (std::string file_name);
void clearResponse (void);
void clearRequest (void);
void clearSession (void);

typedef struct request_report {
	bool success;
	std::string abs_path;
	std::string host;
	std::string method;
} request_report;

typedef struct response_report {
    std::string content_type;
    std::string content_length;
    std::string status_code;
    long data_cursor;
} response_report;

typedef struct session_report {
    std::vector <request_report> request;
    std::vector <response_report> response;
    std::string timestamp;
	std::string request_file_name;
	std::string data_file_name;
	std::string output_directory;
    std::string stat_file;
} session_report;

session_report session;
request_report temp_request;
response_report temp_response;

int createFile (std::ifstream &file, int id, long cursor, long length, std::string type) {
    if (session.data_file_name.length() == 0) {
        return 0;
    }

    std::stringstream ss;
    ss << id;

    std::string output_filename;
    output_filename += session.output_directory;
    output_filename += session.timestamp;
    output_filename += ss.str();
    output_filename += "_";
    output_filename += session.data_file_name;
    output_filename += "." + type;

    std::ofstream output_file(output_filename.c_str(), std::ios::binary);
    if(output_file.is_open()) {
        printf("\tHTTP Response Body saved to: %s\n", output_filename.c_str());
		char* memblock = new char[length];
        printf("Afdfsf: %d\n", cursor);
        file.seekg(cursor);
        file.read (memblock, length);
        output_file.write (memblock, length);
        delete[] memblock;
    }
    return 0;
}

int report (int report_type, long result) {
    switch (report_type) {
        case CREATE_FILE:
            temp_response.data_cursor = result;
            break;
		case RESPONSE_RESULT:
            session.response.push_back(temp_response);
            clearResponse();
			break;
		case REQUEST_RESULT:
			session.request.push_back(temp_request);
            clearRequest();
			break;
		case SCAN_RESULT:
			break;
	}
    return 0;
}

int report (int report_type, std::ifstream &file) {
    switch (report_type) {
        case FINISH:
            finish(file);
            break;
    }
    return 0;
}

int report (int report_type, std::string result) {
	switch (report_type) {
        case EXTENSION:
            temp_response.content_type = result;
            break;
        case STAT:
            session.stat_file = result;
            break;
		case OUTPUT:
            session.output_directory = result;
            break;
		case START:
			start(result);
			break;
		case FINISH:
			finish(result);
			break;
		case METHOD:
			temp_request.method = result;
			break;
		case HOST:
			temp_request.host = result;
			break;
		case STATUS_CODE:
			temp_response.status_code = result;
			break;
		case CONTENT_LENGTH:
			temp_response.content_length = result;
			break;
		case ABS_PATH:
			temp_request.abs_path = result;
			break;
	}
    return 0;
}

void clearSession() {
    session.request.clear();
    session.response.clear();
	session.request_file_name.clear();
	session.data_file_name.clear();
    session.timestamp.clear();
}

void clearRequest() {
    temp_request.host.clear();
    temp_request.abs_path.clear();
    temp_request.method.clear();
    temp_request.success = false;
}

void clearResponse() {
    temp_response.content_length.clear();
    temp_response.status_code.clear();
    temp_response.data_cursor = 0;
}

int finish (std::string log_file) {
    printf("Finishing session for: %s\n", session.request_file_name.c_str());
    clearRequest();
    clearResponse();
	clearSession();
    return 0;
}

int finish (std::ifstream &file) {

    typedef std::vector<request_report>::iterator req_v;
    typedef std::vector<response_report>::iterator res_v;
    int id = 1;
    if (session.request.size() == 0) {
        printf("\tHTTP Request Not Found\n");
    } else {
        res_v res = session.response.begin();
        for(req_v req = session.request.begin(); req != session.request.end(); ++req) {

            long content_length = 0;

            if (res != session.response.end()) {
                content_length = atoi((*res).content_length.c_str());
            }

            printf("\t%d. HTTP Request founded with method: %s\n", id, (*req).method.c_str());
            std::ofstream out_file(session.stat_file.c_str(), std::ios::app);
		    if (out_file.is_open()) {
			    out_file << session.request_file_name << " ; " \
			    	<< id << " ; " \
				    << (*req).method << " ; " \
				    << (*req).host << " ; " \
				    << content_length << "\n";
		    }
		    out_file.close();
            if (res != session.response.end()) {
                if (file.is_open()) {
                    long length = atoi ((*res).content_length.c_str());
                    if (length != NOT_FOUND) { createFile(file, id, (*res).data_cursor, length, (*res).content_type); }
                }
                ++res;
            }
            id++;
        }
    }
    return 0;
}

int start (std::string file_name) {
    printf("\nStarting session for: %s\n", file_name.c_str());
	session.request_file_name = file_name;
    session.data_file_name = getResponseFileFromRequestFile(file_name);
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
