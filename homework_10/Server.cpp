#include <iostream>
#include <thread>

#include <boost/asio.hpp>


void write_messages (boost::asio::ip::tcp::socket & socket, const std::string& nick, bool & end) {

    std::string buf;
    std::string message;

    while (true) {

        std::getline(std::cin, buf);

        if (end) break;

        if (buf.empty()) continue;

        if (buf == "exit") {
            message = buf + '\n';

            boost::asio::write(socket, boost::asio::buffer(message));

            break;
        }

        message = "[" + nick + "]: " + buf + '\n';

        boost::asio::write(socket, boost::asio::buffer(message));

    }

    end = true;

}

void read_messages (boost::asio::ip::tcp::socket & socket, bool & end) {

    // Printing new messages
    while (true) {

        boost::asio::streambuf buffer;

        boost::asio::read_until(socket, buffer, '\n');

        std::string message;

        std::istream input_stream(&buffer);
        std::getline(input_stream, message, '\n');

        if (message == "exit") {
            if (!end) {

                end = true;

                message = "exit\n";
                boost::asio::write(socket, boost::asio::buffer(message));

                std::cout << std::endl << "Your interlocutor close his program, exiting...\n";
            }

            break;
        }


        std::cout << message << "\n";

    }

}

int main() {

    system("chcp 1251");

    std::string nick;

    std::cout << "Enter your nickname:\n";
    std::cin >> nick;

    const std::size_t size = 30;

    auto port = 3333;

    boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::address_v4::any(), port);

    boost::asio::io_service io_service;

    try {

        boost::asio::ip::tcp::acceptor acceptor(io_service, endpoint.protocol());

        acceptor.bind(endpoint);

        acceptor.listen(size);

        boost::asio::ip::tcp::socket socket(io_service);

        acceptor.accept(socket);

        std::cout << "Connection set, you can start chatting!" << std::endl << std::endl;

        bool end = false;

        std::thread thread(write_messages, std::ref(socket), std::cref(nick), std::ref(end));

        thread.detach();

        read_messages(socket, end);

        system("pause");
    }
    catch (boost::system::system_error & e) {
        std::cout << "Error occured! Error code = " << e.code() << ". Message: " << e.what() << std::endl;

        system("pause");

        return e.code().value();
    }



    return EXIT_SUCCESS;
}