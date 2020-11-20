#ifndef PACKET_HH
#define PACKET_HH

#include <deque>
#include <utility>
#include <cinttypes>

typedef size_t Addr;
typedef std::pair<Addr, Addr> AddrRange;

class MasterPort;

class Packet{
private:
    bool _isRead; // is read or write access to memory
    uint8_t *buffer; // Buffer to store data. Byte array
    std::deque<MasterPort *> header; // Routing scheme. Keeps track of the ports passed through
    Addr dst; // Destination/Address
    size_t size; // Size of the packet
public:
    Packet(bool read, Addr _dst, size_t _size):
        _isRead(read),
        buffer(new uint8_t(_size)),
        dst(_dst),
        size(_size) {}
    ~Packet() {
        header.clear();
        if(buffer) delete buffer;
    }
    uint8_t *getBuffer() { return buffer; }
    bool isRead() { return _isRead; }
    size_t getSize() { return size; }
    Addr getAddr() { return dst; } // Getting address

    // Modifying header. When pass from one device to another we are going to append to the header where we came from
    void appendHeader(MasterPort *port) { header.push_back(port); }
    void popHeader() { header.pop_back(); } // When traversing back through the memory hiearchy to return back to the device that originated the request. Pop off the header
    MasterPort * getHeaderEnd() { return header.back(); } // Find the last location we were at by getting the last part of the header.

};

typedef Packet *PacketPtr;

#endif // PACKET.HH
