#include <string>

long response (std::string path, std::string file_name);
long statusLine (long cursor, std::ifstream &file);
long httpVersionResponse (long cursor, std::ifstream &file);
long statusCode (long cursor, std::ifstream &file);
long reasonPhrase (long cursor, std::ifstream &file);
long responseHeader (long cursor, std::ifstream &file);
long contentLength (long cursor, std::ifstream &file);
void messageBody (long cursor, std::ifstream &file);
