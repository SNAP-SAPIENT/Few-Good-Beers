import React, { Component } from 'react';
import {
  AppRegistry,
  StyleSheet,
  Text,
  View
} from 'react-native';

import { Router, Scene } from 'react-native-router-flux';

import Launch from './src/components/pages/Launch';
import PageTwo from './src/components/pages/PageTwo';

export default class App extends Component {
    render(){
      return (
        <Router>
          <Scene key="root">
            <Scene key="launch" component={Launch} title="Launch" initial={true} />
            <Scene key="pageTwo" component={PageTwo} title="PageTwo" />
          </Scene>
        </Router>
      )
    }
}


const styles = StyleSheet.create({
  container: {
    flex: 1,
    justifyContent: 'center',
    alignItems: 'center',
    backgroundColor: '#F5FCFF',
  },
  welcome: {
    fontSize: 20,
    textAlign: 'center',
    margin: 10,
  },
  instructions: {
    textAlign: 'center',
    color: '#333333',
    marginBottom: 5,
  },
});