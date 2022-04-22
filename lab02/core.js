const germanStr = "A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z, Ä, Ö, Ü, ß";
const serbianStr = "абвгдђежзијклљмнњопрстћуфхцчџш";
const russianStr = "абвгдеёжзийклмнопрстуфхцчшщъыьэюя";
const englishStr = "abcdefghijklmnopqrstuvwxyz";
const binaryStr = "01";

const alphabetStrToMap = (alphabet) => {
    const simplified = simplifyText(alphabet);
    const alphabetMap = new Map();
    for (let i = 0; i < simplified.length; i++) {
        alphabetMap.set(simplified[i], 0);
    }
    return alphabetMap;
}

const simplifyText = (text) => {
    let newText = text.toString();
    const uselessChars = " ,.!?:-;()[]'\"";
    for (let i = 0; i < uselessChars.length; i++) {
        newText = newText.replaceAll(uselessChars[i], "");
    }

    return newText.toLowerCase();
}

const textToBinaryText = (text) => {
    const buffer = Buffer.from(text, "ascii");
    return buffer.reduce((previousValue, currentValue) =>
        previousValue + currentValue.toString(2).padStart(8, "0"), "");
}

const calcEntropy = (text, alphabet) => {
    let alphabeticMap;
    switch (alphabet) {
        case "binary":
            alphabeticMap = alphabetStrToMap(binaryStr);
            break;
        case "german":
            alphabeticMap = alphabetStrToMap(germanStr);
            break;
        case "serbian":
            alphabeticMap = alphabetStrToMap(serbianStr);
            break;
        case "russian":
            alphabeticMap = alphabetStrToMap(russianStr);
            break;
        case "english":
            alphabeticMap = alphabetStrToMap(englishStr);
            break;
        default:
            return null;
    }

    // fill counts of each character of the text
    let ignoreCharsCount = 0;
    for (let i = 0; i < text.length; i++) {
        const number = alphabeticMap.get(text[i]);
        if (number !== undefined) {
            alphabeticMap.set(text[i], number + 1);
            continue;
        }
        ignoreCharsCount++;
    }

    // calc percentage
    const charsPercentage = Array.from(alphabeticMap, ([_, value]) => value);
    for (let i = 0; i < charsPercentage.length; i++) {
        charsPercentage[i] = charsPercentage[i] / (text.length - ignoreCharsCount);
    }

    console.log(charsPercentage);
    // calc entropy
    let entropy = 0;
    for (let i = 0; i < charsPercentage.length; i++) {
        if (charsPercentage[i] !== 0) {
            entropy += charsPercentage[i] * Math.log2(charsPercentage[i]);
        }
    }

    return {
        entropy: -entropy,
        charsPercentage: charsPercentage,
        charsAlphabet: Array.from(alphabeticMap, ([key, _]) => key),
        charsCount: Array.from(alphabeticMap, ([key, value]) => [key, value]),
        textLength: text.length - ignoreCharsCount,
    };
}

const handleEntropy = (request, response) => {
    const body = request.body;
    let text = simplifyText(body.text);

    if (body.alphabet === "binary") {
        text = textToBinaryText(text);
    }

    response.status(200).json({
        ...calcEntropy(text, body.alphabet)
    });
};

const handleInformationCount = (request, response) => {
    const body = request.body;
    const text = simplifyText(body.text);
    const binaryText = textToBinaryText(text);

    const serbianEntropy = calcEntropy(text, "serbian");
    const binaryEntropy = calcEntropy(binaryText, "binary");
    const entropyReal01 = (1 - (- 0.1 * Math.log2(0.1) - 0.9 * Math.log2(0.9)));
    const entropyReal05 = (1 - (- 0.5 * Math.log2(0.5) - (0.5) * Math.log2(0.5)));
    const entropyReal10 = (1 - (- 1 * Math.log2(1)));
    response.status(200).json({
        textLength: text.length,
        binaryTextLength: binaryText.length,
        serbianInfoQuantity:serbianEntropy.entropy * serbianEntropy.textLength,
        binaryInfoQuantity: binaryEntropy.entropy * binaryEntropy.textLength,
        binaryInfoQuantityReal01: entropyReal01 * binaryEntropy.textLength,
        binaryInfoQuantityReal05: entropyReal05 * binaryEntropy.textLength,
        binaryInfoQuantityReal10: entropyReal10 * binaryEntropy.textLength,
    });
};

module.exports = {
    handleEntropy,
    handleInformationCount,
}