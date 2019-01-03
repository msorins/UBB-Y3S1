const koa = require('koa');
const app = new koa();
const server = require('http').createServer(app.callback());
const WebSocket = require('ws');
const wss = new WebSocket.Server({server});
const Router = require('koa-router');
const cors = require('koa-cors');
const bodyParser = require('koa-bodyparser');
const convert = require('koa-convert');

app.use(bodyParser());
app.use(convert(cors()));
app.use(async(ctx, next) => {
    const start = new Date();
    await next();
    const ms = new Date() - start;
    console.log(`${ctx.method} ${ctx.url} ${ctx.response.status} - ${ms}ms`);
});

const getRandomInt = (min, max) => {
    min = Math.ceil(min);
    max = Math.floor(max);
    return Math.floor(Math.random() * (max - min)) + min;
};

const rvNames = ['Yellow', 'Blue', 'Small', 'Big', 'Lincoln'];
const rvModels = ['Serenity', 'Orion', 'Whisper'];
const statusTypes = ['free', 'busy'];
const rvs = [];
for (let i = 0; i < 10; i++) {
    rvs.push({
        id: i + 1,
        name: rvNames[getRandomInt(0, rvNames.length)],
        model: rvModels[getRandomInt(0, rvModels.length )],
        status: statusTypes[getRandomInt(0, statusTypes.length )],
        seats: getRandomInt(1, 10),
        rides: getRandomInt(1, 1000)
    });
}

const router = new Router();
router.get('/rvs', ctx => {
    ctx.response.body = rvs.filter(rv => rv.status == statusTypes[0]);
    ctx.response.status = 200;
});

router.get('/busy', ctx => {
    ctx.response.body = rvs.filter(rv => rv.status == statusTypes[1]);
    ctx.response.status = 200;
});


router.post('/change', ctx => {
    // console.log("ctx: " + JSON.stringify(ctx));
    const headers = ctx.request.body;
    console.log("body: " + JSON.stringify(headers));
    const id = headers.id;
    const name = headers.name;
    const status = headers.status;
    const seats = headers.seats;
    if (typeof id !== 'undefined' && typeof name !== 'undefined' &&
        typeof status !== 'undefined' && typeof seats !== 'undefined') {
        const index = rvs.findIndex(rv => rv.id == id);
        if (index === -1) {
            console.log("rv not available!");
            ctx.response.body = {text: 'rv not available!'};
            ctx.response.status = 404;
        } else {
            let rv = rvs[index];
            rv.name = name;
            rv.status = status;
            rv.seats = seats;
            ctx.response.body = rv;
            ctx.response.status = 200;
        }
    } else {
        console.log("Missing or invalid: id or name or status or seats!");
        ctx.response.body = {text: 'Missing or invalid: id or name or status or seats!'};
        ctx.response.status = 404;
    }
});


router.post('/rides', ctx => {
    // console.log("ctx: " + JSON.stringify(ctx));
    const headers = ctx.request.body;
    console.log("body: " + JSON.stringify(headers));
    const id = headers.id;
    const rides = headers.rides;
    if (typeof id !== 'undefined' && typeof rides !== 'undefined') {
        const index = rvs.findIndex(rv => rv.id == id);
        if (index === -1) {
            console.log("rv not available!");
            ctx.response.body = {text: 'rv not available!'};
            ctx.response.status = 404;
        } else {
            let rv = rvs[index];
            rv.rides = rv.rides + rides;
            ctx.response.body = rv;
            ctx.response.status = 200;
        }
    } else {
        console.log("Missing or invalid: id or rides!");
        ctx.response.body = {text: 'Missing or invalid: id or rides!'};
        ctx.response.status = 404;
    }
});

const broadcast = (data) =>
    wss.clients.forEach((client) => {
        if (client.readyState === WebSocket.OPEN) {
            client.send(JSON.stringify(data));
        }
    });

router.post('/new', ctx => {
    // console.log("ctx: " + JSON.stringify(ctx));
    const headers = ctx.request.body;
    console.log("body: " + JSON.stringify(headers));
    const name = headers.name;
    const model = headers.model;
    const seats = headers.seats;
    if (typeof name !== 'undefined' && typeof model !== 'undefined' &&
        typeof seats !== 'undefined') {
        const index = rvs.findIndex(rv => rv.name == name &&
        rv.model == model && rv.seats == seats);
        if (index !== -1) {
            console.log("rv already exists!");
            ctx.response.body = {text: 'rv already exists!'};
            ctx.response.status = 404;
        } else {
            let maxId = Math.max.apply(Math, rvs.map(function (rv) {
                    return rv.id;
                })) + 1;
            let rv = {
                id: maxId,
                name,
                model,
                status: statusTypes[0],
                seats,
                km: 0
            };
            rvs.push(rv);
            broadcast(rv);
            ctx.response.body = rv;
            ctx.response.status = 200;
        }
    } else {
        console.log("Missing or invalid: name or model or rides!");
        ctx.response.body = {text: 'Missing or invalid: name or model or rides!"'};
        ctx.response.status = 404;
    }
});

router.del('/rv/:id', ctx => {
    // console.log("ctx: " + JSON.stringify(ctx));
    const headers = ctx.params;
    console.log("body: " + JSON.stringify(headers));
    const id = headers.id;
    if (typeof id !== 'undefined') {
        const index = rvs.findIndex(rv => rv.id == id);
        if (index === -1) {
            console.log("No rv with id: " + id);
            ctx.response.body = {text: 'Invalid rv id'};
            ctx.response.status = 404;
        } else {
            let rv = rvs[index];
            rvs.splice(index, 1);
            ctx.response.body = rv;
            ctx.response.status = 200;
        }
    } else {
        ctx.response.body = {text: 'Id missing or invalid'};
        ctx.response.status = 404;
    }
});

app.use(router.routes());
app.use(router.allowedMethods());

server.listen(4023);
