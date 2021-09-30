const EventEmitter = require("events");

let id = 0;

class Database extends EventEmitter
{
    data = [
        {id: id++, name: "Oleg", bDay: new Date()},
        {id: id++, name: "Joker", bDay: new Date()},
        {id: id++, name: "Kak?", bDay: new Date()},
    ];

    constructor(props) {
        super(props);
    }

    async select() {
        return await new Promise(resolve => resolve(this.data));
    }

    async insert(newEntity) {
        return await new Promise(resolve => {
            const entityToAdd = {
                ...newEntity,
                id: id++,
                bDay: new Date(newEntity.bDay),
            };
            this.data.push(entityToAdd);
            resolve(entityToAdd);
        })
    }

    async update(newEntity) {
        return await new Promise((resolve, reject) => {
            const updatedEntity = {
                ...newEntity,
                id: Number(newEntity.id),
                bDay: new Date(newEntity.bDay)
            }
            const index = this.data.findIndex(entity => entity.id === updatedEntity.id);
            if (index === -1) {
                reject("Specified id is invalid");
            }
            this.data[index] = updatedEntity;
            resolve();
        })
    }

    async delete(id) {
        return await new Promise((resolve, reject) => {
            const index = this.data.findIndex(entity => entity.id === id);
            if (index === -1) {
                reject("Specified id is invalid");
            }
            const returnData = this.data[index];
            this.data.splice(index, 1);
            resolve(returnData);
        })
    }

    commit() {
        console.log("I'm committing now...");
    }
}

module.exports = Database;