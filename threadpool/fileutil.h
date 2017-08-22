#ifndef __FILEUTIL_H_
#define __FILEUTIL_H_

#include "nocopyable.h"
#include <string>

static const int kBufferSize = 64 * 1024;

class ReadSmallFile : nocopyable
{
public:
    ReadSmallFile(const std::string& filename);
    ~ReadSmallFile();

    //return errno
    int readToString(int maxSize,
        std::string* content,
        int64_t* fileSize,
        int64_t* modifyTime,
        int64_t* createTime);
    
    //return errno
    int readToBuf(int* size);

    const char* buffer() const { return buf_; }
private:
    int fd_;
    int err_;
    char buf_[kBufferSize];
};

int readfile(const std::string& filename, int maxSize, 
    std::string* content, 
    int64_t* fileSize = NULL, 
    int64_t* modifyTime = NULL, 
    int64_t* createTime = NULL)
{
    ReadSmallFile file(filename);
    return file.readToString(maxSize, content, fileSize, modifyTime, createTime);
}

/*类用于打开一个文件，并在文件后添加内容。
类将fopen, fwrite等操作进行了包装。打开文件后，将文件与自定义的缓冲区关联。
每次是先将文件写到缓冲区，再通过flush将缓冲区内容冲刷到文件中。*/
class AppendFile : nocopyable
{
public:
    explicit AppendFile(const std::string& name);
    ~AppendFile();

    void append(const char*, const size_t);
    void flush();
    size_t writtenBytes() const { return writtenBytes_; }
private:
    size_t write(const char*, const size_t);
    FILE *fp_;
    char buf_[kBufferSize];
    size_t writtenBytes_;
};
#endif