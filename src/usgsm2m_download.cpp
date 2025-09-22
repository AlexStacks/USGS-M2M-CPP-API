/// @author Alexander Stackpoole
/// @date 9/20/25
/// @brief Implementation of USGS M2M API C++ dataset class methods

#include "usgsm2m.hpp"

DefaultResponse USGS_M2M_API::downloadCompleteProxied(const std::vector<ProxiedDownload>& downloads) {
    DefaultResponse result;

    if (downloads.empty()) {
        result.success = false;
        result.errorData.errorCode = -1;
        result.errorData.errorMessage = "'downloads' cannot be empty for download-complete-proxied.";
        return result;
    }

    nlohmann::json requestJson;
    requestJson["proxiedDownloads"] = nlohmann::json::array();

    for (const auto& d : downloads) {
        nlohmann::json entry;
        entry["downloadId"] = d.downloadId;
        entry["downloadedSize"] = d.downloadedSize;
        requestJson["proxiedDownloads"].push_back(std::move(entry));
    }

    std::string jsonPayload = requestJson.dump();

    return defaultJsonResponseParsing(API_URL + "download-complete-proxied", result.data, jsonPayload);
}

DefaultResponse USGS_M2M_API::downloadEula(
    const std::optional<std::string>& eulaCode,
    const std::vector<std::string>& eulaCodes
) {
    DefaultResponse result;

    nlohmann::json requestJson;

    if (eulaCode) {
        requestJson["eulaCode"] = *eulaCode;
    }

    if (!eulaCodes.empty()) {
        requestJson["eulaCodes"] = eulaCodes;
    }

    std::string jsonPayload = requestJson.dump();

    return defaultJsonResponseParsing(API_URL + "download-eula", result.data, jsonPayload);
}

DefaultResponse USGS_M2M_API::downloadLabels(const std::optional<std::string>& downloadApplication) {
    DefaultResponse result;

    nlohmann::json requestJson;
    if (downloadApplication) {
        requestJson["downloadApplication"] = *downloadApplication;
    }

    std::string jsonPayload = requestJson.dump();

    return defaultJsonResponseParsing(API_URL + "download-labels", result.data, jsonPayload);
}

DefaultResponse USGS_M2M_API::downloadOptions(
    const std::string& datasetName,
    const std::optional<std::string>& entityIds,
    const std::optional<std::string>& listId,
    const std::optional<bool>& includeSecondaryFileGroups
) {
    DefaultResponse result;

    if (datasetName.empty()) {
        result.success = false;
        result.errorData.errorCode = -1;
        result.errorData.errorMessage = "'datasetName' is required for download-options.";
        return result;
    }

    nlohmann::json requestJson;
    requestJson["datasetName"] = datasetName;

    if (entityIds) requestJson["entityIds"] = *entityIds;
    if (listId) requestJson["listId"] = *listId;
    if (includeSecondaryFileGroups) requestJson["includeSecondaryFileGroups"] = *includeSecondaryFileGroups;

    std::string jsonPayload = requestJson.dump();

    return defaultJsonResponseParsing(API_URL + "download-options", result.data, jsonPayload);
}

DefaultResponse USGS_M2M_API::downloadOrderLoad(
    const std::optional<std::string>& label,
    const std::optional<std::string>& downloadApplication
) {
    DefaultResponse result;

    if (!label && !downloadApplication) {
        result.success = false;
        result.errorData.errorCode = -1;
        result.errorData.errorMessage = "At least one of 'label' or 'downloadApplication' must be provided for download-order-load.";
        return result;
    }

    nlohmann::json requestJson;

    if (label) requestJson["label"] = *label;
    if (downloadApplication) requestJson["downloadApplication"] = *downloadApplication;

    std::string jsonPayload = requestJson.dump();

    return defaultJsonResponseParsing(API_URL + "download-order-load", result.data, jsonPayload);
}

DefaultResponse USGS_M2M_API::downloadOrderRemove(
    const std::string& label,
    const std::optional<std::string>& downloadApplication
) {
    DefaultResponse result;

    if (label.empty()) {
        result.success = false;
        result.errorData.errorCode = -1;
        result.errorData.errorMessage = "'label' is required for download-order-remove.";
        return result;
    }

    nlohmann::json requestJson;
    requestJson["label"] = label;
    if (downloadApplication) requestJson["downloadApplication"] = *downloadApplication;

    std::string jsonPayload = requestJson.dump();

    return defaultJsonResponseParsing(API_URL + "download-order-remove", result.data, jsonPayload);
}

DefaultResponse USGS_M2M_API::downloadRemove(int downloadId) {
    DefaultResponse result;

    if (downloadId <= 0) {
        result.success = false;
        result.errorData.errorCode = -1;
        result.errorData.errorMessage = "'downloadId' must be a positive integer for download-remove.";
        return result;
    }

    nlohmann::json requestJson;
    requestJson["downloadId"] = downloadId;

    std::string jsonPayload = requestJson.dump();

    return defaultJsonResponseParsing(API_URL + "download-remove", result.data, jsonPayload);
}

DefaultResponse USGS_M2M_API::downloadRequest(
    const std::optional<std::string>& configurationCode,
    const std::optional<std::string>& downloadApplication,
    const std::optional<std::vector<Download>>& downloads,
    const std::optional<std::vector<FilepathDownload>>& dataPaths,
    const std::optional<std::string>& label,
    const std::optional<std::string>& systemId,
    const std::optional<std::vector<FilegroupDownload>>& dataGroups
) {
    DefaultResponse result;
    nlohmann::json payload;

    if (configurationCode) payload["configurationCode"] = *configurationCode;
    if (downloadApplication) payload["downloadApplication"] = *downloadApplication;
    if (label) payload["label"] = *label;
    if (systemId) payload["systemId"] = *systemId;

    if (downloads) {
        payload["downloads"] = nlohmann::json::array();
        for (const auto& d : *downloads) {
            if (d.entityId.empty()) {
                DefaultResponse resp;
                resp.success = false;
                resp.errorData.errorCode = -1;
                resp.errorData.errorMessage = "Download.entityId is required!";
                return resp;
            }
            nlohmann::json item;
            item["entityId"] = d.entityId;
            if (d.productId) item["productId"] = *d.productId;
            if (d.dataUse) item["dataUse"] = *d.dataUse;
            if (d.label) item["label"] = *d.label;
            payload["downloads"].push_back(item);
        }
    }
    
    if (dataPaths) {
        payload["dataPaths"] = nlohmann::json::array();
        for (const auto& f : *dataPaths) {
            if (f.datasetName.empty()) {
                DefaultResponse resp;
                resp.success = false;
                resp.errorData.errorCode = -1;
                resp.errorData.errorMessage = "FilepathDownload.datasetName is required!";
                return resp;
            }
            nlohmann::json item;
            item["datasetName"] = f.datasetName;
            if (f.productCode) item["productCode"] = *f.productCode;
            if (f.dataPath) item["dataPath"] = *f.dataPath;
            if (f.dataUse) item["dataUse"] = *f.dataUse;
            if (f.label) item["label"] = *f.label;
            payload["dataPaths"].push_back(item);
        }
    }
    
    if (dataGroups) {
        payload["dataGroups"] = nlohmann::json::array();
        for (const auto& fg : *dataGroups) {
            if (fg.datasetName.empty()) {
                DefaultResponse resp;
                resp.success = false;
                resp.errorData.errorCode = -1;
                resp.errorData.errorMessage = "FilegroupDownload.datasetName is required!";
                return resp;
            }
            nlohmann::json item;
            item["datasetName"] = fg.datasetName;
            if (fg.fileGroups) item["fileGroups"] = *fg.fileGroups;
            if (fg.listId) item["listId"] = *fg.listId;
            if (fg.dataUse) item["dataUse"] = *fg.dataUse;
            if (fg.label) item["label"] = *fg.label;
            payload["dataGroups"].push_back(item);
        }
    }

    std::string jsonPayload = payload.dump();

    return defaultJsonResponseParsing(API_URL + "download-request", result.data, jsonPayload);
}

DefaultResponse USGS_M2M_API::downloadRetrieve(
    const std::optional<std::string>& label,
    const std::optional<std::string>& downloadApplication
) {
    DefaultResponse result;
    nlohmann::json payload;

    // Only include parameters if they are provided
    if (label) {
        payload["label"] = *label;
    }
    if (downloadApplication) {
        payload["downloadApplication"] = *downloadApplication;
    }

    std::string jsonPayload = payload.dump();

    return defaultJsonResponseParsing(API_URL + "download-retrieve", result.data, jsonPayload);
}

DefaultResponse USGS_M2M_API::downloadSearch(
    const std::optional<bool>& activeOnly,
    const std::optional<std::string>& label,
    const std::optional<std::string>& downloadApplication,
    const std::optional<bool>& includeArchived
) {
    DefaultResponse result;
    nlohmann::json payload;

    if (activeOnly) payload["activeOnly"] = *activeOnly;
    if (label) payload["label"] = *label;
    if (downloadApplication) payload["downloadApplication"] = *downloadApplication;
    if (includeArchived) payload["includeArchived"] = *includeArchived;

    std::string jsonPayload = payload.dump();

    return defaultJsonResponseParsing(API_URL + "download-search", result.data, jsonPayload);
}

DefaultResponse USGS_M2M_API::downloadSummary(
    const std::string& downloadApplication,
    const std::string& label,
    const std::optional<bool>& sendEmail
) {
    DefaultResponse result;

    if (downloadApplication.empty()) {
        result.success = false;
        result.errorData.errorCode = -1;
        result.errorData.errorMessage = "'downloadApplication' is required for download-summary.";
        return result;
    }

    if (label.empty()) {
        result.success = false;
        result.errorData.errorCode = -1;
        result.errorData.errorMessage = "'label' is required for download-summary.";
        return result;
    }

    nlohmann::json payload;
    payload["downloadApplication"] = downloadApplication;
    payload["label"] = label;
    if (sendEmail) {
        payload["sendEmail"] = *sendEmail;
    }

    std::string jsonPayload = payload.dump();

    return defaultJsonResponseParsing(API_URL + "download-summary", result.data, jsonPayload);
}