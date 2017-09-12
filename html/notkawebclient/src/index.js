import React from 'react';
import ReactDOM from 'react-dom';
import './index.css';

import HelloGreeting from './HelloGreeting';
import FormLogin from './FormLogin';
import registerServiceWorker from './registerServiceWorker';


function Tick() {
    ReactDOM.render(<HelloGreeting/>, document.getElementById('HelloMessage'));
}

setInterval(Tick, 15);

ReactDOM.render(<FormLogin />, document.getElementById('root'));

registerServiceWorker();

var ws = require('./Websocket.js');
ws.initWebSocket();
