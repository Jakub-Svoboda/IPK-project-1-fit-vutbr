**NAME**
>**ftrest** - connect to server and execute the command

**SYNOPSIS**
>**ftrest** COMMAND REMOTE-PATH [LOCAL-PATH]

**DESCRIPTION**
>**frest** is a client application for executing commands on server running with ftrestd. Substitute the COMMAND with one of these keywords to perform a simple operation on the server side.

**OPTIONS**

>the COMMAND parameter can be substituted with one of these keywords:

>**del** - Deletes a file on the server side specified by the remote-path. If no file is found, 404 return code is sent back in the HTTP header. When the path points to a file, user receives a "Not a file." message in the response.

>**get** - Copies a file from the remote server to the clients side. The file is saved to the current folder by default. When the local-path parametetr is specified, the file is save to the given location, if possible.  

>**put** - Transfers a file from the local-path parameter (mandatory for this command) to the servers remote directory. When a file with the same name already exists on the server, the file will not be copied and client receives an error message instead.

>**lst** - Lists the contents of remote directory specified by the remote-path. On output the file names are separated with a single white space, similar to the unix ls program.

>**mkd** - Makes a new directory on the server in location specified by the remote-path parameter. New user folder cannot be created. When the specified folder already exists, client receives an error message.

>**rmd** - Removes a folder from a server. The folder cannot be removed if it contains files, when the specified path is incorrect or when the specified folder is a user's home folder. In these cases a proper error will be returned in the HTTP response. This operation also cannot be used on files, use the del command instead.
 
>

 
**NAME**
>**ftrestd** - the server application the executes commands called by the client's ftrest program.
 
**SYNOPSIS**
>**ftrest** [-p port] [-r root]
 
**DESCRIPTION**
>**frestd** receives a command specified in a HTTP header and, if possible, executes the given command. Ftrestd also sends back a response, containing a HTTP header with a proper return code and in case of the "get" command also a file, that is to be stored on the client's side.

**OPTIONS**

>**-p** Specifies a port that the server will listen to. If no value is given, the value of 6677 is assigned by default. The client must specify the same port in it's query.   

>**-r** Sets the root folder for the commands to be executed. When no folder is specified, the current folder will be selected as the root folder. When a different folder that the current one is given, ftrestd acts as if the commands were executed from the new root folder.