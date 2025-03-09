# Example of a simple server and client http post request

## Description
This example was made for demonstration. It's based on task requirements below:

##Task*
1) Create light http server
2) Server should response "Status ok" on /info request
3) Server should write logs
4) Server should send logs on /log request
5) Server should upload file somewhere in "/temp" directory by /upload request
6) Server should be able uploading by wget or curl**

##Note!
*
** This wasn't achieved directly. Server is able to take on files in next algorithm:
- Client send file via single http POST request
- File is transfered as string in req.body() with additional information about FileName.size() and FileName.extension decoded in front
- First byte is a size of fileName + .extension
- Next bytes are the file name

##Requirements


##Instructions
