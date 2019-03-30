#include "router.hpp"

//This function will open a new UDP socket, forwards the data then closes
void router::forwardDatagram(datagram d){
    //to do: use the route-table to select the correct router to forward to
    udp::endpoint destEndpoint = udp::endpoint(boost::asio::ip::address::from_string("127.0.0.1"), 10001);
    
    socket_->async_send_to(
        boost::asio::buffer(&(d.buildDatagram()[0]), d.getLength()), destEndpoint,
        [this](boost::system::error_code, std::size_t) {
            receive();  //Boost specifies that a handler must be passed to call after the send
           
        });

}

router::router(uint16_t port_){
    socket_ = new udp::socket(io_context, udp::endpoint(udp::v4(), port_));
    buffer.resize(BUFFERLENGTH);
    l = new logger(routerID);
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
                        std::cout << "Received: " << &buffer[1] << std::endl;
                        std::cout << "Type: " << (datagram::type)d.getType() << std::endl;
                        std::cout << "Source: " << d.getID() << std::endl;
                        std::cout << "Payload: " << &(d.getPayload())[0] << std::endl;
                    #endif

                    //Check if the datagram is a DV update or packet to route
                    if (d.getType() == datagram::control){   //DV update
        
                        distance_vector dv(d.getPayload());
                        int updateStatus = table.updateTable(dv);
                        
                        if (updateStatus == 0x7FFFFFFF){  //There were multiple table updates -> retransmit the entire table
                            for (struct link l : table.getTable()){
                                distance_vector new_dv(routerID, l.routerID, l.cost, ip, port);
                                datagram d(datagram::type::control, routerID, new_dv.buildDV());
                                broadcast(d);
                            }
                        }
                        else if (updateStatus != -1){ //A single entry was updated -> transmit the updated link
                                struct link l = table.getTable()[updateStatus];
                                distance_vector new_dv(routerID, l.routerID, l.cost, ip, port);
                                datagram d(datagram::type::control, routerID, new_dv.buildDV());
                                broadcast(d);
                        }
                    }
                    else    //The datagram contains data to be forwarded to another router
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
        boost::asio::buffer(&(d.buildDatagram()[0]), d.getLength()), ep,
        [this](boost::system::error_code, std::size_t) {
        });
    socket_->close();
    std::cout << "Sent\n";
    io_context.run();

}

void router::broadcast(datagram d){
    //to do: implement broadcasting, and repeated broadcasting every 5 seconds
}