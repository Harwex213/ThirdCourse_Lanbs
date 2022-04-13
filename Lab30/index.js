const TelegramBot = require("node-telegram-bot-api");
const token = require("./config.json").token;

const bot = new TelegramBot(token, {
    polling: true
});

bot.onText(/\/echo (.+)/, async (msg, match) => {
    const chatId = msg.chat.id;
    const message = match[1];
    await bot.sendMessage(chatId, message);
})