/// @author Alexander Stackpoole
/// @date 9/20/25
/// @brief Implementation of USGS_M2M_API login class methods

#include "usgsm2m.hpp"

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

    // Call HTTP POST helper
    std::string responseBody;
    long httpCode = 0;
    result.success = performJsonGetRequest(API_URL + "logout", responseBody, httpCode);
    
    if(!httpRequestSuccessful(httpCode, result.success, result.errorData)) {
        return result;
    }

    return result;
}