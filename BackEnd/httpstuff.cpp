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

pplx::task<void> HTTPGetAsync()
{
	// I want to make the following HTTP GET: http://api.flickr.com/services/rest/?method=flickr.test.echo&name=value
	//http_client client(U("http://api.flickr.com/services/rest/"));
	http_client client(U("http://crud.meteor.com/collectionapi/players"));

	uri_builder builder;
	// Append the query parameters: ?method=flickr.test.echo&name=value
	//builder.append_query(U("method"), U("flickr.test.echo"));
	//builder.append_query(U("name"), U("value"));
	builder.append_query(U("auth-token"), U("97f0ad9e24ca5e0408a269748d7fe0a0"));


	auto path_query_fragment = builder.to_string();

	// Make an HTTP GET request and asynchronously process the response
	return client.request(methods::GET, path_query_fragment).then([](http_response response)
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
int getTest()
{
	HTTPGetAsync().wait();
	return 0;
}

pplx::task<int> postTest()
{
	return pplx::create_task([]
	{
		json::value postData;
		postData[L"name"] = json::value::string(L"Dolan");
		postData[L"score"] = json::value::number(1993);
		postData[L"auth-token"] = json::value::string(L"97f0ad9e24ca5e0408a269748d7fe0a0");

		http_client client(L"http://crud.meteor.com/collectionapi/players");
		return client.request(methods::POST, L"",
			postData.to_string().c_str(), L"application/json");
	}).then([](http_response response)
	{
		if (response.status_code() == status_codes::OK)
		{
			auto body = response.extract_string();
			std::wcout << L"Added new Id: " << body.get().c_str() << std::endl;

			return std::stoi(body.get().c_str());
		}

		return 0;
	});
}

pplx::task<void> HTTPPostAsync()
{
	// I want to make the following HTTP GET: http://api.flickr.com/services/rest/?method=flickr.test.echo&name=value
	//http_client client(U("http://api.flickr.com/services/rest/"));
	http_client client(U("http://crud.meteor.com/collectionapi/players"));

	uri_builder builder;
	// Append the query parameters: ?method=flickr.test.echo&name=value
	//builder.append_query(U("method"), U("flickr.test.echo"));
	//builder.append_query(U("name"), U("value"));
	builder.append_query(U("X-Auth-Token"), U("97f0ad9e24ca5e0408a269748d7fe0a0"));
	builder.append_query(U("name"), U("Dolan"));
	//builder.append_query(U("score"), U("97f0ad9e24ca5e0408a269748d7fe0a0"));
	
	//postData[L"name"] = json::value::string(L"Dolan");
	//postData[L"score"] = json::value::number(1993);


	auto path_query_fragment = builder.to_string();
	//return client.request(methods::POST, L"",
		//postData.to_string().c_str(), L"application/json");
	// Make an HTTP GET request and asynchronously process the response
	return client.request(methods::POST, L"", path_query_fragment, L"application/json").then([](http_response response)
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