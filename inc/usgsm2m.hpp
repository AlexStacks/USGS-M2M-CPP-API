/// @author Alexander Stackpoole
/// @date 9/20/25
/// @brief USGS M2M API C++ Header


#ifndef USGSM2M_HPP
#define USGSM2M_HPP

#include <curl/curl.h>
#include <nlohmann/json.hpp>
#include <iostream>
#include <string>
#include <optional>
#include <ctime>

static const std::string API_URL =  "https://m2m.cr.usgs.gov/api/api/json/stable/";

struct ErrorResponse {
    std::string errorMessage;
    std::optional<int> errorCode;
};

struct UserContext {
    std::string contactId;
    std::string ipAddress;
};

struct MetaDataResponse {
    std::optional<std::string> version;
    int requestId = -1;
    int sessionId = -1;
};

struct DefaultResponse {
    nlohmann::json data;
    ErrorResponse errorData;
    MetaDataResponse metaData;
    bool success = false;
};

struct LogoutResponse {
    ErrorResponse errorData;
    bool success = false;
};

/// @brief Represents a single metadata entry used in dataset customizations
struct MetadataEntry {
    /// @brief Metadata ID
    std::string id;        
    /// @brief Sort order, -1 if not set
    int sortOrder = -1;    
};

/// @brief Represents a single search sort entry
struct SearchSortEntry {
    /// @brief Field ID to sort on
    std::string id;
    /// @brief Sort direction, either "ASC" or "DESC"
    std::string direction;
};

/// @brief Represents a dataset customization request
struct DatasetCustomization {
    std::string datasetName;
    std::optional<bool> excluded;
    /// @brief Map of metadata type to list of MetadataEntry
    std::map<std::string, std::vector<MetadataEntry>> metadata;
    /// @brief List of search sort entries
    std::vector<SearchSortEntry> searchSort;
    /// @brief Map of file group IDs to list of product IDs
    std::map<std::string, std::vector<std::string>> fileGroups;
};

struct ProxiedDownload {
    int downloadId;
    size_t downloadedSize;
};

struct Download {
    std::string entityId;
    std::optional<std::string> productId;
    std::optional<std::string> dataUse;
    std::optional<std::string> label;
};

struct FilepathDownload {
    std::string datasetName;
    std::optional<std::string> productCode;
    std::optional<std::string> dataPath;
    std::optional<std::string> dataUse;
    std::optional<std::string> label;
};

struct FilegroupDownload {
    std::string datasetName;
    std::optional<std::vector<std::string>> fileGroups;
    std::optional<std::string> listId;
    std::optional<std::string> dataUse;
    std::optional<std::string> label;
};

struct Product {
    std::string datasetName;
    std::string entityId;
    std::string productId;
    std::optional<std::string> productCode;
};

/// @brief Struct for custom sorting options
struct SortCustomization {
    /// @brief Field name to sort on
    std::string field_name;
    /// @brief Sort direction, either "ASC" or "DESC"
    std::string direction;
};

/// @brief Structure to filter scene deletion by acquisition date
struct TemporalFilter {
    time_t start;
    time_t end;
};


class USGS_M2M_API {
public:

    /// @brief Constructor
    USGS_M2M_API();

    /// @brief Destructor
    ~USGS_M2M_API();

    /**********************************  Logging in API Functions ***********************************************/
    /// @brief Login as an application guest
    /// @param applicationToken The application token
    /// @param userToken The user token
    /// @return LoginResponse struct containing the response data
    DefaultResponse loginAppGuest(const std::string& applicationToken, const std::string& userToken);

    /// @brief Login using username and token
    /// @param username ERS username
    /// @param token Application token
    /// @return LoginTokenResponse struct
    DefaultResponse loginToken(const std::string& username, const std::string& token, const UserContext& context = {});

    /// @brief Login using Single Sign-On (SSO) cookie
    /// @param context Metadata describing the user (contact ID and IP address)
    /// @return LoginSSOResponse struct
    DefaultResponse loginSSO(const UserContext& context = {});

    /// @brief Logout the current session
    /// @return LogoutResponse struct
    LogoutResponse logout();

    /**********************************  Dataset API Functions ***********************************************/
    /// @brief Get dataset information by either name or ID, one is required
    /// @param datasetName The name of the dataset
    /// @param datasetId The ID of the dataset
    /// @return DatasetResponse struct containing the response data
    DefaultResponse dataset(const std::string& datasetName = "", const std::string& datasetId = "");

    /// @brief Browse available browses for a given dataset ID
    /// @param datasetId The ID of the dataset (required)
    /// @return DatasetBrowseResponse struct containing the response data
    DefaultResponse datasetBrowse(const std::string& datasetId);

    /// @brief Get bulk products for a given dataset name
    /// @param datasetName The name of the dataset (optional)
    /// @return BulkProductsResponse struct containing the response data
    DefaultResponse datasetBulkProducts(const std::string& datasetName = "");

    /// @brief Get available dataset catalogs
    /// @return DatasetCatalogsResponse struct containing the response data
    DefaultResponse datasetCatalogs();

    /// @brief Get dataset categories with optional filters
    /// @param catalog Optional catalog filter
    /// @param includeMessages Optional flag to include messages
    /// @param publicOnly Optional flag to include only public categories
    /// @param useCustomization Optional flag to use customization
    /// @param parentId Optional parent category ID filter
    /// @param datasetFilter Optional dataset filter
    /// @return DatasetCategoriesResponse struct containing the response data
    DefaultResponse datasetCategories(
        const std::optional<std::string>& catalog = std::nullopt,
        const std::optional<bool>& includeMessages = std::nullopt,
        const std::optional<bool>& publicOnly = std::nullopt,
        const std::optional<bool>& useCustomization = std::nullopt,
        const std::optional<std::string>& parentId = std::nullopt,
        const std::optional<std::string>& datasetFilter = std::nullopt
    );

    /// @brief Clear dataset customizations or specific metadata/file groups
    /// @param datasetName Optional dataset name
    /// @param metadataType Optional array of metadata types to clear
    /// @param fileGroupIds Optional array of file group IDs to clear
    /// @return DatasetClearCustomizationResponse struct
    DefaultResponse datasetClearCustomization(
        const std::optional<std::string>& datasetName = std::nullopt,
        const std::vector<std::string>& metadataType = {},
        const std::vector<std::string>& fileGroupIds = {}
    );

    /// @brief Get dataset coverage (bounds, geoJson, etc.) for a given dataset
    /// @param datasetName The dataset name (required)
    /// @return DatasetCoverageResponse struct containing the response data
    DefaultResponse datasetCoverage(const std::string& datasetName);

    /// @brief List all available products for a given dataset
    /// @param datasetName The dataset name (required)
    /// @param sceneFilter Optional JSON object to filter results
    /// @return defaultResponse containing the response data
    DefaultResponse datasetDownloadOptions(
        const std::string& datasetName,
        const std::optional<nlohmann::json>& sceneFilter = std::nullopt
    );

    /// @brief List all configured file groups for a dataset
    /// @param datasetName The dataset name (required)
    /// @return defaultResponse containing the response data
    DefaultResponse datasetFileGroups(const std::string& datasetName);

    /// @brief Get metadata filter fields for a given dataset
    /// @param datasetName The dataset name (required)
    /// @return defaultResponse containing the response data
    DefaultResponse datasetFilters(const std::string& datasetName);

    /// @brief Retrieve metadata customization for a specific dataset
    /// @param datasetName The name of the dataset
    /// @return defaultResponse struct containing the customization data
    DefaultResponse datasetGetCustomization(const std::string& datasetName);


    /// @brief Retrieve metadata customizations for multiple datasets
    /// @param datasetNames Optional list of dataset names to retrieve
    /// @param metadataType Optional list of metadata types to return (export, full, res_sum, shp)
    /// @return defaultResponse struct containing the customizations data
    DefaultResponse datasetGetCustomizations(
        const std::vector<std::string>& datasetNames = {},
        const std::vector<std::string>& metadataType = {}
    );

    /// @brief Retrieve messages/notices for one or more datasets
    /// @param catalog Optional catalog filter
    /// @param datasetName Optional single dataset name
    /// @param datasetNames Optional array of dataset names
    /// @return defaultResponse struct containing the dataset messages
    DefaultResponse datasetMessages(
        const std::optional<std::string>& catalog = std::nullopt,
        const std::optional<std::string>& datasetName = std::nullopt,
        const std::vector<std::string>& datasetNames = {}
    );

    /// @brief Retrieve all metadata fields for a given dataset
    /// @param datasetName The system-friendly dataset name (required)
    /// @return defaultResponse struct containing metadata fields for the dataset
    DefaultResponse datasetMetadata(const std::string& datasetName);


    /// @brief List all available order products for a dataset
    /// @param datasetName The system-friendly dataset name (required)
    /// @return defaultResponse struct containing available order products
    DefaultResponse datasetOrderProducts(const std::string& datasetName);


    /// @brief Search for datasets with optional filters and sorting
    /// @param catalog Optional catalog name
    /// @param categoryId Optional category ID
    /// @param datasetName Optional dataset name (wildcards assumed at beginning and end)
    /// @param includeMessages Optional flag to include messages
    /// @param publicOnly Optional flag to filter public datasets only
    /// @param includeUnknownSpatial Optional flag to include datasets without spatial info
    /// @param temporalFilter Optional temporal filter as JSON object
    /// @param spatialFilter Optional spatial filter as JSON object
    /// @param sortDirection Optional sorting direction (ASC or DESC)
    /// @param sortField Optional sorting field
    /// @param useCustomization Optional flag to use customization
    /// @return defaultResponse struct containing dataset search results
    DefaultResponse datasetSearch(
        const std::optional<std::string>& catalog = std::nullopt,
        const std::optional<std::string>& categoryId = std::nullopt,
        const std::optional<std::string>& datasetName = std::nullopt,
        const std::optional<bool>& includeMessages = std::nullopt,
        const std::optional<bool>& publicOnly = std::nullopt,
        const std::optional<bool>& includeUnknownSpatial = std::nullopt,
        const std::optional<nlohmann::json>& temporalFilter = std::nullopt,
        const std::optional<nlohmann::json>& spatialFilter = std::nullopt,
        const std::optional<std::string>& sortDirection = std::nullopt,
        const std::optional<std::string>& sortField = std::nullopt,
        const std::optional<bool>& useCustomization = std::nullopt
    );

    /// @brief Create or update dataset customization
    /// @param datasetName The dataset to customize
    /// @param excluded Optional flag to exclude the dataset
    /// @param metadata Optional metadata customization JSON
    /// @param searchSort Optional search sort JSON
    /// @param fileGroups Optional file groups JSON
    /// @return defaultResponse struct containing the response
    DefaultResponse datasetSetCustomization(
        const std::string& datasetName,
        const std::optional<bool>& excluded = std::nullopt,
        const std::optional<nlohmann::json>& metadata = std::nullopt,
        const std::optional<nlohmann::json>& searchSort = std::nullopt,
        const std::optional<nlohmann::json>& fileGroups = std::nullopt
    );

    /// @brief Create or update multiple dataset customizations in a single request
    /// @param customizations Vector of DatasetCustomization structs
    /// @return defaultResponse struct containing the response
    DefaultResponse datasetSetCustomizations(const std::vector<DatasetCustomization>& customizations);


    /**********************************  Download API Functions ***********************************************/
    
    /// @brief Marks proxied downloads as complete and updates their downloaded sizes
    /// @param downloads Vector of ProxiedDownload structs containing download IDs and sizes
    /// @return defaultResponse struct containing the response data
    DefaultResponse downloadCompleteProxied(const std::vector<ProxiedDownload>& downloads);

    /// @brief Retrieves the contents of one or more EULAs
    /// @param eulaCode Optional single EULA code
    /// @param eulaCodes Optional vector of multiple EULA codes
    /// @return defaultResponse struct containing the EULA content(s)
    DefaultResponse downloadEula(
        const std::optional<std::string>& eulaCode = std::nullopt,
        const std::vector<std::string>& eulaCodes = {}
    );

    /// @brief Get a list of unique download labels associated with orders
    /// @param downloadApplication Optional application performing the download
    /// @return defaultResponse containing an array of label objects
    DefaultResponse downloadLabels(const std::optional<std::string>& downloadApplication = std::nullopt);

    /// @brief Get download options for a dataset
    /// @param datasetName Dataset alias (required)
    /// @param entityIds Optional list of entity IDs (comma-separated string)
    /// @param listId Optional list ID for customized scene lists
    /// @param includeSecondaryFileGroups Optional flag to include secondary file groups
    /// @return defaultResponse containing download option objects
    DefaultResponse downloadOptions(
        const std::string& datasetName,
        const std::optional<std::string>& entityIds = std::nullopt,
        const std::optional<std::string>& listId = std::nullopt,
        const std::optional<bool>& includeSecondaryFileGroups = std::nullopt
    );

    /// @brief Prepare a download order for processing by moving a single scene into the queue
    /// @param label Optional label to load
    /// @param downloadApplication Optional application performing the download
    /// @return defaultResponse containing download order details
    DefaultResponse downloadOrderLoad(
        const std::optional<std::string>& label = std::nullopt,
        const std::optional<std::string>& downloadApplication = std::nullopt
    );

    /// @brief Remove a download order from the queue
    /// @param label The label of the order to remove (required)
    /// @param downloadApplication Optional application performing the removal
    /// @return defaultResponse containing the number of removed orders
    DefaultResponse downloadOrderRemove(
        const std::string& label,
        const std::optional<std::string>& downloadApplication = std::nullopt
    );

    /// @brief Remove a single download from the queue
    /// @param downloadId The ID of the download to remove (required)
    /// @return defaultResponse containing a boolean indicating success
    DefaultResponse downloadRemove(int downloadId);

    /// @brief Request downloads to be added to the queue and retrieve available URLs
    /// @param configurationCode Optional configuration string for customizing the download routine
    /// @param downloadApplication Optional application name performing the download
    /// @param downloads Optional vector of Download structs for higher-level products
    /// @param dataPaths Optional vector of FilepathDownload structs for specific paths
    /// @param label Optional label to override individual download labels
    /// @param systemId Optional system identifier submitting the download
    /// @param dataGroups Optional vector of FilegroupDownload structs
    /// @return downloadRequestResponse containing available, preparing, failed downloads and metadata
    DefaultResponse downloadRequest(
        const std::optional<std::string>& configurationCode = std::nullopt,
        const std::optional<std::string>& downloadApplication = std::nullopt,
        const std::optional<std::vector<Download>>& downloads = std::nullopt,
        const std::optional<std::vector<FilepathDownload>>& dataPaths = std::nullopt,
        const std::optional<std::string>& label = std::nullopt,
        const std::optional<std::string>& systemId = std::nullopt,
        const std::optional<std::vector<FilegroupDownload>>& dataGroups = std::nullopt
    );


    /// @brief Retrieve all available and previously requested downloads that are not completed
    /// @param label Optional label to filter downloads
    /// @param downloadApplication Optional application performing the retrieval
    /// @return defaultResponse containing available downloads, requested downloads, EULAs, and queue size
    DefaultResponse downloadRetrieve(
        const std::optional<std::string>& label = std::nullopt,
        const std::optional<std::string>& downloadApplication = std::nullopt
    );

    /// @brief Search for downloads within the queue by label or application
    /// @param activeOnly Optional flag to return only active downloads
    /// @param label Optional label to filter downloads
    /// @param downloadApplication Optional application performing the download
    /// @param includeArchived Optional flag to include archived downloads
    /// @return defaultResponse containing the JSON array of matching downloads
    DefaultResponse downloadSearch(
        const std::optional<bool>& activeOnly = std::nullopt,
        const std::optional<std::string>& label = std::nullopt,
        const std::optional<std::string>& downloadApplication = std::nullopt,
        const std::optional<bool>& includeArchived = std::nullopt
    );

    /// @brief Get a summary of all downloads, by dataset, for any matching labels
    /// @param downloadApplication Application performing the download (required)
    /// @param label Label to filter downloads (required)
    /// @param sendEmail Optional flag to send summary via email
    /// @return defaultResponse containing download summary data
    DefaultResponse downloadSummary(
        const std::string& downloadApplication,
        const std::string& label,
        const std::optional<bool>& sendEmail = std::nullopt
    );

    /**********************************  Download API Functions ***********************************************/
    /// @brief Translate between known grids and coordinates
    /// @param gridType The grid system to use (WRS1 or WRS2) (required)
    /// @param responseShape Optional geometry type: "polygon" or "point"
    /// @param path Optional x coordinate in the grid system
    /// @param row Optional y coordinate in the grid system
    /// @return defaultResponse containing coordinates or bounding box
    DefaultResponse grid2ll(
        const std::string& gridType,
        const std::optional<std::string>& responseShape = std::nullopt,
        const std::optional<std::string>& path = std::nullopt,
        const std::optional<std::string>& row = std::nullopt
    );

    /// @brief Get a list of notifications associated with a given system/application
    /// @param systemId System identifier (required)
    /// @return defaultResponse containing notification objects
    DefaultResponse notifications(const std::string& systemId);


    /// @brief Get a list of currently selected products for a dataset (paginated)
    /// @param datasetName Dataset alias (required)
    /// @param entityIds Optional comma-separated list of scene entity IDs
    /// @param listId Optional list ID to identify a customized scene list
    /// @return defaultResponse containing the list of selected products
    DefaultResponse orderProducts(
        const std::string& datasetName,
        const std::optional<std::string>& entityIds = std::nullopt,
        const std::optional<std::string>& listId = std::nullopt
    );

    /// @brief Submits the current product list as a TRAM order - internally calling tram-order-create. 
    /// @param products Used to identify higher level products that this data may be used to create
    /// @param autoBulkOrder If any products can be bulk ordered as a result of completed processing this option allows users to have orders automatically submitted.
    /// @param processingParameters Optional processing parameters to send to the processing system
    /// @param priority Processing Priority
    /// @param orderComment Optional textual identifier for the order
    /// @param systemId Identifies the system submitting the order
    /// @return defaultResponse containing the results of the submitted order.
    DefaultResponse orderSubmit(
        const std::vector<Product>& products,
        const std::optional<bool>& autoBulkOrder = std::nullopt,
        const std::optional<std::string>& processingParameters = std::nullopt,
        const std::optional<int>& priority = std::nullopt,
        const std::optional<std::string>& orderComment = std::nullopt,
        const std::optional<std::string>& systemId = std::nullopt
    );

    /// @brief Returns a list of user permissions for the authenticated user
    /// @return defaultResponse containing an array of permission strings
    DefaultResponse permissions();


    /// @brief Lookup geographic features by name
    /// @param featureType Optional type of feature ("US" or "world")
    /// @param name Optional feature name
    /// @return defaultResponse containing matching geographic features
    DefaultResponse placename(
        const std::optional<std::string>& featureType = std::nullopt,
        const std::optional<std::string>& name = std::nullopt
    );

    /// @brief Retrieve download rate limits and current download counts
    /// @param ipAddress Optional list of IP addresses to check
    /// @return defaultResponse containing initial limits, remaining limits, and recent download counts
    DefaultResponse rateLimitSummary(const std::optional<std::vector<std::string>>& ipAddress = std::nullopt);

    /// @brief Retrieve user's preference settings
    /// @param systemId Optional system ID to filter preferences
    /// @param setting Optional list of specific preference settings to return
    /// @return defaultResponse containing user preferences as JSON
    DefaultResponse userPreferenceGet(
        const std::optional<std::string>& systemId = std::nullopt,
        const std::optional<std::vector<std::string>>& setting = std::nullopt
    );

    /// @brief Create or update user's preferences
    /// @param systemId System ID the preferences belong to (required)
    /// @param userPreferences JSON object representing user preferences (required)
    /// @return defaultResponse indicating success of the operation
    DefaultResponse userPreferenceSet(
        const std::string& systemId,
        const nlohmann::json& userPreferences
    );
    /**********************************  Scene API Functions ***********************************************/
    /// @brief Adds items to a user-defined scene list
    /// @param listId User-defined name for the list (required)
    /// @param datasetName Dataset alias (required)
    /// @param idField Optional ID field type ("entityId" default, or "displayId")
    /// @param entityId Optional single scene identifier
    /// @param entityIds Optional list of scene identifiers
    /// @param timeToLive Optional ISO-8601 duration string specifying list lifetime
    /// @param checkDownloadRestriction Optional flag to check download restricted access
    /// @return defaultResponse containing success status and any failed additions
    DefaultResponse sceneListAdd(
        const std::string& listId,
        const std::string& datasetName,
        const std::optional<std::string>& idField = std::nullopt,
        const std::optional<std::string>& entityId = std::nullopt,
        const std::optional<std::vector<std::string>>& entityIds = std::nullopt,
        const std::optional<std::string>& timeToLive = std::nullopt,
        const std::optional<bool>& checkDownloadRestriction = std::nullopt
    );

    /// @brief Returns items in the given scene list
    /// @param listId User defined name for the list (required)
    /// @param datasetName Optional dataset alias
    /// @param startingNumber Optional starting number to search from
    /// @param maxResults Optional maximum number of results to return
    /// @return defaultResponse containing the list of scenes
    DefaultResponse sceneListGet(
        const std::string& listId,
        const std::optional<std::string>& datasetName = std::nullopt,
        const std::optional<int>& startingNumber = std::nullopt,
        const std::optional<int>& maxResults = std::nullopt
    );

    /// @brief Removes items from a given scene list.  
    /// If no datasetName is provided, the whole list is removed.  
    /// If datasetName is provided but no entityId(s), the dataset is removed with all its IDs.  
    /// If datasetName and entityId(s) are provided, only those IDs are removed.  
    /// @param listId User defined name for the list (required)  
    /// @param datasetName Optional dataset alias  
    /// @param entityId Optional single scene identifier  
    /// @param entityIds Optional vector of scene identifiers  
    /// @return defaultResponse indicating success or failure
    DefaultResponse sceneListRemove(
        const std::string& listId,
        const std::optional<std::string>& datasetName = std::nullopt,
        const std::optional<std::string>& entityId = std::nullopt,
        const std::optional<std::vector<std::string>>& entityIds = std::nullopt
    );

    /// @brief Returns summary information for a given scene list
    /// @param listId User defined name for the list (required)
    /// @param datasetName Optional dataset alias to filter summary
    /// @return defaultResponse containing scene count, spatial bounds, temporal extent, and dataset info
    DefaultResponse sceneListSummary(
        const std::string& listId,
        const std::optional<std::string>& datasetName = std::nullopt
    );

    /// @brief Returns available scene list types (exclude, search, order, bulk, etc.)
    /// @param listFilter Optional string to filter listIds
    /// @return defaultResponse containing array of scene list types and counts
    DefaultResponse sceneListTypes(const std::optional<std::string>& listFilter = std::nullopt);

    /// @brief Retrieve metadata for a specific scene
    /// @param datasetName The dataset alias (required)
    /// @param entityId The scene identifier (required)
    /// @param idType Optional ID type to use: "entityId", "displayId", "orderingId" (default = "entityId")
    /// @param metadataType Optional metadata type: "summary", "full", "fgdc", "iso"
    /// @param includeNullMetadataValues Optional flag to include null metadata values
    /// @param useCustomization Optional flag to apply user customization view
    /// @return defaultResponse containing the scene metadata
    DefaultResponse sceneMetadata(
        const std::string& datasetName,
        const std::string& entityId,
        const std::optional<std::string>& idType = std::nullopt,
        const std::optional<std::string>& metadataType = std::nullopt,
        const std::optional<bool>& includeNullMetadataValues = std::nullopt,
        const std::optional<bool>& useCustomization = std::nullopt
    );

    /// @brief Retrieve metadata for a pre-defined list of scenes
    /// @param listId The scene list identifier (required)
    /// @param datasetName Optional dataset alias
    /// @param metadataType Optional metadata type: "summary" or "full"
    /// @param includeNullMetadataValues Optional flag to include null metadata values
    /// @param useCustomization Optional flag to display metadata as per user customization
    /// @return defaultResponse containing metadata for the scenes in the list
    DefaultResponse sceneMetadataList(
        const std::string& listId,
        const std::optional<std::string>& datasetName = std::nullopt,
        const std::optional<std::string>& metadataType = std::nullopt,
        const std::optional<bool>& includeNullMetadataValues = std::nullopt,
        const std::optional<bool>& useCustomization = std::nullopt
    );


    /// @brief Retrieve XML-formatted metadata for a given scene
    /// @param datasetName Dataset alias (required)
    /// @param entityId Scene identifier (required)
    /// @param metadataType Optional metadata type: "full", "fgdc", or "iso"
    /// @return defaultResponse containing XML metadata for the scene
    DefaultResponse sceneMetadataXml(
        const std::string& datasetName,
        const std::string& entityId,
        const std::optional<std::string>& metadataType = std::nullopt
    );

    /// @brief Search for scenes in a dataset using limited search criteria, bounding boxes, date ranges, and optional filters.
    /// @param datasetName Dataset alias to search (required)
    /// @param maxResults Maximum number of results to return (optional, default = 100)
    /// @param startingNumber Starting index for search results (optional)
    /// @param metadataType Metadata type to return ("summary" or "full") (optional)
    /// @param sortField Field to sort results by (optional)
    /// @param sortDirection Sort direction: "ASC" or "DESC" (optional)
    /// @param sortCustomization Optional custom sort parameters
    /// @param useCustomization Whether to use user customizations (optional)
    /// @param sceneFilter Optional JSON object to filter data
    /// @param compareListName Optional scene-list listId to track comparison scenes
    /// @param bulkListName Optional scene-list listId to track bulk order scenes
    /// @param orderListName Optional scene-list listId to track on-demand order scenes
    /// @param excludeListName Optional scene-list listId to exclude scenes from results
    /// @param includeNullMetadataValues Whether to include null metadata values (optional)
    /// @return defaultResponse containing the search results
    DefaultResponse sceneSearch(
        const std::string& datasetName,
        const std::optional<int>& maxResults = std::nullopt,
        const std::optional<int>& startingNumber = std::nullopt,
        const std::optional<std::string>& metadataType = std::nullopt,
        const std::optional<std::string>& sortField = std::nullopt,
        const std::optional<std::string>& sortDirection = std::nullopt,
        const std::optional<SortCustomization>& sortCustomization = std::nullopt,
        const std::optional<bool>& useCustomization = std::nullopt,
        const std::optional<nlohmann::json>& sceneFilter = std::nullopt,
        const std::optional<std::string>& compareListName = std::nullopt,
        const std::optional<std::string>& bulkListName = std::nullopt,
        const std::optional<std::string>& orderListName = std::nullopt,
        const std::optional<std::string>& excludeListName = std::nullopt,
        const std::optional<bool>& includeNullMetadataValues = std::nullopt
    );

    /// @brief Search for deleted scenes in a dataset
    /// @param datasetName Dataset to search
    /// @param maxResults Maximum results to return (optional)
    /// @param startingNumber Starting index for pagination (optional)
    /// @param sortField Field to sort by (optional)
    /// @param sortDirection Sort order: ASC or DESC (optional)
    /// @param temporalFilter Filter by acquisition date (optional)
    /// @return DefaultResponse with deletion information
    DefaultResponse sceneSearchDelete(
        const std::string& datasetName,
        const std::optional<int>& maxResults = std::nullopt,
        const std::optional<int>& startingNumber = std::nullopt,
        const std::optional<std::string>& sortField = std::nullopt,
        const std::optional<std::string>& sortDirection = std::nullopt,
        const std::optional<TemporalFilter>& temporalFilter = std::nullopt
    );

    /// @brief Searches for related scenes for a given scene.
    /// @param entityId Scene ID to find related scenes for
    /// @param datasetName Dataset to search
    /// @param maxResults Optional: number of results to return
    /// @param startingNumber Optional: starting number for pagination
    /// @param metadataType Optional: metadata type to return (summary or full)
    /// @param sortField Optional: field to sort results on
    /// @param sortDirection Optional: direction to sort results (ASC or DESC)
    /// @param compareListName Optional: scene-list listId for comparison
    /// @param bulkListName Optional: scene-list listId for bulk ordering
    /// @param orderListName Optional: scene-list listId for on-demand ordering
    /// @param excludeListName Optional: scene-list listId for excluding scenes
    /// @return DefaultResponse containing the search results
    DefaultResponse sceneSearchSecondary(
        const std::string& entityId,
        const std::string& datasetName,
        const std::optional<int>& maxResults = std::nullopt,
        const std::optional<int>& startingNumber = std::nullopt,
        const std::optional<std::string>& metadataType = std::nullopt,
        const std::optional<std::string>& sortField = std::nullopt,
        const std::optional<std::string>& sortDirection = std::nullopt,
        const std::optional<std::string>& compareListName = std::nullopt,
        const std::optional<std::string>& bulkListName = std::nullopt,
        const std::optional<std::string>& orderListName = std::nullopt,
        const std::optional<std::string>& excludeListName = std::nullopt
    );

    /**********************************  Tram API Functions ***********************************************/
    /// @brief Update a specific metadata detail for an order
    /// @param orderNumber Order ID to update (required)
    /// @param detailKey Metadata key to set (required)
    /// @param detailValue Value to assign to the metadata key (required)
    /// @return DefaultResponse containing the updated key/value or error info
    DefaultResponse tramOrderDetailUpdate(
        const std::string& orderNumber,
        const std::string& detailKey,
        const std::string& detailValue
    );

    /// @brief Retrieves metadata details for a given TRAM order
    /// @param orderNumber The order ID to retrieve details for (required)
    /// @return DefaultResponse containing the order metadata
    DefaultResponse tramOrderDetails(
        const std::string& orderNumber
    );

    /// @brief Clears all metadata for a given TRAM order
    /// @param orderNumber The order ID to clear details for (required)
    /// @return DefaultResponse indicating success or failure
    DefaultResponse tramOrderDetailsClear(
        const std::string& orderNumber
    );

    /// @brief Removes a specific metadata key from a given TRAM order
    /// @param orderNumber The order ID to remove the detail from (required)
    /// @param detailKey The key to remove (required)
    /// @return DefaultResponse containing the remaining metadata
    DefaultResponse tramOrderDetailsRemove(
        const std::string& orderNumber,
        const std::string& detailKey
    );

    /// @brief Searches TRAM orders based on optional filters
    /// @param orderId Optional order ID (supports '%' wildcard)
    /// @param maxResults Optional maximum number of results per page
    /// @param systemId Optional system/application ID that submitted the order
    /// @param sortAsc Optional: true for ascending, false for descending (default = descending)
    /// @param sortField Optional: field to sort by ("order_id", "date_entered", "date_updated")
    /// @param statusFilter Optional vector of status codes to filter orders
    /// @return DefaultResponse containing the search results
    DefaultResponse tramOrderSearch(
        const std::optional<std::string>& orderId = std::nullopt,
        const std::optional<int>& maxResults = std::nullopt,
        const std::optional<std::string>& systemId = std::nullopt,
        const std::optional<bool>& sortAsc = std::nullopt,
        const std::optional<std::string>& sortField = std::nullopt,
        const std::optional<std::vector<std::string>>& statusFilter = std::nullopt
    );

    /// @brief Retrieves the status of a specific TRAM order
    /// @param orderNumber The order ID to get status for (required)
    /// @return DefaultResponse containing the order status and unit information
    DefaultResponse tramOrderStatus(const std::string& orderNumber);

    /// @brief Lists all units for a specific TRAM order
    /// @param orderNumber The order ID to get units for (required)
    /// @return DefaultResponse containing unit details for the order
    DefaultResponse tramOrderUnits(const std::string& orderNumber);

    /**********************************  HTTP Header Updating functions ***********************************************/
    /// @brief Set the X-Auth-Token header
    /// @param token The authentication token to be used in the request
    void setAuthToken(const std::string& token);

    /// @brief Updates the header list with a new header or modifies an existing one
    /// @param header 
    void updateHeader(const std::string& header);

private:
    /// @brief CURL handle
    CURL *curl = nullptr;
    /// @brief CURL headers
    struct curl_slist *headers = nullptr;

    /// @brief headers vector for all headers
    std::vector<std::string> headersVector;

    /// @brief Callback function for CURL write
    /// @param contents Pointer to the data
    /// @param size Size of each data element
    /// @param nmemb Number of data elements
    /// @param userp Pointer to user data
    /// @return Number of bytes processed
    static size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp);

    /// @brief Perform a JSON POST request
    /// @param url The URL to send the request to
    /// @param jsonPayload The JSON payload to send
    /// @param responseBody The response body (output)
    /// @param httpCodeOut The HTTP response code (output)
    /// @return true if the request was successful, false otherwise
    bool performJsonPostRequest(const std::string& url, const std::string& jsonPayload, std::string& responseBody, long& httpCodeOut);

    /// @brief Perform a JSON GET request
    /// @param url The URL to send the request to
    /// @param responseBody The response body (output)
    /// @param httpCodeOut The HTTP response code (output)
    /// @return true if the request was successful, false otherwise
    bool performJsonGetRequest(const std::string& url, std::string& responseBody, long& httpCodeOut);

    /// @brief Setup CURL
    void setup_curl();

    /// @brief Cleanup CURLs
    void cleanup_curl();

    /// @brief Check for JSON error in response
    /// @param success Whether the request was successful
    /// @param httpCode The HTTP response code
    /// @return true if the request was successful, false otherwise
    bool httpRequestSuccessful(long httpCode, bool& success, ErrorResponse& errorData);

    /// @brief Parse JSON response for errors
    /// @param responseBody The response body
    /// @param errorData The ErrorResponse struct to populate
    /// @param success Whether the request was successful
    /// @return true if the there were no errors, false otherwise
    bool jsonErrorParsing(nlohmann::json& jsonResponse, ErrorResponse& errorData, bool& success);

    /// @brief Parse JSON response for errors
    /// @param responseBody The response body
    /// @param errorData The ErrorResponse struct to populate
    /// @param success Whether the request was successful
    /// @return true if the there were no errors, false otherwise
    void jsonMetaDataParsing(nlohmann::json& jsonResponse, MetaDataResponse& metaData);

    /// @brief Common JSON response parsing for defaultResponse types
    /// @param responseBody The response body
    /// @param jsonPayload The JSON payload to send (optional, for POST requests)
    /// @return struct representing the response.
    DefaultResponse defaultJsonResponseParsing(const std::string& url, nlohmann::json& jsonResponse, const std::string& jsonPayload = "");

    /// @brief Safely get an optional integer from a JSON object
    /// @param j JSON object
    /// @param key Key to extract
    /// @return Optional integer, std::nullopt if key missing or not an int
    std::optional<int> safeGetIntOpt(const nlohmann::json& j, const std::string& key) const;

    /// @brief Safely get an optional string from a JSON object
    /// @param j JSON object
    /// @param key Key to extract
    /// @return Optional string, std::nullopt if key missing or not a string
    std::optional<std::string> safeGetStringOpt(const nlohmann::json& j, const std::string& key) const;

    /// @brief Convert DatasetCustomization to JSON
    /// @param dc The DatasetCustomization struct to convert
    /// @return JSON representation of the DatasetCustomization 
    nlohmann::json datasetCustomizationToJson(const DatasetCustomization& dc);

    /// @brief Converts a time_t to an ISO 8601 formatted string in UTC.
    /// @param t Time to convert
    /// @return ISO 8601 string (e.g., "2025-09-21T23:00:00Z")
    std::string timeToISO8601UTC(time_t t);
};

#endif //USGSM2M_HPP