/// @author Alexander Stackpoole
/// @date 9/20/25
/// @brief Implementation of USGS M2M API C++ dataset class methods

#include "usgsm2m.hpp"

DefaultResponse USGS_M2M_API::dataset(const std::string& datasetName, const std::string& datasetId) {
    DefaultResponse result;

    if (datasetName.empty() && datasetId.empty()) {
        result.success = false;
        result.errorData.errorCode = -1;
        result.errorData.errorMessage = "At least one of 'datasetName' or 'datasetId' must be provided.";
        return result;
    }

    nlohmann::json requestJson;
    if (!datasetName.empty()) requestJson["datasetName"] = datasetName;
    if (!datasetId.empty()) requestJson["datasetId"] = datasetId;

    std::string jsonPayload = requestJson.dump();
    
    return defaultJsonResponseParsing(API_URL + "dataset", result.data, jsonPayload);
}

DefaultResponse USGS_M2M_API::datasetBrowse(const std::string& datasetId) {
    DefaultResponse result;

    if (datasetId.empty()) {
        result.success = false;
        result.errorData.errorCode = -1;
        result.errorData.errorMessage = "'datasetId' is required for dataset-browse.";
        return result;
    }

    nlohmann::json requestJson;
    requestJson["datasetId"] = datasetId;

    std::string jsonPayload = requestJson.dump();
    
    return defaultJsonResponseParsing(API_URL + "dataset-browse", result.data, jsonPayload);
}

DefaultResponse USGS_M2M_API::datasetBulkProducts(const std::string& datasetName) {

    DefaultResponse result;

    nlohmann::json requestJson;
    if (!datasetName.empty()) requestJson["datasetName"] = datasetName;

    std::string jsonPayload = requestJson.dump();
    
    return defaultJsonResponseParsing(API_URL + "dataset-bulk-products", result.data, jsonPayload);

}

DefaultResponse USGS_M2M_API::datasetCatalogs() {
    DefaultResponse result;

    return defaultJsonResponseParsing(API_URL + "dataset-catalogs", result.data);
}

DefaultResponse USGS_M2M_API::datasetCategories(
    const std::optional<std::string>& catalog,
    const std::optional<bool>& includeMessages,
    const std::optional<bool>& publicOnly,
    const std::optional<bool>& useCustomization,
    const std::optional<std::string>& parentId,
    const std::optional<std::string>& datasetFilter
) {
    DefaultResponse result;

    nlohmann::json requestJson;
    if(catalog) requestJson["catalog"] = *catalog;
    if(includeMessages) requestJson["includeMessages"] = *includeMessages;
    if(publicOnly) requestJson["publicOnly"] = *publicOnly;
    if(useCustomization) requestJson["useCustomization"] = *useCustomization;
    if(parentId) requestJson["parentId"] = *parentId;
    if(datasetFilter) requestJson["datasetFilter"] = *datasetFilter;

    std::string jsonPayload = requestJson.dump();

    return defaultJsonResponseParsing(API_URL + "dataset-categories", result.data, jsonPayload);
}

DefaultResponse USGS_M2M_API::datasetClearCustomization(
    const std::optional<std::string>& datasetName,
    const std::vector<std::string>& metadataType,
    const std::vector<std::string>& fileGroupIds
) {
    DefaultResponse result;

    nlohmann::json requestJson;

    if(datasetName) requestJson["datasetName"] = *datasetName;
    if(!metadataType.empty()) requestJson["metadataType"] = metadataType;
    if(!fileGroupIds.empty()) requestJson["fileGroupIds"] = fileGroupIds;

    std::string jsonPayload = requestJson.dump();
    
    return defaultJsonResponseParsing(API_URL + "dataset-clear-customization", result.data, jsonPayload);
}

DefaultResponse USGS_M2M_API::datasetCoverage(const std::string& datasetName) {
    DefaultResponse result;

    if (datasetName.empty()) {
        result.success = false;
        result.errorData.errorCode = -1;
        result.errorData.errorMessage = "'datasetName' is required for dataset-coverage.";
        return result;
    }

    nlohmann::json requestJson;
    requestJson["datasetName"] = datasetName;

    std::string jsonPayload = requestJson.dump();

    return defaultJsonResponseParsing(API_URL + "dataset-coverage", result.data, jsonPayload);
}

DefaultResponse USGS_M2M_API::datasetDownloadOptions(
    const std::string& datasetName,
    const std::optional<nlohmann::json>& sceneFilter
) {
    DefaultResponse result;

    if (datasetName.empty()) {
        result.success = false;
        result.errorData.errorCode = -1;
        result.errorData.errorMessage = "'datasetName' is required for dataset-download-options.";
        return result;
    }

    nlohmann::json requestJson;
    requestJson["datasetName"] = datasetName;
    if (sceneFilter) requestJson["sceneFilter"] = *sceneFilter;

    std::string jsonPayload = requestJson.dump();
    return defaultJsonResponseParsing(API_URL + "dataset-download-options", result.data, jsonPayload);
}

DefaultResponse USGS_M2M_API::datasetFileGroups(const std::string& datasetName) {
    DefaultResponse result;

    if (datasetName.empty()) {
        result.success = false;
        result.errorData.errorCode = -1;
        result.errorData.errorMessage = "'datasetName' is required for dataset-file-groups.";
        return result;
    }

    nlohmann::json requestJson;
    requestJson["datasetName"] = datasetName;

    std::string jsonPayload = requestJson.dump();
    return defaultJsonResponseParsing(API_URL + "dataset-file-groups", result.data, jsonPayload);
}

DefaultResponse USGS_M2M_API::datasetFilters(const std::string& datasetName) {
    DefaultResponse result;

    if (datasetName.empty()) {
        result.success = false;
        result.errorData.errorCode = -1;
        result.errorData.errorMessage = "'datasetName' is required for dataset-filters.";
        return result;
    }

    nlohmann::json requestJson;
    requestJson["datasetName"] = datasetName;

    std::string jsonPayload = requestJson.dump();
    return defaultJsonResponseParsing(API_URL + "dataset-filters", result.data, jsonPayload);
}

DefaultResponse USGS_M2M_API::datasetGetCustomization(const std::string& datasetName) {
    DefaultResponse result;

    if (datasetName.empty()) {
        result.success = false;
        result.errorData.errorCode = -1;
        result.errorData.errorMessage = "'datasetName' is required for dataset-get-customization.";
        return result;
    }

    nlohmann::json requestJson;
    requestJson["datasetName"] = datasetName;

    std::string jsonPayload = requestJson.dump();

    return defaultJsonResponseParsing(API_URL + "dataset-get-customization", result.data, jsonPayload);
}

DefaultResponse USGS_M2M_API::datasetGetCustomizations(
    const std::vector<std::string>& datasetNames,
    const std::vector<std::string>& metadataType
) {
    DefaultResponse result;

    nlohmann::json requestJson;

    if (!datasetNames.empty()) requestJson["datasetNames"] = datasetNames;
    if (!metadataType.empty()) requestJson["metadataType"] = metadataType;

    std::string jsonPayload = requestJson.dump();

    return defaultJsonResponseParsing(API_URL + "dataset-get-customizations", result.data, jsonPayload);
}

DefaultResponse USGS_M2M_API::datasetMessages(
    const std::optional<std::string>& catalog,
    const std::optional<std::string>& datasetName,
    const std::vector<std::string>& datasetNames
) {
    DefaultResponse result;

    nlohmann::json requestJson;

    if (catalog) requestJson["catalog"] = *catalog;
    if (datasetName) requestJson["datasetName"] = *datasetName;
    if (!datasetNames.empty()) requestJson["datasetNames"] = datasetNames;

    std::string jsonPayload = requestJson.dump();

    return defaultJsonResponseParsing(API_URL + "dataset-messages", result.data, jsonPayload);
}

DefaultResponse USGS_M2M_API::datasetMetadata(const std::string& datasetName) {
    DefaultResponse result;

    if (datasetName.empty()) {
        result.success = false;
        result.errorData.errorCode = -1;
        result.errorData.errorMessage = "'datasetName' is required for dataset-metadata.";
        return result;
    }

    nlohmann::json requestJson;
    requestJson["datasetName"] = datasetName;

    std::string jsonPayload = requestJson.dump();

    return defaultJsonResponseParsing(API_URL + "dataset-metadata", result.data, jsonPayload);
}

DefaultResponse USGS_M2M_API::datasetOrderProducts(const std::string& datasetName) {
    DefaultResponse result;

    if (datasetName.empty()) {
        result.success = false;
        result.errorData.errorCode = -1;
        result.errorData.errorMessage = "'datasetName' is required for dataset-order-products.";
        return result;
    }

    nlohmann::json requestJson;
    requestJson["datasetName"] = datasetName;

    std::string jsonPayload = requestJson.dump();

    return defaultJsonResponseParsing(API_URL + "dataset-order-products", result.data, jsonPayload);
}

DefaultResponse USGS_M2M_API::datasetSearch(
    const std::optional<std::string>& catalog,
    const std::optional<std::string>& categoryId,
    const std::optional<std::string>& datasetName,
    const std::optional<bool>& includeMessages,
    const std::optional<bool>& publicOnly,
    const std::optional<bool>& includeUnknownSpatial,
    const std::optional<nlohmann::json>& temporalFilter,
    const std::optional<nlohmann::json>& spatialFilter,
    const std::optional<std::string>& sortDirection,
    const std::optional<std::string>& sortField,
    const std::optional<bool>& useCustomization
) {
    DefaultResponse result;

    nlohmann::json requestJson;
    if(catalog) requestJson["catalog"] = *catalog;
    if(categoryId) requestJson["categoryId"] = *categoryId;
    if(datasetName) requestJson["datasetName"] = *datasetName;
    if(includeMessages) requestJson["includeMessages"] = *includeMessages;
    if(publicOnly) requestJson["publicOnly"] = *publicOnly;
    if(includeUnknownSpatial) requestJson["includeUnknownSpatial"] = *includeUnknownSpatial;
    if(temporalFilter) requestJson["temporalFilter"] = *temporalFilter;
    if(spatialFilter) requestJson["spatialFilter"] = *spatialFilter;
    if(sortDirection) requestJson["sortDirection"] = *sortDirection;
    if(sortField) requestJson["sortField"] = *sortField;
    if(useCustomization) requestJson["useCustomization"] = *useCustomization;

    std::string jsonPayload = requestJson.dump();

    return defaultJsonResponseParsing(API_URL + "dataset-search", result.data, jsonPayload);
}

DefaultResponse USGS_M2M_API::datasetSetCustomization(
    const std::string& datasetName,
    const std::optional<bool>& excluded,
    const std::optional<nlohmann::json>& metadata,
    const std::optional<nlohmann::json>& searchSort,
    const std::optional<nlohmann::json>& fileGroups
) {
    DefaultResponse result;

    if(datasetName.empty()) {
        result.success = false;
        result.errorData.errorCode = -1;
        result.errorData.errorMessage = "'datasetName' is required for dataset-set-customization.";
        return result;
    }

    nlohmann::json requestJson;
    requestJson["datasetName"] = datasetName;
    if(excluded) requestJson["excluded"] = *excluded;
    if(metadata) requestJson["metadata"] = *metadata;
    if(searchSort) requestJson["searchSort"] = *searchSort;
    if(fileGroups) requestJson["fileGroups"] = *fileGroups;

    std::string jsonPayload = requestJson.dump();

    return defaultJsonResponseParsing(API_URL + "dataset-set-customization", result.data, jsonPayload);
}

DefaultResponse USGS_M2M_API::datasetSetCustomizations(const std::vector<DatasetCustomization>& customizations) {
    DefaultResponse result;

    nlohmann::json jsonRequest;
    nlohmann::json datasetJson;

    for (const auto& dc : customizations) {
        nlohmann::json entry = datasetCustomizationToJson(dc);
        if (!entry.empty()) {
            datasetJson[dc.datasetName].push_back(std::move(entry));
        }
    }

    if (!datasetJson.empty()) {
        jsonRequest["datasetCustomization"] = std::move(datasetJson);
    }

    return defaultJsonResponseParsing(API_URL + "dataset-set-customizations", result.data, jsonRequest.dump());
}

nlohmann::json USGS_M2M_API::datasetCustomizationToJson(const DatasetCustomization& dc) {
    nlohmann::json entry;

    // Only include excluded if explicitly set
    if (dc.excluded.has_value()) {
        entry["excluded"] = *dc.excluded;
    }

    // Metadata
    if (!dc.metadata.empty()) {
        nlohmann::json metadataJson;
        for (const auto& [key, metaList] : dc.metadata) {
            if (!metaList.empty()) {
                nlohmann::json metaArray = nlohmann::json::array();
                for (const auto& m : metaList) {
                    nlohmann::json metaEntry;
                    if (!m.id.empty()) {
                        metaEntry["id"] = m.id;
                    }
                    if (m.sortOrder >= 0) {
                        metaEntry["sortOrder"] = m.sortOrder;
                    }
                    if (!metaEntry.empty()) {
                        metaArray.push_back(std::move(metaEntry));
                    }
                }
                if (!metaArray.empty()) {
                    metadataJson[key] = std::move(metaArray);
                }
            }
        }
        if (!metadataJson.empty()) {
            entry["metadata"] = std::move(metadataJson);
        }
    }

    // Search sort
    if (!dc.searchSort.empty()) {
        nlohmann::json searchSortArray = nlohmann::json::array();
        for (const auto& ss : dc.searchSort) {
            nlohmann::json ssEntry;
            if (!ss.id.empty()) {
                ssEntry["id"] = ss.id;
            }
            if (!ss.direction.empty()) {
                ssEntry["direction"] = ss.direction;
            }
            if (!ssEntry.empty()) {
                searchSortArray.push_back(std::move(ssEntry));
            }
        }
        if (!searchSortArray.empty()) {
            entry["search_sort"] = std::move(searchSortArray);
        }
    }

    // File groups
    if (!dc.fileGroups.empty()) {
        nlohmann::json fileGroupsJson;
        for (const auto& [groupId, productList] : dc.fileGroups) {
            if (!productList.empty()) {
                fileGroupsJson[groupId] = productList;
            }
        }
        if (!fileGroupsJson.empty()) {
            entry["fileGroups"] = std::move(fileGroupsJson);
        }
    }

    return entry;
}