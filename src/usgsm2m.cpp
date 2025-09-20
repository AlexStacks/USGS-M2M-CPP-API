/// @author Alexander Stackpoole
/// @date 9/20/25
/// @brief Implementation of USGS_M2M_API class methods

#include "usgsM2M.hpp"

static const std::string API_URL =  "https://m2m.cr.usgs.gov/api/api/json/stable/";

LoginAppGuestResponse USGS_M2M_API::loginAppGuest(const std::string& applicationToken, const std::string& userToken) {
    LoginAppGuestResponse result;

    // Prepare JSON payload
    nlohmann::json requestJson;
    requestJson["applicationToken"] = applicationToken;
    requestJson["userToken"] = userToken;
    std::string jsonPayload = requestJson.dump();

    // Call HTTP POST helper
    std::string responseBody;
    long httpCode = 0;
    bool success = performJsonPostRequest(API_URL + "login-app-guest", jsonPayload, responseBody, httpCode);

    if (!success) {
        result.errorData.errorMessage = "Failed to perform HTTP POST request";
        result.success = false;
        return result;
    }

    if (httpCode != 200) {
        result.errorData.errorMessage = "HTTP error code: " + std::to_string(httpCode);
        result.success = false;
        return result;
    }

    // Parse JSON response
    try {
        nlohmann::json jsonResponse = nlohmann::json::parse(responseBody);

        if (!jsonResponse["errorCode"].is_null()) {
            result.errorData.errorCode = jsonResponse["errorCode"].get<int>();
            result.errorData.errorMessage = jsonResponse["errorMessage"].get<std::string>();
            result.metaData.requestId = jsonResponse["requestId"].get<int>();
            result.metaData.sessionId = jsonResponse["sessionId"].get<int>();
            result.success = false;
            return result;
        }

        result.data = jsonResponse["data"].get<std::string>();
        result.metaData.requestId = jsonResponse["requestId"].get<int>();
        result.metaData.sessionId = jsonResponse["sessionId"].get<int>();
        result.success = true;

    } catch (const std::exception& e) {
        result.errorData.errorMessage = std::string("JSON parse error: ") + e.what();
        result.success = false;
    }

    return result;
}
LoginTokenResponse USGS_M2M_API::loginToken(const std::string& username, const std::string& token, const nlohmann::json& userContext = {}) {
    LoginTokenResponse result;

    // Prepare JSON payload
    nlohmann::json requestJson;
    requestJson["username"] = username;
    requestJson["token"] = token;
    
    // Only add userContext if it's not empty
    if (!userContext.empty()) {
        requestJson["userContext"] = userContext;
    }

    std::string jsonPayload = requestJson.dump();

    // Call HTTP POST helper
    std::string responseBody;
    long httpCode = 0;
    bool success = performJsonPostRequest(API_URL + "login-token", jsonPayload, responseBody, httpCode);

    if (!success) {
        result.errorData.errorMessage = "Failed to perform HTTP POST request";
        result.success = false;
        return result;
    }

    if (httpCode != 200) {
        result.errorData.errorMessage = "HTTP error code: " + std::to_string(httpCode);
        result.success = false;
        return result;
    }

    // Parse JSON response
    try {
        nlohmann::json jsonResponse = nlohmann::json::parse(responseBody);

        if (!jsonResponse["errorCode"].is_null()) {
            result.errorData.errorCode = jsonResponse["errorCode"].get<int>();
            result.errorData.errorMessage = jsonResponse["errorMessage"].get<std::string>();
            result.metaData.requestId = jsonResponse["requestId"].get<int>();
            result.metaData.sessionId = jsonResponse["sessionId"].get<int>();
            result.success = false;
            return result;
        }

        result.data = jsonResponse["data"].get<std::string>();
        result.metaData.requestId = jsonResponse["requestId"].get<int>();
        result.metaData.sessionId = jsonResponse["sessionId"].get<int>();
        result.success = true;

    } catch (const std::exception& e) {
        result.errorData.errorMessage = std::string("JSON parse error: ") + e.what();
        result.success = false;
    }

    return result;
}

void USGS_M2M_API::setup_curl() {

    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();
    if (!curl) {
        std::cerr << "Failed to initialize cURL." << std::endl;
        return;
    }

    if (!headers) {
        headers = curl_slist_append(nullptr, "Content-Type: application/json");
    }

}

void USGS_M2M_API::cleanup_curl() {
    if (curl) {
        curl_easy_cleanup(curl);
    }
    
    // Free existing headers
    if (headers) {
        curl_slist_free_all(headers);
        headers = nullptr;
    }

    curl_global_cleanup();
}

bool USGS_M2M_API::performJsonPostRequest(const std::string& url,
    const std::string& jsonPayload,
    std::string& responseBody,
    long& httpCodeOut) {

if (!curl) {
    setup_curl();
    if (!curl) {
        return false;
    }
}

curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
curl_easy_setopt(curl, CURLOPT_POSTFIELDS, jsonPayload.c_str());
curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
curl_easy_setopt(curl, CURLOPT_WRITEDATA, &responseBody);
curl_easy_setopt(curl, CURLOPT_TIMEOUT, 10L);

CURLcode res = curl_easy_perform(curl);
if (res != CURLE_OK) {
    return false;
}
curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &httpCodeOut);
return true;
}

size_t USGS_M2M_API::WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    std::string* response = static_cast<std::string*>(userp);
    size_t totalSize = size * nmemb;
    response->append(static_cast<char*>(contents), totalSize);
    return totalSize;
}

void USGS_M2M_API::setAuthToken(const std::string& token) {
    // Free existing headers
    curl_slist_free_all(headers);
    headers = nullptr;
    headers = curl_slist_append(nullptr, "Content-Type: application/json");

    // Add the X-Auth-Token header
    std::string authHeader = "X-Auth-Token: " + token;
    headers = curl_slist_append(headers, authHeader.c_str());
}