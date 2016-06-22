import React, { Component } from 'react';
import {
  AppRegistry,
  StyleSheet,
  Text,
  View
} from 'react-native';

import { Router, Scene } from 'react-native-router-flux';

import Dashboard from './src/components/pages/Dashboard';
import MyBeer from './src/components/pages/MyBeer';
import MyBars from './src/components/pages/MyBars';
import Profile from './src/components/pages/Profile';

export default class App extends Component {
    render(){
      return (
        <Router>
          <Scene key="root">
            <Scene key="dashboard" component={Dashboard} title="Dashboard" initial={true} />
            <Scene key="mybeer" component={MyBeer} title="My Beers" />
            <Scene key="mybars" component={MyBars} title="My Bars" />
            <Scene key="profile" component={Profile} title="My Profile" />
          </Scene>
        </Router>
      )
    }
}
