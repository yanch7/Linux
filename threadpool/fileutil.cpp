#include "fileutil.h"
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <cassert>
#include <cstdio>
#include <sys/stat.h>
#include <algorithm>
#include <cstring>

ReadSmallFile::ReadSmallFile(const std::string& filename) 
    : fd_(::open(filename.c_str(), O_APPEND | O_CLOEXEC)), err_(0)
{
    buf_[0] = '\0';
    if(fd_ < 0) err_ = errno;
}

ReadSmallFile::~ReadSmallFile()
{
    if(fd_ > 0) close(fd_);
}

int ReadSmallFile::readToBuf(int *size)
{
    int err = err_;
    if(fd_ >= 0)
    {
        ssize_t n = ::pread(fd_, buf_, sizeof(buf_) - 1, 0);
        if(n >= 0)
        {
            if(size) *size = static_cast<int>(n);
            buf_[n] = '\0';
        }
        else err = errno;
    }
    
    return err;
}

int ReadSmallFile::readToString(int maxSize, std::string* content, 
    int64_t* fileSize, int64_t* modifyTime, int64_t* createTime)
{
    assert(content != NULL);
    int err = err_;
    if(fd_ >= 0)
    {
        content->clear();

        if(fileSize)
        {
            struct stat statbuf;
            if(::fstat(fd_, &statbuf) == 0)
            {
                if(S_ISREG(statbuf.st_mode))
                {
                    *fileSize = statbuf.st_size;
                    content->reserve(static_cast<int>(std::min(static_cast<int64_t>(maxSize), *fileSize)));
                }
                
                if(modifyTime)
                {
                    *modifyTime = statbuf.st_mtime;
                }

                if(createTime)
                {
                    *createTime = statbuf.st_ctime;
                }
            }
            else err = errno;
        }
    }

    while(content->size() < static_cast<size_t>(maxSize))
    {
        size_t toRead = std::min(maxSize - content->size(), sizeof(buf_));
        ssize_t n = ::read(fd_, buf_, toRead);
        if(n > 0)
        {
            content->append(buf_, n);
        }
        else 
        {
            if(n < 0) err = errno;
            break;
        }
    }

    return err;
}

AppendFile::AppendFile(const std::string& name) 
    : fp_(fopen(name.c_str(), "a")), writtenBytes_(0)
{
    assert(fp_);
    //设置文件缓冲区
    ::setbuffer(fp_, buf_, sizeof(buf_));
}

AppendFile::~AppendFile()
{
    if(fp_) fclose(fp_);
}

size_t AppendFile::write(const char* line, size_t length)
{
    return fwrite(line, 1, length, fp_);
}

void AppendFile::flush()
{
    fflush(fp_);
}

void AppendFile::append(const char* line, size_t length)
{
    size_t n = this->write(line, length);
    size_t remain = length - n;

    while(remain > 0)
    {
        size_t x = this->write(line, length);
        if(x == 0)
        {
            int err = ferror(fp_);
            if(err) fprintf(stderr, "AppendFile::append() failed %s\n", strerror(err));
            break;
        }

        n += x;
        remain = length - n;
    }

    writtenBytes_ += n;
}