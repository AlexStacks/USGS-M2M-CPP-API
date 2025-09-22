/// @author Alexander Stackpoole
/// @date 9/20/25
/// @brief Implementation of USGS M2M API C++ scene class methods

#include "usgsm2m.hpp"

DefaultResponse USGS_M2M_API::sceneListAdd(
    const std::string& listId,
    const std::string& datasetName,
    const std::optional<std::string>& idField,
    const std::optional<std::string>& entityId,
    const std::optional<std::vector<std::string>>& entityIds,
    const std::optional<std::string>& timeToLive,
    const std::optional<bool>& checkDownloadRestriction
) {
    DefaultResponse result;

    if (listId.empty()) {
        result.success = false;
        result.errorData.errorCode = -1;
        result.errorData.errorMessage = "'listId' is required for scene-list-add.";
        return result;
    }
    if (datasetName.empty()) {
        result.success = false;
        result.errorData.errorCode = -1;
        result.errorData.errorMessage = "'datasetName' is required for scene-list-add.";
        return result;
    }

    nlohmann::json payload;
    payload["listId"] = listId;
    payload["datasetName"] = datasetName;

    if (idField) payload["idField"] = *idField;
    if (entityId) payload["entityId"] = *entityId;
    if (entityIds && !entityIds->empty()) payload["entityIds"] = *entityIds;
    if (timeToLive) payload["timeToLive"] = *timeToLive;
    if (checkDownloadRestriction) payload["checkDownloadRestriction"] = *checkDownloadRestriction;

    std::string jsonPayload = payload.dump();

    return defaultJsonResponseParsing(API_URL + "scene-list-add", result.data, jsonPayload);
}

/// @brief Retrieves items from a given scene list
/// @param listId Required list identifier
/// @param datasetName Optional dataset alias
/// @param startingNumber Optional starting number
/// @param maxResults Optional maximum number of results
/// @return defaultResponse containing the scene list
DefaultResponse USGS_M2M_API::sceneListGet(
    const std::string& listId,
    const std::optional<std::string>& datasetName,
    const std::optional<int>& startingNumber,
    const std::optional<int>& maxResults
) {
    DefaultResponse result;

    if (listId.empty()) {
        result.success = false;
        result.errorData.errorCode = -1;
        result.errorData.errorMessage = "'listId' is required for scene-list-get.";
        return result;
    }

    nlohmann::json payload;
    payload["listId"] = listId;
    if (datasetName) payload["datasetName"] = *datasetName;
    if (startingNumber) payload["startingNumber"] = *startingNumber;
    if (maxResults) payload["maxResults"] = *maxResults;

    std::string jsonPayload = payload.dump();

    return defaultJsonResponseParsing(API_URL + "scene-list-get", result.data, jsonPayload);
}

DefaultResponse USGS_M2M_API::sceneListRemove(
    const std::string& listId,
    const std::optional<std::string>& datasetName,
    const std::optional<std::string>& entityId,
    const std::optional<std::vector<std::string>>& entityIds
) {
    DefaultResponse result;

    if (listId.empty()) {
        result.success = false;
        result.errorData.errorCode = -1;
        result.errorData.errorMessage = "'listId' is required for scene-list-remove.";
        return result;
    }

    nlohmann::json payload;
    payload["listId"] = listId;
    if (datasetName) payload["datasetName"] = *datasetName;
    if (entityId) payload["entityId"] = *entityId;
    if (entityIds) payload["entityIds"] = *entityIds;

    std::string jsonPayload = payload.dump();

    return defaultJsonResponseParsing(API_URL + "scene-list-remove", result.data, jsonPayload);
}

DefaultResponse USGS_M2M_API::sceneListSummary(
    const std::string& listId,
    const std::optional<std::string>& datasetName
) {
    DefaultResponse result;

    if (listId.empty()) {
        result.success = false;
        result.errorData.errorCode = -1;
        result.errorData.errorMessage = "'listId' is required for scene-list-summary.";
        return result;
    }

    nlohmann::json payload;
    payload["listId"] = listId;
    if (datasetName) payload["datasetName"] = *datasetName;

    std::string jsonPayload = payload.dump();

    return defaultJsonResponseParsing(API_URL + "scene-list-summary", result.data, jsonPayload);
}

DefaultResponse USGS_M2M_API::sceneListTypes(const std::optional<std::string>& listFilter) {
    DefaultResponse result;
    nlohmann::json payload;

    if (listFilter) payload["listFilter"] = *listFilter;

    std::string jsonPayload = payload.dump();

    return defaultJsonResponseParsing(API_URL + "scene-list-types", result.data, jsonPayload);
}

DefaultResponse USGS_M2M_API::sceneMetadata(
    const std::string& datasetName,
    const std::string& entityId,
    const std::optional<std::string>& idType,
    const std::optional<std::string>& metadataType,
    const std::optional<bool>& includeNullMetadataValues,
    const std::optional<bool>& useCustomization
) {
    DefaultResponse result;

    if (datasetName.empty()) {
        result.success = false;
        result.errorData.errorCode = -1;
        result.errorData.errorMessage = "'datasetName' is required for scene-metadata.";
        return result;
    }

    if (entityId.empty()) {
        result.success = false;
        result.errorData.errorCode = -1;
        result.errorData.errorMessage = "'entityId' is required for scene-metadata.";
        return result;
    }

    nlohmann::json payload;
    payload["datasetName"] = datasetName;
    payload["entityId"] = entityId;

    if (idType) payload["idType"] = *idType;
    if (metadataType) payload["metadataType"] = *metadataType;
    if (includeNullMetadataValues) payload["includeNullMetadataValues"] = *includeNullMetadataValues;
    if (useCustomization) payload["useCustomization"] = *useCustomization;

    std::string jsonPayload = payload.dump();

    return defaultJsonResponseParsing(API_URL + "scene-metadata", result.data, jsonPayload);
}

DefaultResponse USGS_M2M_API::sceneMetadataList(
    const std::string& listId,
    const std::optional<std::string>& datasetName,
    const std::optional<std::string>& metadataType,
    const std::optional<bool>& includeNullMetadataValues,
    const std::optional<bool>& useCustomization
) {
    DefaultResponse result;

    if (listId.empty()) {
        result.success = false;
        result.errorData.errorCode = -1;
        result.errorData.errorMessage = "'listId' is required for scene-metadata-list.";
        return result;
    }

    nlohmann::json payload;
    payload["listId"] = listId;

    if (datasetName) payload["datasetName"] = *datasetName;
    if (metadataType) payload["metadataType"] = *metadataType;
    if (includeNullMetadataValues) payload["includeNullMetadataValues"] = *includeNullMetadataValues;
    if (useCustomization) payload["useCustomization"] = *useCustomization;

    std::string jsonPayload = payload.dump();

    return defaultJsonResponseParsing(API_URL + "scene-metadata-list", result.data, jsonPayload);
}

DefaultResponse USGS_M2M_API::sceneMetadataXml(
    const std::string& datasetName,
    const std::string& entityId,
    const std::optional<std::string>& metadataType
) {
    DefaultResponse result;

    if (datasetName.empty()) {
        result.success = false;
        result.errorData.errorCode = -1;
        result.errorData.errorMessage = "'datasetName' is required for scene-metadata-xml.";
        return result;
    }

    if (entityId.empty()) {
        result.success = false;
        result.errorData.errorCode = -1;
        result.errorData.errorMessage = "'entityId' is required for scene-metadata-xml.";
        return result;
    }

    nlohmann::json payload;
    payload["datasetName"] = datasetName;
    payload["entityId"] = entityId;

    if (metadataType) payload["metadataType"] = *metadataType;

    std::string jsonPayload = payload.dump();

    return defaultJsonResponseParsing(API_URL + "scene-metadata-xml", result.data, jsonPayload);
}

DefaultResponse USGS_M2M_API::sceneSearch(
    const std::string& datasetName,
    const std::optional<int>& maxResults,
    const std::optional<int>& startingNumber,
    const std::optional<std::string>& metadataType,
    const std::optional<std::string>& sortField,
    const std::optional<std::string>& sortDirection,
    const std::optional<SortCustomization>& sortCustomization,
    const std::optional<bool>& useCustomization,
    const std::optional<nlohmann::json>& sceneFilter,
    const std::optional<std::string>& compareListName,
    const std::optional<std::string>& bulkListName,
    const std::optional<std::string>& orderListName,
    const std::optional<std::string>& excludeListName,
    const std::optional<bool>& includeNullMetadataValues
) {
    DefaultResponse result;

    if (datasetName.empty()) {
        result.success = false;
        result.errorData.errorCode = -1;
        result.errorData.errorMessage = "'datasetName' is required for sceneSearch.";
        return result;
    }

    nlohmann::json payload;
    payload["datasetName"] = datasetName;

    if (maxResults) payload["maxResults"] = *maxResults;
    if (startingNumber) payload["startingNumber"] = *startingNumber;
    if (metadataType) payload["metadataType"] = *metadataType;
    if (sortField) payload["sortField"] = *sortField;
    if (sortDirection) payload["sortDirection"] = *sortDirection;
    if (useCustomization) payload["useCustomization"] = *useCustomization;
    if (compareListName) payload["compareListName"] = *compareListName;
    if (bulkListName) payload["bulkListName"] = *bulkListName;
    if (orderListName) payload["orderListName"] = *orderListName;
    if (excludeListName) payload["excludeListName"] = *excludeListName;
    if (includeNullMetadataValues) payload["includeNullMetadataValues"] = *includeNullMetadataValues;

    if (sortCustomization) {
        nlohmann::json obj;
        obj["field_name"] = sortCustomization->field_name;
        obj["direction"] = sortCustomization->direction;
        payload["sortCustomization"] = obj;
    }

    if (sceneFilter) {
        payload["sceneFilter"] = *sceneFilter;
    }

    std::string jsonPayload = payload.dump();

    return defaultJsonResponseParsing(API_URL + "scene-search", result.data, jsonPayload);
}

DefaultResponse USGS_M2M_API::sceneSearchDelete(
    const std::string& datasetName,
    const std::optional<int>& maxResults,
    const std::optional<int>& startingNumber,
    const std::optional<std::string>& sortField,
    const std::optional<std::string>& sortDirection,
    const std::optional<TemporalFilter>& temporalFilter
) {
    DefaultResponse result;

    if (datasetName.empty()) {
        result.success = false;
        result.errorData.errorCode = -1;
        result.errorData.errorMessage = "'datasetName' is required for sceneSearchDelete.";
        return result;
    }

    nlohmann::json payload;
    payload["datasetName"] = datasetName;

    if (maxResults) payload["maxResults"] = *maxResults;
    if (startingNumber) payload["startingNumber"] = *startingNumber;
    if (sortField) payload["sortField"] = *sortField;
    if (sortDirection) payload["sortDirection"] = *sortDirection;

    if (temporalFilter) {
        nlohmann::json tempJson;
        tempJson["start"] = timeToISO8601UTC(temporalFilter->start);
        tempJson["end"]   = timeToISO8601UTC(temporalFilter->end);
        payload["temporalFilter"] = tempJson;
    }

    std::string jsonPayload = payload.dump();

    return defaultJsonResponseParsing(API_URL + "scene-search-delete", result.data, jsonPayload);
}


DefaultResponse USGS_M2M_API::sceneSearchSecondary(
    const std::string& entityId,
    const std::string& datasetName,
    const std::optional<int>& maxResults,
    const std::optional<int>& startingNumber,
    const std::optional<std::string>& metadataType,
    const std::optional<std::string>& sortField,
    const std::optional<std::string>& sortDirection,
    const std::optional<std::string>& compareListName,
    const std::optional<std::string>& bulkListName,
    const std::optional<std::string>& orderListName,
    const std::optional<std::string>& excludeListName
) {
    DefaultResponse result;

    if (entityId.empty() || datasetName.empty()) {
        result.success = false;
        result.errorData.errorCode = -1;
        result.errorData.errorMessage = "'entityId' and 'datasetName' are required for sceneSearchSecondary.";
        return result;
    }

    nlohmann::json payload;
    payload["entityId"] = entityId;
    payload["datasetName"] = datasetName;

    if (maxResults) payload["maxResults"] = *maxResults;
    if (startingNumber) payload["startingNumber"] = *startingNumber;
    if (metadataType) payload["metadataType"] = *metadataType;
    if (sortField) payload["sortField"] = *sortField;
    if (sortDirection) payload["sortDirection"] = *sortDirection;
    if (compareListName) payload["compareListName"] = *compareListName;
    if (bulkListName) payload["bulkListName"] = *bulkListName;
    if (orderListName) payload["orderListName"] = *orderListName;
    if (excludeListName) payload["excludeListName"] = *excludeListName;

    std::string jsonPayload = payload.dump();

    return defaultJsonResponseParsing(API_URL + "scene-search-secondary", result.data, jsonPayload);
}

