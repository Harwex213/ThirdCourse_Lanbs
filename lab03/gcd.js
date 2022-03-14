const calcGcd = (m, n) => {
    const numbers = [m, n];
    numbers.sort((a, b) => b - a);
    let [a, b] = numbers;
    let c = 0;

    do {
        c = a % b;
        console.log(`${a} = ${b} * ${a / b} + ${c}`);

        a = b;
        b = c;
    } while (c !== 0);

    return a;
}
// a = b * c + q

const calcGcdMany = (numbers) => {
    if (numbers.length < 2) {
        return -1;
    }

    const [first, second] = numbers;
    numbers.shift();
    numbers.shift();

    let result = calcGcd(first, second);
    while (numbers.length) {
        result = calcGcd(result, numbers[0]);
        numbers.shift();
    }

    return result;
}

module.exports = {
    calcGcd,
    calcGcdMany
}