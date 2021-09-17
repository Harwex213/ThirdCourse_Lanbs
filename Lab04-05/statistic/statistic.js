const dateFormat = require("dateformat");

let statisticGatheringTimer = null;
const defaultInfo = {
    startTime: "",
    endTime: "",
    queriesAmount: 0,
    commitsAmount: 0,
}

let info = {
    ...defaultInfo
}

const isGathering = () => (statisticGatheringTimer !== null);

const getCurrentStatistic = () => (info);

const startGathering = (duration) => {
    console.log("start gathering statistic...");

    info = {
        ...defaultInfo,
        startTime: dateFormat(new Date(), "dd.mm.yyyy, HH:MM")
    };

    statisticGatheringTimer = setTimeout(() => stopGathering(), duration);
    statisticGatheringTimer.unref();
}

const stopGathering = () => {
    if (statisticGatheringTimer) {
        clearTimeout(statisticGatheringTimer);
        statisticGatheringTimer = null;
        info.endTime = dateFormat(new Date(), "dd.mm.yyyy, HH:MM");

        console.log("finish gathering statistic...", info);
    }
}

const onQuery = () => info.queriesAmount++;
const onCommit = () => info.commitsAmount++;

module.exports = {
    isGathering,
    getCurrentStatistic,
    startGathering,
    stopGathering,
    onQuery,
    onCommit,
};