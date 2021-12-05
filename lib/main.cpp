#include <napi.h> // Include header file of node-addon-api
#include <string>
#include <iostream>
#include "shared.h"

// Native c++ function that is assigned to `greetHello` function property on node "export object"
Napi::String greetHello(const Napi::CallbackInfo &info)
{
  Napi::Env env = info.Env();

  // Call "helloUser" function from 'shared.cpp' file
  std::string user = (std::string)info[0].ToString();
  std::string result = helloUser(user);

  // Return new "Napi::string" value
  return Napi::String::New(env, result);
};

// Native c++ function that is assigned to `greetHello` function property on node "export object"
Napi::Number sum(const Napi::CallbackInfo &info)
{
  Napi::Env env = info.Env();

  // Call "helloUser" function from 'shared.cpp' file
  int num1 = (int)info[0].ToNumber();
  int num2 = (int)info[1].ToNumber();
  int result = add(num1, num2);

  // Return new "Napi::string" value
  return Napi::Number::New(env, result);
};

// Callback method when module is registered with node.js
Napi::Object Init(Napi::Env env, Napi::Object exports)
{

  // Set a 'key' exports object
  exports.Set(
      Napi::String::New(env, "greetHello"), // property name => "greetHello"
      Napi::Function::New(env, greetHello)  // property value => "greetHello" function
  );

  exports.Set(
      Napi::String::New(env, "sum"), // property name => "sum"
      Napi::Function::New(env, sum)  // property value => "sum" function
  );

  // Return 'exports' object (always)
  return exports;
};

// Register "Simurisk" module witch calls "Init" method
NODE_API_MODULE(Simurisk, Init)