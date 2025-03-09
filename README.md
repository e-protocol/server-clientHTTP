# Example of a simple server and client http post request

## Description
This solution was made for demonstration. It's based on task requirements below:

##Task*
1) Create light http server
2) Server should response "Status ok" on /info request
3) Server should write logs
4) Server should send logs on /log request
5) Server should upload file somewhere in "/temp" directory by /upload request
6) Server should be able uploading by wget or curl**

##Note!
* No additional information was provided. So this solution is abstract and not applicable for real purposes.
Moreover, due time limitation and various ways to process http requests, the last point was skipped. See below:
** No wget and curl support. But! Custom client was created to demonstrate that server is able to take on files in next algorithm:
- Client send file via single http POST request
- File is transfered as string in req.body() with additional information about FileName.size() and FileName.extension decoded in front
- First byte is a size of fileName + .extension
- Next bytes are the file name
- Server just read this information and remove it from file before saving
*** No multi-part request write support. No file size check!

##Build Requirements
- Boost 1.83 (Both)
- GCC 14.2 (Server)
- VS Code with cmake and c++ extensions (Server)
- Visual Studio 2022 (client)

##Instructions
