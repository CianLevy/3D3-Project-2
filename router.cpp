#include "router.hpp"

//This function will open a new UDP socket, forwards the data then closes
void router::forwardDatagram(datagram d){

    udp::endpoint destEndpoint = udp::endpoint(boost::asio::ip::address::from_string("127.0.0.1"), 10001);
    
    socket_->async_send_to(
        boost::asio::buffer(&(d.getPacket()[0]), d.getLength()), destEndpoint,
        [this](boost::system::error_code, std::size_t) {
            receive();  //Boost specifies that a handler must be passed to call after the send
           
        });

}

router::router(uint16_t port){
    socket_ = new udp::socket(io_context, udp::endpoint(udp::v4(), port));
    buffer.resize(BUFFERLENGTH);
}


void router::run(){
    stop = false;
    receive();
    io_context.run();
}

void router::receive(){
    if (!stop){
        try {
            socket_->async_receive_from(
                boost::asio::buffer(&buffer[0], BUFFERLENGTH), server_endpoint,
                [this](boost::system::error_code ec, std::size_t bytes_recvd) {
                if (!ec && bytes_recvd > 0) {
                   
                    datagram d(buffer);

                    #if DEBUG
                        std::cout << "Received: " << &buffer[0] << std::endl;
                    #endif
                
                    forwardDatagram(d);
                }
                else
                    receive();
            });
        }
        catch (std::exception& e) {
            std::cerr << "Exception occured in router: " << e.what() << "\n";
        }

    }
}

void router::singleSend(datagram d){
    udp::endpoint ep = udp::endpoint(boost::asio::ip::address::from_string("127.0.0.1"), 10000);
    

    socket_->async_send_to(
        boost::asio::buffer(&(d.getPacket()[0]), d.getLength()), ep,
        [this](boost::system::error_code, std::size_t) {
        });
    socket_->close();
    std::cout << "Sent\n";
    io_context.run();

}