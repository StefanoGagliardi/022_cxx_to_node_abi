const simuriskModule = require('./build/release/Simurisk.node');
console.log("simuriskModule: ", simuriskModule);
console.log("simuriskModule.greetHello(): ", simuriskModule.greetHello(" SPOLA "));
console.log("simuriskModule.sum(): ", simuriskModule.sum(10, 15));

// Reponse Terminal

// C:\Users\User\Desktop\022_cxx_to_node_abi>node index.js
// simuriskModule:  { greetHello: [Function (anonymous)], sum: [Function (anonymous)] }
// simuriskModule.greetHello():  Hello  SPOLA !
// simuriskModule.sum():  25