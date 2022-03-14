const { generateSequence, foundPrimeNumbers, removeQuotients } = require("./primeNumbers");
const { calcGcd, calcGcdMany } = require("./gcd");

console.log(`result: ${calcGcdMany([78, 61572, 721])}`);

const n = 499;
const m = 531;
foundPrimeNumbers(n, m);

console.log("Count: " + foundPrimeNumbers(2, n));
const check = (n) / (Math.log(n));
console.log(`n / ln(n) = ${check}`);

