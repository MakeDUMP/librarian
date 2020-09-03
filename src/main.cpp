#include <curl/curl.h>
#include <logger.hpp>
#include <memory>
#include <string>

auto main() -> int
try {
    makedump::logger logger { makedump::logger::format("{white+}", ">>>") };

    logger.println("Curl Version: {}", curl_version());

    std::unique_ptr<CURL, void (*)(CURL*)> curl { curl_easy_init(), curl_easy_cleanup };
    if (curl.get() == nullptr)
        throw std::runtime_error("Not make `curl` object");

    std::string error_str {};
    error_str.reserve(CURL_ERROR_SIZE);

    curl_easy_setopt(curl.get(), CURLOPT_URL, "https://habr.com/ru/post/439540/");
    curl_easy_setopt(curl.get(), CURLOPT_ERRORBUFFER, error_str.data());
    curl_easy_setopt(curl.get(), CURLOPT_SSL_VERIFYPEER, false);

    auto result = curl_easy_perform(curl.get());
    logger.println("{}", curl_easy_strerror(result));

    return 0;
} catch (std::exception const& e) {
    makedump::logger {}.println("{red+}: {}", "ERROR", e.what());
}

/*
    size_t write_callback(char* ptr, size_t size, size_t nmemb, void* userdata);
    curl_easy_setopt(curl.get(), CURLOPT_WRITEDATA, dataPointer);

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
