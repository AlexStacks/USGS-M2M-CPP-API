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
    USGS_M2M_API();

    /// @brief Destructor
    ~USGS_M2M_API();

    /// @brief Login as an application guest
    /// @param applicationToken The application token
    /// @param userToken The user token
    /// @return LoginResponse struct containing the response data
    LoginAppGuestResponse loginAppGuest(const std::string& applicationToken, const std::string& userToken);

    /// @brief Login using username and token
    /// @param username ERS username
    /// @param token Application token
    /// @return LoginTokenResponse struct
    LoginTokenResponse loginToken(const std::string& username, const std::string& token, const UserContext& context = {});

    /// @brief Login using Single Sign-On (SSO) cookie
    /// @param context Metadata describing the user (contact ID and IP address)
    /// @return LoginSSOResponse struct
    LoginSSOResponse loginSSO(const UserContext& context = {});

    /// @brief Logout the current session
    /// @return LogoutResponse struct
    LogoutResponse logout();

    /// @brief Set the X-Auth-Token header
    /// @param token The authentication token to be used in the request
    void setAuthToken(const std::string& token);

    /// @brief Updates the header list with a new header or modifies an existing one
    /// @param header 
    void updateHeader(const std::string& header);

private:
    /// @brief CURL handle
    CURL *curl = nullptr;
    /// @brief CURL headers
    struct curl_slist *headers = nullptr;

    /// @brief headers vector for all headers
    std::vector<std::string> headersVector;

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

    /// @brief Cleanup CURLs
    void cleanup_curl();

    /// @brief Check for JSON error in response
    /// @param success Whether the request was successful
    /// @param httpCode The HTTP response code
    /// @return true if the request was successful, false otherwise
    bool httpRequestSuccessful(long httpCode, bool& success, ErrorResponse& errorData);

    /// @brief Parse JSON response for errors
    /// @param responseBody The response body
    /// @param errorData The ErrorResponse struct to populate
    /// @param success Whether the request was successful
    /// @return true if the there were no errors, false otherwise
    bool jsonErrorParsing(nlohmann::json& jsonResponse, ErrorResponse& errorData, bool& success);

    /// @brief Parse JSON response for errors
    /// @param responseBody The response body
    /// @param errorData The ErrorResponse struct to populate
    /// @param success Whether the request was successful
    /// @return true if the there were no errors, false otherwise
    void jsonMetaDataParsing(nlohmann::json& jsonResponse, MetaDataResponse& metaData);

};

#endif //USGSM2M_HPP