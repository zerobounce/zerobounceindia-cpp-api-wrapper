{\rtf1\ansi\ansicpg1252\cocoartf2709
\cocoatextscaling0\cocoaplatform0{\fonttbl\f0\fswiss\fcharset0 Helvetica;}
{\colortbl;\red255\green255\blue255;}
{\*\expandedcolortbl;;}
\paperw11900\paperh16840\margl1440\margr1440\vieww11520\viewh8400\viewkind0
\pard\tx566\tx1133\tx1700\tx2267\tx2834\tx3401\tx3968\tx4535\tx5102\tx5669\tx6236\tx6803\pardirnatural\partightenfactor0

\f0\fs24 \cf0 ## Envoltorio de la API de Zero Bounce en C++\
Este envoltorio de la API contiene m\'e9todos para interactuar f\'e1cilmente con la API de ZeroBounce.\
Puedes encontrar m\'e1s informaci\'f3n sobre ZeroBounce en la [documentaci\'f3n oficial](https://www.zerobounce.net/docs/).\
\
## INSTALACI\'d3N\
Antes de instalar el envoltorio, aseg\'farate de tener instalado `CMake` en tu sistema. Puedes descargarlo desde [aqu\'ed](https://cmake.org/download/).\
\
Puedes instalarlo utilizando CMake con `FetchContent`. Agrega lo siguiente a tu archivo `CMakeLists.txt`:\
```cmake\
include(FetchContent)\
FetchContent_Declare(\
    ZeroBounce\
    GIT_REPOSITORY https://github.com/zerobounce/zerobounce-cpp-api-wrapper.git\
    GIT_TAG <DESIRED_TAG>\
)\
FetchContent_MakeAvailable(ZeroBounce)\
```\
\
Esto generar\'e1 el objetivo `ZeroBounce` al que puedes enlazar de la manera habitual:\
\
```cmake\
target_link_libraries(nombre_de_tu_objetivo PRIVATE ZeroBounce)\
```\
\
Si utilizas Visual Studio Code, puedes construir la biblioteca utilizando las extensiones `CMake` y `CMake Tools`. Despu\'e9s de instalar las extensiones, simplemente abre los comandos con `CTRL + SHIFT + P` y utiliza `CMake: Configure`, seguido de `CMake: Build`.\
\
Alternativamente, tambi\'e9n puedes utilizar los comandos de la l\'ednea de comandos en el directorio ra\'edz de tu proyecto.\
\
#### LINUX\
\
```bash\
mkdir build\
cmake -S . -B build -G "Unix Makefiles"\
cmake --build build\
```\
\
#### WINDOWS\
\
Aseg\'farate de tener instalado `MinGW` en tu sistema. Puede ser instalado siguiendo los pasos [aqu\'ed](https://code.visualstudio.com/docs/cpp/config-mingw).\
\
```bash\
mkdir build\
cmake -S . -B build -G "MinGW Makefiles"\
cmake --build build\
```\
\
Ten en cuenta que es posible que la biblioteca requiera algunos archivos .dll (que se encuentran en el directorio `build`) para funcionar correctamente en tu proyecto.\
\
## USO\
Incluye la biblioteca en tu archivo:\
```cpp\
#include <ZeroBounce/ZeroBounce.h>\
``` \
\
Inicializa el envoltorio con tu clave de API:\
```cpp\
ZeroBounce::getInstance()->initialize("<TU_CLAVE_DE_API>");\
```\
\
## Ejemplos\
Luego puedes utilizar cualquiera de los m\'e9todos del envoltorio, por ejemplo:\
* ##### Comprobar cu\'e1ntos cr\'e9ditos te quedan en tu cuenta\
```cpp\
ZeroBounce::getInstance()->getCredits(\
    [](ZBCreditsResponse response) \{\
        cout << response.toString() << endl;\
    \},\
    [](ZBErrorResponse errorResponse) \{\
        cout << errorResponse.toString() << endl;\
    \}\
);\
```\
\
* ##### Comprobar el uso de tu API durante un per\'edodo de tiempo determinado\
```cpp\
std::tm startDate = \{0\};\
startDate.tm_year = 118;\
startDate.tm_mon = 0;\
startDate.tm_mday = 1;\
\
std::tm endDate = \{0\};\
endDate.tm_year = 123;\
endDate.tm_mon = 11;\
endDate.tm_mday = 12;\
\
ZeroBounce::getInstance()->getApiUsage(\
    startDate,\
    endDate,\
    [](ZBGetApiUsageResponse response) \{\
        cout << response.toString() << endl;\
    \},\
\
\
    [](ZBErrorResponse errorResponse) \{\
        cout << errorResponse.toString() << endl;\
    \}\
);\
```\
\
* ##### Validar una direcci\'f3n de correo electr\'f3nico\
```cpp\
std::string email = "valido@ejemplo.com";   // La direcci\'f3n de correo electr\'f3nico que deseas validar\
std::string ipAddress = "127.0.0.1";     // La direcci\'f3n IP desde la que se registr\'f3 el correo electr\'f3nico (opcional)\
\
ZeroBounce::getInstance()->validate(\
    email,\
    ipAddress,\
    [](ZBValidateResponse response) \{\
        cout << response.toString() << endl;\
    \},\
    [](ZBErrorResponse errorResponse) \{\
        cout << errorResponse.toString() << endl;\
    \}\
);\
```\
\
* ##### Validar un lote de correos electr\'f3nicos\
```cpp\
std::vector<ZBEmailToValidate> emails = \{\
    \{"valido@ejemplo.com", "1.1.1.1"\},\
    \{"invalido@ejemplo.com"\}\
\};\
\
ZeroBounce::getInstance()->validateBatch(\
    emails,\
    [](ZBValidateBatchResponse response) \{\
        cout << response.toString() << endl;\
    \},\
    [](ZBErrorResponse errorResponse) \{\
        cout << errorResponse.toString() << endl;\
    \}\
);\
```\
\
* ##### Utilizar el punto final de la API de actividad para obtener informaci\'f3n sobre la participaci\'f3n general de los suscriptores en los correos electr\'f3nicos\
```cpp\
std::string email = "valido@ejemplo.com";    // Direcci\'f3n de correo electr\'f3nico del suscriptor\
\
ZeroBounce::getInstance()->getActivityData(\
    email,\
    [](ZBActivityDataResponse response) \{\
        cout << response.toString() << endl;\
    \},\
    [](ZBErrorResponse errorResponse) \{\
        cout << errorResponse.toString() << endl;\
    \}\
);\
```\
\
* ##### La API sendfile permite al usuario enviar un archivo para la validaci\'f3n masiva de correos electr\'f3nicos\
```cpp\
std::string filePath = "<RUTA_DEL_ARCHIVO>"; 	// Ruta del archivo csv o txt\
int emailAddressColumn = 3; 			// El \'edndice de la columna "email" en el archivo. El \'edndice comienza en 1\
SendFileOptions options;			// Opciones adicionales\
\
options.returnUrl = "https://dominio.com/llamado/despu\'e9s/de/procesar/solicitud";\
options.firstNameColumn = 4;		// El \'edndice de la columna "nombre" en el archivo\
options.lastNameColumn = 5;		// El \'edndice de la columna "apellido" en el archivo\
options.genderColumn = 6;		// El \'edndice de la columna "g\'e9nero" en el archivo\
options.ipAddressColumn = 7;		// El \'edndice de la columna "direcci\'f3n IP" en el archivo\
options.hasHeaderRow = true;		// Si esto es `true`, la primera fila se considera como encabezados de tabla\
options.removeDuplicate = true;	// Si esto es `true`, se eliminar\'e1n los correos electr\'f3nicos duplicados\
\
ZeroBounce::getInstance()->sendFile(\
    filePath,\
    emailAddressColumn,\
    options,\
    [](ZBSendFileResponse response) \{\
        cout << response.toString() << endl;\
    \},\
    [](ZBErrorResponse errorResponse) \{\
        cout << errorResponse.toString() << endl;\
    \}\
);\
```\
\
* ##### La API getfile permite a los usuarios obtener el archivo de resultados de validaci\'f3n para el archivo enviado utilizando la API sendfile\
```cpp\
std::string fileId = "<ID_DE\
\
_ARCHIVO>";				// El ID de archivo devuelto al llamar a la API sendfile\
std::string localDownloadPath = "<RUTA_DE_DESCARGA_DEL_ARCHIVO>"; 	// La ubicaci\'f3n donde se guardar\'e1 el archivo descargado\
\
ZeroBounce::getInstance()->getFile(\
    fileId,\
    localDownloadPath,\
    [](ZBGetFileResponse response) \{\
        cout << response.toString() << endl;\
    \},\
    [](ZBErrorResponse errorResponse) \{\
        cout << errorResponse.toString() << endl;\
    \}\
);\
```\
\
* ##### Comprobar el estado de un archivo cargado a trav\'e9s del m\'e9todo "sendFile"\
```cpp\
std::string fileId = "<ID_DE_ARCHIVO>";		// El ID de archivo devuelto al llamar a la API sendfile\
\
ZeroBounce::getInstance()->fileStatus(\
    fileId,\
    [](ZBFileStatusResponse response) \{\
        cout << response.toString() << endl;\
    \},\
    [](ZBErrorResponse errorResponse) \{\
        cout << errorResponse.toString() << endl;\
    \}\
);\
```\
\
* ##### Eliminar el archivo que se envi\'f3 utilizando la API sendfile. El archivo solo se puede eliminar si su estado es _`Completo`_\
```cpp\
std::string fileId = "<ID_DE_ARCHIVO>";		// El ID de archivo devuelto al llamar a la API sendfile\
\
ZeroBounce::getInstance()->deleteFile(\
    fileId,\
    [](ZBDeleteFileResponse response) \{\
        cout << response.toString() << endl;\
    \},\
    [](ZBErrorResponse errorResponse) \{\
        cout << errorResponse.toString() << endl;\
    \}\
);\
```\
\
* ##### La API scoringSendfile permite al usuario enviar un archivo para la validaci\'f3n masiva de correos electr\'f3nicos\
```cpp\
std::string filePath = "<RUTA_DEL_ARCHIVO>";		// Ruta del archivo csv o txt\
int emailAddressColumn = 3; 			// El \'edndice de la columna "email" en el archivo. El \'edndice comienza en 1\
SendFileOptions options;			// Opciones adicionales\
\
options.returnUrl = "https://dominio.com/llamado/despu\'e9s/de/procesar/solicitud";\
options.hasHeaderRow = true;            // Si esto es `true`, la primera fila se considera como encabezados de tabla\
\
\
ZeroBounce::getInstance()->scoringSendFile(\
    filePath,\
    emailAddressColumn,\
    options,\
    [](ZBSendFileResponse response) \{\
        cout << response.toString() << endl;\
        scoringFileId = response.fileId;\
    \},\
    [](ZBErrorResponse errorResponse) \{\
        cout << errorResponse.toString() << endl;\
    \}\
);\
```\
\
* ##### La API scoringGetFile permite a los usuarios obtener el archivo de resultados de validaci\'f3n para el archivo enviado utilizando la API scoringSendfile\
```cpp\
std::string fileId = "<ID_DE_ARCHIVO>";                       	// El ID de archivo devuelto al llamar a la API scoringSendfile\
std::string localDownloadPath = "<RUTA_DE_DESCARGA_DEL_ARCHIVO>";  	// La ubicaci\'f3n donde se guardar\'e1 el archivo descargado\
\
ZeroBounce::getInstance()->scoringGetFile(\
    fileId,\
    localDownloadPath,\
    [](ZBGetFileResponse response) \{\
        cout << response.toString() << endl;\
    \},\
    [](ZBErrorResponse errorResponse) \{\
        cout << errorResponse.toString() << endl;\
    \}\
);\
```\
\
* ##### Comprobar el estado de un archivo cargado a trav\'e9s del m\'e9todo "scoringSendFile"\
```cpp\
std::\
\
string fileId = "<ID_DE_ARCHIVO>";                    // El ID de archivo devuelto al llamar a la API scoringSendfile\
\
ZeroBounce::getInstance()->scoringFileStatus(\
    fileId,\
    [](ZBFileStatusResponse response) \{\
        cout << response.toString() << endl;\
    \},\
    [](ZBErrorResponse errorResponse) \{\
        cout << errorResponse.toString() << endl;\
    \}\
);\
```\
\
* ##### Eliminar el archivo que se envi\'f3 utilizando la API scoring scoringSendfile. El archivo solo se puede eliminar si su estado es _`Completo`_\
```cpp\
std::string fileId = "<ID_DE_ARCHIVO>";                       // El ID de archivo devuelto al llamar a la API scoringSendfile\
\
ZeroBounce::getInstance()->scoringDeleteFile(\
    scoringFileId,\
    [](ZBDeleteFileResponse response) \{\
        cout << response.toString() << endl;\
    \},\
    [](ZBErrorResponse errorResponse) \{\
        cout << errorResponse.toString() << endl;\
    \}\
);\
```\
\
## DESARROLLO\
\
Se recomienda realizar el desarrollo en Linux.\
\
Despu\'e9s de revisar el repositorio, construye la biblioteca y luego ejecuta las pruebas.\
\
```bash\
./build/bin/ZeroBounceTests\
```\
\
Deber\'edas ver una salida como esta:\
\
```bash\
[==========] 26 pruebas desde 1 suite de pruebas se ejecutaron. (7 ms en total)\
[  PASSED  ] 26 pruebas.\
```}