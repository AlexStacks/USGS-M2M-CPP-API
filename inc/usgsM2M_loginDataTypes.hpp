/// @author Alexander Stackpoole
/// @date 9/20/25
/// @brief Data types for Logging in and out with the USGS M2M API

#ifndef USGSM2M_LOGINDATATYPES_HPP
#define USGSM2M_LOGINDATATYPES_HPP

#include "usgsM2M_commonDataTypes.hpp"

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

struct LoginSSOResponse {
    std::string apiKey;
    std::string username;
    ErrorResponse errorData;
    MetaDataResponse metaData;
    bool success = false;
};

struct LogoutResponse {
    ErrorResponse errorData;
    bool success = false;
};

#endif //USGSM2M_LOGINDATATYPES_HPP