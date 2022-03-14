const generateSequence = (start, to) => Array.from({ length: to }, (_, i) => i + start);

const removeQuotients = (sequence, divider, log = true) => {
    if (divider === 1) {
        return sequence;
    }

    const clearedSequence = [];
    for (let i = 0; i < sequence.length; i++) {
        if (divider === sequence[i] || sequence[i] % divider !== 0) {
            clearedSequence.push(sequence[i]);
        }
    }

    if (log) {
        console.log("Cleared sequence by divider " + divider, clearedSequence);
    }
    return clearedSequence;
}

const foundPrimeNumbers = (from, to) => {
    const maxDivider = Math.sqrt(to);
    let dividers = generateSequence(1, maxDivider);
    let primeNumbers = generateSequence(from, to - from + 1);
    console.log("Started sequence: ", primeNumbers);

    for (let i = 0; i < dividers.length; i++) {
        dividers = removeQuotients(dividers, dividers[i], true);
    }

    for (let i = 0; i < dividers.length; i++) {
        primeNumbers = removeQuotients(primeNumbers, dividers[i], true);
    }

    console.log("Found prime Numbers: ", primeNumbers);
    return primeNumbers.length;
}

module.exports = {
    generateSequence,
    removeQuotients,
    foundPrimeNumbers
}
