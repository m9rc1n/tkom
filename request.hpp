#include <string>
#include <fstream>

long request (std::string path, std::string file_name);
long requestLine (long request_line_cursor, std::ifstream &file);
long method (long method_cursor, std::ifstream &file);
long requestUri (long request_uri_cursor, std::ifstream &file);
long absoluteUri (long absolute_uri_cursor, std::ifstream &file);
// long host (long host_cursor, std::ifstream &file);
// long port (long port_cursor, std::ifstream &file);
long absPath (long abs_path_cursor, std::ifstream &file);
long httpVersionRequest (long http_version_cursor, std::ifstream &file);
long requestHeader(long requestHeader_cursor, std::ifstream &file);
