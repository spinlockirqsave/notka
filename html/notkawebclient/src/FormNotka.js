import React from 'react';
import './FormNotka.css';


export class FormNotka extends React.Component {
  constructor(props) {
    super(props);

    this.handleChange = this.handleChange.bind(this);
    this.handleSubmit = this.handleSubmit.bind(this);
  }

  static notka = "You don't have any notka yet. Simply write this here and click save to store it in the cloud. You can access your notka from wherever you want.";

  handleChange(event) {
    FormNotka.notka = event.target.value;
  }

  handleSubmit(event) {
    alert('Notka submitted: ' + FormNotka.notka);
    event.preventDefault();
  }

  static setNotka(text) {
        FormNotka.notka = text;
  }

  render() {
    return (
      <form onSubmit={this.handleSubmit}>
        <textarea value={FormNotka.notka} onChange={this.handleChange} />
        <input type="submit" value="Save" />
      </form>
    );
  }
}

export default FormNotka;
