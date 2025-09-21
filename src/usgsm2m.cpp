/// @author Alexander Stackpoole
/// @date 9/20/25
/// @brief Implementation of USGS_M2M_API class methods

#include "usgsM2M.hpp"

static const std::string API_URL =  "https://m2m.cr.usgs.gov/api/api/json/stable/";

USGS_M2M_API::USGS_M2M_API() {
    setup_curl();
}

USGS_M2M_API::~USGS_M2M_API() {
    cleanup_curl();
}

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
    result.success = performJsonPostRequest(API_URL + "login-app-guest", jsonPayload, responseBody, httpCode);
    
    nlohmann::json jsonResponse;
    // Parse JSON response
    try {
        jsonResponse = nlohmann::json::parse(responseBody);
    } catch (const std::exception& e) {
        result.errorData.errorCode = -1;
        result.errorData.errorMessage = std::string("JSON parse error: ") + e.what();
        result.success = false;
        return result;
    }

    if(!httpRequestSuccessful(httpCode, result.success, result.errorData)) {
        return result;
    }

    if(!jsonErrorParsing(jsonResponse, result.errorData, result.success)) {
        return result;
    }

    jsonMetaDataParsing(jsonResponse, result.metaData);

    if(jsonResponse.contains("data") && !jsonResponse["data"].is_null()) {
        result.data = jsonResponse["data"].get<std::string>();
    }else{
        result.success = false;
    }
    return result;
}

LoginTokenResponse USGS_M2M_API::loginToken(const std::string& username, const std::string& token, const UserContext& context) {
    LoginTokenResponse result;

    // Prepare JSON payload
    nlohmann::json requestJson;
    requestJson["username"] = username;
    requestJson["token"] = token;

    // Only add userContext if one of the fields is non-empty
    if (!context.contactId.empty() || !context.ipAddress.empty()) {
        requestJson["userContext"] = {
            { "contactId", context.contactId },
            { "ipAddress", context.ipAddress }
        };
    }

    std::string jsonPayload = requestJson.dump();

    // Call HTTP POST helper
    std::string responseBody;
    long httpCode = 0;
    result.success = performJsonPostRequest(API_URL + "login-token", jsonPayload, responseBody, httpCode);
    
    nlohmann::json jsonResponse;
    // Parse JSON response
    try {
        jsonResponse = nlohmann::json::parse(responseBody);
    } catch (const std::exception& e) {
        result.errorData.errorCode = -1;
        result.errorData.errorMessage = std::string("JSON parse error: ") + e.what();
        result.success = false;
        return result;
    }

    if(!httpRequestSuccessful(httpCode, result.success, result.errorData)) {
        return result;
    }

    if(!jsonErrorParsing(jsonResponse, result.errorData, result.success)) {
        return result;
    }

    jsonMetaDataParsing(jsonResponse, result.metaData);

    if(jsonResponse.contains("data") && !jsonResponse["data"].is_null()) {
        result.data = jsonResponse["data"].get<std::string>();
    }else{
        result.success = false;
    }

    return result;
}

LoginSSOResponse USGS_M2M_API::loginSSO(const UserContext& context) {
    LoginSSOResponse result;

    // Prepare JSON payload
    nlohmann::json requestJson;

    // Only add userContext if one of the fields is non-empty
    if (!context.contactId.empty() || !context.ipAddress.empty()) {
        requestJson["userContext"] = {
            { "contactId", context.contactId },
            { "ipAddress", context.ipAddress }
        };
    }

    std::string jsonPayload = requestJson.dump();

    // Perform POST
    std::string responseBody;
    long httpCode = 0;
    result.success = performJsonPostRequest(API_URL + "login-sso", jsonPayload, responseBody, httpCode);

    nlohmann::json jsonResponse;
    // Parse JSON response
    try {
        jsonResponse = nlohmann::json::parse(responseBody);
    } catch (const std::exception& e) {
        result.errorData.errorCode = -1;
        result.errorData.errorMessage = std::string("JSON parse error: ") + e.what();
        result.success = false;
        return result;
    }

    if(!httpRequestSuccessful(httpCode, result.success, result.errorData)) {
        return result;
    }

    if(!jsonErrorParsing(jsonResponse, result.errorData, result.success)) {
        return result;
    }

    jsonMetaDataParsing(jsonResponse, result.metaData);

    // Parse JSON Data field
    if(jsonResponse.contains("data") && !jsonResponse["data"].is_null() &&
            jsonResponse["data"].contains("apiKey") && !jsonResponse["data"]["apiKey"].is_null() &&
            jsonResponse["data"].contains("username") && !jsonResponse["data"]["username"].is_null()) {
            result.apiKey = jsonResponse["data"]["apiKey"].get<std::string>();
            result.username = jsonResponse["data"]["username"].get<std::string>();
    }else{
        result.success = false;
    }

    return result;
}

LogoutResponse USGS_M2M_API::logout() {
    LogoutResponse result;

    // Prepare empty JSON payload since logout takes no parameters
    std::string jsonPayload = "{}";

    // Call HTTP POST helper
    std::string responseBody;
    long httpCode = 0;
    result.success = performJsonPostRequest(API_URL + "logout", jsonPayload, responseBody, httpCode);
    
    if(!httpRequestSuccessful(httpCode, result.success, result.errorData)) {
        return result;
    }

    return result;
}

void USGS_M2M_API::setAuthToken(const std::string& token) {
    updateHeader("X-Auth-Token: " + token);
}

void USGS_M2M_API::updateHeader(const std::string& header){
    bool foundHeader = false;

    for(auto it = headersVector.begin(); it != headersVector.end(); ++it) {
        std::string key = header.substr(0, header.find(':'));
        if (it->substr(0, it->find(':')) == key) {
            *it = header;
            foundHeader = true;
            break;
        }
    }
    if(!foundHeader) {
        headersVector.push_back(header);
    }

    // Free existing headers
    curl_slist_free_all(headers);
    headers = nullptr;

    for(auto& hdr : headersVector) {
        headers = curl_slist_append(headers, hdr.c_str());
    }
}

size_t USGS_M2M_API::WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    std::string* response = static_cast<std::string*>(userp);
    size_t totalSize = size * nmemb;
    response->append(static_cast<char*>(contents), totalSize);
    return totalSize;
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

void USGS_M2M_API::setup_curl() {

    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();
    if (!curl) {
        std::cerr << "Failed to initialize cURL." << std::endl;
        return;
    }

    updateHeader("Content-Type: application/json");
    updateHeader("Accept: application/json");

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

bool checkJsonErrorRequest(long httpCode, bool& success, ErrorResponse& errorData) {
    if (!success) {
        errorData.errorCode = -1;
        errorData.errorMessage = "Failed to perform HTTP POST request";
    }

    if (httpCode != 200) {
        errorData.errorCode = -1;
        errorData.errorMessage = "HTTP error code: " + std::to_string(httpCode);
        success = false;
    }
    return success;
}

bool jsonErrorParsing(nlohmann::json& jsonResponse, ErrorResponse& errorData, bool& success) {
    if (jsonResponse.contains("errorCode") && !jsonResponse["errorCode"].is_null()) {
        errorData.errorCode = jsonResponse["errorCode"].get<int>();
        if(jsonResponse.contains("errorMessage") && !jsonResponse["errorMessage"].is_null()){
            errorData.errorMessage = jsonResponse["errorMessage"].get<std::string>();
        }
        return (success = false);
    }
    return true;
}

void jsonMetaDataParsing(nlohmann::json& jsonResponse, MetaDataResponse& metaData) {

    if (jsonResponse.contains("requestId") && !jsonResponse["requestId"].is_null()) {
        metaData.requestId = jsonResponse["requestId"].get<int>();
    }

    if (jsonResponse.contains("sessionId") && !jsonResponse["sessionId"].is_null()) {
        metaData.sessionId = jsonResponse["sessionId"].get<int>();
    }
}