#pragma once
#include <boost/asio/dispatch.hpp>
#include <boost/asio/strand.hpp>
#include <boost/config.hpp>
#include <boost/beast/version.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <iostream>
#include <memory>
#include <utility>
#include <list>
#include <string>
#include <functional>
#include <cstdlib>
#include <algorithm>
#include <filesystem>
#include <string>
#include "log.h"

using namespace boost;
using namespace beast;


class Session : public std::enable_shared_from_this<Session>, public Log
{
    std::string _ip;
    http::request<http::string_body> _req;
    beast::flat_buffer _buf;
    beast::tcp_stream _stream;
 
public:
    Session(asio::ip::tcp::socket&& sock) : _stream(std::move(sock))
    { 
        _ip = _stream.socket().remote_endpoint().address().to_string();
        writeLog("Connected ip: " + _ip);
    }

    ~Session() 
    {
        writeLog("Disconected ip: " + _ip);
    }

    void run() 
    {
          doRead();
    }

private:
    void doRead()
    {
        _req = {};
        _buf.clear();
        _stream.expires_after(std::chrono::seconds(30));
        http::async_read(_stream, _buf, _req, 
            beast::bind_front_handler(&Session::onRead, shared_from_this()));
    }

    void onRead(beast::error_code e, size_t bytesTransfered)
    {
        ignore_unused(bytesTransfered);
        
        if(e == http::error::end_of_stream)
            return doClose();
        
        if(e)
        {
            writeLog("Failed read: " + e.message());
            return;
        }

        doWrite(handleRequest(std::move(_req)));
    }

    void doWrite(http::message_generator&& msg)
    {
        bool keepAlive = msg.keep_alive();
        beast::async_write(_stream, std::move(msg), 
            beast::bind_front_handler(&Session::onWrite, shared_from_this(), keepAlive));
    }

    void onWrite(bool keepAlive, beast::error_code e, size_t bytesTransfered)
    {
        boost::ignore_unused(bytesTransfered);

        if(e)
        {
            writeLog("Failed write: " + e.message());
            return;
        }
        if(!keepAlive)
            return doClose();
        doRead();
    }

    void doClose()
    {
        beast::error_code e;
        _stream.socket().shutdown(asio::ip::tcp::socket::shutdown_send, e);
    }

    http::message_generator handleRequest(http::request<http::string_body>&& req) 
    {
        std::string target(req.target());
        http::response<http::string_body> res;
        res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
        res.set(http::field::content_type, "text/plain");
        res.keep_alive(req.keep_alive());
        res.result(http::status::not_found);
        res.body() = "invalid method or target";

        if(req.method() != http::verb::post)
            return res;

        if(target == "/info")
        {
            res.result(http::status::ok);
            res.body() = "Server status ok";
        }
        else if(target == "/log")
        {
            res.result(http::status::ok);
            res.body() = getLog();
        }
        else if(target == "/upload")
        {
            if(saveFile(std::move(req)))
            {
                res.result(http::status::ok);
                res.body() = "File uploaded";
            }
            else
            {
                res.result(http::status::ok);
                res.body() = "Failed uploading";
            }
        }

        res.prepare_payload();
        return res;
    }

    bool saveFile(http::request<http::string_body>&& req)
    {
        namespace fs = std::filesystem;
        std::string path = "/tmp";
        std::string name;

        for(auto& f : req.base())
            if(f.name_string() == "Content-Disposition")
            {
                size_t found = f.value().find("filename=");
                
                if(found == std::string::npos)
                {
                    writeLog("Error: Invalid Content-Disposition");
                    return false;
                }

                name = f.value().substr(found + std::string("filename=").size(), f.value().size());
            }

        
        try
        {
            if(!fs::exists(path))
            {
                writeLog("Error: /tmp folder");
                return false;
            }

            path += "/" + name;

            if(fs::exists(path))
                fs::remove(path);
        }
        catch(const std::exception& e)
        {
            writeLog("Error: /tmp folder " + std::string(e.what()));
            return false;
        }

        std::ofstream file;

        try
        {
            file.open(path, std::ios::out | std::ios::binary | std::ios::app);
            file.write(req.body().data(), req.body().size());
            file.close();
        }
        catch(const std::exception& e)
        {
            writeLog("Error: save file " + std::string(e.what()));
            return false;
        }

        writeLog("File uploaded: " + name);
        return true;
    }
    
};

class Server : public Log
{
    asio::ip::tcp::acceptor _accp;

public:
    Server(asio::io_context& ioc, unsigned short port) 
                : _accp(ioc, asio::ip::tcp::endpoint(asio::ip::tcp::v4(), port))
        {
            beast::error_code e;
            std::stringstream ss;
            ss << port;
            std::string portName(ss.str());
            std::cout << "Using Boost v" << BOOST_VERSION << "\n";

            writeLog("Server start. Listen port: " + portName);
            doAccept();
        }

        void close()
        {
            asio::post(_accp.get_executor(), [&]() { doClose(); });
        }
private:
        void doAccept()
        {
              _accp.async_accept(_accp.get_executor(),[this](beast::error_code e, asio::ip::tcp::socket sock)
            {
                if(!e)
                {
                    std::make_shared<Session>(std::move(sock))->run();
                    doAccept(); //allow next client connection
                }
                else if(e == asio::error::operation_aborted)
                    std::cout << "Server shutdown\n";
                else
                    writeLog("Error Accept: " + e.message());
            });
        }

        void doClose()
        {
            _accp.close();
        }
};