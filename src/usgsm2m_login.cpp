/// @author Alexander Stackpoole
/// @date 9/20/25
/// @brief Implementation of USGS M2M API C++ login class methods

#include "usgsm2m.hpp"

DefaultResponse USGS_M2M_API::loginAppGuest(const std::string& applicationToken, const std::string& userToken) {
    DefaultResponse result;

    // Prepare JSON payload
    nlohmann::json requestJson;
    requestJson["applicationToken"] = applicationToken;
    requestJson["userToken"] = userToken;
    std::string jsonPayload = requestJson.dump();

    return defaultJsonResponseParsing(API_URL + "login-app-guest", result.data, jsonPayload);
}

DefaultResponse USGS_M2M_API::loginToken(const std::string& username, const std::string& token, const UserContext& context) {
    DefaultResponse result;

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

    return defaultJsonResponseParsing(API_URL + "login-token", result.data, jsonPayload);
}

DefaultResponse USGS_M2M_API::loginSSO(const UserContext& context) {
    DefaultResponse result;

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

    return defaultJsonResponseParsing(API_URL + "login-sso", result.data, jsonPayload);;
}

LogoutResponse USGS_M2M_API::logout() {
    LogoutResponse result;

    // Call HTTP POST helper
    std::string responseBody;
    long httpCode = 0;
    result.success = performJsonGetRequest(API_URL + "logout", responseBody, httpCode);
    
    httpRequestSuccessful(httpCode, result.success, result.errorData);

    return result;
}