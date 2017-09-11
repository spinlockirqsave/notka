import React, { Component } from 'react';
//import ReactDOM from 'react-dom';


//function NotkaText() {
//    return <h1>OK</h1>;
//}

class FormLogin extends Component {
  constructor(props) {
    super(props);
    this.state = {login: '', pass: ''};

    this.handleChange = this.handleChange.bind(this);
    this.handleSubmit = this.handleSubmit.bind(this);
  }

  handleChange(event) {
        var newState = {};
        newState[event.target.name] = event.target.value;
        this.setState(newState);
  }

  handleSubmit(event) {
    //alert('Login: ' + this.state.login + ' Pass: ' + this.state.pass);
    event.preventDefault();

    var ws = require('./Websocket.js');
    ws.tx_msg_login(this.state.login, this.state.pass);
  }

  render() {
    return (
      <form onSubmit={this.handleSubmit}>
        <label>
          <h1><br/>Login</h1>
          <input type="text" name="login" onChange={this.handleChange} />
        </label>
        <label>
          <h1><br/>Password</h1>
          <input type="text" name="pass" onChange={this.handleChange} />
        </label>
        <input type="submit" value="Go" />
      </form>
    );
  }
}

export default FormLogin;
