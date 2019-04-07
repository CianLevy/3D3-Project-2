#include "router.hpp"

//This function will open a new UDP socket, forwards the data then closes
void router::forwardDatagram(datagram d){
    d.setPreviousHopID(routerID);

    udp::endpoint destEndpoint = udp::endpoint(boost::asio::ip::address::from_string(dv.getLink(d.getDestID()).ip), dv.getLink(d.getDestID()).port);

    socket_->async_send_to(
        boost::asio::buffer(&(d.buildDatagram()[0]), d.getLength()), destEndpoint,
        [this](boost::system::error_code, std::size_t){});

}

router::router(char ID, bool mode, std::string topologyCSV) :
    timer(io_context, boost::posix_time::seconds(REFRESHPERIOD)),
    dv(ID, topologyCSV) {

    buffer.resize(BUFFERLENGTH);
    routerID = ID;    

    #if DEBUG == 1
        dv.printForwardTable();
    #endif

    if (!mode){
        socket_ = new udp::socket(io_context, udp::endpoint(udp::v4(), dv.getListenPort()));
        log = new logger(ID);
        log->recordInitialDV(dv);
    
        //Extract the list of immediate neighbours before the distance vector is changed by received updates
        for (std::size_t i = 0; i < dv.getCurrentDV().size(); i++)
            if (dv.getLink(i + 65).nextHopID == (char)(i + 65) && dv.getLink(i + 65).nextHopID != routerID){
                timeSinceReceived.push_back(0);
                live.push_back(true);   //Assume all routers are live until proven otherwise
                
                if ((char)(i + 65) == routerID)
                    live.at(i) = false;
                
            }
        
        timer.async_wait(boost::bind(&router::periodicRetransmit,  this));
    }
    else
        socket_ = new udp::socket(io_context, udp::endpoint(udp::v4(), 15000));
    
    

}

void router::periodicRetransmit(){
    if (!stop){
        std::cout << "Advertised distance vector\n";
        for (std::size_t i = 0; i < timeSinceReceived.size(); i++){
            timeSinceReceived.at(i)++;
            
            if (timeSinceReceived.at(i) > DROP_OUT_PERIOD_COUNT && live.at(i)){   //The neighbour has not provided an update within the required period and is now considered dead
                std::cout << "Router " << dv.getNeighbours().at(i).nextHopID << " has not responded for " << DROP_OUT_PERIOD_COUNT * REFRESHPERIOD << " seconds and is now considered dead." << std::endl;
                live.at(i) = false;
                dv.removeRouter(dv.getNeighbours().at(i).nextHopID);
                log->recordRouterDropout(dv, dv.getNeighbours().at(i).nextHopID); 
            }
        }
            
        broadcast(1, dv.getDVUpdate());
        

        timer.expires_at(timer.expires_at() + boost::posix_time::seconds(REFRESHPERIOD));    
        timer.async_wait(boost::bind(&router::periodicRetransmit,  this));
    }
    else
        timer.get_io_service().post([&]{timer.cancel();});
    
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

                    #if DEBUG == 2
                        std::cout << "Received: " << &buffer[1] << std::endl;
                        std::cout << "Type: " << (datagram::type)d.getType() << std::endl;
                        std::cout << "Source: " << d.getID() << std::endl;
                        std::cout << "Payload: " << &(d.getPayload())[0] << std::endl;
                        std::cout << "Total datagram size: " << bytes_recvd << std::endl;
                    #endif
      
                    //Check if the datagram is a DV update or packet to route
                    if (d.getType() == datagram::control){   //DV update

                        //Loop through neighbours to determine which one was the source and reset the receive time count
                        for (std::size_t i = 0; i < dv.getNeighbours().size(); i++){
                            if (dv.getNeighbours().at(i).nextHopID == d.getID()){ //When the neighbour is found reset the received count
                                timeSinceReceived.at(i) = 0; //Reset the time since communication with the router
                               
                                if (!live.at(i)){   //If the router was not alive the dv must be reset to the original link cost
                                    live.at(i) = true;
                                    std::cout << "Router " << d.getID() << " joined" << std::endl;
                                    dv.restoreLink(d.getID()); 
                                    log->recordRouterJoin(dv, d.getID());                
                                }
                                break;
                            }
                        }
          
                        struct dv_update update = dv.buildDVUpdate(d.getPayload());
                        if (dv.updateDV(update)){
                            std::cout << "DV update caused by router " << d.getID() << std::endl;
                            log->recordDVUpdate(dv, update);
                            broadcast(1, dv.getDVUpdate());
                            
                            #if DEBUG == 1
                                dv.printForwardTable();
                            #endif
                        }
      
                    }
                    else {   //The datagram contains data to be forwarded to another router
                        std::cout << "Received datagram from " << d.getPreviousHopID() << " for destination " << d.getDestID() << std::endl;
                        log->recordRoutedDatagram(d, dv.getLink(routerID).port, dv.getLink(d.getDestID()).port);
                        if (d.getDestID() != routerID)
                            forwardDatagram(d);
                    }
                }
                
                receive(); //Note: receive will be called regardless of whether or not a packet was received. This ensures the router will continue to listen in all cases
            });
        }
        catch (std::exception& e) {
            std::cerr << "Exception occured in router: " << e.what() << "\n";
        }

    }
}

void router::singleSend(datagram d){
   
    udp::endpoint ep = udp::endpoint(boost::asio::ip::address::from_string(dv.getLink(routerID).ip), dv.getLink(routerID).port);
    socket_->async_send_to(
        boost::asio::buffer(&(d.buildDatagram()[0]), d.getLength()), ep,
        [this](boost::system::error_code, std::size_t) {});
    io_context.run();
}

void router::broadcast(bool control, std::vector<uint8_t> payload){
    datagram new_datagram(datagram::type(control), routerID, 'A', routerID, payload); //The datagram is initialised with destination A arbitrarily bevause it will be changed in the loop

    for (std::size_t i = 0; i < dv.getNeighbours().size(); i++){
        if (live.at(i)){    

            new_datagram.setDestID(dv.getNeighbours().at(i).nextHopID);
                udp::endpoint ep = udp::endpoint(boost::asio::ip::address::from_string(dv.getNeighbours().at(i).ip), dv.getNeighbours().at(i).port);

            socket_->async_send_to(
                boost::asio::buffer(&(new_datagram.buildDatagram()[0]), new_datagram.getLength()), ep,
                [this](boost::system::error_code, std::size_t) {});
        }
    }
}

void router::close(){
    stop = true;
    io_context.stop();
}

router::~router(){
    socket_->close();
    delete socket_;

    delete log;

    buffer.resize(0);
    timeSinceReceived.resize(0);
    live.resize(0);
}