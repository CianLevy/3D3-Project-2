#include <vector>

class datagram{
    enum type {control, data};

    private:
        std::vector<uint8_t> packet;
        type t;
        char sourceID;
        uint8_t length;

    public:
        datagram(type t, char sourceID, std::vector<uint8_t> payload);
        datagram(std::vector<uint8_t> d); //Demultiplex a received datagram and extract its payload
};