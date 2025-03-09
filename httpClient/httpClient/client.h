#pragma once
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>
#include <boost/asio/strand.hpp>
#include <boost/asio.hpp>
#include <iostream>
#include <fstream>

using namespace boost;
using namespace beast;

beast::string_view mimeType(beast::string_view path)
{
	using beast::iequals;
	auto const ext = [&path]
		{
			auto const pos = path.rfind(".");
			if (pos == beast::string_view::npos)
				return beast::string_view{};
			return path.substr(pos);
		}();
	if (iequals(ext, ".htm"))  return "text/html";
	if (iequals(ext, ".html")) return "text/html";
	if (iequals(ext, ".php"))  return "text/html";
	if (iequals(ext, ".css"))  return "text/css";
	if (iequals(ext, ".txt"))  return "text/plain";
	if (iequals(ext, ".js"))   return "application/javascript";
	if (iequals(ext, ".json")) return "application/json";
	if (iequals(ext, ".xml"))  return "application/xml";
	if (iequals(ext, ".swf"))  return "application/x-shockwave-flash";
	if (iequals(ext, ".flv"))  return "video/x-flv";
	if (iequals(ext, ".png"))  return "image/png";
	if (iequals(ext, ".jpe"))  return "image/jpeg";
	if (iequals(ext, ".jpeg")) return "image/jpeg";
	if (iequals(ext, ".jpg"))  return "image/jpeg";
	if (iequals(ext, ".gif"))  return "image/gif";
	if (iequals(ext, ".bmp"))  return "image/bmp";
	if (iequals(ext, ".ico"))  return "image/vnd.microsoft.icon";
	if (iequals(ext, ".tiff")) return "image/tiff";
	if (iequals(ext, ".tif"))  return "image/tiff";
	if (iequals(ext, ".svg"))  return "image/svg+xml";
	if (iequals(ext, ".svgz")) return "image/svg+xml";
	return "application/text";
}

class Client : public std::enable_shared_from_this<Client>
{
	asio::ip::tcp::socket _sock;
	asio::ip::tcp::resolver _resolver;
	tcp_stream _stream;
	flat_buffer _buffer; // (Must persist between reads)
	http::request<http::string_body> _req;
	http::response<http::string_body> _res;
	std::string _host;
	std::string _port;
	std::string _target;
	std::string _filePath;

public:
	Client() = delete;
	Client(asio::io_context& ioc) : _resolver(beast::net::make_strand(ioc)), 
									_stream(beast::net::make_strand(ioc)), _sock(ioc) 
	{

	}

	void run(const std::string& ip, const std::string& portName, const std::string& target)
	{
		_host = ip;
		_port = portName;
		_target = target;

		if (prepareRequest())
		{
			system::error_code e;
			connect(e);

			if (!e)
				write(e);

			if (!e)
				read(e);
		}
		else
			fail("Error: prepareRequest");
	}

	void run(const std::string& target)
	{
		if (_host.empty() || _port.empty())
			return fail("Invalid ip or port");
		run(_host, _port, target);
	}

	void upload(const std::string& target, const std::string& path)
	{
		_filePath = path;
		run(target);
	}

	void close()
	{
		if(_stream.socket().is_open())
			_stream.socket().shutdown(asio::ip::tcp::socket::shutdown_both);
	}

private:

	void connect(system::error_code& e)
	{
		asio::ip::tcp::resolver resolver(_sock.get_executor());
		asio::connect(_sock, resolver.resolve(_host, _port), e);

		if (e)
			fail("connect " + e.message());
	}

	void write(system::error_code& e)
	{
		http::write(_sock, _req, e);

		if (e)
			fail("write " + e.message());
	}

	bool prepareRequest()
	{
		if (!_target.empty() && _target[0] != '/')
			_target = "/" + _target;

		_req = {};
		_req.version(10);
		_req.method(http::verb::post);
		_req.target(_target);
		_req.set(http::field::host, _host);
		_req.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);
		_req.set(http::field::content_type, "text/plain");

		if (_target == "/upload")
		{
			if (!fileTransfer())
				return false;
		}

		_req.prepare_payload();
		return true;
	}

	bool fileTransfer()
	{
		if (_filePath.empty())
		{
			fail("invalid filepath");
			return false;
		}

		std::ifstream file;
		file.open(_filePath, std::ios::in | std::ios_base::binary);

		if (!file.is_open())
		{
			fail("Open file");
			return false;
		}

		std::string data;
		char c;

		while (file >> std::noskipws >> c) data += c;
		file.close();

		if (data.empty())
		{
			fail("Read file");
			return false;
		}

		//!!!should check here data.size() before sending

		std::string fileName = getName(_filePath); //retrieve filename

		if (fileName.empty())
		{
			fail("Invalid file name");
			return false;
		}

		_req.body() = (char)fileName.size() + fileName + data; //first char is num of next chars as fileName
		_req.content_length(_req.body().size());
		_req.set(http::field::content_type, mimeType(_filePath));
		return true;
	}

	std::string getName(const std::string& path)
	{
		#ifdef WIN32
				char delimiter = '\\';
		#else
				char delimiter = '/';
		#endif

		bool ok = false;

		for (int i = path.size() - 1; i >= 0; --i)
		{
			if (path[i] == '.')
				ok = true;

			if (path[i] == delimiter && ok)
				return path.substr(++i, path.size() - 1);
		}

		return std::string();
	}

	void read(system::error_code& e)
	{
		_res = {};
		beast::flat_buffer buffer;
		http::read(_sock, buffer, _res, e);

		if (e && e != http::error::end_of_stream)
			fail("read " + e.message());
		else
			std::cout << _res << std::endl;
	}

	void fail(std::string&& str)
	{
		std::cout << "Error: " << str << std::endl;
	}
};