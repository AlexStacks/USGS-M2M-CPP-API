/// @author Alexander Stackpoole
/// @date 9/20/25
/// @brief Implementation of USGS M2M API C++ tram class methods

#include "usgsm2m.hpp"

DefaultResponse USGS_M2M_API::tramOrderDetailUpdate(
    const std::string& orderNumber,
    const std::string& detailKey,
    const std::string& detailValue
) {
    DefaultResponse result;

    if (orderNumber.empty()) {
        result.success = false;
        result.errorData.errorCode = -1;
        result.errorData.errorMessage = "'orderNumber' is required for tram-order-detail-update.";
        return result;
    }

    if (detailKey.empty()) {
        result.success = false;
        result.errorData.errorCode = -1;
        result.errorData.errorMessage = "'detailKey' is required for tram-order-detail-update.";
        return result;
    }

    if (detailValue.empty()) {
        result.success = false;
        result.errorData.errorCode = -1;
        result.errorData.errorMessage = "'detailValue' is required for tram-order-detail-update.";
        return result;
    }

    nlohmann::json payload;
    payload["orderNumber"] = orderNumber;
    payload["detailKey"]   = detailKey;
    payload["detailValue"] = detailValue;

    std::string jsonPayload = payload.dump();

    return defaultJsonResponseParsing(API_URL + "tram-order-detail-update", result.data, jsonPayload);
}

DefaultResponse USGS_M2M_API::tramOrderDetails(
    const std::string& orderNumber
) {
    DefaultResponse result;

    if (orderNumber.empty()) {
        result.success = false;
        result.errorData.errorCode = -1;
        result.errorData.errorMessage = "'orderNumber' is required for tram-order-details.";
        return result;
    }

    nlohmann::json payload;
    payload["orderNumber"] = orderNumber;

    std::string jsonPayload = payload.dump();

    return defaultJsonResponseParsing(API_URL + "tram-order-details", result.data, jsonPayload);
}

DefaultResponse USGS_M2M_API::tramOrderDetailsClear(
    const std::string& orderNumber
) {
    DefaultResponse result;

    if (orderNumber.empty()) {
        result.success = false;
        result.errorData.errorCode = -1;
        result.errorData.errorMessage = "'orderNumber' is required for tram-order-details-clear.";
        return result;
    }

    nlohmann::json payload;
    payload["orderNumber"] = orderNumber;

    std::string jsonPayload = payload.dump();

    return defaultJsonResponseParsing(API_URL + "tram-order-details-clear", result.data, jsonPayload);
}

DefaultResponse USGS_M2M_API::tramOrderDetailsRemove(
    const std::string& orderNumber,
    const std::string& detailKey
) {
    DefaultResponse result;

    if (orderNumber.empty()) {
        result.success = false;
        result.errorData.errorCode = -1;
        result.errorData.errorMessage = "'orderNumber' is required for tram-order-details-remove.";
        return result;
    }

    if (detailKey.empty()) {
        result.success = false;
        result.errorData.errorCode = -1;
        result.errorData.errorMessage = "'detailKey' is required for tram-order-details-remove.";
        return result;
    }

    nlohmann::json payload;
    payload["orderNumber"] = orderNumber;
    payload["detailKey"] = detailKey;

    std::string jsonPayload = payload.dump();

    return defaultJsonResponseParsing(API_URL + "tram-order-details-remove", result.data, jsonPayload);
}

DefaultResponse USGS_M2M_API::tramOrderSearch(
    const std::optional<std::string>& orderId,
    const std::optional<int>& maxResults,
    const std::optional<std::string>& systemId,
    const std::optional<bool>& sortAsc,
    const std::optional<std::string>& sortField,
    const std::optional<std::vector<std::string>>& statusFilter
) {
    DefaultResponse result;

    nlohmann::json payload;

    if (orderId) payload["orderId"] = *orderId;
    if (maxResults) payload["maxResults"] = *maxResults;
    if (systemId) payload["systemId"] = *systemId;
    if (sortAsc) payload["sortAsc"] = *sortAsc;
    if (sortField) payload["sortField"] = *sortField;
    if (statusFilter && !statusFilter->empty()) payload["statusFilter"] = *statusFilter;

    std::string jsonPayload = payload.dump();

    return defaultJsonResponseParsing(API_URL + "tram-order-search", result.data, jsonPayload);
}

DefaultResponse USGS_M2M_API::tramOrderStatus(const std::string& orderNumber) {
    DefaultResponse result;

    if (orderNumber.empty()) {
        result.success = false;
        result.errorData.errorCode = -1;
        result.errorData.errorMessage = "'orderNumber' is required for tram-order-status.";
        return result;
    }

    nlohmann::json payload;
    payload["orderNumber"] = orderNumber;

    std::string jsonPayload = payload.dump();

    return defaultJsonResponseParsing(API_URL + "tram-order-status", result.data, jsonPayload);
}

DefaultResponse USGS_M2M_API::tramOrderUnits(const std::string& orderNumber) {
    DefaultResponse result;

    if (orderNumber.empty()) {
        result.success = false;
        result.errorData.errorCode = -1;
        result.errorData.errorMessage = "'orderNumber' is required for tram-order-units.";
        return result;
    }

    nlohmann::json payload;
    payload["orderNumber"] = orderNumber;

    std::string jsonPayload = payload.dump();

    return defaultJsonResponseParsing(API_URL + "tram-order-units", result.data, jsonPayload);
}
