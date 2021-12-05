# C++ to NodeJs ABI Library

**Date:** 04/12/2021  
**Author:** stefano.gagliardi@sitisrl.it  
**Reference:** [Medium article](https://medium.com/jspoint/a-simple-guide-to-load-c-c-code-into-node-js-javascript-applications-3fcccf54fd32)

Target of this small project is compile c++ functions (small library) cross
compiled for Nodejs. C++ compiled in .node file is binary that expose ABI.

**ABI**
An Application Binary Interface is a way for a program to communicate with other compiled programs. This is a lot similar to an API but in the case of ABI, we are interacting with a binary file and accessing memory addresses to look for symbols like numbers, objects, classes, functions, etc.

There are two main reason to want this:

1. Write very complex alghorithm / function like encryption or mathematical funciton or even performance memory problem.
2. Create shippable software with a high level of security about code obfuscation. Some core function are even the value in the software.
   For create shippable DEMO is requried a "copy protection policty" like c++ compiled code in Machine Language.
   NB: obviously ABI work only with **NodeJS** not with javascript in browser.

![Code Overview](https://github.com/StefanoGagliardi/022_cxx_to_node_abi/blob/main/doc/cxx_dump.PNG)

## Proof of work

How do they really work behind the scene? To understand this, we need to look at the Node.js architecture

In Node.js, when we compile the native C or C++ code into a DLL, it should be exported to a file .node extension. We can use require function, as usual, to import this file inside a JavaScript program.

Node.js uses Google’s open-source V8 engine as the default JavaScript engine which executes the JavaScript code. The V8 engine is written in C++ language.

But, what ABI has to do with Native Addons? The compiled DLL file (Native Addon) when loaded into Node.js runtime actually communicates with Node.js using an ABI provided by the Node.js to access/register values and to perform tasks through APIs exposed by Node.js and its libraries.

## Integration

1. C++ code
2. C++ to Native Module
3. ABI compiled code
4. Node Js SW for test
5. Unit test

### 1. Integration - c++ code

Typically, when we compile a C or C++ program into a DLL, we use some meta-data files called header files, which ends with .h extension.
These header files are provided by Node.js and its libraries and they contain the declarations of the APIs exposed by the Node.js and its libraries.

Just include Header file and use his declartion, for example [`#include <v8.h>`](https://github.com/nodejs/node/blob/master/deps/v8/include/v8.h) with Google JS Engine API.

**Write lib/shared.h**
Write declaration for exported ABI functions

```c++
#include <string>
std::string helloUser(std::string name);
int add(int, int);
```

**Write lib/shared.cpp**
Function implementation previously declarated

```c++
#include <iostream>
#include "shared.h"
#include <string>

// function definition
int add(int a, int b) {
    return a + b;
}

std::string helloUser( std::string name ) {
  return "Hello " + name + "!";
}
```

**Write lib/main.cpp**
See after point 8 for integration of shared.cpp in main.cpp for ABI communication

### 2. Integration - C++ to Native module

**Prerequisites**
Prerequisite for build

1. We need to have C and C++ compilers installed on our system. (g++ on win)

2. Install **node-gyp** globally run `npm i -g node-gyp` (NOT WORK)
   **NB:** See relative section about installation of **node-gyp**
   **NB:** this provide a boilerplate for c++ DLL build with .node extension.
   **NB:** See section in this file About this package for all details and explaination.

3. Initialize node project `npm init -y` (-y force all true setting)
4. Install `npm install -S node-addon-api` that provide `napi.h`
5. Update `shared.h` e `shared.cpp` with two base funcion>
   5.1 String concatenation with one param
   5.2 Sum between two int
   NB: See file content for reference
6. Let’s create another file with the name index.cpp which contains the logic of implementing Node.js API using napi.h header file.
7. Set up builder `binding.gyp` is a Python file that contains JSON like data structure to tell node-gyp about the configuration of our Native Module

```json
{
  "targets": [
    {
      "target_name": "library_name",
      "cflags!": ["-fno-exceptions"],
      "cflags_cc!": ["-fno-exceptions"],
      "sources": ["./src/shared.cpp", "./src/main.cpp"],
      "include_dirs": ["<!@(node -p \"require('node-addon-api').include\")"],
      "defines": ["NAPI_DISABLE_CPP_EXCEPTIONS"]
    }
  ]
}
```

**Some explaination:**
**NB:** `target_name` DLL library name  
**NB:** `cflags!` and `cflags_cc!` suppres some exception and warnings  
**NB:** `sources` list of source to build  
**NB:** Since we are going to need napi.h file soon, we need to provide its location inside include_dirs. The <!@(node -p \"require('node-addon-api').include\") command is executed by node-gyp to find the correct directory path of node-addon-api module while building the Native Addons.  
**NB:** The index.cpp file is going to contain some logic that will communicate with Node.js through N-API (using node-addon-api). Hence this file is also going to import napi.h header file.

**Write lib/main.cpp**
See content with comment for explaination.

8. Write lib/main.cpp:
   See content with comment for explaination.
   Use Napi Class (node-addon-api) for export function declared in `shared.cpp`

9. **BUILD:** for run build and generate DDL with `.node` extension (Run as Administrator and move in folder): `npm install`.
   This process show log about node-gyp buid and build  our code.
   **NB:** Eventually error must read very well, code syntax error is diffult to understand beacuse debug work bad and all log point
           to same file and it's config file.

### 3. Integration - ABI File (DLL)
Yeah! if we are here we got our dll for Nodejs.
1. I suggest to install this extension for vscode: `hexdump for VScode`
2. Open file `./build/release/Simurisk.node`
   2.1 Righ click on Tab, and Click: "Show Hex content" (Last item)

### 4. Integration - NodJs Use
Finally we can use our functions

Create `index.js` to with this simple follow content:
```js
const simuriskModule = require('./build/release/Simurisk.node');
console.log("simuriskModule: ", simuriskModule);
console.log("simuriskModule.greetHello(): ", simuriskModule.greetHello(" SPOLA "));
console.log("simuriskModule.sum(): ", simuriskModule.sum(10, 15));
```

Run file with this command: `node index.js`;

### 5. Integration - Unit test

@todo

### NODE-GYP - Node.js native addon build tool

[Github repo](https://github.com/nodejs/node-gyp)

#### 1. Install on Windows 10

1. Check if Python is installed (min 2.7): `python --version`
   1.1 Check Path `C:\python27\python.exe`, if miss downlaod and install [Python27](https://www.python.org/ftp/python/2.7.3/python-2.7.3.amd64.msi)
2. Run CMD as Administrator and run this two commands:
  2.1 `npm install -g node-gyp`
  2.2 `setx PYTHON C:\Python27\python.exe /m`
3. [Downlaod and Install](https://go.microsoft.com/fwlink/?LinkId=532606&amp;clcid=0x409) Microsoft Visual Studio 2015 Community

#### 2. Package extras details

The boilerplate is generated from a binding.gyp file by the node-gyp which contains some instructions about the name of the Native Addon (the DLL file) and what files should be considered in the compilation.

To use node-gyp, we need to have Python installed in our system since node-gyp is based on GYP tool which is written in Python. GYP is an acronym for Generate Your Project, hence node-gyp is not a compiler but a facilitator.

#### Napi.h - Resouddrce



[Stackoverflow - Napi::Array](https://stackoverflow.com/questions/59788339/calling-a-c-function-from-node-js-using-n-api)

[Napi.h - Napi::Number class](https://github.com/nodejs/node-addon-api/blob/main/napi.h#L466)
