const { createClient } = require("redis");
const MAX_NUMBER = 10000;

(async () => {
    const client = createClient();
    client.on("error", (err) => console.log("Redis Client Error", err));
    await client.connect();

    // task 2.
    console.time("set");
    for (let i = 0; i < MAX_NUMBER; i++) {
        await client.set(i, `set${i}`);
    }
    console.timeEnd("set");

    console.time("get");
    for (let i = 0; i < MAX_NUMBER; i++) {
        await client.get(i);
    }
    console.timeEnd("get");

    console.time("del");
    for (let i = 0; i < MAX_NUMBER; i++) {
        await client.del(i);
    }
    console.timeEnd("del");

    // task 3.
    await client.set("incr", 0);
    console.time("incr");
    for (let i = 0; i < MAX_NUMBER; i++) {
        await client.incr("incr");
    }
    console.timeEnd("incr");

    console.time("decr");
    for (let i = 0; i < MAX_NUMBER; i++) {
        await client.decr("incr");
    }
    console.timeEnd("decr");
    await client.del("incr");

    // task 4.
    console.time("hSet");
    for (let i = 0; i < MAX_NUMBER; i++) {
        await client.hSet(0, i, `{id:${i},val:"val-${i}"}`);
    }
    console.timeEnd("hSet");

    console.time("hGet");
    for (let i = 0; i < MAX_NUMBER; i++) {
        await client.hGet(0, i);
    }
    console.timeEnd("hGet");

    // task 5.
    const subscriber = client.duplicate();
    await subscriber.connect();
    await subscriber.subscribe("channel", (message) => {
        console.log(message);
    });

    await client.publish("channel", "message");
})();