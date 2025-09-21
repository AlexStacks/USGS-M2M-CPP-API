/// @author Alexander Stackpoole
/// @date 9/20/25
/// @brief Implementation of USGS_M2M_API dataset class methods

#include "usgsm2m.hpp"

DatasetResponse USGS_M2M_API::dataset(const std::string& datasetName, const std::string& datasetId) {
    DatasetResponse result;

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
    std::string responseBody;
    long httpCode = 0;

    result.success = performJsonPostRequest(API_URL + "dataset", jsonPayload, responseBody, httpCode);

    nlohmann::json jsonResponse;
    try { jsonResponse = nlohmann::json::parse(responseBody); }
    catch (const std::exception& e) {
        result.success = false;
        result.errorData.errorCode = -1;
        result.errorData.errorMessage = std::string("JSON parse error: ") + e.what();
        return result;
    }

    if (!httpRequestSuccessful(httpCode, result.success, result.errorData)) return result;
    if (!jsonErrorParsing(jsonResponse, result.errorData, result.success)) return result;
    jsonMetaDataParsing(jsonResponse, result.metaData);

    if (!jsonResponse.contains("data") || jsonResponse["data"].is_null()) {
        result.success = false;
        result.errorData.errorMessage = "Missing 'data' field in response.";
        return result;
    }

    const auto& data = jsonResponse["data"];

    // Strings
    result.datasetId           = USGS_M2M_API::safeGetString(data, "datasetId");
    result.datasetAlias        = USGS_M2M_API::safeGetString(data, "datasetAlias");
    result.datasetCategoryName = USGS_M2M_API::safeGetString(data, "datasetCategoryName");
    result.collectionName      = USGS_M2M_API::safeGetString(data, "collectionName");
    result.collectionLongName  = USGS_M2M_API::safeGetString(data, "collectionLongName");
    result.abstractText        = USGS_M2M_API::safeGetString(data, "abstractText");
    result.doiNumber           = USGS_M2M_API::safeGetString(data, "doiNumber");
    result.keywords            = USGS_M2M_API::safeGetString(data, "keywords");
    result.dataOwner           = USGS_M2M_API::safeGetString(data, "dataOwner");
    result.acquisitionStart    = USGS_M2M_API::safeGetString(data, "acquisitionStart");
    result.acquisitionEnd      = USGS_M2M_API::safeGetString(data, "acquisitionEnd");

    // Optional strings
    result.dateUpdated         = USGS_M2M_API::safeGetStringOpt(data, "dateUpdated");
    result.ingestFrequency     = USGS_M2M_API::safeGetStringOpt(data, "ingestFrequency");
    result.legacyId            = USGS_M2M_API::safeGetStringOpt(data, "legacyId");
    result.temporalCoverage    = USGS_M2M_API::safeGetStringOpt(data, "temporalCoverage");

    // Arrays
    if (data.contains("catalogs") && data["catalogs"].is_array()) {
        result.catalogs = parseStringArray(data, "catalogs");
    }

    // Integers
    result.sceneCount = USGS_M2M_API::safeGetIntOpt(data, "sceneCount").value_or(0);

    // Booleans
    result.supportCloudCover    = USGS_M2M_API::safeGetBoolOpt(data, "supportCloudCover").value_or(false);
    result.supportDeletionSearch = USGS_M2M_API::safeGetBoolOpt(data, "supportDeletionSearch").value_or(false);

    // Spatial bounds
    if (data.contains("spatialBounds")) {
        auto sbOpt = parseSpatialBounds(data["spatialBounds"]);
        if (sbOpt) result.spatialBounds = *sbOpt;
    }

    result.success = true;
    return result;
}

DatasetBrowseResponse USGS_M2M_API::datasetBrowse(const std::string& datasetId) {
    DatasetBrowseResponse result;

    if (datasetId.empty()) {
        result.success = false;
        result.errorData.errorCode = -1;
        result.errorData.errorMessage = "'datasetId' is required for dataset-browse.";
        return result;
    }

    nlohmann::json requestJson;
    requestJson["datasetId"] = datasetId;

    std::string jsonPayload = requestJson.dump();
    std::string responseBody;
    long httpCode = 0;

    result.success = performJsonPostRequest(API_URL + "dataset-browse", jsonPayload, responseBody, httpCode);

    nlohmann::json jsonResponse;
    try { jsonResponse = nlohmann::json::parse(responseBody); }
    catch (const std::exception& e) {
        result.success = false;
        result.errorData.errorCode = -1;
        result.errorData.errorMessage = std::string("JSON parse error: ") + e.what();
        return result;
    }

    if (!httpRequestSuccessful(httpCode, result.success, result.errorData)) return result;
    if (!jsonErrorParsing(jsonResponse, result.errorData, result.success)) return result;
    jsonMetaDataParsing(jsonResponse, result.metaData);

    if (!jsonResponse.contains("data") || !jsonResponse["data"].is_array()) {
        result.success = false;
        result.errorData.errorMessage = "Missing or invalid 'data' field in response.";
        return result;
    }

    for (const auto& item : jsonResponse["data"]) {
        DatasetBrowseEntry entry;

        entry.id                 = USGS_M2M_API::safeGetString(item, "id");
        entry.browseName         = USGS_M2M_API::safeGetString(item, "browseName");
        entry.browseSource       = USGS_M2M_API::safeGetString(item, "browseSource");
        entry.browseSourceName   = USGS_M2M_API::safeGetString(item, "browseSourceName");
        entry.overlaySpec        = USGS_M2M_API::safeGetString(item, "overlaySpec");

        entry.browseRotationEnabled = USGS_M2M_API::safeGetBoolOpt(item, "browseRotationEnabled").value_or(false);
        entry.browseKmzEnabled      = USGS_M2M_API::safeGetBoolOpt(item, "browseKmzEnabled").value_or(false);
        entry.isGeolocated          = USGS_M2M_API::safeGetBoolOpt(item, "isGeolocated").value_or(false);
        entry.displayOrder          = USGS_M2M_API::safeGetIntOpt(item, "displayOrder").value_or(0);

        result.entries.push_back(entry);
    }

    result.success = true;
    return result;
}

BulkProductsResponse USGS_M2M_API::datasetBulkProducts(const std::string& datasetName) {

    BulkProductsResponse result;

    nlohmann::json requestJson;
    if (!datasetName.empty()) requestJson["datasetName"] = datasetName;

    std::string jsonPayload = requestJson.dump();
    std::string responseBody;
    long httpCode = 0;

    result.success = performJsonPostRequest(API_URL + "dataset-bulk-products", jsonPayload, responseBody, httpCode);

    nlohmann::json jsonResponse;
    try { jsonResponse = nlohmann::json::parse(responseBody); }
    catch (const std::exception& e) {
        result.success = false;
        result.errorData.errorCode = -1;
        result.errorData.errorMessage = std::string("JSON parse error: ") + e.what();
        return result;
    }

    if (!httpRequestSuccessful(httpCode, result.success, result.errorData)) return result;
    if (!jsonErrorParsing(jsonResponse, result.errorData, result.success)) return result;
    jsonMetaDataParsing(jsonResponse, result.metaData);

    if (!jsonResponse.contains("data") || !jsonResponse["data"].is_array()) {
        result.success = false;
        result.errorData.errorMessage = "Missing or invalid 'data' field in response.";
        return result;
    }

    for (const auto& item : jsonResponse["data"]) {
        BulkProduct product;

        product.productCode = USGS_M2M_API::safeGetString(item, "productCode");
        product.productName = USGS_M2M_API::safeGetString(item, "productName");
        product.downloadName = USGS_M2M_API::safeGetStringOpt(item, "downloadName");
        product.fileGroups   = USGS_M2M_API::safeGetStringOpt(item, "fileGroups");

        result.products.push_back(product);
    }

    result.success = true;
    return result;
}

DatasetCatalogsResponse USGS_M2M_API::datasetCatalogs() {
    DatasetCatalogsResponse result;

    std::string responseBody;
    long httpCode = 0;

    result.success = performJsonGetRequest(API_URL + "dataset-catalogs", responseBody, httpCode);

    nlohmann::json jsonResponse;
    try { jsonResponse = nlohmann::json::parse(responseBody); }
    catch (const std::exception& e) {
        result.success = false;
        result.errorData.errorCode = -1;
        result.errorData.errorMessage = std::string("JSON parse error: ") + e.what();
        return result;
    }

    if (!httpRequestSuccessful(httpCode, result.success, result.errorData)) return result;
    if (!jsonErrorParsing(jsonResponse, result.errorData, result.success)) return result;
    jsonMetaDataParsing(jsonResponse, result.metaData);

    if (!jsonResponse.contains("data") || !jsonResponse["data"].is_object()) {
        result.success = false;
        result.errorData.errorMessage = "Missing or invalid 'data' field in response.";
        return result;
    }

    const auto& catalogsJson = jsonResponse["data"];
    for (auto it = catalogsJson.begin(); it != catalogsJson.end(); ++it) {
        if (it.value().is_string()) {
            result.catalogs[it.key()] = safeGetString(it.value(), "");
        }
    }

    result.success = true;
    return result;
}

DatasetCategoriesResponse USGS_M2M_API::datasetCategories(
    const std::optional<std::string>& catalog,
    const std::optional<bool>& includeMessages,
    const std::optional<bool>& publicOnly,
    const std::optional<bool>& useCustomization,
    const std::optional<std::string>& parentId,
    const std::optional<std::string>& datasetFilter
) {
    DatasetCategoriesResponse result;

    nlohmann::json requestJson;
    if(catalog) requestJson["catalog"] = *catalog;
    if(includeMessages) requestJson["includeMessages"] = *includeMessages;
    if(publicOnly) requestJson["publicOnly"] = *publicOnly;
    if(useCustomization) requestJson["useCustomization"] = *useCustomization;
    if(parentId) requestJson["parentId"] = *parentId;
    if(datasetFilter) requestJson["datasetFilter"] = *datasetFilter;

    std::string jsonPayload = requestJson.dump();
    std::string responseBody;
    long httpCode = 0;

    result.success = performJsonPostRequest(API_URL + "dataset-categories", jsonPayload, responseBody, httpCode);

    nlohmann::json jsonResponse;
    try { jsonResponse = nlohmann::json::parse(responseBody); }
    catch (const std::exception& e) {
        result.success = false;
        result.errorData.errorCode = -1;
        result.errorData.errorMessage = std::string("JSON parse error: ") + e.what();
        return result;
    }

    if (!httpRequestSuccessful(httpCode, result.success, result.errorData)) return result;
    if (!jsonErrorParsing(jsonResponse, result.errorData, result.success)) return result;
    jsonMetaDataParsing(jsonResponse, result.metaData);

    if (!jsonResponse.contains("data") || !jsonResponse["data"].is_object()) {
        result.success = false;
        result.errorData.errorMessage = "Missing or invalid 'data' field in response.";
        return result;
    }

    const auto& categoriesJson = jsonResponse["data"];
    for (auto it = categoriesJson.begin(); it != categoriesJson.end(); ++it) {
        if (it.value().is_object()) {
            result.categories[it.key()] = parseCategoryRecursive(it.value());
        }
    }

    result.success = true;
    return result;
}

DatasetCategoryInfo USGS_M2M_API::parseCategoryRecursive(const nlohmann::json& categoryJson) {
    DatasetCategoryInfo category;

    category.id                 = safeGetStringOpt(categoryJson, "id").value_or("");
    category.categoryName       = safeGetStringOpt(categoryJson, "categoryName").value_or("");
    category.referenceLink      = safeGetStringOpt(categoryJson, "referenceLink");
    category.parentCategoryId   = safeGetStringOpt(categoryJson, "parentCategoryId");
    category.parentCategoryName = safeGetStringOpt(categoryJson, "parentCategoryName");
    category.categoryDescription= safeGetStringOpt(categoryJson, "categoryDescription");

    if(categoryJson.contains("datasets") && categoryJson["datasets"].is_array()) {
        for(const auto& dsJson : categoryJson["datasets"]) {
            DatasetCategoryDatasetInfo ds;

            if(dsJson.contains("catalogs") && dsJson["catalogs"].is_array())
                ds.catalogs = parseStringArray(dsJson, "catalogs");

            ds.keywords            = safeGetString(dsJson, "keywords");
            ds.dataOwner           = safeGetString(dsJson, "dataOwner");
            ds.datasetId           = safeGetString(dsJson, "datasetId");
            ds.doiNumber           = safeGetString(dsJson, "doiNumber");
            ds.abstractText        = safeGetString(dsJson, "abstractText");
            ds.datasetAlias        = safeGetString(dsJson, "datasetAlias");
            ds.collectionName      = safeGetString(dsJson, "collectionName");
            ds.collectionLongName  = safeGetString(dsJson, "collectionLongName");
            ds.datasetCategoryName = safeGetString(dsJson, "datasetCategoryName");

            ds.sceneCount           = safeGetIntOpt(dsJson, "sceneCount").value_or(0);
            ds.supportCloudCover    = safeGetBoolOpt(dsJson, "supportCloudCover").value_or(false);
            ds.supportDeletionSearch= safeGetBoolOpt(dsJson, "supportDeletionSearch").value_or(false);

            if(dsJson.contains("spatialBounds")) {
                auto sbOpt = parseSpatialBounds(dsJson["spatialBounds"]);
                if(sbOpt) ds.spatialBounds = *sbOpt;
            }

            ds.acquisitionStart     = safeGetStringOpt(dsJson, "acquisitionStart");
            ds.acquisitionEnd       = safeGetStringOpt(dsJson, "acquisitionEnd");
            ds.dateUpdated          = safeGetStringOpt(dsJson, "dateUpdated");
            ds.ingestFrequency      = safeGetStringOpt(dsJson, "ingestFrequency");
            ds.temporalCoverage     = safeGetStringOpt(dsJson, "temporalCoverage");

            category.datasets.push_back(ds);
        }
    }

    if(categoryJson.contains("subCategories") && categoryJson["subCategories"].is_array()) {
        for(const auto& subJson : categoryJson["subCategories"]) {
            category.subCategories.push_back(parseCategoryRecursive(subJson));
        }
    }

    return category;
}

DatasetClearCustomizationResponse USGS_M2M_API::datasetClearCustomization(
    const std::optional<std::string>& datasetName,
    const std::vector<std::string>& metadataType,
    const std::vector<std::string>& fileGroupIds
) {
    DatasetClearCustomizationResponse result;

    nlohmann::json requestJson;

    if(datasetName) requestJson["datasetName"] = *datasetName;
    if(!metadataType.empty()) requestJson["metadataType"] = metadataType;
    if(!fileGroupIds.empty()) requestJson["fileGroupIds"] = fileGroupIds;

    std::string jsonPayload = requestJson.dump();
    std::string responseBody;
    long httpCode = 0;

    result.success = performJsonPostRequest(API_URL + "dataset-clear-customization", jsonPayload, responseBody, httpCode);

    nlohmann::json jsonResponse;
    try { jsonResponse = nlohmann::json::parse(responseBody); }
    catch (const std::exception& e) {
        result.success = false;
        result.errorData.errorCode = -1;
        result.errorData.errorMessage = std::string("JSON parse error: ") + e.what();
        return result;
    }

    if(!httpRequestSuccessful(httpCode, result.success, result.errorData)) return result;
    if(!jsonErrorParsing(jsonResponse, result.errorData, result.success)) return result;
    jsonMetaDataParsing(jsonResponse, result.metaData);

    // Parse "data" (int) from response
    if(jsonResponse.contains("data") && jsonResponse["data"].is_number_integer()) {
        result.data = jsonResponse["data"].get<int>();
    } else {
        result.success = false;
        result.errorData.errorMessage = "Missing or invalid 'data' field in response.";
    }

    return result;
}

