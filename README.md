# USGS-M2M-CPP-API

Library for USGS M2M API for C++

This is a wrapper for the USGS M2M API here: [m2m.cr.usgs.gov/api/docs/reference/](m2m.cr.usgs.gov/api/docs/reference/)

Please see the basic usages here:
[https://m2m.cr.usgs.gov/api/docs/json/#section-basicUsage](https://m2m.cr.usgs.gov/api/docs/json/#section-basicUsage)

Please note that users will need to login before sending any commands and set the Auth token for the http header using the setAuthToken function.

It is recommended to logout when the session is finished.

## Output

- The build will create a shared library - usgsm2mcpp.so
- The hpp files usgsM2M.hpp and usgsM2MDataTypes.hpp will also be needed to use the library's functions.

## Dependencies

```c++
sudo apt-get install -y nlohmann-json3-dev libcurl4-openssl-dev
```
