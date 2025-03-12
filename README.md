# Example of a simple server and client http post request

## Description
This solution was made for demonstration. It's based on task requirements below:

## Task*
1) Create light http server to listen any specified port
2) Server should operate POST http requests
3) Server should response "Status ok" on /info request
4) Server should write logs
5) Server should send logs on /log request
6) Server should upload file somewhere in "/tmp" directory by /upload request
7) Server should start only under root for Linux target machine
8) Server should be able uploading files by wget or curl**

## Note!
*No additional information was provided. So this solution is abstract and not applicable for real purposes.
Moreover, due time limitation and various ways to process http requests, the last point was skipped. See below:

**Server is able to take on files if they include in http request content-field: "Content-Disposition" with value: "filename=<uploading_filename>"

**No multi-part request write support! No file size check! Server low lvl security!

## Build Requirements
- Boost 1.83 (Both)
- GCC 14.2 (Server)
- VS Code with cmake and c++ extensions (Server)
- Visual Studio 2022 (client)

## Usage
Server Linux:
- location of executable: server-clientHTTP/serverHTTP/build/serverHTTP/src/serverHTTP 
- start under root with command: ./serverHTTP {port}
- {port} is optional, if not provided = default port 8080
- ifconfig your server machine ip

Client Windows 10:
- location of executable: server-clientHTTP/httpClient/out/build/x64-release/httpClient/httpClient.exe
- start app
- type command: ip {server ip} {port}

## P.S.
- type "exit" in both apps to close them
- all commands have space as delimiter

## P.P.S.
- added armhf build
