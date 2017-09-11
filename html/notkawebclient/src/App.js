import React, { Component } from 'react';
import logo from './logo.svg';
import './App.css';


class App extends Component {

  render() {
    return (
      <div className="App">
        <div className="App-header">
          <img src={logo} className="App-logo" alt="logo" />
          <h2>Notka</h2>
        </div>
        <p className="App-intro">
          Online clipboard.
          Store your notes online and use them from wherever you want.
        </p>
          <div>
          <input type="text" id="inputLogin" value="login" />
          </div>
      </div>
    );
  }
}

export default App;
