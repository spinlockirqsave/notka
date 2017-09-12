/*
 * FormNotka.js
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

import React from 'react';
import ReactDOM from 'react-dom';
import './FormNotka.css';


var ws = require('./Websocket.js');

export class FormNotka extends React.Component {
  constructor(props) {
    super(props);

    this.handleChange = this.handleChange.bind(this);
    this.handleSubmit = this.handleSubmit.bind(this);
    this.handleBlur = this.handleBlur.bind(this);
  }

  static notka = "You don't have any notka yet. Simply write this here and click save to store it in the cloud. You can access your notka from wherever you want.";

  handleChange(event) {
    FormNotka.notka = event.target.value;
    ReactDOM.render(<FormNotka />, document.getElementById('Notka-text'));
  }

  handleSubmit(event) {
    //alert('Notka submitted: ' + FormNotka.notka);
    event.preventDefault();
    var ws = require('./Websocket.js');
    ws.tx_msg_save_req();
  }

  handleBlur(event) {
      //alert('Notka blur: ' + FormNotka.notka);
      event.preventDefault();

      setTimeout(() => function(){alert('Notka blur: timeeout');}, 1000);
      var relatedTarget = event.nativeEvent.relatedTarget;
      if (relatedTarget != null) {
            if (relatedTarget.defaultValue === "Save") {
                // Save was clicked while on focus
                setTimeout(() => {this.handleSubmit(event)}, 1000);
            }
      }
  }

  static setNotka(text) {
        FormNotka.notka = text;
  }

  render() {
    return (
      <form onSubmit={this.handleSubmit}>
        <textarea id="NotkaTextArea" value={FormNotka.notka} onChange={this.handleChange} onBlur={this.handleBlur} />
        <input type="submit" value="Save" />
      </form>
    );
  }
}

export default FormNotka;
