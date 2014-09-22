#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>

using namespace boost::asio;
using namespace boost::posix_time;

io_service service;

#define MEM_FN(x) boost::bind(&self_type::x, shared_from_this())
#define MEM_FN1(x,y) boost::bind(&self_type::x, shared_from_this(),y)
#define MEM_FN2(x,y,z) boost::bind(&self_type::x, shared_from_this(),y,z)

class talk_to_client : public boost::enable_shared_from_this<talk_to_client>, boost::noncopyable {
    typedef talk_to_client self_type;
    talk_to_client() : sock_(service), started_(false) {}
public:
    typedef boost::system::error_code error_code;
    typedef boost::shared_ptr<talk_to_client> ptr;

    void start() {
        started_ = true;
        do_read();
    }

    static ptr new_() {
        ptr new_(new talk_to_client);
        return new_;
    }

    void stop() {
        if ( !started_) return;
        started_ = false;
        sock_.close();
    }

    ip::tcp::socket & sock() { return sock_;}
private:
    void on_read(const error_code & err, size_t bytes) {
        if ( !err) {
            //std::string msg(read_buffer_, bytes);
            char buffer_[max_msg] =
                    "HTTP/1.1 200 OK\r\n"
                    "Date: Mon, 22 Sep 2014 16:00:00 GMT\r\n"
                    "Server: BOOST-ASIO (CentOS)\r\n"
                    "Last-Modified: Sun, 21 Sep 2014 19:00:00 GMT\r\n";
            // echo message back, and then stop
            std::string msg(buffer_, 1024);
            std::cout << bytes << std::endl;
            do_write(msg + "\n");
        }
    stop();
    }

    void on_write(const error_code & err, size_t bytes) {
        do_read();
    }

    void do_read() {

        async_read(sock_, buffer(read_buffer_),
        MEM_FN2(read_complete,_1,_2), MEM_FN2(on_read,_1,_2));
    }

    void do_write(const std::string & msg) {
        std::copy(msg.begin(), msg.end(), write_buffer_);
        sock_.async_write_some( buffer(write_buffer_, msg.size()),
        MEM_FN2(on_write,_1,_2));
    }

    size_t read_complete(const boost::system::error_code & err, size_t bytes) {
        if ( err) return 0;
        bool found = std::find(read_buffer_, read_buffer_ + bytes, '\n') < read_buffer_ + bytes;
        std::cout << ">>read: " << bytes << std::endl;
        //bool found = 1;
        // we read one-by-one until we get to enter, no buffering
        return found ? 0 : 1;
    }

private:
    ip::tcp::socket sock_;
    enum { max_msg = 1024 };
    char read_buffer_[max_msg];
    char write_buffer_[max_msg];
    bool started_;
};

ip::tcp::acceptor acceptor(service, ip::tcp::endpoint(ip::tcp::v4(), 8081));

void handle_accept(talk_to_client::ptr client, const boost::system::error_code & err) {
    client->start();
    talk_to_client::ptr new_client = talk_to_client::new_();
    acceptor.async_accept(new_client->sock(), boost::bind(handle_accept,new_client,_1));
}

int main(int argc, char* argv[]) {
    talk_to_client::ptr client = talk_to_client::new_();
    acceptor.async_accept(client->sock(), boost::bind(handle_accept,client,_1));
    service.run();
}

