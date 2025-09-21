/// @author Alexander Stackpoole
/// @date 9/20/25
/// @brief Data types for Logging in and out with the USGS M2M API

#ifndef USGSM2M_LOGIN_HPP
#define USGSM2M_LOGIN_HPP

#include "usgsm2m_common.hpp"

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

#endif //USGSM2M_LOGIN_HPP