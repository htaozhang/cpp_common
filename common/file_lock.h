#ifndef _FILE_LOCK_H_
#define _FILE_LOCK_H_
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <string>

class FileLock {
 public:
  FileLock() : fd_(-1), name_("./LOCK") {
  }
  ~FileLock() { 
      if (fd_ >= 0) {
          close(fd_);
          unlink(name_.c_str());
      }
  }
  int Create() {
      fd_ = open(name_.c_str(), O_RDWR | O_CREAT, 0644);
      if (fd_ < 0) {
          printf("open LOCK file fail:%s\n", strerror(errno));
          return -1;
      }
      return 0;
  }
  int TryLock() {
      struct flock  fl;
      memset(&fl, 0, sizeof(struct flock));
      fl.l_type = F_WRLCK;
      fl.l_whence = SEEK_SET;
      fl.l_start = 0;
      fl.l_len = 0; 
      int err = fcntl(fd_, F_SETLK, &fl);
      if (err == 0) {
          printf("lock file ok\n");
          return 0;
      }
      if (errno == EAGAIN) {
          return -1;
      }
      printf("Try lock LOCK file fail:%s\n", strerror(errno));
      return -2;
  }
  int UnLock() {
      if (fd_ < 0 ) {
          printf("file lock may not create\n");
          return -1;
      }
      struct flock  fl;
      memset(&fl, 0, sizeof(struct flock));
      fl.l_type = F_UNLCK;
      fl.l_whence = SEEK_SET;
      int err = fcntl(fd_, F_SETLK, &fl);
      if (err == 0) {
          printf("unlock file ok\n");
          return 0;
      }
      printf("Unlock LOCK file fail:%s\n", strerror(errno));
      return -2;
  }

 public:
  int           fd_;
  std::string   name_;
};

#endif  // _FILE_LOCK_H_

