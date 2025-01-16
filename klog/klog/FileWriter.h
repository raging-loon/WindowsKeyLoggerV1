#ifndef FILE_WRITER_H_
#define FILE_WRITER_H_

#include <string>

bool writeToFile(const std::string& location, const unsigned char* bytes, unsigned int length);

#endif // FILE_WRITER_H_