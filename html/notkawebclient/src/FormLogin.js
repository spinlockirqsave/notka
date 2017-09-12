/*
 * FormLogin.js
 *
 * Copyright(C) 2017, Piotr Gregor <piotrgregor@rsyncme.org>
 *
 * Notka Online Clipboard
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

import React, { Component } from 'react';

var ws = require('./Websocket.js');

function LoginStatus(text) {
        return <h1>{text}</h1>;
}

export class FormLogin extends Component {
  constructor(props) {
    super(props);
    this.state = {login: '', pass: '' };

    this.handleChange = this.handleChange.bind(this);
    this.handleSubmit = this.handleSubmit.bind(this);
  }

  static loginState = ws.WsState.LOGIN;

  handleChange(event) {
        var newState = {};
        newState[event.target.name] = event.target.value;
        this.setState(newState);
  }

  handleSubmit(event) {
    //alert('Login: ' + this.state.login + ' Pass: ' + this.state.pass);
    event.preventDefault();
    ws.tx_msg_login(this.state.login, this.state.pass);
  }

  static getLoginState() {
        return FormLogin.loginState;
  }

  static updateLoginState(status) {
        FormLogin.loginState = status;
  }

  render() {
        let label = null;
        if (FormLogin.loginState === ws.WsState.LOGIN ) {
            label = <label>
            <h1><br/>Login</h1>
            <input type="text" name="login" onChange={this.handleChange} />
          </label>;
        } else if (FormLogin.loginState === ws.WsState.LOGIN_PASS) {
                    label = <label>
                      <h1><br/>Password</h1>
                      <input type="text" name="pass" onChange={this.handleChange} />
                    </label>;
        } else if (FormLogin.loginState === ws.WsState.LOGGED_IN) {
                    return LoginStatus("OK");
        } else {
                    return LoginStatus("Logging failed");
        }

    return (
      <form onSubmit={this.handleSubmit}>
        {label}
        <input type="submit" value="Go" />
      </form>
    );
  }
}

export default FormLogin;
