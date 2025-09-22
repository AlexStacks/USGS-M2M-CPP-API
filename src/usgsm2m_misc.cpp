/// @author Alexander Stackpoole
/// @date 9/20/25
/// @brief Implementation of miscellaneous USGS M2M API C++ methods

#include "usgsm2m.hpp"

DefaultResponse USGS_M2M_API::grid2ll(
    const std::string& gridType,
    const std::optional<std::string>& responseShape,
    const std::optional<std::string>& path,
    const std::optional<std::string>& row
) {
    DefaultResponse result;

    if (gridType.empty()) {
        result.success = false;
        result.errorData.errorCode = -1;
        result.errorData.errorMessage = "'gridType' is required for grid2ll.";
        return result;
    }

    nlohmann::json payload;
    payload["gridType"] = gridType;
    if (responseShape) payload["responseShape"] = *responseShape;
    if (path) payload["path"] = *path;
    if (row) payload["row"] = *row;

    std::string jsonPayload = payload.dump();

    return defaultJsonResponseParsing(API_URL + "grid2ll", result.data, jsonPayload);
}

DefaultResponse USGS_M2M_API::notifications(const std::string& systemId) {
    DefaultResponse result;

    if (systemId.empty()) {
        result.success = false;
        result.errorData.errorCode = -1;
        result.errorData.errorMessage = "'systemId' is required for notifications.";
        return result;
    }

    nlohmann::json payload;
    payload["systemId"] = systemId;

    std::string jsonPayload = payload.dump();

    return defaultJsonResponseParsing(API_URL + "notifications", result.data, jsonPayload);
}

DefaultResponse USGS_M2M_API::orderProducts(
    const std::string& datasetName,
    const std::optional<std::string>& entityIds,
    const std::optional<std::string>& listId
) {
    DefaultResponse result;

    if (datasetName.empty()) {
        result.success = false;
        result.errorData.errorCode = -1;
        result.errorData.errorMessage = "'datasetName' is required for order-products.";
        return result;
    }

    nlohmann::json payload;
    payload["datasetName"] = datasetName;

    if (entityIds) payload["entityIds"] = *entityIds;
    if (listId) payload["listId"] = *listId;

    std::string jsonPayload = payload.dump();

    return defaultJsonResponseParsing(API_URL + "order-products", result.data, jsonPayload);
}

DefaultResponse USGS_M2M_API::orderSubmit(
    const std::vector<Product>& products,
    const std::optional<bool>& autoBulkOrder,
    const std::optional<std::string>& processingParameters,
    const std::optional<int>& priority,
    const std::optional<std::string>& orderComment,
    const std::optional<std::string>& systemId
) {
    DefaultResponse result;

    if (products.empty()) {
        result.success = false;
        result.errorData.errorCode = -1;
        result.errorData.errorMessage = "'products' cannot be empty for order-submit.";
        return result;
    }

    nlohmann::json payload;
    payload["products"] = nlohmann::json::array();

    for (const auto& p : products) {
        if (p.entityId.empty() || p.productId.empty() || p.datasetName.empty()) {
            DefaultResponse resp;
            resp.success = false;
            resp.errorData.errorCode = -1;
            resp.errorData.errorMessage = "Product.datasetName, entityId, and productId are required!";
            return resp;
        }

        nlohmann::json item;
        item["datasetName"] = p.datasetName;
        item["entityId"] = p.entityId;
        item["productId"] = p.productId;
        if (p.productCode) item["productCode"] = *p.productCode;

        payload["products"].push_back(item);
    }

    if (autoBulkOrder) payload["autoBulkOrder"] = *autoBulkOrder;
    if (processingParameters) payload["processingParameters"] = *processingParameters;
    if (priority) payload["priority"] = *priority;
    if (orderComment) payload["orderComment"] = *orderComment;
    if (systemId) payload["systemId"] = *systemId;

    std::string jsonPayload = payload.dump();

    return defaultJsonResponseParsing(API_URL + "order-submit", result.data, jsonPayload);
}

DefaultResponse USGS_M2M_API::permissions() {
    DefaultResponse result;
    return defaultJsonResponseParsing(API_URL + "permissions", result.data);
}

DefaultResponse USGS_M2M_API::placename(
    const std::optional<std::string>& featureType,
    const std::optional<std::string>& name
) {
    DefaultResponse result;
    nlohmann::json payload;

    if (featureType) payload["featureType"] = *featureType;
    if (name) payload["name"] = *name;

    std::string jsonPayload = payload.dump();

    return defaultJsonResponseParsing(API_URL + "placename", result.data, jsonPayload);
}

DefaultResponse USGS_M2M_API::rateLimitSummary(
    const std::optional<std::vector<std::string>>& ipAddress
) {
    DefaultResponse result;
    nlohmann::json payload;

    if (ipAddress) payload["ipAddress"] = *ipAddress;

    std::string jsonPayload = payload.dump();

    return defaultJsonResponseParsing(API_URL + "rate-limit-summary", result.data, jsonPayload);
}

DefaultResponse USGS_M2M_API::userPreferenceGet(
    const std::optional<std::string>& systemId,
    const std::optional<std::vector<std::string>>& setting
) {
    DefaultResponse result;
    nlohmann::json payload;

    if (systemId) payload["systemId"] = *systemId;
    if (setting) payload["setting"] = *setting;

    std::string jsonPayload = payload.dump();

    return defaultJsonResponseParsing(API_URL + "user-preference-get", result.data, jsonPayload);
}

DefaultResponse USGS_M2M_API::userPreferenceSet(
    const std::string& systemId,
    const nlohmann::json& userPreferences
) {
    DefaultResponse result;

    if (systemId.empty()) {
        result.success = false;
        result.errorData.errorCode = -1;
        result.errorData.errorMessage = "'systemId' is required for user-preference-set.";
        return result;
    }

    if (userPreferences.empty()) {
        result.success = false;
        result.errorData.errorCode = -1;
        result.errorData.errorMessage = "'userPreferences' cannot be empty for user-preference-set.";
        return result;
    }

    nlohmann::json payload;
    payload["systemId"] = systemId;
    payload["userPreferences"] = userPreferences;

    std::string jsonPayload = payload.dump();

    return defaultJsonResponseParsing(API_URL + "user-preference-set", result.data, jsonPayload);
}
