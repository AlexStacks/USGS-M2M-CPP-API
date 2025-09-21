/// @author Alexander Stackpoole
/// @date 9/20/25
/// @brief Common Data types for USGS M2M API responses

#ifndef USGSM2M_COMMONDATATYPES_HPP
#define USGSM2M_COMMONDATATYPES_HPP

#include <iostream>
#include <string>
#include <optional>

/// Various structs for all API calls
struct ErrorResponse {
    std::string errorMessage;
    std::optional<int> errorCode;
};

struct UserContext {
    std::string contactId;
    std::string ipAddress;
};

struct MetaDataResponse {
    int requestId = -1;
    int sessionId = -1;
};

#endif //USGSM2M_COMMONDATATYPES_HPP