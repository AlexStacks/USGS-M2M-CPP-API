/// @author Alexander Stackpoole
/// @date 9/20/25
/// @brief Data types for USGS M2M API responses

#ifndef USGSM2MDATATYPES_HPP
#define USGSM2MDATATYPES_HPP

#include <iostream>
#include <string>
#include <optional>

/// Various structs for all API calls
struct ErrorResponse {
    std::string errorMessage;
    std::optional<int> errorCode;
};

struct MetaDataResponse {
    int requestId = -1;
    int sessionId = -1;
};

struct LoginAppGuestResponse {
    std::string data;
    ErrorResponse errorData;
    MetaDataResponse metaData;
    bool success = false;
};

struct LoginTokenResponse {
    std::string data;
    ErrorResponse errorData;
    MetaDataResponse metaData;
    bool success = false;
};

#endif //USGSM2MDATATYPES_HPP