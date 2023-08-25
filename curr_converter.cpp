#include <iostream>
#include <string>
#include <curl/curl.h>
#include "json.hpp"

using json = nlohmann::json;

size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

std::string performCurrencyConversion(const std::string& apiURL) {
    std::string result;
    
    CURL* curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, apiURL.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &result);

        CURLcode res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            std::cerr << "cURL error: " << curl_easy_strerror(res) << std::endl;
        }

        curl_easy_cleanup(curl);
    }
    
    return result;
}

int main() {
    // Ask for user input
    double amount;
    std::string fromCurrency, toCurrency;

    std::cout << "Enter the amount: ";
    std::cin >> amount;

    std::cout << "Enter the source currency (e.g., USD): ";
    std::cin >> fromCurrency;

    std::cout << "Enter the target currency (e.g., EUR): ";
    std::cin >> toCurrency;

    // Replace with your actual API key and API address
    std::string apiKey = "your_api_key_here";
    std::string apiURL = "your_api_address_here";

    std::string response = performCurrencyConversion(apiURL);

    // Parse the JSON response
    try {
        json jsonResponse = json::parse(response);
        double conversionRate = jsonResponse["data"][toCurrency]; // Access the rate using the currency code
        double convertedAmount = amount * conversionRate;

        std::cout << amount << " " << fromCurrency << " is equivalent to " << convertedAmount << " " << toCurrency << std::endl;
    } catch (const json::parse_error& e) {
        std::cerr << "JSON parsing error: " << e.what() << std::endl;
    }

    return 0;
}
