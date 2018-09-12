
#include <cpprest/http_client.h>
#include <cpprest/filestream.h>

web::http::client::http_client_config client_config_for_proxy()
{
	web::http::client::http_client_config client_config;
	wchar_t* pValue = nullptr;
	std::unique_ptr<wchar_t, void(*)(wchar_t*)> holder(nullptr, [](wchar_t* p) { free(p); });
	size_t len = 0;
	auto err = _wdupenv_s(&pValue, &len, L"http_proxy");
	if (pValue)
		holder.reset(pValue);
	if (!err && pValue && len) {
		std::wstring env_http_proxy_string(pValue, len - 1);
		if (env_http_proxy_string == U("auto"))
			client_config.set_proxy(web::web_proxy::use_auto_discovery);
		else
			client_config.set_proxy(web::web_proxy(env_http_proxy_string));
	}
	return client_config;
}

int synchronousSyntax()
{
	auto fileStream = std::make_shared<concurrency::streams::ostream>();
	concurrency::streams::ostream outFile = concurrency::streams::fstream::open_ostream(U("results.html")).get();
	*fileStream = outFile;

	web::http::client::http_client client(U("http://www.bing.com/"));

	web::http::uri_builder builder(U("/search"));
	builder.append_query(U("q"), U("cpprestsdk github"));
	web::http::http_response response = client.request(web::http::methods::GET, builder.to_string()).get();
	// Write response body into the file.
	response.body().read_to_end(fileStream->streambuf()).get();
	fileStream->close().get();
	return 0;
}

int wmain(int argc, wchar_t *args[])
{
	int result = synchronousSyntax();
	return result;
}


