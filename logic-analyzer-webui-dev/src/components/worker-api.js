import Worker from 'worker-loader!./worker';

const worker = new Worker();

worker.addEventListener('message', (e) => {
    // console.log('we got back', e);
})

export function sendMessage(msg){
    worker.postMessage(msg);
}