const sjcl = require("sjcl-complete");
var keys = sjcl.ecc.elGamal.generateKeys(sjcl.ecc.curves.k256);; //choose a stronger/weaker curve

var pubkem = keys.pub.kem(); //KEM is Key Encapsulation Mechanism
var pubkey = pubkem.key;
var seckey = keys.sec.unkem(pubkem.tag); //tag is used to derive the secret (private) key

var plain = "hello world!";
var cipher = sjcl.encrypt(pubkey, plain); //defaults to AES
var result = sjcl.decrypt(seckey, cipher);

console.log(plain === result); //true
