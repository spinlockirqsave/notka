import React, { Component } from 'react';
import ReactDOM from 'react-dom';


function NotkaText() {
    return <h1>OK</h1>;
}

class FormLogin extends Component {
  constructor(props) {
    super(props);
    this.state = {login: ''};

    this.handleChange = this.handleChange.bind(this);
    this.handleSubmit = this.handleSubmit.bind(this);
  }

  handleChange(event) {
    this.setState({value: event.target.value});
  }

  handleSubmit(event) {
    alert('Login: ' + this.state.value);
    event.preventDefault();

    const element = <NotkaText />;
    ReactDOM.render(element, document.getElementById('root'));
  }

  render() {
    return (
      <form onSubmit={this.handleSubmit}>
        <label>
          <h1><br/>Login</h1>
          <input type="text" value={this.state.value} onChange={this.handleChange} />
        </label>
        <input type="submit" value="Go" />
      </form>
    );
  }
}

export default FormLogin;
