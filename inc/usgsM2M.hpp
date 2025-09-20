/// @author Alexander Stackpoole
/// @date 9/20/25
/// @brief USGS M2M API C++ Header


#ifndef USGSM2M_HPP
#define USGSM2M_HPP

#include "usgsM2MDataTypes.hpp"

#include <curl/curl.h>
#include <nlohmann/json.hpp>

class USGS_M2M_API {
public:

    /// @brief Constructor
    USGS_M2M_API() = default;

    /// @brief Destructor
    ~USGS_M2M_API() = default;

    /// @brief Login as an application guest
    /// @param applicationToken The application token
    /// @param userToken The user token
    /// @return LoginResponse struct containing the response data
    LoginAppGuestResponse loginAppGuest(const std::string& applicationToken, const std::string& userToken);

    /// @brief Login using username and token
    /// @param username ERS username
    /// @param token Application token
    /// @return LoginTokenResponse struct
    LoginTokenResponse loginToken(const std::string& username, const std::string& token, const nlohmann::json& userContext = {});

    /// @brief Set the X-Auth-Token header
    /// @param token The authentication token to be used in the request
    void setAuthToken(const std::string& token);


private:
    /// @brief CURL handle
    CURL *curl = nullptr;
    /// @brief CURL headers
    struct curl_slist *headers = nullptr;

    /// @brief Callback function for CURL write
    /// @param contents Pointer to the data
    /// @param size Size of each data element
    /// @param nmemb Number of data elements
    /// @param userp Pointer to user data
    /// @return Number of bytes processed
    static size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp);

    /// @brief Perform a JSON POST request
    /// @param url The URL to send the request to
    /// @param jsonPayload The JSON payload to send
    /// @param responseBody The response body (output)
    /// @param httpCodeOut The HTTP response code (output)
    /// @return true if the request was successful, false otherwise
    bool performJsonPostRequest(const std::string& url, const std::string& jsonPayload, std::string& responseBody, long& httpCodeOut);

    /// @brief Setup CURL
    void setup_curl();

    /// @brief Cleanup CURL
    void cleanup_curl();
};




#endif //USGSM2M_HPP