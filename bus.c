#include "inc/bus.h"

uint8_t bus_read (uint16_t address) {
    if (address < 0x8000 ) {
        return read_memory(address);
    }
    return 0;

}

void bus_write (uint16_t address , uint8_t value) {
    if (address < 0x8000) {
        write_memory(address, value);}
}
