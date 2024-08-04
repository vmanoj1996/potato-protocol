/******************************************************************************
 * @file        main.cpp
 * @brief       Demo program that reads data from the serial port and decodes the packet
 * 
 * @author      Manoj Velmurugan
 * @date        August 3, 2024
 * @license     MIT License
 ******************************************************************************/

#include <iostream>
// sudo apt-get install libboost-all-dev
#include <boost/asio.hpp>

#include <cstring>
#include "../arduino/packet.hpp"

#include <termios.h>
#include <sys/ioctl.h>

#include "crc16.h"

Packet ref_packet;

using namespace std;

uint16_t previousID = 0;
unsigned long errorCount = 0;

class Timer {
public:
    Timer() : last_time_point(std::chrono::steady_clock::now()) {}

    // Call this function to get the elapsed time in milliseconds
    double elapsed() {
        auto now = std::chrono::steady_clock::now();
        std::chrono::duration<double, std::micro> elapsed_time = now - last_time_point;
        last_time_point = now; // Update the last time point
        return elapsed_time.count();
    }

private:
    std::chrono::steady_clock::time_point last_time_point;
};


int main() {
    Timer timer;
    try {
        boost::asio::io_service io;
        boost::asio::serial_port serial(io, "/dev/ttyACM0"); // Change to your serial port

        cout<<"opened serial port\n";
        // serial.set_option(boost::asio::serial_port_base::baud_rate(6e6));
        // cout<<"Set baud rate\n";
        serial.set_option(boost::asio::serial_port_base::character_size(8));
        cout<<"Set character size\n";
        serial.set_option(boost::asio::serial_port_base::parity(boost::asio::serial_port_base::parity::none));
        cout<<"Set parity\n";
        serial.set_option(boost::asio::serial_port_base::stop_bits(boost::asio::serial_port_base::stop_bits::one));
        cout<<"Set stop bit\n";
        serial.set_option(boost::asio::serial_port_base::flow_control(boost::asio::serial_port_base::flow_control::none));
        cout<<"Set flow control\n";

        while (true) {
            uint8_t c;
            // keep reading until the special characters are encountered
            boost::asio::read(serial, boost::asio::buffer((void*) &c, 1));
            if(c == ref_packet.start)
            {   
                uint8_t header[HEADER_LENGTH];
                // check the header
                boost::asio::read(serial, boost::asio::buffer((void*) &header, HEADER_LENGTH));
                if(memcmp(header, ref_packet.header, HEADER_LENGTH) == 0)
                {
                    uint32_t payloadLength=0;
                    boost::asio::read(serial, boost::asio::buffer((void*)&payloadLength, 4));
                    // cout<<payloadLength<<endl;
                    if(payloadLength != ref_packet.payloadLength)
                    {
                        break;
                    }
                    else
                    {
                        Payload data;
                        boost::asio::read(serial, boost::asio::buffer(reinterpret_cast<uint8_t*> (&data), ref_packet.payloadLength));
                        
                        uint16_t crc;
                        boost::asio::read(serial, boost::asio::buffer(reinterpret_cast<uint8_t*> (&crc), 2));
                        
                        CRC16 calculator;
                        // make the new object to clear any previous memory
                        calculator.processBuffer((uint8_t*) (&data), ref_packet.payloadLength);
                        uint16_t crc_calculated = calculator.getCrc();

                        // cout<<crc_calculated<<" "<<crc<<endl;

                        if(crc_calculated == crc)
                        {
                            // This means that the packet is valid
                            if(data.packetID-previousID != 1) errorCount++;

                            cout<<"err count = "<<errorCount<<" packet ID = "<<data.packetID<<" temperature = "<<data.temperature<<", timer = "<<timer.elapsed()<<endl;
                            previousID = data.packetID;
                        }

                    }
                }
            }
        }
    } catch (boost::system::system_error& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}