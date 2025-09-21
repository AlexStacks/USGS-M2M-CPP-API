/// @author Alexander Stackpoole
/// @date 9/20/25
/// @brief Data types for the dataset USGS M2M API

#ifndef USGSM2M_DATASET_HPP
#define USGSM2M_DATASET_HPP

#include "usgsm2m_common.hpp"
#include <vector>

struct SpatialBounds {
    double east;
    double west;
    double north;
    double south;
};

struct DatasetResponse {
    std::string datasetId;
    std::string datasetAlias;
    std::string datasetCategoryName;
    std::string collectionName;
    std::string collectionLongName;
    std::string abstractText;
    std::string doiNumber;
    std::string keywords;
    std::string dataOwner;
    std::string acquisitionStart;
    std::string acquisitionEnd;
    std::vector<std::string> catalogs;
    SpatialBounds spatialBounds;
    std::optional<int> sceneCount;
    std::optional<std::string> dateUpdated;
    std::optional<std::string> ingestFrequency;
    std::optional<std::string> legacyId;
    std::optional<std::string> temporalCoverage;
    bool supportCloudCover = false;
    bool supportDeletionSearch = false;
    ErrorResponse errorData;
    MetaDataResponse metaData;
    bool success = false;
};

struct DatasetBrowseEntry {
    std::string id;
    std::string browseName;
    std::string browseSource;
    std::string browseSourceName;
    bool browseRotationEnabled = false;
    bool browseKmzEnabled = false;
    bool isGeolocated = false;
    int displayOrder = 0;
    std::string overlaySpec;
};

struct DatasetBrowseResponse {
    std::vector<DatasetBrowseEntry> entries;
    ErrorResponse errorData;
    MetaDataResponse metaData;
    bool success = false;
};

struct BulkProduct {
    std::string productCode;
    std::string productName;
    std::optional<std::string> downloadName;
    std::optional<std::string> fileGroups;
};

struct BulkProductsResponse {
    std::vector<BulkProduct> products;
    ErrorResponse errorData;
    MetaDataResponse metaData;
    bool success = false;
};

struct DatasetCatalogsResponse {
    std::unordered_map<std::string, std::string> catalogs;
    ErrorResponse errorData;
    MetaDataResponse metaData;
    bool success = false;
};


struct DatasetCategoryDatasetInfo {
    std::vector<std::string> catalogs;
    std::string keywords;
    std::string dataOwner;
    std::string datasetId;
    std::string doiNumber;
    int sceneCount = 0;
    std::optional<std::string> dateUpdated;
    std::string abstractText;
    std::string datasetAlias;
    std::optional<SpatialBounds> spatialBounds;
    std::optional<std::string> acquisitionEnd;
    std::string collectionName;
    std::optional<std::string> ingestFrequency;
    std::optional<std::string> acquisitionStart;
    std::optional<std::string> temporalCoverage;
    bool supportCloudCover = false;
    std::string collectionLongName;
    std::string datasetCategoryName;
    bool supportDeletionSearch = false;
};

struct DatasetCategoryInfo {
    std::string id;
    std::string categoryName;
    std::optional<std::string> referenceLink;
    std::vector<DatasetCategoryInfo> subCategories;
    std::optional<std::string> parentCategoryId;
    std::optional<std::string> parentCategoryName;
    std::optional<std::string> categoryDescription;
    std::vector<DatasetCategoryDatasetInfo> datasets;
};

struct DatasetCategoriesResponse {
    std::map<std::string, DatasetCategoryInfo> categories;
    ErrorResponse errorData;
    MetaDataResponse metaData;
    bool success = false;
};

struct DatasetClearCustomizationResponse {
    int data = 0;
    ErrorResponse errorData;
    MetaDataResponse metaData;
    bool success = false;
};

#endif //USGSM2M_DATASET_HPP