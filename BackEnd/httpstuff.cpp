#include "httpstuff.h"
#include <cpprest/http_client.h>
#include <cpprest/filestream.h>
#include <ppltasks.h>
#include <iostream>
#include <cpprest/json.h>
#include "cpprest/containerstream.h"
#include "cpprest/producerconsumerstream.h"
#include <sstream>
using namespace utility;                    // Common utilities like string conversions
using namespace web;                        // Common features like URIs.
using namespace web::http;                  // Common HTTP functionality
using namespace web::http::client;          // HTTP client features
using namespace concurrency::streams;       // Asynchronous streams
using namespace ::pplx;
using namespace web::json;

int bingtest()
{
	auto fileStream = std::make_shared<ostream>();

	// Open stream to output file.
	pplx::task<void> requestTask = fstream::open_ostream(U("results.html")).then([=](ostream outFile)
	{
		*fileStream = outFile;

		// Create http_client to send the request.
		http_client client(U("http://www.bing.com/"));

		// Build request URI and start the request.
		uri_builder builder(U("/search"));
		builder.append_query(U("q"), U("Casablanca CodePlex"));
		return client.request(methods::GET, builder.to_string());
	})

		// Handle response headers arriving.
		.then([=](http_response response)
	{
		printf("Received response status code:%u\n", response.status_code());

		// Write response body into the file.
		return response.body().read_to_end(fileStream->streambuf());
	})

		// Close the file stream.
		.then([=](size_t)
	{
		return fileStream->close();
	});

	// Wait for all the outstanding I/O to complete and handle any exceptions
	try
	{
		requestTask.wait();
	}
	catch (const std::exception &e)
	{
		printf("Error exception:%s\n", e.what());
	}

	return 0;
}
std::string jsonval;
std::string screenvals;
pplx::task<void> HTTPGetScreenAsync()
{
	// I want to make the following HTTP GET: http://api.flickr.com/services/rest/?method=flickr.test.echo&name=value
	//http_client client(U("http://api.flickr.com/services/rest/"));
	http_client client(U("http://erisgaming.cloudapp.net:3000/collectionapi/screen"));

	uri_builder builder;
	// Append the query parameters: ?method=flickr.test.echo&name=value
	//builder.append_query(U("method"), U("flickr.test.echo"));
	//builder.append_query(U("name"), U("value"));
	builder.append_query(U("auth-token"), U("97f0ad9e24ca5e0408a269748d7fe0a0"));
	//builder.append_query(U("name"), U("Ada Lovelace"));



	auto path_query_fragment = builder.to_string();

	// Make an HTTP GET request and asynchronously process the response
	return client.request(methods::GET, path_query_fragment).then([](http_response response)
	{

		// Display the status code that the server returned
		std::wostringstream stream;
		//stream << L"Server returned returned status code " << response.status_code() << L'.' << std::endl;
		//	std::wcout << stream.str();

		stream.str(std::wstring());
		//stream << L"Content type: " << response.headers().content_type() << std::endl;
		//	stream << L"Content length: " << response.headers().content_length() << L"bytes" << std::endl;
		//	std::wcout << stream.str();

		auto bodyStream = response.body();
		streams::stringstreambuf sbuffer;
		auto& target = sbuffer.collection();

		bodyStream.read_to_end(sbuffer).get();

		stream.str(std::wstring());
		//	stream << L"Response body: " << target.c_str();
		//	std::wcout << stream.str();

		// set global
		screenvals = target;


	});
}
pplx::task<void> HTTPGetAsync()
{
	// I want to make the following HTTP GET: http://api.flickr.com/services/rest/?method=flickr.test.echo&name=value
	//http_client client(U("http://api.flickr.com/services/rest/"));
	http_client client(U("http://erisgaming.cloudapp.net:3000/collectionapi/latest"));

	uri_builder builder;
	// Append the query parameters: ?method=flickr.test.echo&name=value
	//builder.append_query(U("method"), U("flickr.test.echo"));
	//builder.append_query(U("name"), U("value"));
	builder.append_query(U("auth-token"), U("97f0ad9e24ca5e0408a269748d7fe0a0"));
	//builder.append_query(U("name"), U("Ada Lovelace"));



	auto path_query_fragment = builder.to_string();

	// Make an HTTP GET request and asynchronously process the response
	return client.request(methods::GET, path_query_fragment).then([](http_response response)
	{

		// Display the status code that the server returned
		std::wostringstream stream;
		stream << L"Server returned returned status code " << response.status_code() << L'.' << std::endl;
	//	std::wcout << stream.str();

		stream.str(std::wstring());
		stream << L"Content type: " << response.headers().content_type() << std::endl;
		stream << L"Content length: " << response.headers().content_length() << L"bytes" << std::endl;
	//	std::wcout << stream.str();

		auto bodyStream = response.body();
		streams::stringstreambuf sbuffer;
		auto& target = sbuffer.collection();

		bodyStream.read_to_end(sbuffer).get();

		stream.str(std::wstring());
		stream << L"Response body: " << target.c_str();
	//	std::wcout << stream.str();

		// set global
		jsonval = target;


	});
}
int getTest()
{
	HTTPGetAsync().wait();
	return 0;
}
int getScreenTest()
{
	HTTPGetScreenAsync().wait();
	return 0;
}
int gcolor = 0;
int gcol = 0;
int grow = 0;
std::basic_string<wchar_t> gid = L"";
pplx::task<int> postTest(int color, int row, int col)
{
	gcolor = color;
	gcol = col;
	grow = row;
	return pplx::create_task([]
	{
		http_client client(L"http://erisgaming.cloudapp.net:3000/collectionapi/screen");

		// man
		http_request request(methods::POST);
		request.headers().add(L"X-Auth-Token", L"97f0ad9e24ca5e0408a269748d7fe0a0");


		json::value postData;
		//postData[L"X-Auth-Token"] = json::value::string(L"97f0ad9e24ca5e0408a269748d7fe0a0");
		postData[L"row"] = json::value::number(grow);
		postData[L"col"] = json::value::number(gcol);
		postData[L"color"] = json::value::number(gcolor);

		request.set_body(postData.to_string().c_str());

		/*	return client.request(methods::POST, L"",
				postData.to_string().c_str(), L"application/json");
				}).then([](http_response response)*/
		return client.request(request).then([](http_response response)
		{
			// Display the status code that the server returned
			std::wostringstream stream;
			stream << L"Server returned returned status code " << response.status_code() << L'.' << std::endl;
		//	std::wcout << stream.str();

			stream.str(std::wstring());
			stream << L"Content type: " << response.headers().content_type() << std::endl;
			stream << L"Content length: " << response.headers().content_length() << L"bytes" << std::endl;
		//	std::wcout << stream.str();

			auto bodyStream = response.body();
			streams::stringstreambuf sbuffer;
			auto& target = sbuffer.collection();

			bodyStream.read_to_end(sbuffer).get();

			stream.str(std::wstring());
			stream << L"Response body: " << target.c_str();
		//	std::wcout << stream.str();

			if (response.status_code() == status_codes::OK)
			{
				auto body = response.extract_string();
			//	std::wcout << L"Added new Id: " << body.get().c_str() << std::endl;

				return std::stoi(body.get().c_str());
			}

			return 0;
		});
	});
}
utility::string_t gdata;
pplx::task<int> putTest(int color, std::basic_string<wchar_t> id, utility::string_t data)
{
	gcolor = color;
	gid = id;
	gdata = data;
	return pplx::create_task([]()
	{
		std::basic_string<wchar_t> baseurl = L"http://erisgaming.cloudapp.net:3000/collectionapi/screen/";
		baseurl.append(gid);
		//std::wcout << baseurl << std::endl;
		http_client client(baseurl);

		// man
		http_request request(methods::PUT);
		request.headers().add(L"X-Auth-Token", L"97f0ad9e24ca5e0408a269748d7fe0a0");

		json::value postData;
		std::vector<std::pair<utility::string_t, json::value> > toInsert;
		std::pair<utility::string_t, json::value> tutu;
		tutu.first = L"bmp";
		tutu.second = json::value::string(gdata); //json::value::number(gcolor);
		toInsert.push_back(tutu);
		postData[L"$set"] = json::value::object(toInsert);

		request.set_body(postData.to_string().c_str());

		/*	return client.request(methods::POST, L"",
		postData.to_string().c_str(), L"application/json");
		}).then([](http_response response)*/
		return client.request(request).then([](http_response response)
		{
			// Display the status code that the server returned
			std::wostringstream stream;
			//stream << L"Server returned returned status code " << response.status_code() << L'.' << std::endl;
			//std::wcout << stream.str();

			stream.str(std::wstring());
			stream << L"Content type: " << response.headers().content_type() << std::endl;
			stream << L"Content length: " << response.headers().content_length() << L"bytes" << std::endl;
			//	std::wcout << stream.str();

			auto bodyStream = response.body();
			streams::stringstreambuf sbuffer;
			auto& target = sbuffer.collection();

			bodyStream.read_to_end(sbuffer).get();

			stream.str(std::wstring());
			stream << L"Response body: " << target.c_str();
			//	std::wcout << stream.str();

			if (response.status_code() == status_codes::OK)
			{
				//auto body = response.extract_string();
				//std::wcout << L"Added new Id: " << body.get().c_str() << std::endl;

				return 1;//std::stoi(body.get().c_str());
			}

			return 0;
		});
	});
}
pplx::task<void> HTTPPostAsync()
{
	// I want to make the following HTTP GET: http://api.flickr.com/services/rest/?method=flickr.test.echo&name=value
	//http_client client(U("http://api.flickr.com/services/rest/"));
	http_client client(U("http://erisgaming.cloudapp.net:3000/collectionapi/players"));

	uri_builder builder;
	// Append the query parameters: ?method=flickr.test.echo&name=value
	//builder.append_query(U("method"), U("flickr.test.echo"));
	//builder.append_query(U("name"), U("value"));

	//builder.append_query(U("X-Auth-Token"), U("97f0ad9e24ca5e0408a269748d7fe0a0"));

	builder.append_query(U("name"), U("Dolan"));
	//builder.append_query(U("score"), U("97f0ad9e24ca5e0408a269748d7fe0a0"));

	//postData[L"name"] = json::value::string(L"Dolan");
	//postData[L"score"] = json::value::number(1993);



	auto path_query_fragment = builder.to_string();

	//build the request from the data brohan

	http_request request(methods::POST);
	request.headers().add(L"X-Auth-Token", L"97f0ad9e24ca5e0408a269748d7fe0a0");
	request.headers().set_content_type(L"application/json");
	request.set_request_uri(path_query_fragment);


	//return client.request(methods::POST, L"",
	//postData.to_string().c_str(), L"application/json");
	// Make an HTTP GET request and asynchronously process the response
	//nearly works methods::POST, L"", path_query_fragment, L"application/json"
	return client.request(request).then([](http_response response)
	{
		// Display the status code that the server returned
		std::wostringstream stream;
		stream << L"Server returned returned status code " << response.status_code() << L'.' << std::endl;
		std::wcout << stream.str();

		stream.str(std::wstring());
		stream << L"Content type: " << response.headers().content_type() << std::endl;
		stream << L"Content length: " << response.headers().content_length() << L"bytes" << std::endl;
		std::wcout << stream.str();

		auto bodyStream = response.body();
		streams::stringstreambuf sbuffer;
		auto& target = sbuffer.collection();

		bodyStream.read_to_end(sbuffer).get();

		stream.str(std::wstring());
		stream << L"Response body: " << target.c_str();
		std::wcout << stream.str();

		// SERIALIZE INTO JSON 
		/*if (response.status_code() == status_codes::OK)
		{
		std::cout << "yo bro" << std::endl;
		}*/
		// DONE WITH JSON

	});
}

int testPost()
{
	HTTPPostAsync();
	return 0;
}
long long superstoi(std::basic_string<wchar_t> s)
{
	long long ret = 0;
	for (int k = 0; k < s.size(); k++)
	{
		int t = s[k] - '0';
		ret += t;
		ret *= 10;
	}
	ret /= 10;
	return ret;
}
std::vector<inputDB> parseInputDB()// must be preceded by a GET call
{
	auto ret = std::vector<inputDB>();
	json::value x = json::value::parse(utility::conversions::to_string_t(jsonval));
//	std::cout << x.to_string().c_str() << std::endl;
	for (auto k = x.as_array().cbegin(); k != x.as_array().cend(); k++)
	{
	//	std::cout << "\tObject" << std::endl;
		int move = -1;
		int movek = -1;
		long long timestamp = -1;
		for (auto j = k->as_object().cbegin(); j != k->as_object().cend(); j++)
		{
			auto a = j->first;


			if (a == L"move")
			{
				auto b = j->second;
				auto c = b.to_string();
				if (c == L"\"start\"")
				{
					move = STARTB;
					movek = STARTK;
				}
				else if (c == L"\"select\"")
				{
					move = SELECTB;
					movek = SELECTK;
				}
				else if (c == L"\"up\"")
				{
					move = UPB;
					movek = UPK;
				}
				else if (c == L"\"down\"")
				{
					move = DOWNB;
					movek = DOWNK;
				}
				else if (c == L"\"left\"")
				{
					move = LEFTB;
					movek = LEFTK;
				}
				else if (c == L"\"right\"")
				{
					move = RIGHTB;
					movek = RIGHTK;
				}
				else if (c == L"\"a\"")
				{
					move = AB;
					movek = AK;
				}
				else if (c == L"\"b\"")
				{
					move = BB;
					movek = BK;
				}
			}
			else if (a == L"time")
			{
				auto b = j->second;
				auto c = b.to_string();
				long long d = superstoi(c);
				timestamp = d;
			}
			if (move >= 0 && timestamp >= 0)
			{
				//we have a valid instance
				ret.push_back(inputDB(move, movek, timestamp));
			}

		}
	}
	return ret;
}
std::vector<screenVal> parseScreenVals()
{
	getScreenTest();
	auto ret = std::vector<screenVal>();
	json::value x = json::value::parse(utility::conversions::to_string_t(screenvals));
	//std::cout << x.to_string().c_str() << std::endl;
	for (auto k = x.as_array().cbegin(); k != x.as_array().cend(); k++)
	{
		screenVal s;
		for (auto j = k->as_object().cbegin(); j != k->as_object().cend(); j++)
		{
			auto a = j->first;

			if (a == L"bmp")
			{
				auto b = j->second;
				auto c = b.to_string();
				s.bmpData64 = c;
			}
			else if (a == L"_id")
			{
				auto b = j->second;
				auto c = b.to_string();
				s.id = c;
			}
			if (s.id != L""  && s.bmpData64 != L"")
			{
				//we have a valid instance
				ret.push_back(s);
			}

		}
	}
	return ret;
}