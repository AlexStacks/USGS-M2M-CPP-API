/// @author Alexander Stackpoole
/// @date 9/20/25
/// @brief Implementation of the USGS M2M C++ basic functionality and helper functions.

#include "usgsm2m.hpp"
#include <iomanip>
#include <sstream>

USGS_M2M_API::USGS_M2M_API() {
    setup_curl();
}

USGS_M2M_API::~USGS_M2M_API() {
    cleanup_curl();
}

void USGS_M2M_API::setAuthToken(const std::string& token) {
    updateHeader("X-Auth-Token: " + token);
}

void USGS_M2M_API::updateHeader(const std::string& header){
    bool foundHeader = false;
    std::string key = header.substr(0, header.find(':'));

    for(auto& hdr : headersVector) {
        if (hdr.substr(0, hdr.find(':')) == key) {
            hdr = header;
            foundHeader = true;
            break;
        }
    }
    if(!foundHeader) headersVector.push_back(header);

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

    if (!curl) setup_curl();
    if (!curl) return false;

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, jsonPayload.c_str());
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &responseBody);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 10L);

    CURLcode res = curl_easy_perform(curl);
    if (res != CURLE_OK) return false;

    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &httpCodeOut);
    return true;
}

bool USGS_M2M_API::performJsonGetRequest(const std::string& url,
    std::string& responseBody,
    long& httpCodeOut) {

    if (!curl) setup_curl();
    if (!curl) return false;

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_HTTPGET, 1L);
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &responseBody);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 10L);

    CURLcode res = curl_easy_perform(curl);
    if (res != CURLE_OK) return false;

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
    if (curl) curl_easy_cleanup(curl);
    if (headers) curl_slist_free_all(headers);
    headers = nullptr;
    curl_global_cleanup();
}

bool USGS_M2M_API::httpRequestSuccessful(long httpCode, bool& success, ErrorResponse& errorData) {
    if (!success) errorData.errorCode = -1, errorData.errorMessage = "Failed to perform HTTP request";
    if (httpCode != 200) errorData.errorCode = -1, errorData.errorMessage = "HTTP error code: " + std::to_string(httpCode), success = false;
    return success;
}

bool USGS_M2M_API::jsonErrorParsing(nlohmann::json& jsonResponse, ErrorResponse& errorData, bool& success) {
    if (jsonResponse.contains("errorCode") && !jsonResponse["errorCode"].is_null()) {
        errorData.errorCode = jsonResponse["errorCode"].get<int>();
        errorData.errorMessage = safeGetStringOpt(jsonResponse, "errorMessage").value_or("");
        return (success = false);
    }
    return true;
}

void USGS_M2M_API::jsonMetaDataParsing(nlohmann::json& jsonResponse, MetaDataResponse& metaData) {
    metaData.requestId = safeGetIntOpt(jsonResponse, "requestId").value_or(0);
    metaData.sessionId = safeGetIntOpt(jsonResponse, "sessionId").value_or(0);
    metaData.version = safeGetStringOpt(jsonResponse, "version");
}

DefaultResponse USGS_M2M_API::defaultJsonResponseParsing(const std::string& url, nlohmann::json& jsonResponse, const std::string& jsonPayload) {
    DefaultResponse result;

    std::string responseBody;
    long httpCode = 0;

    if(jsonPayload.empty()){
        result.success = performJsonGetRequest(url, responseBody, httpCode);
    }
    else{
        result.success = performJsonPostRequest(url, jsonPayload, responseBody, httpCode);
    }
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
    if(jsonResponse.contains("data") && !jsonResponse["data"].is_null()) {
        result.data = jsonResponse["data"];
    }else{
        result.success = false;
    }

    return result;
}

std::optional<int> USGS_M2M_API::safeGetIntOpt(const nlohmann::json& j, const std::string& key) const {
    if (!j.contains(key)) return std::nullopt;
    if (j[key].is_number_integer()) return j[key].get<int>();
    if (j[key].is_string()) {
        try { return std::stoi(j[key].get<std::string>()); } 
        catch(...) { return std::nullopt; }
    }
    return std::nullopt;
}

std::optional<std::string> USGS_M2M_API::safeGetStringOpt(const nlohmann::json& j, const std::string& key) const {
    if (j.contains(key) && j[key].is_string()) return j[key].get<std::string>();
    return std::nullopt;
}

std::string USGS_M2M_API::timeToISO8601UTC(time_t t) {
    std::tm tm_utc;
    gmtime_r(&t, &tm_utc); // POSIX thread-safe

    std::ostringstream oss;
    oss << std::put_time(&tm_utc, "%FT%TZ"); // ISO 8601 UTC format
    return oss.str();
}