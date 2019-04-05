#include "router.hpp"

//This function will open a new UDP socket, forwards the data then closes
void router::forwardDatagram(datagram d){
    udp::endpoint destEndpoint = udp::endpoint(boost::asio::ip::address::from_string(dv->getLink(d.getDestID()).ip), dv->getLink(d.getDestID()).port);

    socket_->async_send_to(
        boost::asio::buffer(&(d.buildDatagram()[0]), d.buildDatagram().size()), destEndpoint,
        [this](boost::system::error_code, std::size_t) {
            receive();  //Boost specifies that a handler must be passed to call after the send
           
        });

}

router::router(char ID) :
    timer(io_context, boost::posix_time::seconds(REFRESHPERIOD)) {

    dv = new distance_vector(ID);
    socket_ = new udp::socket(io_context, udp::endpoint(udp::v4(), dv->getListenPort()));
    buffer.resize(BUFFERLENGTH);
    routerID = ID;    
    l = new logger(routerID);
    l->recordInitialDV(*dv);

    for (int i = 0; i < MAXCONNECTIONS; i++){
        timeSinceReceived[i] = 0;
        received[i] = false;
    }
    timer.async_wait(boost::bind(&router::periodicRetransmit,  this));

}

void router::periodicRetransmit(){
    std::cout << "Advertised distance vector\n";
    broadcast(1, dv->getDVUpdate());

    timer.expires_at(timer.expires_at() + boost::posix_time::seconds(REFRESHPERIOD));    
    timer.async_wait(boost::bind(&router::periodicRetransmit,  this));
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

                    datagram d(buffer, bytes_recvd);

                    #if DEBUG
                        std::cout << "Received: " << &buffer[1] << std::endl;
                        std::cout << "Type: " << (datagram::type)d.getType() << std::endl;
                        std::cout << "Source: " << d.getID() << std::endl;
                        std::cout << "Payload: " << &(d.getPayload())[0] << std::endl;
                        std::cout << "Total datagram size: " << bytes_recvd << std::endl;
                    #endif
          
                    //Check if the datagram is a DV update or packet to route
                    if (d.getType() == datagram::control){   //DV update
                        struct dv_update update = dv->buildDVUpdate(d.getPayload());
                        if (dv->updateDV(update)){
                            std::cout << "DV update caused by router " << d.getID() << std::endl;
                            l->recordDVUpdate(*dv, update);
                            broadcast(1, dv->getDVUpdate());
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
    io_context.run();

}

void router::broadcast(bool control, std::vector<uint8_t> payload){
    //To do: Implement a more rigorous method of tracking immediate neighbours
    datagram new_datagram(datagram::type(control), routerID, 'A', payload);

    for (std::size_t i = 0; i < dv->getCurrentDV().size(); i++){
        if (dv->getLink(i + 65).nextHopID == (char)(i + 65) && dv->getLink(i + 65).nextHopID != routerID){ 
            new_datagram.setDestID((char)(i + 65));
            forwardDatagram(new_datagram);
        }
    }
}

