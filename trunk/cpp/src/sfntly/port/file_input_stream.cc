/*
 * Copyright 2011 Google Inc. All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#if defined (WIN32)
#include <windows.h>
#endif

#include "sfntly/port/file_input_stream.h"
#include "sfntly/port/exception_type.h"

namespace sfntly {

FileInputStream::FileInputStream() : file_(NULL), position_(0), length_(0) {
}

FileInputStream::~FileInputStream() {
  close();
}

int32_t FileInputStream::available() {
  return length_ - position_;
}

bool FileInputStream::open(const char* file_path) {
  assert(file_path);
  if (file_) {
    close();
  }
#if defined (WIN32)
  fopen_s(&file_, file_path, "rb");
#else
  file_ = fopen(file_path, "rb");
#endif
  if (file_ == NULL) {
    return false;
  }

  fseek(file_, 0, SEEK_END);
  length_ = ftell(file_);
  fseek(file_, 0, SEEK_SET);
  return true;
}

void FileInputStream::close() {
  if (file_) {
    fclose(file_);
    length_ = 0;
    position_ = 0;
    file_ = NULL;
  }
}

void FileInputStream::mark(int32_t readlimit) {
  // NOP
  UNREFERENCED_PARAMETER(readlimit);
}

bool FileInputStream::markSupported() {
  return false;
}

int32_t FileInputStream::read() {
  if (!file_) {
#if defined (SFNTLY_NO_EXCEPTION)
    return 0;
#else
    throw IOException("no opened file");
#endif
  }
  if (feof(file_)) {
#if defined (SFNTLY_NO_EXCEPTION)
    return 0;
#else
    throw IOException("eof reached");
#endif
  }
  byte_t value;
  fread(&value, 1, 1, file_);
  position_++;
  return value;
}

int32_t FileInputStream::read(ByteVector* b) {
  return read(b, 0, b->capacity());
}

int32_t FileInputStream::read(ByteVector* b, int32_t offset, int32_t length) {
  assert(b);
  if (!file_) {
#if defined (SFNTLY_NO_EXCEPTION)
    return 0;
#else
    throw IOException("no opened file");
#endif
  }
  if (feof(file_)) {
#if defined (SFNTLY_NO_EXCEPTION)
    return 0;
#else
    throw IOException("eof reached");
#endif
  }
  size_t read_count = std::min<size_t>(length_ - position_, length);
  if (b->size() < (size_t)(offset + read_count)) {
    b->resize((size_t)(offset + read_count));
  }
  int32_t actual_read = fread(&((*b)[offset]), 1, read_count, file_);
  position_ += actual_read;
  return actual_read;
}

void FileInputStream::reset() {
  // NOP
}

int64_t FileInputStream::skip(int64_t n) {
  if (!file_) {
#if defined (SFNTLY_NO_EXCEPTION)
    return 0;
#else
    throw IOException("no opened file");
#endif
  }
  int64_t skip_count = 0;
  if (n < 0) {  // move backwards
    skip_count = std::max<int64_t>(0 - (int64_t)position_, n);
    position_ -= (size_t)(0 - skip_count);
    fseek(file_, position_, SEEK_SET);
  } else {
    skip_count = std::min<size_t>(length_ - position_, (size_t)n);
    position_ += (size_t)skip_count;
    fseek(file_, (size_t)skip_count, SEEK_CUR);
  }
  return skip_count;
}

void FileInputStream::unread(ByteVector* b) {
  unread(b, 0, b->capacity());
}

void FileInputStream::unread(ByteVector* b, int32_t offset, int32_t length) {
  assert(b);
  assert(b->size() >= size_t(offset + length));
  if (!file_) {
#if defined (SFNTLY_NO_EXCEPTION)
    return;
#else
    throw IOException("no opened file");
#endif
  }
  size_t unread_count = std::min<size_t>(position_, length);
  fseek(file_, position_ - unread_count, SEEK_SET);
  position_ -= unread_count;
  read(b, offset, length);
  fseek(file_, position_ - unread_count, SEEK_SET);
  position_ -= unread_count;
}

}  // namespace sfntly
