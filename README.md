# Example of a simple server and client http post request

## Description
This solution was made for demonstration. It's based on task requirements below:

## Task*
1) Create light http server to listen any specified port
2) Server should response "Status ok" on /info request
3) Server should write logs
4) Server should send logs on /log request
5) Server should upload file somewhere in "/tmp" directory by /upload request
6) Server should start only under root for Linux target machine
7) Server should be able uploading files by wget or curl**

## Note!
*No additional information was provided. So this solution is abstract and not applicable for real purposes.
Moreover, due time limitation and various ways to process http requests, the last point was skipped. See below:

**No wget and curl support. But! Custom client was created on Windows 10 to demonstrate that server is able to take on files in next algorithm:
- Client send file via single http POST request
- File is transfered as string in req.body() with additional information:
   FileName.size() and FileName.extension decoded in front where:
- First byte is a size of fileName + .extension
- Next bytes are the file name
- Server just read this information and remove it from file before saving
***No multi-part request write support. No file size check!

## Build Requirements
- Boost 1.83 (Both)
- GCC 14.2 (Server)
- VS Code with cmake and c++ extensions (Server)
- Visual Studio 2022 (client)

## Usage
Server Linux:
- location of executable: server-clientHTTP/serverHTTP/build/serverHTTP/src/serverHTTP 
- start under root with example command ./serverHTTP {port}
- ifconfig your server machine ip
Client Windows 10:
- location of executable: server-clientHTTP/httpClient/out/build/x64-release/httpClient/httpClient.exe
- start app
- type command: ip {server ip} {port}

## P.S.
- type exit in both apss to close them
- all command have space as delimiter
