#include <ClientBufferInfo.h>

ClientBufferInfo::ClientBufferInfo(std::string address) {
    this->expectedSize = 0;
    this->address = address;
    this->buffer = new std::string();
}

void ClientBufferInfo::setExpectedSize(size_t expectedSize) {
    this->expectedSize = expectedSize;
}

bool ClientBufferInfo::isDone() {
    return expectedSize != 0 && buffer->size() == expectedSize;
}

std::string* ClientBufferInfo::getBuffer() {
    return buffer;
}

void ClientBufferInfo::add(char* str) {
    this->buffer->append(str);
}

std::string& ClientBufferInfo::getAddress() {
    return address;
}

size_t ClientBufferInfo::getExpectedSize() {
    return expectedSize;
}
