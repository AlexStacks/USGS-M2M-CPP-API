/// @author Alexander Stackpoole
/// @date 9/20/25
/// @brief USGS M2M API C++ Header


#ifndef USGSM2M_HPP
#define USGSM2M_HPP

#include <curl/curl.h>
#include <nlohmann/json.hpp>

#include "usgsm2m_login.hpp"
#include "usgsm2m_dataset.hpp"

static const std::string API_URL =  "https://m2m.cr.usgs.gov/api/api/json/stable/";

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
    LoginAppGuestResponse loginAppGuest(const std::string& applicationToken, const std::string& userToken);

    /// @brief Login using username and token
    /// @param username ERS username
    /// @param token Application token
    /// @return LoginTokenResponse struct
    LoginTokenResponse loginToken(const std::string& username, const std::string& token, const UserContext& context = {});

    /// @brief Login using Single Sign-On (SSO) cookie
    /// @param context Metadata describing the user (contact ID and IP address)
    /// @return LoginSSOResponse struct
    LoginSSOResponse loginSSO(const UserContext& context = {});

    /// @brief Logout the current session
    /// @return LogoutResponse struct
    LogoutResponse logout();

    /**********************************  Dataset API Functions ***********************************************/
    /// @brief Get dataset information by either name or ID, one is required
    /// @param datasetName The name of the dataset
    /// @param datasetId The ID of the dataset
    /// @return DatasetResponse struct containing the response data
    DatasetResponse dataset(const std::string& datasetName = "", const std::string& datasetId = "");

    /// @brief Browse available browses for a given dataset ID
    /// @param datasetId The ID of the dataset (required)
    /// @return DatasetBrowseResponse struct containing the response data
    DatasetBrowseResponse datasetBrowse(const std::string& datasetId);

    /// @brief Get bulk products for a given dataset name
    /// @param datasetName The name of the dataset (optional)
    /// @return BulkProductsResponse struct containing the response data
    BulkProductsResponse datasetBulkProducts(const std::string& datasetName = "");

    /// @brief Get available dataset catalogs
    /// @return DatasetCatalogsResponse struct containing the response data
    DatasetCatalogsResponse datasetCatalogs();


    /// @brief Get dataset categories with optional filters
    /// @param catalog Optional catalog filter
    /// @param includeMessages Optional flag to include messages
    /// @param publicOnly Optional flag to include only public categories
    /// @param useCustomization Optional flag to use customization
    /// @param parentId Optional parent category ID filter
    /// @param datasetFilter Optional dataset filter
    /// @return DatasetCategoriesResponse struct containing the response data
    DatasetCategoriesResponse datasetCategories(
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
    DatasetClearCustomizationResponse datasetClearCustomization(
        const std::optional<std::string>& datasetName = std::nullopt,
        const std::vector<std::string>& metadataType = {},
        const std::vector<std::string>& fileGroupIds = {}
    );

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

    /// @brief Safely get an optional string from a JSON object
    /// @param j JSON object
    /// @param key Key to extract
    /// @return Optional string, std::nullopt if key missing or not a string
    std::optional<std::string> safeGetStringOpt(const nlohmann::json& j, const std::string& key) const;

    /// @brief Safely get a string from a JSON object, returns empty string if key missing or not a string
    /// @param j JSON object
    /// @param key Key to extract
    /// @return String value or empty string
    std::string safeGetString(const nlohmann::json& j, const std::string& key) const;

    /// @brief Parse a JSON array of strings safely
    /// @param j The JSON object
    /// @param key The key to extract the array from 
    /// @return Vector of strings, empty if key missing or not an array of strings  
    std::vector<std::string> USGS_M2M_API::parseStringArray(const nlohmann::json& j, const std::string& key) const;

    /// @brief Safely get a boolean from a JSON object, returns false if key missing or not a bool
    /// @param j JSON object
    /// @param key Key to extract
    /// @return Boolean value or false
    bool safeGetBool(const nlohmann::json& j, const std::string& key) const;

    /// @brief Safely get an optional boolean from a JSON object
    /// @param j JSON object
    /// @param key Key to extract
    /// @return Optional boolean, std::nullopt if key missing or not a bool
    std::optional<bool> safeGetBoolOpt(const nlohmann::json& j, const std::string& key) const;

    /// @brief Safely get an integer from a JSON object, returns 0 if key missing or not an int
    /// @param j JSON object
    /// @param key Key to extract
    /// @return Integer value or 0
    int safeGetInt(const nlohmann::json& j, const std::string& key) const;
    
    /// @brief Safely get an optional integer from a JSON object
    /// @param j JSON object
    /// @param key Key to extract
    /// @return Optional integer, std::nullopt if key missing or not an int
    std::optional<int> safeGetIntOpt(const nlohmann::json& j, const std::string& key) const;

    /// @brief Safely parse spatial bounds from a JSON object
    /// @param j The JSON object
    /// @return Optional SpatialBounds struct, std::nullopt if parsing fails
    std::optional<SpatialBounds> parseSpatialBounds(const nlohmann::json& j) const;

    /// @brief Recursively parse category and its subcategories
    /// @param categoryJson The JSON object representing the category
    /// @return DatasetCategoryInfo struct representing the category and its subcategories
    DatasetCategoryInfo USGS_M2M_API::parseCategoryRecursive(const nlohmann::json& categoryJson);
};

#endif //USGSM2M_HPP