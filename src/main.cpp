#include <curl/curl.h>
#include <logger.hpp>
#include <memory>
#include <string>

auto main() -> int
try {
    makedump::logger logger { makedump::logger::format("{white+}", ">>>") };

    logger.println("Curl Version: {yellow}", curl_version());

    std::unique_ptr<CURL, void (*)(CURL*)> curl { curl_easy_init(), curl_easy_cleanup };
    if (curl.get() == nullptr)
        throw std::runtime_error("Not make `curl` object");

    // download page URL
    std::string url { "https://habr.com/ru/post/439540/" };
    curl_easy_setopt(curl.get(), CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl.get(), CURLOPT_SSL_VERIFYPEER, false);

    // error string buffer
    std::string error_str {};
    error_str.reserve(CURL_ERROR_SIZE);
    curl_easy_setopt(curl.get(), CURLOPT_ERRORBUFFER, error_str.data());

    // HTML page string buffer
    std::string buffer {};
    auto write_callback = [](void* ptr, size_t size, size_t nmemb, void* userdata) -> size_t {
        auto buffer = reinterpret_cast<std::string*>(userdata);
        auto realsize = size * nmemb;
        buffer->append(reinterpret_cast<char const*>(ptr), realsize);
        return realsize;
    };
    curl_easy_setopt(curl.get(), CURLOPT_WRITEFUNCTION, static_cast<size_t (*)(void*, size_t, size_t, void*)>(write_callback));
    curl_easy_setopt(curl.get(), CURLOPT_WRITEDATA, &buffer);

    // get HTML page
    auto result = curl_easy_perform(curl.get());
    if (result != CURLE_OK)
        throw std::runtime_error(curl_easy_strerror(result));

    logger.println("Get from {green} {yellow} bytes | {blue+}", url, buffer.length(), buffer.substr(0, 15));

    return 0;
} catch (std::exception const& e) {
    makedump::logger {}.println("{red+}: {}", "ERROR", e.what());
}

/*
    CURLOPT_RETURNTRANSFER => true,     // return web page
    CURLOPT_HEADER         => false,    // don't return headers
    CURLOPT_FOLLOWLOCATION => true,     // follow redirects
    CURLOPT_ENCODING       => "",       // handle all encodings
    CURLOPT_USERAGENT      => "spider", // who am i
    CURLOPT_AUTOREFERER    => true,     // set referer on redirect
    CURLOPT_CONNECTTIMEOUT => 120,      // timeout on connect
    CURLOPT_TIMEOUT        => 120,      // timeout on response
    CURLOPT_MAXREDIRS      => 10,       // stop after 10 redirects
    CURLOPT_SSL_VERIFYPEER => false     // Disabled SSL Cert checks
*/
